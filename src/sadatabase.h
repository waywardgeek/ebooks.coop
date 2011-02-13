/*----------------------------------------------------------------------------------------
  Module header file for: sa module
----------------------------------------------------------------------------------------*/
#ifndef SADATABASE_H

#define SADATABASE_H

#if defined __cplusplus
extern "C" {
#endif

#ifndef DD_UTIL_H
#include "ddutil.h"
#endif

extern uint8 saModuleID;
/* Class reference definitions */
#if (defined(DD_DEBUG) && !defined(DD_NOSTRICT)) || defined(DD_STRICT)
typedef struct _struct_saRoot{char val;} *saRoot;
#define saRootNull ((saRoot)0)
typedef struct _struct_saRegion{char val;} *saRegion;
#define saRegionNull ((saRegion)0)
typedef struct _struct_saSession{char val;} *saSession;
#define saSessionNull ((saSession)0)
typedef struct _struct_saUser{char val;} *saUser;
#define saUserNull ((saUser)0)
typedef struct _struct_saTransaction{char val;} *saTransaction;
#define saTransactionNull ((saTransaction)0)
typedef struct _struct_saCharity{char val;} *saCharity;
#define saCharityNull ((saCharity)0)
typedef struct _struct_saMembership{char val;} *saMembership;
#define saMembershipNull ((saMembership)0)
typedef struct _struct_saCategory{char val;} *saCategory;
#define saCategoryNull ((saCategory)0)
typedef struct _struct_saListing{char val;} *saListing;
#define saListingNull ((saListing)0)
typedef struct _struct_saThread{char val;} *saThread;
#define saThreadNull ((saThread)0)
typedef struct _struct_saPost{char val;} *saPost;
#define saPostNull ((saPost)0)
typedef struct _struct_saSubscription{char val;} *saSubscription;
#define saSubscriptionNull ((saSubscription)0)
typedef struct _struct_saAnnouncement{char val;} *saAnnouncement;
#define saAnnouncementNull ((saAnnouncement)0)
typedef struct _struct_saTransactionArray{char val;} *saTransactionArray;
#define saTransactionArrayNull ((saTransactionArray)0)
#else
typedef uint32 saRoot;
#define saRootNull 0
typedef uint32 saRegion;
#define saRegionNull 0
typedef uint32 saSession;
#define saSessionNull 0
typedef uint32 saUser;
#define saUserNull 0
typedef uint32 saTransaction;
#define saTransactionNull 0
typedef uint32 saCharity;
#define saCharityNull 0
typedef uint32 saMembership;
#define saMembershipNull 0
typedef uint32 saCategory;
#define saCategoryNull 0
typedef uint32 saListing;
#define saListingNull 0
typedef uint32 saThread;
#define saThreadNull 0
typedef uint32 saPost;
#define saPostNull 0
typedef uint32 saSubscription;
#define saSubscriptionNull 0
typedef uint32 saAnnouncement;
#define saAnnouncementNull 0
typedef uint32 saTransactionArray;
#define saTransactionArrayNull 0
#endif

/* Constructor/Destructor hooks. */
typedef void (*saRootCallbackType)(saRoot);
extern saRootCallbackType saRootConstructorCallback;
extern saRootCallbackType saRootDestructorCallback;
typedef void (*saRegionCallbackType)(saRegion);
extern saRegionCallbackType saRegionConstructorCallback;
extern saRegionCallbackType saRegionDestructorCallback;
typedef void (*saSessionCallbackType)(saSession);
extern saSessionCallbackType saSessionConstructorCallback;
extern saSessionCallbackType saSessionDestructorCallback;
typedef void (*saUserCallbackType)(saUser);
extern saUserCallbackType saUserConstructorCallback;
extern saUserCallbackType saUserDestructorCallback;
typedef void (*saTransactionCallbackType)(saTransaction);
extern saTransactionCallbackType saTransactionConstructorCallback;
extern saTransactionCallbackType saTransactionDestructorCallback;
typedef void (*saCharityCallbackType)(saCharity);
extern saCharityCallbackType saCharityConstructorCallback;
extern saCharityCallbackType saCharityDestructorCallback;
typedef void (*saMembershipCallbackType)(saMembership);
extern saMembershipCallbackType saMembershipConstructorCallback;
extern saMembershipCallbackType saMembershipDestructorCallback;
typedef void (*saCategoryCallbackType)(saCategory);
extern saCategoryCallbackType saCategoryConstructorCallback;
extern saCategoryCallbackType saCategoryDestructorCallback;
typedef void (*saListingCallbackType)(saListing);
extern saListingCallbackType saListingConstructorCallback;
extern saListingCallbackType saListingDestructorCallback;
typedef void (*saThreadCallbackType)(saThread);
extern saThreadCallbackType saThreadConstructorCallback;
extern saThreadCallbackType saThreadDestructorCallback;
typedef void (*saPostCallbackType)(saPost);
extern saPostCallbackType saPostConstructorCallback;
extern saPostCallbackType saPostDestructorCallback;
typedef void (*saSubscriptionCallbackType)(saSubscription);
extern saSubscriptionCallbackType saSubscriptionConstructorCallback;
extern saSubscriptionCallbackType saSubscriptionDestructorCallback;
typedef void (*saAnnouncementCallbackType)(saAnnouncement);
extern saAnnouncementCallbackType saAnnouncementConstructorCallback;
extern saAnnouncementCallbackType saAnnouncementDestructorCallback;
typedef void (*saTransactionArrayCallbackType)(saTransactionArray);
extern saTransactionArrayCallbackType saTransactionArrayConstructorCallback;
extern saTransactionArrayCallbackType saTransactionArrayDestructorCallback;

/*----------------------------------------------------------------------------------------
  Root structure
----------------------------------------------------------------------------------------*/
struct saRootType_ {
    uint32 hash; /* This depends only on the structure of the database */
    saRoot firstFreeRoot;
    uint32 usedRoot, allocatedRoot;
    uint32 usedRootRegionTable, allocatedRootRegionTable, freeRootRegionTable;
    uint32 usedRootUserTable, allocatedRootUserTable, freeRootUserTable;
    uint32 usedRootByEmailUserTable, allocatedRootByEmailUserTable, freeRootByEmailUserTable;
    uint32 usedRootCategoryTable, allocatedRootCategoryTable, freeRootCategoryTable;
    uint32 usedRootListingTable, allocatedRootListingTable, freeRootListingTable;
    uint32 usedRootSessionTable, allocatedRootSessionTable, freeRootSessionTable;
    uint32 usedRootThreadTable, allocatedRootThreadTable, freeRootThreadTable;
    uint32 usedRootPostTable, allocatedRootPostTable, freeRootPostTable;
    uint32 usedRootAnnouncementTable, allocatedRootAnnouncementTable, freeRootAnnouncementTable;
    saRegion firstFreeRegion;
    uint32 usedRegion, allocatedRegion;
    saSession firstFreeSession;
    uint32 usedSession, allocatedSession;
    saUser firstFreeUser;
    uint32 usedUser, allocatedUser;
    uint32 usedUserShownName, allocatedUserShownName, freeUserShownName;
    uint32 usedUserHashedPassword, allocatedUserHashedPassword, freeUserHashedPassword;
    uint32 usedUserNounce, allocatedUserNounce, freeUserNounce;
    uint32 usedUserValidationKey, allocatedUserValidationKey, freeUserValidationKey;
    uint32 usedUserMembershipTable, allocatedUserMembershipTable, freeUserMembershipTable;
    uint32 usedUserPostTable, allocatedUserPostTable, freeUserPostTable;
    saTransaction firstFreeTransaction;
    uint32 usedTransaction, allocatedTransaction;
    uint32 usedTransactionDescription, allocatedTransactionDescription, freeTransactionDescription;
    saCharity firstFreeCharity;
    uint32 usedCharity, allocatedCharity;
    saMembership firstFreeMembership;
    uint32 usedMembership, allocatedMembership;
    saCategory firstFreeCategory;
    uint32 usedCategory, allocatedCategory;
    saListing firstFreeListing;
    uint32 usedListing, allocatedListing;
    uint32 usedListingTitle, allocatedListingTitle, freeListingTitle;
    uint32 usedListingDescription, allocatedListingDescription, freeListingDescription;
    saThread firstFreeThread;
    uint32 usedThread, allocatedThread;
    uint32 usedThreadSubject, allocatedThreadSubject, freeThreadSubject;
    saPost firstFreePost;
    uint32 usedPost, allocatedPost;
    uint32 usedPostMessage, allocatedPostMessage, freePostMessage;
    saSubscription firstFreeSubscription;
    uint32 usedSubscription, allocatedSubscription;
    saAnnouncement firstFreeAnnouncement;
    uint32 usedAnnouncement, allocatedAnnouncement;
    uint32 usedAnnouncementSubject, allocatedAnnouncementSubject, freeAnnouncementSubject;
    uint32 usedAnnouncementMessage, allocatedAnnouncementMessage, freeAnnouncementMessage;
    saTransactionArray firstFreeTransactionArray;
    uint32 usedTransactionArray, allocatedTransactionArray;
    uint32 usedTransactionArrayTransaction, allocatedTransactionArrayTransaction, freeTransactionArrayTransaction;
};
extern struct saRootType_ saRootData;

utInlineC uint32 saHash(void) {return saRootData.hash;}
utInlineC saRoot saFirstFreeRoot(void) {return saRootData.firstFreeRoot;}
utInlineC void saSetFirstFreeRoot(saRoot value) {saRootData.firstFreeRoot = (value);}
utInlineC uint32 saUsedRoot(void) {return saRootData.usedRoot;}
utInlineC uint32 saAllocatedRoot(void) {return saRootData.allocatedRoot;}
utInlineC void saSetUsedRoot(uint32 value) {saRootData.usedRoot = value;}
utInlineC void saSetAllocatedRoot(uint32 value) {saRootData.allocatedRoot = value;}
utInlineC uint32 saUsedRootRegionTable(void) {return saRootData.usedRootRegionTable;}
utInlineC uint32 saAllocatedRootRegionTable(void) {return saRootData.allocatedRootRegionTable;}
utInlineC uint32 saFreeRootRegionTable(void) {return saRootData.freeRootRegionTable;}
utInlineC void saSetUsedRootRegionTable(uint32 value) {saRootData.usedRootRegionTable = value;}
utInlineC void saSetAllocatedRootRegionTable(uint32 value) {saRootData.allocatedRootRegionTable = value;}
utInlineC void saSetFreeRootRegionTable(int32 value) {saRootData.freeRootRegionTable = value;}
utInlineC uint32 saUsedRootUserTable(void) {return saRootData.usedRootUserTable;}
utInlineC uint32 saAllocatedRootUserTable(void) {return saRootData.allocatedRootUserTable;}
utInlineC uint32 saFreeRootUserTable(void) {return saRootData.freeRootUserTable;}
utInlineC void saSetUsedRootUserTable(uint32 value) {saRootData.usedRootUserTable = value;}
utInlineC void saSetAllocatedRootUserTable(uint32 value) {saRootData.allocatedRootUserTable = value;}
utInlineC void saSetFreeRootUserTable(int32 value) {saRootData.freeRootUserTable = value;}
utInlineC uint32 saUsedRootByEmailUserTable(void) {return saRootData.usedRootByEmailUserTable;}
utInlineC uint32 saAllocatedRootByEmailUserTable(void) {return saRootData.allocatedRootByEmailUserTable;}
utInlineC uint32 saFreeRootByEmailUserTable(void) {return saRootData.freeRootByEmailUserTable;}
utInlineC void saSetUsedRootByEmailUserTable(uint32 value) {saRootData.usedRootByEmailUserTable = value;}
utInlineC void saSetAllocatedRootByEmailUserTable(uint32 value) {saRootData.allocatedRootByEmailUserTable = value;}
utInlineC void saSetFreeRootByEmailUserTable(int32 value) {saRootData.freeRootByEmailUserTable = value;}
utInlineC uint32 saUsedRootCategoryTable(void) {return saRootData.usedRootCategoryTable;}
utInlineC uint32 saAllocatedRootCategoryTable(void) {return saRootData.allocatedRootCategoryTable;}
utInlineC uint32 saFreeRootCategoryTable(void) {return saRootData.freeRootCategoryTable;}
utInlineC void saSetUsedRootCategoryTable(uint32 value) {saRootData.usedRootCategoryTable = value;}
utInlineC void saSetAllocatedRootCategoryTable(uint32 value) {saRootData.allocatedRootCategoryTable = value;}
utInlineC void saSetFreeRootCategoryTable(int32 value) {saRootData.freeRootCategoryTable = value;}
utInlineC uint32 saUsedRootListingTable(void) {return saRootData.usedRootListingTable;}
utInlineC uint32 saAllocatedRootListingTable(void) {return saRootData.allocatedRootListingTable;}
utInlineC uint32 saFreeRootListingTable(void) {return saRootData.freeRootListingTable;}
utInlineC void saSetUsedRootListingTable(uint32 value) {saRootData.usedRootListingTable = value;}
utInlineC void saSetAllocatedRootListingTable(uint32 value) {saRootData.allocatedRootListingTable = value;}
utInlineC void saSetFreeRootListingTable(int32 value) {saRootData.freeRootListingTable = value;}
utInlineC uint32 saUsedRootSessionTable(void) {return saRootData.usedRootSessionTable;}
utInlineC uint32 saAllocatedRootSessionTable(void) {return saRootData.allocatedRootSessionTable;}
utInlineC uint32 saFreeRootSessionTable(void) {return saRootData.freeRootSessionTable;}
utInlineC void saSetUsedRootSessionTable(uint32 value) {saRootData.usedRootSessionTable = value;}
utInlineC void saSetAllocatedRootSessionTable(uint32 value) {saRootData.allocatedRootSessionTable = value;}
utInlineC void saSetFreeRootSessionTable(int32 value) {saRootData.freeRootSessionTable = value;}
utInlineC uint32 saUsedRootThreadTable(void) {return saRootData.usedRootThreadTable;}
utInlineC uint32 saAllocatedRootThreadTable(void) {return saRootData.allocatedRootThreadTable;}
utInlineC uint32 saFreeRootThreadTable(void) {return saRootData.freeRootThreadTable;}
utInlineC void saSetUsedRootThreadTable(uint32 value) {saRootData.usedRootThreadTable = value;}
utInlineC void saSetAllocatedRootThreadTable(uint32 value) {saRootData.allocatedRootThreadTable = value;}
utInlineC void saSetFreeRootThreadTable(int32 value) {saRootData.freeRootThreadTable = value;}
utInlineC uint32 saUsedRootPostTable(void) {return saRootData.usedRootPostTable;}
utInlineC uint32 saAllocatedRootPostTable(void) {return saRootData.allocatedRootPostTable;}
utInlineC uint32 saFreeRootPostTable(void) {return saRootData.freeRootPostTable;}
utInlineC void saSetUsedRootPostTable(uint32 value) {saRootData.usedRootPostTable = value;}
utInlineC void saSetAllocatedRootPostTable(uint32 value) {saRootData.allocatedRootPostTable = value;}
utInlineC void saSetFreeRootPostTable(int32 value) {saRootData.freeRootPostTable = value;}
utInlineC uint32 saUsedRootAnnouncementTable(void) {return saRootData.usedRootAnnouncementTable;}
utInlineC uint32 saAllocatedRootAnnouncementTable(void) {return saRootData.allocatedRootAnnouncementTable;}
utInlineC uint32 saFreeRootAnnouncementTable(void) {return saRootData.freeRootAnnouncementTable;}
utInlineC void saSetUsedRootAnnouncementTable(uint32 value) {saRootData.usedRootAnnouncementTable = value;}
utInlineC void saSetAllocatedRootAnnouncementTable(uint32 value) {saRootData.allocatedRootAnnouncementTable = value;}
utInlineC void saSetFreeRootAnnouncementTable(int32 value) {saRootData.freeRootAnnouncementTable = value;}
utInlineC saRegion saFirstFreeRegion(void) {return saRootData.firstFreeRegion;}
utInlineC void saSetFirstFreeRegion(saRegion value) {saRootData.firstFreeRegion = (value);}
utInlineC uint32 saUsedRegion(void) {return saRootData.usedRegion;}
utInlineC uint32 saAllocatedRegion(void) {return saRootData.allocatedRegion;}
utInlineC void saSetUsedRegion(uint32 value) {saRootData.usedRegion = value;}
utInlineC void saSetAllocatedRegion(uint32 value) {saRootData.allocatedRegion = value;}
utInlineC saSession saFirstFreeSession(void) {return saRootData.firstFreeSession;}
utInlineC void saSetFirstFreeSession(saSession value) {saRootData.firstFreeSession = (value);}
utInlineC uint32 saUsedSession(void) {return saRootData.usedSession;}
utInlineC uint32 saAllocatedSession(void) {return saRootData.allocatedSession;}
utInlineC void saSetUsedSession(uint32 value) {saRootData.usedSession = value;}
utInlineC void saSetAllocatedSession(uint32 value) {saRootData.allocatedSession = value;}
utInlineC saUser saFirstFreeUser(void) {return saRootData.firstFreeUser;}
utInlineC void saSetFirstFreeUser(saUser value) {saRootData.firstFreeUser = (value);}
utInlineC uint32 saUsedUser(void) {return saRootData.usedUser;}
utInlineC uint32 saAllocatedUser(void) {return saRootData.allocatedUser;}
utInlineC void saSetUsedUser(uint32 value) {saRootData.usedUser = value;}
utInlineC void saSetAllocatedUser(uint32 value) {saRootData.allocatedUser = value;}
utInlineC uint32 saUsedUserShownName(void) {return saRootData.usedUserShownName;}
utInlineC uint32 saAllocatedUserShownName(void) {return saRootData.allocatedUserShownName;}
utInlineC uint32 saFreeUserShownName(void) {return saRootData.freeUserShownName;}
utInlineC void saSetUsedUserShownName(uint32 value) {saRootData.usedUserShownName = value;}
utInlineC void saSetAllocatedUserShownName(uint32 value) {saRootData.allocatedUserShownName = value;}
utInlineC void saSetFreeUserShownName(int32 value) {saRootData.freeUserShownName = value;}
utInlineC uint32 saUsedUserHashedPassword(void) {return saRootData.usedUserHashedPassword;}
utInlineC uint32 saAllocatedUserHashedPassword(void) {return saRootData.allocatedUserHashedPassword;}
utInlineC uint32 saFreeUserHashedPassword(void) {return saRootData.freeUserHashedPassword;}
utInlineC void saSetUsedUserHashedPassword(uint32 value) {saRootData.usedUserHashedPassword = value;}
utInlineC void saSetAllocatedUserHashedPassword(uint32 value) {saRootData.allocatedUserHashedPassword = value;}
utInlineC void saSetFreeUserHashedPassword(int32 value) {saRootData.freeUserHashedPassword = value;}
utInlineC uint32 saUsedUserNounce(void) {return saRootData.usedUserNounce;}
utInlineC uint32 saAllocatedUserNounce(void) {return saRootData.allocatedUserNounce;}
utInlineC uint32 saFreeUserNounce(void) {return saRootData.freeUserNounce;}
utInlineC void saSetUsedUserNounce(uint32 value) {saRootData.usedUserNounce = value;}
utInlineC void saSetAllocatedUserNounce(uint32 value) {saRootData.allocatedUserNounce = value;}
utInlineC void saSetFreeUserNounce(int32 value) {saRootData.freeUserNounce = value;}
utInlineC uint32 saUsedUserValidationKey(void) {return saRootData.usedUserValidationKey;}
utInlineC uint32 saAllocatedUserValidationKey(void) {return saRootData.allocatedUserValidationKey;}
utInlineC uint32 saFreeUserValidationKey(void) {return saRootData.freeUserValidationKey;}
utInlineC void saSetUsedUserValidationKey(uint32 value) {saRootData.usedUserValidationKey = value;}
utInlineC void saSetAllocatedUserValidationKey(uint32 value) {saRootData.allocatedUserValidationKey = value;}
utInlineC void saSetFreeUserValidationKey(int32 value) {saRootData.freeUserValidationKey = value;}
utInlineC uint32 saUsedUserMembershipTable(void) {return saRootData.usedUserMembershipTable;}
utInlineC uint32 saAllocatedUserMembershipTable(void) {return saRootData.allocatedUserMembershipTable;}
utInlineC uint32 saFreeUserMembershipTable(void) {return saRootData.freeUserMembershipTable;}
utInlineC void saSetUsedUserMembershipTable(uint32 value) {saRootData.usedUserMembershipTable = value;}
utInlineC void saSetAllocatedUserMembershipTable(uint32 value) {saRootData.allocatedUserMembershipTable = value;}
utInlineC void saSetFreeUserMembershipTable(int32 value) {saRootData.freeUserMembershipTable = value;}
utInlineC uint32 saUsedUserPostTable(void) {return saRootData.usedUserPostTable;}
utInlineC uint32 saAllocatedUserPostTable(void) {return saRootData.allocatedUserPostTable;}
utInlineC uint32 saFreeUserPostTable(void) {return saRootData.freeUserPostTable;}
utInlineC void saSetUsedUserPostTable(uint32 value) {saRootData.usedUserPostTable = value;}
utInlineC void saSetAllocatedUserPostTable(uint32 value) {saRootData.allocatedUserPostTable = value;}
utInlineC void saSetFreeUserPostTable(int32 value) {saRootData.freeUserPostTable = value;}
utInlineC saTransaction saFirstFreeTransaction(void) {return saRootData.firstFreeTransaction;}
utInlineC void saSetFirstFreeTransaction(saTransaction value) {saRootData.firstFreeTransaction = (value);}
utInlineC uint32 saUsedTransaction(void) {return saRootData.usedTransaction;}
utInlineC uint32 saAllocatedTransaction(void) {return saRootData.allocatedTransaction;}
utInlineC void saSetUsedTransaction(uint32 value) {saRootData.usedTransaction = value;}
utInlineC void saSetAllocatedTransaction(uint32 value) {saRootData.allocatedTransaction = value;}
utInlineC uint32 saUsedTransactionDescription(void) {return saRootData.usedTransactionDescription;}
utInlineC uint32 saAllocatedTransactionDescription(void) {return saRootData.allocatedTransactionDescription;}
utInlineC uint32 saFreeTransactionDescription(void) {return saRootData.freeTransactionDescription;}
utInlineC void saSetUsedTransactionDescription(uint32 value) {saRootData.usedTransactionDescription = value;}
utInlineC void saSetAllocatedTransactionDescription(uint32 value) {saRootData.allocatedTransactionDescription = value;}
utInlineC void saSetFreeTransactionDescription(int32 value) {saRootData.freeTransactionDescription = value;}
utInlineC saCharity saFirstFreeCharity(void) {return saRootData.firstFreeCharity;}
utInlineC void saSetFirstFreeCharity(saCharity value) {saRootData.firstFreeCharity = (value);}
utInlineC uint32 saUsedCharity(void) {return saRootData.usedCharity;}
utInlineC uint32 saAllocatedCharity(void) {return saRootData.allocatedCharity;}
utInlineC void saSetUsedCharity(uint32 value) {saRootData.usedCharity = value;}
utInlineC void saSetAllocatedCharity(uint32 value) {saRootData.allocatedCharity = value;}
utInlineC saMembership saFirstFreeMembership(void) {return saRootData.firstFreeMembership;}
utInlineC void saSetFirstFreeMembership(saMembership value) {saRootData.firstFreeMembership = (value);}
utInlineC uint32 saUsedMembership(void) {return saRootData.usedMembership;}
utInlineC uint32 saAllocatedMembership(void) {return saRootData.allocatedMembership;}
utInlineC void saSetUsedMembership(uint32 value) {saRootData.usedMembership = value;}
utInlineC void saSetAllocatedMembership(uint32 value) {saRootData.allocatedMembership = value;}
utInlineC saCategory saFirstFreeCategory(void) {return saRootData.firstFreeCategory;}
utInlineC void saSetFirstFreeCategory(saCategory value) {saRootData.firstFreeCategory = (value);}
utInlineC uint32 saUsedCategory(void) {return saRootData.usedCategory;}
utInlineC uint32 saAllocatedCategory(void) {return saRootData.allocatedCategory;}
utInlineC void saSetUsedCategory(uint32 value) {saRootData.usedCategory = value;}
utInlineC void saSetAllocatedCategory(uint32 value) {saRootData.allocatedCategory = value;}
utInlineC saListing saFirstFreeListing(void) {return saRootData.firstFreeListing;}
utInlineC void saSetFirstFreeListing(saListing value) {saRootData.firstFreeListing = (value);}
utInlineC uint32 saUsedListing(void) {return saRootData.usedListing;}
utInlineC uint32 saAllocatedListing(void) {return saRootData.allocatedListing;}
utInlineC void saSetUsedListing(uint32 value) {saRootData.usedListing = value;}
utInlineC void saSetAllocatedListing(uint32 value) {saRootData.allocatedListing = value;}
utInlineC uint32 saUsedListingTitle(void) {return saRootData.usedListingTitle;}
utInlineC uint32 saAllocatedListingTitle(void) {return saRootData.allocatedListingTitle;}
utInlineC uint32 saFreeListingTitle(void) {return saRootData.freeListingTitle;}
utInlineC void saSetUsedListingTitle(uint32 value) {saRootData.usedListingTitle = value;}
utInlineC void saSetAllocatedListingTitle(uint32 value) {saRootData.allocatedListingTitle = value;}
utInlineC void saSetFreeListingTitle(int32 value) {saRootData.freeListingTitle = value;}
utInlineC uint32 saUsedListingDescription(void) {return saRootData.usedListingDescription;}
utInlineC uint32 saAllocatedListingDescription(void) {return saRootData.allocatedListingDescription;}
utInlineC uint32 saFreeListingDescription(void) {return saRootData.freeListingDescription;}
utInlineC void saSetUsedListingDescription(uint32 value) {saRootData.usedListingDescription = value;}
utInlineC void saSetAllocatedListingDescription(uint32 value) {saRootData.allocatedListingDescription = value;}
utInlineC void saSetFreeListingDescription(int32 value) {saRootData.freeListingDescription = value;}
utInlineC saThread saFirstFreeThread(void) {return saRootData.firstFreeThread;}
utInlineC void saSetFirstFreeThread(saThread value) {saRootData.firstFreeThread = (value);}
utInlineC uint32 saUsedThread(void) {return saRootData.usedThread;}
utInlineC uint32 saAllocatedThread(void) {return saRootData.allocatedThread;}
utInlineC void saSetUsedThread(uint32 value) {saRootData.usedThread = value;}
utInlineC void saSetAllocatedThread(uint32 value) {saRootData.allocatedThread = value;}
utInlineC uint32 saUsedThreadSubject(void) {return saRootData.usedThreadSubject;}
utInlineC uint32 saAllocatedThreadSubject(void) {return saRootData.allocatedThreadSubject;}
utInlineC uint32 saFreeThreadSubject(void) {return saRootData.freeThreadSubject;}
utInlineC void saSetUsedThreadSubject(uint32 value) {saRootData.usedThreadSubject = value;}
utInlineC void saSetAllocatedThreadSubject(uint32 value) {saRootData.allocatedThreadSubject = value;}
utInlineC void saSetFreeThreadSubject(int32 value) {saRootData.freeThreadSubject = value;}
utInlineC saPost saFirstFreePost(void) {return saRootData.firstFreePost;}
utInlineC void saSetFirstFreePost(saPost value) {saRootData.firstFreePost = (value);}
utInlineC uint32 saUsedPost(void) {return saRootData.usedPost;}
utInlineC uint32 saAllocatedPost(void) {return saRootData.allocatedPost;}
utInlineC void saSetUsedPost(uint32 value) {saRootData.usedPost = value;}
utInlineC void saSetAllocatedPost(uint32 value) {saRootData.allocatedPost = value;}
utInlineC uint32 saUsedPostMessage(void) {return saRootData.usedPostMessage;}
utInlineC uint32 saAllocatedPostMessage(void) {return saRootData.allocatedPostMessage;}
utInlineC uint32 saFreePostMessage(void) {return saRootData.freePostMessage;}
utInlineC void saSetUsedPostMessage(uint32 value) {saRootData.usedPostMessage = value;}
utInlineC void saSetAllocatedPostMessage(uint32 value) {saRootData.allocatedPostMessage = value;}
utInlineC void saSetFreePostMessage(int32 value) {saRootData.freePostMessage = value;}
utInlineC saSubscription saFirstFreeSubscription(void) {return saRootData.firstFreeSubscription;}
utInlineC void saSetFirstFreeSubscription(saSubscription value) {saRootData.firstFreeSubscription = (value);}
utInlineC uint32 saUsedSubscription(void) {return saRootData.usedSubscription;}
utInlineC uint32 saAllocatedSubscription(void) {return saRootData.allocatedSubscription;}
utInlineC void saSetUsedSubscription(uint32 value) {saRootData.usedSubscription = value;}
utInlineC void saSetAllocatedSubscription(uint32 value) {saRootData.allocatedSubscription = value;}
utInlineC saAnnouncement saFirstFreeAnnouncement(void) {return saRootData.firstFreeAnnouncement;}
utInlineC void saSetFirstFreeAnnouncement(saAnnouncement value) {saRootData.firstFreeAnnouncement = (value);}
utInlineC uint32 saUsedAnnouncement(void) {return saRootData.usedAnnouncement;}
utInlineC uint32 saAllocatedAnnouncement(void) {return saRootData.allocatedAnnouncement;}
utInlineC void saSetUsedAnnouncement(uint32 value) {saRootData.usedAnnouncement = value;}
utInlineC void saSetAllocatedAnnouncement(uint32 value) {saRootData.allocatedAnnouncement = value;}
utInlineC uint32 saUsedAnnouncementSubject(void) {return saRootData.usedAnnouncementSubject;}
utInlineC uint32 saAllocatedAnnouncementSubject(void) {return saRootData.allocatedAnnouncementSubject;}
utInlineC uint32 saFreeAnnouncementSubject(void) {return saRootData.freeAnnouncementSubject;}
utInlineC void saSetUsedAnnouncementSubject(uint32 value) {saRootData.usedAnnouncementSubject = value;}
utInlineC void saSetAllocatedAnnouncementSubject(uint32 value) {saRootData.allocatedAnnouncementSubject = value;}
utInlineC void saSetFreeAnnouncementSubject(int32 value) {saRootData.freeAnnouncementSubject = value;}
utInlineC uint32 saUsedAnnouncementMessage(void) {return saRootData.usedAnnouncementMessage;}
utInlineC uint32 saAllocatedAnnouncementMessage(void) {return saRootData.allocatedAnnouncementMessage;}
utInlineC uint32 saFreeAnnouncementMessage(void) {return saRootData.freeAnnouncementMessage;}
utInlineC void saSetUsedAnnouncementMessage(uint32 value) {saRootData.usedAnnouncementMessage = value;}
utInlineC void saSetAllocatedAnnouncementMessage(uint32 value) {saRootData.allocatedAnnouncementMessage = value;}
utInlineC void saSetFreeAnnouncementMessage(int32 value) {saRootData.freeAnnouncementMessage = value;}
utInlineC saTransactionArray saFirstFreeTransactionArray(void) {return saRootData.firstFreeTransactionArray;}
utInlineC void saSetFirstFreeTransactionArray(saTransactionArray value) {saRootData.firstFreeTransactionArray = (value);}
utInlineC uint32 saUsedTransactionArray(void) {return saRootData.usedTransactionArray;}
utInlineC uint32 saAllocatedTransactionArray(void) {return saRootData.allocatedTransactionArray;}
utInlineC void saSetUsedTransactionArray(uint32 value) {saRootData.usedTransactionArray = value;}
utInlineC void saSetAllocatedTransactionArray(uint32 value) {saRootData.allocatedTransactionArray = value;}
utInlineC uint32 saUsedTransactionArrayTransaction(void) {return saRootData.usedTransactionArrayTransaction;}
utInlineC uint32 saAllocatedTransactionArrayTransaction(void) {return saRootData.allocatedTransactionArrayTransaction;}
utInlineC uint32 saFreeTransactionArrayTransaction(void) {return saRootData.freeTransactionArrayTransaction;}
utInlineC void saSetUsedTransactionArrayTransaction(uint32 value) {saRootData.usedTransactionArrayTransaction = value;}
utInlineC void saSetAllocatedTransactionArrayTransaction(uint32 value) {saRootData.allocatedTransactionArrayTransaction = value;}
utInlineC void saSetFreeTransactionArrayTransaction(int32 value) {saRootData.freeTransactionArrayTransaction = value;}

/* Validate macros */
#if defined(DD_DEBUG)
utInlineC saRoot saValidRoot(saRoot Root) {
    utAssert(utLikely(Root != saRootNull && (uint32)(Root - (saRoot)0) < saRootData.usedRoot));
    return Root;}
utInlineC saRegion saValidRegion(saRegion Region) {
    utAssert(utLikely(Region != saRegionNull && (uint32)(Region - (saRegion)0) < saRootData.usedRegion));
    return Region;}
utInlineC saSession saValidSession(saSession Session) {
    utAssert(utLikely(Session != saSessionNull && (uint32)(Session - (saSession)0) < saRootData.usedSession));
    return Session;}
utInlineC saUser saValidUser(saUser User) {
    utAssert(utLikely(User != saUserNull && (uint32)(User - (saUser)0) < saRootData.usedUser));
    return User;}
utInlineC saTransaction saValidTransaction(saTransaction Transaction) {
    utAssert(utLikely(Transaction != saTransactionNull && (uint32)(Transaction - (saTransaction)0) < saRootData.usedTransaction));
    return Transaction;}
utInlineC saCharity saValidCharity(saCharity Charity) {
    utAssert(utLikely(Charity != saCharityNull && (uint32)(Charity - (saCharity)0) < saRootData.usedCharity));
    return Charity;}
utInlineC saMembership saValidMembership(saMembership Membership) {
    utAssert(utLikely(Membership != saMembershipNull && (uint32)(Membership - (saMembership)0) < saRootData.usedMembership));
    return Membership;}
utInlineC saCategory saValidCategory(saCategory Category) {
    utAssert(utLikely(Category != saCategoryNull && (uint32)(Category - (saCategory)0) < saRootData.usedCategory));
    return Category;}
utInlineC saListing saValidListing(saListing Listing) {
    utAssert(utLikely(Listing != saListingNull && (uint32)(Listing - (saListing)0) < saRootData.usedListing));
    return Listing;}
utInlineC saThread saValidThread(saThread Thread) {
    utAssert(utLikely(Thread != saThreadNull && (uint32)(Thread - (saThread)0) < saRootData.usedThread));
    return Thread;}
utInlineC saPost saValidPost(saPost Post) {
    utAssert(utLikely(Post != saPostNull && (uint32)(Post - (saPost)0) < saRootData.usedPost));
    return Post;}
utInlineC saSubscription saValidSubscription(saSubscription Subscription) {
    utAssert(utLikely(Subscription != saSubscriptionNull && (uint32)(Subscription - (saSubscription)0) < saRootData.usedSubscription));
    return Subscription;}
utInlineC saAnnouncement saValidAnnouncement(saAnnouncement Announcement) {
    utAssert(utLikely(Announcement != saAnnouncementNull && (uint32)(Announcement - (saAnnouncement)0) < saRootData.usedAnnouncement));
    return Announcement;}
utInlineC saTransactionArray saValidTransactionArray(saTransactionArray TransactionArray) {
    utAssert(utLikely(TransactionArray != saTransactionArrayNull && (uint32)(TransactionArray - (saTransactionArray)0) < saRootData.usedTransactionArray));
    return TransactionArray;}
#else
utInlineC saRoot saValidRoot(saRoot Root) {return Root;}
utInlineC saRegion saValidRegion(saRegion Region) {return Region;}
utInlineC saSession saValidSession(saSession Session) {return Session;}
utInlineC saUser saValidUser(saUser User) {return User;}
utInlineC saTransaction saValidTransaction(saTransaction Transaction) {return Transaction;}
utInlineC saCharity saValidCharity(saCharity Charity) {return Charity;}
utInlineC saMembership saValidMembership(saMembership Membership) {return Membership;}
utInlineC saCategory saValidCategory(saCategory Category) {return Category;}
utInlineC saListing saValidListing(saListing Listing) {return Listing;}
utInlineC saThread saValidThread(saThread Thread) {return Thread;}
utInlineC saPost saValidPost(saPost Post) {return Post;}
utInlineC saSubscription saValidSubscription(saSubscription Subscription) {return Subscription;}
utInlineC saAnnouncement saValidAnnouncement(saAnnouncement Announcement) {return Announcement;}
utInlineC saTransactionArray saValidTransactionArray(saTransactionArray TransactionArray) {return TransactionArray;}
#endif

/* Object ref to integer conversions */
#if (defined(DD_DEBUG) && !defined(DD_NOSTRICT)) || defined(DD_STRICT)
utInlineC uint32 saRoot2Index(saRoot Root) {return Root - (saRoot)0;}
utInlineC uint32 saRoot2ValidIndex(saRoot Root) {return saValidRoot(Root) - (saRoot)0;}
utInlineC saRoot saIndex2Root(uint32 xRoot) {return (saRoot)(xRoot + (saRoot)(0));}
utInlineC uint32 saRegion2Index(saRegion Region) {return Region - (saRegion)0;}
utInlineC uint32 saRegion2ValidIndex(saRegion Region) {return saValidRegion(Region) - (saRegion)0;}
utInlineC saRegion saIndex2Region(uint32 xRegion) {return (saRegion)(xRegion + (saRegion)(0));}
utInlineC uint32 saSession2Index(saSession Session) {return Session - (saSession)0;}
utInlineC uint32 saSession2ValidIndex(saSession Session) {return saValidSession(Session) - (saSession)0;}
utInlineC saSession saIndex2Session(uint32 xSession) {return (saSession)(xSession + (saSession)(0));}
utInlineC uint32 saUser2Index(saUser User) {return User - (saUser)0;}
utInlineC uint32 saUser2ValidIndex(saUser User) {return saValidUser(User) - (saUser)0;}
utInlineC saUser saIndex2User(uint32 xUser) {return (saUser)(xUser + (saUser)(0));}
utInlineC uint32 saTransaction2Index(saTransaction Transaction) {return Transaction - (saTransaction)0;}
utInlineC uint32 saTransaction2ValidIndex(saTransaction Transaction) {return saValidTransaction(Transaction) - (saTransaction)0;}
utInlineC saTransaction saIndex2Transaction(uint32 xTransaction) {return (saTransaction)(xTransaction + (saTransaction)(0));}
utInlineC uint32 saCharity2Index(saCharity Charity) {return Charity - (saCharity)0;}
utInlineC uint32 saCharity2ValidIndex(saCharity Charity) {return saValidCharity(Charity) - (saCharity)0;}
utInlineC saCharity saIndex2Charity(uint32 xCharity) {return (saCharity)(xCharity + (saCharity)(0));}
utInlineC uint32 saMembership2Index(saMembership Membership) {return Membership - (saMembership)0;}
utInlineC uint32 saMembership2ValidIndex(saMembership Membership) {return saValidMembership(Membership) - (saMembership)0;}
utInlineC saMembership saIndex2Membership(uint32 xMembership) {return (saMembership)(xMembership + (saMembership)(0));}
utInlineC uint32 saCategory2Index(saCategory Category) {return Category - (saCategory)0;}
utInlineC uint32 saCategory2ValidIndex(saCategory Category) {return saValidCategory(Category) - (saCategory)0;}
utInlineC saCategory saIndex2Category(uint32 xCategory) {return (saCategory)(xCategory + (saCategory)(0));}
utInlineC uint32 saListing2Index(saListing Listing) {return Listing - (saListing)0;}
utInlineC uint32 saListing2ValidIndex(saListing Listing) {return saValidListing(Listing) - (saListing)0;}
utInlineC saListing saIndex2Listing(uint32 xListing) {return (saListing)(xListing + (saListing)(0));}
utInlineC uint32 saThread2Index(saThread Thread) {return Thread - (saThread)0;}
utInlineC uint32 saThread2ValidIndex(saThread Thread) {return saValidThread(Thread) - (saThread)0;}
utInlineC saThread saIndex2Thread(uint32 xThread) {return (saThread)(xThread + (saThread)(0));}
utInlineC uint32 saPost2Index(saPost Post) {return Post - (saPost)0;}
utInlineC uint32 saPost2ValidIndex(saPost Post) {return saValidPost(Post) - (saPost)0;}
utInlineC saPost saIndex2Post(uint32 xPost) {return (saPost)(xPost + (saPost)(0));}
utInlineC uint32 saSubscription2Index(saSubscription Subscription) {return Subscription - (saSubscription)0;}
utInlineC uint32 saSubscription2ValidIndex(saSubscription Subscription) {return saValidSubscription(Subscription) - (saSubscription)0;}
utInlineC saSubscription saIndex2Subscription(uint32 xSubscription) {return (saSubscription)(xSubscription + (saSubscription)(0));}
utInlineC uint32 saAnnouncement2Index(saAnnouncement Announcement) {return Announcement - (saAnnouncement)0;}
utInlineC uint32 saAnnouncement2ValidIndex(saAnnouncement Announcement) {return saValidAnnouncement(Announcement) - (saAnnouncement)0;}
utInlineC saAnnouncement saIndex2Announcement(uint32 xAnnouncement) {return (saAnnouncement)(xAnnouncement + (saAnnouncement)(0));}
utInlineC uint32 saTransactionArray2Index(saTransactionArray TransactionArray) {return TransactionArray - (saTransactionArray)0;}
utInlineC uint32 saTransactionArray2ValidIndex(saTransactionArray TransactionArray) {return saValidTransactionArray(TransactionArray) - (saTransactionArray)0;}
utInlineC saTransactionArray saIndex2TransactionArray(uint32 xTransactionArray) {return (saTransactionArray)(xTransactionArray + (saTransactionArray)(0));}
#else
utInlineC uint32 saRoot2Index(saRoot Root) {return Root;}
utInlineC uint32 saRoot2ValidIndex(saRoot Root) {return saValidRoot(Root);}
utInlineC saRoot saIndex2Root(uint32 xRoot) {return xRoot;}
utInlineC uint32 saRegion2Index(saRegion Region) {return Region;}
utInlineC uint32 saRegion2ValidIndex(saRegion Region) {return saValidRegion(Region);}
utInlineC saRegion saIndex2Region(uint32 xRegion) {return xRegion;}
utInlineC uint32 saSession2Index(saSession Session) {return Session;}
utInlineC uint32 saSession2ValidIndex(saSession Session) {return saValidSession(Session);}
utInlineC saSession saIndex2Session(uint32 xSession) {return xSession;}
utInlineC uint32 saUser2Index(saUser User) {return User;}
utInlineC uint32 saUser2ValidIndex(saUser User) {return saValidUser(User);}
utInlineC saUser saIndex2User(uint32 xUser) {return xUser;}
utInlineC uint32 saTransaction2Index(saTransaction Transaction) {return Transaction;}
utInlineC uint32 saTransaction2ValidIndex(saTransaction Transaction) {return saValidTransaction(Transaction);}
utInlineC saTransaction saIndex2Transaction(uint32 xTransaction) {return xTransaction;}
utInlineC uint32 saCharity2Index(saCharity Charity) {return Charity;}
utInlineC uint32 saCharity2ValidIndex(saCharity Charity) {return saValidCharity(Charity);}
utInlineC saCharity saIndex2Charity(uint32 xCharity) {return xCharity;}
utInlineC uint32 saMembership2Index(saMembership Membership) {return Membership;}
utInlineC uint32 saMembership2ValidIndex(saMembership Membership) {return saValidMembership(Membership);}
utInlineC saMembership saIndex2Membership(uint32 xMembership) {return xMembership;}
utInlineC uint32 saCategory2Index(saCategory Category) {return Category;}
utInlineC uint32 saCategory2ValidIndex(saCategory Category) {return saValidCategory(Category);}
utInlineC saCategory saIndex2Category(uint32 xCategory) {return xCategory;}
utInlineC uint32 saListing2Index(saListing Listing) {return Listing;}
utInlineC uint32 saListing2ValidIndex(saListing Listing) {return saValidListing(Listing);}
utInlineC saListing saIndex2Listing(uint32 xListing) {return xListing;}
utInlineC uint32 saThread2Index(saThread Thread) {return Thread;}
utInlineC uint32 saThread2ValidIndex(saThread Thread) {return saValidThread(Thread);}
utInlineC saThread saIndex2Thread(uint32 xThread) {return xThread;}
utInlineC uint32 saPost2Index(saPost Post) {return Post;}
utInlineC uint32 saPost2ValidIndex(saPost Post) {return saValidPost(Post);}
utInlineC saPost saIndex2Post(uint32 xPost) {return xPost;}
utInlineC uint32 saSubscription2Index(saSubscription Subscription) {return Subscription;}
utInlineC uint32 saSubscription2ValidIndex(saSubscription Subscription) {return saValidSubscription(Subscription);}
utInlineC saSubscription saIndex2Subscription(uint32 xSubscription) {return xSubscription;}
utInlineC uint32 saAnnouncement2Index(saAnnouncement Announcement) {return Announcement;}
utInlineC uint32 saAnnouncement2ValidIndex(saAnnouncement Announcement) {return saValidAnnouncement(Announcement);}
utInlineC saAnnouncement saIndex2Announcement(uint32 xAnnouncement) {return xAnnouncement;}
utInlineC uint32 saTransactionArray2Index(saTransactionArray TransactionArray) {return TransactionArray;}
utInlineC uint32 saTransactionArray2ValidIndex(saTransactionArray TransactionArray) {return saValidTransactionArray(TransactionArray);}
utInlineC saTransactionArray saIndex2TransactionArray(uint32 xTransactionArray) {return xTransactionArray;}
#endif

/*----------------------------------------------------------------------------------------
  Fields for class Root.
----------------------------------------------------------------------------------------*/
struct saRootFields {
    uint64 *NextListingID;
    uint64 *NextThreadID;
    uint64 *NextPostID;
    uint64 *NextAnnouncementID;
    saRegion *FirstRegion;
    saRegion *LastRegion;
    uint32 *RegionTableIndex_;
    uint32 *NumRegionTable;
    saRegion *RegionTable;
    uint32 *NumRegion;
    saUser *FirstUser;
    saUser *LastUser;
    uint32 *UserTableIndex_;
    uint32 *NumUserTable;
    saUser *UserTable;
    uint32 *NumUser;
    saUser *FirstByEmailUser;
    saUser *LastByEmailUser;
    uint32 *ByEmailUserTableIndex_;
    uint32 *NumByEmailUserTable;
    saUser *ByEmailUserTable;
    uint32 *NumByEmailUser;
    saCharity *FirstCharity;
    saCharity *LastCharity;
    saCategory *FirstCategory;
    saCategory *LastCategory;
    uint32 *CategoryTableIndex_;
    uint32 *NumCategoryTable;
    saCategory *CategoryTable;
    uint32 *NumCategory;
    saListing *FirstListing;
    saListing *LastListing;
    uint32 *ListingTableIndex_;
    uint32 *NumListingTable;
    saListing *ListingTable;
    uint32 *NumListing;
    saSession *FirstSession;
    saSession *LastSession;
    uint32 *SessionTableIndex_;
    uint32 *NumSessionTable;
    saSession *SessionTable;
    uint32 *NumSession;
    saThread *FirstThread;
    saThread *LastThread;
    uint32 *ThreadTableIndex_;
    uint32 *NumThreadTable;
    saThread *ThreadTable;
    uint32 *NumThread;
    saPost *FirstPost;
    saPost *LastPost;
    uint32 *PostTableIndex_;
    uint32 *NumPostTable;
    saPost *PostTable;
    uint32 *NumPost;
    saAnnouncement *FirstAnnouncement;
    saAnnouncement *LastAnnouncement;
    uint32 *AnnouncementTableIndex_;
    uint32 *NumAnnouncementTable;
    saAnnouncement *AnnouncementTable;
    uint32 *NumAnnouncement;
};
extern struct saRootFields saRoots;

void saRootAllocMore(void);
void saRootCopyProps(saRoot saOldRoot, saRoot saNewRoot);
void saRootAllocRegionTables(saRoot Root, uint32 numRegionTables);
void saRootResizeRegionTables(saRoot Root, uint32 numRegionTables);
void saRootFreeRegionTables(saRoot Root);
void saCompactRootRegionTables(void);
void saRootAllocUserTables(saRoot Root, uint32 numUserTables);
void saRootResizeUserTables(saRoot Root, uint32 numUserTables);
void saRootFreeUserTables(saRoot Root);
void saCompactRootUserTables(void);
void saRootAllocByEmailUserTables(saRoot Root, uint32 numByEmailUserTables);
void saRootResizeByEmailUserTables(saRoot Root, uint32 numByEmailUserTables);
void saRootFreeByEmailUserTables(saRoot Root);
void saCompactRootByEmailUserTables(void);
void saRootAllocCategoryTables(saRoot Root, uint32 numCategoryTables);
void saRootResizeCategoryTables(saRoot Root, uint32 numCategoryTables);
void saRootFreeCategoryTables(saRoot Root);
void saCompactRootCategoryTables(void);
void saRootAllocListingTables(saRoot Root, uint32 numListingTables);
void saRootResizeListingTables(saRoot Root, uint32 numListingTables);
void saRootFreeListingTables(saRoot Root);
void saCompactRootListingTables(void);
void saRootAllocSessionTables(saRoot Root, uint32 numSessionTables);
void saRootResizeSessionTables(saRoot Root, uint32 numSessionTables);
void saRootFreeSessionTables(saRoot Root);
void saCompactRootSessionTables(void);
void saRootAllocThreadTables(saRoot Root, uint32 numThreadTables);
void saRootResizeThreadTables(saRoot Root, uint32 numThreadTables);
void saRootFreeThreadTables(saRoot Root);
void saCompactRootThreadTables(void);
void saRootAllocPostTables(saRoot Root, uint32 numPostTables);
void saRootResizePostTables(saRoot Root, uint32 numPostTables);
void saRootFreePostTables(saRoot Root);
void saCompactRootPostTables(void);
void saRootAllocAnnouncementTables(saRoot Root, uint32 numAnnouncementTables);
void saRootResizeAnnouncementTables(saRoot Root, uint32 numAnnouncementTables);
void saRootFreeAnnouncementTables(saRoot Root);
void saCompactRootAnnouncementTables(void);
utInlineC uint64 saRootGetNextListingID(saRoot Root) {return saRoots.NextListingID[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNextListingID(saRoot Root, uint64 value) {saRoots.NextListingID[saRoot2ValidIndex(Root)] = value;}
utInlineC uint64 saRootGetNextThreadID(saRoot Root) {return saRoots.NextThreadID[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNextThreadID(saRoot Root, uint64 value) {saRoots.NextThreadID[saRoot2ValidIndex(Root)] = value;}
utInlineC uint64 saRootGetNextPostID(saRoot Root) {return saRoots.NextPostID[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNextPostID(saRoot Root, uint64 value) {saRoots.NextPostID[saRoot2ValidIndex(Root)] = value;}
utInlineC uint64 saRootGetNextAnnouncementID(saRoot Root) {return saRoots.NextAnnouncementID[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNextAnnouncementID(saRoot Root, uint64 value) {saRoots.NextAnnouncementID[saRoot2ValidIndex(Root)] = value;}
utInlineC saRegion saRootGetFirstRegion(saRoot Root) {return saRoots.FirstRegion[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetFirstRegion(saRoot Root, saRegion value) {saRoots.FirstRegion[saRoot2ValidIndex(Root)] = value;}
utInlineC saRegion saRootGetLastRegion(saRoot Root) {return saRoots.LastRegion[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetLastRegion(saRoot Root, saRegion value) {saRoots.LastRegion[saRoot2ValidIndex(Root)] = value;}
utInlineC uint32 saRootGetRegionTableIndex_(saRoot Root) {return saRoots.RegionTableIndex_[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetRegionTableIndex_(saRoot Root, uint32 value) {saRoots.RegionTableIndex_[saRoot2ValidIndex(Root)] = value;}
utInlineC uint32 saRootGetNumRegionTable(saRoot Root) {return saRoots.NumRegionTable[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNumRegionTable(saRoot Root, uint32 value) {saRoots.NumRegionTable[saRoot2ValidIndex(Root)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saRootCheckRegionTableIndex(saRoot Root, uint32 x) {utAssert(x < saRootGetNumRegionTable(Root)); return x;}
#else
utInlineC uint32 saRootCheckRegionTableIndex(saRoot Root, uint32 x) {return x;}
#endif
utInlineC saRegion saRootGetiRegionTable(saRoot Root, uint32 x) {return saRoots.RegionTable[
    saRootGetRegionTableIndex_(Root) + saRootCheckRegionTableIndex(Root, x)];}
utInlineC saRegion *saRootGetRegionTable(saRoot Root) {return saRoots.RegionTable + saRootGetRegionTableIndex_(Root);}
#define saRootGetRegionTables saRootGetRegionTable
utInlineC void saRootSetRegionTable(saRoot Root, saRegion *valuePtr, uint32 numRegionTable) {
    saRootResizeRegionTables(Root, numRegionTable);
    memcpy(saRootGetRegionTables(Root), valuePtr, numRegionTable*sizeof(saRegion));}
utInlineC void saRootSetiRegionTable(saRoot Root, uint32 x, saRegion value) {
    saRoots.RegionTable[saRootGetRegionTableIndex_(Root) + saRootCheckRegionTableIndex(Root, (x))] = value;}
utInlineC uint32 saRootGetNumRegion(saRoot Root) {return saRoots.NumRegion[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNumRegion(saRoot Root, uint32 value) {saRoots.NumRegion[saRoot2ValidIndex(Root)] = value;}
utInlineC saUser saRootGetFirstUser(saRoot Root) {return saRoots.FirstUser[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetFirstUser(saRoot Root, saUser value) {saRoots.FirstUser[saRoot2ValidIndex(Root)] = value;}
utInlineC saUser saRootGetLastUser(saRoot Root) {return saRoots.LastUser[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetLastUser(saRoot Root, saUser value) {saRoots.LastUser[saRoot2ValidIndex(Root)] = value;}
utInlineC uint32 saRootGetUserTableIndex_(saRoot Root) {return saRoots.UserTableIndex_[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetUserTableIndex_(saRoot Root, uint32 value) {saRoots.UserTableIndex_[saRoot2ValidIndex(Root)] = value;}
utInlineC uint32 saRootGetNumUserTable(saRoot Root) {return saRoots.NumUserTable[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNumUserTable(saRoot Root, uint32 value) {saRoots.NumUserTable[saRoot2ValidIndex(Root)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saRootCheckUserTableIndex(saRoot Root, uint32 x) {utAssert(x < saRootGetNumUserTable(Root)); return x;}
#else
utInlineC uint32 saRootCheckUserTableIndex(saRoot Root, uint32 x) {return x;}
#endif
utInlineC saUser saRootGetiUserTable(saRoot Root, uint32 x) {return saRoots.UserTable[
    saRootGetUserTableIndex_(Root) + saRootCheckUserTableIndex(Root, x)];}
utInlineC saUser *saRootGetUserTable(saRoot Root) {return saRoots.UserTable + saRootGetUserTableIndex_(Root);}
#define saRootGetUserTables saRootGetUserTable
utInlineC void saRootSetUserTable(saRoot Root, saUser *valuePtr, uint32 numUserTable) {
    saRootResizeUserTables(Root, numUserTable);
    memcpy(saRootGetUserTables(Root), valuePtr, numUserTable*sizeof(saUser));}
utInlineC void saRootSetiUserTable(saRoot Root, uint32 x, saUser value) {
    saRoots.UserTable[saRootGetUserTableIndex_(Root) + saRootCheckUserTableIndex(Root, (x))] = value;}
utInlineC uint32 saRootGetNumUser(saRoot Root) {return saRoots.NumUser[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNumUser(saRoot Root, uint32 value) {saRoots.NumUser[saRoot2ValidIndex(Root)] = value;}
utInlineC saUser saRootGetFirstByEmailUser(saRoot Root) {return saRoots.FirstByEmailUser[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetFirstByEmailUser(saRoot Root, saUser value) {saRoots.FirstByEmailUser[saRoot2ValidIndex(Root)] = value;}
utInlineC saUser saRootGetLastByEmailUser(saRoot Root) {return saRoots.LastByEmailUser[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetLastByEmailUser(saRoot Root, saUser value) {saRoots.LastByEmailUser[saRoot2ValidIndex(Root)] = value;}
utInlineC uint32 saRootGetByEmailUserTableIndex_(saRoot Root) {return saRoots.ByEmailUserTableIndex_[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetByEmailUserTableIndex_(saRoot Root, uint32 value) {saRoots.ByEmailUserTableIndex_[saRoot2ValidIndex(Root)] = value;}
utInlineC uint32 saRootGetNumByEmailUserTable(saRoot Root) {return saRoots.NumByEmailUserTable[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNumByEmailUserTable(saRoot Root, uint32 value) {saRoots.NumByEmailUserTable[saRoot2ValidIndex(Root)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saRootCheckByEmailUserTableIndex(saRoot Root, uint32 x) {utAssert(x < saRootGetNumByEmailUserTable(Root)); return x;}
#else
utInlineC uint32 saRootCheckByEmailUserTableIndex(saRoot Root, uint32 x) {return x;}
#endif
utInlineC saUser saRootGetiByEmailUserTable(saRoot Root, uint32 x) {return saRoots.ByEmailUserTable[
    saRootGetByEmailUserTableIndex_(Root) + saRootCheckByEmailUserTableIndex(Root, x)];}
utInlineC saUser *saRootGetByEmailUserTable(saRoot Root) {return saRoots.ByEmailUserTable + saRootGetByEmailUserTableIndex_(Root);}
#define saRootGetByEmailUserTables saRootGetByEmailUserTable
utInlineC void saRootSetByEmailUserTable(saRoot Root, saUser *valuePtr, uint32 numByEmailUserTable) {
    saRootResizeByEmailUserTables(Root, numByEmailUserTable);
    memcpy(saRootGetByEmailUserTables(Root), valuePtr, numByEmailUserTable*sizeof(saUser));}
utInlineC void saRootSetiByEmailUserTable(saRoot Root, uint32 x, saUser value) {
    saRoots.ByEmailUserTable[saRootGetByEmailUserTableIndex_(Root) + saRootCheckByEmailUserTableIndex(Root, (x))] = value;}
utInlineC uint32 saRootGetNumByEmailUser(saRoot Root) {return saRoots.NumByEmailUser[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNumByEmailUser(saRoot Root, uint32 value) {saRoots.NumByEmailUser[saRoot2ValidIndex(Root)] = value;}
utInlineC saCharity saRootGetFirstCharity(saRoot Root) {return saRoots.FirstCharity[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetFirstCharity(saRoot Root, saCharity value) {saRoots.FirstCharity[saRoot2ValidIndex(Root)] = value;}
utInlineC saCharity saRootGetLastCharity(saRoot Root) {return saRoots.LastCharity[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetLastCharity(saRoot Root, saCharity value) {saRoots.LastCharity[saRoot2ValidIndex(Root)] = value;}
utInlineC saCategory saRootGetFirstCategory(saRoot Root) {return saRoots.FirstCategory[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetFirstCategory(saRoot Root, saCategory value) {saRoots.FirstCategory[saRoot2ValidIndex(Root)] = value;}
utInlineC saCategory saRootGetLastCategory(saRoot Root) {return saRoots.LastCategory[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetLastCategory(saRoot Root, saCategory value) {saRoots.LastCategory[saRoot2ValidIndex(Root)] = value;}
utInlineC uint32 saRootGetCategoryTableIndex_(saRoot Root) {return saRoots.CategoryTableIndex_[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetCategoryTableIndex_(saRoot Root, uint32 value) {saRoots.CategoryTableIndex_[saRoot2ValidIndex(Root)] = value;}
utInlineC uint32 saRootGetNumCategoryTable(saRoot Root) {return saRoots.NumCategoryTable[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNumCategoryTable(saRoot Root, uint32 value) {saRoots.NumCategoryTable[saRoot2ValidIndex(Root)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saRootCheckCategoryTableIndex(saRoot Root, uint32 x) {utAssert(x < saRootGetNumCategoryTable(Root)); return x;}
#else
utInlineC uint32 saRootCheckCategoryTableIndex(saRoot Root, uint32 x) {return x;}
#endif
utInlineC saCategory saRootGetiCategoryTable(saRoot Root, uint32 x) {return saRoots.CategoryTable[
    saRootGetCategoryTableIndex_(Root) + saRootCheckCategoryTableIndex(Root, x)];}
utInlineC saCategory *saRootGetCategoryTable(saRoot Root) {return saRoots.CategoryTable + saRootGetCategoryTableIndex_(Root);}
#define saRootGetCategoryTables saRootGetCategoryTable
utInlineC void saRootSetCategoryTable(saRoot Root, saCategory *valuePtr, uint32 numCategoryTable) {
    saRootResizeCategoryTables(Root, numCategoryTable);
    memcpy(saRootGetCategoryTables(Root), valuePtr, numCategoryTable*sizeof(saCategory));}
utInlineC void saRootSetiCategoryTable(saRoot Root, uint32 x, saCategory value) {
    saRoots.CategoryTable[saRootGetCategoryTableIndex_(Root) + saRootCheckCategoryTableIndex(Root, (x))] = value;}
utInlineC uint32 saRootGetNumCategory(saRoot Root) {return saRoots.NumCategory[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNumCategory(saRoot Root, uint32 value) {saRoots.NumCategory[saRoot2ValidIndex(Root)] = value;}
utInlineC saListing saRootGetFirstListing(saRoot Root) {return saRoots.FirstListing[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetFirstListing(saRoot Root, saListing value) {saRoots.FirstListing[saRoot2ValidIndex(Root)] = value;}
utInlineC saListing saRootGetLastListing(saRoot Root) {return saRoots.LastListing[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetLastListing(saRoot Root, saListing value) {saRoots.LastListing[saRoot2ValidIndex(Root)] = value;}
utInlineC uint32 saRootGetListingTableIndex_(saRoot Root) {return saRoots.ListingTableIndex_[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetListingTableIndex_(saRoot Root, uint32 value) {saRoots.ListingTableIndex_[saRoot2ValidIndex(Root)] = value;}
utInlineC uint32 saRootGetNumListingTable(saRoot Root) {return saRoots.NumListingTable[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNumListingTable(saRoot Root, uint32 value) {saRoots.NumListingTable[saRoot2ValidIndex(Root)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saRootCheckListingTableIndex(saRoot Root, uint32 x) {utAssert(x < saRootGetNumListingTable(Root)); return x;}
#else
utInlineC uint32 saRootCheckListingTableIndex(saRoot Root, uint32 x) {return x;}
#endif
utInlineC saListing saRootGetiListingTable(saRoot Root, uint32 x) {return saRoots.ListingTable[
    saRootGetListingTableIndex_(Root) + saRootCheckListingTableIndex(Root, x)];}
utInlineC saListing *saRootGetListingTable(saRoot Root) {return saRoots.ListingTable + saRootGetListingTableIndex_(Root);}
#define saRootGetListingTables saRootGetListingTable
utInlineC void saRootSetListingTable(saRoot Root, saListing *valuePtr, uint32 numListingTable) {
    saRootResizeListingTables(Root, numListingTable);
    memcpy(saRootGetListingTables(Root), valuePtr, numListingTable*sizeof(saListing));}
utInlineC void saRootSetiListingTable(saRoot Root, uint32 x, saListing value) {
    saRoots.ListingTable[saRootGetListingTableIndex_(Root) + saRootCheckListingTableIndex(Root, (x))] = value;}
utInlineC uint32 saRootGetNumListing(saRoot Root) {return saRoots.NumListing[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNumListing(saRoot Root, uint32 value) {saRoots.NumListing[saRoot2ValidIndex(Root)] = value;}
utInlineC saSession saRootGetFirstSession(saRoot Root) {return saRoots.FirstSession[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetFirstSession(saRoot Root, saSession value) {saRoots.FirstSession[saRoot2ValidIndex(Root)] = value;}
utInlineC saSession saRootGetLastSession(saRoot Root) {return saRoots.LastSession[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetLastSession(saRoot Root, saSession value) {saRoots.LastSession[saRoot2ValidIndex(Root)] = value;}
utInlineC uint32 saRootGetSessionTableIndex_(saRoot Root) {return saRoots.SessionTableIndex_[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetSessionTableIndex_(saRoot Root, uint32 value) {saRoots.SessionTableIndex_[saRoot2ValidIndex(Root)] = value;}
utInlineC uint32 saRootGetNumSessionTable(saRoot Root) {return saRoots.NumSessionTable[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNumSessionTable(saRoot Root, uint32 value) {saRoots.NumSessionTable[saRoot2ValidIndex(Root)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saRootCheckSessionTableIndex(saRoot Root, uint32 x) {utAssert(x < saRootGetNumSessionTable(Root)); return x;}
#else
utInlineC uint32 saRootCheckSessionTableIndex(saRoot Root, uint32 x) {return x;}
#endif
utInlineC saSession saRootGetiSessionTable(saRoot Root, uint32 x) {return saRoots.SessionTable[
    saRootGetSessionTableIndex_(Root) + saRootCheckSessionTableIndex(Root, x)];}
utInlineC saSession *saRootGetSessionTable(saRoot Root) {return saRoots.SessionTable + saRootGetSessionTableIndex_(Root);}
#define saRootGetSessionTables saRootGetSessionTable
utInlineC void saRootSetSessionTable(saRoot Root, saSession *valuePtr, uint32 numSessionTable) {
    saRootResizeSessionTables(Root, numSessionTable);
    memcpy(saRootGetSessionTables(Root), valuePtr, numSessionTable*sizeof(saSession));}
utInlineC void saRootSetiSessionTable(saRoot Root, uint32 x, saSession value) {
    saRoots.SessionTable[saRootGetSessionTableIndex_(Root) + saRootCheckSessionTableIndex(Root, (x))] = value;}
utInlineC uint32 saRootGetNumSession(saRoot Root) {return saRoots.NumSession[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNumSession(saRoot Root, uint32 value) {saRoots.NumSession[saRoot2ValidIndex(Root)] = value;}
utInlineC saThread saRootGetFirstThread(saRoot Root) {return saRoots.FirstThread[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetFirstThread(saRoot Root, saThread value) {saRoots.FirstThread[saRoot2ValidIndex(Root)] = value;}
utInlineC saThread saRootGetLastThread(saRoot Root) {return saRoots.LastThread[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetLastThread(saRoot Root, saThread value) {saRoots.LastThread[saRoot2ValidIndex(Root)] = value;}
utInlineC uint32 saRootGetThreadTableIndex_(saRoot Root) {return saRoots.ThreadTableIndex_[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetThreadTableIndex_(saRoot Root, uint32 value) {saRoots.ThreadTableIndex_[saRoot2ValidIndex(Root)] = value;}
utInlineC uint32 saRootGetNumThreadTable(saRoot Root) {return saRoots.NumThreadTable[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNumThreadTable(saRoot Root, uint32 value) {saRoots.NumThreadTable[saRoot2ValidIndex(Root)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saRootCheckThreadTableIndex(saRoot Root, uint32 x) {utAssert(x < saRootGetNumThreadTable(Root)); return x;}
#else
utInlineC uint32 saRootCheckThreadTableIndex(saRoot Root, uint32 x) {return x;}
#endif
utInlineC saThread saRootGetiThreadTable(saRoot Root, uint32 x) {return saRoots.ThreadTable[
    saRootGetThreadTableIndex_(Root) + saRootCheckThreadTableIndex(Root, x)];}
utInlineC saThread *saRootGetThreadTable(saRoot Root) {return saRoots.ThreadTable + saRootGetThreadTableIndex_(Root);}
#define saRootGetThreadTables saRootGetThreadTable
utInlineC void saRootSetThreadTable(saRoot Root, saThread *valuePtr, uint32 numThreadTable) {
    saRootResizeThreadTables(Root, numThreadTable);
    memcpy(saRootGetThreadTables(Root), valuePtr, numThreadTable*sizeof(saThread));}
utInlineC void saRootSetiThreadTable(saRoot Root, uint32 x, saThread value) {
    saRoots.ThreadTable[saRootGetThreadTableIndex_(Root) + saRootCheckThreadTableIndex(Root, (x))] = value;}
utInlineC uint32 saRootGetNumThread(saRoot Root) {return saRoots.NumThread[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNumThread(saRoot Root, uint32 value) {saRoots.NumThread[saRoot2ValidIndex(Root)] = value;}
utInlineC saPost saRootGetFirstPost(saRoot Root) {return saRoots.FirstPost[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetFirstPost(saRoot Root, saPost value) {saRoots.FirstPost[saRoot2ValidIndex(Root)] = value;}
utInlineC saPost saRootGetLastPost(saRoot Root) {return saRoots.LastPost[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetLastPost(saRoot Root, saPost value) {saRoots.LastPost[saRoot2ValidIndex(Root)] = value;}
utInlineC uint32 saRootGetPostTableIndex_(saRoot Root) {return saRoots.PostTableIndex_[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetPostTableIndex_(saRoot Root, uint32 value) {saRoots.PostTableIndex_[saRoot2ValidIndex(Root)] = value;}
utInlineC uint32 saRootGetNumPostTable(saRoot Root) {return saRoots.NumPostTable[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNumPostTable(saRoot Root, uint32 value) {saRoots.NumPostTable[saRoot2ValidIndex(Root)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saRootCheckPostTableIndex(saRoot Root, uint32 x) {utAssert(x < saRootGetNumPostTable(Root)); return x;}
#else
utInlineC uint32 saRootCheckPostTableIndex(saRoot Root, uint32 x) {return x;}
#endif
utInlineC saPost saRootGetiPostTable(saRoot Root, uint32 x) {return saRoots.PostTable[
    saRootGetPostTableIndex_(Root) + saRootCheckPostTableIndex(Root, x)];}
utInlineC saPost *saRootGetPostTable(saRoot Root) {return saRoots.PostTable + saRootGetPostTableIndex_(Root);}
#define saRootGetPostTables saRootGetPostTable
utInlineC void saRootSetPostTable(saRoot Root, saPost *valuePtr, uint32 numPostTable) {
    saRootResizePostTables(Root, numPostTable);
    memcpy(saRootGetPostTables(Root), valuePtr, numPostTable*sizeof(saPost));}
utInlineC void saRootSetiPostTable(saRoot Root, uint32 x, saPost value) {
    saRoots.PostTable[saRootGetPostTableIndex_(Root) + saRootCheckPostTableIndex(Root, (x))] = value;}
utInlineC uint32 saRootGetNumPost(saRoot Root) {return saRoots.NumPost[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNumPost(saRoot Root, uint32 value) {saRoots.NumPost[saRoot2ValidIndex(Root)] = value;}
utInlineC saAnnouncement saRootGetFirstAnnouncement(saRoot Root) {return saRoots.FirstAnnouncement[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetFirstAnnouncement(saRoot Root, saAnnouncement value) {saRoots.FirstAnnouncement[saRoot2ValidIndex(Root)] = value;}
utInlineC saAnnouncement saRootGetLastAnnouncement(saRoot Root) {return saRoots.LastAnnouncement[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetLastAnnouncement(saRoot Root, saAnnouncement value) {saRoots.LastAnnouncement[saRoot2ValidIndex(Root)] = value;}
utInlineC uint32 saRootGetAnnouncementTableIndex_(saRoot Root) {return saRoots.AnnouncementTableIndex_[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetAnnouncementTableIndex_(saRoot Root, uint32 value) {saRoots.AnnouncementTableIndex_[saRoot2ValidIndex(Root)] = value;}
utInlineC uint32 saRootGetNumAnnouncementTable(saRoot Root) {return saRoots.NumAnnouncementTable[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNumAnnouncementTable(saRoot Root, uint32 value) {saRoots.NumAnnouncementTable[saRoot2ValidIndex(Root)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saRootCheckAnnouncementTableIndex(saRoot Root, uint32 x) {utAssert(x < saRootGetNumAnnouncementTable(Root)); return x;}
#else
utInlineC uint32 saRootCheckAnnouncementTableIndex(saRoot Root, uint32 x) {return x;}
#endif
utInlineC saAnnouncement saRootGetiAnnouncementTable(saRoot Root, uint32 x) {return saRoots.AnnouncementTable[
    saRootGetAnnouncementTableIndex_(Root) + saRootCheckAnnouncementTableIndex(Root, x)];}
utInlineC saAnnouncement *saRootGetAnnouncementTable(saRoot Root) {return saRoots.AnnouncementTable + saRootGetAnnouncementTableIndex_(Root);}
#define saRootGetAnnouncementTables saRootGetAnnouncementTable
utInlineC void saRootSetAnnouncementTable(saRoot Root, saAnnouncement *valuePtr, uint32 numAnnouncementTable) {
    saRootResizeAnnouncementTables(Root, numAnnouncementTable);
    memcpy(saRootGetAnnouncementTables(Root), valuePtr, numAnnouncementTable*sizeof(saAnnouncement));}
utInlineC void saRootSetiAnnouncementTable(saRoot Root, uint32 x, saAnnouncement value) {
    saRoots.AnnouncementTable[saRootGetAnnouncementTableIndex_(Root) + saRootCheckAnnouncementTableIndex(Root, (x))] = value;}
utInlineC uint32 saRootGetNumAnnouncement(saRoot Root) {return saRoots.NumAnnouncement[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNumAnnouncement(saRoot Root, uint32 value) {saRoots.NumAnnouncement[saRoot2ValidIndex(Root)] = value;}
utInlineC void saRootSetConstructorCallback(void(*func)(saRoot)) {saRootConstructorCallback = func;}
utInlineC saRootCallbackType saRootGetConstructorCallback(void) {return saRootConstructorCallback;}
utInlineC void saRootSetDestructorCallback(void(*func)(saRoot)) {saRootDestructorCallback = func;}
utInlineC saRootCallbackType saRootGetDestructorCallback(void) {return saRootDestructorCallback;}
utInlineC saRoot saRootNextFree(saRoot Root) {return ((saRoot *)(void *)(saRoots.FirstRegion))[saRoot2ValidIndex(Root)];}
utInlineC void saRootSetNextFree(saRoot Root, saRoot value) {
    ((saRoot *)(void *)(saRoots.FirstRegion))[saRoot2ValidIndex(Root)] = value;}
utInlineC void saRootFree(saRoot Root) {
    saRootFreeRegionTables(Root);
    saRootFreeUserTables(Root);
    saRootFreeByEmailUserTables(Root);
    saRootFreeCategoryTables(Root);
    saRootFreeListingTables(Root);
    saRootFreeSessionTables(Root);
    saRootFreeThreadTables(Root);
    saRootFreePostTables(Root);
    saRootFreeAnnouncementTables(Root);
    saRootSetNextFree(Root, saRootData.firstFreeRoot);
    saSetFirstFreeRoot(Root);}
void saRootDestroy(saRoot Root);
utInlineC saRoot saRootAllocRaw(void) {
    saRoot Root;
    if(saRootData.firstFreeRoot != saRootNull) {
        Root = saRootData.firstFreeRoot;
        saSetFirstFreeRoot(saRootNextFree(Root));
    } else {
        if(saRootData.usedRoot == saRootData.allocatedRoot) {
            saRootAllocMore();
        }
        Root = saIndex2Root(saRootData.usedRoot);
        saSetUsedRoot(saUsedRoot() + 1);
    }
    return Root;}
utInlineC saRoot saRootAlloc(void) {
    saRoot Root = saRootAllocRaw();
    saRootSetNextListingID(Root, 0);
    saRootSetNextThreadID(Root, 0);
    saRootSetNextPostID(Root, 0);
    saRootSetNextAnnouncementID(Root, 0);
    saRootSetFirstRegion(Root, saRegionNull);
    saRootSetLastRegion(Root, saRegionNull);
    saRootSetRegionTableIndex_(Root, 0);
    saRootSetNumRegionTable(Root, 0);
    saRootSetNumRegionTable(Root, 0);
    saRootSetNumRegion(Root, 0);
    saRootSetFirstUser(Root, saUserNull);
    saRootSetLastUser(Root, saUserNull);
    saRootSetUserTableIndex_(Root, 0);
    saRootSetNumUserTable(Root, 0);
    saRootSetNumUserTable(Root, 0);
    saRootSetNumUser(Root, 0);
    saRootSetFirstByEmailUser(Root, saUserNull);
    saRootSetLastByEmailUser(Root, saUserNull);
    saRootSetByEmailUserTableIndex_(Root, 0);
    saRootSetNumByEmailUserTable(Root, 0);
    saRootSetNumByEmailUserTable(Root, 0);
    saRootSetNumByEmailUser(Root, 0);
    saRootSetFirstCharity(Root, saCharityNull);
    saRootSetLastCharity(Root, saCharityNull);
    saRootSetFirstCategory(Root, saCategoryNull);
    saRootSetLastCategory(Root, saCategoryNull);
    saRootSetCategoryTableIndex_(Root, 0);
    saRootSetNumCategoryTable(Root, 0);
    saRootSetNumCategoryTable(Root, 0);
    saRootSetNumCategory(Root, 0);
    saRootSetFirstListing(Root, saListingNull);
    saRootSetLastListing(Root, saListingNull);
    saRootSetListingTableIndex_(Root, 0);
    saRootSetNumListingTable(Root, 0);
    saRootSetNumListingTable(Root, 0);
    saRootSetNumListing(Root, 0);
    saRootSetFirstSession(Root, saSessionNull);
    saRootSetLastSession(Root, saSessionNull);
    saRootSetSessionTableIndex_(Root, 0);
    saRootSetNumSessionTable(Root, 0);
    saRootSetNumSessionTable(Root, 0);
    saRootSetNumSession(Root, 0);
    saRootSetFirstThread(Root, saThreadNull);
    saRootSetLastThread(Root, saThreadNull);
    saRootSetThreadTableIndex_(Root, 0);
    saRootSetNumThreadTable(Root, 0);
    saRootSetNumThreadTable(Root, 0);
    saRootSetNumThread(Root, 0);
    saRootSetFirstPost(Root, saPostNull);
    saRootSetLastPost(Root, saPostNull);
    saRootSetPostTableIndex_(Root, 0);
    saRootSetNumPostTable(Root, 0);
    saRootSetNumPostTable(Root, 0);
    saRootSetNumPost(Root, 0);
    saRootSetFirstAnnouncement(Root, saAnnouncementNull);
    saRootSetLastAnnouncement(Root, saAnnouncementNull);
    saRootSetAnnouncementTableIndex_(Root, 0);
    saRootSetNumAnnouncementTable(Root, 0);
    saRootSetNumAnnouncementTable(Root, 0);
    saRootSetNumAnnouncement(Root, 0);
    if(saRootConstructorCallback != NULL) {
        saRootConstructorCallback(Root);
    }
    return Root;}

/*----------------------------------------------------------------------------------------
  Fields for class Region.
----------------------------------------------------------------------------------------*/
struct saRegionFields {
    utSym *Sym;
    uint32 *NumListings;
    saRoot *Root;
    saRegion *NextRootRegion;
    saRegion *PrevRootRegion;
    saRegion *NextTableRootRegion;
    saUser *FirstUser;
    saUser *LastUser;
    saSession *Session;
};
extern struct saRegionFields saRegions;

void saRegionAllocMore(void);
void saRegionCopyProps(saRegion saOldRegion, saRegion saNewRegion);
utInlineC utSym saRegionGetSym(saRegion Region) {return saRegions.Sym[saRegion2ValidIndex(Region)];}
utInlineC void saRegionSetSym(saRegion Region, utSym value) {saRegions.Sym[saRegion2ValidIndex(Region)] = value;}
utInlineC uint32 saRegionGetNumListings(saRegion Region) {return saRegions.NumListings[saRegion2ValidIndex(Region)];}
utInlineC void saRegionSetNumListings(saRegion Region, uint32 value) {saRegions.NumListings[saRegion2ValidIndex(Region)] = value;}
utInlineC saRoot saRegionGetRoot(saRegion Region) {return saRegions.Root[saRegion2ValidIndex(Region)];}
utInlineC void saRegionSetRoot(saRegion Region, saRoot value) {saRegions.Root[saRegion2ValidIndex(Region)] = value;}
utInlineC saRegion saRegionGetNextRootRegion(saRegion Region) {return saRegions.NextRootRegion[saRegion2ValidIndex(Region)];}
utInlineC void saRegionSetNextRootRegion(saRegion Region, saRegion value) {saRegions.NextRootRegion[saRegion2ValidIndex(Region)] = value;}
utInlineC saRegion saRegionGetPrevRootRegion(saRegion Region) {return saRegions.PrevRootRegion[saRegion2ValidIndex(Region)];}
utInlineC void saRegionSetPrevRootRegion(saRegion Region, saRegion value) {saRegions.PrevRootRegion[saRegion2ValidIndex(Region)] = value;}
utInlineC saRegion saRegionGetNextTableRootRegion(saRegion Region) {return saRegions.NextTableRootRegion[saRegion2ValidIndex(Region)];}
utInlineC void saRegionSetNextTableRootRegion(saRegion Region, saRegion value) {saRegions.NextTableRootRegion[saRegion2ValidIndex(Region)] = value;}
utInlineC saUser saRegionGetFirstUser(saRegion Region) {return saRegions.FirstUser[saRegion2ValidIndex(Region)];}
utInlineC void saRegionSetFirstUser(saRegion Region, saUser value) {saRegions.FirstUser[saRegion2ValidIndex(Region)] = value;}
utInlineC saUser saRegionGetLastUser(saRegion Region) {return saRegions.LastUser[saRegion2ValidIndex(Region)];}
utInlineC void saRegionSetLastUser(saRegion Region, saUser value) {saRegions.LastUser[saRegion2ValidIndex(Region)] = value;}
utInlineC saSession saRegionGetSession(saRegion Region) {return saRegions.Session[saRegion2ValidIndex(Region)];}
utInlineC void saRegionSetSession(saRegion Region, saSession value) {saRegions.Session[saRegion2ValidIndex(Region)] = value;}
utInlineC void saRegionSetConstructorCallback(void(*func)(saRegion)) {saRegionConstructorCallback = func;}
utInlineC saRegionCallbackType saRegionGetConstructorCallback(void) {return saRegionConstructorCallback;}
utInlineC void saRegionSetDestructorCallback(void(*func)(saRegion)) {saRegionDestructorCallback = func;}
utInlineC saRegionCallbackType saRegionGetDestructorCallback(void) {return saRegionDestructorCallback;}
utInlineC saRegion saRegionNextFree(saRegion Region) {return ((saRegion *)(void *)(saRegions.Sym))[saRegion2ValidIndex(Region)];}
utInlineC void saRegionSetNextFree(saRegion Region, saRegion value) {
    ((saRegion *)(void *)(saRegions.Sym))[saRegion2ValidIndex(Region)] = value;}
utInlineC void saRegionFree(saRegion Region) {
    saRegionSetNextFree(Region, saRootData.firstFreeRegion);
    saSetFirstFreeRegion(Region);}
void saRegionDestroy(saRegion Region);
utInlineC saRegion saRegionAllocRaw(void) {
    saRegion Region;
    if(saRootData.firstFreeRegion != saRegionNull) {
        Region = saRootData.firstFreeRegion;
        saSetFirstFreeRegion(saRegionNextFree(Region));
    } else {
        if(saRootData.usedRegion == saRootData.allocatedRegion) {
            saRegionAllocMore();
        }
        Region = saIndex2Region(saRootData.usedRegion);
        saSetUsedRegion(saUsedRegion() + 1);
    }
    return Region;}
utInlineC saRegion saRegionAlloc(void) {
    saRegion Region = saRegionAllocRaw();
    saRegionSetSym(Region, utSymNull);
    saRegionSetNumListings(Region, 0);
    saRegionSetRoot(Region, saRootNull);
    saRegionSetNextRootRegion(Region, saRegionNull);
    saRegionSetPrevRootRegion(Region, saRegionNull);
    saRegionSetNextTableRootRegion(Region, saRegionNull);
    saRegionSetFirstUser(Region, saUserNull);
    saRegionSetLastUser(Region, saUserNull);
    saRegionSetSession(Region, saSessionNull);
    if(saRegionConstructorCallback != NULL) {
        saRegionConstructorCallback(Region);
    }
    return Region;}

/*----------------------------------------------------------------------------------------
  Fields for class Session.
----------------------------------------------------------------------------------------*/
struct saSessionFields {
    utSym *Sym;
    saRoot *Root;
    saSession *NextRootSession;
    saSession *PrevRootSession;
    saSession *NextTableRootSession;
    saUser *User;
    saRegion *Region;
};
extern struct saSessionFields saSessions;

void saSessionAllocMore(void);
void saSessionCopyProps(saSession saOldSession, saSession saNewSession);
utInlineC utSym saSessionGetSym(saSession Session) {return saSessions.Sym[saSession2ValidIndex(Session)];}
utInlineC void saSessionSetSym(saSession Session, utSym value) {saSessions.Sym[saSession2ValidIndex(Session)] = value;}
utInlineC saRoot saSessionGetRoot(saSession Session) {return saSessions.Root[saSession2ValidIndex(Session)];}
utInlineC void saSessionSetRoot(saSession Session, saRoot value) {saSessions.Root[saSession2ValidIndex(Session)] = value;}
utInlineC saSession saSessionGetNextRootSession(saSession Session) {return saSessions.NextRootSession[saSession2ValidIndex(Session)];}
utInlineC void saSessionSetNextRootSession(saSession Session, saSession value) {saSessions.NextRootSession[saSession2ValidIndex(Session)] = value;}
utInlineC saSession saSessionGetPrevRootSession(saSession Session) {return saSessions.PrevRootSession[saSession2ValidIndex(Session)];}
utInlineC void saSessionSetPrevRootSession(saSession Session, saSession value) {saSessions.PrevRootSession[saSession2ValidIndex(Session)] = value;}
utInlineC saSession saSessionGetNextTableRootSession(saSession Session) {return saSessions.NextTableRootSession[saSession2ValidIndex(Session)];}
utInlineC void saSessionSetNextTableRootSession(saSession Session, saSession value) {saSessions.NextTableRootSession[saSession2ValidIndex(Session)] = value;}
utInlineC saUser saSessionGetUser(saSession Session) {return saSessions.User[saSession2ValidIndex(Session)];}
utInlineC void saSessionSetUser(saSession Session, saUser value) {saSessions.User[saSession2ValidIndex(Session)] = value;}
utInlineC saRegion saSessionGetRegion(saSession Session) {return saSessions.Region[saSession2ValidIndex(Session)];}
utInlineC void saSessionSetRegion(saSession Session, saRegion value) {saSessions.Region[saSession2ValidIndex(Session)] = value;}
utInlineC void saSessionSetConstructorCallback(void(*func)(saSession)) {saSessionConstructorCallback = func;}
utInlineC saSessionCallbackType saSessionGetConstructorCallback(void) {return saSessionConstructorCallback;}
utInlineC void saSessionSetDestructorCallback(void(*func)(saSession)) {saSessionDestructorCallback = func;}
utInlineC saSessionCallbackType saSessionGetDestructorCallback(void) {return saSessionDestructorCallback;}
utInlineC saSession saSessionNextFree(saSession Session) {return ((saSession *)(void *)(saSessions.Sym))[saSession2ValidIndex(Session)];}
utInlineC void saSessionSetNextFree(saSession Session, saSession value) {
    ((saSession *)(void *)(saSessions.Sym))[saSession2ValidIndex(Session)] = value;}
utInlineC void saSessionFree(saSession Session) {
    saSessionSetNextFree(Session, saRootData.firstFreeSession);
    saSetFirstFreeSession(Session);}
void saSessionDestroy(saSession Session);
utInlineC saSession saSessionAllocRaw(void) {
    saSession Session;
    if(saRootData.firstFreeSession != saSessionNull) {
        Session = saRootData.firstFreeSession;
        saSetFirstFreeSession(saSessionNextFree(Session));
    } else {
        if(saRootData.usedSession == saRootData.allocatedSession) {
            saSessionAllocMore();
        }
        Session = saIndex2Session(saRootData.usedSession);
        saSetUsedSession(saUsedSession() + 1);
    }
    return Session;}
utInlineC saSession saSessionAlloc(void) {
    saSession Session = saSessionAllocRaw();
    saSessionSetSym(Session, utSymNull);
    saSessionSetRoot(Session, saRootNull);
    saSessionSetNextRootSession(Session, saSessionNull);
    saSessionSetPrevRootSession(Session, saSessionNull);
    saSessionSetNextTableRootSession(Session, saSessionNull);
    saSessionSetUser(Session, saUserNull);
    saSessionSetRegion(Session, saRegionNull);
    if(saSessionConstructorCallback != NULL) {
        saSessionConstructorCallback(Session);
    }
    return Session;}

/*----------------------------------------------------------------------------------------
  Fields for class User.
----------------------------------------------------------------------------------------*/
struct saUserFields {
    utSym *Sym;
    uint32 *ShownNameIndex_;
    uint32 *NumShownName;
    char *ShownName;
    utSym *Email;
    uint64 *JoinDate;
    uint8 *Active;
    uint32 *HashedPasswordIndex_;
    uint32 *NumHashedPassword;
    uint8 *HashedPassword;
    uint32 *NounceIndex_;
    uint32 *NumNounce;
    uint8 *Nounce;
    uint8 *LoggedIn;
    uint64 *LastActivity;
    uint32 *Balance;
    uint8 *Validated;
    uint32 *ValidationKeyIndex_;
    uint32 *NumValidationKey;
    uint8 *ValidationKey;
    uint8 *SupremeLeader;
    uint8 *RegionManager;
    saRoot *Root;
    saUser *NextRootUser;
    saUser *PrevRootUser;
    saUser *NextTableRootUser;
    saRoot *ByEmailRoot;
    saUser *NextRootByEmailUser;
    saUser *PrevRootByEmailUser;
    saUser *NextTableRootByEmailUser;
    saRegion *Region;
    saUser *NextRegionUser;
    saUser *PrevRegionUser;
    saSession *Session;
    saCharity *Charity;
    saMembership *FirstMembership;
    saMembership *LastMembership;
    uint32 *MembershipTableIndex_;
    uint32 *NumMembershipTable;
    saMembership *MembershipTable;
    uint32 *NumMembership;
    saTransaction *FirstOutTransaction;
    saTransaction *LastOutTransaction;
    saTransaction *FirstInTransaction;
    saTransaction *LastInTransaction;
    saListing *FirstListing;
    saListing *LastListing;
    saPost *FirstPost;
    saPost *LastPost;
    uint32 *PostTableIndex_;
    uint32 *NumPostTable;
    saPost *PostTable;
    uint32 *NumPost;
    saSubscription *FirstSubscription;
    saSubscription *LastSubscription;
    saAnnouncement *FirstAnnouncement;
    saAnnouncement *LastAnnouncement;
};
extern struct saUserFields saUsers;

void saUserAllocMore(void);
void saUserCopyProps(saUser saOldUser, saUser saNewUser);
void saUserAllocShownNames(saUser User, uint32 numShownNames);
void saUserResizeShownNames(saUser User, uint32 numShownNames);
void saUserFreeShownNames(saUser User);
void saCompactUserShownNames(void);
void saUserAllocHashedPasswords(saUser User, uint32 numHashedPasswords);
void saUserResizeHashedPasswords(saUser User, uint32 numHashedPasswords);
void saUserFreeHashedPasswords(saUser User);
void saCompactUserHashedPasswords(void);
void saUserAllocNounces(saUser User, uint32 numNounces);
void saUserResizeNounces(saUser User, uint32 numNounces);
void saUserFreeNounces(saUser User);
void saCompactUserNounces(void);
void saUserAllocValidationKeys(saUser User, uint32 numValidationKeys);
void saUserResizeValidationKeys(saUser User, uint32 numValidationKeys);
void saUserFreeValidationKeys(saUser User);
void saCompactUserValidationKeys(void);
void saUserAllocMembershipTables(saUser User, uint32 numMembershipTables);
void saUserResizeMembershipTables(saUser User, uint32 numMembershipTables);
void saUserFreeMembershipTables(saUser User);
void saCompactUserMembershipTables(void);
void saUserAllocPostTables(saUser User, uint32 numPostTables);
void saUserResizePostTables(saUser User, uint32 numPostTables);
void saUserFreePostTables(saUser User);
void saCompactUserPostTables(void);
utInlineC utSym saUserGetSym(saUser User) {return saUsers.Sym[saUser2ValidIndex(User)];}
utInlineC void saUserSetSym(saUser User, utSym value) {saUsers.Sym[saUser2ValidIndex(User)] = value;}
utInlineC uint32 saUserGetShownNameIndex_(saUser User) {return saUsers.ShownNameIndex_[saUser2ValidIndex(User)];}
utInlineC void saUserSetShownNameIndex_(saUser User, uint32 value) {saUsers.ShownNameIndex_[saUser2ValidIndex(User)] = value;}
utInlineC uint32 saUserGetNumShownName(saUser User) {return saUsers.NumShownName[saUser2ValidIndex(User)];}
utInlineC void saUserSetNumShownName(saUser User, uint32 value) {saUsers.NumShownName[saUser2ValidIndex(User)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saUserCheckShownNameIndex(saUser User, uint32 x) {utAssert(x < saUserGetNumShownName(User)); return x;}
#else
utInlineC uint32 saUserCheckShownNameIndex(saUser User, uint32 x) {return x;}
#endif
utInlineC char saUserGetiShownName(saUser User, uint32 x) {return saUsers.ShownName[
    saUserGetShownNameIndex_(User) + saUserCheckShownNameIndex(User, x)];}
utInlineC char *saUserGetShownName(saUser User) {return saUsers.ShownName + saUserGetShownNameIndex_(User);}
#define saUserGetShownNames saUserGetShownName
utInlineC void saUserSetShownName(saUser User, char *valuePtr, uint32 numShownName) {
    saUserResizeShownNames(User, numShownName);
    memcpy(saUserGetShownNames(User), valuePtr, numShownName*sizeof(char));}
utInlineC void saUserSetiShownName(saUser User, uint32 x, char value) {
    saUsers.ShownName[saUserGetShownNameIndex_(User) + saUserCheckShownNameIndex(User, (x))] = value;}
utInlineC void saUserMoveShownNames(saUser User, uint32 from, uint32 to, uint32 count) {
    utAssert((to+count) <= saUserGetNumShownName(User));
    utAssert((from+count) <= saUserGetNumShownName(User));
    memmove(saUserGetShownNames(User)+to,saUserGetShownNames(User)+from,((int32)count)*sizeof(char));
}
utInlineC void saUserCopyShownNames(saUser User, uint32 x, char * values, uint32 count) {
    utAssert((x+count) <= saUserGetNumShownName(User));
    memcpy(saUserGetShownNames(User)+x, values, count*sizeof(char));
}
utInlineC void saUserAppendShownNames(saUser User, char * values, uint32 count) {
    uint32 num = saUserGetNumShownName(User);
    saUserResizeShownNames(User, num+count);
    saUserCopyShownNames(User, num, values, count);
}
utInlineC void saUserAppendShownName(saUser User, char ShownName) {
    saUserResizeShownNames(User, saUserGetNumShownName(User)+1);
    saUserSetiShownName(User, saUserGetNumShownName(User)-1, ShownName);
}
utInlineC void saUserInsertShownNames(saUser User, uint32 x, char *ShownName, uint32 count) {
    utAssert(x <= saUserGetNumShownName(User));
    if(x < saUserGetNumShownName(User)) {
        saUserResizeShownNames(User, saUserGetNumShownName(User)+count);
        saUserMoveShownNames(User, x, x+count, saUserGetNumShownName(User)-x-count);
        saUserCopyShownNames(User, x, ShownName, count);
    }
    else {
        saUserAppendShownNames(User, ShownName, count);
    }
}
utInlineC void saUserInsertShownName(saUser User, uint32 x, char ShownName) {
    saUserInsertShownNames(User, x, &ShownName, 1);
}
utInlineC void saUserRemoveShownNames(saUser User, uint32 x, uint32 count) {
    utAssert((x+count) <= saUserGetNumShownName(User));
    if((x+count) < saUserGetNumShownName(User)) {
        saUserMoveShownNames(User, x+count,x,saUserGetNumShownName(User)-x-count);
    }
    saUserResizeShownNames(User, saUserGetNumShownName(User)-(int32)count);
}
utInlineC void saUserRemoveShownName(saUser User, uint32 x) {
    saUserRemoveShownNames(User, x, 1);
}
utInlineC void saUserSwapShownName(saUser User, uint32 from, uint32 to) {
    utAssert(from <= saUserGetNumShownName(User));
    utAssert(to <= saUserGetNumShownName(User));
    char tmp = saUserGetiShownName(User, from);
    saUserSetiShownName(User, from, saUserGetiShownName(User, to));
    saUserSetiShownName(User, to, tmp);
}
utInlineC void saUserSwapShownNames(saUser User, uint32 from, uint32 to, uint32 count) {
    utAssert((from+count) < saUserGetNumShownName(User));
    utAssert((to+count) < saUserGetNumShownName(User));
    char tmp[count];
    memcpy(tmp, saUserGetShownNames(User)+from, count*sizeof(char));
    memcpy(saUserGetShownNames(User)+from, saUserGetShownNames(User)+to, count*sizeof(char));
    memcpy(saUserGetShownNames(User)+to, tmp, count*sizeof(char));
}
#define saForeachUserShownName(pVar, cVar) { \
    uint32 _xShownName; \
    for(_xShownName = 0; _xShownName < saUserGetNumShownName(pVar); _xShownName++) { \
        cVar = saUserGetiShownName(pVar, _xShownName);
#define saEndUserShownName }}
utInlineC utSym saUserGetEmail(saUser User) {return saUsers.Email[saUser2ValidIndex(User)];}
utInlineC void saUserSetEmail(saUser User, utSym value) {saUsers.Email[saUser2ValidIndex(User)] = value;}
utInlineC uint64 saUserGetJoinDate(saUser User) {return saUsers.JoinDate[saUser2ValidIndex(User)];}
utInlineC void saUserSetJoinDate(saUser User, uint64 value) {saUsers.JoinDate[saUser2ValidIndex(User)] = value;}
utInlineC uint8 saUserActive(saUser User) {return saUsers.Active[saUser2ValidIndex(User)];}
utInlineC void saUserSetActive(saUser User, uint8 value) {saUsers.Active[saUser2ValidIndex(User)] = value;}
utInlineC uint32 saUserGetHashedPasswordIndex_(saUser User) {return saUsers.HashedPasswordIndex_[saUser2ValidIndex(User)];}
utInlineC void saUserSetHashedPasswordIndex_(saUser User, uint32 value) {saUsers.HashedPasswordIndex_[saUser2ValidIndex(User)] = value;}
utInlineC uint32 saUserGetNumHashedPassword(saUser User) {return saUsers.NumHashedPassword[saUser2ValidIndex(User)];}
utInlineC void saUserSetNumHashedPassword(saUser User, uint32 value) {saUsers.NumHashedPassword[saUser2ValidIndex(User)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saUserCheckHashedPasswordIndex(saUser User, uint32 x) {utAssert(x < saUserGetNumHashedPassword(User)); return x;}
#else
utInlineC uint32 saUserCheckHashedPasswordIndex(saUser User, uint32 x) {return x;}
#endif
utInlineC uint8 saUserGetiHashedPassword(saUser User, uint32 x) {return saUsers.HashedPassword[
    saUserGetHashedPasswordIndex_(User) + saUserCheckHashedPasswordIndex(User, x)];}
utInlineC uint8 *saUserGetHashedPassword(saUser User) {return saUsers.HashedPassword + saUserGetHashedPasswordIndex_(User);}
#define saUserGetHashedPasswords saUserGetHashedPassword
utInlineC void saUserSetHashedPassword(saUser User, uint8 *valuePtr, uint32 numHashedPassword) {
    saUserResizeHashedPasswords(User, numHashedPassword);
    memcpy(saUserGetHashedPasswords(User), valuePtr, numHashedPassword*sizeof(uint8));}
utInlineC void saUserSetiHashedPassword(saUser User, uint32 x, uint8 value) {
    saUsers.HashedPassword[saUserGetHashedPasswordIndex_(User) + saUserCheckHashedPasswordIndex(User, (x))] = value;}
utInlineC void saUserMoveHashedPasswords(saUser User, uint32 from, uint32 to, uint32 count) {
    utAssert((to+count) <= saUserGetNumHashedPassword(User));
    utAssert((from+count) <= saUserGetNumHashedPassword(User));
    memmove(saUserGetHashedPasswords(User)+to,saUserGetHashedPasswords(User)+from,((int32)count)*sizeof(uint8));
}
utInlineC void saUserCopyHashedPasswords(saUser User, uint32 x, uint8 * values, uint32 count) {
    utAssert((x+count) <= saUserGetNumHashedPassword(User));
    memcpy(saUserGetHashedPasswords(User)+x, values, count*sizeof(uint8));
}
utInlineC void saUserAppendHashedPasswords(saUser User, uint8 * values, uint32 count) {
    uint32 num = saUserGetNumHashedPassword(User);
    saUserResizeHashedPasswords(User, num+count);
    saUserCopyHashedPasswords(User, num, values, count);
}
utInlineC void saUserAppendHashedPassword(saUser User, uint8 HashedPassword) {
    saUserResizeHashedPasswords(User, saUserGetNumHashedPassword(User)+1);
    saUserSetiHashedPassword(User, saUserGetNumHashedPassword(User)-1, HashedPassword);
}
utInlineC void saUserInsertHashedPasswords(saUser User, uint32 x, uint8 *HashedPassword, uint32 count) {
    utAssert(x <= saUserGetNumHashedPassword(User));
    if(x < saUserGetNumHashedPassword(User)) {
        saUserResizeHashedPasswords(User, saUserGetNumHashedPassword(User)+count);
        saUserMoveHashedPasswords(User, x, x+count, saUserGetNumHashedPassword(User)-x-count);
        saUserCopyHashedPasswords(User, x, HashedPassword, count);
    }
    else {
        saUserAppendHashedPasswords(User, HashedPassword, count);
    }
}
utInlineC void saUserInsertHashedPassword(saUser User, uint32 x, uint8 HashedPassword) {
    saUserInsertHashedPasswords(User, x, &HashedPassword, 1);
}
utInlineC void saUserRemoveHashedPasswords(saUser User, uint32 x, uint32 count) {
    utAssert((x+count) <= saUserGetNumHashedPassword(User));
    if((x+count) < saUserGetNumHashedPassword(User)) {
        saUserMoveHashedPasswords(User, x+count,x,saUserGetNumHashedPassword(User)-x-count);
    }
    saUserResizeHashedPasswords(User, saUserGetNumHashedPassword(User)-(int32)count);
}
utInlineC void saUserRemoveHashedPassword(saUser User, uint32 x) {
    saUserRemoveHashedPasswords(User, x, 1);
}
utInlineC void saUserSwapHashedPassword(saUser User, uint32 from, uint32 to) {
    utAssert(from <= saUserGetNumHashedPassword(User));
    utAssert(to <= saUserGetNumHashedPassword(User));
    uint8 tmp = saUserGetiHashedPassword(User, from);
    saUserSetiHashedPassword(User, from, saUserGetiHashedPassword(User, to));
    saUserSetiHashedPassword(User, to, tmp);
}
utInlineC void saUserSwapHashedPasswords(saUser User, uint32 from, uint32 to, uint32 count) {
    utAssert((from+count) < saUserGetNumHashedPassword(User));
    utAssert((to+count) < saUserGetNumHashedPassword(User));
    uint8 tmp[count];
    memcpy(tmp, saUserGetHashedPasswords(User)+from, count*sizeof(uint8));
    memcpy(saUserGetHashedPasswords(User)+from, saUserGetHashedPasswords(User)+to, count*sizeof(uint8));
    memcpy(saUserGetHashedPasswords(User)+to, tmp, count*sizeof(uint8));
}
#define saForeachUserHashedPassword(pVar, cVar) { \
    uint32 _xHashedPassword; \
    for(_xHashedPassword = 0; _xHashedPassword < saUserGetNumHashedPassword(pVar); _xHashedPassword++) { \
        cVar = saUserGetiHashedPassword(pVar, _xHashedPassword);
#define saEndUserHashedPassword }}
utInlineC uint32 saUserGetNounceIndex_(saUser User) {return saUsers.NounceIndex_[saUser2ValidIndex(User)];}
utInlineC void saUserSetNounceIndex_(saUser User, uint32 value) {saUsers.NounceIndex_[saUser2ValidIndex(User)] = value;}
utInlineC uint32 saUserGetNumNounce(saUser User) {return saUsers.NumNounce[saUser2ValidIndex(User)];}
utInlineC void saUserSetNumNounce(saUser User, uint32 value) {saUsers.NumNounce[saUser2ValidIndex(User)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saUserCheckNounceIndex(saUser User, uint32 x) {utAssert(x < saUserGetNumNounce(User)); return x;}
#else
utInlineC uint32 saUserCheckNounceIndex(saUser User, uint32 x) {return x;}
#endif
utInlineC uint8 saUserGetiNounce(saUser User, uint32 x) {return saUsers.Nounce[
    saUserGetNounceIndex_(User) + saUserCheckNounceIndex(User, x)];}
utInlineC uint8 *saUserGetNounce(saUser User) {return saUsers.Nounce + saUserGetNounceIndex_(User);}
#define saUserGetNounces saUserGetNounce
utInlineC void saUserSetNounce(saUser User, uint8 *valuePtr, uint32 numNounce) {
    saUserResizeNounces(User, numNounce);
    memcpy(saUserGetNounces(User), valuePtr, numNounce*sizeof(uint8));}
utInlineC void saUserSetiNounce(saUser User, uint32 x, uint8 value) {
    saUsers.Nounce[saUserGetNounceIndex_(User) + saUserCheckNounceIndex(User, (x))] = value;}
utInlineC void saUserMoveNounces(saUser User, uint32 from, uint32 to, uint32 count) {
    utAssert((to+count) <= saUserGetNumNounce(User));
    utAssert((from+count) <= saUserGetNumNounce(User));
    memmove(saUserGetNounces(User)+to,saUserGetNounces(User)+from,((int32)count)*sizeof(uint8));
}
utInlineC void saUserCopyNounces(saUser User, uint32 x, uint8 * values, uint32 count) {
    utAssert((x+count) <= saUserGetNumNounce(User));
    memcpy(saUserGetNounces(User)+x, values, count*sizeof(uint8));
}
utInlineC void saUserAppendNounces(saUser User, uint8 * values, uint32 count) {
    uint32 num = saUserGetNumNounce(User);
    saUserResizeNounces(User, num+count);
    saUserCopyNounces(User, num, values, count);
}
utInlineC void saUserAppendNounce(saUser User, uint8 Nounce) {
    saUserResizeNounces(User, saUserGetNumNounce(User)+1);
    saUserSetiNounce(User, saUserGetNumNounce(User)-1, Nounce);
}
utInlineC void saUserInsertNounces(saUser User, uint32 x, uint8 *Nounce, uint32 count) {
    utAssert(x <= saUserGetNumNounce(User));
    if(x < saUserGetNumNounce(User)) {
        saUserResizeNounces(User, saUserGetNumNounce(User)+count);
        saUserMoveNounces(User, x, x+count, saUserGetNumNounce(User)-x-count);
        saUserCopyNounces(User, x, Nounce, count);
    }
    else {
        saUserAppendNounces(User, Nounce, count);
    }
}
utInlineC void saUserInsertNounce(saUser User, uint32 x, uint8 Nounce) {
    saUserInsertNounces(User, x, &Nounce, 1);
}
utInlineC void saUserRemoveNounces(saUser User, uint32 x, uint32 count) {
    utAssert((x+count) <= saUserGetNumNounce(User));
    if((x+count) < saUserGetNumNounce(User)) {
        saUserMoveNounces(User, x+count,x,saUserGetNumNounce(User)-x-count);
    }
    saUserResizeNounces(User, saUserGetNumNounce(User)-(int32)count);
}
utInlineC void saUserRemoveNounce(saUser User, uint32 x) {
    saUserRemoveNounces(User, x, 1);
}
utInlineC void saUserSwapNounce(saUser User, uint32 from, uint32 to) {
    utAssert(from <= saUserGetNumNounce(User));
    utAssert(to <= saUserGetNumNounce(User));
    uint8 tmp = saUserGetiNounce(User, from);
    saUserSetiNounce(User, from, saUserGetiNounce(User, to));
    saUserSetiNounce(User, to, tmp);
}
utInlineC void saUserSwapNounces(saUser User, uint32 from, uint32 to, uint32 count) {
    utAssert((from+count) < saUserGetNumNounce(User));
    utAssert((to+count) < saUserGetNumNounce(User));
    uint8 tmp[count];
    memcpy(tmp, saUserGetNounces(User)+from, count*sizeof(uint8));
    memcpy(saUserGetNounces(User)+from, saUserGetNounces(User)+to, count*sizeof(uint8));
    memcpy(saUserGetNounces(User)+to, tmp, count*sizeof(uint8));
}
#define saForeachUserNounce(pVar, cVar) { \
    uint32 _xNounce; \
    for(_xNounce = 0; _xNounce < saUserGetNumNounce(pVar); _xNounce++) { \
        cVar = saUserGetiNounce(pVar, _xNounce);
#define saEndUserNounce }}
utInlineC uint8 saUserLoggedIn(saUser User) {return saUsers.LoggedIn[saUser2ValidIndex(User)];}
utInlineC void saUserSetLoggedIn(saUser User, uint8 value) {saUsers.LoggedIn[saUser2ValidIndex(User)] = value;}
utInlineC uint64 saUserGetLastActivity(saUser User) {return saUsers.LastActivity[saUser2ValidIndex(User)];}
utInlineC void saUserSetLastActivity(saUser User, uint64 value) {saUsers.LastActivity[saUser2ValidIndex(User)] = value;}
utInlineC uint32 saUserGetBalance(saUser User) {return saUsers.Balance[saUser2ValidIndex(User)];}
utInlineC void saUserSetBalance(saUser User, uint32 value) {saUsers.Balance[saUser2ValidIndex(User)] = value;}
utInlineC uint8 saUserValidated(saUser User) {return saUsers.Validated[saUser2ValidIndex(User)];}
utInlineC void saUserSetValidated(saUser User, uint8 value) {saUsers.Validated[saUser2ValidIndex(User)] = value;}
utInlineC uint32 saUserGetValidationKeyIndex_(saUser User) {return saUsers.ValidationKeyIndex_[saUser2ValidIndex(User)];}
utInlineC void saUserSetValidationKeyIndex_(saUser User, uint32 value) {saUsers.ValidationKeyIndex_[saUser2ValidIndex(User)] = value;}
utInlineC uint32 saUserGetNumValidationKey(saUser User) {return saUsers.NumValidationKey[saUser2ValidIndex(User)];}
utInlineC void saUserSetNumValidationKey(saUser User, uint32 value) {saUsers.NumValidationKey[saUser2ValidIndex(User)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saUserCheckValidationKeyIndex(saUser User, uint32 x) {utAssert(x < saUserGetNumValidationKey(User)); return x;}
#else
utInlineC uint32 saUserCheckValidationKeyIndex(saUser User, uint32 x) {return x;}
#endif
utInlineC uint8 saUserGetiValidationKey(saUser User, uint32 x) {return saUsers.ValidationKey[
    saUserGetValidationKeyIndex_(User) + saUserCheckValidationKeyIndex(User, x)];}
utInlineC uint8 *saUserGetValidationKey(saUser User) {return saUsers.ValidationKey + saUserGetValidationKeyIndex_(User);}
#define saUserGetValidationKeys saUserGetValidationKey
utInlineC void saUserSetValidationKey(saUser User, uint8 *valuePtr, uint32 numValidationKey) {
    saUserResizeValidationKeys(User, numValidationKey);
    memcpy(saUserGetValidationKeys(User), valuePtr, numValidationKey*sizeof(uint8));}
utInlineC void saUserSetiValidationKey(saUser User, uint32 x, uint8 value) {
    saUsers.ValidationKey[saUserGetValidationKeyIndex_(User) + saUserCheckValidationKeyIndex(User, (x))] = value;}
utInlineC void saUserMoveValidationKeys(saUser User, uint32 from, uint32 to, uint32 count) {
    utAssert((to+count) <= saUserGetNumValidationKey(User));
    utAssert((from+count) <= saUserGetNumValidationKey(User));
    memmove(saUserGetValidationKeys(User)+to,saUserGetValidationKeys(User)+from,((int32)count)*sizeof(uint8));
}
utInlineC void saUserCopyValidationKeys(saUser User, uint32 x, uint8 * values, uint32 count) {
    utAssert((x+count) <= saUserGetNumValidationKey(User));
    memcpy(saUserGetValidationKeys(User)+x, values, count*sizeof(uint8));
}
utInlineC void saUserAppendValidationKeys(saUser User, uint8 * values, uint32 count) {
    uint32 num = saUserGetNumValidationKey(User);
    saUserResizeValidationKeys(User, num+count);
    saUserCopyValidationKeys(User, num, values, count);
}
utInlineC void saUserAppendValidationKey(saUser User, uint8 ValidationKey) {
    saUserResizeValidationKeys(User, saUserGetNumValidationKey(User)+1);
    saUserSetiValidationKey(User, saUserGetNumValidationKey(User)-1, ValidationKey);
}
utInlineC void saUserInsertValidationKeys(saUser User, uint32 x, uint8 *ValidationKey, uint32 count) {
    utAssert(x <= saUserGetNumValidationKey(User));
    if(x < saUserGetNumValidationKey(User)) {
        saUserResizeValidationKeys(User, saUserGetNumValidationKey(User)+count);
        saUserMoveValidationKeys(User, x, x+count, saUserGetNumValidationKey(User)-x-count);
        saUserCopyValidationKeys(User, x, ValidationKey, count);
    }
    else {
        saUserAppendValidationKeys(User, ValidationKey, count);
    }
}
utInlineC void saUserInsertValidationKey(saUser User, uint32 x, uint8 ValidationKey) {
    saUserInsertValidationKeys(User, x, &ValidationKey, 1);
}
utInlineC void saUserRemoveValidationKeys(saUser User, uint32 x, uint32 count) {
    utAssert((x+count) <= saUserGetNumValidationKey(User));
    if((x+count) < saUserGetNumValidationKey(User)) {
        saUserMoveValidationKeys(User, x+count,x,saUserGetNumValidationKey(User)-x-count);
    }
    saUserResizeValidationKeys(User, saUserGetNumValidationKey(User)-(int32)count);
}
utInlineC void saUserRemoveValidationKey(saUser User, uint32 x) {
    saUserRemoveValidationKeys(User, x, 1);
}
utInlineC void saUserSwapValidationKey(saUser User, uint32 from, uint32 to) {
    utAssert(from <= saUserGetNumValidationKey(User));
    utAssert(to <= saUserGetNumValidationKey(User));
    uint8 tmp = saUserGetiValidationKey(User, from);
    saUserSetiValidationKey(User, from, saUserGetiValidationKey(User, to));
    saUserSetiValidationKey(User, to, tmp);
}
utInlineC void saUserSwapValidationKeys(saUser User, uint32 from, uint32 to, uint32 count) {
    utAssert((from+count) < saUserGetNumValidationKey(User));
    utAssert((to+count) < saUserGetNumValidationKey(User));
    uint8 tmp[count];
    memcpy(tmp, saUserGetValidationKeys(User)+from, count*sizeof(uint8));
    memcpy(saUserGetValidationKeys(User)+from, saUserGetValidationKeys(User)+to, count*sizeof(uint8));
    memcpy(saUserGetValidationKeys(User)+to, tmp, count*sizeof(uint8));
}
#define saForeachUserValidationKey(pVar, cVar) { \
    uint32 _xValidationKey; \
    for(_xValidationKey = 0; _xValidationKey < saUserGetNumValidationKey(pVar); _xValidationKey++) { \
        cVar = saUserGetiValidationKey(pVar, _xValidationKey);
#define saEndUserValidationKey }}
utInlineC uint8 saUserSupremeLeader(saUser User) {return saUsers.SupremeLeader[saUser2ValidIndex(User)];}
utInlineC void saUserSetSupremeLeader(saUser User, uint8 value) {saUsers.SupremeLeader[saUser2ValidIndex(User)] = value;}
utInlineC uint8 saUserRegionManager(saUser User) {return saUsers.RegionManager[saUser2ValidIndex(User)];}
utInlineC void saUserSetRegionManager(saUser User, uint8 value) {saUsers.RegionManager[saUser2ValidIndex(User)] = value;}
utInlineC saRoot saUserGetRoot(saUser User) {return saUsers.Root[saUser2ValidIndex(User)];}
utInlineC void saUserSetRoot(saUser User, saRoot value) {saUsers.Root[saUser2ValidIndex(User)] = value;}
utInlineC saUser saUserGetNextRootUser(saUser User) {return saUsers.NextRootUser[saUser2ValidIndex(User)];}
utInlineC void saUserSetNextRootUser(saUser User, saUser value) {saUsers.NextRootUser[saUser2ValidIndex(User)] = value;}
utInlineC saUser saUserGetPrevRootUser(saUser User) {return saUsers.PrevRootUser[saUser2ValidIndex(User)];}
utInlineC void saUserSetPrevRootUser(saUser User, saUser value) {saUsers.PrevRootUser[saUser2ValidIndex(User)] = value;}
utInlineC saUser saUserGetNextTableRootUser(saUser User) {return saUsers.NextTableRootUser[saUser2ValidIndex(User)];}
utInlineC void saUserSetNextTableRootUser(saUser User, saUser value) {saUsers.NextTableRootUser[saUser2ValidIndex(User)] = value;}
utInlineC saRoot saUserGetByEmailRoot(saUser User) {return saUsers.ByEmailRoot[saUser2ValidIndex(User)];}
utInlineC void saUserSetByEmailRoot(saUser User, saRoot value) {saUsers.ByEmailRoot[saUser2ValidIndex(User)] = value;}
utInlineC saUser saUserGetNextRootByEmailUser(saUser User) {return saUsers.NextRootByEmailUser[saUser2ValidIndex(User)];}
utInlineC void saUserSetNextRootByEmailUser(saUser User, saUser value) {saUsers.NextRootByEmailUser[saUser2ValidIndex(User)] = value;}
utInlineC saUser saUserGetPrevRootByEmailUser(saUser User) {return saUsers.PrevRootByEmailUser[saUser2ValidIndex(User)];}
utInlineC void saUserSetPrevRootByEmailUser(saUser User, saUser value) {saUsers.PrevRootByEmailUser[saUser2ValidIndex(User)] = value;}
utInlineC saUser saUserGetNextTableRootByEmailUser(saUser User) {return saUsers.NextTableRootByEmailUser[saUser2ValidIndex(User)];}
utInlineC void saUserSetNextTableRootByEmailUser(saUser User, saUser value) {saUsers.NextTableRootByEmailUser[saUser2ValidIndex(User)] = value;}
utInlineC saRegion saUserGetRegion(saUser User) {return saUsers.Region[saUser2ValidIndex(User)];}
utInlineC void saUserSetRegion(saUser User, saRegion value) {saUsers.Region[saUser2ValidIndex(User)] = value;}
utInlineC saUser saUserGetNextRegionUser(saUser User) {return saUsers.NextRegionUser[saUser2ValidIndex(User)];}
utInlineC void saUserSetNextRegionUser(saUser User, saUser value) {saUsers.NextRegionUser[saUser2ValidIndex(User)] = value;}
utInlineC saUser saUserGetPrevRegionUser(saUser User) {return saUsers.PrevRegionUser[saUser2ValidIndex(User)];}
utInlineC void saUserSetPrevRegionUser(saUser User, saUser value) {saUsers.PrevRegionUser[saUser2ValidIndex(User)] = value;}
utInlineC saSession saUserGetSession(saUser User) {return saUsers.Session[saUser2ValidIndex(User)];}
utInlineC void saUserSetSession(saUser User, saSession value) {saUsers.Session[saUser2ValidIndex(User)] = value;}
utInlineC saCharity saUserGetCharity(saUser User) {return saUsers.Charity[saUser2ValidIndex(User)];}
utInlineC void saUserSetCharity(saUser User, saCharity value) {saUsers.Charity[saUser2ValidIndex(User)] = value;}
utInlineC saMembership saUserGetFirstMembership(saUser User) {return saUsers.FirstMembership[saUser2ValidIndex(User)];}
utInlineC void saUserSetFirstMembership(saUser User, saMembership value) {saUsers.FirstMembership[saUser2ValidIndex(User)] = value;}
utInlineC saMembership saUserGetLastMembership(saUser User) {return saUsers.LastMembership[saUser2ValidIndex(User)];}
utInlineC void saUserSetLastMembership(saUser User, saMembership value) {saUsers.LastMembership[saUser2ValidIndex(User)] = value;}
utInlineC uint32 saUserGetMembershipTableIndex_(saUser User) {return saUsers.MembershipTableIndex_[saUser2ValidIndex(User)];}
utInlineC void saUserSetMembershipTableIndex_(saUser User, uint32 value) {saUsers.MembershipTableIndex_[saUser2ValidIndex(User)] = value;}
utInlineC uint32 saUserGetNumMembershipTable(saUser User) {return saUsers.NumMembershipTable[saUser2ValidIndex(User)];}
utInlineC void saUserSetNumMembershipTable(saUser User, uint32 value) {saUsers.NumMembershipTable[saUser2ValidIndex(User)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saUserCheckMembershipTableIndex(saUser User, uint32 x) {utAssert(x < saUserGetNumMembershipTable(User)); return x;}
#else
utInlineC uint32 saUserCheckMembershipTableIndex(saUser User, uint32 x) {return x;}
#endif
utInlineC saMembership saUserGetiMembershipTable(saUser User, uint32 x) {return saUsers.MembershipTable[
    saUserGetMembershipTableIndex_(User) + saUserCheckMembershipTableIndex(User, x)];}
utInlineC saMembership *saUserGetMembershipTable(saUser User) {return saUsers.MembershipTable + saUserGetMembershipTableIndex_(User);}
#define saUserGetMembershipTables saUserGetMembershipTable
utInlineC void saUserSetMembershipTable(saUser User, saMembership *valuePtr, uint32 numMembershipTable) {
    saUserResizeMembershipTables(User, numMembershipTable);
    memcpy(saUserGetMembershipTables(User), valuePtr, numMembershipTable*sizeof(saMembership));}
utInlineC void saUserSetiMembershipTable(saUser User, uint32 x, saMembership value) {
    saUsers.MembershipTable[saUserGetMembershipTableIndex_(User) + saUserCheckMembershipTableIndex(User, (x))] = value;}
utInlineC uint32 saUserGetNumMembership(saUser User) {return saUsers.NumMembership[saUser2ValidIndex(User)];}
utInlineC void saUserSetNumMembership(saUser User, uint32 value) {saUsers.NumMembership[saUser2ValidIndex(User)] = value;}
utInlineC saTransaction saUserGetFirstOutTransaction(saUser User) {return saUsers.FirstOutTransaction[saUser2ValidIndex(User)];}
utInlineC void saUserSetFirstOutTransaction(saUser User, saTransaction value) {saUsers.FirstOutTransaction[saUser2ValidIndex(User)] = value;}
utInlineC saTransaction saUserGetLastOutTransaction(saUser User) {return saUsers.LastOutTransaction[saUser2ValidIndex(User)];}
utInlineC void saUserSetLastOutTransaction(saUser User, saTransaction value) {saUsers.LastOutTransaction[saUser2ValidIndex(User)] = value;}
utInlineC saTransaction saUserGetFirstInTransaction(saUser User) {return saUsers.FirstInTransaction[saUser2ValidIndex(User)];}
utInlineC void saUserSetFirstInTransaction(saUser User, saTransaction value) {saUsers.FirstInTransaction[saUser2ValidIndex(User)] = value;}
utInlineC saTransaction saUserGetLastInTransaction(saUser User) {return saUsers.LastInTransaction[saUser2ValidIndex(User)];}
utInlineC void saUserSetLastInTransaction(saUser User, saTransaction value) {saUsers.LastInTransaction[saUser2ValidIndex(User)] = value;}
utInlineC saListing saUserGetFirstListing(saUser User) {return saUsers.FirstListing[saUser2ValidIndex(User)];}
utInlineC void saUserSetFirstListing(saUser User, saListing value) {saUsers.FirstListing[saUser2ValidIndex(User)] = value;}
utInlineC saListing saUserGetLastListing(saUser User) {return saUsers.LastListing[saUser2ValidIndex(User)];}
utInlineC void saUserSetLastListing(saUser User, saListing value) {saUsers.LastListing[saUser2ValidIndex(User)] = value;}
utInlineC saPost saUserGetFirstPost(saUser User) {return saUsers.FirstPost[saUser2ValidIndex(User)];}
utInlineC void saUserSetFirstPost(saUser User, saPost value) {saUsers.FirstPost[saUser2ValidIndex(User)] = value;}
utInlineC saPost saUserGetLastPost(saUser User) {return saUsers.LastPost[saUser2ValidIndex(User)];}
utInlineC void saUserSetLastPost(saUser User, saPost value) {saUsers.LastPost[saUser2ValidIndex(User)] = value;}
utInlineC uint32 saUserGetPostTableIndex_(saUser User) {return saUsers.PostTableIndex_[saUser2ValidIndex(User)];}
utInlineC void saUserSetPostTableIndex_(saUser User, uint32 value) {saUsers.PostTableIndex_[saUser2ValidIndex(User)] = value;}
utInlineC uint32 saUserGetNumPostTable(saUser User) {return saUsers.NumPostTable[saUser2ValidIndex(User)];}
utInlineC void saUserSetNumPostTable(saUser User, uint32 value) {saUsers.NumPostTable[saUser2ValidIndex(User)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saUserCheckPostTableIndex(saUser User, uint32 x) {utAssert(x < saUserGetNumPostTable(User)); return x;}
#else
utInlineC uint32 saUserCheckPostTableIndex(saUser User, uint32 x) {return x;}
#endif
utInlineC saPost saUserGetiPostTable(saUser User, uint32 x) {return saUsers.PostTable[
    saUserGetPostTableIndex_(User) + saUserCheckPostTableIndex(User, x)];}
utInlineC saPost *saUserGetPostTable(saUser User) {return saUsers.PostTable + saUserGetPostTableIndex_(User);}
#define saUserGetPostTables saUserGetPostTable
utInlineC void saUserSetPostTable(saUser User, saPost *valuePtr, uint32 numPostTable) {
    saUserResizePostTables(User, numPostTable);
    memcpy(saUserGetPostTables(User), valuePtr, numPostTable*sizeof(saPost));}
utInlineC void saUserSetiPostTable(saUser User, uint32 x, saPost value) {
    saUsers.PostTable[saUserGetPostTableIndex_(User) + saUserCheckPostTableIndex(User, (x))] = value;}
utInlineC uint32 saUserGetNumPost(saUser User) {return saUsers.NumPost[saUser2ValidIndex(User)];}
utInlineC void saUserSetNumPost(saUser User, uint32 value) {saUsers.NumPost[saUser2ValidIndex(User)] = value;}
utInlineC saSubscription saUserGetFirstSubscription(saUser User) {return saUsers.FirstSubscription[saUser2ValidIndex(User)];}
utInlineC void saUserSetFirstSubscription(saUser User, saSubscription value) {saUsers.FirstSubscription[saUser2ValidIndex(User)] = value;}
utInlineC saSubscription saUserGetLastSubscription(saUser User) {return saUsers.LastSubscription[saUser2ValidIndex(User)];}
utInlineC void saUserSetLastSubscription(saUser User, saSubscription value) {saUsers.LastSubscription[saUser2ValidIndex(User)] = value;}
utInlineC saAnnouncement saUserGetFirstAnnouncement(saUser User) {return saUsers.FirstAnnouncement[saUser2ValidIndex(User)];}
utInlineC void saUserSetFirstAnnouncement(saUser User, saAnnouncement value) {saUsers.FirstAnnouncement[saUser2ValidIndex(User)] = value;}
utInlineC saAnnouncement saUserGetLastAnnouncement(saUser User) {return saUsers.LastAnnouncement[saUser2ValidIndex(User)];}
utInlineC void saUserSetLastAnnouncement(saUser User, saAnnouncement value) {saUsers.LastAnnouncement[saUser2ValidIndex(User)] = value;}
utInlineC void saUserSetConstructorCallback(void(*func)(saUser)) {saUserConstructorCallback = func;}
utInlineC saUserCallbackType saUserGetConstructorCallback(void) {return saUserConstructorCallback;}
utInlineC void saUserSetDestructorCallback(void(*func)(saUser)) {saUserDestructorCallback = func;}
utInlineC saUserCallbackType saUserGetDestructorCallback(void) {return saUserDestructorCallback;}
utInlineC saUser saUserNextFree(saUser User) {return ((saUser *)(void *)(saUsers.Sym))[saUser2ValidIndex(User)];}
utInlineC void saUserSetNextFree(saUser User, saUser value) {
    ((saUser *)(void *)(saUsers.Sym))[saUser2ValidIndex(User)] = value;}
utInlineC void saUserFree(saUser User) {
    saUserFreeShownNames(User);
    saUserFreeHashedPasswords(User);
    saUserFreeNounces(User);
    saUserFreeValidationKeys(User);
    saUserFreeMembershipTables(User);
    saUserFreePostTables(User);
    saUserSetNextFree(User, saRootData.firstFreeUser);
    saSetFirstFreeUser(User);}
void saUserDestroy(saUser User);
utInlineC saUser saUserAllocRaw(void) {
    saUser User;
    if(saRootData.firstFreeUser != saUserNull) {
        User = saRootData.firstFreeUser;
        saSetFirstFreeUser(saUserNextFree(User));
    } else {
        if(saRootData.usedUser == saRootData.allocatedUser) {
            saUserAllocMore();
        }
        User = saIndex2User(saRootData.usedUser);
        saSetUsedUser(saUsedUser() + 1);
    }
    return User;}
utInlineC saUser saUserAlloc(void) {
    saUser User = saUserAllocRaw();
    saUserSetSym(User, utSymNull);
    saUserSetShownNameIndex_(User, 0);
    saUserSetNumShownName(User, 0);
    saUserSetNumShownName(User, 0);
    saUserSetEmail(User, utSymNull);
    saUserSetJoinDate(User, 0);
    saUserSetActive(User, 0);
    saUserSetHashedPasswordIndex_(User, 0);
    saUserSetNumHashedPassword(User, 0);
    saUserSetNumHashedPassword(User, 0);
    saUserSetNounceIndex_(User, 0);
    saUserSetNumNounce(User, 0);
    saUserSetNumNounce(User, 0);
    saUserSetLoggedIn(User, 0);
    saUserSetLastActivity(User, 0);
    saUserSetBalance(User, 0);
    saUserSetValidated(User, 0);
    saUserSetValidationKeyIndex_(User, 0);
    saUserSetNumValidationKey(User, 0);
    saUserSetNumValidationKey(User, 0);
    saUserSetSupremeLeader(User, 0);
    saUserSetRegionManager(User, 0);
    saUserSetRoot(User, saRootNull);
    saUserSetNextRootUser(User, saUserNull);
    saUserSetPrevRootUser(User, saUserNull);
    saUserSetNextTableRootUser(User, saUserNull);
    saUserSetByEmailRoot(User, saRootNull);
    saUserSetNextRootByEmailUser(User, saUserNull);
    saUserSetPrevRootByEmailUser(User, saUserNull);
    saUserSetNextTableRootByEmailUser(User, saUserNull);
    saUserSetRegion(User, saRegionNull);
    saUserSetNextRegionUser(User, saUserNull);
    saUserSetPrevRegionUser(User, saUserNull);
    saUserSetSession(User, saSessionNull);
    saUserSetCharity(User, saCharityNull);
    saUserSetFirstMembership(User, saMembershipNull);
    saUserSetLastMembership(User, saMembershipNull);
    saUserSetMembershipTableIndex_(User, 0);
    saUserSetNumMembershipTable(User, 0);
    saUserSetNumMembershipTable(User, 0);
    saUserSetNumMembership(User, 0);
    saUserSetFirstOutTransaction(User, saTransactionNull);
    saUserSetLastOutTransaction(User, saTransactionNull);
    saUserSetFirstInTransaction(User, saTransactionNull);
    saUserSetLastInTransaction(User, saTransactionNull);
    saUserSetFirstListing(User, saListingNull);
    saUserSetLastListing(User, saListingNull);
    saUserSetFirstPost(User, saPostNull);
    saUserSetLastPost(User, saPostNull);
    saUserSetPostTableIndex_(User, 0);
    saUserSetNumPostTable(User, 0);
    saUserSetNumPostTable(User, 0);
    saUserSetNumPost(User, 0);
    saUserSetFirstSubscription(User, saSubscriptionNull);
    saUserSetLastSubscription(User, saSubscriptionNull);
    saUserSetFirstAnnouncement(User, saAnnouncementNull);
    saUserSetLastAnnouncement(User, saAnnouncementNull);
    if(saUserConstructorCallback != NULL) {
        saUserConstructorCallback(User);
    }
    return User;}

/*----------------------------------------------------------------------------------------
  Fields for class Transaction.
----------------------------------------------------------------------------------------*/
struct saTransactionFields {
    uint64 *Date;
    uint32 *Amount;
    uint32 *DescriptionIndex_;
    uint32 *NumDescription;
    char *Description;
    saUser *FromUser;
    saTransaction *NextUserOutTransaction;
    saTransaction *PrevUserOutTransaction;
    saUser *ToUser;
    saTransaction *NextUserInTransaction;
    saTransaction *PrevUserInTransaction;
};
extern struct saTransactionFields saTransactions;

void saTransactionAllocMore(void);
void saTransactionCopyProps(saTransaction saOldTransaction, saTransaction saNewTransaction);
void saTransactionAllocDescriptions(saTransaction Transaction, uint32 numDescriptions);
void saTransactionResizeDescriptions(saTransaction Transaction, uint32 numDescriptions);
void saTransactionFreeDescriptions(saTransaction Transaction);
void saCompactTransactionDescriptions(void);
utInlineC uint64 saTransactionGetDate(saTransaction Transaction) {return saTransactions.Date[saTransaction2ValidIndex(Transaction)];}
utInlineC void saTransactionSetDate(saTransaction Transaction, uint64 value) {saTransactions.Date[saTransaction2ValidIndex(Transaction)] = value;}
utInlineC uint32 saTransactionGetAmount(saTransaction Transaction) {return saTransactions.Amount[saTransaction2ValidIndex(Transaction)];}
utInlineC void saTransactionSetAmount(saTransaction Transaction, uint32 value) {saTransactions.Amount[saTransaction2ValidIndex(Transaction)] = value;}
utInlineC uint32 saTransactionGetDescriptionIndex_(saTransaction Transaction) {return saTransactions.DescriptionIndex_[saTransaction2ValidIndex(Transaction)];}
utInlineC void saTransactionSetDescriptionIndex_(saTransaction Transaction, uint32 value) {saTransactions.DescriptionIndex_[saTransaction2ValidIndex(Transaction)] = value;}
utInlineC uint32 saTransactionGetNumDescription(saTransaction Transaction) {return saTransactions.NumDescription[saTransaction2ValidIndex(Transaction)];}
utInlineC void saTransactionSetNumDescription(saTransaction Transaction, uint32 value) {saTransactions.NumDescription[saTransaction2ValidIndex(Transaction)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saTransactionCheckDescriptionIndex(saTransaction Transaction, uint32 x) {utAssert(x < saTransactionGetNumDescription(Transaction)); return x;}
#else
utInlineC uint32 saTransactionCheckDescriptionIndex(saTransaction Transaction, uint32 x) {return x;}
#endif
utInlineC char saTransactionGetiDescription(saTransaction Transaction, uint32 x) {return saTransactions.Description[
    saTransactionGetDescriptionIndex_(Transaction) + saTransactionCheckDescriptionIndex(Transaction, x)];}
utInlineC char *saTransactionGetDescription(saTransaction Transaction) {return saTransactions.Description + saTransactionGetDescriptionIndex_(Transaction);}
#define saTransactionGetDescriptions saTransactionGetDescription
utInlineC void saTransactionSetDescription(saTransaction Transaction, char *valuePtr, uint32 numDescription) {
    saTransactionResizeDescriptions(Transaction, numDescription);
    memcpy(saTransactionGetDescriptions(Transaction), valuePtr, numDescription*sizeof(char));}
utInlineC void saTransactionSetiDescription(saTransaction Transaction, uint32 x, char value) {
    saTransactions.Description[saTransactionGetDescriptionIndex_(Transaction) + saTransactionCheckDescriptionIndex(Transaction, (x))] = value;}
utInlineC void saTransactionMoveDescriptions(saTransaction Transaction, uint32 from, uint32 to, uint32 count) {
    utAssert((to+count) <= saTransactionGetNumDescription(Transaction));
    utAssert((from+count) <= saTransactionGetNumDescription(Transaction));
    memmove(saTransactionGetDescriptions(Transaction)+to,saTransactionGetDescriptions(Transaction)+from,((int32)count)*sizeof(char));
}
utInlineC void saTransactionCopyDescriptions(saTransaction Transaction, uint32 x, char * values, uint32 count) {
    utAssert((x+count) <= saTransactionGetNumDescription(Transaction));
    memcpy(saTransactionGetDescriptions(Transaction)+x, values, count*sizeof(char));
}
utInlineC void saTransactionAppendDescriptions(saTransaction Transaction, char * values, uint32 count) {
    uint32 num = saTransactionGetNumDescription(Transaction);
    saTransactionResizeDescriptions(Transaction, num+count);
    saTransactionCopyDescriptions(Transaction, num, values, count);
}
utInlineC void saTransactionAppendDescription(saTransaction Transaction, char Description) {
    saTransactionResizeDescriptions(Transaction, saTransactionGetNumDescription(Transaction)+1);
    saTransactionSetiDescription(Transaction, saTransactionGetNumDescription(Transaction)-1, Description);
}
utInlineC void saTransactionInsertDescriptions(saTransaction Transaction, uint32 x, char *Description, uint32 count) {
    utAssert(x <= saTransactionGetNumDescription(Transaction));
    if(x < saTransactionGetNumDescription(Transaction)) {
        saTransactionResizeDescriptions(Transaction, saTransactionGetNumDescription(Transaction)+count);
        saTransactionMoveDescriptions(Transaction, x, x+count, saTransactionGetNumDescription(Transaction)-x-count);
        saTransactionCopyDescriptions(Transaction, x, Description, count);
    }
    else {
        saTransactionAppendDescriptions(Transaction, Description, count);
    }
}
utInlineC void saTransactionInsertDescription(saTransaction Transaction, uint32 x, char Description) {
    saTransactionInsertDescriptions(Transaction, x, &Description, 1);
}
utInlineC void saTransactionRemoveDescriptions(saTransaction Transaction, uint32 x, uint32 count) {
    utAssert((x+count) <= saTransactionGetNumDescription(Transaction));
    if((x+count) < saTransactionGetNumDescription(Transaction)) {
        saTransactionMoveDescriptions(Transaction, x+count,x,saTransactionGetNumDescription(Transaction)-x-count);
    }
    saTransactionResizeDescriptions(Transaction, saTransactionGetNumDescription(Transaction)-(int32)count);
}
utInlineC void saTransactionRemoveDescription(saTransaction Transaction, uint32 x) {
    saTransactionRemoveDescriptions(Transaction, x, 1);
}
utInlineC void saTransactionSwapDescription(saTransaction Transaction, uint32 from, uint32 to) {
    utAssert(from <= saTransactionGetNumDescription(Transaction));
    utAssert(to <= saTransactionGetNumDescription(Transaction));
    char tmp = saTransactionGetiDescription(Transaction, from);
    saTransactionSetiDescription(Transaction, from, saTransactionGetiDescription(Transaction, to));
    saTransactionSetiDescription(Transaction, to, tmp);
}
utInlineC void saTransactionSwapDescriptions(saTransaction Transaction, uint32 from, uint32 to, uint32 count) {
    utAssert((from+count) < saTransactionGetNumDescription(Transaction));
    utAssert((to+count) < saTransactionGetNumDescription(Transaction));
    char tmp[count];
    memcpy(tmp, saTransactionGetDescriptions(Transaction)+from, count*sizeof(char));
    memcpy(saTransactionGetDescriptions(Transaction)+from, saTransactionGetDescriptions(Transaction)+to, count*sizeof(char));
    memcpy(saTransactionGetDescriptions(Transaction)+to, tmp, count*sizeof(char));
}
#define saForeachTransactionDescription(pVar, cVar) { \
    uint32 _xDescription; \
    for(_xDescription = 0; _xDescription < saTransactionGetNumDescription(pVar); _xDescription++) { \
        cVar = saTransactionGetiDescription(pVar, _xDescription);
#define saEndTransactionDescription }}
utInlineC saUser saTransactionGetFromUser(saTransaction Transaction) {return saTransactions.FromUser[saTransaction2ValidIndex(Transaction)];}
utInlineC void saTransactionSetFromUser(saTransaction Transaction, saUser value) {saTransactions.FromUser[saTransaction2ValidIndex(Transaction)] = value;}
utInlineC saTransaction saTransactionGetNextUserOutTransaction(saTransaction Transaction) {return saTransactions.NextUserOutTransaction[saTransaction2ValidIndex(Transaction)];}
utInlineC void saTransactionSetNextUserOutTransaction(saTransaction Transaction, saTransaction value) {saTransactions.NextUserOutTransaction[saTransaction2ValidIndex(Transaction)] = value;}
utInlineC saTransaction saTransactionGetPrevUserOutTransaction(saTransaction Transaction) {return saTransactions.PrevUserOutTransaction[saTransaction2ValidIndex(Transaction)];}
utInlineC void saTransactionSetPrevUserOutTransaction(saTransaction Transaction, saTransaction value) {saTransactions.PrevUserOutTransaction[saTransaction2ValidIndex(Transaction)] = value;}
utInlineC saUser saTransactionGetToUser(saTransaction Transaction) {return saTransactions.ToUser[saTransaction2ValidIndex(Transaction)];}
utInlineC void saTransactionSetToUser(saTransaction Transaction, saUser value) {saTransactions.ToUser[saTransaction2ValidIndex(Transaction)] = value;}
utInlineC saTransaction saTransactionGetNextUserInTransaction(saTransaction Transaction) {return saTransactions.NextUserInTransaction[saTransaction2ValidIndex(Transaction)];}
utInlineC void saTransactionSetNextUserInTransaction(saTransaction Transaction, saTransaction value) {saTransactions.NextUserInTransaction[saTransaction2ValidIndex(Transaction)] = value;}
utInlineC saTransaction saTransactionGetPrevUserInTransaction(saTransaction Transaction) {return saTransactions.PrevUserInTransaction[saTransaction2ValidIndex(Transaction)];}
utInlineC void saTransactionSetPrevUserInTransaction(saTransaction Transaction, saTransaction value) {saTransactions.PrevUserInTransaction[saTransaction2ValidIndex(Transaction)] = value;}
utInlineC void saTransactionSetConstructorCallback(void(*func)(saTransaction)) {saTransactionConstructorCallback = func;}
utInlineC saTransactionCallbackType saTransactionGetConstructorCallback(void) {return saTransactionConstructorCallback;}
utInlineC void saTransactionSetDestructorCallback(void(*func)(saTransaction)) {saTransactionDestructorCallback = func;}
utInlineC saTransactionCallbackType saTransactionGetDestructorCallback(void) {return saTransactionDestructorCallback;}
utInlineC saTransaction saTransactionNextFree(saTransaction Transaction) {return ((saTransaction *)(void *)(saTransactions.FromUser))[saTransaction2ValidIndex(Transaction)];}
utInlineC void saTransactionSetNextFree(saTransaction Transaction, saTransaction value) {
    ((saTransaction *)(void *)(saTransactions.FromUser))[saTransaction2ValidIndex(Transaction)] = value;}
utInlineC void saTransactionFree(saTransaction Transaction) {
    saTransactionFreeDescriptions(Transaction);
    saTransactionSetNextFree(Transaction, saRootData.firstFreeTransaction);
    saSetFirstFreeTransaction(Transaction);}
void saTransactionDestroy(saTransaction Transaction);
utInlineC saTransaction saTransactionAllocRaw(void) {
    saTransaction Transaction;
    if(saRootData.firstFreeTransaction != saTransactionNull) {
        Transaction = saRootData.firstFreeTransaction;
        saSetFirstFreeTransaction(saTransactionNextFree(Transaction));
    } else {
        if(saRootData.usedTransaction == saRootData.allocatedTransaction) {
            saTransactionAllocMore();
        }
        Transaction = saIndex2Transaction(saRootData.usedTransaction);
        saSetUsedTransaction(saUsedTransaction() + 1);
    }
    return Transaction;}
utInlineC saTransaction saTransactionAlloc(void) {
    saTransaction Transaction = saTransactionAllocRaw();
    saTransactionSetDate(Transaction, 0);
    saTransactionSetAmount(Transaction, 0);
    saTransactionSetDescriptionIndex_(Transaction, 0);
    saTransactionSetNumDescription(Transaction, 0);
    saTransactionSetNumDescription(Transaction, 0);
    saTransactionSetFromUser(Transaction, saUserNull);
    saTransactionSetNextUserOutTransaction(Transaction, saTransactionNull);
    saTransactionSetPrevUserOutTransaction(Transaction, saTransactionNull);
    saTransactionSetToUser(Transaction, saUserNull);
    saTransactionSetNextUserInTransaction(Transaction, saTransactionNull);
    saTransactionSetPrevUserInTransaction(Transaction, saTransactionNull);
    if(saTransactionConstructorCallback != NULL) {
        saTransactionConstructorCallback(Transaction);
    }
    return Transaction;}

/*----------------------------------------------------------------------------------------
  Fields for class Charity.
----------------------------------------------------------------------------------------*/
struct saCharityFields {
    saRoot *Root;
    saCharity *NextRootCharity;
    saCharity *PrevRootCharity;
    saUser *User;
    saMembership *FirstMembership;
    saMembership *LastMembership;
};
extern struct saCharityFields saCharitys;

void saCharityAllocMore(void);
void saCharityCopyProps(saCharity saOldCharity, saCharity saNewCharity);
utInlineC saRoot saCharityGetRoot(saCharity Charity) {return saCharitys.Root[saCharity2ValidIndex(Charity)];}
utInlineC void saCharitySetRoot(saCharity Charity, saRoot value) {saCharitys.Root[saCharity2ValidIndex(Charity)] = value;}
utInlineC saCharity saCharityGetNextRootCharity(saCharity Charity) {return saCharitys.NextRootCharity[saCharity2ValidIndex(Charity)];}
utInlineC void saCharitySetNextRootCharity(saCharity Charity, saCharity value) {saCharitys.NextRootCharity[saCharity2ValidIndex(Charity)] = value;}
utInlineC saCharity saCharityGetPrevRootCharity(saCharity Charity) {return saCharitys.PrevRootCharity[saCharity2ValidIndex(Charity)];}
utInlineC void saCharitySetPrevRootCharity(saCharity Charity, saCharity value) {saCharitys.PrevRootCharity[saCharity2ValidIndex(Charity)] = value;}
utInlineC saUser saCharityGetUser(saCharity Charity) {return saCharitys.User[saCharity2ValidIndex(Charity)];}
utInlineC void saCharitySetUser(saCharity Charity, saUser value) {saCharitys.User[saCharity2ValidIndex(Charity)] = value;}
utInlineC saMembership saCharityGetFirstMembership(saCharity Charity) {return saCharitys.FirstMembership[saCharity2ValidIndex(Charity)];}
utInlineC void saCharitySetFirstMembership(saCharity Charity, saMembership value) {saCharitys.FirstMembership[saCharity2ValidIndex(Charity)] = value;}
utInlineC saMembership saCharityGetLastMembership(saCharity Charity) {return saCharitys.LastMembership[saCharity2ValidIndex(Charity)];}
utInlineC void saCharitySetLastMembership(saCharity Charity, saMembership value) {saCharitys.LastMembership[saCharity2ValidIndex(Charity)] = value;}
utInlineC void saCharitySetConstructorCallback(void(*func)(saCharity)) {saCharityConstructorCallback = func;}
utInlineC saCharityCallbackType saCharityGetConstructorCallback(void) {return saCharityConstructorCallback;}
utInlineC void saCharitySetDestructorCallback(void(*func)(saCharity)) {saCharityDestructorCallback = func;}
utInlineC saCharityCallbackType saCharityGetDestructorCallback(void) {return saCharityDestructorCallback;}
utInlineC saCharity saCharityNextFree(saCharity Charity) {return ((saCharity *)(void *)(saCharitys.Root))[saCharity2ValidIndex(Charity)];}
utInlineC void saCharitySetNextFree(saCharity Charity, saCharity value) {
    ((saCharity *)(void *)(saCharitys.Root))[saCharity2ValidIndex(Charity)] = value;}
utInlineC void saCharityFree(saCharity Charity) {
    saCharitySetNextFree(Charity, saRootData.firstFreeCharity);
    saSetFirstFreeCharity(Charity);}
void saCharityDestroy(saCharity Charity);
utInlineC saCharity saCharityAllocRaw(void) {
    saCharity Charity;
    if(saRootData.firstFreeCharity != saCharityNull) {
        Charity = saRootData.firstFreeCharity;
        saSetFirstFreeCharity(saCharityNextFree(Charity));
    } else {
        if(saRootData.usedCharity == saRootData.allocatedCharity) {
            saCharityAllocMore();
        }
        Charity = saIndex2Charity(saRootData.usedCharity);
        saSetUsedCharity(saUsedCharity() + 1);
    }
    return Charity;}
utInlineC saCharity saCharityAlloc(void) {
    saCharity Charity = saCharityAllocRaw();
    saCharitySetRoot(Charity, saRootNull);
    saCharitySetNextRootCharity(Charity, saCharityNull);
    saCharitySetPrevRootCharity(Charity, saCharityNull);
    saCharitySetUser(Charity, saUserNull);
    saCharitySetFirstMembership(Charity, saMembershipNull);
    saCharitySetLastMembership(Charity, saMembershipNull);
    if(saCharityConstructorCallback != NULL) {
        saCharityConstructorCallback(Charity);
    }
    return Charity;}

/*----------------------------------------------------------------------------------------
  Fields for class Membership.
----------------------------------------------------------------------------------------*/
struct saMembershipFields {
    saUser *User;
    saMembership *NextUserMembership;
    saMembership *PrevUserMembership;
    saMembership *NextTableUserMembership;
    saCharity *Charity;
    saMembership *NextCharityMembership;
    saMembership *PrevCharityMembership;
};
extern struct saMembershipFields saMemberships;

void saMembershipAllocMore(void);
void saMembershipCopyProps(saMembership saOldMembership, saMembership saNewMembership);
utInlineC saUser saMembershipGetUser(saMembership Membership) {return saMemberships.User[saMembership2ValidIndex(Membership)];}
utInlineC void saMembershipSetUser(saMembership Membership, saUser value) {saMemberships.User[saMembership2ValidIndex(Membership)] = value;}
utInlineC saMembership saMembershipGetNextUserMembership(saMembership Membership) {return saMemberships.NextUserMembership[saMembership2ValidIndex(Membership)];}
utInlineC void saMembershipSetNextUserMembership(saMembership Membership, saMembership value) {saMemberships.NextUserMembership[saMembership2ValidIndex(Membership)] = value;}
utInlineC saMembership saMembershipGetPrevUserMembership(saMembership Membership) {return saMemberships.PrevUserMembership[saMembership2ValidIndex(Membership)];}
utInlineC void saMembershipSetPrevUserMembership(saMembership Membership, saMembership value) {saMemberships.PrevUserMembership[saMembership2ValidIndex(Membership)] = value;}
utInlineC saMembership saMembershipGetNextTableUserMembership(saMembership Membership) {return saMemberships.NextTableUserMembership[saMembership2ValidIndex(Membership)];}
utInlineC void saMembershipSetNextTableUserMembership(saMembership Membership, saMembership value) {saMemberships.NextTableUserMembership[saMembership2ValidIndex(Membership)] = value;}
utInlineC saCharity saMembershipGetCharity(saMembership Membership) {return saMemberships.Charity[saMembership2ValidIndex(Membership)];}
utInlineC void saMembershipSetCharity(saMembership Membership, saCharity value) {saMemberships.Charity[saMembership2ValidIndex(Membership)] = value;}
utInlineC saMembership saMembershipGetNextCharityMembership(saMembership Membership) {return saMemberships.NextCharityMembership[saMembership2ValidIndex(Membership)];}
utInlineC void saMembershipSetNextCharityMembership(saMembership Membership, saMembership value) {saMemberships.NextCharityMembership[saMembership2ValidIndex(Membership)] = value;}
utInlineC saMembership saMembershipGetPrevCharityMembership(saMembership Membership) {return saMemberships.PrevCharityMembership[saMembership2ValidIndex(Membership)];}
utInlineC void saMembershipSetPrevCharityMembership(saMembership Membership, saMembership value) {saMemberships.PrevCharityMembership[saMembership2ValidIndex(Membership)] = value;}
utInlineC void saMembershipSetConstructorCallback(void(*func)(saMembership)) {saMembershipConstructorCallback = func;}
utInlineC saMembershipCallbackType saMembershipGetConstructorCallback(void) {return saMembershipConstructorCallback;}
utInlineC void saMembershipSetDestructorCallback(void(*func)(saMembership)) {saMembershipDestructorCallback = func;}
utInlineC saMembershipCallbackType saMembershipGetDestructorCallback(void) {return saMembershipDestructorCallback;}
utInlineC saMembership saMembershipNextFree(saMembership Membership) {return ((saMembership *)(void *)(saMemberships.User))[saMembership2ValidIndex(Membership)];}
utInlineC void saMembershipSetNextFree(saMembership Membership, saMembership value) {
    ((saMembership *)(void *)(saMemberships.User))[saMembership2ValidIndex(Membership)] = value;}
utInlineC void saMembershipFree(saMembership Membership) {
    saMembershipSetNextFree(Membership, saRootData.firstFreeMembership);
    saSetFirstFreeMembership(Membership);}
void saMembershipDestroy(saMembership Membership);
utInlineC saMembership saMembershipAllocRaw(void) {
    saMembership Membership;
    if(saRootData.firstFreeMembership != saMembershipNull) {
        Membership = saRootData.firstFreeMembership;
        saSetFirstFreeMembership(saMembershipNextFree(Membership));
    } else {
        if(saRootData.usedMembership == saRootData.allocatedMembership) {
            saMembershipAllocMore();
        }
        Membership = saIndex2Membership(saRootData.usedMembership);
        saSetUsedMembership(saUsedMembership() + 1);
    }
    return Membership;}
utInlineC saMembership saMembershipAlloc(void) {
    saMembership Membership = saMembershipAllocRaw();
    saMembershipSetUser(Membership, saUserNull);
    saMembershipSetNextUserMembership(Membership, saMembershipNull);
    saMembershipSetPrevUserMembership(Membership, saMembershipNull);
    saMembershipSetNextTableUserMembership(Membership, saMembershipNull);
    saMembershipSetCharity(Membership, saCharityNull);
    saMembershipSetNextCharityMembership(Membership, saMembershipNull);
    saMembershipSetPrevCharityMembership(Membership, saMembershipNull);
    if(saMembershipConstructorCallback != NULL) {
        saMembershipConstructorCallback(Membership);
    }
    return Membership;}

/*----------------------------------------------------------------------------------------
  Fields for class Category.
----------------------------------------------------------------------------------------*/
struct saCategoryFields {
    utSym *Sym;
    uint32 *NumListings;
    saRoot *Root;
    saCategory *NextRootCategory;
    saCategory *PrevRootCategory;
    saCategory *NextTableRootCategory;
    saListing *FirstListing;
    saListing *LastListing;
};
extern struct saCategoryFields saCategorys;

void saCategoryAllocMore(void);
void saCategoryCopyProps(saCategory saOldCategory, saCategory saNewCategory);
utInlineC utSym saCategoryGetSym(saCategory Category) {return saCategorys.Sym[saCategory2ValidIndex(Category)];}
utInlineC void saCategorySetSym(saCategory Category, utSym value) {saCategorys.Sym[saCategory2ValidIndex(Category)] = value;}
utInlineC uint32 saCategoryGetNumListings(saCategory Category) {return saCategorys.NumListings[saCategory2ValidIndex(Category)];}
utInlineC void saCategorySetNumListings(saCategory Category, uint32 value) {saCategorys.NumListings[saCategory2ValidIndex(Category)] = value;}
utInlineC saRoot saCategoryGetRoot(saCategory Category) {return saCategorys.Root[saCategory2ValidIndex(Category)];}
utInlineC void saCategorySetRoot(saCategory Category, saRoot value) {saCategorys.Root[saCategory2ValidIndex(Category)] = value;}
utInlineC saCategory saCategoryGetNextRootCategory(saCategory Category) {return saCategorys.NextRootCategory[saCategory2ValidIndex(Category)];}
utInlineC void saCategorySetNextRootCategory(saCategory Category, saCategory value) {saCategorys.NextRootCategory[saCategory2ValidIndex(Category)] = value;}
utInlineC saCategory saCategoryGetPrevRootCategory(saCategory Category) {return saCategorys.PrevRootCategory[saCategory2ValidIndex(Category)];}
utInlineC void saCategorySetPrevRootCategory(saCategory Category, saCategory value) {saCategorys.PrevRootCategory[saCategory2ValidIndex(Category)] = value;}
utInlineC saCategory saCategoryGetNextTableRootCategory(saCategory Category) {return saCategorys.NextTableRootCategory[saCategory2ValidIndex(Category)];}
utInlineC void saCategorySetNextTableRootCategory(saCategory Category, saCategory value) {saCategorys.NextTableRootCategory[saCategory2ValidIndex(Category)] = value;}
utInlineC saListing saCategoryGetFirstListing(saCategory Category) {return saCategorys.FirstListing[saCategory2ValidIndex(Category)];}
utInlineC void saCategorySetFirstListing(saCategory Category, saListing value) {saCategorys.FirstListing[saCategory2ValidIndex(Category)] = value;}
utInlineC saListing saCategoryGetLastListing(saCategory Category) {return saCategorys.LastListing[saCategory2ValidIndex(Category)];}
utInlineC void saCategorySetLastListing(saCategory Category, saListing value) {saCategorys.LastListing[saCategory2ValidIndex(Category)] = value;}
utInlineC void saCategorySetConstructorCallback(void(*func)(saCategory)) {saCategoryConstructorCallback = func;}
utInlineC saCategoryCallbackType saCategoryGetConstructorCallback(void) {return saCategoryConstructorCallback;}
utInlineC void saCategorySetDestructorCallback(void(*func)(saCategory)) {saCategoryDestructorCallback = func;}
utInlineC saCategoryCallbackType saCategoryGetDestructorCallback(void) {return saCategoryDestructorCallback;}
utInlineC saCategory saCategoryNextFree(saCategory Category) {return ((saCategory *)(void *)(saCategorys.Sym))[saCategory2ValidIndex(Category)];}
utInlineC void saCategorySetNextFree(saCategory Category, saCategory value) {
    ((saCategory *)(void *)(saCategorys.Sym))[saCategory2ValidIndex(Category)] = value;}
utInlineC void saCategoryFree(saCategory Category) {
    saCategorySetNextFree(Category, saRootData.firstFreeCategory);
    saSetFirstFreeCategory(Category);}
void saCategoryDestroy(saCategory Category);
utInlineC saCategory saCategoryAllocRaw(void) {
    saCategory Category;
    if(saRootData.firstFreeCategory != saCategoryNull) {
        Category = saRootData.firstFreeCategory;
        saSetFirstFreeCategory(saCategoryNextFree(Category));
    } else {
        if(saRootData.usedCategory == saRootData.allocatedCategory) {
            saCategoryAllocMore();
        }
        Category = saIndex2Category(saRootData.usedCategory);
        saSetUsedCategory(saUsedCategory() + 1);
    }
    return Category;}
utInlineC saCategory saCategoryAlloc(void) {
    saCategory Category = saCategoryAllocRaw();
    saCategorySetSym(Category, utSymNull);
    saCategorySetNumListings(Category, 0);
    saCategorySetRoot(Category, saRootNull);
    saCategorySetNextRootCategory(Category, saCategoryNull);
    saCategorySetPrevRootCategory(Category, saCategoryNull);
    saCategorySetNextTableRootCategory(Category, saCategoryNull);
    saCategorySetFirstListing(Category, saListingNull);
    saCategorySetLastListing(Category, saListingNull);
    if(saCategoryConstructorCallback != NULL) {
        saCategoryConstructorCallback(Category);
    }
    return Category;}

/*----------------------------------------------------------------------------------------
  Fields for class Listing.
----------------------------------------------------------------------------------------*/
struct saListingFields {
    uint32 *TitleIndex_;
    uint32 *NumTitle;
    char *Title;
    uint32 *DescriptionIndex_;
    uint32 *NumDescription;
    char *Description;
    uint64 *ID;
    uint32 *Rate;
    uint8 *Offered;
    uint8 *FixedPrice;
    saRoot *Root;
    saListing *NextRootListing;
    saListing *PrevRootListing;
    saListing *NextTableRootListing;
    saUser *User;
    saListing *NextUserListing;
    saListing *PrevUserListing;
    saCategory *Category;
    saListing *NextCategoryListing;
    saListing *PrevCategoryListing;
};
extern struct saListingFields saListings;

void saListingAllocMore(void);
void saListingCopyProps(saListing saOldListing, saListing saNewListing);
void saListingAllocTitles(saListing Listing, uint32 numTitles);
void saListingResizeTitles(saListing Listing, uint32 numTitles);
void saListingFreeTitles(saListing Listing);
void saCompactListingTitles(void);
void saListingAllocDescriptions(saListing Listing, uint32 numDescriptions);
void saListingResizeDescriptions(saListing Listing, uint32 numDescriptions);
void saListingFreeDescriptions(saListing Listing);
void saCompactListingDescriptions(void);
utInlineC uint32 saListingGetTitleIndex_(saListing Listing) {return saListings.TitleIndex_[saListing2ValidIndex(Listing)];}
utInlineC void saListingSetTitleIndex_(saListing Listing, uint32 value) {saListings.TitleIndex_[saListing2ValidIndex(Listing)] = value;}
utInlineC uint32 saListingGetNumTitle(saListing Listing) {return saListings.NumTitle[saListing2ValidIndex(Listing)];}
utInlineC void saListingSetNumTitle(saListing Listing, uint32 value) {saListings.NumTitle[saListing2ValidIndex(Listing)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saListingCheckTitleIndex(saListing Listing, uint32 x) {utAssert(x < saListingGetNumTitle(Listing)); return x;}
#else
utInlineC uint32 saListingCheckTitleIndex(saListing Listing, uint32 x) {return x;}
#endif
utInlineC char saListingGetiTitle(saListing Listing, uint32 x) {return saListings.Title[
    saListingGetTitleIndex_(Listing) + saListingCheckTitleIndex(Listing, x)];}
utInlineC char *saListingGetTitle(saListing Listing) {return saListings.Title + saListingGetTitleIndex_(Listing);}
#define saListingGetTitles saListingGetTitle
utInlineC void saListingSetTitle(saListing Listing, char *valuePtr, uint32 numTitle) {
    saListingResizeTitles(Listing, numTitle);
    memcpy(saListingGetTitles(Listing), valuePtr, numTitle*sizeof(char));}
utInlineC void saListingSetiTitle(saListing Listing, uint32 x, char value) {
    saListings.Title[saListingGetTitleIndex_(Listing) + saListingCheckTitleIndex(Listing, (x))] = value;}
utInlineC void saListingMoveTitles(saListing Listing, uint32 from, uint32 to, uint32 count) {
    utAssert((to+count) <= saListingGetNumTitle(Listing));
    utAssert((from+count) <= saListingGetNumTitle(Listing));
    memmove(saListingGetTitles(Listing)+to,saListingGetTitles(Listing)+from,((int32)count)*sizeof(char));
}
utInlineC void saListingCopyTitles(saListing Listing, uint32 x, char * values, uint32 count) {
    utAssert((x+count) <= saListingGetNumTitle(Listing));
    memcpy(saListingGetTitles(Listing)+x, values, count*sizeof(char));
}
utInlineC void saListingAppendTitles(saListing Listing, char * values, uint32 count) {
    uint32 num = saListingGetNumTitle(Listing);
    saListingResizeTitles(Listing, num+count);
    saListingCopyTitles(Listing, num, values, count);
}
utInlineC void saListingAppendTitle(saListing Listing, char Title) {
    saListingResizeTitles(Listing, saListingGetNumTitle(Listing)+1);
    saListingSetiTitle(Listing, saListingGetNumTitle(Listing)-1, Title);
}
utInlineC void saListingInsertTitles(saListing Listing, uint32 x, char *Title, uint32 count) {
    utAssert(x <= saListingGetNumTitle(Listing));
    if(x < saListingGetNumTitle(Listing)) {
        saListingResizeTitles(Listing, saListingGetNumTitle(Listing)+count);
        saListingMoveTitles(Listing, x, x+count, saListingGetNumTitle(Listing)-x-count);
        saListingCopyTitles(Listing, x, Title, count);
    }
    else {
        saListingAppendTitles(Listing, Title, count);
    }
}
utInlineC void saListingInsertTitle(saListing Listing, uint32 x, char Title) {
    saListingInsertTitles(Listing, x, &Title, 1);
}
utInlineC void saListingRemoveTitles(saListing Listing, uint32 x, uint32 count) {
    utAssert((x+count) <= saListingGetNumTitle(Listing));
    if((x+count) < saListingGetNumTitle(Listing)) {
        saListingMoveTitles(Listing, x+count,x,saListingGetNumTitle(Listing)-x-count);
    }
    saListingResizeTitles(Listing, saListingGetNumTitle(Listing)-(int32)count);
}
utInlineC void saListingRemoveTitle(saListing Listing, uint32 x) {
    saListingRemoveTitles(Listing, x, 1);
}
utInlineC void saListingSwapTitle(saListing Listing, uint32 from, uint32 to) {
    utAssert(from <= saListingGetNumTitle(Listing));
    utAssert(to <= saListingGetNumTitle(Listing));
    char tmp = saListingGetiTitle(Listing, from);
    saListingSetiTitle(Listing, from, saListingGetiTitle(Listing, to));
    saListingSetiTitle(Listing, to, tmp);
}
utInlineC void saListingSwapTitles(saListing Listing, uint32 from, uint32 to, uint32 count) {
    utAssert((from+count) < saListingGetNumTitle(Listing));
    utAssert((to+count) < saListingGetNumTitle(Listing));
    char tmp[count];
    memcpy(tmp, saListingGetTitles(Listing)+from, count*sizeof(char));
    memcpy(saListingGetTitles(Listing)+from, saListingGetTitles(Listing)+to, count*sizeof(char));
    memcpy(saListingGetTitles(Listing)+to, tmp, count*sizeof(char));
}
#define saForeachListingTitle(pVar, cVar) { \
    uint32 _xTitle; \
    for(_xTitle = 0; _xTitle < saListingGetNumTitle(pVar); _xTitle++) { \
        cVar = saListingGetiTitle(pVar, _xTitle);
#define saEndListingTitle }}
utInlineC uint32 saListingGetDescriptionIndex_(saListing Listing) {return saListings.DescriptionIndex_[saListing2ValidIndex(Listing)];}
utInlineC void saListingSetDescriptionIndex_(saListing Listing, uint32 value) {saListings.DescriptionIndex_[saListing2ValidIndex(Listing)] = value;}
utInlineC uint32 saListingGetNumDescription(saListing Listing) {return saListings.NumDescription[saListing2ValidIndex(Listing)];}
utInlineC void saListingSetNumDescription(saListing Listing, uint32 value) {saListings.NumDescription[saListing2ValidIndex(Listing)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saListingCheckDescriptionIndex(saListing Listing, uint32 x) {utAssert(x < saListingGetNumDescription(Listing)); return x;}
#else
utInlineC uint32 saListingCheckDescriptionIndex(saListing Listing, uint32 x) {return x;}
#endif
utInlineC char saListingGetiDescription(saListing Listing, uint32 x) {return saListings.Description[
    saListingGetDescriptionIndex_(Listing) + saListingCheckDescriptionIndex(Listing, x)];}
utInlineC char *saListingGetDescription(saListing Listing) {return saListings.Description + saListingGetDescriptionIndex_(Listing);}
#define saListingGetDescriptions saListingGetDescription
utInlineC void saListingSetDescription(saListing Listing, char *valuePtr, uint32 numDescription) {
    saListingResizeDescriptions(Listing, numDescription);
    memcpy(saListingGetDescriptions(Listing), valuePtr, numDescription*sizeof(char));}
utInlineC void saListingSetiDescription(saListing Listing, uint32 x, char value) {
    saListings.Description[saListingGetDescriptionIndex_(Listing) + saListingCheckDescriptionIndex(Listing, (x))] = value;}
utInlineC void saListingMoveDescriptions(saListing Listing, uint32 from, uint32 to, uint32 count) {
    utAssert((to+count) <= saListingGetNumDescription(Listing));
    utAssert((from+count) <= saListingGetNumDescription(Listing));
    memmove(saListingGetDescriptions(Listing)+to,saListingGetDescriptions(Listing)+from,((int32)count)*sizeof(char));
}
utInlineC void saListingCopyDescriptions(saListing Listing, uint32 x, char * values, uint32 count) {
    utAssert((x+count) <= saListingGetNumDescription(Listing));
    memcpy(saListingGetDescriptions(Listing)+x, values, count*sizeof(char));
}
utInlineC void saListingAppendDescriptions(saListing Listing, char * values, uint32 count) {
    uint32 num = saListingGetNumDescription(Listing);
    saListingResizeDescriptions(Listing, num+count);
    saListingCopyDescriptions(Listing, num, values, count);
}
utInlineC void saListingAppendDescription(saListing Listing, char Description) {
    saListingResizeDescriptions(Listing, saListingGetNumDescription(Listing)+1);
    saListingSetiDescription(Listing, saListingGetNumDescription(Listing)-1, Description);
}
utInlineC void saListingInsertDescriptions(saListing Listing, uint32 x, char *Description, uint32 count) {
    utAssert(x <= saListingGetNumDescription(Listing));
    if(x < saListingGetNumDescription(Listing)) {
        saListingResizeDescriptions(Listing, saListingGetNumDescription(Listing)+count);
        saListingMoveDescriptions(Listing, x, x+count, saListingGetNumDescription(Listing)-x-count);
        saListingCopyDescriptions(Listing, x, Description, count);
    }
    else {
        saListingAppendDescriptions(Listing, Description, count);
    }
}
utInlineC void saListingInsertDescription(saListing Listing, uint32 x, char Description) {
    saListingInsertDescriptions(Listing, x, &Description, 1);
}
utInlineC void saListingRemoveDescriptions(saListing Listing, uint32 x, uint32 count) {
    utAssert((x+count) <= saListingGetNumDescription(Listing));
    if((x+count) < saListingGetNumDescription(Listing)) {
        saListingMoveDescriptions(Listing, x+count,x,saListingGetNumDescription(Listing)-x-count);
    }
    saListingResizeDescriptions(Listing, saListingGetNumDescription(Listing)-(int32)count);
}
utInlineC void saListingRemoveDescription(saListing Listing, uint32 x) {
    saListingRemoveDescriptions(Listing, x, 1);
}
utInlineC void saListingSwapDescription(saListing Listing, uint32 from, uint32 to) {
    utAssert(from <= saListingGetNumDescription(Listing));
    utAssert(to <= saListingGetNumDescription(Listing));
    char tmp = saListingGetiDescription(Listing, from);
    saListingSetiDescription(Listing, from, saListingGetiDescription(Listing, to));
    saListingSetiDescription(Listing, to, tmp);
}
utInlineC void saListingSwapDescriptions(saListing Listing, uint32 from, uint32 to, uint32 count) {
    utAssert((from+count) < saListingGetNumDescription(Listing));
    utAssert((to+count) < saListingGetNumDescription(Listing));
    char tmp[count];
    memcpy(tmp, saListingGetDescriptions(Listing)+from, count*sizeof(char));
    memcpy(saListingGetDescriptions(Listing)+from, saListingGetDescriptions(Listing)+to, count*sizeof(char));
    memcpy(saListingGetDescriptions(Listing)+to, tmp, count*sizeof(char));
}
#define saForeachListingDescription(pVar, cVar) { \
    uint32 _xDescription; \
    for(_xDescription = 0; _xDescription < saListingGetNumDescription(pVar); _xDescription++) { \
        cVar = saListingGetiDescription(pVar, _xDescription);
#define saEndListingDescription }}
utInlineC uint64 saListingGetID(saListing Listing) {return saListings.ID[saListing2ValidIndex(Listing)];}
utInlineC void saListingSetID(saListing Listing, uint64 value) {saListings.ID[saListing2ValidIndex(Listing)] = value;}
utInlineC uint32 saListingGetRate(saListing Listing) {return saListings.Rate[saListing2ValidIndex(Listing)];}
utInlineC void saListingSetRate(saListing Listing, uint32 value) {saListings.Rate[saListing2ValidIndex(Listing)] = value;}
utInlineC uint8 saListingOffered(saListing Listing) {return saListings.Offered[saListing2ValidIndex(Listing)];}
utInlineC void saListingSetOffered(saListing Listing, uint8 value) {saListings.Offered[saListing2ValidIndex(Listing)] = value;}
utInlineC uint8 saListingFixedPrice(saListing Listing) {return saListings.FixedPrice[saListing2ValidIndex(Listing)];}
utInlineC void saListingSetFixedPrice(saListing Listing, uint8 value) {saListings.FixedPrice[saListing2ValidIndex(Listing)] = value;}
utInlineC saRoot saListingGetRoot(saListing Listing) {return saListings.Root[saListing2ValidIndex(Listing)];}
utInlineC void saListingSetRoot(saListing Listing, saRoot value) {saListings.Root[saListing2ValidIndex(Listing)] = value;}
utInlineC saListing saListingGetNextRootListing(saListing Listing) {return saListings.NextRootListing[saListing2ValidIndex(Listing)];}
utInlineC void saListingSetNextRootListing(saListing Listing, saListing value) {saListings.NextRootListing[saListing2ValidIndex(Listing)] = value;}
utInlineC saListing saListingGetPrevRootListing(saListing Listing) {return saListings.PrevRootListing[saListing2ValidIndex(Listing)];}
utInlineC void saListingSetPrevRootListing(saListing Listing, saListing value) {saListings.PrevRootListing[saListing2ValidIndex(Listing)] = value;}
utInlineC saListing saListingGetNextTableRootListing(saListing Listing) {return saListings.NextTableRootListing[saListing2ValidIndex(Listing)];}
utInlineC void saListingSetNextTableRootListing(saListing Listing, saListing value) {saListings.NextTableRootListing[saListing2ValidIndex(Listing)] = value;}
utInlineC saUser saListingGetUser(saListing Listing) {return saListings.User[saListing2ValidIndex(Listing)];}
utInlineC void saListingSetUser(saListing Listing, saUser value) {saListings.User[saListing2ValidIndex(Listing)] = value;}
utInlineC saListing saListingGetNextUserListing(saListing Listing) {return saListings.NextUserListing[saListing2ValidIndex(Listing)];}
utInlineC void saListingSetNextUserListing(saListing Listing, saListing value) {saListings.NextUserListing[saListing2ValidIndex(Listing)] = value;}
utInlineC saListing saListingGetPrevUserListing(saListing Listing) {return saListings.PrevUserListing[saListing2ValidIndex(Listing)];}
utInlineC void saListingSetPrevUserListing(saListing Listing, saListing value) {saListings.PrevUserListing[saListing2ValidIndex(Listing)] = value;}
utInlineC saCategory saListingGetCategory(saListing Listing) {return saListings.Category[saListing2ValidIndex(Listing)];}
utInlineC void saListingSetCategory(saListing Listing, saCategory value) {saListings.Category[saListing2ValidIndex(Listing)] = value;}
utInlineC saListing saListingGetNextCategoryListing(saListing Listing) {return saListings.NextCategoryListing[saListing2ValidIndex(Listing)];}
utInlineC void saListingSetNextCategoryListing(saListing Listing, saListing value) {saListings.NextCategoryListing[saListing2ValidIndex(Listing)] = value;}
utInlineC saListing saListingGetPrevCategoryListing(saListing Listing) {return saListings.PrevCategoryListing[saListing2ValidIndex(Listing)];}
utInlineC void saListingSetPrevCategoryListing(saListing Listing, saListing value) {saListings.PrevCategoryListing[saListing2ValidIndex(Listing)] = value;}
utInlineC void saListingSetConstructorCallback(void(*func)(saListing)) {saListingConstructorCallback = func;}
utInlineC saListingCallbackType saListingGetConstructorCallback(void) {return saListingConstructorCallback;}
utInlineC void saListingSetDestructorCallback(void(*func)(saListing)) {saListingDestructorCallback = func;}
utInlineC saListingCallbackType saListingGetDestructorCallback(void) {return saListingDestructorCallback;}
utInlineC saListing saListingNextFree(saListing Listing) {return ((saListing *)(void *)(saListings.Root))[saListing2ValidIndex(Listing)];}
utInlineC void saListingSetNextFree(saListing Listing, saListing value) {
    ((saListing *)(void *)(saListings.Root))[saListing2ValidIndex(Listing)] = value;}
utInlineC void saListingFree(saListing Listing) {
    saListingFreeTitles(Listing);
    saListingFreeDescriptions(Listing);
    saListingSetNextFree(Listing, saRootData.firstFreeListing);
    saSetFirstFreeListing(Listing);}
void saListingDestroy(saListing Listing);
utInlineC saListing saListingAllocRaw(void) {
    saListing Listing;
    if(saRootData.firstFreeListing != saListingNull) {
        Listing = saRootData.firstFreeListing;
        saSetFirstFreeListing(saListingNextFree(Listing));
    } else {
        if(saRootData.usedListing == saRootData.allocatedListing) {
            saListingAllocMore();
        }
        Listing = saIndex2Listing(saRootData.usedListing);
        saSetUsedListing(saUsedListing() + 1);
    }
    return Listing;}
utInlineC saListing saListingAlloc(void) {
    saListing Listing = saListingAllocRaw();
    saListingSetTitleIndex_(Listing, 0);
    saListingSetNumTitle(Listing, 0);
    saListingSetNumTitle(Listing, 0);
    saListingSetDescriptionIndex_(Listing, 0);
    saListingSetNumDescription(Listing, 0);
    saListingSetNumDescription(Listing, 0);
    saListingSetID(Listing, 0);
    saListingSetRate(Listing, 0);
    saListingSetOffered(Listing, 0);
    saListingSetFixedPrice(Listing, 0);
    saListingSetRoot(Listing, saRootNull);
    saListingSetNextRootListing(Listing, saListingNull);
    saListingSetPrevRootListing(Listing, saListingNull);
    saListingSetNextTableRootListing(Listing, saListingNull);
    saListingSetUser(Listing, saUserNull);
    saListingSetNextUserListing(Listing, saListingNull);
    saListingSetPrevUserListing(Listing, saListingNull);
    saListingSetCategory(Listing, saCategoryNull);
    saListingSetNextCategoryListing(Listing, saListingNull);
    saListingSetPrevCategoryListing(Listing, saListingNull);
    if(saListingConstructorCallback != NULL) {
        saListingConstructorCallback(Listing);
    }
    return Listing;}

/*----------------------------------------------------------------------------------------
  Fields for class Thread.
----------------------------------------------------------------------------------------*/
struct saThreadFields {
    uint32 *SubjectIndex_;
    uint32 *NumSubject;
    char *Subject;
    uint32 *ID;
    uint32 *NumPosts;
    saRoot *Root;
    saThread *NextRootThread;
    saThread *PrevRootThread;
    saThread *NextTableRootThread;
    saPost *FirstPost;
    saPost *LastPost;
    saSubscription *FirstSubscription;
    saSubscription *LastSubscription;
};
extern struct saThreadFields saThreads;

void saThreadAllocMore(void);
void saThreadCopyProps(saThread saOldThread, saThread saNewThread);
void saThreadAllocSubjects(saThread Thread, uint32 numSubjects);
void saThreadResizeSubjects(saThread Thread, uint32 numSubjects);
void saThreadFreeSubjects(saThread Thread);
void saCompactThreadSubjects(void);
utInlineC uint32 saThreadGetSubjectIndex_(saThread Thread) {return saThreads.SubjectIndex_[saThread2ValidIndex(Thread)];}
utInlineC void saThreadSetSubjectIndex_(saThread Thread, uint32 value) {saThreads.SubjectIndex_[saThread2ValidIndex(Thread)] = value;}
utInlineC uint32 saThreadGetNumSubject(saThread Thread) {return saThreads.NumSubject[saThread2ValidIndex(Thread)];}
utInlineC void saThreadSetNumSubject(saThread Thread, uint32 value) {saThreads.NumSubject[saThread2ValidIndex(Thread)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saThreadCheckSubjectIndex(saThread Thread, uint32 x) {utAssert(x < saThreadGetNumSubject(Thread)); return x;}
#else
utInlineC uint32 saThreadCheckSubjectIndex(saThread Thread, uint32 x) {return x;}
#endif
utInlineC char saThreadGetiSubject(saThread Thread, uint32 x) {return saThreads.Subject[
    saThreadGetSubjectIndex_(Thread) + saThreadCheckSubjectIndex(Thread, x)];}
utInlineC char *saThreadGetSubject(saThread Thread) {return saThreads.Subject + saThreadGetSubjectIndex_(Thread);}
#define saThreadGetSubjects saThreadGetSubject
utInlineC void saThreadSetSubject(saThread Thread, char *valuePtr, uint32 numSubject) {
    saThreadResizeSubjects(Thread, numSubject);
    memcpy(saThreadGetSubjects(Thread), valuePtr, numSubject*sizeof(char));}
utInlineC void saThreadSetiSubject(saThread Thread, uint32 x, char value) {
    saThreads.Subject[saThreadGetSubjectIndex_(Thread) + saThreadCheckSubjectIndex(Thread, (x))] = value;}
utInlineC void saThreadMoveSubjects(saThread Thread, uint32 from, uint32 to, uint32 count) {
    utAssert((to+count) <= saThreadGetNumSubject(Thread));
    utAssert((from+count) <= saThreadGetNumSubject(Thread));
    memmove(saThreadGetSubjects(Thread)+to,saThreadGetSubjects(Thread)+from,((int32)count)*sizeof(char));
}
utInlineC void saThreadCopySubjects(saThread Thread, uint32 x, char * values, uint32 count) {
    utAssert((x+count) <= saThreadGetNumSubject(Thread));
    memcpy(saThreadGetSubjects(Thread)+x, values, count*sizeof(char));
}
utInlineC void saThreadAppendSubjects(saThread Thread, char * values, uint32 count) {
    uint32 num = saThreadGetNumSubject(Thread);
    saThreadResizeSubjects(Thread, num+count);
    saThreadCopySubjects(Thread, num, values, count);
}
utInlineC void saThreadAppendSubject(saThread Thread, char Subject) {
    saThreadResizeSubjects(Thread, saThreadGetNumSubject(Thread)+1);
    saThreadSetiSubject(Thread, saThreadGetNumSubject(Thread)-1, Subject);
}
utInlineC void saThreadInsertSubjects(saThread Thread, uint32 x, char *Subject, uint32 count) {
    utAssert(x <= saThreadGetNumSubject(Thread));
    if(x < saThreadGetNumSubject(Thread)) {
        saThreadResizeSubjects(Thread, saThreadGetNumSubject(Thread)+count);
        saThreadMoveSubjects(Thread, x, x+count, saThreadGetNumSubject(Thread)-x-count);
        saThreadCopySubjects(Thread, x, Subject, count);
    }
    else {
        saThreadAppendSubjects(Thread, Subject, count);
    }
}
utInlineC void saThreadInsertSubject(saThread Thread, uint32 x, char Subject) {
    saThreadInsertSubjects(Thread, x, &Subject, 1);
}
utInlineC void saThreadRemoveSubjects(saThread Thread, uint32 x, uint32 count) {
    utAssert((x+count) <= saThreadGetNumSubject(Thread));
    if((x+count) < saThreadGetNumSubject(Thread)) {
        saThreadMoveSubjects(Thread, x+count,x,saThreadGetNumSubject(Thread)-x-count);
    }
    saThreadResizeSubjects(Thread, saThreadGetNumSubject(Thread)-(int32)count);
}
utInlineC void saThreadRemoveSubject(saThread Thread, uint32 x) {
    saThreadRemoveSubjects(Thread, x, 1);
}
utInlineC void saThreadSwapSubject(saThread Thread, uint32 from, uint32 to) {
    utAssert(from <= saThreadGetNumSubject(Thread));
    utAssert(to <= saThreadGetNumSubject(Thread));
    char tmp = saThreadGetiSubject(Thread, from);
    saThreadSetiSubject(Thread, from, saThreadGetiSubject(Thread, to));
    saThreadSetiSubject(Thread, to, tmp);
}
utInlineC void saThreadSwapSubjects(saThread Thread, uint32 from, uint32 to, uint32 count) {
    utAssert((from+count) < saThreadGetNumSubject(Thread));
    utAssert((to+count) < saThreadGetNumSubject(Thread));
    char tmp[count];
    memcpy(tmp, saThreadGetSubjects(Thread)+from, count*sizeof(char));
    memcpy(saThreadGetSubjects(Thread)+from, saThreadGetSubjects(Thread)+to, count*sizeof(char));
    memcpy(saThreadGetSubjects(Thread)+to, tmp, count*sizeof(char));
}
#define saForeachThreadSubject(pVar, cVar) { \
    uint32 _xSubject; \
    for(_xSubject = 0; _xSubject < saThreadGetNumSubject(pVar); _xSubject++) { \
        cVar = saThreadGetiSubject(pVar, _xSubject);
#define saEndThreadSubject }}
utInlineC uint32 saThreadGetID(saThread Thread) {return saThreads.ID[saThread2ValidIndex(Thread)];}
utInlineC void saThreadSetID(saThread Thread, uint32 value) {saThreads.ID[saThread2ValidIndex(Thread)] = value;}
utInlineC uint32 saThreadGetNumPosts(saThread Thread) {return saThreads.NumPosts[saThread2ValidIndex(Thread)];}
utInlineC void saThreadSetNumPosts(saThread Thread, uint32 value) {saThreads.NumPosts[saThread2ValidIndex(Thread)] = value;}
utInlineC saRoot saThreadGetRoot(saThread Thread) {return saThreads.Root[saThread2ValidIndex(Thread)];}
utInlineC void saThreadSetRoot(saThread Thread, saRoot value) {saThreads.Root[saThread2ValidIndex(Thread)] = value;}
utInlineC saThread saThreadGetNextRootThread(saThread Thread) {return saThreads.NextRootThread[saThread2ValidIndex(Thread)];}
utInlineC void saThreadSetNextRootThread(saThread Thread, saThread value) {saThreads.NextRootThread[saThread2ValidIndex(Thread)] = value;}
utInlineC saThread saThreadGetPrevRootThread(saThread Thread) {return saThreads.PrevRootThread[saThread2ValidIndex(Thread)];}
utInlineC void saThreadSetPrevRootThread(saThread Thread, saThread value) {saThreads.PrevRootThread[saThread2ValidIndex(Thread)] = value;}
utInlineC saThread saThreadGetNextTableRootThread(saThread Thread) {return saThreads.NextTableRootThread[saThread2ValidIndex(Thread)];}
utInlineC void saThreadSetNextTableRootThread(saThread Thread, saThread value) {saThreads.NextTableRootThread[saThread2ValidIndex(Thread)] = value;}
utInlineC saPost saThreadGetFirstPost(saThread Thread) {return saThreads.FirstPost[saThread2ValidIndex(Thread)];}
utInlineC void saThreadSetFirstPost(saThread Thread, saPost value) {saThreads.FirstPost[saThread2ValidIndex(Thread)] = value;}
utInlineC saPost saThreadGetLastPost(saThread Thread) {return saThreads.LastPost[saThread2ValidIndex(Thread)];}
utInlineC void saThreadSetLastPost(saThread Thread, saPost value) {saThreads.LastPost[saThread2ValidIndex(Thread)] = value;}
utInlineC saSubscription saThreadGetFirstSubscription(saThread Thread) {return saThreads.FirstSubscription[saThread2ValidIndex(Thread)];}
utInlineC void saThreadSetFirstSubscription(saThread Thread, saSubscription value) {saThreads.FirstSubscription[saThread2ValidIndex(Thread)] = value;}
utInlineC saSubscription saThreadGetLastSubscription(saThread Thread) {return saThreads.LastSubscription[saThread2ValidIndex(Thread)];}
utInlineC void saThreadSetLastSubscription(saThread Thread, saSubscription value) {saThreads.LastSubscription[saThread2ValidIndex(Thread)] = value;}
utInlineC void saThreadSetConstructorCallback(void(*func)(saThread)) {saThreadConstructorCallback = func;}
utInlineC saThreadCallbackType saThreadGetConstructorCallback(void) {return saThreadConstructorCallback;}
utInlineC void saThreadSetDestructorCallback(void(*func)(saThread)) {saThreadDestructorCallback = func;}
utInlineC saThreadCallbackType saThreadGetDestructorCallback(void) {return saThreadDestructorCallback;}
utInlineC saThread saThreadNextFree(saThread Thread) {return ((saThread *)(void *)(saThreads.Root))[saThread2ValidIndex(Thread)];}
utInlineC void saThreadSetNextFree(saThread Thread, saThread value) {
    ((saThread *)(void *)(saThreads.Root))[saThread2ValidIndex(Thread)] = value;}
utInlineC void saThreadFree(saThread Thread) {
    saThreadFreeSubjects(Thread);
    saThreadSetNextFree(Thread, saRootData.firstFreeThread);
    saSetFirstFreeThread(Thread);}
void saThreadDestroy(saThread Thread);
utInlineC saThread saThreadAllocRaw(void) {
    saThread Thread;
    if(saRootData.firstFreeThread != saThreadNull) {
        Thread = saRootData.firstFreeThread;
        saSetFirstFreeThread(saThreadNextFree(Thread));
    } else {
        if(saRootData.usedThread == saRootData.allocatedThread) {
            saThreadAllocMore();
        }
        Thread = saIndex2Thread(saRootData.usedThread);
        saSetUsedThread(saUsedThread() + 1);
    }
    return Thread;}
utInlineC saThread saThreadAlloc(void) {
    saThread Thread = saThreadAllocRaw();
    saThreadSetSubjectIndex_(Thread, 0);
    saThreadSetNumSubject(Thread, 0);
    saThreadSetNumSubject(Thread, 0);
    saThreadSetID(Thread, 0);
    saThreadSetNumPosts(Thread, 0);
    saThreadSetRoot(Thread, saRootNull);
    saThreadSetNextRootThread(Thread, saThreadNull);
    saThreadSetPrevRootThread(Thread, saThreadNull);
    saThreadSetNextTableRootThread(Thread, saThreadNull);
    saThreadSetFirstPost(Thread, saPostNull);
    saThreadSetLastPost(Thread, saPostNull);
    saThreadSetFirstSubscription(Thread, saSubscriptionNull);
    saThreadSetLastSubscription(Thread, saSubscriptionNull);
    if(saThreadConstructorCallback != NULL) {
        saThreadConstructorCallback(Thread);
    }
    return Thread;}

/*----------------------------------------------------------------------------------------
  Fields for class Post.
----------------------------------------------------------------------------------------*/
struct saPostFields {
    uint64 *Date;
    uint32 *MessageIndex_;
    uint32 *NumMessage;
    char *Message;
    uint32 *ID;
    saRoot *Root;
    saPost *NextRootPost;
    saPost *PrevRootPost;
    saPost *NextTableRootPost;
    saUser *User;
    saPost *NextUserPost;
    saPost *PrevUserPost;
    saPost *NextTableUserPost;
    saThread *Thread;
    saPost *NextThreadPost;
    saPost *PrevThreadPost;
};
extern struct saPostFields saPosts;

void saPostAllocMore(void);
void saPostCopyProps(saPost saOldPost, saPost saNewPost);
void saPostAllocMessages(saPost Post, uint32 numMessages);
void saPostResizeMessages(saPost Post, uint32 numMessages);
void saPostFreeMessages(saPost Post);
void saCompactPostMessages(void);
utInlineC uint64 saPostGetDate(saPost Post) {return saPosts.Date[saPost2ValidIndex(Post)];}
utInlineC void saPostSetDate(saPost Post, uint64 value) {saPosts.Date[saPost2ValidIndex(Post)] = value;}
utInlineC uint32 saPostGetMessageIndex_(saPost Post) {return saPosts.MessageIndex_[saPost2ValidIndex(Post)];}
utInlineC void saPostSetMessageIndex_(saPost Post, uint32 value) {saPosts.MessageIndex_[saPost2ValidIndex(Post)] = value;}
utInlineC uint32 saPostGetNumMessage(saPost Post) {return saPosts.NumMessage[saPost2ValidIndex(Post)];}
utInlineC void saPostSetNumMessage(saPost Post, uint32 value) {saPosts.NumMessage[saPost2ValidIndex(Post)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saPostCheckMessageIndex(saPost Post, uint32 x) {utAssert(x < saPostGetNumMessage(Post)); return x;}
#else
utInlineC uint32 saPostCheckMessageIndex(saPost Post, uint32 x) {return x;}
#endif
utInlineC char saPostGetiMessage(saPost Post, uint32 x) {return saPosts.Message[
    saPostGetMessageIndex_(Post) + saPostCheckMessageIndex(Post, x)];}
utInlineC char *saPostGetMessage(saPost Post) {return saPosts.Message + saPostGetMessageIndex_(Post);}
#define saPostGetMessages saPostGetMessage
utInlineC void saPostSetMessage(saPost Post, char *valuePtr, uint32 numMessage) {
    saPostResizeMessages(Post, numMessage);
    memcpy(saPostGetMessages(Post), valuePtr, numMessage*sizeof(char));}
utInlineC void saPostSetiMessage(saPost Post, uint32 x, char value) {
    saPosts.Message[saPostGetMessageIndex_(Post) + saPostCheckMessageIndex(Post, (x))] = value;}
utInlineC void saPostMoveMessages(saPost Post, uint32 from, uint32 to, uint32 count) {
    utAssert((to+count) <= saPostGetNumMessage(Post));
    utAssert((from+count) <= saPostGetNumMessage(Post));
    memmove(saPostGetMessages(Post)+to,saPostGetMessages(Post)+from,((int32)count)*sizeof(char));
}
utInlineC void saPostCopyMessages(saPost Post, uint32 x, char * values, uint32 count) {
    utAssert((x+count) <= saPostGetNumMessage(Post));
    memcpy(saPostGetMessages(Post)+x, values, count*sizeof(char));
}
utInlineC void saPostAppendMessages(saPost Post, char * values, uint32 count) {
    uint32 num = saPostGetNumMessage(Post);
    saPostResizeMessages(Post, num+count);
    saPostCopyMessages(Post, num, values, count);
}
utInlineC void saPostAppendMessage(saPost Post, char Message) {
    saPostResizeMessages(Post, saPostGetNumMessage(Post)+1);
    saPostSetiMessage(Post, saPostGetNumMessage(Post)-1, Message);
}
utInlineC void saPostInsertMessages(saPost Post, uint32 x, char *Message, uint32 count) {
    utAssert(x <= saPostGetNumMessage(Post));
    if(x < saPostGetNumMessage(Post)) {
        saPostResizeMessages(Post, saPostGetNumMessage(Post)+count);
        saPostMoveMessages(Post, x, x+count, saPostGetNumMessage(Post)-x-count);
        saPostCopyMessages(Post, x, Message, count);
    }
    else {
        saPostAppendMessages(Post, Message, count);
    }
}
utInlineC void saPostInsertMessage(saPost Post, uint32 x, char Message) {
    saPostInsertMessages(Post, x, &Message, 1);
}
utInlineC void saPostRemoveMessages(saPost Post, uint32 x, uint32 count) {
    utAssert((x+count) <= saPostGetNumMessage(Post));
    if((x+count) < saPostGetNumMessage(Post)) {
        saPostMoveMessages(Post, x+count,x,saPostGetNumMessage(Post)-x-count);
    }
    saPostResizeMessages(Post, saPostGetNumMessage(Post)-(int32)count);
}
utInlineC void saPostRemoveMessage(saPost Post, uint32 x) {
    saPostRemoveMessages(Post, x, 1);
}
utInlineC void saPostSwapMessage(saPost Post, uint32 from, uint32 to) {
    utAssert(from <= saPostGetNumMessage(Post));
    utAssert(to <= saPostGetNumMessage(Post));
    char tmp = saPostGetiMessage(Post, from);
    saPostSetiMessage(Post, from, saPostGetiMessage(Post, to));
    saPostSetiMessage(Post, to, tmp);
}
utInlineC void saPostSwapMessages(saPost Post, uint32 from, uint32 to, uint32 count) {
    utAssert((from+count) < saPostGetNumMessage(Post));
    utAssert((to+count) < saPostGetNumMessage(Post));
    char tmp[count];
    memcpy(tmp, saPostGetMessages(Post)+from, count*sizeof(char));
    memcpy(saPostGetMessages(Post)+from, saPostGetMessages(Post)+to, count*sizeof(char));
    memcpy(saPostGetMessages(Post)+to, tmp, count*sizeof(char));
}
#define saForeachPostMessage(pVar, cVar) { \
    uint32 _xMessage; \
    for(_xMessage = 0; _xMessage < saPostGetNumMessage(pVar); _xMessage++) { \
        cVar = saPostGetiMessage(pVar, _xMessage);
#define saEndPostMessage }}
utInlineC uint32 saPostGetID(saPost Post) {return saPosts.ID[saPost2ValidIndex(Post)];}
utInlineC void saPostSetID(saPost Post, uint32 value) {saPosts.ID[saPost2ValidIndex(Post)] = value;}
utInlineC saRoot saPostGetRoot(saPost Post) {return saPosts.Root[saPost2ValidIndex(Post)];}
utInlineC void saPostSetRoot(saPost Post, saRoot value) {saPosts.Root[saPost2ValidIndex(Post)] = value;}
utInlineC saPost saPostGetNextRootPost(saPost Post) {return saPosts.NextRootPost[saPost2ValidIndex(Post)];}
utInlineC void saPostSetNextRootPost(saPost Post, saPost value) {saPosts.NextRootPost[saPost2ValidIndex(Post)] = value;}
utInlineC saPost saPostGetPrevRootPost(saPost Post) {return saPosts.PrevRootPost[saPost2ValidIndex(Post)];}
utInlineC void saPostSetPrevRootPost(saPost Post, saPost value) {saPosts.PrevRootPost[saPost2ValidIndex(Post)] = value;}
utInlineC saPost saPostGetNextTableRootPost(saPost Post) {return saPosts.NextTableRootPost[saPost2ValidIndex(Post)];}
utInlineC void saPostSetNextTableRootPost(saPost Post, saPost value) {saPosts.NextTableRootPost[saPost2ValidIndex(Post)] = value;}
utInlineC saUser saPostGetUser(saPost Post) {return saPosts.User[saPost2ValidIndex(Post)];}
utInlineC void saPostSetUser(saPost Post, saUser value) {saPosts.User[saPost2ValidIndex(Post)] = value;}
utInlineC saPost saPostGetNextUserPost(saPost Post) {return saPosts.NextUserPost[saPost2ValidIndex(Post)];}
utInlineC void saPostSetNextUserPost(saPost Post, saPost value) {saPosts.NextUserPost[saPost2ValidIndex(Post)] = value;}
utInlineC saPost saPostGetPrevUserPost(saPost Post) {return saPosts.PrevUserPost[saPost2ValidIndex(Post)];}
utInlineC void saPostSetPrevUserPost(saPost Post, saPost value) {saPosts.PrevUserPost[saPost2ValidIndex(Post)] = value;}
utInlineC saPost saPostGetNextTableUserPost(saPost Post) {return saPosts.NextTableUserPost[saPost2ValidIndex(Post)];}
utInlineC void saPostSetNextTableUserPost(saPost Post, saPost value) {saPosts.NextTableUserPost[saPost2ValidIndex(Post)] = value;}
utInlineC saThread saPostGetThread(saPost Post) {return saPosts.Thread[saPost2ValidIndex(Post)];}
utInlineC void saPostSetThread(saPost Post, saThread value) {saPosts.Thread[saPost2ValidIndex(Post)] = value;}
utInlineC saPost saPostGetNextThreadPost(saPost Post) {return saPosts.NextThreadPost[saPost2ValidIndex(Post)];}
utInlineC void saPostSetNextThreadPost(saPost Post, saPost value) {saPosts.NextThreadPost[saPost2ValidIndex(Post)] = value;}
utInlineC saPost saPostGetPrevThreadPost(saPost Post) {return saPosts.PrevThreadPost[saPost2ValidIndex(Post)];}
utInlineC void saPostSetPrevThreadPost(saPost Post, saPost value) {saPosts.PrevThreadPost[saPost2ValidIndex(Post)] = value;}
utInlineC void saPostSetConstructorCallback(void(*func)(saPost)) {saPostConstructorCallback = func;}
utInlineC saPostCallbackType saPostGetConstructorCallback(void) {return saPostConstructorCallback;}
utInlineC void saPostSetDestructorCallback(void(*func)(saPost)) {saPostDestructorCallback = func;}
utInlineC saPostCallbackType saPostGetDestructorCallback(void) {return saPostDestructorCallback;}
utInlineC saPost saPostNextFree(saPost Post) {return ((saPost *)(void *)(saPosts.Root))[saPost2ValidIndex(Post)];}
utInlineC void saPostSetNextFree(saPost Post, saPost value) {
    ((saPost *)(void *)(saPosts.Root))[saPost2ValidIndex(Post)] = value;}
utInlineC void saPostFree(saPost Post) {
    saPostFreeMessages(Post);
    saPostSetNextFree(Post, saRootData.firstFreePost);
    saSetFirstFreePost(Post);}
void saPostDestroy(saPost Post);
utInlineC saPost saPostAllocRaw(void) {
    saPost Post;
    if(saRootData.firstFreePost != saPostNull) {
        Post = saRootData.firstFreePost;
        saSetFirstFreePost(saPostNextFree(Post));
    } else {
        if(saRootData.usedPost == saRootData.allocatedPost) {
            saPostAllocMore();
        }
        Post = saIndex2Post(saRootData.usedPost);
        saSetUsedPost(saUsedPost() + 1);
    }
    return Post;}
utInlineC saPost saPostAlloc(void) {
    saPost Post = saPostAllocRaw();
    saPostSetDate(Post, 0);
    saPostSetMessageIndex_(Post, 0);
    saPostSetNumMessage(Post, 0);
    saPostSetNumMessage(Post, 0);
    saPostSetID(Post, 0);
    saPostSetRoot(Post, saRootNull);
    saPostSetNextRootPost(Post, saPostNull);
    saPostSetPrevRootPost(Post, saPostNull);
    saPostSetNextTableRootPost(Post, saPostNull);
    saPostSetUser(Post, saUserNull);
    saPostSetNextUserPost(Post, saPostNull);
    saPostSetPrevUserPost(Post, saPostNull);
    saPostSetNextTableUserPost(Post, saPostNull);
    saPostSetThread(Post, saThreadNull);
    saPostSetNextThreadPost(Post, saPostNull);
    saPostSetPrevThreadPost(Post, saPostNull);
    if(saPostConstructorCallback != NULL) {
        saPostConstructorCallback(Post);
    }
    return Post;}

/*----------------------------------------------------------------------------------------
  Fields for class Subscription.
----------------------------------------------------------------------------------------*/
struct saSubscriptionFields {
    saUser *User;
    saSubscription *NextUserSubscription;
    saSubscription *PrevUserSubscription;
    saThread *Thread;
    saSubscription *NextThreadSubscription;
    saSubscription *PrevThreadSubscription;
};
extern struct saSubscriptionFields saSubscriptions;

void saSubscriptionAllocMore(void);
void saSubscriptionCopyProps(saSubscription saOldSubscription, saSubscription saNewSubscription);
utInlineC saUser saSubscriptionGetUser(saSubscription Subscription) {return saSubscriptions.User[saSubscription2ValidIndex(Subscription)];}
utInlineC void saSubscriptionSetUser(saSubscription Subscription, saUser value) {saSubscriptions.User[saSubscription2ValidIndex(Subscription)] = value;}
utInlineC saSubscription saSubscriptionGetNextUserSubscription(saSubscription Subscription) {return saSubscriptions.NextUserSubscription[saSubscription2ValidIndex(Subscription)];}
utInlineC void saSubscriptionSetNextUserSubscription(saSubscription Subscription, saSubscription value) {saSubscriptions.NextUserSubscription[saSubscription2ValidIndex(Subscription)] = value;}
utInlineC saSubscription saSubscriptionGetPrevUserSubscription(saSubscription Subscription) {return saSubscriptions.PrevUserSubscription[saSubscription2ValidIndex(Subscription)];}
utInlineC void saSubscriptionSetPrevUserSubscription(saSubscription Subscription, saSubscription value) {saSubscriptions.PrevUserSubscription[saSubscription2ValidIndex(Subscription)] = value;}
utInlineC saThread saSubscriptionGetThread(saSubscription Subscription) {return saSubscriptions.Thread[saSubscription2ValidIndex(Subscription)];}
utInlineC void saSubscriptionSetThread(saSubscription Subscription, saThread value) {saSubscriptions.Thread[saSubscription2ValidIndex(Subscription)] = value;}
utInlineC saSubscription saSubscriptionGetNextThreadSubscription(saSubscription Subscription) {return saSubscriptions.NextThreadSubscription[saSubscription2ValidIndex(Subscription)];}
utInlineC void saSubscriptionSetNextThreadSubscription(saSubscription Subscription, saSubscription value) {saSubscriptions.NextThreadSubscription[saSubscription2ValidIndex(Subscription)] = value;}
utInlineC saSubscription saSubscriptionGetPrevThreadSubscription(saSubscription Subscription) {return saSubscriptions.PrevThreadSubscription[saSubscription2ValidIndex(Subscription)];}
utInlineC void saSubscriptionSetPrevThreadSubscription(saSubscription Subscription, saSubscription value) {saSubscriptions.PrevThreadSubscription[saSubscription2ValidIndex(Subscription)] = value;}
utInlineC void saSubscriptionSetConstructorCallback(void(*func)(saSubscription)) {saSubscriptionConstructorCallback = func;}
utInlineC saSubscriptionCallbackType saSubscriptionGetConstructorCallback(void) {return saSubscriptionConstructorCallback;}
utInlineC void saSubscriptionSetDestructorCallback(void(*func)(saSubscription)) {saSubscriptionDestructorCallback = func;}
utInlineC saSubscriptionCallbackType saSubscriptionGetDestructorCallback(void) {return saSubscriptionDestructorCallback;}
utInlineC saSubscription saSubscriptionNextFree(saSubscription Subscription) {return ((saSubscription *)(void *)(saSubscriptions.User))[saSubscription2ValidIndex(Subscription)];}
utInlineC void saSubscriptionSetNextFree(saSubscription Subscription, saSubscription value) {
    ((saSubscription *)(void *)(saSubscriptions.User))[saSubscription2ValidIndex(Subscription)] = value;}
utInlineC void saSubscriptionFree(saSubscription Subscription) {
    saSubscriptionSetNextFree(Subscription, saRootData.firstFreeSubscription);
    saSetFirstFreeSubscription(Subscription);}
void saSubscriptionDestroy(saSubscription Subscription);
utInlineC saSubscription saSubscriptionAllocRaw(void) {
    saSubscription Subscription;
    if(saRootData.firstFreeSubscription != saSubscriptionNull) {
        Subscription = saRootData.firstFreeSubscription;
        saSetFirstFreeSubscription(saSubscriptionNextFree(Subscription));
    } else {
        if(saRootData.usedSubscription == saRootData.allocatedSubscription) {
            saSubscriptionAllocMore();
        }
        Subscription = saIndex2Subscription(saRootData.usedSubscription);
        saSetUsedSubscription(saUsedSubscription() + 1);
    }
    return Subscription;}
utInlineC saSubscription saSubscriptionAlloc(void) {
    saSubscription Subscription = saSubscriptionAllocRaw();
    saSubscriptionSetUser(Subscription, saUserNull);
    saSubscriptionSetNextUserSubscription(Subscription, saSubscriptionNull);
    saSubscriptionSetPrevUserSubscription(Subscription, saSubscriptionNull);
    saSubscriptionSetThread(Subscription, saThreadNull);
    saSubscriptionSetNextThreadSubscription(Subscription, saSubscriptionNull);
    saSubscriptionSetPrevThreadSubscription(Subscription, saSubscriptionNull);
    if(saSubscriptionConstructorCallback != NULL) {
        saSubscriptionConstructorCallback(Subscription);
    }
    return Subscription;}

/*----------------------------------------------------------------------------------------
  Fields for class Announcement.
----------------------------------------------------------------------------------------*/
struct saAnnouncementFields {
    uint64 *Date;
    uint32 *SubjectIndex_;
    uint32 *NumSubject;
    char *Subject;
    uint32 *MessageIndex_;
    uint32 *NumMessage;
    char *Message;
    uint32 *ID;
    saRoot *Root;
    saAnnouncement *NextRootAnnouncement;
    saAnnouncement *PrevRootAnnouncement;
    saAnnouncement *NextTableRootAnnouncement;
    saUser *User;
    saAnnouncement *NextUserAnnouncement;
    saAnnouncement *PrevUserAnnouncement;
};
extern struct saAnnouncementFields saAnnouncements;

void saAnnouncementAllocMore(void);
void saAnnouncementCopyProps(saAnnouncement saOldAnnouncement, saAnnouncement saNewAnnouncement);
void saAnnouncementAllocSubjects(saAnnouncement Announcement, uint32 numSubjects);
void saAnnouncementResizeSubjects(saAnnouncement Announcement, uint32 numSubjects);
void saAnnouncementFreeSubjects(saAnnouncement Announcement);
void saCompactAnnouncementSubjects(void);
void saAnnouncementAllocMessages(saAnnouncement Announcement, uint32 numMessages);
void saAnnouncementResizeMessages(saAnnouncement Announcement, uint32 numMessages);
void saAnnouncementFreeMessages(saAnnouncement Announcement);
void saCompactAnnouncementMessages(void);
utInlineC uint64 saAnnouncementGetDate(saAnnouncement Announcement) {return saAnnouncements.Date[saAnnouncement2ValidIndex(Announcement)];}
utInlineC void saAnnouncementSetDate(saAnnouncement Announcement, uint64 value) {saAnnouncements.Date[saAnnouncement2ValidIndex(Announcement)] = value;}
utInlineC uint32 saAnnouncementGetSubjectIndex_(saAnnouncement Announcement) {return saAnnouncements.SubjectIndex_[saAnnouncement2ValidIndex(Announcement)];}
utInlineC void saAnnouncementSetSubjectIndex_(saAnnouncement Announcement, uint32 value) {saAnnouncements.SubjectIndex_[saAnnouncement2ValidIndex(Announcement)] = value;}
utInlineC uint32 saAnnouncementGetNumSubject(saAnnouncement Announcement) {return saAnnouncements.NumSubject[saAnnouncement2ValidIndex(Announcement)];}
utInlineC void saAnnouncementSetNumSubject(saAnnouncement Announcement, uint32 value) {saAnnouncements.NumSubject[saAnnouncement2ValidIndex(Announcement)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saAnnouncementCheckSubjectIndex(saAnnouncement Announcement, uint32 x) {utAssert(x < saAnnouncementGetNumSubject(Announcement)); return x;}
#else
utInlineC uint32 saAnnouncementCheckSubjectIndex(saAnnouncement Announcement, uint32 x) {return x;}
#endif
utInlineC char saAnnouncementGetiSubject(saAnnouncement Announcement, uint32 x) {return saAnnouncements.Subject[
    saAnnouncementGetSubjectIndex_(Announcement) + saAnnouncementCheckSubjectIndex(Announcement, x)];}
utInlineC char *saAnnouncementGetSubject(saAnnouncement Announcement) {return saAnnouncements.Subject + saAnnouncementGetSubjectIndex_(Announcement);}
#define saAnnouncementGetSubjects saAnnouncementGetSubject
utInlineC void saAnnouncementSetSubject(saAnnouncement Announcement, char *valuePtr, uint32 numSubject) {
    saAnnouncementResizeSubjects(Announcement, numSubject);
    memcpy(saAnnouncementGetSubjects(Announcement), valuePtr, numSubject*sizeof(char));}
utInlineC void saAnnouncementSetiSubject(saAnnouncement Announcement, uint32 x, char value) {
    saAnnouncements.Subject[saAnnouncementGetSubjectIndex_(Announcement) + saAnnouncementCheckSubjectIndex(Announcement, (x))] = value;}
utInlineC void saAnnouncementMoveSubjects(saAnnouncement Announcement, uint32 from, uint32 to, uint32 count) {
    utAssert((to+count) <= saAnnouncementGetNumSubject(Announcement));
    utAssert((from+count) <= saAnnouncementGetNumSubject(Announcement));
    memmove(saAnnouncementGetSubjects(Announcement)+to,saAnnouncementGetSubjects(Announcement)+from,((int32)count)*sizeof(char));
}
utInlineC void saAnnouncementCopySubjects(saAnnouncement Announcement, uint32 x, char * values, uint32 count) {
    utAssert((x+count) <= saAnnouncementGetNumSubject(Announcement));
    memcpy(saAnnouncementGetSubjects(Announcement)+x, values, count*sizeof(char));
}
utInlineC void saAnnouncementAppendSubjects(saAnnouncement Announcement, char * values, uint32 count) {
    uint32 num = saAnnouncementGetNumSubject(Announcement);
    saAnnouncementResizeSubjects(Announcement, num+count);
    saAnnouncementCopySubjects(Announcement, num, values, count);
}
utInlineC void saAnnouncementAppendSubject(saAnnouncement Announcement, char Subject) {
    saAnnouncementResizeSubjects(Announcement, saAnnouncementGetNumSubject(Announcement)+1);
    saAnnouncementSetiSubject(Announcement, saAnnouncementGetNumSubject(Announcement)-1, Subject);
}
utInlineC void saAnnouncementInsertSubjects(saAnnouncement Announcement, uint32 x, char *Subject, uint32 count) {
    utAssert(x <= saAnnouncementGetNumSubject(Announcement));
    if(x < saAnnouncementGetNumSubject(Announcement)) {
        saAnnouncementResizeSubjects(Announcement, saAnnouncementGetNumSubject(Announcement)+count);
        saAnnouncementMoveSubjects(Announcement, x, x+count, saAnnouncementGetNumSubject(Announcement)-x-count);
        saAnnouncementCopySubjects(Announcement, x, Subject, count);
    }
    else {
        saAnnouncementAppendSubjects(Announcement, Subject, count);
    }
}
utInlineC void saAnnouncementInsertSubject(saAnnouncement Announcement, uint32 x, char Subject) {
    saAnnouncementInsertSubjects(Announcement, x, &Subject, 1);
}
utInlineC void saAnnouncementRemoveSubjects(saAnnouncement Announcement, uint32 x, uint32 count) {
    utAssert((x+count) <= saAnnouncementGetNumSubject(Announcement));
    if((x+count) < saAnnouncementGetNumSubject(Announcement)) {
        saAnnouncementMoveSubjects(Announcement, x+count,x,saAnnouncementGetNumSubject(Announcement)-x-count);
    }
    saAnnouncementResizeSubjects(Announcement, saAnnouncementGetNumSubject(Announcement)-(int32)count);
}
utInlineC void saAnnouncementRemoveSubject(saAnnouncement Announcement, uint32 x) {
    saAnnouncementRemoveSubjects(Announcement, x, 1);
}
utInlineC void saAnnouncementSwapSubject(saAnnouncement Announcement, uint32 from, uint32 to) {
    utAssert(from <= saAnnouncementGetNumSubject(Announcement));
    utAssert(to <= saAnnouncementGetNumSubject(Announcement));
    char tmp = saAnnouncementGetiSubject(Announcement, from);
    saAnnouncementSetiSubject(Announcement, from, saAnnouncementGetiSubject(Announcement, to));
    saAnnouncementSetiSubject(Announcement, to, tmp);
}
utInlineC void saAnnouncementSwapSubjects(saAnnouncement Announcement, uint32 from, uint32 to, uint32 count) {
    utAssert((from+count) < saAnnouncementGetNumSubject(Announcement));
    utAssert((to+count) < saAnnouncementGetNumSubject(Announcement));
    char tmp[count];
    memcpy(tmp, saAnnouncementGetSubjects(Announcement)+from, count*sizeof(char));
    memcpy(saAnnouncementGetSubjects(Announcement)+from, saAnnouncementGetSubjects(Announcement)+to, count*sizeof(char));
    memcpy(saAnnouncementGetSubjects(Announcement)+to, tmp, count*sizeof(char));
}
#define saForeachAnnouncementSubject(pVar, cVar) { \
    uint32 _xSubject; \
    for(_xSubject = 0; _xSubject < saAnnouncementGetNumSubject(pVar); _xSubject++) { \
        cVar = saAnnouncementGetiSubject(pVar, _xSubject);
#define saEndAnnouncementSubject }}
utInlineC uint32 saAnnouncementGetMessageIndex_(saAnnouncement Announcement) {return saAnnouncements.MessageIndex_[saAnnouncement2ValidIndex(Announcement)];}
utInlineC void saAnnouncementSetMessageIndex_(saAnnouncement Announcement, uint32 value) {saAnnouncements.MessageIndex_[saAnnouncement2ValidIndex(Announcement)] = value;}
utInlineC uint32 saAnnouncementGetNumMessage(saAnnouncement Announcement) {return saAnnouncements.NumMessage[saAnnouncement2ValidIndex(Announcement)];}
utInlineC void saAnnouncementSetNumMessage(saAnnouncement Announcement, uint32 value) {saAnnouncements.NumMessage[saAnnouncement2ValidIndex(Announcement)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saAnnouncementCheckMessageIndex(saAnnouncement Announcement, uint32 x) {utAssert(x < saAnnouncementGetNumMessage(Announcement)); return x;}
#else
utInlineC uint32 saAnnouncementCheckMessageIndex(saAnnouncement Announcement, uint32 x) {return x;}
#endif
utInlineC char saAnnouncementGetiMessage(saAnnouncement Announcement, uint32 x) {return saAnnouncements.Message[
    saAnnouncementGetMessageIndex_(Announcement) + saAnnouncementCheckMessageIndex(Announcement, x)];}
utInlineC char *saAnnouncementGetMessage(saAnnouncement Announcement) {return saAnnouncements.Message + saAnnouncementGetMessageIndex_(Announcement);}
#define saAnnouncementGetMessages saAnnouncementGetMessage
utInlineC void saAnnouncementSetMessage(saAnnouncement Announcement, char *valuePtr, uint32 numMessage) {
    saAnnouncementResizeMessages(Announcement, numMessage);
    memcpy(saAnnouncementGetMessages(Announcement), valuePtr, numMessage*sizeof(char));}
utInlineC void saAnnouncementSetiMessage(saAnnouncement Announcement, uint32 x, char value) {
    saAnnouncements.Message[saAnnouncementGetMessageIndex_(Announcement) + saAnnouncementCheckMessageIndex(Announcement, (x))] = value;}
utInlineC void saAnnouncementMoveMessages(saAnnouncement Announcement, uint32 from, uint32 to, uint32 count) {
    utAssert((to+count) <= saAnnouncementGetNumMessage(Announcement));
    utAssert((from+count) <= saAnnouncementGetNumMessage(Announcement));
    memmove(saAnnouncementGetMessages(Announcement)+to,saAnnouncementGetMessages(Announcement)+from,((int32)count)*sizeof(char));
}
utInlineC void saAnnouncementCopyMessages(saAnnouncement Announcement, uint32 x, char * values, uint32 count) {
    utAssert((x+count) <= saAnnouncementGetNumMessage(Announcement));
    memcpy(saAnnouncementGetMessages(Announcement)+x, values, count*sizeof(char));
}
utInlineC void saAnnouncementAppendMessages(saAnnouncement Announcement, char * values, uint32 count) {
    uint32 num = saAnnouncementGetNumMessage(Announcement);
    saAnnouncementResizeMessages(Announcement, num+count);
    saAnnouncementCopyMessages(Announcement, num, values, count);
}
utInlineC void saAnnouncementAppendMessage(saAnnouncement Announcement, char Message) {
    saAnnouncementResizeMessages(Announcement, saAnnouncementGetNumMessage(Announcement)+1);
    saAnnouncementSetiMessage(Announcement, saAnnouncementGetNumMessage(Announcement)-1, Message);
}
utInlineC void saAnnouncementInsertMessages(saAnnouncement Announcement, uint32 x, char *Message, uint32 count) {
    utAssert(x <= saAnnouncementGetNumMessage(Announcement));
    if(x < saAnnouncementGetNumMessage(Announcement)) {
        saAnnouncementResizeMessages(Announcement, saAnnouncementGetNumMessage(Announcement)+count);
        saAnnouncementMoveMessages(Announcement, x, x+count, saAnnouncementGetNumMessage(Announcement)-x-count);
        saAnnouncementCopyMessages(Announcement, x, Message, count);
    }
    else {
        saAnnouncementAppendMessages(Announcement, Message, count);
    }
}
utInlineC void saAnnouncementInsertMessage(saAnnouncement Announcement, uint32 x, char Message) {
    saAnnouncementInsertMessages(Announcement, x, &Message, 1);
}
utInlineC void saAnnouncementRemoveMessages(saAnnouncement Announcement, uint32 x, uint32 count) {
    utAssert((x+count) <= saAnnouncementGetNumMessage(Announcement));
    if((x+count) < saAnnouncementGetNumMessage(Announcement)) {
        saAnnouncementMoveMessages(Announcement, x+count,x,saAnnouncementGetNumMessage(Announcement)-x-count);
    }
    saAnnouncementResizeMessages(Announcement, saAnnouncementGetNumMessage(Announcement)-(int32)count);
}
utInlineC void saAnnouncementRemoveMessage(saAnnouncement Announcement, uint32 x) {
    saAnnouncementRemoveMessages(Announcement, x, 1);
}
utInlineC void saAnnouncementSwapMessage(saAnnouncement Announcement, uint32 from, uint32 to) {
    utAssert(from <= saAnnouncementGetNumMessage(Announcement));
    utAssert(to <= saAnnouncementGetNumMessage(Announcement));
    char tmp = saAnnouncementGetiMessage(Announcement, from);
    saAnnouncementSetiMessage(Announcement, from, saAnnouncementGetiMessage(Announcement, to));
    saAnnouncementSetiMessage(Announcement, to, tmp);
}
utInlineC void saAnnouncementSwapMessages(saAnnouncement Announcement, uint32 from, uint32 to, uint32 count) {
    utAssert((from+count) < saAnnouncementGetNumMessage(Announcement));
    utAssert((to+count) < saAnnouncementGetNumMessage(Announcement));
    char tmp[count];
    memcpy(tmp, saAnnouncementGetMessages(Announcement)+from, count*sizeof(char));
    memcpy(saAnnouncementGetMessages(Announcement)+from, saAnnouncementGetMessages(Announcement)+to, count*sizeof(char));
    memcpy(saAnnouncementGetMessages(Announcement)+to, tmp, count*sizeof(char));
}
#define saForeachAnnouncementMessage(pVar, cVar) { \
    uint32 _xMessage; \
    for(_xMessage = 0; _xMessage < saAnnouncementGetNumMessage(pVar); _xMessage++) { \
        cVar = saAnnouncementGetiMessage(pVar, _xMessage);
#define saEndAnnouncementMessage }}
utInlineC uint32 saAnnouncementGetID(saAnnouncement Announcement) {return saAnnouncements.ID[saAnnouncement2ValidIndex(Announcement)];}
utInlineC void saAnnouncementSetID(saAnnouncement Announcement, uint32 value) {saAnnouncements.ID[saAnnouncement2ValidIndex(Announcement)] = value;}
utInlineC saRoot saAnnouncementGetRoot(saAnnouncement Announcement) {return saAnnouncements.Root[saAnnouncement2ValidIndex(Announcement)];}
utInlineC void saAnnouncementSetRoot(saAnnouncement Announcement, saRoot value) {saAnnouncements.Root[saAnnouncement2ValidIndex(Announcement)] = value;}
utInlineC saAnnouncement saAnnouncementGetNextRootAnnouncement(saAnnouncement Announcement) {return saAnnouncements.NextRootAnnouncement[saAnnouncement2ValidIndex(Announcement)];}
utInlineC void saAnnouncementSetNextRootAnnouncement(saAnnouncement Announcement, saAnnouncement value) {saAnnouncements.NextRootAnnouncement[saAnnouncement2ValidIndex(Announcement)] = value;}
utInlineC saAnnouncement saAnnouncementGetPrevRootAnnouncement(saAnnouncement Announcement) {return saAnnouncements.PrevRootAnnouncement[saAnnouncement2ValidIndex(Announcement)];}
utInlineC void saAnnouncementSetPrevRootAnnouncement(saAnnouncement Announcement, saAnnouncement value) {saAnnouncements.PrevRootAnnouncement[saAnnouncement2ValidIndex(Announcement)] = value;}
utInlineC saAnnouncement saAnnouncementGetNextTableRootAnnouncement(saAnnouncement Announcement) {return saAnnouncements.NextTableRootAnnouncement[saAnnouncement2ValidIndex(Announcement)];}
utInlineC void saAnnouncementSetNextTableRootAnnouncement(saAnnouncement Announcement, saAnnouncement value) {saAnnouncements.NextTableRootAnnouncement[saAnnouncement2ValidIndex(Announcement)] = value;}
utInlineC saUser saAnnouncementGetUser(saAnnouncement Announcement) {return saAnnouncements.User[saAnnouncement2ValidIndex(Announcement)];}
utInlineC void saAnnouncementSetUser(saAnnouncement Announcement, saUser value) {saAnnouncements.User[saAnnouncement2ValidIndex(Announcement)] = value;}
utInlineC saAnnouncement saAnnouncementGetNextUserAnnouncement(saAnnouncement Announcement) {return saAnnouncements.NextUserAnnouncement[saAnnouncement2ValidIndex(Announcement)];}
utInlineC void saAnnouncementSetNextUserAnnouncement(saAnnouncement Announcement, saAnnouncement value) {saAnnouncements.NextUserAnnouncement[saAnnouncement2ValidIndex(Announcement)] = value;}
utInlineC saAnnouncement saAnnouncementGetPrevUserAnnouncement(saAnnouncement Announcement) {return saAnnouncements.PrevUserAnnouncement[saAnnouncement2ValidIndex(Announcement)];}
utInlineC void saAnnouncementSetPrevUserAnnouncement(saAnnouncement Announcement, saAnnouncement value) {saAnnouncements.PrevUserAnnouncement[saAnnouncement2ValidIndex(Announcement)] = value;}
utInlineC void saAnnouncementSetConstructorCallback(void(*func)(saAnnouncement)) {saAnnouncementConstructorCallback = func;}
utInlineC saAnnouncementCallbackType saAnnouncementGetConstructorCallback(void) {return saAnnouncementConstructorCallback;}
utInlineC void saAnnouncementSetDestructorCallback(void(*func)(saAnnouncement)) {saAnnouncementDestructorCallback = func;}
utInlineC saAnnouncementCallbackType saAnnouncementGetDestructorCallback(void) {return saAnnouncementDestructorCallback;}
utInlineC saAnnouncement saAnnouncementNextFree(saAnnouncement Announcement) {return ((saAnnouncement *)(void *)(saAnnouncements.Root))[saAnnouncement2ValidIndex(Announcement)];}
utInlineC void saAnnouncementSetNextFree(saAnnouncement Announcement, saAnnouncement value) {
    ((saAnnouncement *)(void *)(saAnnouncements.Root))[saAnnouncement2ValidIndex(Announcement)] = value;}
utInlineC void saAnnouncementFree(saAnnouncement Announcement) {
    saAnnouncementFreeSubjects(Announcement);
    saAnnouncementFreeMessages(Announcement);
    saAnnouncementSetNextFree(Announcement, saRootData.firstFreeAnnouncement);
    saSetFirstFreeAnnouncement(Announcement);}
void saAnnouncementDestroy(saAnnouncement Announcement);
utInlineC saAnnouncement saAnnouncementAllocRaw(void) {
    saAnnouncement Announcement;
    if(saRootData.firstFreeAnnouncement != saAnnouncementNull) {
        Announcement = saRootData.firstFreeAnnouncement;
        saSetFirstFreeAnnouncement(saAnnouncementNextFree(Announcement));
    } else {
        if(saRootData.usedAnnouncement == saRootData.allocatedAnnouncement) {
            saAnnouncementAllocMore();
        }
        Announcement = saIndex2Announcement(saRootData.usedAnnouncement);
        saSetUsedAnnouncement(saUsedAnnouncement() + 1);
    }
    return Announcement;}
utInlineC saAnnouncement saAnnouncementAlloc(void) {
    saAnnouncement Announcement = saAnnouncementAllocRaw();
    saAnnouncementSetDate(Announcement, 0);
    saAnnouncementSetSubjectIndex_(Announcement, 0);
    saAnnouncementSetNumSubject(Announcement, 0);
    saAnnouncementSetNumSubject(Announcement, 0);
    saAnnouncementSetMessageIndex_(Announcement, 0);
    saAnnouncementSetNumMessage(Announcement, 0);
    saAnnouncementSetNumMessage(Announcement, 0);
    saAnnouncementSetID(Announcement, 0);
    saAnnouncementSetRoot(Announcement, saRootNull);
    saAnnouncementSetNextRootAnnouncement(Announcement, saAnnouncementNull);
    saAnnouncementSetPrevRootAnnouncement(Announcement, saAnnouncementNull);
    saAnnouncementSetNextTableRootAnnouncement(Announcement, saAnnouncementNull);
    saAnnouncementSetUser(Announcement, saUserNull);
    saAnnouncementSetNextUserAnnouncement(Announcement, saAnnouncementNull);
    saAnnouncementSetPrevUserAnnouncement(Announcement, saAnnouncementNull);
    if(saAnnouncementConstructorCallback != NULL) {
        saAnnouncementConstructorCallback(Announcement);
    }
    return Announcement;}

/*----------------------------------------------------------------------------------------
  Fields for class TransactionArray.
----------------------------------------------------------------------------------------*/
struct saTransactionArrayFields {
    uint32 *TransactionIndex_;
    uint32 *NumTransaction;
    saTransaction *Transaction;
    uint32 *UsedTransaction;
    saTransactionArray *FreeList;
};
extern struct saTransactionArrayFields saTransactionArrays;

void saTransactionArrayAllocMore(void);
void saTransactionArrayCopyProps(saTransactionArray saOldTransactionArray, saTransactionArray saNewTransactionArray);
void saTransactionArrayAllocTransactions(saTransactionArray TransactionArray, uint32 numTransactions);
void saTransactionArrayResizeTransactions(saTransactionArray TransactionArray, uint32 numTransactions);
void saTransactionArrayFreeTransactions(saTransactionArray TransactionArray);
void saCompactTransactionArrayTransactions(void);
utInlineC uint32 saTransactionArrayGetTransactionIndex_(saTransactionArray TransactionArray) {return saTransactionArrays.TransactionIndex_[saTransactionArray2ValidIndex(TransactionArray)];}
utInlineC void saTransactionArraySetTransactionIndex_(saTransactionArray TransactionArray, uint32 value) {saTransactionArrays.TransactionIndex_[saTransactionArray2ValidIndex(TransactionArray)] = value;}
utInlineC uint32 saTransactionArrayGetNumTransaction(saTransactionArray TransactionArray) {return saTransactionArrays.NumTransaction[saTransactionArray2ValidIndex(TransactionArray)];}
utInlineC void saTransactionArraySetNumTransaction(saTransactionArray TransactionArray, uint32 value) {saTransactionArrays.NumTransaction[saTransactionArray2ValidIndex(TransactionArray)] = value;}
#if defined(DD_DEBUG)
utInlineC uint32 saTransactionArrayCheckTransactionIndex(saTransactionArray TransactionArray, uint32 x) {utAssert(x < saTransactionArrayGetNumTransaction(TransactionArray)); return x;}
#else
utInlineC uint32 saTransactionArrayCheckTransactionIndex(saTransactionArray TransactionArray, uint32 x) {return x;}
#endif
utInlineC saTransaction saTransactionArrayGetiTransaction(saTransactionArray TransactionArray, uint32 x) {return saTransactionArrays.Transaction[
    saTransactionArrayGetTransactionIndex_(TransactionArray) + saTransactionArrayCheckTransactionIndex(TransactionArray, x)];}
utInlineC saTransaction *saTransactionArrayGetTransaction(saTransactionArray TransactionArray) {return saTransactionArrays.Transaction + saTransactionArrayGetTransactionIndex_(TransactionArray);}
#define saTransactionArrayGetTransactions saTransactionArrayGetTransaction
utInlineC void saTransactionArraySetTransaction(saTransactionArray TransactionArray, saTransaction *valuePtr, uint32 numTransaction) {
    saTransactionArrayResizeTransactions(TransactionArray, numTransaction);
    memcpy(saTransactionArrayGetTransactions(TransactionArray), valuePtr, numTransaction*sizeof(saTransaction));}
utInlineC void saTransactionArraySetiTransaction(saTransactionArray TransactionArray, uint32 x, saTransaction value) {
    saTransactionArrays.Transaction[saTransactionArrayGetTransactionIndex_(TransactionArray) + saTransactionArrayCheckTransactionIndex(TransactionArray, (x))] = value;}
utInlineC uint32 saTransactionArrayGetUsedTransaction(saTransactionArray TransactionArray) {return saTransactionArrays.UsedTransaction[saTransactionArray2ValidIndex(TransactionArray)];}
utInlineC void saTransactionArraySetUsedTransaction(saTransactionArray TransactionArray, uint32 value) {saTransactionArrays.UsedTransaction[saTransactionArray2ValidIndex(TransactionArray)] = value;}
utInlineC saTransactionArray saTransactionArrayGetFreeList(saTransactionArray TransactionArray) {return saTransactionArrays.FreeList[saTransactionArray2ValidIndex(TransactionArray)];}
utInlineC void saTransactionArraySetFreeList(saTransactionArray TransactionArray, saTransactionArray value) {saTransactionArrays.FreeList[saTransactionArray2ValidIndex(TransactionArray)] = value;}
utInlineC void saTransactionArraySetConstructorCallback(void(*func)(saTransactionArray)) {saTransactionArrayConstructorCallback = func;}
utInlineC saTransactionArrayCallbackType saTransactionArrayGetConstructorCallback(void) {return saTransactionArrayConstructorCallback;}
utInlineC void saTransactionArraySetDestructorCallback(void(*func)(saTransactionArray)) {saTransactionArrayDestructorCallback = func;}
utInlineC saTransactionArrayCallbackType saTransactionArrayGetDestructorCallback(void) {return saTransactionArrayDestructorCallback;}
utInlineC saTransactionArray saTransactionArrayNextFree(saTransactionArray TransactionArray) {return ((saTransactionArray *)(void *)(saTransactionArrays.FreeList))[saTransactionArray2ValidIndex(TransactionArray)];}
utInlineC void saTransactionArraySetNextFree(saTransactionArray TransactionArray, saTransactionArray value) {
    ((saTransactionArray *)(void *)(saTransactionArrays.FreeList))[saTransactionArray2ValidIndex(TransactionArray)] = value;}
utInlineC void saTransactionArrayFree(saTransactionArray TransactionArray) {
    saTransactionArrayFreeTransactions(TransactionArray);
    saTransactionArraySetNextFree(TransactionArray, saRootData.firstFreeTransactionArray);
    saSetFirstFreeTransactionArray(TransactionArray);}
void saTransactionArrayDestroy(saTransactionArray TransactionArray);
utInlineC saTransactionArray saTransactionArrayAllocRaw(void) {
    saTransactionArray TransactionArray;
    if(saRootData.firstFreeTransactionArray != saTransactionArrayNull) {
        TransactionArray = saRootData.firstFreeTransactionArray;
        saSetFirstFreeTransactionArray(saTransactionArrayNextFree(TransactionArray));
    } else {
        if(saRootData.usedTransactionArray == saRootData.allocatedTransactionArray) {
            saTransactionArrayAllocMore();
        }
        TransactionArray = saIndex2TransactionArray(saRootData.usedTransactionArray);
        saSetUsedTransactionArray(saUsedTransactionArray() + 1);
    }
    return TransactionArray;}
utInlineC saTransactionArray saTransactionArrayAlloc(void) {
    saTransactionArray TransactionArray = saTransactionArrayAllocRaw();
    saTransactionArraySetTransactionIndex_(TransactionArray, 0);
    saTransactionArraySetNumTransaction(TransactionArray, 0);
    saTransactionArraySetNumTransaction(TransactionArray, 0);
    saTransactionArraySetUsedTransaction(TransactionArray, 0);
    saTransactionArraySetFreeList(TransactionArray, saTransactionArrayNull);
    if(saTransactionArrayConstructorCallback != NULL) {
        saTransactionArrayConstructorCallback(TransactionArray);
    }
    return TransactionArray;}

/*----------------------------------------------------------------------------------------
  Relationship macros between classes.
----------------------------------------------------------------------------------------*/
saRegion saRootFindRegion(saRoot Root, utSym Sym);
void saRootRenameRegion(saRoot Root, saRegion _Region, utSym sym);
utInlineC char *saRegionGetName(saRegion Region) {return utSymGetName(saRegionGetSym(Region));}
#define saForeachRootRegion(pVar, cVar) \
    for(cVar = saRootGetFirstRegion(pVar); cVar != saRegionNull; \
        cVar = saRegionGetNextRootRegion(cVar))
#define saEndRootRegion
#define saSafeForeachRootRegion(pVar, cVar) { \
    saRegion _nextRegion; \
    for(cVar = saRootGetFirstRegion(pVar); cVar != saRegionNull; cVar = _nextRegion) { \
        _nextRegion = saRegionGetNextRootRegion(cVar);
#define saEndSafeRootRegion }}
saUser saRootFindUser(saRoot Root, utSym Sym);
void saRootRenameUser(saRoot Root, saUser _User, utSym sym);
utInlineC char *saUserGetName(saUser User) {return utSymGetName(saUserGetSym(User));}
#define saForeachRootUser(pVar, cVar) \
    for(cVar = saRootGetFirstUser(pVar); cVar != saUserNull; \
        cVar = saUserGetNextRootUser(cVar))
#define saEndRootUser
#define saSafeForeachRootUser(pVar, cVar) { \
    saUser _nextUser; \
    for(cVar = saRootGetFirstUser(pVar); cVar != saUserNull; cVar = _nextUser) { \
        _nextUser = saUserGetNextRootUser(cVar);
#define saEndSafeRootUser }}
saUser saRootFindByEmailUser(saRoot Root, utSym Email);
#define saForeachRootByEmailUser(pVar, cVar) \
    for(cVar = saRootGetFirstByEmailUser(pVar); cVar != saUserNull; \
        cVar = saUserGetNextRootByEmailUser(cVar))
#define saEndRootByEmailUser
#define saSafeForeachRootByEmailUser(pVar, cVar) { \
    saUser _nextUser; \
    for(cVar = saRootGetFirstByEmailUser(pVar); cVar != saUserNull; cVar = _nextUser) { \
        _nextUser = saUserGetNextRootByEmailUser(cVar);
#define saEndSafeRootByEmailUser }}
#define saForeachRootCharity(pVar, cVar) \
    for(cVar = saRootGetFirstCharity(pVar); cVar != saCharityNull; \
        cVar = saCharityGetNextRootCharity(cVar))
#define saEndRootCharity
#define saSafeForeachRootCharity(pVar, cVar) { \
    saCharity _nextCharity; \
    for(cVar = saRootGetFirstCharity(pVar); cVar != saCharityNull; cVar = _nextCharity) { \
        _nextCharity = saCharityGetNextRootCharity(cVar);
#define saEndSafeRootCharity }}
saCategory saRootFindCategory(saRoot Root, utSym Sym);
void saRootRenameCategory(saRoot Root, saCategory _Category, utSym sym);
utInlineC char *saCategoryGetName(saCategory Category) {return utSymGetName(saCategoryGetSym(Category));}
#define saForeachRootCategory(pVar, cVar) \
    for(cVar = saRootGetFirstCategory(pVar); cVar != saCategoryNull; \
        cVar = saCategoryGetNextRootCategory(cVar))
#define saEndRootCategory
#define saSafeForeachRootCategory(pVar, cVar) { \
    saCategory _nextCategory; \
    for(cVar = saRootGetFirstCategory(pVar); cVar != saCategoryNull; cVar = _nextCategory) { \
        _nextCategory = saCategoryGetNextRootCategory(cVar);
#define saEndSafeRootCategory }}
saListing saRootFindListing(saRoot Root, uint64 ID);
#define saForeachRootListing(pVar, cVar) \
    for(cVar = saRootGetFirstListing(pVar); cVar != saListingNull; \
        cVar = saListingGetNextRootListing(cVar))
#define saEndRootListing
#define saSafeForeachRootListing(pVar, cVar) { \
    saListing _nextListing; \
    for(cVar = saRootGetFirstListing(pVar); cVar != saListingNull; cVar = _nextListing) { \
        _nextListing = saListingGetNextRootListing(cVar);
#define saEndSafeRootListing }}
saSession saRootFindSession(saRoot Root, utSym Sym);
void saRootRenameSession(saRoot Root, saSession _Session, utSym sym);
utInlineC char *saSessionGetName(saSession Session) {return utSymGetName(saSessionGetSym(Session));}
#define saForeachRootSession(pVar, cVar) \
    for(cVar = saRootGetFirstSession(pVar); cVar != saSessionNull; \
        cVar = saSessionGetNextRootSession(cVar))
#define saEndRootSession
#define saSafeForeachRootSession(pVar, cVar) { \
    saSession _nextSession; \
    for(cVar = saRootGetFirstSession(pVar); cVar != saSessionNull; cVar = _nextSession) { \
        _nextSession = saSessionGetNextRootSession(cVar);
#define saEndSafeRootSession }}
saThread saRootFindThread(saRoot Root, uint32 ID);
#define saForeachRootThread(pVar, cVar) \
    for(cVar = saRootGetFirstThread(pVar); cVar != saThreadNull; \
        cVar = saThreadGetNextRootThread(cVar))
#define saEndRootThread
#define saSafeForeachRootThread(pVar, cVar) { \
    saThread _nextThread; \
    for(cVar = saRootGetFirstThread(pVar); cVar != saThreadNull; cVar = _nextThread) { \
        _nextThread = saThreadGetNextRootThread(cVar);
#define saEndSafeRootThread }}
saPost saRootFindPost(saRoot Root, uint32 ID);
#define saForeachRootPost(pVar, cVar) \
    for(cVar = saRootGetFirstPost(pVar); cVar != saPostNull; \
        cVar = saPostGetNextRootPost(cVar))
#define saEndRootPost
#define saSafeForeachRootPost(pVar, cVar) { \
    saPost _nextPost; \
    for(cVar = saRootGetFirstPost(pVar); cVar != saPostNull; cVar = _nextPost) { \
        _nextPost = saPostGetNextRootPost(cVar);
#define saEndSafeRootPost }}
saAnnouncement saRootFindAnnouncement(saRoot Root, uint32 ID);
#define saForeachRootAnnouncement(pVar, cVar) \
    for(cVar = saRootGetFirstAnnouncement(pVar); cVar != saAnnouncementNull; \
        cVar = saAnnouncementGetNextRootAnnouncement(cVar))
#define saEndRootAnnouncement
#define saSafeForeachRootAnnouncement(pVar, cVar) { \
    saAnnouncement _nextAnnouncement; \
    for(cVar = saRootGetFirstAnnouncement(pVar); cVar != saAnnouncementNull; cVar = _nextAnnouncement) { \
        _nextAnnouncement = saAnnouncementGetNextRootAnnouncement(cVar);
#define saEndSafeRootAnnouncement }}
void saRootInsertRegion(saRoot Root, saRegion _Region);
void saRootRemoveRegion(saRoot Root, saRegion _Region);
void saRootInsertAfterRegion(saRoot Root, saRegion prevRegion, saRegion _Region);
void saRootAppendRegion(saRoot Root, saRegion _Region);
void saRootInsertUser(saRoot Root, saUser _User);
void saRootRemoveUser(saRoot Root, saUser _User);
void saRootInsertAfterUser(saRoot Root, saUser prevUser, saUser _User);
void saRootAppendUser(saRoot Root, saUser _User);
void saRootInsertByEmailUser(saRoot Root, saUser _User);
void saRootRemoveByEmailUser(saRoot Root, saUser _User);
void saRootInsertAfterByEmailUser(saRoot Root, saUser prevUser, saUser _User);
void saRootAppendByEmailUser(saRoot Root, saUser _User);
void saRootInsertCharity(saRoot Root, saCharity _Charity);
void saRootRemoveCharity(saRoot Root, saCharity _Charity);
void saRootInsertAfterCharity(saRoot Root, saCharity prevCharity, saCharity _Charity);
void saRootAppendCharity(saRoot Root, saCharity _Charity);
void saRootInsertCategory(saRoot Root, saCategory _Category);
void saRootRemoveCategory(saRoot Root, saCategory _Category);
void saRootInsertAfterCategory(saRoot Root, saCategory prevCategory, saCategory _Category);
void saRootAppendCategory(saRoot Root, saCategory _Category);
void saRootInsertListing(saRoot Root, saListing _Listing);
void saRootRemoveListing(saRoot Root, saListing _Listing);
void saRootInsertAfterListing(saRoot Root, saListing prevListing, saListing _Listing);
void saRootAppendListing(saRoot Root, saListing _Listing);
void saRootInsertSession(saRoot Root, saSession _Session);
void saRootRemoveSession(saRoot Root, saSession _Session);
void saRootInsertAfterSession(saRoot Root, saSession prevSession, saSession _Session);
void saRootAppendSession(saRoot Root, saSession _Session);
void saRootInsertThread(saRoot Root, saThread _Thread);
void saRootRemoveThread(saRoot Root, saThread _Thread);
void saRootInsertAfterThread(saRoot Root, saThread prevThread, saThread _Thread);
void saRootAppendThread(saRoot Root, saThread _Thread);
void saRootInsertPost(saRoot Root, saPost _Post);
void saRootRemovePost(saRoot Root, saPost _Post);
void saRootInsertAfterPost(saRoot Root, saPost prevPost, saPost _Post);
void saRootAppendPost(saRoot Root, saPost _Post);
void saRootInsertAnnouncement(saRoot Root, saAnnouncement _Announcement);
void saRootRemoveAnnouncement(saRoot Root, saAnnouncement _Announcement);
void saRootInsertAfterAnnouncement(saRoot Root, saAnnouncement prevAnnouncement, saAnnouncement _Announcement);
void saRootAppendAnnouncement(saRoot Root, saAnnouncement _Announcement);
#define saForeachRegionUser(pVar, cVar) \
    for(cVar = saRegionGetFirstUser(pVar); cVar != saUserNull; \
        cVar = saUserGetNextRegionUser(cVar))
#define saEndRegionUser
#define saSafeForeachRegionUser(pVar, cVar) { \
    saUser _nextUser; \
    for(cVar = saRegionGetFirstUser(pVar); cVar != saUserNull; cVar = _nextUser) { \
        _nextUser = saUserGetNextRegionUser(cVar);
#define saEndSafeRegionUser }}
void saRegionInsertUser(saRegion Region, saUser _User);
void saRegionRemoveUser(saRegion Region, saUser _User);
void saRegionInsertAfterUser(saRegion Region, saUser prevUser, saUser _User);
void saRegionAppendUser(saRegion Region, saUser _User);
utInlineC void saSessionInsertUser(saSession Session, saUser _User) {saSessionSetUser(Session, _User); saUserSetSession(_User, Session);}
utInlineC void saSessionRemoveUser(saSession Session, saUser _User) {saSessionSetUser(Session, saUserNull); saUserSetSession(_User, saSessionNull);}
utInlineC void saSessionInsertRegion(saSession Session, saRegion _Region) {saSessionSetRegion(Session, _Region); saRegionSetSession(_Region, Session);}
utInlineC void saSessionRemoveRegion(saSession Session, saRegion _Region) {saSessionSetRegion(Session, saRegionNull); saRegionSetSession(_Region, saSessionNull);}
saMembership saUserFindMembership(saUser User, saCharity Charity);
#define saForeachUserMembership(pVar, cVar) \
    for(cVar = saUserGetFirstMembership(pVar); cVar != saMembershipNull; \
        cVar = saMembershipGetNextUserMembership(cVar))
#define saEndUserMembership
#define saSafeForeachUserMembership(pVar, cVar) { \
    saMembership _nextMembership; \
    for(cVar = saUserGetFirstMembership(pVar); cVar != saMembershipNull; cVar = _nextMembership) { \
        _nextMembership = saMembershipGetNextUserMembership(cVar);
#define saEndSafeUserMembership }}
#define saForeachUserOutTransaction(pVar, cVar) \
    for(cVar = saUserGetFirstOutTransaction(pVar); cVar != saTransactionNull; \
        cVar = saTransactionGetNextUserOutTransaction(cVar))
#define saEndUserOutTransaction
#define saSafeForeachUserOutTransaction(pVar, cVar) { \
    saTransaction _nextTransaction; \
    for(cVar = saUserGetFirstOutTransaction(pVar); cVar != saTransactionNull; cVar = _nextTransaction) { \
        _nextTransaction = saTransactionGetNextUserOutTransaction(cVar);
#define saEndSafeUserOutTransaction }}
#define saForeachUserInTransaction(pVar, cVar) \
    for(cVar = saUserGetFirstInTransaction(pVar); cVar != saTransactionNull; \
        cVar = saTransactionGetNextUserInTransaction(cVar))
#define saEndUserInTransaction
#define saSafeForeachUserInTransaction(pVar, cVar) { \
    saTransaction _nextTransaction; \
    for(cVar = saUserGetFirstInTransaction(pVar); cVar != saTransactionNull; cVar = _nextTransaction) { \
        _nextTransaction = saTransactionGetNextUserInTransaction(cVar);
#define saEndSafeUserInTransaction }}
#define saForeachUserListing(pVar, cVar) \
    for(cVar = saUserGetFirstListing(pVar); cVar != saListingNull; \
        cVar = saListingGetNextUserListing(cVar))
#define saEndUserListing
#define saSafeForeachUserListing(pVar, cVar) { \
    saListing _nextListing; \
    for(cVar = saUserGetFirstListing(pVar); cVar != saListingNull; cVar = _nextListing) { \
        _nextListing = saListingGetNextUserListing(cVar);
#define saEndSafeUserListing }}
saPost saUserFindPost(saUser User, uint32 ID);
#define saForeachUserPost(pVar, cVar) \
    for(cVar = saUserGetFirstPost(pVar); cVar != saPostNull; \
        cVar = saPostGetNextUserPost(cVar))
#define saEndUserPost
#define saSafeForeachUserPost(pVar, cVar) { \
    saPost _nextPost; \
    for(cVar = saUserGetFirstPost(pVar); cVar != saPostNull; cVar = _nextPost) { \
        _nextPost = saPostGetNextUserPost(cVar);
#define saEndSafeUserPost }}
#define saForeachUserSubscription(pVar, cVar) \
    for(cVar = saUserGetFirstSubscription(pVar); cVar != saSubscriptionNull; \
        cVar = saSubscriptionGetNextUserSubscription(cVar))
#define saEndUserSubscription
#define saSafeForeachUserSubscription(pVar, cVar) { \
    saSubscription _nextSubscription; \
    for(cVar = saUserGetFirstSubscription(pVar); cVar != saSubscriptionNull; cVar = _nextSubscription) { \
        _nextSubscription = saSubscriptionGetNextUserSubscription(cVar);
#define saEndSafeUserSubscription }}
#define saForeachUserAnnouncement(pVar, cVar) \
    for(cVar = saUserGetFirstAnnouncement(pVar); cVar != saAnnouncementNull; \
        cVar = saAnnouncementGetNextUserAnnouncement(cVar))
#define saEndUserAnnouncement
#define saSafeForeachUserAnnouncement(pVar, cVar) { \
    saAnnouncement _nextAnnouncement; \
    for(cVar = saUserGetFirstAnnouncement(pVar); cVar != saAnnouncementNull; cVar = _nextAnnouncement) { \
        _nextAnnouncement = saAnnouncementGetNextUserAnnouncement(cVar);
#define saEndSafeUserAnnouncement }}
utInlineC void saUserInsertCharity(saUser User, saCharity _Charity) {saUserSetCharity(User, _Charity); saCharitySetUser(_Charity, User);}
utInlineC void saUserRemoveCharity(saUser User, saCharity _Charity) {saUserSetCharity(User, saCharityNull); saCharitySetUser(_Charity, saUserNull);}
void saUserInsertMembership(saUser User, saMembership _Membership);
void saUserRemoveMembership(saUser User, saMembership _Membership);
void saUserInsertAfterMembership(saUser User, saMembership prevMembership, saMembership _Membership);
void saUserAppendMembership(saUser User, saMembership _Membership);
void saUserInsertOutTransaction(saUser User, saTransaction _Transaction);
void saUserRemoveOutTransaction(saUser User, saTransaction _Transaction);
void saUserInsertAfterOutTransaction(saUser User, saTransaction prevTransaction, saTransaction _Transaction);
void saUserAppendOutTransaction(saUser User, saTransaction _Transaction);
void saUserInsertInTransaction(saUser User, saTransaction _Transaction);
void saUserRemoveInTransaction(saUser User, saTransaction _Transaction);
void saUserInsertAfterInTransaction(saUser User, saTransaction prevTransaction, saTransaction _Transaction);
void saUserAppendInTransaction(saUser User, saTransaction _Transaction);
void saUserInsertListing(saUser User, saListing _Listing);
void saUserRemoveListing(saUser User, saListing _Listing);
void saUserInsertAfterListing(saUser User, saListing prevListing, saListing _Listing);
void saUserAppendListing(saUser User, saListing _Listing);
void saUserInsertPost(saUser User, saPost _Post);
void saUserRemovePost(saUser User, saPost _Post);
void saUserInsertAfterPost(saUser User, saPost prevPost, saPost _Post);
void saUserAppendPost(saUser User, saPost _Post);
void saUserInsertSubscription(saUser User, saSubscription _Subscription);
void saUserRemoveSubscription(saUser User, saSubscription _Subscription);
void saUserInsertAfterSubscription(saUser User, saSubscription prevSubscription, saSubscription _Subscription);
void saUserAppendSubscription(saUser User, saSubscription _Subscription);
void saUserInsertAnnouncement(saUser User, saAnnouncement _Announcement);
void saUserRemoveAnnouncement(saUser User, saAnnouncement _Announcement);
void saUserInsertAfterAnnouncement(saUser User, saAnnouncement prevAnnouncement, saAnnouncement _Announcement);
void saUserAppendAnnouncement(saUser User, saAnnouncement _Announcement);
#define saForeachCharityMembership(pVar, cVar) \
    for(cVar = saCharityGetFirstMembership(pVar); cVar != saMembershipNull; \
        cVar = saMembershipGetNextCharityMembership(cVar))
#define saEndCharityMembership
#define saSafeForeachCharityMembership(pVar, cVar) { \
    saMembership _nextMembership; \
    for(cVar = saCharityGetFirstMembership(pVar); cVar != saMembershipNull; cVar = _nextMembership) { \
        _nextMembership = saMembershipGetNextCharityMembership(cVar);
#define saEndSafeCharityMembership }}
void saCharityInsertMembership(saCharity Charity, saMembership _Membership);
void saCharityRemoveMembership(saCharity Charity, saMembership _Membership);
void saCharityInsertAfterMembership(saCharity Charity, saMembership prevMembership, saMembership _Membership);
void saCharityAppendMembership(saCharity Charity, saMembership _Membership);
#define saForeachCategoryListing(pVar, cVar) \
    for(cVar = saCategoryGetFirstListing(pVar); cVar != saListingNull; \
        cVar = saListingGetNextCategoryListing(cVar))
#define saEndCategoryListing
#define saSafeForeachCategoryListing(pVar, cVar) { \
    saListing _nextListing; \
    for(cVar = saCategoryGetFirstListing(pVar); cVar != saListingNull; cVar = _nextListing) { \
        _nextListing = saListingGetNextCategoryListing(cVar);
#define saEndSafeCategoryListing }}
void saCategoryInsertListing(saCategory Category, saListing _Listing);
void saCategoryRemoveListing(saCategory Category, saListing _Listing);
void saCategoryInsertAfterListing(saCategory Category, saListing prevListing, saListing _Listing);
void saCategoryAppendListing(saCategory Category, saListing _Listing);
#define saForeachThreadPost(pVar, cVar) \
    for(cVar = saThreadGetFirstPost(pVar); cVar != saPostNull; \
        cVar = saPostGetNextThreadPost(cVar))
#define saEndThreadPost
#define saSafeForeachThreadPost(pVar, cVar) { \
    saPost _nextPost; \
    for(cVar = saThreadGetFirstPost(pVar); cVar != saPostNull; cVar = _nextPost) { \
        _nextPost = saPostGetNextThreadPost(cVar);
#define saEndSafeThreadPost }}
#define saForeachThreadSubscription(pVar, cVar) \
    for(cVar = saThreadGetFirstSubscription(pVar); cVar != saSubscriptionNull; \
        cVar = saSubscriptionGetNextThreadSubscription(cVar))
#define saEndThreadSubscription
#define saSafeForeachThreadSubscription(pVar, cVar) { \
    saSubscription _nextSubscription; \
    for(cVar = saThreadGetFirstSubscription(pVar); cVar != saSubscriptionNull; cVar = _nextSubscription) { \
        _nextSubscription = saSubscriptionGetNextThreadSubscription(cVar);
#define saEndSafeThreadSubscription }}
void saThreadInsertPost(saThread Thread, saPost _Post);
void saThreadRemovePost(saThread Thread, saPost _Post);
void saThreadInsertAfterPost(saThread Thread, saPost prevPost, saPost _Post);
void saThreadAppendPost(saThread Thread, saPost _Post);
void saThreadInsertSubscription(saThread Thread, saSubscription _Subscription);
void saThreadRemoveSubscription(saThread Thread, saSubscription _Subscription);
void saThreadInsertAfterSubscription(saThread Thread, saSubscription prevSubscription, saSubscription _Subscription);
void saThreadAppendSubscription(saThread Thread, saSubscription _Subscription);
#define saForeachTransactionArrayTransaction(pVar, cVar) { \
    uint32 _xTransaction; \
    for(_xTransaction = 0; _xTransaction < saTransactionArrayGetUsedTransaction(pVar); _xTransaction++) { \
        cVar = saTransactionArrayGetiTransaction(pVar, _xTransaction); \
        if(cVar != saTransactionNull) {
#define saEndTransactionArrayTransaction }}}
void saTransactionArrayInsertTransaction(saTransactionArray TransactionArray, uint32 x, saTransaction _Transaction);
void saTransactionArrayAppendTransaction(saTransactionArray TransactionArray, saTransaction _Transaction);
void saDatabaseStart(void);
void saDatabaseStop(void);
#if defined __cplusplus
}
#endif

#endif
