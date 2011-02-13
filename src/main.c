/*--------------------------------------------------------------------------------------------------
  This is the main prgram for editing all data at ShareALot.org.  It usese Command Serializer
  from comserial.sourceforge.net inorder to communicate with multiple users at once.  PHP scripts
  are then used to glue into Apache.
--------------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cgiutil.h"
#include "sha256.h"
#include "sadatabase.h"
#include "comserver.h"

#define MAX_ARGS 10
#define NOUNCE_LENGTH 32
#define MIN_RATE 10
#define MAX_RATE 200
#define NEW_USER_DONATION 889
#define INITIAL_BALANCE 1000
#define LOG_FILE "sharealot.log"
#define OLD_LOG_FILE "sharealot.log.old"
#define DATABASE_FILE "sharealot.db"

saRoot saTheRoot;
static char *saLineBuffer;
static uint32 saLineSize;
static saSession saCurrentSession;
static saUser saCurrentUser;
static saRegion saCurrentRegion;
static FILE *saLogFile;
static bool saProcessingLogFile;
static uint64 saCurrentTime;

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
    char *command = utSprintf("echo %s | mail -a 'From: admin@sharealot.org' -s %s %s",
        escapeString(message), escapeString(subject), email);

    system(command);
}

/*--------------------------------------------------------------------------------------------------
  Send mail to a user.  Note that users should not be able to access this, as we don't validate
  the formats of parameters.  Single quotes are not allowed in the message.
--------------------------------------------------------------------------------------------------*/
static void mailPost(
    saUser user,
    saPost post)
{
    saThread thread = saPostGetThread(post);

    if(saProcessingLogFile) {
        return;
    }
    sendMail(utSymGetName(saUserGetEmail(user)), saThreadGetSubject(thread),
        saPostGetMessage(post));
}

/*--------------------------------------------------------------------------------------------------
  Send an announcement to a region, or globally.
--------------------------------------------------------------------------------------------------*/
static void sendAnnouncement(
    saAnnouncement announcement)
{
    saUser sender = saAnnouncementGetUser(announcement);
    saUser user;
    char *subject = saAnnouncementGetSubject(announcement);
    char *message = saAnnouncementGetMessage(announcement);

    if(saUserSupremeLeader(sender)) {
        saForeachRootUser(saTheRoot, user) {
            sendMail(utSymGetName(saUserGetEmail(user)), subject, message);
        } saEndRootUser;
    } else {
        saForeachRegionUser(saUserGetRegion(sender), user) {
            sendMail(utSymGetName(saUserGetEmail(user)), subject, message);
        } saEndRegionUser;
    }
}

/*--------------------------------------------------------------------------------------------------
  Destroy the session.
--------------------------------------------------------------------------------------------------*/
static void endSession(
    char *sessionID)
{
    saSession session = saRootFindSession(saTheRoot, utSymCreate(sessionID));

    if(session != saSessionNull) {
        saSessionDestroy(session);
        if(session == saCurrentSession) {
            saCurrentSession = saSessionNull;
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
        if(saProcessingLogFile) {
            c = getc(file);
        } else {
            c = coGetc();
        }
        if(c == EOF) {
            return false;
        }
    } utWhile(c != '\n') {
        if(linePosition + 1 == saLineSize) {
            saLineSize <<= 1;
            utResizeArray(saLineBuffer, saLineSize);
        }
        saLineBuffer[linePosition++] = c;
    } utRepeat;
    saLineBuffer[linePosition] = '\0';
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

    if(saLogFile == NULL) {
        return;
    }
    tm = localtime(&t);
    strftime(timeBuf, 100, "%x-%X", tm);
    fprintf(saLogFile, "%s ", timeBuf);
    if(saCurrentUser != saUserNull) {
        fprintf(saLogFile, ":%s ", cgiEncode(saUserGetName(saCurrentUser)));
    }
    for(xArg = 0; xArg < argc; xArg++) {
        if(xArg != 0) {
            fprintf(saLogFile, " ");
        }
        encodedArg = cgiEncode(argv[xArg]);
        fprintf(saLogFile, "%s", encodedArg);
    }
    fprintf(saLogFile, "\n");
    fflush(saLogFile);
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
            "    send userName amount note - send stars to an account\n"
            "    transactions [fromDate [toDate]] - list transactions\n"
            "    listRegions\n"
            "    chooseRegion region\n"
            "    list [cateory]\n"
            "    show listingID\n"
            "    newListing (offered|wanted) category rate title description\n"
            "    editListing listingID (offered|wanted) category rate title description\n"
            "    deleteListing listingID\n"
            "For manipulating accounts:\n"
            "    newUser userName region email password\n"
            "    newCharity userName region email password\n"
            "    updateAccount userName region email oldPassword [password]\n"
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
            "    createRegion region\n"
            "    deleteRegion region\n"
            "    renameRegion oldName newName\n"
            "    setRegionManager userName (true/false)\n"
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
            "Send stars from your account to someone else.  This is how you reward people\n"
            "for their services to you.  The amount should be whatever you agreed to.  You\n"
            " must make a note to yourself to remind you what this transaction is for.\n");
    } else if(!strcmp(command, "transactions")) {
        coPrintf("transactions [fromDate [toDate]]\n"
            "List transactions in your account between the dates listed (inclusive).  Dates\n"
            "are in MM/DD/YY format.\n");
    } else if(!strcmp(command, "listRegions")) {
        coPrintf("listRegions\n"
            "List regions.\n");
    } else if(!strcmp(command, "chooseRegion")) {
        coPrintf("chooseRegion region\n"
            "Select a region.  You must do this to view listings.\n");
    } else if(!strcmp(command, "list")) {
        coPrintf("list [cateory]\n"
            "List all categories, or the listings in a category.\n");
    } else if(!strcmp(command, "show")) {
        coPrintf("show listingID\n"
            "Show details on a specific listing.  The listingID is shown by the\n"
            "list command.\n");
    } else if(!strcmp(command, "newUser")) {
        coPrintf("newUser userName region email password\n"
            "Create a new unvalidated user.  The user should receive an e-mail with\n"
            "a validation key.\n");
    } else if(!strcmp(command, "newCharity")) {
        coPrintf("newCharity charityName region email password\n"
            "Create a new charity, which is a special kind of user, representing a\n"
            "participating charity.  The user should receive an e-mail with\n"
            "a validation key.\n");
    } else if(!strcmp(command, "updateAccount")) {
        coPrintf("updateAccount userName region email oldPassword [password]\n"
            "Change account settings.  If no new password is provided, keep the old one.\n");
    } else if(!strcmp(command, "accountSettings")) {
        coPrintf("accountSettings\n"
            "List account settings.\n");
    } else if(!strcmp(command, "resetPassword")) {
        coPrintf("resetPassword userName\n"
            "Reset your password to a random string.  E-mail the new password to you.\n");
    } else if(!strcmp(command, "joinCharity")) {
        coPrintf("joinCharity charityName\n"
            "Join a charity.  You must join at least one charity to validate your account.\n"
            "When you validate your account, 1000 stars will be distributed among your\n"
            "chosen charities.\n");
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
            "Announce a message to your community.  If you are a region leader, announce\n"
            "to everyone in your region.  The supreme leader can announce globally.\n");
    } else if(!strcmp(command, "listAnnouncements")) {
        coPrintf("listAnnouncements\n"
            "List announcements in your region.\n");
    } else if(!strcmp(command, "showAnnouncement")) {
        coPrintf("showAnnouncement announcementID\n"
            "Show an announcement.\n");
    } else if(!strcmp(command, "newListing")) {
        coPrintf("newListing (offered|wanted) category rate title description\n"
            "Create a new listing for a service you offer or want in a specific category.\n"
            "The rate is in stars per hour unless you start with =, in which case\n"
            "It's fixed price.\n");
    } else if(!strcmp(command, "editListing")) {
        coPrintf("editListing listintID (offered|wanted) category rate title description\n"
            "Edit an existing listing.\n");
    } else if(!strcmp(command, "deleteListing")) {
        coPrintf("deleteListing listingID\n"
            "Delete a service you offered or wanted.  The listingID is reported by\n"
            "the list command.\n");
    } else if(!strcmp(command, "createCategory")) {
        coPrintf("createCategory category\n"
            "Create a new category.  Only region managers can do this.\n");
    } else if(!strcmp(command, "deleteCategory")) {
        coPrintf("deleteCategory category\n"
            "Delete a category.  Only region managers can do this.\n");
    } else if(!strcmp(command, "renameCategory")) {
        coPrintf("renameCategory oldName newName\n"
            "Rename a category.  Only region managers can do this.\n");
    } else if(!strcmp(command, "createRegion")) {
        coPrintf("createRegion region\n"
            "Create a new region.  Only the supreme leader can do this.\n");
    } else if(!strcmp(command, "deleteRegion")) {
        coPrintf("deleteRegion region\n"
            "Destroy a region.  Only the supreme leader can do this.\n");
    } else if(!strcmp(command, "renameRegion")) {
        coPrintf("renameRegion oldName newName\n"
            "Rename a region.  Only the supreme leader can do this.\n");
    } else if(!strcmp(command, "setRegionManager")) {
        coPrintf("setRegionManager userName (true/false)\n"
            "Set the region manager flag on a user.  Only the supreme leader can do this.\n");
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
    uint32 unicorns = balance/1000;
    uint32 butterflyBalance = balance - 1000*unicorns;
    uint32 butterflies = butterflyBalance/100;
    uint32 moonBalance = butterflyBalance - 100*butterflies;
    uint32 moons = moonBalance/10;
    uint32 stars = moonBalance - 10*moons;
    static char buf[sizeof("xxxxxxxxxx unicorns, x butterflies, x stars")];
    char *p = buf;

    if(unicorns > 0) {
        if(unicorns == 1) {
            p += sprintf(p, "%u unicorn", unicorns);
        } else {
            p += sprintf(p, "%u unicorns", unicorns);
        }
    }
    if(butterflies > 0) {
        if(p != buf) {
            *p++ = ',';
            *p++ = ' ';
        }
        if(butterflies == 1) {
            p += sprintf(p, "%u butterfly", butterflies);
        } else {
            p += sprintf(p, "%u butterflies", butterflies);
        }
    }
    if(moons > 0) {
        if(p != buf) {
            *p++ = ',';
            *p++ = ' ';
        }
        if(moons == 1) {
            p += sprintf(p, "%u moon", moons);
        } else {
            p += sprintf(p, "%u moons", moons);
        }
    }
    if(stars > 0) {
        if(p != buf) {
            *p++ = ',';
            *p++ = ' ';
        }
        if(stars == 1) {
            p += sprintf(p, "%u star", stars);
        } else {
            p += sprintf(p, "%u stars", stars);
        }
    }
    if(p == buf) {
        strcpy(buf, "no stars");
    }
    return buf;
}

