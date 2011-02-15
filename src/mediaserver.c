/*--------------------------------------------------------------------------------------------------
  This is the main prgram for editing all data at Ebooks.coop.  It usese Command Serializer
  from comserial.sourceforge.net inorder to communicate with multiple users at once.  PHP scripts
  are then used to glue into Apache.
--------------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cgiutil.h"
#include "sha256.h"
#include "medatabase.h"
#include "comserver.h"

#define MAX_ARGS 10
#define NOUNCE_LENGTH 32
#define NEW_USER_DONATION 889
#define LOG_FILE "media.log"
#define OLD_LOG_FILE "media.log.old"
#define DATABASE_FILE "media.db"

meRoot meTheRoot;
static char *meLineBuffer;
static uint32 meLineSize;
static meSession meCurrentSession;
static meUser meCurrentUser;
static FILE *meLogFile;
static bool meProcessingLogFile;
static uint64 meCurrentTime;

/*--------------------------------------------------------------------------------------------------
  Escape a string so that it can be passed inside single quotes in shell commands.
--------------------------------------------------------------------------------------------------*/
static char *escapeString(
    char *message)
{
    char *newMessage = utMakeString((strlen(message) << 1) + 1);
    char *p = newMessage;
    char c;

    utDo {
        c = *message++;
    } utWhile(c != '\0') {
        if(!isalnum(c)) {
            *p++ = '\\';
        }
        *p++ = c;
    } utRepeat;
    *p = '\0';
    return newMessage;
}

/*--------------------------------------------------------------------------------------------------
  Send mail to a user.  Note that users should not be able to access this, as we don't validate
  the formats of parameters.  Single quotes are not allowed in the message.
--------------------------------------------------------------------------------------------------*/
static void sendMail(
    char *email,
    char *subject,
    char *message)
{
    char *command = utSprintf("echo %s | mail -a 'From: admin@Ebooks.coop' -s %s %s",
        escapeString(message), escapeString(subject), email);

    system(command);
}

/*--------------------------------------------------------------------------------------------------
  Send mail to a user.  Note that users should not be able to access this, as we don't validate
  the formats of parameters.  Single quotes are not allowed in the message.
--------------------------------------------------------------------------------------------------*/
static void mailPost(
    meUser user,
    mePost post)
{
    meThread thread = mePostGetThread(post);

    if(meProcessingLogFile) {
        return;
    }
    sendMail(utSymGetName(meUserGetEmail(user)), meThreadGetSubject(thread),
        mePostGetMessage(post));
}

/*--------------------------------------------------------------------------------------------------
  Send an announcement to everyone.
--------------------------------------------------------------------------------------------------*/
static void sendAnnouncement(
    meAnnouncement announcement)
{
    meUser sender = meAnnouncementGetUser(announcement);
    meUser user;
    char *subject = meAnnouncementGetSubject(announcement);
    char *message = meAnnouncementGetMessage(announcement);

    meForeachRootUser(meTheRoot, user) {
        sendMail(utSymGetName(meUserGetEmail(user)), subject, message);
    } meEndRootUser;
}

/*--------------------------------------------------------------------------------------------------
  Destroy the session.
--------------------------------------------------------------------------------------------------*/
static void endSession(
    char *sessionID)
{
    meSession session = meRootFindSession(meTheRoot, utSymCreate(sessionID));

    if(session != meSessionNull) {
        meSessionDestroy(session);
        if(session == meCurrentSession) {
            meCurrentSession = meSessionNull;
        }
    }
}

/*--------------------------------------------------------------------------------------------------
  Read a line of text from stdin.  Return false on EOF.
--------------------------------------------------------------------------------------------------*/
static bool readLine(
    FILE *file)
{
    uint32 linePosition = 0;
    int c;

    utDo {
        if(meProcessingLogFile) {
            c = getc(file);
        } else {
            c = coGetc();
        }
        if(c == EOF) {
            return false;
        }
    } utWhile(c != '\n') {
        if(linePosition + 1 == meLineSize) {
            meLineSize <<= 1;
            utResizeArray(meLineBuffer, meLineSize);
        }
        meLineBuffer[linePosition++] = c;
    } utRepeat;
    meLineBuffer[linePosition] = '\0';
    return true;
}

/*--------------------------------------------------------------------------------------------------
  Log the command to the log file.
--------------------------------------------------------------------------------------------------*/
static void logCommand(
    uint32 argc,
    char **argv)
{
    char *encodedArg;
    uint32 xArg;
    time_t t = time(NULL);
    char timeBuf[100];
    struct tm *tm;

    if(meLogFile == NULL) {
        return;
    }
    tm = localtime(&t);
    strftime(timeBuf, 100, "%x-%X", tm);
    fprintf(meLogFile, "%s ", timeBuf);
    if(meCurrentUser != meUserNull) {
        fprintf(meLogFile, ":%s ", cgiEncode(meUserGetName(meCurrentUser)));
    }
    for(xArg = 0; xArg < argc; xArg++) {
        if(xArg != 0) {
            fprintf(meLogFile, " ");
        }
        encodedArg = cgiEncode(argv[xArg]);
        fprintf(meLogFile, "%s", encodedArg);
    }
    fprintf(meLogFile, "\n");
    fflush(meLogFile);
}