/*--------------------------------------------------------------------------------------------------
  Log out.
--------------------------------------------------------------------------------------------------*/
static void logout(void)
{
    saUserSetLoggedIn(saCurrentUser, false);
    saUserSetSession(saCurrentUser, saSessionNull);
    saSessionSetUser(saCurrentSession, saUserNull);
    saCurrentUser = saUserNull;
}

/*--------------------------------------------------------------------------------------------------
  Process a login command.
--------------------------------------------------------------------------------------------------*/
static void processLoginCommand(
    uint32 argc,
    char **argv)
{
    saRegion region;
    saUser user;
    char *hashedPassword;
    bool supremeLeader = false, regionManager = false;
    bool passwdOk;

    if(saCurrentUser != saUserNull) {
        supremeLeader = saUserSupremeLeader(saCurrentUser);
        regionManager = saUserRegionManager(saCurrentUser);
        region = saUserGetRegion(saCurrentUser);
    }
    if(argc != 3 && !(argc == 2 && (supremeLeader || regionManager))) {
        coPrintf("Login requires a userName and a password.\n");
        return;
    } 
    user = saRootFindUser(saTheRoot, utSymCreate(utStringToLowerCase(argv[1])));
    if(user != saUserNull) {
        passwdOk = supremeLeader || (regionManager && region == saUserGetRegion(user) &&
            !saUserSupremeLeader(user));
        if(!passwdOk) {
            hashedPassword = hashPassword(saUserGetNounce(user), argv[2]);
            passwdOk = !memcmp(hashedPassword, saUserGetHashedPassword(user), NOUNCE_LENGTH);
        }
        if(passwdOk) {
            if(saCurrentUser != saUserNull) {
                logout();
            }
            saUserSetLoggedIn(user, true);
            saCurrentUser = user;
            saCurrentRegion = saUserGetRegion(user);
            saSessionSetUser(saCurrentSession, user);
            saUserSetSession(user, saCurrentSession);
            coPrintf("Login successful.  You have %s.\n", findBalance(saUserGetBalance(user)));
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
    if(saCurrentUser == saUserNull) {
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
    if(saCurrentUser == saUserNull) {
        coPrintf("Not logged in.\n");
    } else {
        coPrintf("Logged in as user %s.", cgiEncode(saUserGetShownName(saCurrentUser)));
        if(saUserSupremeLeader(saCurrentUser)) {
            coPrintf("  You are the supreme leader.\n");
        } else if(saUserRegionManager(saCurrentUser)) {
            coPrintf("  You manage region %s.\n", cgiEncode(saRegionGetName(saUserGetRegion(saCurrentUser))));
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
    if(saCurrentUser == saUserNull) {
        coPrintf("Not currently logged in.\n");
    } else {
        coPrintf("%s\n", findBalance(saUserGetBalance(saCurrentUser)));
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
  Process a send command to transfer stars.
--------------------------------------------------------------------------------------------------*/
static void processSendCommand(
    uint32 argc,
    char **argv)
{
    saUser otherUser;
    saTransaction transaction;
    int64 amount, userBalance, otherBalance;
    uint64 t = time(NULL);
    char *endPtr;
    char userBalanceBuf[42], otherBalanceBuf[42];

    if(saCurrentUser == saUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(saProcessingLogFile? argc != 6 : argc != 4) {
        printHelp(argv[0]);
        return;
    }
    if(saCurrentUser == saUserNull) {
        coPrintf("You must be logged in to send stars.\n");
        return;
    }
    otherUser = saRootFindUser(saTheRoot, utSymCreate(argv[1]));
    if(otherUser == saUserNull) {
        coPrintf("User %s not found\n", argv[1]);
        return;
    }
    if(!readInt(argv[2], &amount, true)) {
        return;
    }
    if(amount > saUserGetBalance(saCurrentUser)) {
        coPrintf("You do not have enough stars.\n");
        return;
    }
    transaction = saTransactionAlloc();
    saTransactionSetAmount(transaction, amount);
    saTransactionSetDescription(transaction, argv[3], strlen(argv[3]) + 1);
    saTransactionSetDate(transaction, t);
    saUserAppendOutTransaction(saCurrentUser, transaction);
    saUserAppendInTransaction(otherUser, transaction);
    saUserSetBalance(saCurrentUser, saUserGetBalance(saCurrentUser) - amount);
    saUserSetBalance(otherUser, saUserGetBalance(otherUser) + amount);
    if(!saProcessingLogFile) {
        sprintf(userBalanceBuf, "%u", saUserGetBalance(saCurrentUser));
        argv[4] = userBalanceBuf;
        sprintf(otherBalanceBuf, "%u", saUserGetBalance(otherUser));
        argv[5] = otherBalanceBuf;
        logCommand(6, argv);
    } else {
        if(!readInt(argv[4], &userBalance, true) ||
                userBalance != saUserGetBalance(saCurrentUser)) {
            coPrintf("Invalid user balance for user %s.\n", saUserGetName(saCurrentUser));
            exit(1);
        }
        if(!readInt(argv[5], &otherBalance, true) ||
                otherBalance != saUserGetBalance(otherUser)) {
            coPrintf("Invalid user balance for user %s.\n", saUserGetName(otherUser));
            exit(1);
        }
    }
    coPrintf("Transaction completed successfully.  Your have %s.\n",
        findBalance(saUserGetBalance(saCurrentUser)));
}

/*--------------------------------------------------------------------------------------------------
  A callback function for qsort for sorting transactions by date.
--------------------------------------------------------------------------------------------------*/
static int compareTransactions(
    const void *transaction1Ptr,
    const void *transaction2Ptr)
{
    saTransaction transaction1 = *(saTransaction *)transaction1Ptr;
    saTransaction transaction2 = *(saTransaction *)transaction2Ptr;
    uint64 date1 = saTransactionGetDate(transaction1);
    uint64 date2 = saTransactionGetDate(transaction2);

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
    saTransaction transaction;
    saTransactionArray tarray;
    uint64 fromDate = 0;
    uint64 toDate = UINT64_MAX;
    uint64 date;
    struct tm tm;
    struct tm *tmPtr;
    time_t t;
    char *p;

    if(saCurrentUser == saUserNull) {
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
    tarray = saTransactionArrayAlloc();
    saForeachUserOutTransaction(saCurrentUser, transaction) {
        date = saTransactionGetDate(transaction);
        if(date >= fromDate && date <= toDate) {
            saTransactionArrayAppendTransaction(tarray, transaction);
        }
    } saEndUserOutTransaction;
    saForeachUserInTransaction(saCurrentUser, transaction) {
        date = saTransactionGetDate(transaction);
        if(date >= fromDate && date <= toDate) {
            saTransactionArrayAppendTransaction(tarray, transaction);
        }
    } saEndUserInTransaction;
    qsort(saTransactionArrayGetTransactions(tarray), saTransactionArrayGetUsedTransaction(tarray),
        sizeof(saTransaction), compareTransactions);
    saForeachTransactionArrayTransaction(tarray, transaction) {
        t = saTransactionGetDate(transaction);
        tmPtr = localtime(&t);
        if(saTransactionGetFromUser(transaction) == saCurrentUser) {
            coPrintf("%02d/%02d/%02d you sent %s %u - %s\n", tmPtr->tm_mon + 1, tmPtr->tm_mday,
                tmPtr->tm_year - 100, saUserGetShownName(saTransactionGetToUser(transaction)),
                saTransactionGetAmount(transaction), saTransactionGetDescription(transaction));
        } else {
            coPrintf("%02d/%02d/%02d %s sent you %u - %s\n", tmPtr->tm_mon + 1, tmPtr->tm_mday,
                tmPtr->tm_year - 100, saUserGetShownName(saTransactionGetFromUser(transaction)),
                saTransactionGetAmount(transaction), saTransactionGetDescription(transaction));
        }
    } saEndTransactionArrayTransaction;
    saTransactionArrayFree(tarray);
}

/*--------------------------------------------------------------------------------------------------
  Process a listRegions command.
--------------------------------------------------------------------------------------------------*/
static void processListRegionsCommand(
    uint32 argc,
    char **argv)
{
    saRegion region;
    bool hasItems = false;

    if(argc != 1) {
        printHelp(argv[0]);
        return;
    }
    saForeachRootRegion(saTheRoot, region) {
        coPrintf("%s (%u)\n", saRegionGetName(region), saRegionGetNumListings(region));
        hasItems = true;
    } saEndRootRegion;
    if(!hasItems) {
        coPrintf("There are no regions.\n");
    }
}

/*--------------------------------------------------------------------------------------------------
  Process a chooseRegions command.
--------------------------------------------------------------------------------------------------*/
static void processChooseRegionCommand(
    uint32 argc,
    char **argv)
{
    saRegion region;
    bool hasItems = false;

    if(argc != 2) {
        printHelp(argv[0]);
        return;
    }
    region = saRootFindRegion(saTheRoot, utSymCreate(argv[1]));
    if(region == saRegionNull) {
        coPrintf("There is no region called '%s'.\n", argv[1]);
        return;
    }
    saCurrentRegion = region;
}

/*--------------------------------------------------------------------------------------------------
  Process a list command.
--------------------------------------------------------------------------------------------------*/
static void processListCommand(
    uint32 argc,
    char **argv)
{
    saCategory category = saCategoryNull;
    saListing listing;
    bool hasItems = false;

    if(argc > 2) {
        printHelp(argv[0]);
        return;
    }
    if(saCurrentRegion == saRegionNull) {
        coPrintf("Please choose a region first.\n");
        return;
    }
    if(argc == 2) {
        category = saRootFindCategory(saTheRoot, utSymCreate(argv[1]));
        if(category == saCategoryNull) {
            coPrintf("Invalid category '%s'\n", argv[1]);
            return;
        }
    }
    if(category != saCategoryNull) {
        saForeachCategoryListing(category, listing) {
            coPrintf("%s listingID=%llu\n", saListingGetTitle(listing), saListingGetID(listing));
            hasItems = true;
        } saEndCategoryListing;
        if(!hasItems) {
            coPrintf("There are no listings in this category.\n");
        }
    } else {
        saForeachRootCategory(saTheRoot, category) {
            //if(saCategoryGetNumListings(category) > 0) {
                coPrintf("%s (%u)\n", saCategoryGetName(category),
                    saCategoryGetNumListings(category));
                hasItems = true;
            //}
        } saEndRootCategory;
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
    saListing listing;
    uint64 listingID;

    if(argc != 2) {
        printHelp(argv[0]);
        return;
    }
    if(!readInt(argv[1], &listingID, true)) {
        return;
    }
    listing = saRootFindListing(saTheRoot, listingID);
    if(listing == saListingNull) {
        coPrintf("Invalid listingID\n");
        return;
    }
    coPrintf("%s: %s %s%u", saListingOffered(listing)? "Offered" : "Wanted",
        cgiEncode(saListingGetTitle(listing)),
        saListingFixedPrice(listing)? "cost=" : "rate=", saListingGetRate(listing));
    coPrintf(" user=%s",
        cgiEncode(saUserGetShownName(saListingGetUser(listing))));
    coPrintf(" category=%s listingID=%llu\n%s\n",
        cgiEncode(saCategoryGetName(saListingGetCategory(listing))), listingID,
        saListingGetDescription(listing));
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
static saUser createNewUser(
    uint32 argc,
    char **argv)
{
    saRegion region;
    saUser user;
    utSym userSym, regionSym, emailSym;
    char *password, *nounce, *validationKey;
    char *hashedPassword;
    char encodedHash[SHA256_DIGEST_SIZE*3+1];

    if(saProcessingLogFile? argc != 7 : argc != 5) {
        printHelp(argv[0]);
        return saUserNull;
    }
    userSym = utSymCreate(utStringToLowerCase(argv[1]));
    emailSym = utSymCreate(argv[3]);
    if(!emailValid(argv[3])) {
        coPrintf("Email %s is not in a format we recognize.\n", argv[3]);
        return;
    }
    user = saRootFindUser(saTheRoot, userSym);
    if(user != saUserNull) {
        coPrintf("User %s already exists.\n", argv[1]);
        return saUserNull;
    }
    if(saRootFindByEmailUser(saTheRoot, emailSym) != saUserNull) {
        coPrintf("E-mail address %s is already in use.\n", argv[3]);
        return saUserNull;
    }
    regionSym = utSymCreate(argv[2]);
    region = saRootFindRegion(saTheRoot, regionSym);
    if(region == saRegionNull) {
        if(saRootGetFirstUser(saTheRoot) != saUserNull &&
                (saCurrentUser == saUserNull || !saUserSupremeLeader(saCurrentUser))) {
            coPrintf("Region %s does not exist.  Contact ShareALot.org to create a new one.\n",
                argv[2]);
            return;
        }
        region = saRegionAlloc();
        saRegionSetSym(region, regionSym);
        saRootInsertRegion(saTheRoot, region);
    }
    user = saUserAlloc();
    saUserSetSym(user, userSym);
    saUserSetShownName(user, argv[1], strlen(argv[1]) + 1);
    saUserSetEmail(user, emailSym);
    saRegionAppendUser(region, user);
    saUserSetBalance(user, INITIAL_BALANCE);
    if(!saProcessingLogFile) {
        password = argv[4];
        nounce = cgiGenerateRandomID(NOUNCE_LENGTH);
        saUserSetNounce(user, nounce, NOUNCE_LENGTH + 1);
        hashedPassword = hashPassword(saUserGetNounce(user), password);
        saUserSetHashedPassword(user, hashedPassword, SHA256_DIGEST_SIZE);
        strcpy(encodedHash, cgiEncodeArray(hashedPassword, SHA256_DIGEST_SIZE));
        validationKey = cgiGenerateRandomID(NOUNCE_LENGTH);
        saUserSetValidationKey(user, validationKey, NOUNCE_LENGTH + 1);
        argv[4] = saUserGetNounce(user);
        argv[5] = encodedHash;
        argv[6] = saUserGetValidationKey(user);
        sendMail(argv[3], "Welcome to ShareALot.org!", 
            utSprintf("Dear %s,\n\n"
                "Thank you for registering at ShareALot.org.  To complete your registration, "
                "please visit this url:\n\n"
                "    http://www.sharealot.org/validate.php?user=%s&key=%s\n\n"
                "Your user name is: %s\n"
                "Your password is: %s\n",
                saUserGetShownName(user), cgiEncode(saUserGetName(user)), validationKey,
                saUserGetShownName(user), password));
        logCommand(7, argv);
    } else {
        nounce = argv[4];
        saUserSetNounce(user, nounce, NOUNCE_LENGTH + 1);
        cgiUnencode(argv[5]);
        saUserSetHashedPassword(user, argv[5], SHA256_DIGEST_SIZE);
        validationKey = argv[6];
        saUserSetValidationKey(user, validationKey, NOUNCE_LENGTH + 1);
    }
    if(saRootGetFirstUser(saTheRoot) == saUserNull) {
        saUserSetSupremeLeader(user, true);
        saUserSetRegionManager(user, true);
        coPrintf("User %s is the supreme leader.\n", saUserGetShownName(user));
    }
    saRootAppendUser(saTheRoot, user);
    saRootInsertByEmailUser(saTheRoot, user);
    return user;
}

/*--------------------------------------------------------------------------------------------------
  Join a charity.
--------------------------------------------------------------------------------------------------*/
static void processJoinCharityCommand(
    uint32 argc,
    char **argv)
{
    saUser user;
    saCharity charity;
    saMembership membership;

    if(saCurrentUser == saUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(argc != 2) {
        printHelp(argv[0]);
        return;
    }
    user = saRootFindUser(saTheRoot, utSymCreate(argv[1]));
    if(user == saUserNull) {
        coPrintf("No charity named '%s' found.\n", argv[1]);
        return;
    }
    charity = saUserGetCharity(user);
    if(charity == saCharityNull) {
        coPrintf("User '%s' is not a charity.\n", argv[1]);
        return;
    }
    if(saUserFindMembership(saCurrentUser, charity) != saMembershipNull) {
        coPrintf("Already belong to charity %s\n", argv[1]);
        return;
    }
    membership = saMembershipAlloc();
    saCharityAppendMembership(charity, membership);
    saUserAppendMembership(saCurrentUser, membership);
    logCommand(argc, argv);
    coPrintf("%s joined charity %s successfully.\n", saUserGetShownName(saCurrentUser), argv[1]);
}

/*--------------------------------------------------------------------------------------------------
  Create a new user.
--------------------------------------------------------------------------------------------------*/
static void processNewUserCommand(
    uint32 argc,
    char **argv)
{
    saUser user = createNewUser(argc, argv);
    saUser savedUser = saCurrentUser;
    char *joinCharityCommand[2] = {"joinCharity", "sharealot.org"};

    if(user != saUserNull) {
        if(!saProcessingLogFile) {
            // This temp hack is until we have more than one charity
            saCurrentUser = user;
            processJoinCharityCommand(2, joinCharityCommand);
            saCurrentUser = savedUser;
        }
        coPrintf("New user %s created with validation key %s.\n", saUserGetShownName(user),
            saUserGetValidationKey(user));
    }
}

/*--------------------------------------------------------------------------------------------------
  Create a new charity.
--------------------------------------------------------------------------------------------------*/
static void processNewCharityCommand(
    uint32 argc,
    char **argv)
{
    saCharity charity;
    saUser user = createNewUser(argc, argv);

    if(user == saUserNull) {
        return;
    }
    charity = saCharityAlloc();
    saRootAppendCharity(saTheRoot, charity);
    saUserInsertCharity(user, charity);
    coPrintf("New charity %s created with validation key %s.\n", saUserGetShownName(user),
        saUserGetValidationKey(user));
}

/*--------------------------------------------------------------------------------------------------
  Update account settings.
--------------------------------------------------------------------------------------------------*/
static void processUpdateAccountCommand(
    uint32 argc,
    char **argv)
{
    saRegion region;
    saUser user;
    utSym userSym, regionSym, emailSym;
    char *email, *password, *nounce, *validationKey;
    char *hashedPassword;
    char encodedHash[SHA256_DIGEST_SIZE*3+1];

    if(saCurrentUser == saUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(saProcessingLogFile? argc != 4 && argc != 6 : argc != 5 && argc != 6) {
        printHelp(argv[0]);
        return;
    }
    if(!saProcessingLogFile) {
        hashedPassword = hashPassword(saUserGetNounce(saCurrentUser), argv[4]);
        if(memcmp(hashedPassword, saUserGetHashedPassword(saCurrentUser), NOUNCE_LENGTH)) {
            coPrintf("Incorrect password.\n");
            return;
        }
    }
    userSym = utSymCreate(utStringToLowerCase(argv[1]));
    emailSym = utSymCreate(argv[3]);
    if(!emailValid(argv[3])) {
        coPrintf("Email %s is not in a format we recognize.\n", argv[3]);
        return;
    }
    user = saRootFindUser(saTheRoot, userSym);
    if(user != saUserNull && user != saCurrentUser) {
        coPrintf("User %s already exists.\n", argv[1]);
        return;
    }
    user = saRootFindByEmailUser(saTheRoot, emailSym);
    if(user != saUserNull && user != saCurrentUser) {
        coPrintf("E-mail address %s is already in use.\n", argv[3]);
        return;
    }
    regionSym = utSymCreate(argv[2]);
    region = saRootFindRegion(saTheRoot, regionSym);
    if(region == saRegionNull) {
        if(saRootGetFirstUser(saTheRoot) != saUserNull &&
                (saCurrentUser == saUserNull || !saUserSupremeLeader(saCurrentUser))) {
            coPrintf("Region %s does not exist.  Contact ShareALot.org to create a new one.\n",
                argv[2]);
            return;
        }
        region = saRegionAlloc();
        saRegionSetSym(region, regionSym);
        saRootInsertRegion(saTheRoot, region);
    }
    if(!saProcessingLogFile) {
        if(argc == 6) {
            password = argv[5];
            nounce = cgiGenerateRandomID(NOUNCE_LENGTH);
            saUserSetNounce(saCurrentUser, nounce, NOUNCE_LENGTH + 1);
            hashedPassword = hashPassword(saUserGetNounce(saCurrentUser), password);
            saUserSetHashedPassword(saCurrentUser, hashedPassword, SHA256_DIGEST_SIZE);
            strcpy(encodedHash, cgiEncodeArray(hashedPassword, SHA256_DIGEST_SIZE));
            argv[4] = saUserGetNounce(saCurrentUser);
            argv[5] = encodedHash;
            argc = 6;
        }
        logCommand(argc, argv);
    } else if(argc == 6) {
        nounce = argv[4];
        saUserSetNounce(saCurrentUser, nounce, NOUNCE_LENGTH + 1);
        cgiUnencode(argv[5]);
        saUserSetHashedPassword(saCurrentUser, argv[5], SHA256_DIGEST_SIZE);
    }
    if(saUserGetSym(saCurrentUser) != userSym) {
        saRootRenameUser(saTheRoot, saCurrentUser, userSym);
    }
    saUserSetShownName(saCurrentUser, argv[1], strlen(argv[1]) + 1);
    saRootRemoveByEmailUser(saTheRoot, saCurrentUser);
    saUserSetEmail(saCurrentUser, emailSym);
    saRootInsertByEmailUser(saTheRoot, saCurrentUser);
    saRegionRemoveUser(saUserGetRegion(saCurrentUser), saCurrentUser);
    saRegionAppendUser(region, saCurrentUser);
    coPrintf("Account updated succesfully.\n");
}

/*--------------------------------------------------------------------------------------------------
  Show account settings.
--------------------------------------------------------------------------------------------------*/
static void processAccountSettingsCommand(
    uint32 argc,
    char **argv)
{
    if(saCurrentUser == saUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    coPrintf("User %s, email %s", cgiEncode(saUserGetShownName(saCurrentUser)),
        utSymGetName(saUserGetEmail(saCurrentUser)));
    coPrintf(", region %s\n", cgiEncode(saRegionGetName(saUserGetRegion(saCurrentUser))));
}

/*--------------------------------------------------------------------------------------------------
  Reset a password.
--------------------------------------------------------------------------------------------------*/
static void processResetPasswordCommand(
    uint32 argc,
    char **argv)
{ 
    saUser user;
    char *nounce, *password, *hashedPassword;
    char encodedHash[SHA256_DIGEST_SIZE*3+1];

    if(saProcessingLogFile? argc!= 4 : argc != 2) {
        printHelp(argv[0]);
        return;
    }
    user = saRootFindUser(saTheRoot, utSymCreate(utStringToLowerCase(argv[1])));
    if(user == saUserNull) {
        coPrintf("User not found.\n");
        return;
    }
    if(!saProcessingLogFile) {
        nounce = cgiGenerateRandomID(NOUNCE_LENGTH);
        saUserSetNounce(user, nounce, NOUNCE_LENGTH + 1);
        password = cgiGenerateRandomID(8);
        hashedPassword = hashPassword(saUserGetNounce(user), password);
        saUserSetHashedPassword(user, hashedPassword, SHA256_DIGEST_SIZE);
        strcpy(encodedHash, cgiEncodeArray(hashedPassword, SHA256_DIGEST_SIZE));
        argv[2] = saUserGetNounce(user);
        argv[3] = encodedHash;
        logCommand(4, argv);
        sendMail(utSymGetName(saUserGetEmail(user)), "Your new ShareALot.org password",
            utSprintf("You have reset your password at ShareALot.org.  "
            "Your new password is:\n\npassword: %s\n", password));
    } else {
        saUserSetNounce(user, argv[2], NOUNCE_LENGTH + 1);
        cgiUnencode(argv[3]);
        saUserSetHashedPassword(user, argv[3], SHA256_DIGEST_SIZE);
    }
    coPrintf("Password reset for user %s.\n", saUserGetShownName(user));
}

/*--------------------------------------------------------------------------------------------------
  Just count the number of charities a user belongs to.
--------------------------------------------------------------------------------------------------*/
static uint32 countUserCharities(
    saUser user)
{
    saMembership membership;
    uint32 numCharities = 0;

    saForeachUserMembership(user, membership) {
        numCharities++;
    } saEndUserMembership;
    return numCharities;
}

/*--------------------------------------------------------------------------------------------------
  Donate stars to a charity.
--------------------------------------------------------------------------------------------------*/
static void donate(
    saCharity charity,
    saUser user,
    uint32 donation)
{
    saUser charityManager = saCharityGetUser(charity);
    uint32 balance = saUserGetBalance(user);

    if(balance < donation) {
        coPrintf("Not enough stars in your account.\n");
        return;
    }
    saUserSetBalance(user, balance - donation);
    saUserSetBalance(charityManager, saUserGetBalance(charityManager) + donation);
}

/*--------------------------------------------------------------------------------------------------
  Distribute stars too this user's charities as evenly as possible.
--------------------------------------------------------------------------------------------------*/
static void distributeToCharity(
    saUser user,
    uint32 numStars)
{
    saCharity charity;
    saMembership membership;
    uint32 numCharities = countUserCharities(user);
    uint32 amount = numStars/numCharities;
    uint32 remainder = numStars - (amount*numCharities);
    uint32 donation;

    saForeachUserMembership(user, membership) {
        charity = saMembershipGetCharity(membership);
        if(remainder > 0) {
            donation = amount + 1;
            remainder--;
        } else {
            donation = amount;
        }
        donate(charity, user, donation);
    } saEndUserMembership;
}

/*--------------------------------------------------------------------------------------------------
  Validate a new user.
--------------------------------------------------------------------------------------------------*/
static void processValidateUserCommand(
    uint32 argc,
    char **argv)
{
    saUser user;
    utSym userSym;
    char key[NOUNCE_LENGTH];

    if(argc != 3) {
        printHelp(argv[0]);
        return;
    }
    userSym = utSymCreate(utStringToLowerCase(argv[1]));
    user = saRootFindUser(saTheRoot, userSym);
    if(user == saUserNull) {
        coPrintf("Invalid user %s\n", argv[1]);
        return;
    }
    if(strlen(argv[2]) != NOUNCE_LENGTH) {
        coPrintf("Invalid validation key\n");
        return;
    }
    if(!saProcessingLogFile && memcmp(saUserGetValidationKey(user), argv[2], NOUNCE_LENGTH)) {
        coPrintf("Incorrect validation key.\n");
        return;
    }
    if(saUserGetCharity(user) == saCharityNull) {
        if(saUserGetFirstMembership(user) == saMembershipNull) {
            coPrintf("You must first join a charity before you can validate your account.\n");
            return;
        }
        distributeToCharity(user, NEW_USER_DONATION);
    }
    saUserSetValidated(user, true);
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
    saRegion region;

    if(saCurrentUser == saUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(saUserGetFirstInTransaction(saCurrentUser) != saTransactionNull) {
        coPrintf("Can't delete users with completed transactions.\n");
        return;
    }
    if(argc != 1) {
        printHelp(argv[0]);
        return;
    }
    region = saUserGetRegion(saCurrentUser);
    coPrintf("Account %s deleted.\n", saUserGetShownName(saCurrentUser));
    logCommand(argc, argv);
    saUserDestroy(saCurrentUser);
    if(saRegionGetFirstUser(region) == saUserNull) {
        saRegionDestroy(region);
        if(saCurrentRegion == region) {
            saCurrentRegion = saRootGetFirstRegion(saTheRoot);
        }
    }
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

    if(saCurrentUser == saUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(saProcessingLogFile? argc != 2 : argc != 3) {
        printHelp(argv[0]);
        return;
    }
    if(!saProcessingLogFile) {
        oldPassword = argv[1];
        newPassword = argv[2];
        hashedPassword = hashPassword(saUserGetNounce(saCurrentUser), oldPassword);
        if(memcmp(hashedPassword, saUserGetHashedPassword(saCurrentUser), SHA256_DIGEST_SIZE)) {
            coPrintf("Incorrect password.\n");
            return;
        }
        hashedPassword = hashPassword(saUserGetNounce(saCurrentUser), newPassword);
        saUserSetHashedPassword(saCurrentUser, hashedPassword, SHA256_DIGEST_SIZE);
        strcpy(encodedHash, cgiEncodeArray(hashedPassword, SHA256_DIGEST_SIZE));
        argv[1] = encodedHash;
        logCommand(2, argv);
    } else {
        cgiUnencode(argv[1]);
        saUserSetHashedPassword(saCurrentUser, argv[1], SHA256_DIGEST_SIZE);
    }
    coPrintf("Succesfully changed password.\n");
}

/*--------------------------------------------------------------------------------------------------
  Create a new post.
--------------------------------------------------------------------------------------------------*/
static saPost saPostCreate(
    saUser user,
    saThread thread,
    char *message)
{
    saPost post = saPostAlloc();
    uint32 postID = saRootGetNextPostID(saTheRoot);

    saPostSetID(post, postID);
    saRootSetNextPostID(saTheRoot, postID + 1);
    saPostSetMessage(post, message, strlen(message) + 1);
    saUserAppendPost(user, post);
    saThreadAppendPost(thread, post);
    saRootAppendPost(saTheRoot, post);
    saThreadSetNumPosts(thread, saThreadGetNumPosts(thread) + 1);
    return post;
}

/*--------------------------------------------------------------------------------------------------
  Find a subscription for the user and thread.
--------------------------------------------------------------------------------------------------*/
static saSubscription findSubscription(
    saUser user,
    saThread thread)
{
    saSubscription subscription;

    saForeachUserSubscription(user, subscription) {
        if(saSubscriptionGetThread(subscription) == thread) {
            return subscription;
        }
    } saEndUserSubscription;
    return saSubscriptionNull;
}

/*--------------------------------------------------------------------------------------------------
  Follow the thread, if we don't already.
--------------------------------------------------------------------------------------------------*/
static void followThread(
    saThread thread)
{
    saSubscription subscription = findSubscription(saCurrentUser, thread);

    if(subscription != saSubscriptionNull) {
        return;
    }
    subscription = saSubscriptionAlloc();
    saUserAppendSubscription(saCurrentUser, subscription);
    saThreadAppendSubscription(thread, subscription);
}

/*--------------------------------------------------------------------------------------------------
  Process a followThread command.
--------------------------------------------------------------------------------------------------*/
static void processFollowThreadCommand(
    uint32 argc,
    char **argv)
{
    saThread thread;
    int64 threadID;

    if(saCurrentUser == saUserNull) {
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
    thread = saRootFindThread(saTheRoot, threadID);
    if(thread == saThreadNull) {
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
    saThread thread;
    saSubscription subscription;
    int64 threadID;

    if(saCurrentUser == saUserNull) {
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
    thread = saRootFindThread(saTheRoot, threadID);
    if(thread == saThreadNull) {
        coPrintf("Invalid thread ID.\n");
        return;
    }
    subscription =  findSubscription(saCurrentUser, thread);
    if(subscription == saSubscriptionNull) {
        return;
    }
    saSubscriptionDestroy(subscription);
    logCommand(argc, argv);
}

/*--------------------------------------------------------------------------------------------------
  Process an announce command.
--------------------------------------------------------------------------------------------------*/
static void processAnnounceCommand(
    uint32 argc,
    char **argv)
{
    saAnnouncement announcement;

    if(saCurrentUser == saUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(argc != 3) {
        printHelp(argv[0]);
        return;
    }
    if(!saUserRegionManager(saCurrentUser) && !saUserSupremeLeader(saCurrentUser)) {
        coPrintf("You must be a region manager to post announcements.\n");
        return;
    }
    announcement = saAnnouncementAlloc();
    saAnnouncementSetID(announcement, saRootGetNextAnnouncementID(saTheRoot));
    saRootSetNextAnnouncementID(saTheRoot, saRootGetNextAnnouncementID(saTheRoot) + 1);
    saAnnouncementSetSubject(announcement, argv[1], strlen(argv[1]) + 1);
    saAnnouncementSetMessage(announcement, argv[2], strlen(argv[2]) + 1);
    saUserAppendAnnouncement(saCurrentUser, announcement);
    saRootAppendAnnouncement(saTheRoot, announcement);
    if(!saProcessingLogFile) {
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
    saAnnouncement announcement;
    saUser user;
    bool printedAnnouncement = false;

    if(argc != 1) {
        printHelp(argv[0]);
        return;
    }
    saForeachRootAnnouncement(saTheRoot, announcement) {
        user = saAnnouncementGetUser(announcement);
        if(saUserSupremeLeader(user) || saUserGetRegion(user) == saCurrentRegion) {
            coPrintf("%u %s\n", saAnnouncementGetID(announcement), cgiEncode(saAnnouncementGetSubject(announcement)));
            printedAnnouncement = true;
        }
    } saEndRootAnnouncement;
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
    saAnnouncement announcement;
    uint64 announcementID;

    if(argc != 2) {
        printHelp(argv[0]);
        return;
    }
    if(!readInt(argv[1], &announcementID, true)) {
        return;
    }
    announcement = saRootFindAnnouncement(saTheRoot, announcementID);
    if(announcement == saAnnouncementNull) {
        coPrintf("Invalid announcement ID.\n");
        return;
    }
    coPrintf("%s\n", saAnnouncementGetMessage(announcement));
}

/*--------------------------------------------------------------------------------------------------
  Process a new post command.
--------------------------------------------------------------------------------------------------*/
static void processNewPostCommand(
    uint32 argc,
    char **argv)
{
    saThread thread;
    saPost post;
    uint32 threadID;

    if(saCurrentUser == saUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(argc != 3) {
        printHelp(argv[0]);
        return;
    }
    thread = saThreadAlloc();
    threadID = saRootGetNextThreadID(saTheRoot);
    saThreadSetID(thread, threadID);
    saRootSetNextThreadID(saTheRoot, threadID + 1);
    saThreadSetSubject(thread, argv[1], strlen(argv[1]) + 1);
    saRootAppendThread(saTheRoot, thread);
    post = saPostCreate(saCurrentUser, thread, argv[2]);
    followThread(thread);
    logCommand(argc, argv);
    coPrintf("Created thread %u\n", saThreadGetID(thread));
}

/*--------------------------------------------------------------------------------------------------
  Process a list threads command.
--------------------------------------------------------------------------------------------------*/
static void processListThreadsCommand(
    uint32 argc,
    char **argv)
{
    saThread thread;

    if(argc != 1) {
        printHelp(argv[0]);
        return;
    }
    if(saRootGetFirstThread(saTheRoot) == saThreadNull) {
        coPrintf("No threads.\n");
        return;
    }
    saForeachRootThread(saTheRoot, thread) {
        if(saCurrentUser != saUserNull &&
                findSubscription(saCurrentUser, thread) != saSubscriptionNull) {
            coPrintf("+");
        }
        coPrintf("%u %s (%u)\n", saThreadGetID(thread), saThreadGetSubject(thread),
            saThreadGetNumPosts(thread));
    } saEndRootThread;
}

/*--------------------------------------------------------------------------------------------------
  Process a listPosts command.
--------------------------------------------------------------------------------------------------*/
processListPostsCommand(
    uint32 argc,
    char **argv)
{
    saThread thread;
    saPost post;
    uint64 threadID;

    if(argc != 2) {
        printHelp(argv[0]);
        return;
    }
    if(!readInt(argv[1], &threadID, true)) {
        return;
    }
    thread = saRootFindThread(saTheRoot, threadID);
    if(thread == saThreadNull) {
        coPrintf("Invalid thread ID.\n");
        return;
    }
    saForeachThreadPost(thread, post) {
        coPrintf("%u %s\n", saPostGetID(post), saUserGetShownName(saPostGetUser(post)));
    } saEndThreadPost;
}

/*--------------------------------------------------------------------------------------------------
  Process a showPost command.
--------------------------------------------------------------------------------------------------*/
static void processShowPostCommand(
    uint32 argc,
    char **argv)
{
    saPost post;
    uint64 postID;

    if(argc != 2) {
        printHelp(argv[0]);
        return;
    }
    if(!readInt(argv[1], &postID, true)) {
        return;
    }
    post = saRootFindPost(saTheRoot, postID);
    if(post == saPostNull) {
        coPrintf("Invalid post ID.\n");
        return;
    }
    coPrintf("%s\n", saPostGetMessage(post));
}

/*--------------------------------------------------------------------------------------------------
  Send this post 
--------------------------------------------------------------------------------------------------*/
static void sendFollowersPost(
    saPost post)
{
    saThread thread = saPostGetThread(post);
    saSubscription subscription;
    saUser user;

    if(saProcessingLogFile) {
        return;
    }
    saForeachThreadSubscription(thread, subscription) {
        user = saSubscriptionGetUser(subscription);
        if(saPostGetUser(post) != user) {
            mailPost(saSubscriptionGetUser(subscription), post);
        }
    } saEndThreadSubscription;
}

/*--------------------------------------------------------------------------------------------------
  Process a reply command.
--------------------------------------------------------------------------------------------------*/
static void processReplyCommand(
    uint32 argc,
    char **argv)
{
    saThread thread;
    saPost post;
    uint64 threadID;

    if(saCurrentUser == saUserNull) {
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
    thread = saRootFindThread(saTheRoot, threadID);
    post = saPostCreate(saCurrentUser, thread, argv[2]);
    sendFollowersPost(post);
    followThread(thread);
    mailPost(saCurrentUser, post);
    logCommand(argc, argv);
    coPrintf("Created post %u\n", saPostGetID(post));
}

/*--------------------------------------------------------------------------------------------------
  Create a new listing.
--------------------------------------------------------------------------------------------------*/
static saListing saListingCreate(
    saUser user,
    saCategory category,
    uint32 rate,
    bool offered,
    bool fixedPrice,
    char *title,
    char *description)
{
    saRegion region = saUserGetRegion(user);
    saListing listing = saListingAlloc();
    uint64 listingID = saRootGetNextListingID(saTheRoot);

    saListingSetID(listing, listingID);
    saRootSetNextListingID(saTheRoot, listingID + 1);
    saListingSetRate(listing, rate);
    saListingSetOffered(listing, offered);
    saListingSetFixedPrice(listing, fixedPrice);
    saListingSetTitle(listing, title, strlen(title) + 1);
    saListingSetDescription(listing, description, strlen(description) + 1);
    saCategoryAppendListing(category, listing);
    saRootInsertListing(saTheRoot, listing);
    saUserAppendListing(user, listing);
    saCategorySetNumListings(category, saCategoryGetNumListings(category) + 1);
    saRegionSetNumListings(region, saRegionGetNumListings(region) + 1);
    return listing;
}

/*--------------------------------------------------------------------------------------------------
  Update a listing
--------------------------------------------------------------------------------------------------*/
static void updateListing(
    saListing listing,
    saCategory category,
    uint32 rate,
    bool offered,
    bool fixedPrice,
    char *title,
    char *description)
{
    saRegion region = saUserGetRegion(saListingGetUser(listing));

    saListingSetRate(listing, rate);
    saListingSetOffered(listing, offered);
    saListingSetFixedPrice(listing, fixedPrice);
    saListingSetTitle(listing, title, strlen(title) + 1);
    saListingSetDescription(listing, description, strlen(description) + 1);
}

/*--------------------------------------------------------------------------------------------------
  Create a new service listing.
--------------------------------------------------------------------------------------------------*/
static void processNewListingCommand(
    uint32 argc,
    char **argv)
{
    saCategory category;
    saListing listing;
    utSym categorySym;
    int64 rate;
    bool fixedPrice = false;
    bool offerService;

    if(saCurrentUser == saUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(argc != 6) {
        printHelp(argv[0]);
        return;
    }
    offerService = strcmp(argv[1], "wanted");
    categorySym = utSymCreate(argv[2]);
    category = saRootFindCategory(saTheRoot, categorySym);
    if(category == saCategoryNull) {
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
    listing = saListingCreate(saCurrentUser, category, rate, offerService, fixedPrice, argv[4],
        argv[5]);
    logCommand(argc, argv);
    coPrintf("New listing created with listingID %llu\n", saListingGetID(listing));
}

/*--------------------------------------------------------------------------------------------------
  Process an edit listing command.
--------------------------------------------------------------------------------------------------*/
static void processEditListingCommand(
    uint32 argc,
    char **argv)
{
    saCategory category;
    saListing listing;
    utSym categorySym;
    int64 rate, listingID;
    bool fixedPrice = false;
    bool offerService;

    if(saCurrentUser == saUserNull) {
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
    listing = saRootFindListing(saTheRoot, (uint32)listingID);
    if(listing == saListingNull) {
        coPrintf("No listing %llu.\n", listingID);
        return;
    }
    if(saListingGetUser(listing) != saCurrentUser && !saUserSupremeLeader(saCurrentUser) &&
            !(saUserRegionManager(saCurrentUser) && saUserGetRegion(saListingGetUser(listing)) == saUserGetRegion(saCurrentUser))) {
        coPrintf("You do not own this listing.\n");
        return;
    }
    offerService = strcmp(argv[2], "wanted");
    categorySym = utSymCreate(argv[3]);
    category = saRootFindCategory(saTheRoot, categorySym);
    if(category == saCategoryNull) {
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
    updateListing(listing, category, rate, offerService, fixedPrice, argv[5], argv[6]);
    logCommand(argc, argv);
    coPrintf("Listing updated\n", saListingGetID(listing));
}

/*--------------------------------------------------------------------------------------------------
  Process a delete listing command.
--------------------------------------------------------------------------------------------------*/
static void processDeleteListingCommand(
    uint32 argc,
    char **argv)
{
    saCategory category;
    saRegion region;
    saListing listing;
    utSym categorySym;
    int64 listingID;

    if(saCurrentUser == saUserNull) {
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
    listing = saRootFindListing(saTheRoot, listingID);
    if(listing == saListingNull) {
        coPrintf("Invalid listingID\n");
        return;
    }
    if(saListingGetUser(listing) != saCurrentUser) {
        coPrintf("You don't own this listing.\n");
        return;
    }
    category = saListingGetCategory(listing);
    region = saUserGetRegion(saListingGetUser(listing));
    saListingDestroy(listing);
    saCategorySetNumListings(category, saCategoryGetNumListings(category) - 1);
    saRegionSetNumListings(region, saRegionGetNumListings(region) - 1);
    if(saCategoryGetFirstListing(category) == saListingNull) {
        saCategoryDestroy(category);
    }
    logCommand(argc, argv);
    coPrintf("Listing %llu deleted.\n", listingID);
}

/*--------------------------------------------------------------------------------------------------
  Process a createCategory command.
--------------------------------------------------------------------------------------------------*/
static void processCreateCategoryCommand(
    uint32 argc,
    char **argv)
{
    saCategory category;
    utSym categorySym;

    if(saCurrentUser == saUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(!saUserRegionManager(saCurrentUser)) {
        coPrintf("Only region managers can create categories.\n");
        return;
    }
    if(argc != 2) {
        printHelp(argv[0]);
        return;
    }
    categorySym = utSymCreate(argv[1]);
    category = saRootFindCategory(saTheRoot, categorySym);
    if(category != saCategoryNull) {
        coPrintf("Category %s already exists.\n", argv[1]);
        return;
    }
    category = saCategoryAlloc();
    saCategorySetSym(category, categorySym);
    saRootInsertCategory(saTheRoot, category);
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
    saCategory category;
    utSym categorySym;

    if(saCurrentUser == saUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(!saUserRegionManager(saCurrentUser)) {
        coPrintf("Only region managers can delete categories.\n");
        return;
    }
    if(argc != 2) {
        printHelp(argv[0]);
        return;
    }
    categorySym = utSymCreate(argv[1]);
    category = saRootFindCategory(saTheRoot, categorySym);
    if(category == saCategoryNull) {
        coPrintf("Category %s does not exist.\n", argv[1]);
        return;
    }
    saCategoryDestroy(category);
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
    saCategory category;
    utSym categorySym;

    if(saCurrentUser == saUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(!saUserRegionManager(saCurrentUser)) {
        coPrintf("Only region managers can rename categories.\n");
        return;
    }
    if(argc != 3) {
        printHelp(argv[0]);
        return;
    }
    categorySym = utSymCreate(argv[1]);
    category = saRootFindCategory(saTheRoot, categorySym);
    if(category == saCategoryNull) {
        coPrintf("Category %s does not exist.\n", argv[1]);
        return;
    }
    saRootRenameCategory(saTheRoot, category, utSymCreate(argv[2]));
    logCommand(argc, argv);
    coPrintf("Category %s renamed to %s.\n", argv[1], argv[2]);
}

/*--------------------------------------------------------------------------------------------------
  Process a createRegion command.
--------------------------------------------------------------------------------------------------*/
static void processCreateRegionCommand(
    uint32 argc,
    char **argv)
{
    saRegion region;
    utSym regionSym;

    if(saCurrentUser == saUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(!saUserSupremeLeader(saCurrentUser)) {
        coPrintf("Only the supreme leader can create regions.\n");
        return;
    }
    if(argc != 2) {
        printHelp(argv[0]);
        return;
    }
    regionSym = utSymCreate(argv[1]);
    region = saRootFindRegion(saTheRoot, regionSym);
    if(region != saRegionNull) {
        coPrintf("Region %s already exists.\n", argv[1]);
        return;
    }
    region = saRegionAlloc();
    saRegionSetSym(region, regionSym);
    saRootInsertRegion(saTheRoot, region);
    logCommand(argc, argv);
    coPrintf("Region %s created.\n", argv[1]);
}

/*--------------------------------------------------------------------------------------------------
  Process a deleteRegion command.
--------------------------------------------------------------------------------------------------*/
static void processDeleteRegionCommand(
    uint32 argc,
    char **argv)
{
    saRegion region;
    utSym regionSym;

    if(saCurrentUser == saUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(!saUserSupremeLeader(saCurrentUser)) {
        coPrintf("Only the supreme leader can delete regions.\n");
        return;
    }
    if(argc != 2) {
        printHelp(argv[0]);
        return;
    }
    regionSym = utSymCreate(argv[1]);
    region = saRootFindRegion(saTheRoot, regionSym);
    if(region == saRegionNull) {
        coPrintf("Region %s does not exist.\n", argv[1]);
        return;
    }
    saRegionDestroy(region);
    logCommand(argc, argv);
    coPrintf("Region %s deleted.\n", argv[1]);
}

/*--------------------------------------------------------------------------------------------------
  Process a renameRegion command.
--------------------------------------------------------------------------------------------------*/
static void processRenameRegionCommand(
    uint32 argc,
    char **argv)
{
    saRegion region;
    utSym regionSym;

    if(saCurrentUser == saUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(!saUserSupremeLeader(saCurrentUser)) {
        coPrintf("Only the supreme leader can rename regions.\n");
        return;
    }
    if(argc != 3) {
        printHelp(argv[0]);
        return;
    }
    regionSym = utSymCreate(argv[1]);
    region = saRootFindRegion(saTheRoot, regionSym);
    if(region == saRegionNull) {
        coPrintf("Region %s does not exist.\n", argv[1]);
        return;
    }
    saRootRenameRegion(saTheRoot, region, utSymCreate(argv[2]));
    logCommand(argc, argv);
    coPrintf("Region %s renamed to %s.\n", argv[1], argv[2]);
}

/*--------------------------------------------------------------------------------------------------
  Process a setRegionManager command.
--------------------------------------------------------------------------------------------------*/
static void processSetRegionManagerCommand(
    uint32 argc,
    char **argv)
{
    saRegion region;
    saUser user;

    if(saCurrentUser == saUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(!saUserSupremeLeader(saCurrentUser)) {
        coPrintf("Only the supreme leader can select region managers.\n");
        return;
    }
    if(argc != 3) {
        printHelp(argv[0]);
        return;
    }
    user = saRootFindUser(saTheRoot, utSymCreate(argv[1]));
    if(user == saUserNull) {
        coPrintf("User %s does not exist.\n", argv[1]);
        return;
    }
    if(!strcmp(argv[2], "true")) {
        saUserSetRegionManager(user, true);
        coPrintf("User %s set as region manager.\n", argv[1]);
    } else if(!strcmp(argv[2], "false")) {
        saUserSetRegionManager(user, false);
        coPrintf("User %s removed as region manager.\n", argv[1]);
    } else {
        printHelp(argv[0]);
        return;
    }
    logCommand(argc, argv);
}

/*--------------------------------------------------------------------------------------------------
  Save the database in ASCII
--------------------------------------------------------------------------------------------------*/
static void processSaveDatabaseCommand(
    uint32 argc,
    char **argv)
{
    saRegion region;
    saUser user;
    FILE *file;

    if(saCurrentUser == saUserNull) {
        coPrintf("Not currently logged in.\n");
        return;
    }
    if(!saUserSupremeLeader(saCurrentUser)) {
        coPrintf("Only the supreme leader can select region managers.\n");
        return;
    }
    if(argc != 1) {
        printHelp(argv[0]);
        return;
    }
    file = fopen(DATABASE_FILE, "w");
    if(file == NULL) {
        coPrintf("Unable to open sharealot.db\n");
        return;
    }
    utSaveTextDatabase(file);
    fclose(file);
    fclose(saLogFile);
    rename(LOG_FILE, OLD_LOG_FILE);
    saLogFile = fopen(LOG_FILE, "w");
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
    } else if(!strcmp(command, "listRegions")) {
        processListRegionsCommand(argc, argv);
    } else if(!strcmp(command, "chooseRegion")) {
        processChooseRegionCommand(argc, argv);
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
    } else if(!strcmp(command, "newListing")) {
        processNewListingCommand(argc, argv);
    } else if(!strcmp(command, "editListing")) {
        processEditListingCommand(argc, argv);
    } else if(!strcmp(command, "deleteListing")) {
        processDeleteListingCommand(argc, argv);
    } else if(!strcmp(command, "createCategory")) {
        processCreateCategoryCommand(argc, argv);
    } else if(!strcmp(command, "deleteCategory")) {
        processDeleteCategoryCommand(argc, argv);
    } else if(!strcmp(command, "renameCategory")) {
        processRenameCategoryCommand(argc, argv);
    } else if(!strcmp(command, "createRegion")) {
        processCreateRegionCommand(argc, argv);
    } else if(!strcmp(command, "deleteRegion")) {
        processDeleteRegionCommand(argc, argv);
    } else if(!strcmp(command, "renameRegion")) {
        processRenameRegionCommand(argc, argv);
    } else if(!strcmp(command, "setRegionManager")) {
        processSetRegionManagerCommand(argc, argv);
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
    saSession session = saRootFindSession(saTheRoot, sym);

    if(session == saCurrentSession && session != saSessionNull) {
        return false;
    }
    if(saCurrentSession != saSessionNull) {
        saSessionSetUser(saCurrentSession, saCurrentUser);
        if(saCurrentUser != saUserNull) {
            saUserSetSession(saCurrentUser, saCurrentSession);
        }
    }
    if(session == saSessionNull) {
        session = saSessionAlloc();
        saSessionSetSym(session, sym);
        saSessionSetRegion(session, saRootGetFirstRegion(saTheRoot));
        saRootInsertSession(saTheRoot, session);
        saCurrentUser = saUserNull;
        saCurrentRegion = saSessionGetRegion(session);
        saCurrentSession = session;
        return true;
    }
    saCurrentSession = session;
    saCurrentUser = saSessionGetUser(session);
    saCurrentRegion = saSessionGetRegion(session);
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
            argc = parseArguments(saLineBuffer, argv);
            saCurrentTime = time(NULL);
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
    saUser user = saRootFindUser(saTheRoot, sym);

    if(user == saUserNull) {
        coPrintf("Unable to find user %s\n", userName);
    } else {
        if(saCurrentUser != saUserNull) {
            saUserSetLoggedIn(saCurrentUser, false);
        }
        saUserSetLoggedIn(user, true);
        saCurrentUser = user;
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
        argc = parseArguments(saLineBuffer, argv);
        if(argc > 1) {
            p = strptime(argv[0], "%x-%X", &tm);
            if(p == NULL || *p != '\0') {
                coPrintf("Invalid date format.  Use MM/DD/YY.\n");
                exit(1);
            }
            saCurrentTime = mktime(&tm);
            if(argv[1][0] == ':') {
                switchUser(argv[1] + 1);
                processCommand(argc - 2, argv + 2);
            } else {
                processCommand(argc - 1, argv + 1);
            }
        }
    }
    fclose(file);
    saCurrentUser = saUserNull;
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
    saDatabaseStart();
    saTheRoot = saRootAlloc();
    saCurrentUser = saUserNull;
    saCurrentRegion = saRegionNull;
    saCurrentSession = saSessionNull;
    saLineSize = 42;
    saLineBuffer = utNewA(char, saLineSize);
    if(utFileExists(DATABASE_FILE)) {
        databaseFile = fopen(DATABASE_FILE, "r");
        utLoadTextDatabase(databaseFile);
        fclose(databaseFile);
    }
    if(utFileExists(LOG_FILE)) {
        saProcessingLogFile = true;
        saLogFile = NULL;
        processFileCommands(LOG_FILE);
        saCurrentRegion = saRootGetFirstRegion(saTheRoot);
    }
    saProcessingLogFile = false;
    saLogFile = fopen(LOG_FILE, "a");
    if(argc == 2) {
        coStartServer(argv[1]);
    }
    commandInterpreter();
    if(argc == 2) {
        coStopServer();
    }
    saDatabaseStop();
    fclose(saLogFile);
    utFree(saLineBuffer);
    utStop(false);
    return 0;
}