/*--------------------------------------------------------------------------------------------------
  Print help.
--------------------------------------------------------------------------------------------------*/
static void printHelp(
    char *command)
{
    if(command == NULL) {
        coPrintf(
            "Common commands:\n"
            "    login userName password\n"
            "    logout\n"
            "    quit\n"
            "    help\n"
            "    status\n"
            "    balance\n"
            "    send userName amount note - send money to an account\n"
            "    transactions [fromDate [toDate]] - list transactions\n"
            "    list [cateory]\n"
            "    show listingID\n"
            "    newMedia (offered|wanted) category rate title description\n"
            "    editMedia listingID (offered|wanted) category rate title description\n"
            "    deleteMedia listingID\n"
            "For manipulating accounts:\n"
            "    newUser userName shownName email password\n"
            "    newCharity userName shownName email password\n"
            "    updateAccount userName email oldPassword [password]\n"
            "    accountSettings\n"
            "    joinCharity charityName\n"
            "    validateUser userName key\n"
            "    deleteMyAccount\n"
            "    resetPassword userName\n"
            "Forums\n"
            "    newPost subject message\n"
            "    listThreads\n"
            "    listPosts threadID\n"
            "    showPost postID\n"
            "    reply threadID message\n"
            "    followThread threadID\n"
            "    leaveThread threadID\n"
            "    announce subject message\n"
            "    listAnnouncements\n"
            "    showAnnouncement announcementID\n"
            "Administrator only commands\n"
            "    createCategory category\n"
            "    deleteCategory category\n"
            "    renameCategory oldName newName\n"
            "    saveDatabase\n"
            "For more detailed description of a command, use \"help command\".\n"
            );
        return;
    }
    if(!strcmp(command, "help")) {
        coPrintf("Where's the fire?\n");
    } else if(!strcmp(command, "quit")) {
        coPrintf("I know... it can be so hard to quit.\n");
    } else if(!strcmp(command, "login")) {
        coPrintf("Whatever you do DONT LOG IN!!!\n");
    } else if(!strcmp(command, "logout")) {
        coPrintf("Whatever you do DONT LOG OUT!!!\n");
    } else if(!strcmp(command, "status")) {
        coPrintf("Report current session status and some statistics.\n");
    } else if(!strcmp(command, "balance")) {
        coPrintf("Report your balance.\n");
    } else if(!strcmp(command, "send")) {
        coPrintf("send userName amount note\n"
            "Send money from your account to someone else.  This is how you reward people\n"
            "for their services to you.  The amount should be whatever you agreed to.  You\n"
            " must make a note to yourself to remind you what this transaction is for.\n");
    } else if(!strcmp(command, "transactions")) {
        coPrintf("transactions [fromDate [toDate]]\n"
            "List transactions in your account between the dates listed (inclusive).  Dates\n"
            "are in MM/DD/YY format.\n");
    } else if(!strcmp(command, "list")) {
        coPrintf("list [cateory]\n"
            "List all categories, or the listings in a category.\n");
    } else if(!strcmp(command, "show")) {
        coPrintf("show listingID\n"
            "Show details on a specific listing.  The listingID is shown by the\n"
            "list command.\n");
    } else if(!strcmp(command, "newUser")) {
        coPrintf("newUser userName shownName email password\n"
            "Create a new unvalidated user.  The user should receive an e-mail with\n"
            "a validation key.\n");
    } else if(!strcmp(command, "newCharity")) {
        coPrintf("newCharity charityName shownName email password\n"
            "Create a new charity, which is a special kind of user, representing a\n"
            "participating charity.  The user should receive an e-mail with\n"
            "a validation key.\n");
    } else if(!strcmp(command, "updateAccount")) {
        coPrintf("updateAccount userName email oldPassword [password]\n"
            "Change account settings.  If no new password is provided, keep the old one.\n");
    } else if(!strcmp(command, "accountSettings")) {
        coPrintf("accountSettings\n"
            "List account settings.\n");
    } else if(!strcmp(command, "resetPassword")) {
        coPrintf("resetPassword userName\n"
            "Reset your password to a random string.  E-mail the new password to you.\n");
    } else if(!strcmp(command, "joinCharity")) {
        coPrintf("joinCharity charityName\n"
            "Join a charity.  You must join at least one charity to validate your account.\n");
    } else if(!strcmp(command, "validateUser")) {
        coPrintf("validateUser userName key\n"
            "Validate the user account by providing a key which was e-mailed to their\n"
            "account.\n");
    } else if(!strcmp(command, "deleteMyAccount")) {
        coPrintf("deleteMyAccount\n"
            "Be very careful with this command.  There is no undoing it, and it does\n"
            "not ask for confirmation.  You account will simply go away.\n");
    } else if(!strcmp(command, "changePassword")) {
        coPrintf("changePassword oldPassword newPassword\n"
            "Change your password.\n");
    } else if(!strcmp(command, "newPost")) {
        coPrintf("newPost subject message\n"
            "Create a new thread on the discussion forum with a first message.\n");
    } else if(!strcmp(command, "listThreads")) {
        coPrintf("listThreads\n"
            "List the threads.\n");
    } else if(!strcmp(command, "listPosts")) {
        coPrintf("listPosts threadID\n"
            "List the posts in the thread.\n");
    } else if(!strcmp(command, "showPost")) {
        coPrintf("showPost postID\n"
            "Show the post.\n");
    } else if(!strcmp(command, "reply")) {
        coPrintf("reply threadID message\n"
            "Reply to an existing thread.\n");
    } else if(!strcmp(command, "followThread")) {
        coPrintf("followThread threadID\n"
            "Follow a thread.  You will receive e-mails whenver there is a new post on\n"
            "any thread you follow.\n");
    } else if(!strcmp(command, "leaveThread")) {
        coPrintf("leaveThread threadID\n"
            "Stop following a thread\n");
    } else if(!strcmp(command, "announce")) {
        coPrintf("announce subject message\n"
            "Announce a message to everyone.  You must be the supreme leader.\n");
    } else if(!strcmp(command, "listAnnouncements")) {
        coPrintf("listAnnouncements\n"
            "List announcements.\n");
    } else if(!strcmp(command, "showAnnouncement")) {
        coPrintf("showAnnouncement announcementID\n"
            "Show an announcement.\n");
    } else if(!strcmp(command, "newMedia")) {
        coPrintf("newMedia (offered|wanted) category rate title description\n"
            "Create a new listing for a service you offer or want in a specific category.\n"
            "The rate is in US dollars per hour unless you start with =, in which case\n"
            "It's fixed price.\n");
    } else if(!strcmp(command, "editMedia")) {
        coPrintf("editMedia listintID (offered|wanted) category rate title description\n"
            "Edit an existing listing.\n");
    } else if(!strcmp(command, "deleteMedia")) {
        coPrintf("deleteMedia listingID\n"
            "Delete a service you offered or wanted.  The listingID is reported by\n"
            "the list command.\n");
    } else if(!strcmp(command, "createCategory")) {
        coPrintf("createCategory category\n"
            "Create a new category.  Only the Supreme Leader can do this.\n");
    } else if(!strcmp(command, "deleteCategory")) {
        coPrintf("deleteCategory category\n"
            "Delete a category.  Only the Supreme Leader can do this.\n");
    } else if(!strcmp(command, "renameCategory")) {
        coPrintf("renameCategory oldName newName\n"
            "Rename a category.  Only the Supreme Leader can do this.\n");
    } else if(!strcmp(command, "saveDatabase")) {
        coPrintf("saveDatabase\n"
            "If you are the supreme leader, you can overwrite the ASCII database, which\n"
            "will be loaded next time we run.  The log file will also be moved to\n"
            OLD_LOG_FILE "\n");
    } else {
        coPrintf("There is no command called '%s'.  Type 'help' for a list of commands\n",
            command);
    }
}

/*--------------------------------------------------------------------------------------------------
  Parse the command line into a set of arguments, separated by spaces.  The arguments are then
  decoded with cgiUnencode, and returned in the argv array.  The number of arguments parsed is
  returned.  The line argument is directly modified in place and the argv array is set to point
  into it.
--------------------------------------------------------------------------------------------------*/
static uint32 parseArguments(
    char *line,
    char **argv)
{
    char *start = line;
    char *end;
    uint32 xArg = 0;
    char c;

    utDo {
        while(*start <= ' ' && *start != '\0') {
            start++;
        }
    } utWhile(*start != '\0' && xArg < MAX_ARGS) {
        if(*start == '"') {
            start++;
            end = strchr(start, '"');
        } else {
            end = strchr(start, ' ');
        }
        if(end != NULL) {
            *end = '\0';
        }
        cgiUnencode(start);
        argv[xArg++] = start;
        if(end == NULL) {
            return xArg;
        }
        start = end + 1;
    } utRepeat;
    return xArg;
}

/*--------------------------------------------------------------------------------------------------
  Hash the nounce and password to get a SHA-256 cryptographic hash.  This is what we store.
--------------------------------------------------------------------------------------------------*/
static char *hashPassword(
    char *nounce,
    char *password)
{
    uint32 passwordLength = strlen(password);
    char *message = utNewA(char, NOUNCE_LENGTH + passwordLength);
    static char digest[SHA256_DIGEST_SIZE];

    memcpy(message, nounce, NOUNCE_LENGTH);
    memcpy(message + NOUNCE_LENGTH, password, passwordLength);
    sha256(message, NOUNCE_LENGTH + passwordLength, digest);
    utFree(message);
    return digest;
}

/*--------------------------------------------------------------------------------------------------
  Return the user's balance in standard terminology.
--------------------------------------------------------------------------------------------------*/
static char *findBalance(
    uint32 balance)
{
    static char buf[20];

    sprintf(buf, "$%d.%02d", balance/100, balance % 100);
    return buf;
}

/*--------------------------------------------------------------------------------------------------
  Log out.
--------------------------------------------------------------------------------------------------*/
static void logout(void)
{
    meUserSetLoggedIn(meCurrentUser, false);
    meUserSetSession(meCurrentUser, meSessionNull);
    meSessionSetUser(meCurrentSession, meUserNull);
    meCurrentUser = meUserNull;
}

/*--------------------------------------------------------------------------------------------------
  Process a login command.
--------------------------------------------------------------------------------------------------*/
static void processLoginCommand(
    uint32 argc,
    char **argv)
{
    meUser user;
    char *hashedPassword;
    bool supremeLeader = false;
    bool passwdOk;

    if(meCurrentUser != meUserNull) {
        supremeLeader = meUserSupremeLeader(meCurrentUser);
    }
    if(argc != 3 && !(argc == 2 && supremeLeader)) {
        coPrintf("Login requires a userName and a password.\n");
        return;
    } 
    user = meRootFindUser(meTheRoot, utSymCreate(utStringToLowerCase(argv[1])));
    if(user != meUserNull) {
        passwdOk = supremeLeader;
        if(!passwdOk) {
            hashedPassword = hashPassword(meUserGetNounce(user), argv[2]);
            passwdOk = !memcmp(hashedPassword, meUserGetHashedPassword(user), NOUNCE_LENGTH);
        }
        if(passwdOk) {
            if(meCurrentUser != meUserNull) {
                logout();
            }
            meUserSetLoggedIn(user, true);
            meCurrentUser = user;
            meSessionSetUser(meCurrentSession, user);
            meUserSetSession(user, meCurrentSession);
            coPrintf("Login successful.  You have %s.\n", findBalance(meUserGetBalance(user)));
            return;
        }
    }
    coPrintf("Invalid user/password combination.\n");
}

/*--------------------------------------------------------------------------------------------------
  Process a logout command.
--------------------------------------------------------------------------------------------------*/
static void processLogoutCommand(
    uint32 argc,
    char **argv)
{
    if(meCurrentUser == meUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    logout();
    coPrintf("Logout successful.\n");
}

/*--------------------------------------------------------------------------------------------------
  Process a status command.
--------------------------------------------------------------------------------------------------*/
static void processStatusCommand(
    uint32 argc,
    char **argv)
{
    if(argc != 1) {
        printHelp(argv[0]);
        return;
    } 
    if(meCurrentUser == meUserNull) {
        coPrintf("Not logged in.\n");
    } else {
        coPrintf("Logged in as user %s.", cgiEncode(meUserGetShownName(meCurrentUser)));
        if(meUserSupremeLeader(meCurrentUser)) {
            coPrintf("  You are the supreme leader.\n");
        } else {
            coPrintf("\n");
        }
    }
}

/*--------------------------------------------------------------------------------------------------
  Process a balance command.
--------------------------------------------------------------------------------------------------*/
static void processBalanceCommand(
    uint32 argc,
    char **argv)
{
    if(argc != 1) {
        printHelp(argv[0]);
        return;
    } 
    if(meCurrentUser == meUserNull) {
        coPrintf("Not currently logged in.\n");
    } else {
        coPrintf("%s\n", findBalance(meUserGetBalance(meCurrentUser)));
    }
}

/*--------------------------------------------------------------------------------------------------
  Read an integer.  Return true on success, false otherwise.
--------------------------------------------------------------------------------------------------*/
static bool readInt(
    char *string,
    int64 *value,
    bool positive)
{
    char *endPtr;

    *value = strtoll(string, &endPtr, 0);
    if(*string == '\0' || *endPtr != '\0' || (positive && *value < 0)) {
        coPrintf("Invalid integer '%s'.\n", string);
        return false;
    }
    return true;
}

/*--------------------------------------------------------------------------------------------------
  Process a send command to transfer money.
--------------------------------------------------------------------------------------------------*/
static void processSendCommand(
    uint32 argc,
    char **argv)
{
    meUser otherUser;
    meTransaction transaction;
    int64 amount, userBalance, otherBalance;
    uint64 t = time(NULL);
    char *endPtr;
    char userBalanceBuf[42], otherBalanceBuf[42];

    if(meCurrentUser == meUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(meProcessingLogFile? argc != 6 : argc != 4) {
        printHelp(argv[0]);
        return;
    }
    if(meCurrentUser == meUserNull) {
        coPrintf("You must be logged in to send money.\n");
        return;
    }
    otherUser = meRootFindUser(meTheRoot, utSymCreate(argv[1]));
    if(otherUser == meUserNull) {
        coPrintf("User %s not found\n", argv[1]);
        return;
    }
    if(!readInt(argv[2], &amount, true)) {
        return;
    }
    if(amount > meUserGetBalance(meCurrentUser)) {
        coPrintf("You do not have enough money.\n");
        return;
    }
    transaction = meTransactionAlloc();
    meTransactionSetAmount(transaction, amount);
    meTransactionSetDescription(transaction, argv[3], strlen(argv[3]) + 1);
    meTransactionSetDate(transaction, t);
    meUserAppendOutTransaction(meCurrentUser, transaction);
    meUserAppendInTransaction(otherUser, transaction);
    meUserSetBalance(meCurrentUser, meUserGetBalance(meCurrentUser) - amount);
    meUserSetBalance(otherUser, meUserGetBalance(otherUser) + amount);
    if(!meProcessingLogFile) {
        sprintf(userBalanceBuf, "%u", meUserGetBalance(meCurrentUser));
        argv[4] = userBalanceBuf;
        sprintf(otherBalanceBuf, "%u", meUserGetBalance(otherUser));
        argv[5] = otherBalanceBuf;
        logCommand(6, argv);
    } else {
        if(!readInt(argv[4], &userBalance, true) ||
                userBalance != meUserGetBalance(meCurrentUser)) {
            coPrintf("Invalid user balance for user %s.\n", meUserGetName(meCurrentUser));
            exit(1);
        }
        if(!readInt(argv[5], &otherBalance, true) ||
                otherBalance != meUserGetBalance(otherUser)) {
            coPrintf("Invalid user balance for user %s.\n", meUserGetName(otherUser));
            exit(1);
        }
    }
    coPrintf("Transaction completed successfully.  Your have %s.\n",
        findBalance(meUserGetBalance(meCurrentUser)));
}

/*--------------------------------------------------------------------------------------------------
  A callback function for qsort for sorting transactions by date.
--------------------------------------------------------------------------------------------------*/
static int compareTransactions(
    const void *transaction1Ptr,
    const void *transaction2Ptr)
{
    meTransaction transaction1 = *(meTransaction *)transaction1Ptr;
    meTransaction transaction2 = *(meTransaction *)transaction2Ptr;
    uint64 date1 = meTransactionGetDate(transaction1);
    uint64 date2 = meTransactionGetDate(transaction2);

    if(date1 < date2) {
        return 1;
    }
    if(date1 > date2) {
        return -1;
    }
    return 0;
}

extern char *strptime (__const char *__restrict __s,
                       __const char *__restrict __fmt, struct tm *__tp);
/*--------------------------------------------------------------------------------------------------
  Process a transactions command.
--------------------------------------------------------------------------------------------------*/
static void processTransactionsCommand(
    uint32 argc,
    char **argv)
{
    meTransaction transaction;
    meTransactionArray tarray;
    uint64 fromDate = 0;
    uint64 toDate = UINT64_MAX;
    uint64 date;
    struct tm tm;
    struct tm *tmPtr;
    time_t t;
    char *p;

    if(meCurrentUser == meUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(argc > 3) {
        printHelp(argv[0]);
        return;
    }
    if(argc >= 2) {
        p = strptime(argv[1], "%x", &tm);
        if(p == NULL || *p != '\0') {
            coPrintf("Invalid date format.  Use MM/DD/YY.\n");
            return;
        }
        fromDate = mktime(&tm);
        if(argc == 3) {
            p = strptime(argv[2], "%x", &tm);
            if(p == NULL || *p != '\0') {
                coPrintf("Invalid date format.  Use MM/DD/YY.\n");
                return;
            }
            toDate = mktime(&tm);
        }
    }
    tarray = meTransactionArrayAlloc();
    meForeachUserOutTransaction(meCurrentUser, transaction) {
        date = meTransactionGetDate(transaction);
        if(date >= fromDate && date <= toDate) {
            meTransactionArrayAppendTransaction(tarray, transaction);
        }
    } meEndUserOutTransaction;
    meForeachUserInTransaction(meCurrentUser, transaction) {
        date = meTransactionGetDate(transaction);
        if(date >= fromDate && date <= toDate) {
            meTransactionArrayAppendTransaction(tarray, transaction);
        }
    } meEndUserInTransaction;
    qsort(meTransactionArrayGetTransactions(tarray), meTransactionArrayGetUsedTransaction(tarray),
        sizeof(meTransaction), compareTransactions);
    meForeachTransactionArrayTransaction(tarray, transaction) {
        t = meTransactionGetDate(transaction);
        tmPtr = localtime(&t);
        if(meTransactionGetFromUser(transaction) == meCurrentUser) {
            coPrintf("%02d/%02d/%02d you sent %s %u - %s\n", tmPtr->tm_mon + 1, tmPtr->tm_mday,
                tmPtr->tm_year - 100, meUserGetShownName(meTransactionGetToUser(transaction)),
                meTransactionGetAmount(transaction), meTransactionGetDescription(transaction));
        } else {
            coPrintf("%02d/%02d/%02d %s sent you %u - %s\n", tmPtr->tm_mon + 1, tmPtr->tm_mday,
                tmPtr->tm_year - 100, meUserGetShownName(meTransactionGetFromUser(transaction)),
                meTransactionGetAmount(transaction), meTransactionGetDescription(transaction));
        }
    } meEndTransactionArrayTransaction;
    meTransactionArrayFree(tarray);
}

/*--------------------------------------------------------------------------------------------------
  Process a list command.
--------------------------------------------------------------------------------------------------*/
static void processListCommand(
    uint32 argc,
    char **argv)
{
    meCategory category = meCategoryNull;
    meMedia listing;
    bool hasItems = false;

    if(argc > 2) {
        printHelp(argv[0]);
        return;
    }
    if(argc == 2) {
        category = meRootFindCategory(meTheRoot, utSymCreate(argv[1]));
        if(category == meCategoryNull) {
            coPrintf("Invalid category '%s'\n", argv[1]);
            return;
        }
    }
    if(category != meCategoryNull) {
        meForeachCategoryMedia(category, listing) {
            coPrintf("%s listingID=%llu\n", meMediaGetTitle(listing), meMediaGetID(listing));
            hasItems = true;
        } meEndCategoryMedia;
        if(!hasItems) {
            coPrintf("There are no listings in this category.\n");
        }
    } else {
        meForeachRootCategory(meTheRoot, category) {
            //if(meCategoryGetNumMedias(category) > 0) {
                coPrintf("%s (%u)\n", meCategoryGetName(category),
                    meCategoryGetNumMedias(category));
                hasItems = true;
            //}
        } meEndRootCategory;
        if(!hasItems) {
            coPrintf("There are no categoreis.\n");
        }
    }
}

/*--------------------------------------------------------------------------------------------------
  Show details of a specific listing.
--------------------------------------------------------------------------------------------------*/
static void processShowCommand(
    uint32 argc,
    char **argv)
{
    meMedia listing;
    uint64 listingID;

    if(argc != 2) {
        printHelp(argv[0]);
        return;
    }
    if(!readInt(argv[1], &listingID, true)) {
        return;
    }
    listing = meRootFindMedia(meTheRoot, listingID);
    if(listing == meMediaNull) {
        coPrintf("Invalid listingID\n");
        return;
    }
    coPrintf("%s: price %u", cgiEncode(meMediaGetTitle(listing)), meMediaGetPrice(listing));
    coPrintf(" user=%s",
        cgiEncode(meUserGetShownName(meMediaGetUser(listing))));
    coPrintf(" category=%s listingID=%llu\n%s\n",
        cgiEncode(meCategoryGetName(meMediaGetCategory(listing))), listingID,
        meMediaGetShortDescription(listing));
    coPrintf("%s\n", meMediaGetLongDescription(listing));
}

/*--------------------------------------------------------------------------------------------------
  Determine if an email address is in a valid format.
--------------------------------------------------------------------------------------------------*/
static bool emailValid(
    char *email)
{
    char c = *email++;

    if(c == '\0' || c == '@') {
        return false;
    }
    while(c != '@') {
        if(!isalnum(c) && c != '_' && c != '.') {
            return false;
        }
        c = *email++;
    }
    c = *email++;
    if(c == '\0') {
        return false;
    }
    while(c != '\0') {
        if(!isalnum(c) && c != '_' && c != '.') {
            return false;
        }
        c = *email++;
    }
    return true;
}

/*--------------------------------------------------------------------------------------------------
  Create a new user.
--------------------------------------------------------------------------------------------------*/
static meUser createNewUser(
    uint32 argc,
    char **argv)
{
    meUser user;
    utSym userSym, emailSym;
    char *password, *nounce, *validationKey;
    char *hashedPassword;
    char encodedHash[SHA256_DIGEST_SIZE*3+1];

    if(meProcessingLogFile? argc != 7 : argc != 5) {
        printHelp(argv[0]);
        return meUserNull;
    }
    userSym = utSymCreate(utStringToLowerCase(argv[1]));
    emailSym = utSymCreate(argv[3]);
    if(!emailValid(argv[3])) {
        coPrintf("Email %s is not in a format we recognize.\n", argv[3]);
        return;
    }
    user = meRootFindUser(meTheRoot, userSym);
    if(user != meUserNull) {
        coPrintf("User %s already exists.\n", argv[1]);
        return meUserNull;
    }
    if(meRootFindByEmailUser(meTheRoot, emailSym) != meUserNull) {
        coPrintf("E-mail address %s is already in use.\n", argv[3]);
        return meUserNull;
    }
    user = meUserAlloc();
    meUserSetSym(user, userSym);
    meUserSetShownName(user, argv[2], strlen(argv[2]) + 1);
    meUserSetEmail(user, emailSym);
    if(!meProcessingLogFile) {
        password = argv[4];
        nounce = cgiGenerateRandomID(NOUNCE_LENGTH);
        meUserSetNounce(user, nounce, NOUNCE_LENGTH + 1);
        hashedPassword = hashPassword(meUserGetNounce(user), password);
        meUserSetHashedPassword(user, hashedPassword, SHA256_DIGEST_SIZE);
        strcpy(encodedHash, cgiEncodeArray(hashedPassword, SHA256_DIGEST_SIZE));
        validationKey = cgiGenerateRandomID(NOUNCE_LENGTH);
        meUserSetValidationKey(user, validationKey, NOUNCE_LENGTH + 1);
        argv[4] = meUserGetNounce(user);
        argv[5] = encodedHash;
        argv[6] = meUserGetValidationKey(user);
        sendMail(argv[3], "Welcome to Ebooks.coop!", 
            utSprintf("Dear %s,\n\n"
                "Thank you for registering at Ebooks.coop.  To complete your registration, "
                "please visit this url:\n\n"
                "    http://Ebooks.coop/validate.php?user=%s&key=%s\n\n"
                "Your user name is: %s\n"
                "Your password is: %s\n",
                meUserGetShownName(user), cgiEncode(meUserGetName(user)), validationKey,
                meUserGetShownName(user), password));
        logCommand(7, argv);
    } else {
        nounce = argv[4];
        meUserSetNounce(user, nounce, NOUNCE_LENGTH + 1);
        cgiUnencode(argv[5]);
        meUserSetHashedPassword(user, argv[5], SHA256_DIGEST_SIZE);
        validationKey = argv[6];
        meUserSetValidationKey(user, validationKey, NOUNCE_LENGTH + 1);
    }
    if(meRootGetFirstUser(meTheRoot) == meUserNull) {
        meUserSetSupremeLeader(user, true);
        coPrintf("User %s is the supreme leader.\n", meUserGetShownName(user));
    }
    meRootAppendUser(meTheRoot, user);
    meRootInsertByEmailUser(meTheRoot, user);
    return user;
}

/*--------------------------------------------------------------------------------------------------
  Join a charity.
--------------------------------------------------------------------------------------------------*/
static void processJoinCharityCommand(
    uint32 argc,
    char **argv)
{
    meUser user;
    meCharity charity;
    meCause cause;

    if(meCurrentUser == meUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(argc != 2) {
        printHelp(argv[0]);
        return;
    }
    user = meRootFindUser(meTheRoot, utSymCreate(argv[1]));
    if(user == meUserNull) {
        coPrintf("No charity named '%s' found.\n", argv[1]);
        return;
    }
    charity = meUserGetCharity(user);
    if(charity == meCharityNull) {
        coPrintf("User '%s' is not a charity.\n", argv[1]);
        return;
    }
    if(meUserFindCause(meCurrentUser, charity) != meCauseNull) {
        coPrintf("Already belong to charity %s\n", argv[1]);
        return;
    }
    cause = meCauseAlloc();
    meCharityAppendCause(charity, cause);
    meUserAppendCause(meCurrentUser, cause);
    logCommand(argc, argv);
    coPrintf("%s joined charity %s successfully.\n", meUserGetShownName(meCurrentUser), argv[1]);
}

/*--------------------------------------------------------------------------------------------------
  Create a new user.
--------------------------------------------------------------------------------------------------*/
static void processNewUserCommand(
    uint32 argc,
    char **argv)
{
    meUser user = createNewUser(argc, argv);
    meUser savedUser = meCurrentUser;
    meCharity charity = meRootGetFirstCharity(meTheRoot);
    char *joinCharityCommand[2] = {"joinCharity",};

    if(user != meUserNull) {
        if(!meProcessingLogFile && charity != meCharityNull) {
	    // Join the first charity by default
            meCurrentUser = user;
	    joinCharityCommand[1] = meUserGetName(meCharityGetUser(charity));
            processJoinCharityCommand(2, joinCharityCommand);
            meCurrentUser = savedUser;
        }
        coPrintf("New user %s created with validation key %s.\n", meUserGetShownName(user),
            meUserGetValidationKey(user));
    }
}

/*--------------------------------------------------------------------------------------------------
  Create a new charity.
--------------------------------------------------------------------------------------------------*/
static void processNewCharityCommand(
    uint32 argc,
    char **argv)
{
    meCharity charity;
    meUser user = createNewUser(argc, argv);

    if(user == meUserNull) {
        return;
    }
    charity = meCharityAlloc();
    meRootAppendCharity(meTheRoot, charity);
    meUserInsertCharity(user, charity);
    coPrintf("New charity %s created with validation key %s.\n", meUserGetShownName(user),
        meUserGetValidationKey(user));
}

/*--------------------------------------------------------------------------------------------------
  Update account settings.
--------------------------------------------------------------------------------------------------*/
static void processUpdateAccountCommand(
    uint32 argc,
    char **argv)
{
    meUser user;
    utSym userSym, emailSym;
    char *email, *password, *nounce, *validationKey;
    char *hashedPassword;
    char encodedHash[SHA256_DIGEST_SIZE*3+1];

    if(meCurrentUser == meUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(meProcessingLogFile? argc != 3 && argc != 5 : argc != 4 && argc != 5) {
        printHelp(argv[0]);
        return;
    }
    if(!meProcessingLogFile) {
        hashedPassword = hashPassword(meUserGetNounce(meCurrentUser), argv[3]);
        if(memcmp(hashedPassword, meUserGetHashedPassword(meCurrentUser), NOUNCE_LENGTH)) {
            coPrintf("Incorrect password.\n");
            return;
        }
    }
    userSym = utSymCreate(utStringToLowerCase(argv[1]));
    emailSym = utSymCreate(argv[2]);
    if(!emailValid(argv[2])) {
        coPrintf("Email %s is not in a format we recognize.\n", argv[2]);
        return;
    }
    user = meRootFindUser(meTheRoot, userSym);
    if(user != meUserNull && user != meCurrentUser) {
        coPrintf("User %s already exists.\n", argv[1]);
        return;
    }
    user = meRootFindByEmailUser(meTheRoot, emailSym);
    if(user != meUserNull && user != meCurrentUser) {
        coPrintf("E-mail address %s is already in use.\n", argv[2]);
        return;
    }
    if(!meProcessingLogFile) {
        if(argc == 5) {
            password = argv[4];
            nounce = cgiGenerateRandomID(NOUNCE_LENGTH);
            meUserSetNounce(meCurrentUser, nounce, NOUNCE_LENGTH + 1);
            hashedPassword = hashPassword(meUserGetNounce(meCurrentUser), password);
            meUserSetHashedPassword(meCurrentUser, hashedPassword, SHA256_DIGEST_SIZE);
            strcpy(encodedHash, cgiEncodeArray(hashedPassword, SHA256_DIGEST_SIZE));
            argv[3] = meUserGetNounce(meCurrentUser);
            argv[4] = encodedHash;
        }
        logCommand(argc, argv);
    } else if(argc == 6) {
        nounce = argv[3];
        meUserSetNounce(meCurrentUser, nounce, NOUNCE_LENGTH + 1);
        cgiUnencode(argv[4]);
        meUserSetHashedPassword(meCurrentUser, argv[4], SHA256_DIGEST_SIZE);
    }
    if(meUserGetSym(meCurrentUser) != userSym) {
        meRootRenameUser(meTheRoot, meCurrentUser, userSym);
    }
    meUserSetShownName(meCurrentUser, argv[1], strlen(argv[1]) + 1);
    meRootRemoveByEmailUser(meTheRoot, meCurrentUser);
    meUserSetEmail(meCurrentUser, emailSym);
    meRootInsertByEmailUser(meTheRoot, meCurrentUser);
    coPrintf("Account updated succesfully.\n");
}

/*--------------------------------------------------------------------------------------------------
  Show account settings.
--------------------------------------------------------------------------------------------------*/
static void processAccountSettingsCommand(
    uint32 argc,
    char **argv)
{
    if(meCurrentUser == meUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    coPrintf("User %s, email %s\n", cgiEncode(meUserGetShownName(meCurrentUser)),
        utSymGetName(meUserGetEmail(meCurrentUser)));
}

/*--------------------------------------------------------------------------------------------------
  Reset a password.
--------------------------------------------------------------------------------------------------*/
static void processResetPasswordCommand(
    uint32 argc,
    char **argv)
{ 
    meUser user;
    char *nounce, *password, *hashedPassword;
    char encodedHash[SHA256_DIGEST_SIZE*3+1];

    if(meProcessingLogFile? argc!= 4 : argc != 2) {
        printHelp(argv[0]);
        return;
    }
    user = meRootFindUser(meTheRoot, utSymCreate(utStringToLowerCase(argv[1])));
    if(user == meUserNull) {
        coPrintf("User not found.\n");
        return;
    }
    if(!meProcessingLogFile) {
        nounce = cgiGenerateRandomID(NOUNCE_LENGTH);
        meUserSetNounce(user, nounce, NOUNCE_LENGTH + 1);
        password = cgiGenerateRandomID(8);
        hashedPassword = hashPassword(meUserGetNounce(user), password);
        meUserSetHashedPassword(user, hashedPassword, SHA256_DIGEST_SIZE);
        strcpy(encodedHash, cgiEncodeArray(hashedPassword, SHA256_DIGEST_SIZE));
        argv[2] = meUserGetNounce(user);
        argv[3] = encodedHash;
        logCommand(4, argv);
        sendMail(utSymGetName(meUserGetEmail(user)), "Your new Ebooks.coop password",
            utSprintf("You have reset your password at Ebooks.coop.  "
            "Your new password is:\n\npassword: %s\n", password));
    } else {
        meUserSetNounce(user, argv[2], NOUNCE_LENGTH + 1);
        cgiUnencode(argv[3]);
        meUserSetHashedPassword(user, argv[3], SHA256_DIGEST_SIZE);
    }
    coPrintf("Password reset for user %s.\n", meUserGetShownName(user));
}

/*--------------------------------------------------------------------------------------------------
  Just count the number of charities a user belongs to.
--------------------------------------------------------------------------------------------------*/
static uint32 countUserCharities(
    meUser user)
{
    meCause cause;
    uint32 numCharities = 0;

    meForeachUserCause(user, cause) {
        numCharities++;
    } meEndUserCause;
    return numCharities;
}

/*--------------------------------------------------------------------------------------------------
  Donate money to a charity.
--------------------------------------------------------------------------------------------------*/
static void donate(
    meCharity charity,
    meUser user,
    uint32 donation)
{
    meUser charityManager = meCharityGetUser(charity);
    uint32 balance = meUserGetBalance(user);

    if(balance < donation) {
        coPrintf("Not enough money in your account.\n");
        return;
    }
    meUserSetBalance(user, balance - donation);
    meUserSetBalance(charityManager, meUserGetBalance(charityManager) + donation);
}

/*--------------------------------------------------------------------------------------------------
  Distribute money too this user's charities as evenly as possible.
--------------------------------------------------------------------------------------------------*/
static void distributeToCharity(
    meUser user,
    uint32 cents)
{
    meCharity charity;
    meCause cause;
    uint32 numCharities = countUserCharities(user);
    uint32 amount = cents/numCharities;
    uint32 remainder = cents - (amount*numCharities);
    uint32 donation;

    meForeachUserCause(user, cause) {
        charity = meCauseGetCharity(cause);
        if(remainder > 0) {
            donation = amount + 1;
            remainder--;
        } else {
            donation = amount;
        }
        donate(charity, user, donation);
    } meEndUserCause;
}

/*--------------------------------------------------------------------------------------------------
  Validate a new user.
--------------------------------------------------------------------------------------------------*/
static void processValidateUserCommand(
    uint32 argc,
    char **argv)
{
    meUser user;
    utSym userSym;
    char key[NOUNCE_LENGTH];

    if(argc != 3) {
        printHelp(argv[0]);
        return;
    }
    userSym = utSymCreate(utStringToLowerCase(argv[1]));
    user = meRootFindUser(meTheRoot, userSym);
    if(user == meUserNull) {
        coPrintf("Invalid user %s\n", argv[1]);
        return;
    }
    if(strlen(argv[2]) != NOUNCE_LENGTH) {
        coPrintf("Invalid validation key\n");
        return;
    }
    if(!meProcessingLogFile && memcmp(meUserGetValidationKey(user), argv[2], NOUNCE_LENGTH)) {
        coPrintf("Incorrect validation key.\n");
        return;
    }
    meUserSetValidated(user, true);
    logCommand(argc, argv);
    coPrintf("User %s validated.\n", argv[1]);
}

/*--------------------------------------------------------------------------------------------------
  Delete a user account.
--------------------------------------------------------------------------------------------------*/
static void processDeleteMyAccountCommand(
    uint32 argc,
    char **argv)
{
    if(meCurrentUser == meUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(meUserGetFirstInTransaction(meCurrentUser) != meTransactionNull) {
        coPrintf("Can't delete users with completed transactions.\n");
        return;
    }
    if(argc != 1) {
        printHelp(argv[0]);
        return;
    }
    coPrintf("Account %s deleted.\n", meUserGetShownName(meCurrentUser));
    logCommand(argc, argv);
    meUserDestroy(meCurrentUser);
}

/*--------------------------------------------------------------------------------------------------
  Process a change password request.
--------------------------------------------------------------------------------------------------*/
static void processChangePasswordCommand(
    uint32 argc,
    char **argv)
{
    char *oldPassword, *newPassword;
    char *hashedPassword;
    char encodedHash[SHA256_DIGEST_SIZE*3+1];

    if(meCurrentUser == meUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(meProcessingLogFile? argc != 2 : argc != 3) {
        printHelp(argv[0]);
        return;
    }
    if(!meProcessingLogFile) {
        oldPassword = argv[1];
        newPassword = argv[2];
        hashedPassword = hashPassword(meUserGetNounce(meCurrentUser), oldPassword);
        if(memcmp(hashedPassword, meUserGetHashedPassword(meCurrentUser), SHA256_DIGEST_SIZE)) {
            coPrintf("Incorrect password.\n");
            return;
        }
        hashedPassword = hashPassword(meUserGetNounce(meCurrentUser), newPassword);
        meUserSetHashedPassword(meCurrentUser, hashedPassword, SHA256_DIGEST_SIZE);
        strcpy(encodedHash, cgiEncodeArray(hashedPassword, SHA256_DIGEST_SIZE));
        argv[1] = encodedHash;
        logCommand(2, argv);
    } else {
        cgiUnencode(argv[1]);
        meUserSetHashedPassword(meCurrentUser, argv[1], SHA256_DIGEST_SIZE);
    }
    coPrintf("Succesfully changed password.\n");
}

/*--------------------------------------------------------------------------------------------------
  Create a new post.
--------------------------------------------------------------------------------------------------*/
static mePost mePostCreate(
    meUser user,
    meThread thread,
    char *message)
{
    mePost post = mePostAlloc();
    uint32 postID = meRootGetNextPostID(meTheRoot);

    mePostSetID(post, postID);
    meRootSetNextPostID(meTheRoot, postID + 1);
    mePostSetMessage(post, message, strlen(message) + 1);
    meUserAppendPost(user, post);
    meThreadAppendPost(thread, post);
    meRootAppendPost(meTheRoot, post);
    meThreadSetNumPosts(thread, meThreadGetNumPosts(thread) + 1);
    return post;
}

/*--------------------------------------------------------------------------------------------------
  Find a subscription for the user and thread.
--------------------------------------------------------------------------------------------------*/
static meSubscription findSubscription(
    meUser user,
    meThread thread)
{
    meSubscription subscription;

    meForeachUserSubscription(user, subscription) {
        if(meSubscriptionGetThread(subscription) == thread) {
            return subscription;
        }
    } meEndUserSubscription;
    return meSubscriptionNull;
}

/*--------------------------------------------------------------------------------------------------
  Follow the thread, if we don't already.
--------------------------------------------------------------------------------------------------*/
static void followThread(
    meThread thread)
{
    meSubscription subscription = findSubscription(meCurrentUser, thread);

    if(subscription != meSubscriptionNull) {
        return;
    }
    subscription = meSubscriptionAlloc();
    meUserAppendSubscription(meCurrentUser, subscription);
    meThreadAppendSubscription(thread, subscription);
}

/*--------------------------------------------------------------------------------------------------
  Process a followThread command.
--------------------------------------------------------------------------------------------------*/
static void processFollowThreadCommand(
    uint32 argc,
    char **argv)
{
    meThread thread;
    int64 threadID;

    if(meCurrentUser == meUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(argc != 2) {
        printHelp(argv[0]);
        return;
    }
    if(!readInt(argv[1], &threadID, true)) {
        return;
    }
    thread = meRootFindThread(meTheRoot, threadID);
    if(thread == meThreadNull) {
        coPrintf("Invalid thread ID.\n");
        return;
    }
    followThread(thread);
    logCommand(argc, argv);
}

/*--------------------------------------------------------------------------------------------------
  Process a leaveThread command.
--------------------------------------------------------------------------------------------------*/
static void processLeaveThreadCommand(
    uint32 argc,
    char **argv)
{
    meThread thread;
    meSubscription subscription;
    int64 threadID;

    if(meCurrentUser == meUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(argc != 2) {
        printHelp(argv[0]);
        return;
    }
    if(!readInt(argv[1], &threadID, true)) {
        return;
    }
    thread = meRootFindThread(meTheRoot, threadID);
    if(thread == meThreadNull) {
        coPrintf("Invalid thread ID.\n");
        return;
    }
    subscription =  findSubscription(meCurrentUser, thread);
    if(subscription == meSubscriptionNull) {
        return;
    }
    meSubscriptionDestroy(subscription);
    logCommand(argc, argv);
}

/*--------------------------------------------------------------------------------------------------
  Process an announce command.
--------------------------------------------------------------------------------------------------*/
static void processAnnounceCommand(
    uint32 argc,
    char **argv)
{
    meAnnouncement announcement;

    if(meCurrentUser == meUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(argc != 3) {
        printHelp(argv[0]);
        return;
    }
    if(!meUserSupremeLeader(meCurrentUser)) {
        coPrintf("You must be the Supreme Leader to post announcements.\n");
        return;
    }
    announcement = meAnnouncementAlloc();
    meAnnouncementSetID(announcement, meRootGetNextAnnouncementID(meTheRoot));
    meRootSetNextAnnouncementID(meTheRoot, meRootGetNextAnnouncementID(meTheRoot) + 1);
    meAnnouncementSetSubject(announcement, argv[1], strlen(argv[1]) + 1);
    meAnnouncementSetMessage(announcement, argv[2], strlen(argv[2]) + 1);
    meUserAppendAnnouncement(meCurrentUser, announcement);
    meRootAppendAnnouncement(meTheRoot, announcement);
    if(!meProcessingLogFile) {
        sendAnnouncement(announcement);
        coPrintf("Announcement sent\n");
        logCommand(argc, argv);
    } else {
        coPrintf("Announcement created\n");
    }
}

/*--------------------------------------------------------------------------------------------------
  Process a list announcements command.
--------------------------------------------------------------------------------------------------*/
static void processListAnnouncementsCommand(
    uint32 argc,
    char **argv)
{
    meAnnouncement announcement;
    meUser user;
    bool printedAnnouncement = false;

    if(argc != 1) {
        printHelp(argv[0]);
        return;
    }
    meForeachRootAnnouncement(meTheRoot, announcement) {
        user = meAnnouncementGetUser(announcement);
        if(meUserSupremeLeader(user)) {
            coPrintf("%u %s\n", meAnnouncementGetID(announcement), cgiEncode(meAnnouncementGetSubject(announcement)));
            printedAnnouncement = true;
        }
    } meEndRootAnnouncement;
    if(!printedAnnouncement) {
        coPrintf("No announcements.\n");
    }
}

/*--------------------------------------------------------------------------------------------------
  Process a showAnnouncement command.
--------------------------------------------------------------------------------------------------*/
static void processShowAnnouncementCommand(
    uint32 argc,
    char **argv)
{
    meAnnouncement announcement;
    uint64 announcementID;

    if(argc != 2) {
        printHelp(argv[0]);
        return;
    }
    if(!readInt(argv[1], &announcementID, true)) {
        return;
    }
    announcement = meRootFindAnnouncement(meTheRoot, announcementID);
    if(announcement == meAnnouncementNull) {
        coPrintf("Invalid announcement ID.\n");
        return;
    }
    coPrintf("%s\n", meAnnouncementGetMessage(announcement));
}

/*--------------------------------------------------------------------------------------------------
  Process a new post command.
--------------------------------------------------------------------------------------------------*/
static void processNewPostCommand(
    uint32 argc,
    char **argv)
{
    meThread thread;
    mePost post;
    uint32 threadID;

    if(meCurrentUser == meUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(argc != 3) {
        printHelp(argv[0]);
        return;
    }
    thread = meThreadAlloc();
    threadID = meRootGetNextThreadID(meTheRoot);
    meThreadSetID(thread, threadID);
    meRootSetNextThreadID(meTheRoot, threadID + 1);
    meThreadSetSubject(thread, argv[1], strlen(argv[1]) + 1);
    meRootAppendThread(meTheRoot, thread);
    post = mePostCreate(meCurrentUser, thread, argv[2]);
    followThread(thread);
    logCommand(argc, argv);
    coPrintf("Created thread %u\n", meThreadGetID(thread));
}

/*--------------------------------------------------------------------------------------------------
  Process a list threads command.
--------------------------------------------------------------------------------------------------*/
static void processListThreadsCommand(
    uint32 argc,
    char **argv)
{
    meThread thread;

    if(argc != 1) {
        printHelp(argv[0]);
        return;
    }
    if(meRootGetFirstThread(meTheRoot) == meThreadNull) {
        coPrintf("No threads.\n");
        return;
    }
    meForeachRootThread(meTheRoot, thread) {
        if(meCurrentUser != meUserNull &&
                findSubscription(meCurrentUser, thread) != meSubscriptionNull) {
            coPrintf("+");
        }
        coPrintf("%u %s (%u)\n", meThreadGetID(thread), meThreadGetSubject(thread),
            meThreadGetNumPosts(thread));
    } meEndRootThread;
}

/*--------------------------------------------------------------------------------------------------
  Process a listPosts command.
--------------------------------------------------------------------------------------------------*/
processListPostsCommand(
    uint32 argc,
    char **argv)
{
    meThread thread;
    mePost post;
    uint64 threadID;

    if(argc != 2) {
        printHelp(argv[0]);
        return;
    }
    if(!readInt(argv[1], &threadID, true)) {
        return;
    }
    thread = meRootFindThread(meTheRoot, threadID);
    if(thread == meThreadNull) {
        coPrintf("Invalid thread ID.\n");
        return;
    }
    meForeachThreadPost(thread, post) {
        coPrintf("%u %s\n", mePostGetID(post), meUserGetShownName(mePostGetUser(post)));
    } meEndThreadPost;
}

/*--------------------------------------------------------------------------------------------------
  Process a showPost command.
--------------------------------------------------------------------------------------------------*/
static void processShowPostCommand(
    uint32 argc,
    char **argv)
{
    mePost post;
    uint64 postID;

    if(argc != 2) {
        printHelp(argv[0]);
        return;
    }
    if(!readInt(argv[1], &postID, true)) {
        return;
    }
    post = meRootFindPost(meTheRoot, postID);
    if(post == mePostNull) {
        coPrintf("Invalid post ID.\n");
        return;
    }
    coPrintf("%s\n", mePostGetMessage(post));
}

/*--------------------------------------------------------------------------------------------------
  Send this post 
--------------------------------------------------------------------------------------------------*/
static void sendFollowersPost(
    mePost post)
{
    meThread thread = mePostGetThread(post);
    meSubscription subscription;
    meUser user;

    if(meProcessingLogFile) {
        return;
    }
    meForeachThreadSubscription(thread, subscription) {
        user = meSubscriptionGetUser(subscription);
        if(mePostGetUser(post) != user) {
            mailPost(meSubscriptionGetUser(subscription), post);
        }
    } meEndThreadSubscription;
}

/*--------------------------------------------------------------------------------------------------
  Process a reply command.
--------------------------------------------------------------------------------------------------*/
static void processReplyCommand(
    uint32 argc,
    char **argv)
{
    meThread thread;
    mePost post;
    uint64 threadID;

    if(meCurrentUser == meUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(argc != 3) {
        printHelp(argv[0]);
        return;
    }
    if(!readInt(argv[1], &threadID, true)) {
        return;
    }
    thread = meRootFindThread(meTheRoot, threadID);
    post = mePostCreate(meCurrentUser, thread, argv[2]);
    sendFollowersPost(post);
    followThread(thread);
    mailPost(meCurrentUser, post);
    logCommand(argc, argv);
    coPrintf("Created post %u\n", mePostGetID(post));
}

/*--------------------------------------------------------------------------------------------------
  Create a new listing.
--------------------------------------------------------------------------------------------------*/
static meMedia meMediaCreate(
    meUser user,
    meMediaType type,
    meCategory category,
    uint32 price,
    char *title,
    char *shortDescription,
    char *longDescription)
{
    meMedia listing = meMediaAlloc();
    uint64 listingID = meRootGetNextMediaID(meTheRoot);

    meMediaSetID(listing, listingID);
    meRootSetNextMediaID(meTheRoot, listingID + 1);
    meMediaSetOffered(listing, offered);
    meMediaSetFixedPrice(listing, fixedPrice);
    meMediaSetTitle(listing, title, strlen(title) + 1);
    meMediaSetDescription(listing, description, strlen(description) + 1);
    meCategoryAppendMedia(category, listing);
    meRootInsertMedia(meTheRoot, listing);
    meUserAppendMedia(user, listing);
    meCategorySetNumMedias(category, meCategoryGetNumMedias(category) + 1);
    return listing;
}

/*--------------------------------------------------------------------------------------------------
  Update a listing
--------------------------------------------------------------------------------------------------*/
static void updateMedia(
    meMedia listing,
    meCategory category,
    uint32 rate,
    bool offered,
    bool fixedPrice,
    char *title,
    char *description)
{
    meMediaSetRate(listing, rate);
    meMediaSetOffered(listing, offered);
    meMediaSetFixedPrice(listing, fixedPrice);
    meMediaSetTitle(listing, title, strlen(title) + 1);
    meMediaSetDescription(listing, description, strlen(description) + 1);
}

/*--------------------------------------------------------------------------------------------------
  Create a new service listing.
--------------------------------------------------------------------------------------------------*/
static void processNewMediaCommand(
    uint32 argc,
    char **argv)
{
    meCategory category;
    meMedia listing;
    utSym categorySym;
    int64 rate;
    bool fixedPrice = false;
    bool offerService;

    if(meCurrentUser == meUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(argc != 6) {
        printHelp(argv[0]);
        return;
    }
    offerService = strcmp(argv[1], "wanted");
    categorySym = utSymCreate(argv[2]);
    category = meRootFindCategory(meTheRoot, categorySym);
    if(category == meCategoryNull) {
        coPrintf("Category %s does not exist.\n", argv[2]);
        return;
    }
    if(*argv[3] == '=') {
        fixedPrice = true;
        if(!readInt(argv[3] + 1, &rate, true)) {
            coPrintf("Invalid rate.\n");
            return;
        }
    } else {
        if(!readInt(argv[3], &rate, true) || rate < 10 || rate > 200) {
            coPrintf("Invalid rate.\n");
            return;
        }
    }
    listing = meMediaCreate(meCurrentUser, category, rate, offerService, fixedPrice, argv[4],
        argv[5]);
    logCommand(argc, argv);
    coPrintf("New listing created with listingID %llu\n", meMediaGetID(listing));
}

/*--------------------------------------------------------------------------------------------------
  Process an edit listing command.
--------------------------------------------------------------------------------------------------*/
static void processEditMediaCommand(
    uint32 argc,
    char **argv)
{
    meCategory category;
    meMedia listing;
    utSym categorySym;
    int64 rate, listingID;
    bool fixedPrice = false;
    bool offerService;

    if(meCurrentUser == meUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(argc != 7) {
        printHelp(argv[0]);
        return;
    }
    if(!readInt(argv[1], &listingID, true)) {
        return;
    }
    listing = meRootFindMedia(meTheRoot, (uint32)listingID);
    if(listing == meMediaNull) {
        coPrintf("No listing %llu.\n", listingID);
        return;
    }
    if(meMediaGetUser(listing) != meCurrentUser && !meUserSupremeLeader(meCurrentUser)) {
        coPrintf("You do not own this listing.\n");
        return;
    }
    offerService = strcmp(argv[2], "wanted");
    categorySym = utSymCreate(argv[3]);
    category = meRootFindCategory(meTheRoot, categorySym);
    if(category == meCategoryNull) {
        coPrintf("Category %s does not exist.\n", argv[3]);
        return;
    }
    if(*argv[4] == '=') {
        fixedPrice = true;
        if(!readInt(argv[4] + 1, &rate, true)) {
            coPrintf("Invalid rate.\n");
            return;
        }
    } else {
        if(!readInt(argv[4], &rate, true) || rate < 10 || rate > 200) {
            coPrintf("Invalid rate.\n");
            return;
        }
    }
    updateMedia(listing, category, rate, offerService, fixedPrice, argv[5], argv[6]);
    logCommand(argc, argv);
    coPrintf("Media updated\n", meMediaGetID(listing));
}

/*--------------------------------------------------------------------------------------------------
  Process a delete listing command.
--------------------------------------------------------------------------------------------------*/
static void processDeleteMediaCommand(
    uint32 argc,
    char **argv)
{
    meCategory category;
    meMedia listing;
    utSym categorySym;
    int64 listingID;

    if(meCurrentUser == meUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(argc != 2) {
        printHelp(argv[0]);
        return;
    }
    if(!readInt(argv[1], &listingID, true)) {
        return;
    }
    listing = meRootFindMedia(meTheRoot, listingID);
    if(listing == meMediaNull) {
        coPrintf("Invalid listingID\n");
        return;
    }
    if(meMediaGetUser(listing) != meCurrentUser) {
        coPrintf("You don't own this listing.\n");
        return;
    }
    category = meMediaGetCategory(listing);
    meMediaDestroy(listing);
    meCategorySetNumMedias(category, meCategoryGetNumMedias(category) - 1);
    if(meCategoryGetFirstMedia(category) == meMediaNull) {
        meCategoryDestroy(category);
    }
    logCommand(argc, argv);
    coPrintf("Media %llu deleted.\n", listingID);
}

/*--------------------------------------------------------------------------------------------------
  Process a createCategory command.
--------------------------------------------------------------------------------------------------*/
static void processCreateCategoryCommand(
    uint32 argc,
    char **argv)
{
    meCategory category;
    utSym categorySym;

    if(meCurrentUser == meUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(!meUserSupremeLeader(meCurrentUser)) {
        coPrintf("Only the Supreme Leander can create categories.\n");
        return;
    }
    if(argc != 2) {
        printHelp(argv[0]);
        return;
    }
    categorySym = utSymCreate(argv[1]);
    category = meRootFindCategory(meTheRoot, categorySym);
    if(category != meCategoryNull) {
        coPrintf("Category %s already exists.\n", argv[1]);
        return;
    }
    category = meCategoryAlloc();
    meCategorySetSym(category, categorySym);
    meRootInsertCategory(meTheRoot, category);
    logCommand(argc, argv);
    coPrintf("Category %s created.\n", argv[1]);
}

/*--------------------------------------------------------------------------------------------------
  Process a deleteCategory command.
--------------------------------------------------------------------------------------------------*/
static void processDeleteCategoryCommand(
    uint32 argc,
    char **argv)
{
    meCategory category;
    utSym categorySym;

    if(meCurrentUser == meUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(!meUserSupremeLeader(meCurrentUser)) {
        coPrintf("Only the Supreme Leader can delete categories.\n");
        return;
    }
    if(argc != 2) {
        printHelp(argv[0]);
        return;
    }
    categorySym = utSymCreate(argv[1]);
    category = meRootFindCategory(meTheRoot, categorySym);
    if(category == meCategoryNull) {
        coPrintf("Category %s does not exist.\n", argv[1]);
        return;
    }
    meCategoryDestroy(category);
    logCommand(argc, argv);
    coPrintf("Category %s deleted.\n", argv[1]);
}

/*--------------------------------------------------------------------------------------------------
  Process a renameCategory command.
--------------------------------------------------------------------------------------------------*/
static void processRenameCategoryCommand(
    uint32 argc,
    char **argv)
{
    meCategory category;
    utSym categorySym;

    if(meCurrentUser == meUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(!meUserSupremeLeader(meCurrentUser)) {
        coPrintf("Only the Supreme Leader can rename categories.\n");
        return;
    }
    if(argc != 3) {
        printHelp(argv[0]);
        return;
    }
    categorySym = utSymCreate(argv[1]);
    category = meRootFindCategory(meTheRoot, categorySym);
    if(category == meCategoryNull) {
        coPrintf("Category %s does not exist.\n", argv[1]);
        return;
    }
    meRootRenameCategory(meTheRoot, category, utSymCreate(argv[2]));
    logCommand(argc, argv);
    coPrintf("Category %s renamed to %s.\n", argv[1], argv[2]);
}

/*--------------------------------------------------------------------------------------------------
  Save the database in ASCII
--------------------------------------------------------------------------------------------------*/
static void processSaveDatabaseCommand(
    uint32 argc,
    char **argv)
{
    meUser user;
    FILE *file;

    if(meCurrentUser == meUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(!meUserSupremeLeader(meCurrentUser)) {
        coPrintf("Only the supreme leader can save the database.\n");
        return;
    }
    if(argc != 1) {
        printHelp(argv[0]);
        return;
    }
    file = fopen(DATABASE_FILE, "w");
    if(file == NULL) {
        coPrintf("Unable to open " DATABASE_FILE "\n");
        return;
    }
    utSaveTextDatabase(file);
    fclose(file);
    fclose(meLogFile);
    rename(LOG_FILE, OLD_LOG_FILE);
    meLogFile = fopen(LOG_FILE, "w");
}

/*--------------------------------------------------------------------------------------------------
  Process the command in the line buffer.
--------------------------------------------------------------------------------------------------*/
static bool processCommand(
    uint32 argc,
    char **argv)
{
    char *command;

    if(argc == 0) {
        /* Empty line */
        return true;
    }
    command = argv[0];
    if(!strcmp(command, "help")) {
        printHelp(argc >= 2? argv[1] : NULL);
    } else if(!strcmp(command, "quit")) {
        return false;
    } else if(!strcmp(command, "login")) {
        processLoginCommand(argc, argv);
    } else if(!strcmp(command, "logout")) {
        processLogoutCommand(argc, argv);
    } else if(!strcmp(command, "status")) {
        processStatusCommand(argc, argv);
    } else if(!strcmp(command, "balance")) {
        processBalanceCommand(argc, argv);
    } else if(!strcmp(command, "send")) {
        processSendCommand(argc, argv);
    } else if(!strcmp(command, "transactions")) {
        processTransactionsCommand(argc, argv);
    } else if(!strcmp(command, "list")) {
        processListCommand(argc, argv);
    } else if(!strcmp(command, "show")) {
        processShowCommand(argc, argv);
    } else if(!strcmp(command, "newUser")) {
        processNewUserCommand(argc, argv);
    } else if(!strcmp(command, "newCharity")) {
        processNewCharityCommand(argc, argv);
    } else if(!strcmp(command, "updateAccount")) {
        processUpdateAccountCommand(argc, argv);
    } else if(!strcmp(command, "accountSettings")) {
        processAccountSettingsCommand(argc, argv);
    } else if(!strcmp(command, "resetPassword")) {
        processResetPasswordCommand(argc, argv);
    } else if(!strcmp(command, "joinCharity")) {
        processJoinCharityCommand(argc, argv);
    } else if(!strcmp(command, "validateUser")) {
        processValidateUserCommand(argc, argv);
    } else if(!strcmp(command, "deleteMyAccount")) {
        processDeleteMyAccountCommand(argc, argv);
    } else if(!strcmp(command, "changePassword")) {
        processChangePasswordCommand(argc, argv);
    } else if(!strcmp(command, "newPost")) {
        processNewPostCommand(argc, argv);
    } else if(!strcmp(command, "listThreads")) {
        processListThreadsCommand(argc, argv);
    } else if(!strcmp(command, "listPosts")) {
        processListPostsCommand(argc, argv);
    } else if(!strcmp(command, "showPost")) {
        processShowPostCommand(argc, argv);
    } else if(!strcmp(command, "reply")) {
        processReplyCommand(argc, argv);
    } else if(!strcmp(command, "followThread")) {
        processFollowThreadCommand(argc, argv);
    } else if(!strcmp(command, "leaveThread")) {
        processLeaveThreadCommand(argc, argv);
    } else if(!strcmp(command, "announce")) {
        processAnnounceCommand(argc, argv);
    } else if(!strcmp(command, "listAnnouncements")) {
        processListAnnouncementsCommand(argc, argv);
    } else if(!strcmp(command, "showAnnouncement")) {
        processShowAnnouncementCommand(argc, argv);
    } else if(!strcmp(command, "newMedia")) {
        processNewMediaCommand(argc, argv);
    } else if(!strcmp(command, "editMedia")) {
        processEditMediaCommand(argc, argv);
    } else if(!strcmp(command, "deleteMedia")) {
        processDeleteMediaCommand(argc, argv);
    } else if(!strcmp(command, "createCategory")) {
        processCreateCategoryCommand(argc, argv);
    } else if(!strcmp(command, "deleteCategory")) {
        processDeleteCategoryCommand(argc, argv);
    } else if(!strcmp(command, "renameCategory")) {
        processRenameCategoryCommand(argc, argv);
    } else if(!strcmp(command, "saveDatabase")) {
        processSaveDatabaseCommand(argc, argv);
    } else {
        coPrintf("Invalid command '%s'.  Type 'help' for a list of commands\n", command);
    }
    return true;
}

/*--------------------------------------------------------------------------------------------------
  Just switch to the session.  Return true if this is a new session.
--------------------------------------------------------------------------------------------------*/
static bool switchSession(
    char *sessionID)
{
    utSym sym = utSymCreate(sessionID);
    meSession session = meRootFindSession(meTheRoot, sym);

    if(session == meCurrentSession && session != meSessionNull) {
        return false;
    }
    if(meCurrentSession != meSessionNull) {
        meSessionSetUser(meCurrentSession, meCurrentUser);
        if(meCurrentUser != meUserNull) {
            meUserSetSession(meCurrentUser, meCurrentSession);
        }
    }
    if(session == meSessionNull) {
        session = meSessionAlloc();
        meSessionSetSym(session, sym);
        meRootInsertSession(meTheRoot, session);
        meCurrentUser = meUserNull;
        meCurrentSession = session;
        return true;
    }
    meCurrentSession = session;
    meCurrentUser = meSessionGetUser(session);
    return false;
}

/*--------------------------------------------------------------------------------------------------
  Interpret commands and modify the graph database.
--------------------------------------------------------------------------------------------------*/
static void commandInterpreter(void)
{
    char *argv[MAX_ARGS];
    uint32 argc;
    char *sessionID;

    while(1) {
        sessionID = coStartResponse();
        if(switchSession(sessionID)) {
            coPrintf("For help, enter the 'help' command\n");
        } else {
            readLine(stdin);
            argc = parseArguments(meLineBuffer, argv);
            meCurrentTime = time(NULL);
            if(!processCommand(argc, argv)) {
                coCompleteResponse();
                return;
            }
        }
        coPrintf("> ");
        coCompleteResponse();
    }
}

/*--------------------------------------------------------------------------------------------------
  Just switch to the user.
--------------------------------------------------------------------------------------------------*/
static void switchUser(
    char *userName)
{
    utSym sym = utSymCreate(userName);
    meUser user = meRootFindUser(meTheRoot, sym);

    if(user == meUserNull) {
        coPrintf("Unable to find user %s\n", userName);
    } else {
        if(meCurrentUser != meUserNull) {
            meUserSetLoggedIn(meCurrentUser, false);
        }
        meUserSetLoggedIn(user, true);
        meCurrentUser = user;
    }
}

/*--------------------------------------------------------------------------------------------------
  Execute commands in the file.
--------------------------------------------------------------------------------------------------*/
static void processFileCommands(
    char *fileName)
{
    FILE *file = fopen(fileName, "r");
    struct tm tm;
    char *argv[MAX_ARGS];
    char *p;
    uint32 argc;

    if(file == NULL) {
        coPrintf("Unable to open file %s for reading.\n", fileName);
        exit(1);
    }
    while(readLine(file)) {
        argc = parseArguments(meLineBuffer, argv);
        if(argc > 1) {
            p = strptime(argv[0], "%x-%X", &tm);
            if(p == NULL || *p != '\0') {
                coPrintf("Invalid date format.  Use MM/DD/YY.\n");
                exit(1);
            }
            meCurrentTime = mktime(&tm);
            if(argv[1][0] == ':') {
                switchUser(argv[1] + 1);
                processCommand(argc - 2, argv + 2);
            } else {
                processCommand(argc - 1, argv + 1);
            }
        }
    }
    fclose(file);
    meCurrentUser = meUserNull;
}

/*--------------------------------------------------------------------------------------------------
  This is the actual main routine.
--------------------------------------------------------------------------------------------------*/
int main(
    int argc,
    char **argv)
{
    FILE *databaseFile;

    if(argc > 2) {
        printf("Usage: %s [fileSocketPath]\n", argv[0]);
        exit(1);
    }
    utStart();
    meDatabaseStart();
    meTheRoot = meRootAlloc();
    meCurrentUser = meUserNull;
    meCurrentSession = meSessionNull;
    meLineSize = 42;
    meLineBuffer = utNewA(char, meLineSize);
    if(utFileExists(DATABASE_FILE)) {
        databaseFile = fopen(DATABASE_FILE, "r");
        utLoadTextDatabase(databaseFile);
        fclose(databaseFile);
    }
    if(utFileExists(LOG_FILE)) {
        meProcessingLogFile = true;
        meLogFile = NULL;
        processFileCommands(LOG_FILE);
    }
    meProcessingLogFile = false;
    meLogFile = fopen(LOG_FILE, "a");
    if(argc == 2) {
        coStartServer(argv[1]);
    }
    commandInterpreter();
    if(argc == 2) {
        coStopServer();
    }
    meDatabaseStop();
    fclose(meLogFile);
    utFree(meLineBuffer);
    utStop(false);
    return 0;
}
