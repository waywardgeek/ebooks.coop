/*----------------------------------------------------------------------------------------
  Database sa
----------------------------------------------------------------------------------------*/

#include "sadatabase.h"

struct saRootType_ saRootData;
uint8 saModuleID;
struct saRootFields saRoots;
struct saRegionFields saRegions;
struct saSessionFields saSessions;
struct saUserFields saUsers;
struct saTransactionFields saTransactions;
struct saCharityFields saCharitys;
struct saMembershipFields saMemberships;
struct saCategoryFields saCategorys;
struct saListingFields saListings;
struct saThreadFields saThreads;
struct saPostFields saPosts;
struct saSubscriptionFields saSubscriptions;
struct saAnnouncementFields saAnnouncements;
struct saTransactionArrayFields saTransactionArrays;

/*----------------------------------------------------------------------------------------
  Constructor/Destructor hooks.
----------------------------------------------------------------------------------------*/
saRootCallbackType saRootConstructorCallback;
saRootCallbackType saRootDestructorCallback;
saRegionCallbackType saRegionConstructorCallback;
saRegionCallbackType saRegionDestructorCallback;
saSessionCallbackType saSessionConstructorCallback;
saSessionCallbackType saSessionDestructorCallback;
saUserCallbackType saUserConstructorCallback;
saUserCallbackType saUserDestructorCallback;
saTransactionCallbackType saTransactionConstructorCallback;
saTransactionCallbackType saTransactionDestructorCallback;
saCharityCallbackType saCharityConstructorCallback;
saCharityCallbackType saCharityDestructorCallback;
saMembershipCallbackType saMembershipConstructorCallback;
saMembershipCallbackType saMembershipDestructorCallback;
saCategoryCallbackType saCategoryConstructorCallback;
saCategoryCallbackType saCategoryDestructorCallback;
saListingCallbackType saListingConstructorCallback;
saListingCallbackType saListingDestructorCallback;
saThreadCallbackType saThreadConstructorCallback;
saThreadCallbackType saThreadDestructorCallback;
saPostCallbackType saPostConstructorCallback;
saPostCallbackType saPostDestructorCallback;
saSubscriptionCallbackType saSubscriptionConstructorCallback;
saSubscriptionCallbackType saSubscriptionDestructorCallback;
saAnnouncementCallbackType saAnnouncementConstructorCallback;
saAnnouncementCallbackType saAnnouncementDestructorCallback;
saTransactionArrayCallbackType saTransactionArrayConstructorCallback;
saTransactionArrayCallbackType saTransactionArrayDestructorCallback;

/*----------------------------------------------------------------------------------------
  Destroy Root including everything in it. Remove from parents.
----------------------------------------------------------------------------------------*/
void saRootDestroy(
    saRoot Root)
{
    saRegion Region_;
    saUser User_;
    saUser ByEmailUser_;
    saCharity Charity_;
    saCategory Category_;
    saListing Listing_;
    saSession Session_;
    saThread Thread_;
    saPost Post_;
    saAnnouncement Announcement_;

    if(saRootDestructorCallback != NULL) {
        saRootDestructorCallback(Root);
    }
    saSafeForeachRootRegion(Root, Region_) {
        saRegionSetRoot(Region_, saRootNull);
    } saEndSafeRootRegion;
    saSafeForeachRootUser(Root, User_) {
        saUserSetRoot(User_, saRootNull);
    } saEndSafeRootUser;
    saSafeForeachRootByEmailUser(Root, ByEmailUser_) {
        saUserSetByEmailRoot(ByEmailUser_, saRootNull);
    } saEndSafeRootByEmailUser;
    saSafeForeachRootCharity(Root, Charity_) {
        saCharitySetRoot(Charity_, saRootNull);
    } saEndSafeRootCharity;
    saSafeForeachRootCategory(Root, Category_) {
        saCategorySetRoot(Category_, saRootNull);
    } saEndSafeRootCategory;
    saSafeForeachRootListing(Root, Listing_) {
        saListingSetRoot(Listing_, saRootNull);
    } saEndSafeRootListing;
    saSafeForeachRootSession(Root, Session_) {
        saSessionSetRoot(Session_, saRootNull);
    } saEndSafeRootSession;
    saSafeForeachRootThread(Root, Thread_) {
        saThreadSetRoot(Thread_, saRootNull);
    } saEndSafeRootThread;
    saSafeForeachRootPost(Root, Post_) {
        saPostSetRoot(Post_, saRootNull);
    } saEndSafeRootPost;
    saSafeForeachRootAnnouncement(Root, Announcement_) {
        saAnnouncementDestroy(Announcement_);
    } saEndSafeRootAnnouncement;
    saRootFree(Root);
}

/*----------------------------------------------------------------------------------------
  Default constructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static uint64 allocRoot(void)
{
    saRoot Root = saRootAlloc();

    return saRoot2Index(Root);
}

/*----------------------------------------------------------------------------------------
  Destructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static void destroyRoot(
    uint64 objectIndex)
{
    saRootDestroy(saIndex2Root((uint32)objectIndex));
}

/*----------------------------------------------------------------------------------------
  Allocate the field arrays of Root.
----------------------------------------------------------------------------------------*/
static void allocRoots(void)
{
    saSetAllocatedRoot(2);
    saSetUsedRoot(1);
    saSetFirstFreeRoot(saRootNull);
    saRoots.NextListingID = utNewAInitFirst(uint64, (saAllocatedRoot()));
    saRoots.NextThreadID = utNewAInitFirst(uint64, (saAllocatedRoot()));
    saRoots.NextPostID = utNewAInitFirst(uint64, (saAllocatedRoot()));
    saRoots.NextAnnouncementID = utNewAInitFirst(uint64, (saAllocatedRoot()));
    saRoots.FirstRegion = utNewAInitFirst(saRegion, (saAllocatedRoot()));
    saRoots.LastRegion = utNewAInitFirst(saRegion, (saAllocatedRoot()));
    saRoots.RegionTableIndex_ = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saRoots.NumRegionTable = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saSetUsedRootRegionTable(0);
    saSetAllocatedRootRegionTable(2);
    saSetFreeRootRegionTable(0);
    saRoots.RegionTable = utNewAInitFirst(saRegion, saAllocatedRootRegionTable());
    saRoots.NumRegion = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saRoots.FirstUser = utNewAInitFirst(saUser, (saAllocatedRoot()));
    saRoots.LastUser = utNewAInitFirst(saUser, (saAllocatedRoot()));
    saRoots.UserTableIndex_ = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saRoots.NumUserTable = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saSetUsedRootUserTable(0);
    saSetAllocatedRootUserTable(2);
    saSetFreeRootUserTable(0);
    saRoots.UserTable = utNewAInitFirst(saUser, saAllocatedRootUserTable());
    saRoots.NumUser = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saRoots.FirstByEmailUser = utNewAInitFirst(saUser, (saAllocatedRoot()));
    saRoots.LastByEmailUser = utNewAInitFirst(saUser, (saAllocatedRoot()));
    saRoots.ByEmailUserTableIndex_ = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saRoots.NumByEmailUserTable = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saSetUsedRootByEmailUserTable(0);
    saSetAllocatedRootByEmailUserTable(2);
    saSetFreeRootByEmailUserTable(0);
    saRoots.ByEmailUserTable = utNewAInitFirst(saUser, saAllocatedRootByEmailUserTable());
    saRoots.NumByEmailUser = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saRoots.FirstCharity = utNewAInitFirst(saCharity, (saAllocatedRoot()));
    saRoots.LastCharity = utNewAInitFirst(saCharity, (saAllocatedRoot()));
    saRoots.FirstCategory = utNewAInitFirst(saCategory, (saAllocatedRoot()));
    saRoots.LastCategory = utNewAInitFirst(saCategory, (saAllocatedRoot()));
    saRoots.CategoryTableIndex_ = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saRoots.NumCategoryTable = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saSetUsedRootCategoryTable(0);
    saSetAllocatedRootCategoryTable(2);
    saSetFreeRootCategoryTable(0);
    saRoots.CategoryTable = utNewAInitFirst(saCategory, saAllocatedRootCategoryTable());
    saRoots.NumCategory = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saRoots.FirstListing = utNewAInitFirst(saListing, (saAllocatedRoot()));
    saRoots.LastListing = utNewAInitFirst(saListing, (saAllocatedRoot()));
    saRoots.ListingTableIndex_ = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saRoots.NumListingTable = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saSetUsedRootListingTable(0);
    saSetAllocatedRootListingTable(2);
    saSetFreeRootListingTable(0);
    saRoots.ListingTable = utNewAInitFirst(saListing, saAllocatedRootListingTable());
    saRoots.NumListing = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saRoots.FirstSession = utNewAInitFirst(saSession, (saAllocatedRoot()));
    saRoots.LastSession = utNewAInitFirst(saSession, (saAllocatedRoot()));
    saRoots.SessionTableIndex_ = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saRoots.NumSessionTable = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saSetUsedRootSessionTable(0);
    saSetAllocatedRootSessionTable(2);
    saSetFreeRootSessionTable(0);
    saRoots.SessionTable = utNewAInitFirst(saSession, saAllocatedRootSessionTable());
    saRoots.NumSession = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saRoots.FirstThread = utNewAInitFirst(saThread, (saAllocatedRoot()));
    saRoots.LastThread = utNewAInitFirst(saThread, (saAllocatedRoot()));
    saRoots.ThreadTableIndex_ = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saRoots.NumThreadTable = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saSetUsedRootThreadTable(0);
    saSetAllocatedRootThreadTable(2);
    saSetFreeRootThreadTable(0);
    saRoots.ThreadTable = utNewAInitFirst(saThread, saAllocatedRootThreadTable());
    saRoots.NumThread = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saRoots.FirstPost = utNewAInitFirst(saPost, (saAllocatedRoot()));
    saRoots.LastPost = utNewAInitFirst(saPost, (saAllocatedRoot()));
    saRoots.PostTableIndex_ = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saRoots.NumPostTable = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saSetUsedRootPostTable(0);
    saSetAllocatedRootPostTable(2);
    saSetFreeRootPostTable(0);
    saRoots.PostTable = utNewAInitFirst(saPost, saAllocatedRootPostTable());
    saRoots.NumPost = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saRoots.FirstAnnouncement = utNewAInitFirst(saAnnouncement, (saAllocatedRoot()));
    saRoots.LastAnnouncement = utNewAInitFirst(saAnnouncement, (saAllocatedRoot()));
    saRoots.AnnouncementTableIndex_ = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saRoots.NumAnnouncementTable = utNewAInitFirst(uint32, (saAllocatedRoot()));
    saSetUsedRootAnnouncementTable(0);
    saSetAllocatedRootAnnouncementTable(2);
    saSetFreeRootAnnouncementTable(0);
    saRoots.AnnouncementTable = utNewAInitFirst(saAnnouncement, saAllocatedRootAnnouncementTable());
    saRoots.NumAnnouncement = utNewAInitFirst(uint32, (saAllocatedRoot()));
}

/*----------------------------------------------------------------------------------------
  Realloc the arrays of properties for class Root.
----------------------------------------------------------------------------------------*/
static void reallocRoots(
    uint32 newSize)
{
    utResizeArray(saRoots.NextListingID, (newSize));
    utResizeArray(saRoots.NextThreadID, (newSize));
    utResizeArray(saRoots.NextPostID, (newSize));
    utResizeArray(saRoots.NextAnnouncementID, (newSize));
    utResizeArray(saRoots.FirstRegion, (newSize));
    utResizeArray(saRoots.LastRegion, (newSize));
    utResizeArray(saRoots.RegionTableIndex_, (newSize));
    utResizeArray(saRoots.NumRegionTable, (newSize));
    utResizeArray(saRoots.NumRegion, (newSize));
    utResizeArray(saRoots.FirstUser, (newSize));
    utResizeArray(saRoots.LastUser, (newSize));
    utResizeArray(saRoots.UserTableIndex_, (newSize));
    utResizeArray(saRoots.NumUserTable, (newSize));
    utResizeArray(saRoots.NumUser, (newSize));
    utResizeArray(saRoots.FirstByEmailUser, (newSize));
    utResizeArray(saRoots.LastByEmailUser, (newSize));
    utResizeArray(saRoots.ByEmailUserTableIndex_, (newSize));
    utResizeArray(saRoots.NumByEmailUserTable, (newSize));
    utResizeArray(saRoots.NumByEmailUser, (newSize));
    utResizeArray(saRoots.FirstCharity, (newSize));
    utResizeArray(saRoots.LastCharity, (newSize));
    utResizeArray(saRoots.FirstCategory, (newSize));
    utResizeArray(saRoots.LastCategory, (newSize));
    utResizeArray(saRoots.CategoryTableIndex_, (newSize));
    utResizeArray(saRoots.NumCategoryTable, (newSize));
    utResizeArray(saRoots.NumCategory, (newSize));
    utResizeArray(saRoots.FirstListing, (newSize));
    utResizeArray(saRoots.LastListing, (newSize));
    utResizeArray(saRoots.ListingTableIndex_, (newSize));
    utResizeArray(saRoots.NumListingTable, (newSize));
    utResizeArray(saRoots.NumListing, (newSize));
    utResizeArray(saRoots.FirstSession, (newSize));
    utResizeArray(saRoots.LastSession, (newSize));
    utResizeArray(saRoots.SessionTableIndex_, (newSize));
    utResizeArray(saRoots.NumSessionTable, (newSize));
    utResizeArray(saRoots.NumSession, (newSize));
    utResizeArray(saRoots.FirstThread, (newSize));
    utResizeArray(saRoots.LastThread, (newSize));
    utResizeArray(saRoots.ThreadTableIndex_, (newSize));
    utResizeArray(saRoots.NumThreadTable, (newSize));
    utResizeArray(saRoots.NumThread, (newSize));
    utResizeArray(saRoots.FirstPost, (newSize));
    utResizeArray(saRoots.LastPost, (newSize));
    utResizeArray(saRoots.PostTableIndex_, (newSize));
    utResizeArray(saRoots.NumPostTable, (newSize));
    utResizeArray(saRoots.NumPost, (newSize));
    utResizeArray(saRoots.FirstAnnouncement, (newSize));
    utResizeArray(saRoots.LastAnnouncement, (newSize));
    utResizeArray(saRoots.AnnouncementTableIndex_, (newSize));
    utResizeArray(saRoots.NumAnnouncementTable, (newSize));
    utResizeArray(saRoots.NumAnnouncement, (newSize));
    saSetAllocatedRoot(newSize);
}

/*----------------------------------------------------------------------------------------
  Allocate more Roots.
----------------------------------------------------------------------------------------*/
void saRootAllocMore(void)
{
    reallocRoots((uint32)(saAllocatedRoot() + (saAllocatedRoot() >> 1)));
}

/*----------------------------------------------------------------------------------------
  Compact the Root.RegionTable heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactRootRegionTables(void)
{
    uint32 elementSize = sizeof(saRegion);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    saRegion *toPtr = saRoots.RegionTable;
    saRegion *fromPtr = toPtr;
    saRoot Root;
    uint32 size;

    while(fromPtr < saRoots.RegionTable + saUsedRootRegionTable()) {
        Root = *(saRoot *)(void *)fromPtr;
        if(Root != saRootNull) {
            /* Need to move it to toPtr */
            size = utMax(saRootGetNumRegionTable(Root) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saRootSetRegionTableIndex_(Root, toPtr - saRoots.RegionTable + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saRoot *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedRootRegionTable(toPtr - saRoots.RegionTable);
    saSetFreeRootRegionTable(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the Root.RegionTable heap.
----------------------------------------------------------------------------------------*/
static void allocMoreRootRegionTables(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedRootRegionTable() - saUsedRootRegionTable();

    if((saFreeRootRegionTable() << 2) > saUsedRootRegionTable()) {
        saCompactRootRegionTables();
        freeSpace = saAllocatedRootRegionTable() - saUsedRootRegionTable();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedRootRegionTable(saAllocatedRootRegionTable() + spaceNeeded - freeSpace +
            (saAllocatedRootRegionTable() >> 1));
        utResizeArray(saRoots.RegionTable, saAllocatedRootRegionTable());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new Root.RegionTable array.
----------------------------------------------------------------------------------------*/
void saRootAllocRegionTables(
    saRoot Root,
    uint32 numRegionTables)
{
    uint32 freeSpace = saAllocatedRootRegionTable() - saUsedRootRegionTable();
    uint32 elementSize = sizeof(saRegion);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numRegionTables + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saRootGetNumRegionTable(Root) == 0);
#endif
    if(numRegionTables == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMoreRootRegionTables(spaceNeeded);
    }
    saRootSetRegionTableIndex_(Root, saUsedRootRegionTable() + usedHeaderSize);
    saRootSetNumRegionTable(Root, numRegionTables);
    *(saRoot *)(void *)(saRoots.RegionTable + saUsedRootRegionTable()) = Root;
    {
        uint32 xValue;
        for(xValue = (uint32)(saRootGetRegionTableIndex_(Root)); xValue < saRootGetRegionTableIndex_(Root) + numRegionTables; xValue++) {
            saRoots.RegionTable[xValue] = saRegionNull;
        }
    }
    saSetUsedRootRegionTable(saUsedRootRegionTable() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saRootGetRegionTables for the database manager.
----------------------------------------------------------------------------------------*/
static void *getRootRegionTables(
    uint64 objectNumber,
    uint32 *numValues)
{
    saRoot Root = saIndex2Root((uint32)objectNumber);

    *numValues = saRootGetNumRegionTable(Root);
    return saRootGetRegionTables(Root);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saRootAllocRegionTables for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocRootRegionTables(
    uint64 objectNumber,
    uint32 numValues)
{
    saRoot Root = saIndex2Root((uint32)objectNumber);

    saRootSetRegionTableIndex_(Root, 0);
    saRootSetNumRegionTable(Root, 0);
    if(numValues == 0) {
        return NULL;
    }
    saRootAllocRegionTables(Root, numValues);
    return saRootGetRegionTables(Root);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the Root.RegionTable array.
----------------------------------------------------------------------------------------*/
void saRootFreeRegionTables(
    saRoot Root)
{
    uint32 elementSize = sizeof(saRegion);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saRootGetNumRegionTable(Root) + usedHeaderSize, freeHeaderSize);
    saRegion *dataPtr = saRootGetRegionTables(Root) - usedHeaderSize;

    if(saRootGetNumRegionTable(Root) == 0) {
        return;
    }
    *(saRoot *)(void *)(dataPtr) = saRootNull;
    *(uint32 *)(void *)(((saRoot *)(void *)dataPtr) + 1) = size;
    saRootSetNumRegionTable(Root, 0);
    saSetFreeRootRegionTable(saFreeRootRegionTable() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the Root.RegionTable array.
----------------------------------------------------------------------------------------*/
void saRootResizeRegionTables(
    saRoot Root,
    uint32 numRegionTables)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(saRegion);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numRegionTables + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saRootGetNumRegionTable(Root) + usedHeaderSize, freeHeaderSize);
    saRegion *dataPtr;

    if(numRegionTables == 0) {
        if(saRootGetNumRegionTable(Root) != 0) {
            saRootFreeRegionTables(Root);
        }
        return;
    }
    if(saRootGetNumRegionTable(Root) == 0) {
        saRootAllocRegionTables(Root, numRegionTables);
        return;
    }
    freeSpace = saAllocatedRootRegionTable() - saUsedRootRegionTable();
    if(freeSpace < newSize) {
        allocMoreRootRegionTables(newSize);
    }
    dataPtr = saRootGetRegionTables(Root) - usedHeaderSize;
    memcpy((void *)(saRoots.RegionTable + saUsedRootRegionTable()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        {
            uint32 xValue;
            for(xValue = (uint32)(saUsedRootRegionTable() + oldSize); xValue < saUsedRootRegionTable() + oldSize + newSize - oldSize; xValue++) {
                saRoots.RegionTable[xValue] = saRegionNull;
            }
        }
    }
    *(saRoot *)(void *)dataPtr = saRootNull;
    *(uint32 *)(void *)(((saRoot *)(void *)dataPtr) + 1) = oldSize;
    saSetFreeRootRegionTable(saFreeRootRegionTable() + oldSize);
    saRootSetRegionTableIndex_(Root, saUsedRootRegionTable() + usedHeaderSize);
    saRootSetNumRegionTable(Root, numRegionTables);
    saSetUsedRootRegionTable(saUsedRootRegionTable() + newSize);
}

/*----------------------------------------------------------------------------------------
  Compact the Root.UserTable heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactRootUserTables(void)
{
    uint32 elementSize = sizeof(saUser);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    saUser *toPtr = saRoots.UserTable;
    saUser *fromPtr = toPtr;
    saRoot Root;
    uint32 size;

    while(fromPtr < saRoots.UserTable + saUsedRootUserTable()) {
        Root = *(saRoot *)(void *)fromPtr;
        if(Root != saRootNull) {
            /* Need to move it to toPtr */
            size = utMax(saRootGetNumUserTable(Root) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saRootSetUserTableIndex_(Root, toPtr - saRoots.UserTable + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saRoot *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedRootUserTable(toPtr - saRoots.UserTable);
    saSetFreeRootUserTable(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the Root.UserTable heap.
----------------------------------------------------------------------------------------*/
static void allocMoreRootUserTables(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedRootUserTable() - saUsedRootUserTable();

    if((saFreeRootUserTable() << 2) > saUsedRootUserTable()) {
        saCompactRootUserTables();
        freeSpace = saAllocatedRootUserTable() - saUsedRootUserTable();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedRootUserTable(saAllocatedRootUserTable() + spaceNeeded - freeSpace +
            (saAllocatedRootUserTable() >> 1));
        utResizeArray(saRoots.UserTable, saAllocatedRootUserTable());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new Root.UserTable array.
----------------------------------------------------------------------------------------*/
void saRootAllocUserTables(
    saRoot Root,
    uint32 numUserTables)
{
    uint32 freeSpace = saAllocatedRootUserTable() - saUsedRootUserTable();
    uint32 elementSize = sizeof(saUser);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numUserTables + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saRootGetNumUserTable(Root) == 0);
#endif
    if(numUserTables == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMoreRootUserTables(spaceNeeded);
    }
    saRootSetUserTableIndex_(Root, saUsedRootUserTable() + usedHeaderSize);
    saRootSetNumUserTable(Root, numUserTables);
    *(saRoot *)(void *)(saRoots.UserTable + saUsedRootUserTable()) = Root;
    {
        uint32 xValue;
        for(xValue = (uint32)(saRootGetUserTableIndex_(Root)); xValue < saRootGetUserTableIndex_(Root) + numUserTables; xValue++) {
            saRoots.UserTable[xValue] = saUserNull;
        }
    }
    saSetUsedRootUserTable(saUsedRootUserTable() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saRootGetUserTables for the database manager.
----------------------------------------------------------------------------------------*/
static void *getRootUserTables(
    uint64 objectNumber,
    uint32 *numValues)
{
    saRoot Root = saIndex2Root((uint32)objectNumber);

    *numValues = saRootGetNumUserTable(Root);
    return saRootGetUserTables(Root);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saRootAllocUserTables for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocRootUserTables(
    uint64 objectNumber,
    uint32 numValues)
{
    saRoot Root = saIndex2Root((uint32)objectNumber);

    saRootSetUserTableIndex_(Root, 0);
    saRootSetNumUserTable(Root, 0);
    if(numValues == 0) {
        return NULL;
    }
    saRootAllocUserTables(Root, numValues);
    return saRootGetUserTables(Root);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the Root.UserTable array.
----------------------------------------------------------------------------------------*/
void saRootFreeUserTables(
    saRoot Root)
{
    uint32 elementSize = sizeof(saUser);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saRootGetNumUserTable(Root) + usedHeaderSize, freeHeaderSize);
    saUser *dataPtr = saRootGetUserTables(Root) - usedHeaderSize;

    if(saRootGetNumUserTable(Root) == 0) {
        return;
    }
    *(saRoot *)(void *)(dataPtr) = saRootNull;
    *(uint32 *)(void *)(((saRoot *)(void *)dataPtr) + 1) = size;
    saRootSetNumUserTable(Root, 0);
    saSetFreeRootUserTable(saFreeRootUserTable() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the Root.UserTable array.
----------------------------------------------------------------------------------------*/
void saRootResizeUserTables(
    saRoot Root,
    uint32 numUserTables)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(saUser);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numUserTables + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saRootGetNumUserTable(Root) + usedHeaderSize, freeHeaderSize);
    saUser *dataPtr;

    if(numUserTables == 0) {
        if(saRootGetNumUserTable(Root) != 0) {
            saRootFreeUserTables(Root);
        }
        return;
    }
    if(saRootGetNumUserTable(Root) == 0) {
        saRootAllocUserTables(Root, numUserTables);
        return;
    }
    freeSpace = saAllocatedRootUserTable() - saUsedRootUserTable();
    if(freeSpace < newSize) {
        allocMoreRootUserTables(newSize);
    }
    dataPtr = saRootGetUserTables(Root) - usedHeaderSize;
    memcpy((void *)(saRoots.UserTable + saUsedRootUserTable()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        {
            uint32 xValue;
            for(xValue = (uint32)(saUsedRootUserTable() + oldSize); xValue < saUsedRootUserTable() + oldSize + newSize - oldSize; xValue++) {
                saRoots.UserTable[xValue] = saUserNull;
            }
        }
    }
    *(saRoot *)(void *)dataPtr = saRootNull;
    *(uint32 *)(void *)(((saRoot *)(void *)dataPtr) + 1) = oldSize;
    saSetFreeRootUserTable(saFreeRootUserTable() + oldSize);
    saRootSetUserTableIndex_(Root, saUsedRootUserTable() + usedHeaderSize);
    saRootSetNumUserTable(Root, numUserTables);
    saSetUsedRootUserTable(saUsedRootUserTable() + newSize);
}

/*----------------------------------------------------------------------------------------
  Compact the Root.ByEmailUserTable heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactRootByEmailUserTables(void)
{
    uint32 elementSize = sizeof(saUser);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    saUser *toPtr = saRoots.ByEmailUserTable;
    saUser *fromPtr = toPtr;
    saRoot Root;
    uint32 size;

    while(fromPtr < saRoots.ByEmailUserTable + saUsedRootByEmailUserTable()) {
        Root = *(saRoot *)(void *)fromPtr;
        if(Root != saRootNull) {
            /* Need to move it to toPtr */
            size = utMax(saRootGetNumByEmailUserTable(Root) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saRootSetByEmailUserTableIndex_(Root, toPtr - saRoots.ByEmailUserTable + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saRoot *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedRootByEmailUserTable(toPtr - saRoots.ByEmailUserTable);
    saSetFreeRootByEmailUserTable(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the Root.ByEmailUserTable heap.
----------------------------------------------------------------------------------------*/
static void allocMoreRootByEmailUserTables(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedRootByEmailUserTable() - saUsedRootByEmailUserTable();

    if((saFreeRootByEmailUserTable() << 2) > saUsedRootByEmailUserTable()) {
        saCompactRootByEmailUserTables();
        freeSpace = saAllocatedRootByEmailUserTable() - saUsedRootByEmailUserTable();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedRootByEmailUserTable(saAllocatedRootByEmailUserTable() + spaceNeeded - freeSpace +
            (saAllocatedRootByEmailUserTable() >> 1));
        utResizeArray(saRoots.ByEmailUserTable, saAllocatedRootByEmailUserTable());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new Root.ByEmailUserTable array.
----------------------------------------------------------------------------------------*/
void saRootAllocByEmailUserTables(
    saRoot Root,
    uint32 numByEmailUserTables)
{
    uint32 freeSpace = saAllocatedRootByEmailUserTable() - saUsedRootByEmailUserTable();
    uint32 elementSize = sizeof(saUser);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numByEmailUserTables + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saRootGetNumByEmailUserTable(Root) == 0);
#endif
    if(numByEmailUserTables == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMoreRootByEmailUserTables(spaceNeeded);
    }
    saRootSetByEmailUserTableIndex_(Root, saUsedRootByEmailUserTable() + usedHeaderSize);
    saRootSetNumByEmailUserTable(Root, numByEmailUserTables);
    *(saRoot *)(void *)(saRoots.ByEmailUserTable + saUsedRootByEmailUserTable()) = Root;
    {
        uint32 xValue;
        for(xValue = (uint32)(saRootGetByEmailUserTableIndex_(Root)); xValue < saRootGetByEmailUserTableIndex_(Root) + numByEmailUserTables; xValue++) {
            saRoots.ByEmailUserTable[xValue] = saUserNull;
        }
    }
    saSetUsedRootByEmailUserTable(saUsedRootByEmailUserTable() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saRootGetByEmailUserTables for the database manager.
----------------------------------------------------------------------------------------*/
static void *getRootByEmailUserTables(
    uint64 objectNumber,
    uint32 *numValues)
{
    saRoot Root = saIndex2Root((uint32)objectNumber);

    *numValues = saRootGetNumByEmailUserTable(Root);
    return saRootGetByEmailUserTables(Root);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saRootAllocByEmailUserTables for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocRootByEmailUserTables(
    uint64 objectNumber,
    uint32 numValues)
{
    saRoot Root = saIndex2Root((uint32)objectNumber);

    saRootSetByEmailUserTableIndex_(Root, 0);
    saRootSetNumByEmailUserTable(Root, 0);
    if(numValues == 0) {
        return NULL;
    }
    saRootAllocByEmailUserTables(Root, numValues);
    return saRootGetByEmailUserTables(Root);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the Root.ByEmailUserTable array.
----------------------------------------------------------------------------------------*/
void saRootFreeByEmailUserTables(
    saRoot Root)
{
    uint32 elementSize = sizeof(saUser);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saRootGetNumByEmailUserTable(Root) + usedHeaderSize, freeHeaderSize);
    saUser *dataPtr = saRootGetByEmailUserTables(Root) - usedHeaderSize;

    if(saRootGetNumByEmailUserTable(Root) == 0) {
        return;
    }
    *(saRoot *)(void *)(dataPtr) = saRootNull;
    *(uint32 *)(void *)(((saRoot *)(void *)dataPtr) + 1) = size;
    saRootSetNumByEmailUserTable(Root, 0);
    saSetFreeRootByEmailUserTable(saFreeRootByEmailUserTable() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the Root.ByEmailUserTable array.
----------------------------------------------------------------------------------------*/
void saRootResizeByEmailUserTables(
    saRoot Root,
    uint32 numByEmailUserTables)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(saUser);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numByEmailUserTables + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saRootGetNumByEmailUserTable(Root) + usedHeaderSize, freeHeaderSize);
    saUser *dataPtr;

    if(numByEmailUserTables == 0) {
        if(saRootGetNumByEmailUserTable(Root) != 0) {
            saRootFreeByEmailUserTables(Root);
        }
        return;
    }
    if(saRootGetNumByEmailUserTable(Root) == 0) {
        saRootAllocByEmailUserTables(Root, numByEmailUserTables);
        return;
    }
    freeSpace = saAllocatedRootByEmailUserTable() - saUsedRootByEmailUserTable();
    if(freeSpace < newSize) {
        allocMoreRootByEmailUserTables(newSize);
    }
    dataPtr = saRootGetByEmailUserTables(Root) - usedHeaderSize;
    memcpy((void *)(saRoots.ByEmailUserTable + saUsedRootByEmailUserTable()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        {
            uint32 xValue;
            for(xValue = (uint32)(saUsedRootByEmailUserTable() + oldSize); xValue < saUsedRootByEmailUserTable() + oldSize + newSize - oldSize; xValue++) {
                saRoots.ByEmailUserTable[xValue] = saUserNull;
            }
        }
    }
    *(saRoot *)(void *)dataPtr = saRootNull;
    *(uint32 *)(void *)(((saRoot *)(void *)dataPtr) + 1) = oldSize;
    saSetFreeRootByEmailUserTable(saFreeRootByEmailUserTable() + oldSize);
    saRootSetByEmailUserTableIndex_(Root, saUsedRootByEmailUserTable() + usedHeaderSize);
    saRootSetNumByEmailUserTable(Root, numByEmailUserTables);
    saSetUsedRootByEmailUserTable(saUsedRootByEmailUserTable() + newSize);
}

/*----------------------------------------------------------------------------------------
  Compact the Root.CategoryTable heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactRootCategoryTables(void)
{
    uint32 elementSize = sizeof(saCategory);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    saCategory *toPtr = saRoots.CategoryTable;
    saCategory *fromPtr = toPtr;
    saRoot Root;
    uint32 size;

    while(fromPtr < saRoots.CategoryTable + saUsedRootCategoryTable()) {
        Root = *(saRoot *)(void *)fromPtr;
        if(Root != saRootNull) {
            /* Need to move it to toPtr */
            size = utMax(saRootGetNumCategoryTable(Root) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saRootSetCategoryTableIndex_(Root, toPtr - saRoots.CategoryTable + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saRoot *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedRootCategoryTable(toPtr - saRoots.CategoryTable);
    saSetFreeRootCategoryTable(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the Root.CategoryTable heap.
----------------------------------------------------------------------------------------*/
static void allocMoreRootCategoryTables(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedRootCategoryTable() - saUsedRootCategoryTable();

    if((saFreeRootCategoryTable() << 2) > saUsedRootCategoryTable()) {
        saCompactRootCategoryTables();
        freeSpace = saAllocatedRootCategoryTable() - saUsedRootCategoryTable();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedRootCategoryTable(saAllocatedRootCategoryTable() + spaceNeeded - freeSpace +
            (saAllocatedRootCategoryTable() >> 1));
        utResizeArray(saRoots.CategoryTable, saAllocatedRootCategoryTable());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new Root.CategoryTable array.
----------------------------------------------------------------------------------------*/
void saRootAllocCategoryTables(
    saRoot Root,
    uint32 numCategoryTables)
{
    uint32 freeSpace = saAllocatedRootCategoryTable() - saUsedRootCategoryTable();
    uint32 elementSize = sizeof(saCategory);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numCategoryTables + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saRootGetNumCategoryTable(Root) == 0);
#endif
    if(numCategoryTables == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMoreRootCategoryTables(spaceNeeded);
    }
    saRootSetCategoryTableIndex_(Root, saUsedRootCategoryTable() + usedHeaderSize);
    saRootSetNumCategoryTable(Root, numCategoryTables);
    *(saRoot *)(void *)(saRoots.CategoryTable + saUsedRootCategoryTable()) = Root;
    {
        uint32 xValue;
        for(xValue = (uint32)(saRootGetCategoryTableIndex_(Root)); xValue < saRootGetCategoryTableIndex_(Root) + numCategoryTables; xValue++) {
            saRoots.CategoryTable[xValue] = saCategoryNull;
        }
    }
    saSetUsedRootCategoryTable(saUsedRootCategoryTable() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saRootGetCategoryTables for the database manager.
----------------------------------------------------------------------------------------*/
static void *getRootCategoryTables(
    uint64 objectNumber,
    uint32 *numValues)
{
    saRoot Root = saIndex2Root((uint32)objectNumber);

    *numValues = saRootGetNumCategoryTable(Root);
    return saRootGetCategoryTables(Root);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saRootAllocCategoryTables for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocRootCategoryTables(
    uint64 objectNumber,
    uint32 numValues)
{
    saRoot Root = saIndex2Root((uint32)objectNumber);

    saRootSetCategoryTableIndex_(Root, 0);
    saRootSetNumCategoryTable(Root, 0);
    if(numValues == 0) {
        return NULL;
    }
    saRootAllocCategoryTables(Root, numValues);
    return saRootGetCategoryTables(Root);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the Root.CategoryTable array.
----------------------------------------------------------------------------------------*/
void saRootFreeCategoryTables(
    saRoot Root)
{
    uint32 elementSize = sizeof(saCategory);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saRootGetNumCategoryTable(Root) + usedHeaderSize, freeHeaderSize);
    saCategory *dataPtr = saRootGetCategoryTables(Root) - usedHeaderSize;

    if(saRootGetNumCategoryTable(Root) == 0) {
        return;
    }
    *(saRoot *)(void *)(dataPtr) = saRootNull;
    *(uint32 *)(void *)(((saRoot *)(void *)dataPtr) + 1) = size;
    saRootSetNumCategoryTable(Root, 0);
    saSetFreeRootCategoryTable(saFreeRootCategoryTable() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the Root.CategoryTable array.
----------------------------------------------------------------------------------------*/
void saRootResizeCategoryTables(
    saRoot Root,
    uint32 numCategoryTables)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(saCategory);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numCategoryTables + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saRootGetNumCategoryTable(Root) + usedHeaderSize, freeHeaderSize);
    saCategory *dataPtr;

    if(numCategoryTables == 0) {
        if(saRootGetNumCategoryTable(Root) != 0) {
            saRootFreeCategoryTables(Root);
        }
        return;
    }
    if(saRootGetNumCategoryTable(Root) == 0) {
        saRootAllocCategoryTables(Root, numCategoryTables);
        return;
    }
    freeSpace = saAllocatedRootCategoryTable() - saUsedRootCategoryTable();
    if(freeSpace < newSize) {
        allocMoreRootCategoryTables(newSize);
    }
    dataPtr = saRootGetCategoryTables(Root) - usedHeaderSize;
    memcpy((void *)(saRoots.CategoryTable + saUsedRootCategoryTable()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        {
            uint32 xValue;
            for(xValue = (uint32)(saUsedRootCategoryTable() + oldSize); xValue < saUsedRootCategoryTable() + oldSize + newSize - oldSize; xValue++) {
                saRoots.CategoryTable[xValue] = saCategoryNull;
            }
        }
    }
    *(saRoot *)(void *)dataPtr = saRootNull;
    *(uint32 *)(void *)(((saRoot *)(void *)dataPtr) + 1) = oldSize;
    saSetFreeRootCategoryTable(saFreeRootCategoryTable() + oldSize);
    saRootSetCategoryTableIndex_(Root, saUsedRootCategoryTable() + usedHeaderSize);
    saRootSetNumCategoryTable(Root, numCategoryTables);
    saSetUsedRootCategoryTable(saUsedRootCategoryTable() + newSize);
}

/*----------------------------------------------------------------------------------------
  Compact the Root.ListingTable heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactRootListingTables(void)
{
    uint32 elementSize = sizeof(saListing);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    saListing *toPtr = saRoots.ListingTable;
    saListing *fromPtr = toPtr;
    saRoot Root;
    uint32 size;

    while(fromPtr < saRoots.ListingTable + saUsedRootListingTable()) {
        Root = *(saRoot *)(void *)fromPtr;
        if(Root != saRootNull) {
            /* Need to move it to toPtr */
            size = utMax(saRootGetNumListingTable(Root) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saRootSetListingTableIndex_(Root, toPtr - saRoots.ListingTable + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saRoot *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedRootListingTable(toPtr - saRoots.ListingTable);
    saSetFreeRootListingTable(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the Root.ListingTable heap.
----------------------------------------------------------------------------------------*/
static void allocMoreRootListingTables(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedRootListingTable() - saUsedRootListingTable();

    if((saFreeRootListingTable() << 2) > saUsedRootListingTable()) {
        saCompactRootListingTables();
        freeSpace = saAllocatedRootListingTable() - saUsedRootListingTable();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedRootListingTable(saAllocatedRootListingTable() + spaceNeeded - freeSpace +
            (saAllocatedRootListingTable() >> 1));
        utResizeArray(saRoots.ListingTable, saAllocatedRootListingTable());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new Root.ListingTable array.
----------------------------------------------------------------------------------------*/
void saRootAllocListingTables(
    saRoot Root,
    uint32 numListingTables)
{
    uint32 freeSpace = saAllocatedRootListingTable() - saUsedRootListingTable();
    uint32 elementSize = sizeof(saListing);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numListingTables + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saRootGetNumListingTable(Root) == 0);
#endif
    if(numListingTables == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMoreRootListingTables(spaceNeeded);
    }
    saRootSetListingTableIndex_(Root, saUsedRootListingTable() + usedHeaderSize);
    saRootSetNumListingTable(Root, numListingTables);
    *(saRoot *)(void *)(saRoots.ListingTable + saUsedRootListingTable()) = Root;
    {
        uint32 xValue;
        for(xValue = (uint32)(saRootGetListingTableIndex_(Root)); xValue < saRootGetListingTableIndex_(Root) + numListingTables; xValue++) {
            saRoots.ListingTable[xValue] = saListingNull;
        }
    }
    saSetUsedRootListingTable(saUsedRootListingTable() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saRootGetListingTables for the database manager.
----------------------------------------------------------------------------------------*/
static void *getRootListingTables(
    uint64 objectNumber,
    uint32 *numValues)
{
    saRoot Root = saIndex2Root((uint32)objectNumber);

    *numValues = saRootGetNumListingTable(Root);
    return saRootGetListingTables(Root);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saRootAllocListingTables for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocRootListingTables(
    uint64 objectNumber,
    uint32 numValues)
{
    saRoot Root = saIndex2Root((uint32)objectNumber);

    saRootSetListingTableIndex_(Root, 0);
    saRootSetNumListingTable(Root, 0);
    if(numValues == 0) {
        return NULL;
    }
    saRootAllocListingTables(Root, numValues);
    return saRootGetListingTables(Root);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the Root.ListingTable array.
----------------------------------------------------------------------------------------*/
void saRootFreeListingTables(
    saRoot Root)
{
    uint32 elementSize = sizeof(saListing);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saRootGetNumListingTable(Root) + usedHeaderSize, freeHeaderSize);
    saListing *dataPtr = saRootGetListingTables(Root) - usedHeaderSize;

    if(saRootGetNumListingTable(Root) == 0) {
        return;
    }
    *(saRoot *)(void *)(dataPtr) = saRootNull;
    *(uint32 *)(void *)(((saRoot *)(void *)dataPtr) + 1) = size;
    saRootSetNumListingTable(Root, 0);
    saSetFreeRootListingTable(saFreeRootListingTable() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the Root.ListingTable array.
----------------------------------------------------------------------------------------*/
void saRootResizeListingTables(
    saRoot Root,
    uint32 numListingTables)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(saListing);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numListingTables + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saRootGetNumListingTable(Root) + usedHeaderSize, freeHeaderSize);
    saListing *dataPtr;

    if(numListingTables == 0) {
        if(saRootGetNumListingTable(Root) != 0) {
            saRootFreeListingTables(Root);
        }
        return;
    }
    if(saRootGetNumListingTable(Root) == 0) {
        saRootAllocListingTables(Root, numListingTables);
        return;
    }
    freeSpace = saAllocatedRootListingTable() - saUsedRootListingTable();
    if(freeSpace < newSize) {
        allocMoreRootListingTables(newSize);
    }
    dataPtr = saRootGetListingTables(Root) - usedHeaderSize;
    memcpy((void *)(saRoots.ListingTable + saUsedRootListingTable()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        {
            uint32 xValue;
            for(xValue = (uint32)(saUsedRootListingTable() + oldSize); xValue < saUsedRootListingTable() + oldSize + newSize - oldSize; xValue++) {
                saRoots.ListingTable[xValue] = saListingNull;
            }
        }
    }
    *(saRoot *)(void *)dataPtr = saRootNull;
    *(uint32 *)(void *)(((saRoot *)(void *)dataPtr) + 1) = oldSize;
    saSetFreeRootListingTable(saFreeRootListingTable() + oldSize);
    saRootSetListingTableIndex_(Root, saUsedRootListingTable() + usedHeaderSize);
    saRootSetNumListingTable(Root, numListingTables);
    saSetUsedRootListingTable(saUsedRootListingTable() + newSize);
}

/*----------------------------------------------------------------------------------------
  Compact the Root.SessionTable heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactRootSessionTables(void)
{
    uint32 elementSize = sizeof(saSession);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    saSession *toPtr = saRoots.SessionTable;
    saSession *fromPtr = toPtr;
    saRoot Root;
    uint32 size;

    while(fromPtr < saRoots.SessionTable + saUsedRootSessionTable()) {
        Root = *(saRoot *)(void *)fromPtr;
        if(Root != saRootNull) {
            /* Need to move it to toPtr */
            size = utMax(saRootGetNumSessionTable(Root) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saRootSetSessionTableIndex_(Root, toPtr - saRoots.SessionTable + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saRoot *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedRootSessionTable(toPtr - saRoots.SessionTable);
    saSetFreeRootSessionTable(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the Root.SessionTable heap.
----------------------------------------------------------------------------------------*/
static void allocMoreRootSessionTables(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedRootSessionTable() - saUsedRootSessionTable();

    if((saFreeRootSessionTable() << 2) > saUsedRootSessionTable()) {
        saCompactRootSessionTables();
        freeSpace = saAllocatedRootSessionTable() - saUsedRootSessionTable();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedRootSessionTable(saAllocatedRootSessionTable() + spaceNeeded - freeSpace +
            (saAllocatedRootSessionTable() >> 1));
        utResizeArray(saRoots.SessionTable, saAllocatedRootSessionTable());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new Root.SessionTable array.
----------------------------------------------------------------------------------------*/
void saRootAllocSessionTables(
    saRoot Root,
    uint32 numSessionTables)
{
    uint32 freeSpace = saAllocatedRootSessionTable() - saUsedRootSessionTable();
    uint32 elementSize = sizeof(saSession);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numSessionTables + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saRootGetNumSessionTable(Root) == 0);
#endif
    if(numSessionTables == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMoreRootSessionTables(spaceNeeded);
    }
    saRootSetSessionTableIndex_(Root, saUsedRootSessionTable() + usedHeaderSize);
    saRootSetNumSessionTable(Root, numSessionTables);
    *(saRoot *)(void *)(saRoots.SessionTable + saUsedRootSessionTable()) = Root;
    {
        uint32 xValue;
        for(xValue = (uint32)(saRootGetSessionTableIndex_(Root)); xValue < saRootGetSessionTableIndex_(Root) + numSessionTables; xValue++) {
            saRoots.SessionTable[xValue] = saSessionNull;
        }
    }
    saSetUsedRootSessionTable(saUsedRootSessionTable() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saRootGetSessionTables for the database manager.
----------------------------------------------------------------------------------------*/
static void *getRootSessionTables(
    uint64 objectNumber,
    uint32 *numValues)
{
    saRoot Root = saIndex2Root((uint32)objectNumber);

    *numValues = saRootGetNumSessionTable(Root);
    return saRootGetSessionTables(Root);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saRootAllocSessionTables for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocRootSessionTables(
    uint64 objectNumber,
    uint32 numValues)
{
    saRoot Root = saIndex2Root((uint32)objectNumber);

    saRootSetSessionTableIndex_(Root, 0);
    saRootSetNumSessionTable(Root, 0);
    if(numValues == 0) {
        return NULL;
    }
    saRootAllocSessionTables(Root, numValues);
    return saRootGetSessionTables(Root);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the Root.SessionTable array.
----------------------------------------------------------------------------------------*/
void saRootFreeSessionTables(
    saRoot Root)
{
    uint32 elementSize = sizeof(saSession);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saRootGetNumSessionTable(Root) + usedHeaderSize, freeHeaderSize);
    saSession *dataPtr = saRootGetSessionTables(Root) - usedHeaderSize;

    if(saRootGetNumSessionTable(Root) == 0) {
        return;
    }
    *(saRoot *)(void *)(dataPtr) = saRootNull;
    *(uint32 *)(void *)(((saRoot *)(void *)dataPtr) + 1) = size;
    saRootSetNumSessionTable(Root, 0);
    saSetFreeRootSessionTable(saFreeRootSessionTable() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the Root.SessionTable array.
----------------------------------------------------------------------------------------*/
void saRootResizeSessionTables(
    saRoot Root,
    uint32 numSessionTables)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(saSession);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numSessionTables + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saRootGetNumSessionTable(Root) + usedHeaderSize, freeHeaderSize);
    saSession *dataPtr;

    if(numSessionTables == 0) {
        if(saRootGetNumSessionTable(Root) != 0) {
            saRootFreeSessionTables(Root);
        }
        return;
    }
    if(saRootGetNumSessionTable(Root) == 0) {
        saRootAllocSessionTables(Root, numSessionTables);
        return;
    }
    freeSpace = saAllocatedRootSessionTable() - saUsedRootSessionTable();
    if(freeSpace < newSize) {
        allocMoreRootSessionTables(newSize);
    }
    dataPtr = saRootGetSessionTables(Root) - usedHeaderSize;
    memcpy((void *)(saRoots.SessionTable + saUsedRootSessionTable()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        {
            uint32 xValue;
            for(xValue = (uint32)(saUsedRootSessionTable() + oldSize); xValue < saUsedRootSessionTable() + oldSize + newSize - oldSize; xValue++) {
                saRoots.SessionTable[xValue] = saSessionNull;
            }
        }
    }
    *(saRoot *)(void *)dataPtr = saRootNull;
    *(uint32 *)(void *)(((saRoot *)(void *)dataPtr) + 1) = oldSize;
    saSetFreeRootSessionTable(saFreeRootSessionTable() + oldSize);
    saRootSetSessionTableIndex_(Root, saUsedRootSessionTable() + usedHeaderSize);
    saRootSetNumSessionTable(Root, numSessionTables);
    saSetUsedRootSessionTable(saUsedRootSessionTable() + newSize);
}

/*----------------------------------------------------------------------------------------
  Compact the Root.ThreadTable heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactRootThreadTables(void)
{
    uint32 elementSize = sizeof(saThread);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    saThread *toPtr = saRoots.ThreadTable;
    saThread *fromPtr = toPtr;
    saRoot Root;
    uint32 size;

    while(fromPtr < saRoots.ThreadTable + saUsedRootThreadTable()) {
        Root = *(saRoot *)(void *)fromPtr;
        if(Root != saRootNull) {
            /* Need to move it to toPtr */
            size = utMax(saRootGetNumThreadTable(Root) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saRootSetThreadTableIndex_(Root, toPtr - saRoots.ThreadTable + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saRoot *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedRootThreadTable(toPtr - saRoots.ThreadTable);
    saSetFreeRootThreadTable(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the Root.ThreadTable heap.
----------------------------------------------------------------------------------------*/
static void allocMoreRootThreadTables(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedRootThreadTable() - saUsedRootThreadTable();

    if((saFreeRootThreadTable() << 2) > saUsedRootThreadTable()) {
        saCompactRootThreadTables();
        freeSpace = saAllocatedRootThreadTable() - saUsedRootThreadTable();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedRootThreadTable(saAllocatedRootThreadTable() + spaceNeeded - freeSpace +
            (saAllocatedRootThreadTable() >> 1));
        utResizeArray(saRoots.ThreadTable, saAllocatedRootThreadTable());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new Root.ThreadTable array.
----------------------------------------------------------------------------------------*/
void saRootAllocThreadTables(
    saRoot Root,
    uint32 numThreadTables)
{
    uint32 freeSpace = saAllocatedRootThreadTable() - saUsedRootThreadTable();
    uint32 elementSize = sizeof(saThread);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numThreadTables + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saRootGetNumThreadTable(Root) == 0);
#endif
    if(numThreadTables == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMoreRootThreadTables(spaceNeeded);
    }
    saRootSetThreadTableIndex_(Root, saUsedRootThreadTable() + usedHeaderSize);
    saRootSetNumThreadTable(Root, numThreadTables);
    *(saRoot *)(void *)(saRoots.ThreadTable + saUsedRootThreadTable()) = Root;
    {
        uint32 xValue;
        for(xValue = (uint32)(saRootGetThreadTableIndex_(Root)); xValue < saRootGetThreadTableIndex_(Root) + numThreadTables; xValue++) {
            saRoots.ThreadTable[xValue] = saThreadNull;
        }
    }
    saSetUsedRootThreadTable(saUsedRootThreadTable() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saRootGetThreadTables for the database manager.
----------------------------------------------------------------------------------------*/
static void *getRootThreadTables(
    uint64 objectNumber,
    uint32 *numValues)
{
    saRoot Root = saIndex2Root((uint32)objectNumber);

    *numValues = saRootGetNumThreadTable(Root);
    return saRootGetThreadTables(Root);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saRootAllocThreadTables for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocRootThreadTables(
    uint64 objectNumber,
    uint32 numValues)
{
    saRoot Root = saIndex2Root((uint32)objectNumber);

    saRootSetThreadTableIndex_(Root, 0);
    saRootSetNumThreadTable(Root, 0);
    if(numValues == 0) {
        return NULL;
    }
    saRootAllocThreadTables(Root, numValues);
    return saRootGetThreadTables(Root);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the Root.ThreadTable array.
----------------------------------------------------------------------------------------*/
void saRootFreeThreadTables(
    saRoot Root)
{
    uint32 elementSize = sizeof(saThread);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saRootGetNumThreadTable(Root) + usedHeaderSize, freeHeaderSize);
    saThread *dataPtr = saRootGetThreadTables(Root) - usedHeaderSize;

    if(saRootGetNumThreadTable(Root) == 0) {
        return;
    }
    *(saRoot *)(void *)(dataPtr) = saRootNull;
    *(uint32 *)(void *)(((saRoot *)(void *)dataPtr) + 1) = size;
    saRootSetNumThreadTable(Root, 0);
    saSetFreeRootThreadTable(saFreeRootThreadTable() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the Root.ThreadTable array.
----------------------------------------------------------------------------------------*/
void saRootResizeThreadTables(
    saRoot Root,
    uint32 numThreadTables)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(saThread);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numThreadTables + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saRootGetNumThreadTable(Root) + usedHeaderSize, freeHeaderSize);
    saThread *dataPtr;

    if(numThreadTables == 0) {
        if(saRootGetNumThreadTable(Root) != 0) {
            saRootFreeThreadTables(Root);
        }
        return;
    }
    if(saRootGetNumThreadTable(Root) == 0) {
        saRootAllocThreadTables(Root, numThreadTables);
        return;
    }
    freeSpace = saAllocatedRootThreadTable() - saUsedRootThreadTable();
    if(freeSpace < newSize) {
        allocMoreRootThreadTables(newSize);
    }
    dataPtr = saRootGetThreadTables(Root) - usedHeaderSize;
    memcpy((void *)(saRoots.ThreadTable + saUsedRootThreadTable()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        {
            uint32 xValue;
            for(xValue = (uint32)(saUsedRootThreadTable() + oldSize); xValue < saUsedRootThreadTable() + oldSize + newSize - oldSize; xValue++) {
                saRoots.ThreadTable[xValue] = saThreadNull;
            }
        }
    }
    *(saRoot *)(void *)dataPtr = saRootNull;
    *(uint32 *)(void *)(((saRoot *)(void *)dataPtr) + 1) = oldSize;
    saSetFreeRootThreadTable(saFreeRootThreadTable() + oldSize);
    saRootSetThreadTableIndex_(Root, saUsedRootThreadTable() + usedHeaderSize);
    saRootSetNumThreadTable(Root, numThreadTables);
    saSetUsedRootThreadTable(saUsedRootThreadTable() + newSize);
}

/*----------------------------------------------------------------------------------------
  Compact the Root.PostTable heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactRootPostTables(void)
{
    uint32 elementSize = sizeof(saPost);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    saPost *toPtr = saRoots.PostTable;
    saPost *fromPtr = toPtr;
    saRoot Root;
    uint32 size;

    while(fromPtr < saRoots.PostTable + saUsedRootPostTable()) {
        Root = *(saRoot *)(void *)fromPtr;
        if(Root != saRootNull) {
            /* Need to move it to toPtr */
            size = utMax(saRootGetNumPostTable(Root) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saRootSetPostTableIndex_(Root, toPtr - saRoots.PostTable + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saRoot *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedRootPostTable(toPtr - saRoots.PostTable);
    saSetFreeRootPostTable(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the Root.PostTable heap.
----------------------------------------------------------------------------------------*/
static void allocMoreRootPostTables(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedRootPostTable() - saUsedRootPostTable();

    if((saFreeRootPostTable() << 2) > saUsedRootPostTable()) {
        saCompactRootPostTables();
        freeSpace = saAllocatedRootPostTable() - saUsedRootPostTable();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedRootPostTable(saAllocatedRootPostTable() + spaceNeeded - freeSpace +
            (saAllocatedRootPostTable() >> 1));
        utResizeArray(saRoots.PostTable, saAllocatedRootPostTable());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new Root.PostTable array.
----------------------------------------------------------------------------------------*/
void saRootAllocPostTables(
    saRoot Root,
    uint32 numPostTables)
{
    uint32 freeSpace = saAllocatedRootPostTable() - saUsedRootPostTable();
    uint32 elementSize = sizeof(saPost);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numPostTables + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saRootGetNumPostTable(Root) == 0);
#endif
    if(numPostTables == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMoreRootPostTables(spaceNeeded);
    }
    saRootSetPostTableIndex_(Root, saUsedRootPostTable() + usedHeaderSize);
    saRootSetNumPostTable(Root, numPostTables);
    *(saRoot *)(void *)(saRoots.PostTable + saUsedRootPostTable()) = Root;
    {
        uint32 xValue;
        for(xValue = (uint32)(saRootGetPostTableIndex_(Root)); xValue < saRootGetPostTableIndex_(Root) + numPostTables; xValue++) {
            saRoots.PostTable[xValue] = saPostNull;
        }
    }
    saSetUsedRootPostTable(saUsedRootPostTable() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saRootGetPostTables for the database manager.
----------------------------------------------------------------------------------------*/
static void *getRootPostTables(
    uint64 objectNumber,
    uint32 *numValues)
{
    saRoot Root = saIndex2Root((uint32)objectNumber);

    *numValues = saRootGetNumPostTable(Root);
    return saRootGetPostTables(Root);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saRootAllocPostTables for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocRootPostTables(
    uint64 objectNumber,
    uint32 numValues)
{
    saRoot Root = saIndex2Root((uint32)objectNumber);

    saRootSetPostTableIndex_(Root, 0);
    saRootSetNumPostTable(Root, 0);
    if(numValues == 0) {
        return NULL;
    }
    saRootAllocPostTables(Root, numValues);
    return saRootGetPostTables(Root);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the Root.PostTable array.
----------------------------------------------------------------------------------------*/
void saRootFreePostTables(
    saRoot Root)
{
    uint32 elementSize = sizeof(saPost);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saRootGetNumPostTable(Root) + usedHeaderSize, freeHeaderSize);
    saPost *dataPtr = saRootGetPostTables(Root) - usedHeaderSize;

    if(saRootGetNumPostTable(Root) == 0) {
        return;
    }
    *(saRoot *)(void *)(dataPtr) = saRootNull;
    *(uint32 *)(void *)(((saRoot *)(void *)dataPtr) + 1) = size;
    saRootSetNumPostTable(Root, 0);
    saSetFreeRootPostTable(saFreeRootPostTable() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the Root.PostTable array.
----------------------------------------------------------------------------------------*/
void saRootResizePostTables(
    saRoot Root,
    uint32 numPostTables)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(saPost);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numPostTables + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saRootGetNumPostTable(Root) + usedHeaderSize, freeHeaderSize);
    saPost *dataPtr;

    if(numPostTables == 0) {
        if(saRootGetNumPostTable(Root) != 0) {
            saRootFreePostTables(Root);
        }
        return;
    }
    if(saRootGetNumPostTable(Root) == 0) {
        saRootAllocPostTables(Root, numPostTables);
        return;
    }
    freeSpace = saAllocatedRootPostTable() - saUsedRootPostTable();
    if(freeSpace < newSize) {
        allocMoreRootPostTables(newSize);
    }
    dataPtr = saRootGetPostTables(Root) - usedHeaderSize;
    memcpy((void *)(saRoots.PostTable + saUsedRootPostTable()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        {
            uint32 xValue;
            for(xValue = (uint32)(saUsedRootPostTable() + oldSize); xValue < saUsedRootPostTable() + oldSize + newSize - oldSize; xValue++) {
                saRoots.PostTable[xValue] = saPostNull;
            }
        }
    }
    *(saRoot *)(void *)dataPtr = saRootNull;
    *(uint32 *)(void *)(((saRoot *)(void *)dataPtr) + 1) = oldSize;
    saSetFreeRootPostTable(saFreeRootPostTable() + oldSize);
    saRootSetPostTableIndex_(Root, saUsedRootPostTable() + usedHeaderSize);
    saRootSetNumPostTable(Root, numPostTables);
    saSetUsedRootPostTable(saUsedRootPostTable() + newSize);
}

/*----------------------------------------------------------------------------------------
  Compact the Root.AnnouncementTable heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactRootAnnouncementTables(void)
{
    uint32 elementSize = sizeof(saAnnouncement);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    saAnnouncement *toPtr = saRoots.AnnouncementTable;
    saAnnouncement *fromPtr = toPtr;
    saRoot Root;
    uint32 size;

    while(fromPtr < saRoots.AnnouncementTable + saUsedRootAnnouncementTable()) {
        Root = *(saRoot *)(void *)fromPtr;
        if(Root != saRootNull) {
            /* Need to move it to toPtr */
            size = utMax(saRootGetNumAnnouncementTable(Root) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saRootSetAnnouncementTableIndex_(Root, toPtr - saRoots.AnnouncementTable + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saRoot *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedRootAnnouncementTable(toPtr - saRoots.AnnouncementTable);
    saSetFreeRootAnnouncementTable(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the Root.AnnouncementTable heap.
----------------------------------------------------------------------------------------*/
static void allocMoreRootAnnouncementTables(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedRootAnnouncementTable() - saUsedRootAnnouncementTable();

    if((saFreeRootAnnouncementTable() << 2) > saUsedRootAnnouncementTable()) {
        saCompactRootAnnouncementTables();
        freeSpace = saAllocatedRootAnnouncementTable() - saUsedRootAnnouncementTable();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedRootAnnouncementTable(saAllocatedRootAnnouncementTable() + spaceNeeded - freeSpace +
            (saAllocatedRootAnnouncementTable() >> 1));
        utResizeArray(saRoots.AnnouncementTable, saAllocatedRootAnnouncementTable());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new Root.AnnouncementTable array.
----------------------------------------------------------------------------------------*/
void saRootAllocAnnouncementTables(
    saRoot Root,
    uint32 numAnnouncementTables)
{
    uint32 freeSpace = saAllocatedRootAnnouncementTable() - saUsedRootAnnouncementTable();
    uint32 elementSize = sizeof(saAnnouncement);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numAnnouncementTables + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saRootGetNumAnnouncementTable(Root) == 0);
#endif
    if(numAnnouncementTables == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMoreRootAnnouncementTables(spaceNeeded);
    }
    saRootSetAnnouncementTableIndex_(Root, saUsedRootAnnouncementTable() + usedHeaderSize);
    saRootSetNumAnnouncementTable(Root, numAnnouncementTables);
    *(saRoot *)(void *)(saRoots.AnnouncementTable + saUsedRootAnnouncementTable()) = Root;
    {
        uint32 xValue;
        for(xValue = (uint32)(saRootGetAnnouncementTableIndex_(Root)); xValue < saRootGetAnnouncementTableIndex_(Root) + numAnnouncementTables; xValue++) {
            saRoots.AnnouncementTable[xValue] = saAnnouncementNull;
        }
    }
    saSetUsedRootAnnouncementTable(saUsedRootAnnouncementTable() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saRootGetAnnouncementTables for the database manager.
----------------------------------------------------------------------------------------*/
static void *getRootAnnouncementTables(
    uint64 objectNumber,
    uint32 *numValues)
{
    saRoot Root = saIndex2Root((uint32)objectNumber);

    *numValues = saRootGetNumAnnouncementTable(Root);
    return saRootGetAnnouncementTables(Root);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saRootAllocAnnouncementTables for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocRootAnnouncementTables(
    uint64 objectNumber,
    uint32 numValues)
{
    saRoot Root = saIndex2Root((uint32)objectNumber);

    saRootSetAnnouncementTableIndex_(Root, 0);
    saRootSetNumAnnouncementTable(Root, 0);
    if(numValues == 0) {
        return NULL;
    }
    saRootAllocAnnouncementTables(Root, numValues);
    return saRootGetAnnouncementTables(Root);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the Root.AnnouncementTable array.
----------------------------------------------------------------------------------------*/
void saRootFreeAnnouncementTables(
    saRoot Root)
{
    uint32 elementSize = sizeof(saAnnouncement);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saRootGetNumAnnouncementTable(Root) + usedHeaderSize, freeHeaderSize);
    saAnnouncement *dataPtr = saRootGetAnnouncementTables(Root) - usedHeaderSize;

    if(saRootGetNumAnnouncementTable(Root) == 0) {
        return;
    }
    *(saRoot *)(void *)(dataPtr) = saRootNull;
    *(uint32 *)(void *)(((saRoot *)(void *)dataPtr) + 1) = size;
    saRootSetNumAnnouncementTable(Root, 0);
    saSetFreeRootAnnouncementTable(saFreeRootAnnouncementTable() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the Root.AnnouncementTable array.
----------------------------------------------------------------------------------------*/
void saRootResizeAnnouncementTables(
    saRoot Root,
    uint32 numAnnouncementTables)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(saAnnouncement);
    uint32 usedHeaderSize = (sizeof(saRoot) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saRoot) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numAnnouncementTables + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saRootGetNumAnnouncementTable(Root) + usedHeaderSize, freeHeaderSize);
    saAnnouncement *dataPtr;

    if(numAnnouncementTables == 0) {
        if(saRootGetNumAnnouncementTable(Root) != 0) {
            saRootFreeAnnouncementTables(Root);
        }
        return;
    }
    if(saRootGetNumAnnouncementTable(Root) == 0) {
        saRootAllocAnnouncementTables(Root, numAnnouncementTables);
        return;
    }
    freeSpace = saAllocatedRootAnnouncementTable() - saUsedRootAnnouncementTable();
    if(freeSpace < newSize) {
        allocMoreRootAnnouncementTables(newSize);
    }
    dataPtr = saRootGetAnnouncementTables(Root) - usedHeaderSize;
    memcpy((void *)(saRoots.AnnouncementTable + saUsedRootAnnouncementTable()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        {
            uint32 xValue;
            for(xValue = (uint32)(saUsedRootAnnouncementTable() + oldSize); xValue < saUsedRootAnnouncementTable() + oldSize + newSize - oldSize; xValue++) {
                saRoots.AnnouncementTable[xValue] = saAnnouncementNull;
            }
        }
    }
    *(saRoot *)(void *)dataPtr = saRootNull;
    *(uint32 *)(void *)(((saRoot *)(void *)dataPtr) + 1) = oldSize;
    saSetFreeRootAnnouncementTable(saFreeRootAnnouncementTable() + oldSize);
    saRootSetAnnouncementTableIndex_(Root, saUsedRootAnnouncementTable() + usedHeaderSize);
    saRootSetNumAnnouncementTable(Root, numAnnouncementTables);
    saSetUsedRootAnnouncementTable(saUsedRootAnnouncementTable() + newSize);
}

/*----------------------------------------------------------------------------------------
  Copy the properties of Root.
----------------------------------------------------------------------------------------*/
void saRootCopyProps(
    saRoot oldRoot,
    saRoot newRoot)
{
    saRootSetNextListingID(newRoot, saRootGetNextListingID(oldRoot));
    saRootSetNextThreadID(newRoot, saRootGetNextThreadID(oldRoot));
    saRootSetNextPostID(newRoot, saRootGetNextPostID(oldRoot));
    saRootSetNextAnnouncementID(newRoot, saRootGetNextAnnouncementID(oldRoot));
}

/*----------------------------------------------------------------------------------------
  Increase the size of the hash table.
----------------------------------------------------------------------------------------*/
static void resizeRootRegionHashTable(
    saRoot Root)
{
    saRegion _Region, prevRegion, nextRegion;
    uint32 oldNumRegions = saRootGetNumRegionTable(Root);
    uint32 newNumRegions = oldNumRegions << 1;
    uint32 xRegion, index;

    if(newNumRegions == 0) {
        newNumRegions = 2;
        saRootAllocRegionTables(Root, 2);
    } else {
        saRootResizeRegionTables(Root, newNumRegions);
    }
    for(xRegion = 0; xRegion < oldNumRegions; xRegion++) {
        _Region = saRootGetiRegionTable(Root, xRegion);
        prevRegion = saRegionNull;
        while(_Region != saRegionNull) {
            nextRegion = saRegionGetNextTableRootRegion(_Region);
            index = (newNumRegions - 1) & utSymGetHashValue(saRegionGetSym(_Region));
            if(index != xRegion) {
                if(prevRegion == saRegionNull) {
                    saRootSetiRegionTable(Root, xRegion, nextRegion);
                } else {
                    saRegionSetNextTableRootRegion(prevRegion, nextRegion);
                }
                saRegionSetNextTableRootRegion(_Region, saRootGetiRegionTable(Root, index));
                saRootSetiRegionTable(Root, index, _Region);
            } else {
                prevRegion = _Region;
            }
            _Region = nextRegion;
        }
    }
}

/*----------------------------------------------------------------------------------------
  Add the Region to the Root.  If the table is near full, build a new one twice
  as big, delete the old one, and return the new one.
----------------------------------------------------------------------------------------*/
static void addRootRegionToHashTable(
    saRoot Root,
    saRegion _Region)
{
    saRegion nextRegion;
    uint32 index;

    if(saRootGetNumRegion(Root) >> 1 >= saRootGetNumRegionTable(Root)) {
        resizeRootRegionHashTable(Root);
    }
    index = (saRootGetNumRegionTable(Root) - 1) & utSymGetHashValue(saRegionGetSym(_Region));
    nextRegion = saRootGetiRegionTable(Root, index);
    saRegionSetNextTableRootRegion(_Region, nextRegion);
    saRootSetiRegionTable(Root, index, _Region);
    saRootSetNumRegion(Root, saRootGetNumRegion(Root) + 1);
}

/*----------------------------------------------------------------------------------------
  Remove the Region from the hash table.
----------------------------------------------------------------------------------------*/
static void removeRootRegionFromHashTable(
    saRoot Root,
    saRegion _Region)
{
    uint32 index = (saRootGetNumRegionTable(Root) - 1) & utSymGetHashValue(saRegionGetSym(_Region));
    saRegion prevRegion, nextRegion;
    
    nextRegion = saRootGetiRegionTable(Root, index);
    if(nextRegion == _Region) {
        saRootSetiRegionTable(Root, index, saRegionGetNextTableRootRegion(nextRegion));
    } else {
        do {
            prevRegion = nextRegion;
            nextRegion = saRegionGetNextTableRootRegion(nextRegion);
        } while(nextRegion != _Region);
        saRegionSetNextTableRootRegion(prevRegion, saRegionGetNextTableRootRegion(_Region));
    }
    saRootSetNumRegion(Root, saRootGetNumRegion(Root) - 1);
    saRegionSetNextTableRootRegion(_Region, saRegionNull);
}

/*----------------------------------------------------------------------------------------
  Find the Region from the Root and its hash key.
----------------------------------------------------------------------------------------*/
saRegion saRootFindRegion(
    saRoot Root,
    utSym Sym)
{
    uint32 mask = saRootGetNumRegionTable(Root) - 1;
    saRegion _Region;

    if(mask + 1 != 0) {
        _Region = saRootGetiRegionTable(Root, utSymGetHashValue(Sym) & mask);
        while(_Region != saRegionNull) {
            if(saRegionGetSym(_Region) == Sym) {
                return _Region;
            }
            _Region = saRegionGetNextTableRootRegion(_Region);
        }
    }
    return saRegionNull;
}

/*----------------------------------------------------------------------------------------
  Find the Region from the Root and its name.
----------------------------------------------------------------------------------------*/
void saRootRenameRegion(
    saRoot Root,
    saRegion _Region,
    utSym sym)
{
    if(saRegionGetSym(_Region) != utSymNull) {
        removeRootRegionFromHashTable(Root, _Region);
    }
    saRegionSetSym(_Region, sym);
    if(sym != utSymNull) {
        addRootRegionToHashTable(Root, _Region);
    }
}

/*----------------------------------------------------------------------------------------
  Add the Region to the head of the list on the Root.
----------------------------------------------------------------------------------------*/
void saRootInsertRegion(
    saRoot Root,
    saRegion _Region)
{
#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Region == saRegionNull) {
        utExit("Non-existent Region");
    }
    if(saRegionGetRoot(_Region) != saRootNull) {
        utExit("Attempting to add Region to Root twice");
    }
#endif
    saRegionSetNextRootRegion(_Region, saRootGetFirstRegion(Root));
    if(saRootGetFirstRegion(Root) != saRegionNull) {
        saRegionSetPrevRootRegion(saRootGetFirstRegion(Root), _Region);
    }
    saRootSetFirstRegion(Root, _Region);
    saRegionSetPrevRootRegion(_Region, saRegionNull);
    if(saRootGetLastRegion(Root) == saRegionNull) {
        saRootSetLastRegion(Root, _Region);
    }
    saRegionSetRoot(_Region, Root);
    if(saRegionGetSym(_Region) != utSymNull) {
        addRootRegionToHashTable(Root, _Region);
    }
}

/*----------------------------------------------------------------------------------------
  Add the Region to the end of the list on the Root.
----------------------------------------------------------------------------------------*/
void saRootAppendRegion(
    saRoot Root,
    saRegion _Region)
{
#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Region == saRegionNull) {
        utExit("Non-existent Region");
    }
    if(saRegionGetRoot(_Region) != saRootNull) {
        utExit("Attempting to add Region to Root twice");
    }
#endif
    saRegionSetPrevRootRegion(_Region, saRootGetLastRegion(Root));
    if(saRootGetLastRegion(Root) != saRegionNull) {
        saRegionSetNextRootRegion(saRootGetLastRegion(Root), _Region);
    }
    saRootSetLastRegion(Root, _Region);
    saRegionSetNextRootRegion(_Region, saRegionNull);
    if(saRootGetFirstRegion(Root) == saRegionNull) {
        saRootSetFirstRegion(Root, _Region);
    }
    saRegionSetRoot(_Region, Root);
    if(saRegionGetSym(_Region) != utSymNull) {
        addRootRegionToHashTable(Root, _Region);
    }
}

/*----------------------------------------------------------------------------------------
  Insert the Region to the Root after the previous Region.
----------------------------------------------------------------------------------------*/
void saRootInsertAfterRegion(
    saRoot Root,
    saRegion prevRegion,
    saRegion _Region)
{
    saRegion nextRegion = saRegionGetNextRootRegion(prevRegion);

#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Region == saRegionNull) {
        utExit("Non-existent Region");
    }
    if(saRegionGetRoot(_Region) != saRootNull) {
        utExit("Attempting to add Region to Root twice");
    }
#endif
    saRegionSetNextRootRegion(_Region, nextRegion);
    saRegionSetNextRootRegion(prevRegion, _Region);
    saRegionSetPrevRootRegion(_Region, prevRegion);
    if(nextRegion != saRegionNull) {
        saRegionSetPrevRootRegion(nextRegion, _Region);
    }
    if(saRootGetLastRegion(Root) == prevRegion) {
        saRootSetLastRegion(Root, _Region);
    }
    saRegionSetRoot(_Region, Root);
    if(saRegionGetSym(_Region) != utSymNull) {
        addRootRegionToHashTable(Root, _Region);
    }
}

/*----------------------------------------------------------------------------------------
 Remove the Region from the Root.
----------------------------------------------------------------------------------------*/
void saRootRemoveRegion(
    saRoot Root,
    saRegion _Region)
{
    saRegion pRegion, nRegion;

#if defined(DD_DEBUG)
    if(_Region == saRegionNull) {
        utExit("Non-existent Region");
    }
    if(saRegionGetRoot(_Region) != saRootNull && saRegionGetRoot(_Region) != Root) {
        utExit("Delete Region from non-owning Root");
    }
#endif
    nRegion = saRegionGetNextRootRegion(_Region);
    pRegion = saRegionGetPrevRootRegion(_Region);
    if(pRegion != saRegionNull) {
        saRegionSetNextRootRegion(pRegion, nRegion);
    } else if(saRootGetFirstRegion(Root) == _Region) {
        saRootSetFirstRegion(Root, nRegion);
    }
    if(nRegion != saRegionNull) {
        saRegionSetPrevRootRegion(nRegion, pRegion);
    } else if(saRootGetLastRegion(Root) == _Region) {
        saRootSetLastRegion(Root, pRegion);
    }
    saRegionSetNextRootRegion(_Region, saRegionNull);
    saRegionSetPrevRootRegion(_Region, saRegionNull);
    saRegionSetRoot(_Region, saRootNull);
    if(saRegionGetSym(_Region) != utSymNull) {
        removeRootRegionFromHashTable(Root, _Region);
    }
}

/*----------------------------------------------------------------------------------------
  Increase the size of the hash table.
----------------------------------------------------------------------------------------*/
static void resizeRootUserHashTable(
    saRoot Root)
{
    saUser _User, prevUser, nextUser;
    uint32 oldNumUsers = saRootGetNumUserTable(Root);
    uint32 newNumUsers = oldNumUsers << 1;
    uint32 xUser, index;

    if(newNumUsers == 0) {
        newNumUsers = 2;
        saRootAllocUserTables(Root, 2);
    } else {
        saRootResizeUserTables(Root, newNumUsers);
    }
    for(xUser = 0; xUser < oldNumUsers; xUser++) {
        _User = saRootGetiUserTable(Root, xUser);
        prevUser = saUserNull;
        while(_User != saUserNull) {
            nextUser = saUserGetNextTableRootUser(_User);
            index = (newNumUsers - 1) & utSymGetHashValue(saUserGetSym(_User));
            if(index != xUser) {
                if(prevUser == saUserNull) {
                    saRootSetiUserTable(Root, xUser, nextUser);
                } else {
                    saUserSetNextTableRootUser(prevUser, nextUser);
                }
                saUserSetNextTableRootUser(_User, saRootGetiUserTable(Root, index));
                saRootSetiUserTable(Root, index, _User);
            } else {
                prevUser = _User;
            }
            _User = nextUser;
        }
    }
}

/*----------------------------------------------------------------------------------------
  Add the User to the Root.  If the table is near full, build a new one twice
  as big, delete the old one, and return the new one.
----------------------------------------------------------------------------------------*/
static void addRootUserToHashTable(
    saRoot Root,
    saUser _User)
{
    saUser nextUser;
    uint32 index;

    if(saRootGetNumUser(Root) >> 1 >= saRootGetNumUserTable(Root)) {
        resizeRootUserHashTable(Root);
    }
    index = (saRootGetNumUserTable(Root) - 1) & utSymGetHashValue(saUserGetSym(_User));
    nextUser = saRootGetiUserTable(Root, index);
    saUserSetNextTableRootUser(_User, nextUser);
    saRootSetiUserTable(Root, index, _User);
    saRootSetNumUser(Root, saRootGetNumUser(Root) + 1);
}

/*----------------------------------------------------------------------------------------
  Remove the User from the hash table.
----------------------------------------------------------------------------------------*/
static void removeRootUserFromHashTable(
    saRoot Root,
    saUser _User)
{
    uint32 index = (saRootGetNumUserTable(Root) - 1) & utSymGetHashValue(saUserGetSym(_User));
    saUser prevUser, nextUser;
    
    nextUser = saRootGetiUserTable(Root, index);
    if(nextUser == _User) {
        saRootSetiUserTable(Root, index, saUserGetNextTableRootUser(nextUser));
    } else {
        do {
            prevUser = nextUser;
            nextUser = saUserGetNextTableRootUser(nextUser);
        } while(nextUser != _User);
        saUserSetNextTableRootUser(prevUser, saUserGetNextTableRootUser(_User));
    }
    saRootSetNumUser(Root, saRootGetNumUser(Root) - 1);
    saUserSetNextTableRootUser(_User, saUserNull);
}

/*----------------------------------------------------------------------------------------
  Find the User from the Root and its hash key.
----------------------------------------------------------------------------------------*/
saUser saRootFindUser(
    saRoot Root,
    utSym Sym)
{
    uint32 mask = saRootGetNumUserTable(Root) - 1;
    saUser _User;

    if(mask + 1 != 0) {
        _User = saRootGetiUserTable(Root, utSymGetHashValue(Sym) & mask);
        while(_User != saUserNull) {
            if(saUserGetSym(_User) == Sym) {
                return _User;
            }
            _User = saUserGetNextTableRootUser(_User);
        }
    }
    return saUserNull;
}

/*----------------------------------------------------------------------------------------
  Find the User from the Root and its name.
----------------------------------------------------------------------------------------*/
void saRootRenameUser(
    saRoot Root,
    saUser _User,
    utSym sym)
{
    if(saUserGetSym(_User) != utSymNull) {
        removeRootUserFromHashTable(Root, _User);
    }
    saUserSetSym(_User, sym);
    if(sym != utSymNull) {
        addRootUserToHashTable(Root, _User);
    }
}

/*----------------------------------------------------------------------------------------
  Add the User to the head of the list on the Root.
----------------------------------------------------------------------------------------*/
void saRootInsertUser(
    saRoot Root,
    saUser _User)
{
#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_User == saUserNull) {
        utExit("Non-existent User");
    }
    if(saUserGetRoot(_User) != saRootNull) {
        utExit("Attempting to add User to Root twice");
    }
#endif
    saUserSetNextRootUser(_User, saRootGetFirstUser(Root));
    if(saRootGetFirstUser(Root) != saUserNull) {
        saUserSetPrevRootUser(saRootGetFirstUser(Root), _User);
    }
    saRootSetFirstUser(Root, _User);
    saUserSetPrevRootUser(_User, saUserNull);
    if(saRootGetLastUser(Root) == saUserNull) {
        saRootSetLastUser(Root, _User);
    }
    saUserSetRoot(_User, Root);
    if(saUserGetSym(_User) != utSymNull) {
        addRootUserToHashTable(Root, _User);
    }
}

/*----------------------------------------------------------------------------------------
  Add the User to the end of the list on the Root.
----------------------------------------------------------------------------------------*/
void saRootAppendUser(
    saRoot Root,
    saUser _User)
{
#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_User == saUserNull) {
        utExit("Non-existent User");
    }
    if(saUserGetRoot(_User) != saRootNull) {
        utExit("Attempting to add User to Root twice");
    }
#endif
    saUserSetPrevRootUser(_User, saRootGetLastUser(Root));
    if(saRootGetLastUser(Root) != saUserNull) {
        saUserSetNextRootUser(saRootGetLastUser(Root), _User);
    }
    saRootSetLastUser(Root, _User);
    saUserSetNextRootUser(_User, saUserNull);
    if(saRootGetFirstUser(Root) == saUserNull) {
        saRootSetFirstUser(Root, _User);
    }
    saUserSetRoot(_User, Root);
    if(saUserGetSym(_User) != utSymNull) {
        addRootUserToHashTable(Root, _User);
    }
}

/*----------------------------------------------------------------------------------------
  Insert the User to the Root after the previous User.
----------------------------------------------------------------------------------------*/
void saRootInsertAfterUser(
    saRoot Root,
    saUser prevUser,
    saUser _User)
{
    saUser nextUser = saUserGetNextRootUser(prevUser);

#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_User == saUserNull) {
        utExit("Non-existent User");
    }
    if(saUserGetRoot(_User) != saRootNull) {
        utExit("Attempting to add User to Root twice");
    }
#endif
    saUserSetNextRootUser(_User, nextUser);
    saUserSetNextRootUser(prevUser, _User);
    saUserSetPrevRootUser(_User, prevUser);
    if(nextUser != saUserNull) {
        saUserSetPrevRootUser(nextUser, _User);
    }
    if(saRootGetLastUser(Root) == prevUser) {
        saRootSetLastUser(Root, _User);
    }
    saUserSetRoot(_User, Root);
    if(saUserGetSym(_User) != utSymNull) {
        addRootUserToHashTable(Root, _User);
    }
}

/*----------------------------------------------------------------------------------------
 Remove the User from the Root.
----------------------------------------------------------------------------------------*/
void saRootRemoveUser(
    saRoot Root,
    saUser _User)
{
    saUser pUser, nUser;

#if defined(DD_DEBUG)
    if(_User == saUserNull) {
        utExit("Non-existent User");
    }
    if(saUserGetRoot(_User) != saRootNull && saUserGetRoot(_User) != Root) {
        utExit("Delete User from non-owning Root");
    }
#endif
    nUser = saUserGetNextRootUser(_User);
    pUser = saUserGetPrevRootUser(_User);
    if(pUser != saUserNull) {
        saUserSetNextRootUser(pUser, nUser);
    } else if(saRootGetFirstUser(Root) == _User) {
        saRootSetFirstUser(Root, nUser);
    }
    if(nUser != saUserNull) {
        saUserSetPrevRootUser(nUser, pUser);
    } else if(saRootGetLastUser(Root) == _User) {
        saRootSetLastUser(Root, pUser);
    }
    saUserSetNextRootUser(_User, saUserNull);
    saUserSetPrevRootUser(_User, saUserNull);
    saUserSetRoot(_User, saRootNull);
    if(saUserGetSym(_User) != utSymNull) {
        removeRootUserFromHashTable(Root, _User);
    }
}

/*----------------------------------------------------------------------------------------
  Increase the size of the hash table.
----------------------------------------------------------------------------------------*/
static void resizeRootByEmailUserHashTable(
    saRoot Root)
{
    saUser _User, prevUser, nextUser;
    uint32 oldNumUsers = saRootGetNumByEmailUserTable(Root);
    uint32 newNumUsers = oldNumUsers << 1;
    uint32 xUser, index;

    if(newNumUsers == 0) {
        newNumUsers = 2;
        saRootAllocByEmailUserTables(Root, 2);
    } else {
        saRootResizeByEmailUserTables(Root, newNumUsers);
    }
    for(xUser = 0; xUser < oldNumUsers; xUser++) {
        _User = saRootGetiByEmailUserTable(Root, xUser);
        prevUser = saUserNull;
        while(_User != saUserNull) {
            nextUser = saUserGetNextTableRootByEmailUser(_User);
            index = (newNumUsers - 1) & utSymGetHashValue(saUserGetEmail(_User));
            if(index != xUser) {
                if(prevUser == saUserNull) {
                    saRootSetiByEmailUserTable(Root, xUser, nextUser);
                } else {
                    saUserSetNextTableRootByEmailUser(prevUser, nextUser);
                }
                saUserSetNextTableRootByEmailUser(_User, saRootGetiByEmailUserTable(Root, index));
                saRootSetiByEmailUserTable(Root, index, _User);
            } else {
                prevUser = _User;
            }
            _User = nextUser;
        }
    }
}

/*----------------------------------------------------------------------------------------
  Add the ByEmailUser to the Root.  If the table is near full, build a new one twice
  as big, delete the old one, and return the new one.
----------------------------------------------------------------------------------------*/
static void addRootByEmailUserToHashTable(
    saRoot Root,
    saUser _User)
{
    saUser nextUser;
    uint32 index;

    if(saRootGetNumByEmailUser(Root) >> 1 >= saRootGetNumByEmailUserTable(Root)) {
        resizeRootByEmailUserHashTable(Root);
    }
    index = (saRootGetNumByEmailUserTable(Root) - 1) & utSymGetHashValue(saUserGetEmail(_User));
    nextUser = saRootGetiByEmailUserTable(Root, index);
    saUserSetNextTableRootByEmailUser(_User, nextUser);
    saRootSetiByEmailUserTable(Root, index, _User);
    saRootSetNumByEmailUser(Root, saRootGetNumByEmailUser(Root) + 1);
}

/*----------------------------------------------------------------------------------------
  Remove the ByEmailUser from the hash table.
----------------------------------------------------------------------------------------*/
static void removeRootByEmailUserFromHashTable(
    saRoot Root,
    saUser _User)
{
    uint32 index = (saRootGetNumByEmailUserTable(Root) - 1) & utSymGetHashValue(saUserGetEmail(_User));
    saUser prevUser, nextUser;
    
    nextUser = saRootGetiByEmailUserTable(Root, index);
    if(nextUser == _User) {
        saRootSetiByEmailUserTable(Root, index, saUserGetNextTableRootByEmailUser(nextUser));
    } else {
        do {
            prevUser = nextUser;
            nextUser = saUserGetNextTableRootByEmailUser(nextUser);
        } while(nextUser != _User);
        saUserSetNextTableRootByEmailUser(prevUser, saUserGetNextTableRootByEmailUser(_User));
    }
    saRootSetNumByEmailUser(Root, saRootGetNumByEmailUser(Root) - 1);
    saUserSetNextTableRootByEmailUser(_User, saUserNull);
}

/*----------------------------------------------------------------------------------------
  Find the ByEmailUser from the Root and its hash key.
----------------------------------------------------------------------------------------*/
saUser saRootFindByEmailUser(
    saRoot Root,
    utSym Email)
{
    uint32 mask = saRootGetNumByEmailUserTable(Root) - 1;
    saUser _User;

    if(mask + 1 != 0) {
        _User = saRootGetiByEmailUserTable(Root, utSymGetHashValue(Email) & mask);
        while(_User != saUserNull) {
            if(saUserGetEmail(_User) == Email) {
                return _User;
            }
            _User = saUserGetNextTableRootByEmailUser(_User);
        }
    }
    return saUserNull;
}

/*----------------------------------------------------------------------------------------
  Add the ByEmailUser to the head of the list on the Root.
----------------------------------------------------------------------------------------*/
void saRootInsertByEmailUser(
    saRoot Root,
    saUser _User)
{
#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_User == saUserNull) {
        utExit("Non-existent User");
    }
    if(saUserGetByEmailRoot(_User) != saRootNull) {
        utExit("Attempting to add User to Root twice");
    }
#endif
    saUserSetNextRootByEmailUser(_User, saRootGetFirstByEmailUser(Root));
    if(saRootGetFirstByEmailUser(Root) != saUserNull) {
        saUserSetPrevRootByEmailUser(saRootGetFirstByEmailUser(Root), _User);
    }
    saRootSetFirstByEmailUser(Root, _User);
    saUserSetPrevRootByEmailUser(_User, saUserNull);
    if(saRootGetLastByEmailUser(Root) == saUserNull) {
        saRootSetLastByEmailUser(Root, _User);
    }
    saUserSetByEmailRoot(_User, Root);
    addRootByEmailUserToHashTable(Root, _User);
}

/*----------------------------------------------------------------------------------------
  Add the ByEmailUser to the end of the list on the Root.
----------------------------------------------------------------------------------------*/
void saRootAppendByEmailUser(
    saRoot Root,
    saUser _User)
{
#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_User == saUserNull) {
        utExit("Non-existent User");
    }
    if(saUserGetByEmailRoot(_User) != saRootNull) {
        utExit("Attempting to add User to Root twice");
    }
#endif
    saUserSetPrevRootByEmailUser(_User, saRootGetLastByEmailUser(Root));
    if(saRootGetLastByEmailUser(Root) != saUserNull) {
        saUserSetNextRootByEmailUser(saRootGetLastByEmailUser(Root), _User);
    }
    saRootSetLastByEmailUser(Root, _User);
    saUserSetNextRootByEmailUser(_User, saUserNull);
    if(saRootGetFirstByEmailUser(Root) == saUserNull) {
        saRootSetFirstByEmailUser(Root, _User);
    }
    saUserSetByEmailRoot(_User, Root);
    addRootByEmailUserToHashTable(Root, _User);
}

/*----------------------------------------------------------------------------------------
  Insert the ByEmailUser to the Root after the previous ByEmailUser.
----------------------------------------------------------------------------------------*/
void saRootInsertAfterByEmailUser(
    saRoot Root,
    saUser prevUser,
    saUser _User)
{
    saUser nextUser = saUserGetNextRootByEmailUser(prevUser);

#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_User == saUserNull) {
        utExit("Non-existent User");
    }
    if(saUserGetByEmailRoot(_User) != saRootNull) {
        utExit("Attempting to add User to Root twice");
    }
#endif
    saUserSetNextRootByEmailUser(_User, nextUser);
    saUserSetNextRootByEmailUser(prevUser, _User);
    saUserSetPrevRootByEmailUser(_User, prevUser);
    if(nextUser != saUserNull) {
        saUserSetPrevRootByEmailUser(nextUser, _User);
    }
    if(saRootGetLastByEmailUser(Root) == prevUser) {
        saRootSetLastByEmailUser(Root, _User);
    }
    saUserSetByEmailRoot(_User, Root);
    addRootByEmailUserToHashTable(Root, _User);
}

/*----------------------------------------------------------------------------------------
 Remove the ByEmailUser from the Root.
----------------------------------------------------------------------------------------*/
void saRootRemoveByEmailUser(
    saRoot Root,
    saUser _User)
{
    saUser pUser, nUser;

#if defined(DD_DEBUG)
    if(_User == saUserNull) {
        utExit("Non-existent User");
    }
    if(saUserGetByEmailRoot(_User) != saRootNull && saUserGetByEmailRoot(_User) != Root) {
        utExit("Delete User from non-owning Root");
    }
#endif
    nUser = saUserGetNextRootByEmailUser(_User);
    pUser = saUserGetPrevRootByEmailUser(_User);
    if(pUser != saUserNull) {
        saUserSetNextRootByEmailUser(pUser, nUser);
    } else if(saRootGetFirstByEmailUser(Root) == _User) {
        saRootSetFirstByEmailUser(Root, nUser);
    }
    if(nUser != saUserNull) {
        saUserSetPrevRootByEmailUser(nUser, pUser);
    } else if(saRootGetLastByEmailUser(Root) == _User) {
        saRootSetLastByEmailUser(Root, pUser);
    }
    saUserSetNextRootByEmailUser(_User, saUserNull);
    saUserSetPrevRootByEmailUser(_User, saUserNull);
    saUserSetByEmailRoot(_User, saRootNull);
    removeRootByEmailUserFromHashTable(Root, _User);
}

/*----------------------------------------------------------------------------------------
  Add the Charity to the head of the list on the Root.
----------------------------------------------------------------------------------------*/
void saRootInsertCharity(
    saRoot Root,
    saCharity _Charity)
{
#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Charity == saCharityNull) {
        utExit("Non-existent Charity");
    }
    if(saCharityGetRoot(_Charity) != saRootNull) {
        utExit("Attempting to add Charity to Root twice");
    }
#endif
    saCharitySetNextRootCharity(_Charity, saRootGetFirstCharity(Root));
    if(saRootGetFirstCharity(Root) != saCharityNull) {
        saCharitySetPrevRootCharity(saRootGetFirstCharity(Root), _Charity);
    }
    saRootSetFirstCharity(Root, _Charity);
    saCharitySetPrevRootCharity(_Charity, saCharityNull);
    if(saRootGetLastCharity(Root) == saCharityNull) {
        saRootSetLastCharity(Root, _Charity);
    }
    saCharitySetRoot(_Charity, Root);
}

/*----------------------------------------------------------------------------------------
  Add the Charity to the end of the list on the Root.
----------------------------------------------------------------------------------------*/
void saRootAppendCharity(
    saRoot Root,
    saCharity _Charity)
{
#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Charity == saCharityNull) {
        utExit("Non-existent Charity");
    }
    if(saCharityGetRoot(_Charity) != saRootNull) {
        utExit("Attempting to add Charity to Root twice");
    }
#endif
    saCharitySetPrevRootCharity(_Charity, saRootGetLastCharity(Root));
    if(saRootGetLastCharity(Root) != saCharityNull) {
        saCharitySetNextRootCharity(saRootGetLastCharity(Root), _Charity);
    }
    saRootSetLastCharity(Root, _Charity);
    saCharitySetNextRootCharity(_Charity, saCharityNull);
    if(saRootGetFirstCharity(Root) == saCharityNull) {
        saRootSetFirstCharity(Root, _Charity);
    }
    saCharitySetRoot(_Charity, Root);
}

/*----------------------------------------------------------------------------------------
  Insert the Charity to the Root after the previous Charity.
----------------------------------------------------------------------------------------*/
void saRootInsertAfterCharity(
    saRoot Root,
    saCharity prevCharity,
    saCharity _Charity)
{
    saCharity nextCharity = saCharityGetNextRootCharity(prevCharity);

#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Charity == saCharityNull) {
        utExit("Non-existent Charity");
    }
    if(saCharityGetRoot(_Charity) != saRootNull) {
        utExit("Attempting to add Charity to Root twice");
    }
#endif
    saCharitySetNextRootCharity(_Charity, nextCharity);
    saCharitySetNextRootCharity(prevCharity, _Charity);
    saCharitySetPrevRootCharity(_Charity, prevCharity);
    if(nextCharity != saCharityNull) {
        saCharitySetPrevRootCharity(nextCharity, _Charity);
    }
    if(saRootGetLastCharity(Root) == prevCharity) {
        saRootSetLastCharity(Root, _Charity);
    }
    saCharitySetRoot(_Charity, Root);
}

/*----------------------------------------------------------------------------------------
 Remove the Charity from the Root.
----------------------------------------------------------------------------------------*/
void saRootRemoveCharity(
    saRoot Root,
    saCharity _Charity)
{
    saCharity pCharity, nCharity;

#if defined(DD_DEBUG)
    if(_Charity == saCharityNull) {
        utExit("Non-existent Charity");
    }
    if(saCharityGetRoot(_Charity) != saRootNull && saCharityGetRoot(_Charity) != Root) {
        utExit("Delete Charity from non-owning Root");
    }
#endif
    nCharity = saCharityGetNextRootCharity(_Charity);
    pCharity = saCharityGetPrevRootCharity(_Charity);
    if(pCharity != saCharityNull) {
        saCharitySetNextRootCharity(pCharity, nCharity);
    } else if(saRootGetFirstCharity(Root) == _Charity) {
        saRootSetFirstCharity(Root, nCharity);
    }
    if(nCharity != saCharityNull) {
        saCharitySetPrevRootCharity(nCharity, pCharity);
    } else if(saRootGetLastCharity(Root) == _Charity) {
        saRootSetLastCharity(Root, pCharity);
    }
    saCharitySetNextRootCharity(_Charity, saCharityNull);
    saCharitySetPrevRootCharity(_Charity, saCharityNull);
    saCharitySetRoot(_Charity, saRootNull);
}

/*----------------------------------------------------------------------------------------
  Increase the size of the hash table.
----------------------------------------------------------------------------------------*/
static void resizeRootCategoryHashTable(
    saRoot Root)
{
    saCategory _Category, prevCategory, nextCategory;
    uint32 oldNumCategorys = saRootGetNumCategoryTable(Root);
    uint32 newNumCategorys = oldNumCategorys << 1;
    uint32 xCategory, index;

    if(newNumCategorys == 0) {
        newNumCategorys = 2;
        saRootAllocCategoryTables(Root, 2);
    } else {
        saRootResizeCategoryTables(Root, newNumCategorys);
    }
    for(xCategory = 0; xCategory < oldNumCategorys; xCategory++) {
        _Category = saRootGetiCategoryTable(Root, xCategory);
        prevCategory = saCategoryNull;
        while(_Category != saCategoryNull) {
            nextCategory = saCategoryGetNextTableRootCategory(_Category);
            index = (newNumCategorys - 1) & utSymGetHashValue(saCategoryGetSym(_Category));
            if(index != xCategory) {
                if(prevCategory == saCategoryNull) {
                    saRootSetiCategoryTable(Root, xCategory, nextCategory);
                } else {
                    saCategorySetNextTableRootCategory(prevCategory, nextCategory);
                }
                saCategorySetNextTableRootCategory(_Category, saRootGetiCategoryTable(Root, index));
                saRootSetiCategoryTable(Root, index, _Category);
            } else {
                prevCategory = _Category;
            }
            _Category = nextCategory;
        }
    }
}

/*----------------------------------------------------------------------------------------
  Add the Category to the Root.  If the table is near full, build a new one twice
  as big, delete the old one, and return the new one.
----------------------------------------------------------------------------------------*/
static void addRootCategoryToHashTable(
    saRoot Root,
    saCategory _Category)
{
    saCategory nextCategory;
    uint32 index;

    if(saRootGetNumCategory(Root) >> 1 >= saRootGetNumCategoryTable(Root)) {
        resizeRootCategoryHashTable(Root);
    }
    index = (saRootGetNumCategoryTable(Root) - 1) & utSymGetHashValue(saCategoryGetSym(_Category));
    nextCategory = saRootGetiCategoryTable(Root, index);
    saCategorySetNextTableRootCategory(_Category, nextCategory);
    saRootSetiCategoryTable(Root, index, _Category);
    saRootSetNumCategory(Root, saRootGetNumCategory(Root) + 1);
}

/*----------------------------------------------------------------------------------------
  Remove the Category from the hash table.
----------------------------------------------------------------------------------------*/
static void removeRootCategoryFromHashTable(
    saRoot Root,
    saCategory _Category)
{
    uint32 index = (saRootGetNumCategoryTable(Root) - 1) & utSymGetHashValue(saCategoryGetSym(_Category));
    saCategory prevCategory, nextCategory;
    
    nextCategory = saRootGetiCategoryTable(Root, index);
    if(nextCategory == _Category) {
        saRootSetiCategoryTable(Root, index, saCategoryGetNextTableRootCategory(nextCategory));
    } else {
        do {
            prevCategory = nextCategory;
            nextCategory = saCategoryGetNextTableRootCategory(nextCategory);
        } while(nextCategory != _Category);
        saCategorySetNextTableRootCategory(prevCategory, saCategoryGetNextTableRootCategory(_Category));
    }
    saRootSetNumCategory(Root, saRootGetNumCategory(Root) - 1);
    saCategorySetNextTableRootCategory(_Category, saCategoryNull);
}

/*----------------------------------------------------------------------------------------
  Find the Category from the Root and its hash key.
----------------------------------------------------------------------------------------*/
saCategory saRootFindCategory(
    saRoot Root,
    utSym Sym)
{
    uint32 mask = saRootGetNumCategoryTable(Root) - 1;
    saCategory _Category;

    if(mask + 1 != 0) {
        _Category = saRootGetiCategoryTable(Root, utSymGetHashValue(Sym) & mask);
        while(_Category != saCategoryNull) {
            if(saCategoryGetSym(_Category) == Sym) {
                return _Category;
            }
            _Category = saCategoryGetNextTableRootCategory(_Category);
        }
    }
    return saCategoryNull;
}

/*----------------------------------------------------------------------------------------
  Find the Category from the Root and its name.
----------------------------------------------------------------------------------------*/
void saRootRenameCategory(
    saRoot Root,
    saCategory _Category,
    utSym sym)
{
    if(saCategoryGetSym(_Category) != utSymNull) {
        removeRootCategoryFromHashTable(Root, _Category);
    }
    saCategorySetSym(_Category, sym);
    if(sym != utSymNull) {
        addRootCategoryToHashTable(Root, _Category);
    }
}

/*----------------------------------------------------------------------------------------
  Add the Category to the head of the list on the Root.
----------------------------------------------------------------------------------------*/
void saRootInsertCategory(
    saRoot Root,
    saCategory _Category)
{
#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Category == saCategoryNull) {
        utExit("Non-existent Category");
    }
    if(saCategoryGetRoot(_Category) != saRootNull) {
        utExit("Attempting to add Category to Root twice");
    }
#endif
    saCategorySetNextRootCategory(_Category, saRootGetFirstCategory(Root));
    if(saRootGetFirstCategory(Root) != saCategoryNull) {
        saCategorySetPrevRootCategory(saRootGetFirstCategory(Root), _Category);
    }
    saRootSetFirstCategory(Root, _Category);
    saCategorySetPrevRootCategory(_Category, saCategoryNull);
    if(saRootGetLastCategory(Root) == saCategoryNull) {
        saRootSetLastCategory(Root, _Category);
    }
    saCategorySetRoot(_Category, Root);
    if(saCategoryGetSym(_Category) != utSymNull) {
        addRootCategoryToHashTable(Root, _Category);
    }
}

/*----------------------------------------------------------------------------------------
  Add the Category to the end of the list on the Root.
----------------------------------------------------------------------------------------*/
void saRootAppendCategory(
    saRoot Root,
    saCategory _Category)
{
#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Category == saCategoryNull) {
        utExit("Non-existent Category");
    }
    if(saCategoryGetRoot(_Category) != saRootNull) {
        utExit("Attempting to add Category to Root twice");
    }
#endif
    saCategorySetPrevRootCategory(_Category, saRootGetLastCategory(Root));
    if(saRootGetLastCategory(Root) != saCategoryNull) {
        saCategorySetNextRootCategory(saRootGetLastCategory(Root), _Category);
    }
    saRootSetLastCategory(Root, _Category);
    saCategorySetNextRootCategory(_Category, saCategoryNull);
    if(saRootGetFirstCategory(Root) == saCategoryNull) {
        saRootSetFirstCategory(Root, _Category);
    }
    saCategorySetRoot(_Category, Root);
    if(saCategoryGetSym(_Category) != utSymNull) {
        addRootCategoryToHashTable(Root, _Category);
    }
}

/*----------------------------------------------------------------------------------------
  Insert the Category to the Root after the previous Category.
----------------------------------------------------------------------------------------*/
void saRootInsertAfterCategory(
    saRoot Root,
    saCategory prevCategory,
    saCategory _Category)
{
    saCategory nextCategory = saCategoryGetNextRootCategory(prevCategory);

#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Category == saCategoryNull) {
        utExit("Non-existent Category");
    }
    if(saCategoryGetRoot(_Category) != saRootNull) {
        utExit("Attempting to add Category to Root twice");
    }
#endif
    saCategorySetNextRootCategory(_Category, nextCategory);
    saCategorySetNextRootCategory(prevCategory, _Category);
    saCategorySetPrevRootCategory(_Category, prevCategory);
    if(nextCategory != saCategoryNull) {
        saCategorySetPrevRootCategory(nextCategory, _Category);
    }
    if(saRootGetLastCategory(Root) == prevCategory) {
        saRootSetLastCategory(Root, _Category);
    }
    saCategorySetRoot(_Category, Root);
    if(saCategoryGetSym(_Category) != utSymNull) {
        addRootCategoryToHashTable(Root, _Category);
    }
}

/*----------------------------------------------------------------------------------------
 Remove the Category from the Root.
----------------------------------------------------------------------------------------*/
void saRootRemoveCategory(
    saRoot Root,
    saCategory _Category)
{
    saCategory pCategory, nCategory;

#if defined(DD_DEBUG)
    if(_Category == saCategoryNull) {
        utExit("Non-existent Category");
    }
    if(saCategoryGetRoot(_Category) != saRootNull && saCategoryGetRoot(_Category) != Root) {
        utExit("Delete Category from non-owning Root");
    }
#endif
    nCategory = saCategoryGetNextRootCategory(_Category);
    pCategory = saCategoryGetPrevRootCategory(_Category);
    if(pCategory != saCategoryNull) {
        saCategorySetNextRootCategory(pCategory, nCategory);
    } else if(saRootGetFirstCategory(Root) == _Category) {
        saRootSetFirstCategory(Root, nCategory);
    }
    if(nCategory != saCategoryNull) {
        saCategorySetPrevRootCategory(nCategory, pCategory);
    } else if(saRootGetLastCategory(Root) == _Category) {
        saRootSetLastCategory(Root, pCategory);
    }
    saCategorySetNextRootCategory(_Category, saCategoryNull);
    saCategorySetPrevRootCategory(_Category, saCategoryNull);
    saCategorySetRoot(_Category, saRootNull);
    if(saCategoryGetSym(_Category) != utSymNull) {
        removeRootCategoryFromHashTable(Root, _Category);
    }
}

/*----------------------------------------------------------------------------------------
  Increase the size of the hash table.
----------------------------------------------------------------------------------------*/
static void resizeRootListingHashTable(
    saRoot Root)
{
    saListing _Listing, prevListing, nextListing;
    uint32 oldNumListings = saRootGetNumListingTable(Root);
    uint32 newNumListings = oldNumListings << 1;
    uint32 xListing, index;

    if(newNumListings == 0) {
        newNumListings = 2;
        saRootAllocListingTables(Root, 2);
    } else {
        saRootResizeListingTables(Root, newNumListings);
    }
    for(xListing = 0; xListing < oldNumListings; xListing++) {
        _Listing = saRootGetiListingTable(Root, xListing);
        prevListing = saListingNull;
        while(_Listing != saListingNull) {
            nextListing = saListingGetNextTableRootListing(_Listing);
            index = (newNumListings - 1) & (uint32)saListingGetID(_Listing);
            if(index != xListing) {
                if(prevListing == saListingNull) {
                    saRootSetiListingTable(Root, xListing, nextListing);
                } else {
                    saListingSetNextTableRootListing(prevListing, nextListing);
                }
                saListingSetNextTableRootListing(_Listing, saRootGetiListingTable(Root, index));
                saRootSetiListingTable(Root, index, _Listing);
            } else {
                prevListing = _Listing;
            }
            _Listing = nextListing;
        }
    }
}

/*----------------------------------------------------------------------------------------
  Add the Listing to the Root.  If the table is near full, build a new one twice
  as big, delete the old one, and return the new one.
----------------------------------------------------------------------------------------*/
static void addRootListingToHashTable(
    saRoot Root,
    saListing _Listing)
{
    saListing nextListing;
    uint32 index;

    if(saRootGetNumListing(Root) >> 1 >= saRootGetNumListingTable(Root)) {
        resizeRootListingHashTable(Root);
    }
    index = (saRootGetNumListingTable(Root) - 1) & (uint32)saListingGetID(_Listing);
    nextListing = saRootGetiListingTable(Root, index);
    saListingSetNextTableRootListing(_Listing, nextListing);
    saRootSetiListingTable(Root, index, _Listing);
    saRootSetNumListing(Root, saRootGetNumListing(Root) + 1);
}

/*----------------------------------------------------------------------------------------
  Remove the Listing from the hash table.
----------------------------------------------------------------------------------------*/
static void removeRootListingFromHashTable(
    saRoot Root,
    saListing _Listing)
{
    uint32 index = (saRootGetNumListingTable(Root) - 1) & (uint32)saListingGetID(_Listing);
    saListing prevListing, nextListing;
    
    nextListing = saRootGetiListingTable(Root, index);
    if(nextListing == _Listing) {
        saRootSetiListingTable(Root, index, saListingGetNextTableRootListing(nextListing));
    } else {
        do {
            prevListing = nextListing;
            nextListing = saListingGetNextTableRootListing(nextListing);
        } while(nextListing != _Listing);
        saListingSetNextTableRootListing(prevListing, saListingGetNextTableRootListing(_Listing));
    }
    saRootSetNumListing(Root, saRootGetNumListing(Root) - 1);
    saListingSetNextTableRootListing(_Listing, saListingNull);
}

/*----------------------------------------------------------------------------------------
  Find the Listing from the Root and its hash key.
----------------------------------------------------------------------------------------*/
saListing saRootFindListing(
    saRoot Root,
    uint64 ID)
{
    uint32 mask = saRootGetNumListingTable(Root) - 1;
    saListing _Listing;

    if(mask + 1 != 0) {
        _Listing = saRootGetiListingTable(Root, (uint32)ID & mask);
        while(_Listing != saListingNull) {
            if(saListingGetID(_Listing) == ID) {
                return _Listing;
            }
            _Listing = saListingGetNextTableRootListing(_Listing);
        }
    }
    return saListingNull;
}

/*----------------------------------------------------------------------------------------
  Add the Listing to the head of the list on the Root.
----------------------------------------------------------------------------------------*/
void saRootInsertListing(
    saRoot Root,
    saListing _Listing)
{
#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Listing == saListingNull) {
        utExit("Non-existent Listing");
    }
    if(saListingGetRoot(_Listing) != saRootNull) {
        utExit("Attempting to add Listing to Root twice");
    }
#endif
    saListingSetNextRootListing(_Listing, saRootGetFirstListing(Root));
    if(saRootGetFirstListing(Root) != saListingNull) {
        saListingSetPrevRootListing(saRootGetFirstListing(Root), _Listing);
    }
    saRootSetFirstListing(Root, _Listing);
    saListingSetPrevRootListing(_Listing, saListingNull);
    if(saRootGetLastListing(Root) == saListingNull) {
        saRootSetLastListing(Root, _Listing);
    }
    saListingSetRoot(_Listing, Root);
    addRootListingToHashTable(Root, _Listing);
}

/*----------------------------------------------------------------------------------------
  Add the Listing to the end of the list on the Root.
----------------------------------------------------------------------------------------*/
void saRootAppendListing(
    saRoot Root,
    saListing _Listing)
{
#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Listing == saListingNull) {
        utExit("Non-existent Listing");
    }
    if(saListingGetRoot(_Listing) != saRootNull) {
        utExit("Attempting to add Listing to Root twice");
    }
#endif
    saListingSetPrevRootListing(_Listing, saRootGetLastListing(Root));
    if(saRootGetLastListing(Root) != saListingNull) {
        saListingSetNextRootListing(saRootGetLastListing(Root), _Listing);
    }
    saRootSetLastListing(Root, _Listing);
    saListingSetNextRootListing(_Listing, saListingNull);
    if(saRootGetFirstListing(Root) == saListingNull) {
        saRootSetFirstListing(Root, _Listing);
    }
    saListingSetRoot(_Listing, Root);
    addRootListingToHashTable(Root, _Listing);
}

/*----------------------------------------------------------------------------------------
  Insert the Listing to the Root after the previous Listing.
----------------------------------------------------------------------------------------*/
void saRootInsertAfterListing(
    saRoot Root,
    saListing prevListing,
    saListing _Listing)
{
    saListing nextListing = saListingGetNextRootListing(prevListing);

#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Listing == saListingNull) {
        utExit("Non-existent Listing");
    }
    if(saListingGetRoot(_Listing) != saRootNull) {
        utExit("Attempting to add Listing to Root twice");
    }
#endif
    saListingSetNextRootListing(_Listing, nextListing);
    saListingSetNextRootListing(prevListing, _Listing);
    saListingSetPrevRootListing(_Listing, prevListing);
    if(nextListing != saListingNull) {
        saListingSetPrevRootListing(nextListing, _Listing);
    }
    if(saRootGetLastListing(Root) == prevListing) {
        saRootSetLastListing(Root, _Listing);
    }
    saListingSetRoot(_Listing, Root);
    addRootListingToHashTable(Root, _Listing);
}

/*----------------------------------------------------------------------------------------
 Remove the Listing from the Root.
----------------------------------------------------------------------------------------*/
void saRootRemoveListing(
    saRoot Root,
    saListing _Listing)
{
    saListing pListing, nListing;

#if defined(DD_DEBUG)
    if(_Listing == saListingNull) {
        utExit("Non-existent Listing");
    }
    if(saListingGetRoot(_Listing) != saRootNull && saListingGetRoot(_Listing) != Root) {
        utExit("Delete Listing from non-owning Root");
    }
#endif
    nListing = saListingGetNextRootListing(_Listing);
    pListing = saListingGetPrevRootListing(_Listing);
    if(pListing != saListingNull) {
        saListingSetNextRootListing(pListing, nListing);
    } else if(saRootGetFirstListing(Root) == _Listing) {
        saRootSetFirstListing(Root, nListing);
    }
    if(nListing != saListingNull) {
        saListingSetPrevRootListing(nListing, pListing);
    } else if(saRootGetLastListing(Root) == _Listing) {
        saRootSetLastListing(Root, pListing);
    }
    saListingSetNextRootListing(_Listing, saListingNull);
    saListingSetPrevRootListing(_Listing, saListingNull);
    saListingSetRoot(_Listing, saRootNull);
    removeRootListingFromHashTable(Root, _Listing);
}

/*----------------------------------------------------------------------------------------
  Increase the size of the hash table.
----------------------------------------------------------------------------------------*/
static void resizeRootSessionHashTable(
    saRoot Root)
{
    saSession _Session, prevSession, nextSession;
    uint32 oldNumSessions = saRootGetNumSessionTable(Root);
    uint32 newNumSessions = oldNumSessions << 1;
    uint32 xSession, index;

    if(newNumSessions == 0) {
        newNumSessions = 2;
        saRootAllocSessionTables(Root, 2);
    } else {
        saRootResizeSessionTables(Root, newNumSessions);
    }
    for(xSession = 0; xSession < oldNumSessions; xSession++) {
        _Session = saRootGetiSessionTable(Root, xSession);
        prevSession = saSessionNull;
        while(_Session != saSessionNull) {
            nextSession = saSessionGetNextTableRootSession(_Session);
            index = (newNumSessions - 1) & utSymGetHashValue(saSessionGetSym(_Session));
            if(index != xSession) {
                if(prevSession == saSessionNull) {
                    saRootSetiSessionTable(Root, xSession, nextSession);
                } else {
                    saSessionSetNextTableRootSession(prevSession, nextSession);
                }
                saSessionSetNextTableRootSession(_Session, saRootGetiSessionTable(Root, index));
                saRootSetiSessionTable(Root, index, _Session);
            } else {
                prevSession = _Session;
            }
            _Session = nextSession;
        }
    }
}

/*----------------------------------------------------------------------------------------
  Add the Session to the Root.  If the table is near full, build a new one twice
  as big, delete the old one, and return the new one.
----------------------------------------------------------------------------------------*/
static void addRootSessionToHashTable(
    saRoot Root,
    saSession _Session)
{
    saSession nextSession;
    uint32 index;

    if(saRootGetNumSession(Root) >> 1 >= saRootGetNumSessionTable(Root)) {
        resizeRootSessionHashTable(Root);
    }
    index = (saRootGetNumSessionTable(Root) - 1) & utSymGetHashValue(saSessionGetSym(_Session));
    nextSession = saRootGetiSessionTable(Root, index);
    saSessionSetNextTableRootSession(_Session, nextSession);
    saRootSetiSessionTable(Root, index, _Session);
    saRootSetNumSession(Root, saRootGetNumSession(Root) + 1);
}

/*----------------------------------------------------------------------------------------
  Remove the Session from the hash table.
----------------------------------------------------------------------------------------*/
static void removeRootSessionFromHashTable(
    saRoot Root,
    saSession _Session)
{
    uint32 index = (saRootGetNumSessionTable(Root) - 1) & utSymGetHashValue(saSessionGetSym(_Session));
    saSession prevSession, nextSession;
    
    nextSession = saRootGetiSessionTable(Root, index);
    if(nextSession == _Session) {
        saRootSetiSessionTable(Root, index, saSessionGetNextTableRootSession(nextSession));
    } else {
        do {
            prevSession = nextSession;
            nextSession = saSessionGetNextTableRootSession(nextSession);
        } while(nextSession != _Session);
        saSessionSetNextTableRootSession(prevSession, saSessionGetNextTableRootSession(_Session));
    }
    saRootSetNumSession(Root, saRootGetNumSession(Root) - 1);
    saSessionSetNextTableRootSession(_Session, saSessionNull);
}

/*----------------------------------------------------------------------------------------
  Find the Session from the Root and its hash key.
----------------------------------------------------------------------------------------*/
saSession saRootFindSession(
    saRoot Root,
    utSym Sym)
{
    uint32 mask = saRootGetNumSessionTable(Root) - 1;
    saSession _Session;

    if(mask + 1 != 0) {
        _Session = saRootGetiSessionTable(Root, utSymGetHashValue(Sym) & mask);
        while(_Session != saSessionNull) {
            if(saSessionGetSym(_Session) == Sym) {
                return _Session;
            }
            _Session = saSessionGetNextTableRootSession(_Session);
        }
    }
    return saSessionNull;
}

/*----------------------------------------------------------------------------------------
  Find the Session from the Root and its name.
----------------------------------------------------------------------------------------*/
void saRootRenameSession(
    saRoot Root,
    saSession _Session,
    utSym sym)
{
    if(saSessionGetSym(_Session) != utSymNull) {
        removeRootSessionFromHashTable(Root, _Session);
    }
    saSessionSetSym(_Session, sym);
    if(sym != utSymNull) {
        addRootSessionToHashTable(Root, _Session);
    }
}

/*----------------------------------------------------------------------------------------
  Add the Session to the head of the list on the Root.
----------------------------------------------------------------------------------------*/
void saRootInsertSession(
    saRoot Root,
    saSession _Session)
{
#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Session == saSessionNull) {
        utExit("Non-existent Session");
    }
    if(saSessionGetRoot(_Session) != saRootNull) {
        utExit("Attempting to add Session to Root twice");
    }
#endif
    saSessionSetNextRootSession(_Session, saRootGetFirstSession(Root));
    if(saRootGetFirstSession(Root) != saSessionNull) {
        saSessionSetPrevRootSession(saRootGetFirstSession(Root), _Session);
    }
    saRootSetFirstSession(Root, _Session);
    saSessionSetPrevRootSession(_Session, saSessionNull);
    if(saRootGetLastSession(Root) == saSessionNull) {
        saRootSetLastSession(Root, _Session);
    }
    saSessionSetRoot(_Session, Root);
    if(saSessionGetSym(_Session) != utSymNull) {
        addRootSessionToHashTable(Root, _Session);
    }
}

/*----------------------------------------------------------------------------------------
  Add the Session to the end of the list on the Root.
----------------------------------------------------------------------------------------*/
void saRootAppendSession(
    saRoot Root,
    saSession _Session)
{
#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Session == saSessionNull) {
        utExit("Non-existent Session");
    }
    if(saSessionGetRoot(_Session) != saRootNull) {
        utExit("Attempting to add Session to Root twice");
    }
#endif
    saSessionSetPrevRootSession(_Session, saRootGetLastSession(Root));
    if(saRootGetLastSession(Root) != saSessionNull) {
        saSessionSetNextRootSession(saRootGetLastSession(Root), _Session);
    }
    saRootSetLastSession(Root, _Session);
    saSessionSetNextRootSession(_Session, saSessionNull);
    if(saRootGetFirstSession(Root) == saSessionNull) {
        saRootSetFirstSession(Root, _Session);
    }
    saSessionSetRoot(_Session, Root);
    if(saSessionGetSym(_Session) != utSymNull) {
        addRootSessionToHashTable(Root, _Session);
    }
}

/*----------------------------------------------------------------------------------------
  Insert the Session to the Root after the previous Session.
----------------------------------------------------------------------------------------*/
void saRootInsertAfterSession(
    saRoot Root,
    saSession prevSession,
    saSession _Session)
{
    saSession nextSession = saSessionGetNextRootSession(prevSession);

#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Session == saSessionNull) {
        utExit("Non-existent Session");
    }
    if(saSessionGetRoot(_Session) != saRootNull) {
        utExit("Attempting to add Session to Root twice");
    }
#endif
    saSessionSetNextRootSession(_Session, nextSession);
    saSessionSetNextRootSession(prevSession, _Session);
    saSessionSetPrevRootSession(_Session, prevSession);
    if(nextSession != saSessionNull) {
        saSessionSetPrevRootSession(nextSession, _Session);
    }
    if(saRootGetLastSession(Root) == prevSession) {
        saRootSetLastSession(Root, _Session);
    }
    saSessionSetRoot(_Session, Root);
    if(saSessionGetSym(_Session) != utSymNull) {
        addRootSessionToHashTable(Root, _Session);
    }
}

/*----------------------------------------------------------------------------------------
 Remove the Session from the Root.
----------------------------------------------------------------------------------------*/
void saRootRemoveSession(
    saRoot Root,
    saSession _Session)
{
    saSession pSession, nSession;

#if defined(DD_DEBUG)
    if(_Session == saSessionNull) {
        utExit("Non-existent Session");
    }
    if(saSessionGetRoot(_Session) != saRootNull && saSessionGetRoot(_Session) != Root) {
        utExit("Delete Session from non-owning Root");
    }
#endif
    nSession = saSessionGetNextRootSession(_Session);
    pSession = saSessionGetPrevRootSession(_Session);
    if(pSession != saSessionNull) {
        saSessionSetNextRootSession(pSession, nSession);
    } else if(saRootGetFirstSession(Root) == _Session) {
        saRootSetFirstSession(Root, nSession);
    }
    if(nSession != saSessionNull) {
        saSessionSetPrevRootSession(nSession, pSession);
    } else if(saRootGetLastSession(Root) == _Session) {
        saRootSetLastSession(Root, pSession);
    }
    saSessionSetNextRootSession(_Session, saSessionNull);
    saSessionSetPrevRootSession(_Session, saSessionNull);
    saSessionSetRoot(_Session, saRootNull);
    if(saSessionGetSym(_Session) != utSymNull) {
        removeRootSessionFromHashTable(Root, _Session);
    }
}

/*----------------------------------------------------------------------------------------
  Increase the size of the hash table.
----------------------------------------------------------------------------------------*/
static void resizeRootThreadHashTable(
    saRoot Root)
{
    saThread _Thread, prevThread, nextThread;
    uint32 oldNumThreads = saRootGetNumThreadTable(Root);
    uint32 newNumThreads = oldNumThreads << 1;
    uint32 xThread, index;

    if(newNumThreads == 0) {
        newNumThreads = 2;
        saRootAllocThreadTables(Root, 2);
    } else {
        saRootResizeThreadTables(Root, newNumThreads);
    }
    for(xThread = 0; xThread < oldNumThreads; xThread++) {
        _Thread = saRootGetiThreadTable(Root, xThread);
        prevThread = saThreadNull;
        while(_Thread != saThreadNull) {
            nextThread = saThreadGetNextTableRootThread(_Thread);
            index = (newNumThreads - 1) & (uint32)saThreadGetID(_Thread);
            if(index != xThread) {
                if(prevThread == saThreadNull) {
                    saRootSetiThreadTable(Root, xThread, nextThread);
                } else {
                    saThreadSetNextTableRootThread(prevThread, nextThread);
                }
                saThreadSetNextTableRootThread(_Thread, saRootGetiThreadTable(Root, index));
                saRootSetiThreadTable(Root, index, _Thread);
            } else {
                prevThread = _Thread;
            }
            _Thread = nextThread;
        }
    }
}

/*----------------------------------------------------------------------------------------
  Add the Thread to the Root.  If the table is near full, build a new one twice
  as big, delete the old one, and return the new one.
----------------------------------------------------------------------------------------*/
static void addRootThreadToHashTable(
    saRoot Root,
    saThread _Thread)
{
    saThread nextThread;
    uint32 index;

    if(saRootGetNumThread(Root) >> 1 >= saRootGetNumThreadTable(Root)) {
        resizeRootThreadHashTable(Root);
    }
    index = (saRootGetNumThreadTable(Root) - 1) & (uint32)saThreadGetID(_Thread);
    nextThread = saRootGetiThreadTable(Root, index);
    saThreadSetNextTableRootThread(_Thread, nextThread);
    saRootSetiThreadTable(Root, index, _Thread);
    saRootSetNumThread(Root, saRootGetNumThread(Root) + 1);
}

/*----------------------------------------------------------------------------------------
  Remove the Thread from the hash table.
----------------------------------------------------------------------------------------*/
static void removeRootThreadFromHashTable(
    saRoot Root,
    saThread _Thread)
{
    uint32 index = (saRootGetNumThreadTable(Root) - 1) & (uint32)saThreadGetID(_Thread);
    saThread prevThread, nextThread;
    
    nextThread = saRootGetiThreadTable(Root, index);
    if(nextThread == _Thread) {
        saRootSetiThreadTable(Root, index, saThreadGetNextTableRootThread(nextThread));
    } else {
        do {
            prevThread = nextThread;
            nextThread = saThreadGetNextTableRootThread(nextThread);
        } while(nextThread != _Thread);
        saThreadSetNextTableRootThread(prevThread, saThreadGetNextTableRootThread(_Thread));
    }
    saRootSetNumThread(Root, saRootGetNumThread(Root) - 1);
    saThreadSetNextTableRootThread(_Thread, saThreadNull);
}

/*----------------------------------------------------------------------------------------
  Find the Thread from the Root and its hash key.
----------------------------------------------------------------------------------------*/
saThread saRootFindThread(
    saRoot Root,
    uint32 ID)
{
    uint32 mask = saRootGetNumThreadTable(Root) - 1;
    saThread _Thread;

    if(mask + 1 != 0) {
        _Thread = saRootGetiThreadTable(Root, (uint32)ID & mask);
        while(_Thread != saThreadNull) {
            if(saThreadGetID(_Thread) == ID) {
                return _Thread;
            }
            _Thread = saThreadGetNextTableRootThread(_Thread);
        }
    }
    return saThreadNull;
}

/*----------------------------------------------------------------------------------------
  Add the Thread to the head of the list on the Root.
----------------------------------------------------------------------------------------*/
void saRootInsertThread(
    saRoot Root,
    saThread _Thread)
{
#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Thread == saThreadNull) {
        utExit("Non-existent Thread");
    }
    if(saThreadGetRoot(_Thread) != saRootNull) {
        utExit("Attempting to add Thread to Root twice");
    }
#endif
    saThreadSetNextRootThread(_Thread, saRootGetFirstThread(Root));
    if(saRootGetFirstThread(Root) != saThreadNull) {
        saThreadSetPrevRootThread(saRootGetFirstThread(Root), _Thread);
    }
    saRootSetFirstThread(Root, _Thread);
    saThreadSetPrevRootThread(_Thread, saThreadNull);
    if(saRootGetLastThread(Root) == saThreadNull) {
        saRootSetLastThread(Root, _Thread);
    }
    saThreadSetRoot(_Thread, Root);
    addRootThreadToHashTable(Root, _Thread);
}

/*----------------------------------------------------------------------------------------
  Add the Thread to the end of the list on the Root.
----------------------------------------------------------------------------------------*/
void saRootAppendThread(
    saRoot Root,
    saThread _Thread)
{
#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Thread == saThreadNull) {
        utExit("Non-existent Thread");
    }
    if(saThreadGetRoot(_Thread) != saRootNull) {
        utExit("Attempting to add Thread to Root twice");
    }
#endif
    saThreadSetPrevRootThread(_Thread, saRootGetLastThread(Root));
    if(saRootGetLastThread(Root) != saThreadNull) {
        saThreadSetNextRootThread(saRootGetLastThread(Root), _Thread);
    }
    saRootSetLastThread(Root, _Thread);
    saThreadSetNextRootThread(_Thread, saThreadNull);
    if(saRootGetFirstThread(Root) == saThreadNull) {
        saRootSetFirstThread(Root, _Thread);
    }
    saThreadSetRoot(_Thread, Root);
    addRootThreadToHashTable(Root, _Thread);
}

/*----------------------------------------------------------------------------------------
  Insert the Thread to the Root after the previous Thread.
----------------------------------------------------------------------------------------*/
void saRootInsertAfterThread(
    saRoot Root,
    saThread prevThread,
    saThread _Thread)
{
    saThread nextThread = saThreadGetNextRootThread(prevThread);

#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Thread == saThreadNull) {
        utExit("Non-existent Thread");
    }
    if(saThreadGetRoot(_Thread) != saRootNull) {
        utExit("Attempting to add Thread to Root twice");
    }
#endif
    saThreadSetNextRootThread(_Thread, nextThread);
    saThreadSetNextRootThread(prevThread, _Thread);
    saThreadSetPrevRootThread(_Thread, prevThread);
    if(nextThread != saThreadNull) {
        saThreadSetPrevRootThread(nextThread, _Thread);
    }
    if(saRootGetLastThread(Root) == prevThread) {
        saRootSetLastThread(Root, _Thread);
    }
    saThreadSetRoot(_Thread, Root);
    addRootThreadToHashTable(Root, _Thread);
}

/*----------------------------------------------------------------------------------------
 Remove the Thread from the Root.
----------------------------------------------------------------------------------------*/
void saRootRemoveThread(
    saRoot Root,
    saThread _Thread)
{
    saThread pThread, nThread;

#if defined(DD_DEBUG)
    if(_Thread == saThreadNull) {
        utExit("Non-existent Thread");
    }
    if(saThreadGetRoot(_Thread) != saRootNull && saThreadGetRoot(_Thread) != Root) {
        utExit("Delete Thread from non-owning Root");
    }
#endif
    nThread = saThreadGetNextRootThread(_Thread);
    pThread = saThreadGetPrevRootThread(_Thread);
    if(pThread != saThreadNull) {
        saThreadSetNextRootThread(pThread, nThread);
    } else if(saRootGetFirstThread(Root) == _Thread) {
        saRootSetFirstThread(Root, nThread);
    }
    if(nThread != saThreadNull) {
        saThreadSetPrevRootThread(nThread, pThread);
    } else if(saRootGetLastThread(Root) == _Thread) {
        saRootSetLastThread(Root, pThread);
    }
    saThreadSetNextRootThread(_Thread, saThreadNull);
    saThreadSetPrevRootThread(_Thread, saThreadNull);
    saThreadSetRoot(_Thread, saRootNull);
    removeRootThreadFromHashTable(Root, _Thread);
}

/*----------------------------------------------------------------------------------------
  Increase the size of the hash table.
----------------------------------------------------------------------------------------*/
static void resizeRootPostHashTable(
    saRoot Root)
{
    saPost _Post, prevPost, nextPost;
    uint32 oldNumPosts = saRootGetNumPostTable(Root);
    uint32 newNumPosts = oldNumPosts << 1;
    uint32 xPost, index;

    if(newNumPosts == 0) {
        newNumPosts = 2;
        saRootAllocPostTables(Root, 2);
    } else {
        saRootResizePostTables(Root, newNumPosts);
    }
    for(xPost = 0; xPost < oldNumPosts; xPost++) {
        _Post = saRootGetiPostTable(Root, xPost);
        prevPost = saPostNull;
        while(_Post != saPostNull) {
            nextPost = saPostGetNextTableRootPost(_Post);
            index = (newNumPosts - 1) & (uint32)saPostGetID(_Post);
            if(index != xPost) {
                if(prevPost == saPostNull) {
                    saRootSetiPostTable(Root, xPost, nextPost);
                } else {
                    saPostSetNextTableRootPost(prevPost, nextPost);
                }
                saPostSetNextTableRootPost(_Post, saRootGetiPostTable(Root, index));
                saRootSetiPostTable(Root, index, _Post);
            } else {
                prevPost = _Post;
            }
            _Post = nextPost;
        }
    }
}

/*----------------------------------------------------------------------------------------
  Add the Post to the Root.  If the table is near full, build a new one twice
  as big, delete the old one, and return the new one.
----------------------------------------------------------------------------------------*/
static void addRootPostToHashTable(
    saRoot Root,
    saPost _Post)
{
    saPost nextPost;
    uint32 index;

    if(saRootGetNumPost(Root) >> 1 >= saRootGetNumPostTable(Root)) {
        resizeRootPostHashTable(Root);
    }
    index = (saRootGetNumPostTable(Root) - 1) & (uint32)saPostGetID(_Post);
    nextPost = saRootGetiPostTable(Root, index);
    saPostSetNextTableRootPost(_Post, nextPost);
    saRootSetiPostTable(Root, index, _Post);
    saRootSetNumPost(Root, saRootGetNumPost(Root) + 1);
}

/*----------------------------------------------------------------------------------------
  Remove the Post from the hash table.
----------------------------------------------------------------------------------------*/
static void removeRootPostFromHashTable(
    saRoot Root,
    saPost _Post)
{
    uint32 index = (saRootGetNumPostTable(Root) - 1) & (uint32)saPostGetID(_Post);
    saPost prevPost, nextPost;
    
    nextPost = saRootGetiPostTable(Root, index);
    if(nextPost == _Post) {
        saRootSetiPostTable(Root, index, saPostGetNextTableRootPost(nextPost));
    } else {
        do {
            prevPost = nextPost;
            nextPost = saPostGetNextTableRootPost(nextPost);
        } while(nextPost != _Post);
        saPostSetNextTableRootPost(prevPost, saPostGetNextTableRootPost(_Post));
    }
    saRootSetNumPost(Root, saRootGetNumPost(Root) - 1);
    saPostSetNextTableRootPost(_Post, saPostNull);
}

/*----------------------------------------------------------------------------------------
  Find the Post from the Root and its hash key.
----------------------------------------------------------------------------------------*/
saPost saRootFindPost(
    saRoot Root,
    uint32 ID)
{
    uint32 mask = saRootGetNumPostTable(Root) - 1;
    saPost _Post;

    if(mask + 1 != 0) {
        _Post = saRootGetiPostTable(Root, (uint32)ID & mask);
        while(_Post != saPostNull) {
            if(saPostGetID(_Post) == ID) {
                return _Post;
            }
            _Post = saPostGetNextTableRootPost(_Post);
        }
    }
    return saPostNull;
}

/*----------------------------------------------------------------------------------------
  Add the Post to the head of the list on the Root.
----------------------------------------------------------------------------------------*/
void saRootInsertPost(
    saRoot Root,
    saPost _Post)
{
#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Post == saPostNull) {
        utExit("Non-existent Post");
    }
    if(saPostGetRoot(_Post) != saRootNull) {
        utExit("Attempting to add Post to Root twice");
    }
#endif
    saPostSetNextRootPost(_Post, saRootGetFirstPost(Root));
    if(saRootGetFirstPost(Root) != saPostNull) {
        saPostSetPrevRootPost(saRootGetFirstPost(Root), _Post);
    }
    saRootSetFirstPost(Root, _Post);
    saPostSetPrevRootPost(_Post, saPostNull);
    if(saRootGetLastPost(Root) == saPostNull) {
        saRootSetLastPost(Root, _Post);
    }
    saPostSetRoot(_Post, Root);
    addRootPostToHashTable(Root, _Post);
}

/*----------------------------------------------------------------------------------------
  Add the Post to the end of the list on the Root.
----------------------------------------------------------------------------------------*/
void saRootAppendPost(
    saRoot Root,
    saPost _Post)
{
#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Post == saPostNull) {
        utExit("Non-existent Post");
    }
    if(saPostGetRoot(_Post) != saRootNull) {
        utExit("Attempting to add Post to Root twice");
    }
#endif
    saPostSetPrevRootPost(_Post, saRootGetLastPost(Root));
    if(saRootGetLastPost(Root) != saPostNull) {
        saPostSetNextRootPost(saRootGetLastPost(Root), _Post);
    }
    saRootSetLastPost(Root, _Post);
    saPostSetNextRootPost(_Post, saPostNull);
    if(saRootGetFirstPost(Root) == saPostNull) {
        saRootSetFirstPost(Root, _Post);
    }
    saPostSetRoot(_Post, Root);
    addRootPostToHashTable(Root, _Post);
}

/*----------------------------------------------------------------------------------------
  Insert the Post to the Root after the previous Post.
----------------------------------------------------------------------------------------*/
void saRootInsertAfterPost(
    saRoot Root,
    saPost prevPost,
    saPost _Post)
{
    saPost nextPost = saPostGetNextRootPost(prevPost);

#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Post == saPostNull) {
        utExit("Non-existent Post");
    }
    if(saPostGetRoot(_Post) != saRootNull) {
        utExit("Attempting to add Post to Root twice");
    }
#endif
    saPostSetNextRootPost(_Post, nextPost);
    saPostSetNextRootPost(prevPost, _Post);
    saPostSetPrevRootPost(_Post, prevPost);
    if(nextPost != saPostNull) {
        saPostSetPrevRootPost(nextPost, _Post);
    }
    if(saRootGetLastPost(Root) == prevPost) {
        saRootSetLastPost(Root, _Post);
    }
    saPostSetRoot(_Post, Root);
    addRootPostToHashTable(Root, _Post);
}

/*----------------------------------------------------------------------------------------
 Remove the Post from the Root.
----------------------------------------------------------------------------------------*/
void saRootRemovePost(
    saRoot Root,
    saPost _Post)
{
    saPost pPost, nPost;

#if defined(DD_DEBUG)
    if(_Post == saPostNull) {
        utExit("Non-existent Post");
    }
    if(saPostGetRoot(_Post) != saRootNull && saPostGetRoot(_Post) != Root) {
        utExit("Delete Post from non-owning Root");
    }
#endif
    nPost = saPostGetNextRootPost(_Post);
    pPost = saPostGetPrevRootPost(_Post);
    if(pPost != saPostNull) {
        saPostSetNextRootPost(pPost, nPost);
    } else if(saRootGetFirstPost(Root) == _Post) {
        saRootSetFirstPost(Root, nPost);
    }
    if(nPost != saPostNull) {
        saPostSetPrevRootPost(nPost, pPost);
    } else if(saRootGetLastPost(Root) == _Post) {
        saRootSetLastPost(Root, pPost);
    }
    saPostSetNextRootPost(_Post, saPostNull);
    saPostSetPrevRootPost(_Post, saPostNull);
    saPostSetRoot(_Post, saRootNull);
    removeRootPostFromHashTable(Root, _Post);
}

/*----------------------------------------------------------------------------------------
  Increase the size of the hash table.
----------------------------------------------------------------------------------------*/
static void resizeRootAnnouncementHashTable(
    saRoot Root)
{
    saAnnouncement _Announcement, prevAnnouncement, nextAnnouncement;
    uint32 oldNumAnnouncements = saRootGetNumAnnouncementTable(Root);
    uint32 newNumAnnouncements = oldNumAnnouncements << 1;
    uint32 xAnnouncement, index;

    if(newNumAnnouncements == 0) {
        newNumAnnouncements = 2;
        saRootAllocAnnouncementTables(Root, 2);
    } else {
        saRootResizeAnnouncementTables(Root, newNumAnnouncements);
    }
    for(xAnnouncement = 0; xAnnouncement < oldNumAnnouncements; xAnnouncement++) {
        _Announcement = saRootGetiAnnouncementTable(Root, xAnnouncement);
        prevAnnouncement = saAnnouncementNull;
        while(_Announcement != saAnnouncementNull) {
            nextAnnouncement = saAnnouncementGetNextTableRootAnnouncement(_Announcement);
            index = (newNumAnnouncements - 1) & (uint32)saAnnouncementGetID(_Announcement);
            if(index != xAnnouncement) {
                if(prevAnnouncement == saAnnouncementNull) {
                    saRootSetiAnnouncementTable(Root, xAnnouncement, nextAnnouncement);
                } else {
                    saAnnouncementSetNextTableRootAnnouncement(prevAnnouncement, nextAnnouncement);
                }
                saAnnouncementSetNextTableRootAnnouncement(_Announcement, saRootGetiAnnouncementTable(Root, index));
                saRootSetiAnnouncementTable(Root, index, _Announcement);
            } else {
                prevAnnouncement = _Announcement;
            }
            _Announcement = nextAnnouncement;
        }
    }
}

/*----------------------------------------------------------------------------------------
  Add the Announcement to the Root.  If the table is near full, build a new one twice
  as big, delete the old one, and return the new one.
----------------------------------------------------------------------------------------*/
static void addRootAnnouncementToHashTable(
    saRoot Root,
    saAnnouncement _Announcement)
{
    saAnnouncement nextAnnouncement;
    uint32 index;

    if(saRootGetNumAnnouncement(Root) >> 1 >= saRootGetNumAnnouncementTable(Root)) {
        resizeRootAnnouncementHashTable(Root);
    }
    index = (saRootGetNumAnnouncementTable(Root) - 1) & (uint32)saAnnouncementGetID(_Announcement);
    nextAnnouncement = saRootGetiAnnouncementTable(Root, index);
    saAnnouncementSetNextTableRootAnnouncement(_Announcement, nextAnnouncement);
    saRootSetiAnnouncementTable(Root, index, _Announcement);
    saRootSetNumAnnouncement(Root, saRootGetNumAnnouncement(Root) + 1);
}

/*----------------------------------------------------------------------------------------
  Remove the Announcement from the hash table.
----------------------------------------------------------------------------------------*/
static void removeRootAnnouncementFromHashTable(
    saRoot Root,
    saAnnouncement _Announcement)
{
    uint32 index = (saRootGetNumAnnouncementTable(Root) - 1) & (uint32)saAnnouncementGetID(_Announcement);
    saAnnouncement prevAnnouncement, nextAnnouncement;
    
    nextAnnouncement = saRootGetiAnnouncementTable(Root, index);
    if(nextAnnouncement == _Announcement) {
        saRootSetiAnnouncementTable(Root, index, saAnnouncementGetNextTableRootAnnouncement(nextAnnouncement));
    } else {
        do {
            prevAnnouncement = nextAnnouncement;
            nextAnnouncement = saAnnouncementGetNextTableRootAnnouncement(nextAnnouncement);
        } while(nextAnnouncement != _Announcement);
        saAnnouncementSetNextTableRootAnnouncement(prevAnnouncement, saAnnouncementGetNextTableRootAnnouncement(_Announcement));
    }
    saRootSetNumAnnouncement(Root, saRootGetNumAnnouncement(Root) - 1);
    saAnnouncementSetNextTableRootAnnouncement(_Announcement, saAnnouncementNull);
}

/*----------------------------------------------------------------------------------------
  Find the Announcement from the Root and its hash key.
----------------------------------------------------------------------------------------*/
saAnnouncement saRootFindAnnouncement(
    saRoot Root,
    uint32 ID)
{
    uint32 mask = saRootGetNumAnnouncementTable(Root) - 1;
    saAnnouncement _Announcement;

    if(mask + 1 != 0) {
        _Announcement = saRootGetiAnnouncementTable(Root, (uint32)ID & mask);
        while(_Announcement != saAnnouncementNull) {
            if(saAnnouncementGetID(_Announcement) == ID) {
                return _Announcement;
            }
            _Announcement = saAnnouncementGetNextTableRootAnnouncement(_Announcement);
        }
    }
    return saAnnouncementNull;
}

/*----------------------------------------------------------------------------------------
  Add the Announcement to the head of the list on the Root.
----------------------------------------------------------------------------------------*/
void saRootInsertAnnouncement(
    saRoot Root,
    saAnnouncement _Announcement)
{
#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Announcement == saAnnouncementNull) {
        utExit("Non-existent Announcement");
    }
    if(saAnnouncementGetRoot(_Announcement) != saRootNull) {
        utExit("Attempting to add Announcement to Root twice");
    }
#endif
    saAnnouncementSetNextRootAnnouncement(_Announcement, saRootGetFirstAnnouncement(Root));
    if(saRootGetFirstAnnouncement(Root) != saAnnouncementNull) {
        saAnnouncementSetPrevRootAnnouncement(saRootGetFirstAnnouncement(Root), _Announcement);
    }
    saRootSetFirstAnnouncement(Root, _Announcement);
    saAnnouncementSetPrevRootAnnouncement(_Announcement, saAnnouncementNull);
    if(saRootGetLastAnnouncement(Root) == saAnnouncementNull) {
        saRootSetLastAnnouncement(Root, _Announcement);
    }
    saAnnouncementSetRoot(_Announcement, Root);
    addRootAnnouncementToHashTable(Root, _Announcement);
}

/*----------------------------------------------------------------------------------------
  Add the Announcement to the end of the list on the Root.
----------------------------------------------------------------------------------------*/
void saRootAppendAnnouncement(
    saRoot Root,
    saAnnouncement _Announcement)
{
#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Announcement == saAnnouncementNull) {
        utExit("Non-existent Announcement");
    }
    if(saAnnouncementGetRoot(_Announcement) != saRootNull) {
        utExit("Attempting to add Announcement to Root twice");
    }
#endif
    saAnnouncementSetPrevRootAnnouncement(_Announcement, saRootGetLastAnnouncement(Root));
    if(saRootGetLastAnnouncement(Root) != saAnnouncementNull) {
        saAnnouncementSetNextRootAnnouncement(saRootGetLastAnnouncement(Root), _Announcement);
    }
    saRootSetLastAnnouncement(Root, _Announcement);
    saAnnouncementSetNextRootAnnouncement(_Announcement, saAnnouncementNull);
    if(saRootGetFirstAnnouncement(Root) == saAnnouncementNull) {
        saRootSetFirstAnnouncement(Root, _Announcement);
    }
    saAnnouncementSetRoot(_Announcement, Root);
    addRootAnnouncementToHashTable(Root, _Announcement);
}

/*----------------------------------------------------------------------------------------
  Insert the Announcement to the Root after the previous Announcement.
----------------------------------------------------------------------------------------*/
void saRootInsertAfterAnnouncement(
    saRoot Root,
    saAnnouncement prevAnnouncement,
    saAnnouncement _Announcement)
{
    saAnnouncement nextAnnouncement = saAnnouncementGetNextRootAnnouncement(prevAnnouncement);

#if defined(DD_DEBUG)
    if(Root == saRootNull) {
        utExit("Non-existent Root");
    }
    if(_Announcement == saAnnouncementNull) {
        utExit("Non-existent Announcement");
    }
    if(saAnnouncementGetRoot(_Announcement) != saRootNull) {
        utExit("Attempting to add Announcement to Root twice");
    }
#endif
    saAnnouncementSetNextRootAnnouncement(_Announcement, nextAnnouncement);
    saAnnouncementSetNextRootAnnouncement(prevAnnouncement, _Announcement);
    saAnnouncementSetPrevRootAnnouncement(_Announcement, prevAnnouncement);
    if(nextAnnouncement != saAnnouncementNull) {
        saAnnouncementSetPrevRootAnnouncement(nextAnnouncement, _Announcement);
    }
    if(saRootGetLastAnnouncement(Root) == prevAnnouncement) {
        saRootSetLastAnnouncement(Root, _Announcement);
    }
    saAnnouncementSetRoot(_Announcement, Root);
    addRootAnnouncementToHashTable(Root, _Announcement);
}

/*----------------------------------------------------------------------------------------
 Remove the Announcement from the Root.
----------------------------------------------------------------------------------------*/
void saRootRemoveAnnouncement(
    saRoot Root,
    saAnnouncement _Announcement)
{
    saAnnouncement pAnnouncement, nAnnouncement;

#if defined(DD_DEBUG)
    if(_Announcement == saAnnouncementNull) {
        utExit("Non-existent Announcement");
    }
    if(saAnnouncementGetRoot(_Announcement) != saRootNull && saAnnouncementGetRoot(_Announcement) != Root) {
        utExit("Delete Announcement from non-owning Root");
    }
#endif
    nAnnouncement = saAnnouncementGetNextRootAnnouncement(_Announcement);
    pAnnouncement = saAnnouncementGetPrevRootAnnouncement(_Announcement);
    if(pAnnouncement != saAnnouncementNull) {
        saAnnouncementSetNextRootAnnouncement(pAnnouncement, nAnnouncement);
    } else if(saRootGetFirstAnnouncement(Root) == _Announcement) {
        saRootSetFirstAnnouncement(Root, nAnnouncement);
    }
    if(nAnnouncement != saAnnouncementNull) {
        saAnnouncementSetPrevRootAnnouncement(nAnnouncement, pAnnouncement);
    } else if(saRootGetLastAnnouncement(Root) == _Announcement) {
        saRootSetLastAnnouncement(Root, pAnnouncement);
    }
    saAnnouncementSetNextRootAnnouncement(_Announcement, saAnnouncementNull);
    saAnnouncementSetPrevRootAnnouncement(_Announcement, saAnnouncementNull);
    saAnnouncementSetRoot(_Announcement, saRootNull);
    removeRootAnnouncementFromHashTable(Root, _Announcement);
}

#if defined(DD_DEBUG)
/*----------------------------------------------------------------------------------------
  Write out all the fields of an object.
----------------------------------------------------------------------------------------*/
void saShowRoot(
    saRoot Root)
{
    utDatabaseShowObject("sa", "Root", saRoot2Index(Root));
}
#endif

/*----------------------------------------------------------------------------------------
  Destroy Region including everything in it. Remove from parents.
----------------------------------------------------------------------------------------*/
void saRegionDestroy(
    saRegion Region)
{
    saUser User_;
    saRoot owningRoot = saRegionGetRoot(Region);
    saSession owningSession = saRegionGetSession(Region);

    if(saRegionDestructorCallback != NULL) {
        saRegionDestructorCallback(Region);
    }
    saSafeForeachRegionUser(Region, User_) {
        saUserDestroy(User_);
    } saEndSafeRegionUser;
    if(owningRoot != saRootNull) {
        saRootRemoveRegion(owningRoot, Region);
    }
    if(owningSession != saSessionNull) {
        saSessionSetRegion(owningSession, saRegionNull);
    }
    saRegionFree(Region);
}

/*----------------------------------------------------------------------------------------
  Default constructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static uint64 allocRegion(void)
{
    saRegion Region = saRegionAlloc();

    return saRegion2Index(Region);
}

/*----------------------------------------------------------------------------------------
  Destructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static void destroyRegion(
    uint64 objectIndex)
{
    saRegionDestroy(saIndex2Region((uint32)objectIndex));
}

/*----------------------------------------------------------------------------------------
  Allocate the field arrays of Region.
----------------------------------------------------------------------------------------*/
static void allocRegions(void)
{
    saSetAllocatedRegion(2);
    saSetUsedRegion(1);
    saSetFirstFreeRegion(saRegionNull);
    saRegions.Sym = utNewAInitFirst(utSym, (saAllocatedRegion()));
    saRegions.NumListings = utNewAInitFirst(uint32, (saAllocatedRegion()));
    saRegions.Root = utNewAInitFirst(saRoot, (saAllocatedRegion()));
    saRegions.NextRootRegion = utNewAInitFirst(saRegion, (saAllocatedRegion()));
    saRegions.PrevRootRegion = utNewAInitFirst(saRegion, (saAllocatedRegion()));
    saRegions.NextTableRootRegion = utNewAInitFirst(saRegion, (saAllocatedRegion()));
    saRegions.FirstUser = utNewAInitFirst(saUser, (saAllocatedRegion()));
    saRegions.LastUser = utNewAInitFirst(saUser, (saAllocatedRegion()));
    saRegions.Session = utNewAInitFirst(saSession, (saAllocatedRegion()));
}

/*----------------------------------------------------------------------------------------
  Realloc the arrays of properties for class Region.
----------------------------------------------------------------------------------------*/
static void reallocRegions(
    uint32 newSize)
{
    utResizeArray(saRegions.Sym, (newSize));
    utResizeArray(saRegions.NumListings, (newSize));
    utResizeArray(saRegions.Root, (newSize));
    utResizeArray(saRegions.NextRootRegion, (newSize));
    utResizeArray(saRegions.PrevRootRegion, (newSize));
    utResizeArray(saRegions.NextTableRootRegion, (newSize));
    utResizeArray(saRegions.FirstUser, (newSize));
    utResizeArray(saRegions.LastUser, (newSize));
    utResizeArray(saRegions.Session, (newSize));
    saSetAllocatedRegion(newSize);
}

/*----------------------------------------------------------------------------------------
  Allocate more Regions.
----------------------------------------------------------------------------------------*/
void saRegionAllocMore(void)
{
    reallocRegions((uint32)(saAllocatedRegion() + (saAllocatedRegion() >> 1)));
}

/*----------------------------------------------------------------------------------------
  Copy the properties of Region.
----------------------------------------------------------------------------------------*/
void saRegionCopyProps(
    saRegion oldRegion,
    saRegion newRegion)
{
    saRegionSetNumListings(newRegion, saRegionGetNumListings(oldRegion));
}

/*----------------------------------------------------------------------------------------
  Add the User to the head of the list on the Region.
----------------------------------------------------------------------------------------*/
void saRegionInsertUser(
    saRegion Region,
    saUser _User)
{
#if defined(DD_DEBUG)
    if(Region == saRegionNull) {
        utExit("Non-existent Region");
    }
    if(_User == saUserNull) {
        utExit("Non-existent User");
    }
    if(saUserGetRegion(_User) != saRegionNull) {
        utExit("Attempting to add User to Region twice");
    }
#endif
    saUserSetNextRegionUser(_User, saRegionGetFirstUser(Region));
    if(saRegionGetFirstUser(Region) != saUserNull) {
        saUserSetPrevRegionUser(saRegionGetFirstUser(Region), _User);
    }
    saRegionSetFirstUser(Region, _User);
    saUserSetPrevRegionUser(_User, saUserNull);
    if(saRegionGetLastUser(Region) == saUserNull) {
        saRegionSetLastUser(Region, _User);
    }
    saUserSetRegion(_User, Region);
}

/*----------------------------------------------------------------------------------------
  Add the User to the end of the list on the Region.
----------------------------------------------------------------------------------------*/
void saRegionAppendUser(
    saRegion Region,
    saUser _User)
{
#if defined(DD_DEBUG)
    if(Region == saRegionNull) {
        utExit("Non-existent Region");
    }
    if(_User == saUserNull) {
        utExit("Non-existent User");
    }
    if(saUserGetRegion(_User) != saRegionNull) {
        utExit("Attempting to add User to Region twice");
    }
#endif
    saUserSetPrevRegionUser(_User, saRegionGetLastUser(Region));
    if(saRegionGetLastUser(Region) != saUserNull) {
        saUserSetNextRegionUser(saRegionGetLastUser(Region), _User);
    }
    saRegionSetLastUser(Region, _User);
    saUserSetNextRegionUser(_User, saUserNull);
    if(saRegionGetFirstUser(Region) == saUserNull) {
        saRegionSetFirstUser(Region, _User);
    }
    saUserSetRegion(_User, Region);
}

/*----------------------------------------------------------------------------------------
  Insert the User to the Region after the previous User.
----------------------------------------------------------------------------------------*/
void saRegionInsertAfterUser(
    saRegion Region,
    saUser prevUser,
    saUser _User)
{
    saUser nextUser = saUserGetNextRegionUser(prevUser);

#if defined(DD_DEBUG)
    if(Region == saRegionNull) {
        utExit("Non-existent Region");
    }
    if(_User == saUserNull) {
        utExit("Non-existent User");
    }
    if(saUserGetRegion(_User) != saRegionNull) {
        utExit("Attempting to add User to Region twice");
    }
#endif
    saUserSetNextRegionUser(_User, nextUser);
    saUserSetNextRegionUser(prevUser, _User);
    saUserSetPrevRegionUser(_User, prevUser);
    if(nextUser != saUserNull) {
        saUserSetPrevRegionUser(nextUser, _User);
    }
    if(saRegionGetLastUser(Region) == prevUser) {
        saRegionSetLastUser(Region, _User);
    }
    saUserSetRegion(_User, Region);
}

/*----------------------------------------------------------------------------------------
 Remove the User from the Region.
----------------------------------------------------------------------------------------*/
void saRegionRemoveUser(
    saRegion Region,
    saUser _User)
{
    saUser pUser, nUser;

#if defined(DD_DEBUG)
    if(_User == saUserNull) {
        utExit("Non-existent User");
    }
    if(saUserGetRegion(_User) != saRegionNull && saUserGetRegion(_User) != Region) {
        utExit("Delete User from non-owning Region");
    }
#endif
    nUser = saUserGetNextRegionUser(_User);
    pUser = saUserGetPrevRegionUser(_User);
    if(pUser != saUserNull) {
        saUserSetNextRegionUser(pUser, nUser);
    } else if(saRegionGetFirstUser(Region) == _User) {
        saRegionSetFirstUser(Region, nUser);
    }
    if(nUser != saUserNull) {
        saUserSetPrevRegionUser(nUser, pUser);
    } else if(saRegionGetLastUser(Region) == _User) {
        saRegionSetLastUser(Region, pUser);
    }
    saUserSetNextRegionUser(_User, saUserNull);
    saUserSetPrevRegionUser(_User, saUserNull);
    saUserSetRegion(_User, saRegionNull);
}

#if defined(DD_DEBUG)
/*----------------------------------------------------------------------------------------
  Write out all the fields of an object.
----------------------------------------------------------------------------------------*/
void saShowRegion(
    saRegion Region)
{
    utDatabaseShowObject("sa", "Region", saRegion2Index(Region));
}
#endif

/*----------------------------------------------------------------------------------------
  Destroy Session including everything in it. Remove from parents.
----------------------------------------------------------------------------------------*/
void saSessionDestroy(
    saSession Session)
{
    saUser User_;
    saRegion Region_;
    saRoot owningRoot = saSessionGetRoot(Session);

    if(saSessionDestructorCallback != NULL) {
        saSessionDestructorCallback(Session);
    }
    User_ = saSessionGetUser(Session);
    if(User_ != saUserNull) {
        saUserSetSession(User_, saSessionNull);
    }
    Region_ = saSessionGetRegion(Session);
    if(Region_ != saRegionNull) {
        saRegionSetSession(Region_, saSessionNull);
    }
    if(owningRoot != saRootNull) {
        saRootRemoveSession(owningRoot, Session);
    }
    saSessionFree(Session);
}

/*----------------------------------------------------------------------------------------
  Default constructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static uint64 allocSession(void)
{
    saSession Session = saSessionAlloc();

    return saSession2Index(Session);
}

/*----------------------------------------------------------------------------------------
  Destructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static void destroySession(
    uint64 objectIndex)
{
    saSessionDestroy(saIndex2Session((uint32)objectIndex));
}

/*----------------------------------------------------------------------------------------
  Allocate the field arrays of Session.
----------------------------------------------------------------------------------------*/
static void allocSessions(void)
{
    saSetAllocatedSession(2);
    saSetUsedSession(1);
    saSetFirstFreeSession(saSessionNull);
    saSessions.Sym = utNewAInitFirst(utSym, (saAllocatedSession()));
    saSessions.Root = utNewAInitFirst(saRoot, (saAllocatedSession()));
    saSessions.NextRootSession = utNewAInitFirst(saSession, (saAllocatedSession()));
    saSessions.PrevRootSession = utNewAInitFirst(saSession, (saAllocatedSession()));
    saSessions.NextTableRootSession = utNewAInitFirst(saSession, (saAllocatedSession()));
    saSessions.User = utNewAInitFirst(saUser, (saAllocatedSession()));
    saSessions.Region = utNewAInitFirst(saRegion, (saAllocatedSession()));
}

/*----------------------------------------------------------------------------------------
  Realloc the arrays of properties for class Session.
----------------------------------------------------------------------------------------*/
static void reallocSessions(
    uint32 newSize)
{
    utResizeArray(saSessions.Sym, (newSize));
    utResizeArray(saSessions.Root, (newSize));
    utResizeArray(saSessions.NextRootSession, (newSize));
    utResizeArray(saSessions.PrevRootSession, (newSize));
    utResizeArray(saSessions.NextTableRootSession, (newSize));
    utResizeArray(saSessions.User, (newSize));
    utResizeArray(saSessions.Region, (newSize));
    saSetAllocatedSession(newSize);
}

/*----------------------------------------------------------------------------------------
  Allocate more Sessions.
----------------------------------------------------------------------------------------*/
void saSessionAllocMore(void)
{
    reallocSessions((uint32)(saAllocatedSession() + (saAllocatedSession() >> 1)));
}

/*----------------------------------------------------------------------------------------
  Copy the properties of Session.
----------------------------------------------------------------------------------------*/
void saSessionCopyProps(
    saSession oldSession,
    saSession newSession)
{
}

#if defined(DD_DEBUG)
/*----------------------------------------------------------------------------------------
  Write out all the fields of an object.
----------------------------------------------------------------------------------------*/
void saShowSession(
    saSession Session)
{
    utDatabaseShowObject("sa", "Session", saSession2Index(Session));
}
#endif

/*----------------------------------------------------------------------------------------
  Destroy User including everything in it. Remove from parents.
----------------------------------------------------------------------------------------*/
void saUserDestroy(
    saUser User)
{
    saCharity Charity_;
    saMembership Membership_;
    saTransaction OutTransaction_;
    saTransaction InTransaction_;
    saListing Listing_;
    saPost Post_;
    saSubscription Subscription_;
    saAnnouncement Announcement_;
    saRoot owningRoot = saUserGetRoot(User);
    saRoot owningByEmailRoot = saUserGetByEmailRoot(User);
    saRegion owningRegion = saUserGetRegion(User);
    saSession owningSession = saUserGetSession(User);

    if(saUserDestructorCallback != NULL) {
        saUserDestructorCallback(User);
    }
    Charity_ = saUserGetCharity(User);
    if(Charity_ != saCharityNull) {
        saCharityDestroy(Charity_);
    }
    saSafeForeachUserMembership(User, Membership_) {
        saMembershipDestroy(Membership_);
    } saEndSafeUserMembership;
    saSafeForeachUserOutTransaction(User, OutTransaction_) {
        saTransactionDestroy(OutTransaction_);
    } saEndSafeUserOutTransaction;
    saSafeForeachUserInTransaction(User, InTransaction_) {
        saTransactionDestroy(InTransaction_);
    } saEndSafeUserInTransaction;
    saSafeForeachUserListing(User, Listing_) {
        saListingDestroy(Listing_);
    } saEndSafeUserListing;
    saSafeForeachUserPost(User, Post_) {
        saPostDestroy(Post_);
    } saEndSafeUserPost;
    saSafeForeachUserSubscription(User, Subscription_) {
        saSubscriptionDestroy(Subscription_);
    } saEndSafeUserSubscription;
    saSafeForeachUserAnnouncement(User, Announcement_) {
        saAnnouncementDestroy(Announcement_);
    } saEndSafeUserAnnouncement;
    if(owningRoot != saRootNull) {
        saRootRemoveUser(owningRoot, User);
    }
    if(owningByEmailRoot != saRootNull) {
        saRootRemoveByEmailUser(owningByEmailRoot, User);
    }
    if(owningRegion != saRegionNull) {
        saRegionRemoveUser(owningRegion, User);
#if defined(DD_DEBUG)
    } else {
        utExit("User without owning Region");
#endif
    }
    if(owningSession != saSessionNull) {
        saSessionSetUser(owningSession, saUserNull);
    }
    saUserFree(User);
}

/*----------------------------------------------------------------------------------------
  Default constructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static uint64 allocUser(void)
{
    saUser User = saUserAlloc();

    return saUser2Index(User);
}

/*----------------------------------------------------------------------------------------
  Destructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static void destroyUser(
    uint64 objectIndex)
{
    saUserDestroy(saIndex2User((uint32)objectIndex));
}

/*----------------------------------------------------------------------------------------
  Allocate the field arrays of User.
----------------------------------------------------------------------------------------*/
static void allocUsers(void)
{
    saSetAllocatedUser(2);
    saSetUsedUser(1);
    saSetFirstFreeUser(saUserNull);
    saUsers.Sym = utNewAInitFirst(utSym, (saAllocatedUser()));
    saUsers.ShownNameIndex_ = utNewAInitFirst(uint32, (saAllocatedUser()));
    saUsers.NumShownName = utNewAInitFirst(uint32, (saAllocatedUser()));
    saSetUsedUserShownName(0);
    saSetAllocatedUserShownName(2);
    saSetFreeUserShownName(0);
    saUsers.ShownName = utNewAInitFirst(char, saAllocatedUserShownName());
    saUsers.Email = utNewAInitFirst(utSym, (saAllocatedUser()));
    saUsers.JoinDate = utNewAInitFirst(uint64, (saAllocatedUser()));
    saUsers.Active = utNewAInitFirst(uint8, (saAllocatedUser()));
    saUsers.HashedPasswordIndex_ = utNewAInitFirst(uint32, (saAllocatedUser()));
    saUsers.NumHashedPassword = utNewAInitFirst(uint32, (saAllocatedUser()));
    saSetUsedUserHashedPassword(0);
    saSetAllocatedUserHashedPassword(2);
    saSetFreeUserHashedPassword(0);
    saUsers.HashedPassword = utNewAInitFirst(uint8, saAllocatedUserHashedPassword());
    saUsers.NounceIndex_ = utNewAInitFirst(uint32, (saAllocatedUser()));
    saUsers.NumNounce = utNewAInitFirst(uint32, (saAllocatedUser()));
    saSetUsedUserNounce(0);
    saSetAllocatedUserNounce(2);
    saSetFreeUserNounce(0);
    saUsers.Nounce = utNewAInitFirst(uint8, saAllocatedUserNounce());
    saUsers.LoggedIn = utNewAInitFirst(uint8, (saAllocatedUser()));
    saUsers.LastActivity = utNewAInitFirst(uint64, (saAllocatedUser()));
    saUsers.Balance = utNewAInitFirst(uint32, (saAllocatedUser()));
    saUsers.Validated = utNewAInitFirst(uint8, (saAllocatedUser()));
    saUsers.ValidationKeyIndex_ = utNewAInitFirst(uint32, (saAllocatedUser()));
    saUsers.NumValidationKey = utNewAInitFirst(uint32, (saAllocatedUser()));
    saSetUsedUserValidationKey(0);
    saSetAllocatedUserValidationKey(2);
    saSetFreeUserValidationKey(0);
    saUsers.ValidationKey = utNewAInitFirst(uint8, saAllocatedUserValidationKey());
    saUsers.SupremeLeader = utNewAInitFirst(uint8, (saAllocatedUser()));
    saUsers.RegionManager = utNewAInitFirst(uint8, (saAllocatedUser()));
    saUsers.Root = utNewAInitFirst(saRoot, (saAllocatedUser()));
    saUsers.NextRootUser = utNewAInitFirst(saUser, (saAllocatedUser()));
    saUsers.PrevRootUser = utNewAInitFirst(saUser, (saAllocatedUser()));
    saUsers.NextTableRootUser = utNewAInitFirst(saUser, (saAllocatedUser()));
    saUsers.ByEmailRoot = utNewAInitFirst(saRoot, (saAllocatedUser()));
    saUsers.NextRootByEmailUser = utNewAInitFirst(saUser, (saAllocatedUser()));
    saUsers.PrevRootByEmailUser = utNewAInitFirst(saUser, (saAllocatedUser()));
    saUsers.NextTableRootByEmailUser = utNewAInitFirst(saUser, (saAllocatedUser()));
    saUsers.Region = utNewAInitFirst(saRegion, (saAllocatedUser()));
    saUsers.NextRegionUser = utNewAInitFirst(saUser, (saAllocatedUser()));
    saUsers.PrevRegionUser = utNewAInitFirst(saUser, (saAllocatedUser()));
    saUsers.Session = utNewAInitFirst(saSession, (saAllocatedUser()));
    saUsers.Charity = utNewAInitFirst(saCharity, (saAllocatedUser()));
    saUsers.FirstMembership = utNewAInitFirst(saMembership, (saAllocatedUser()));
    saUsers.LastMembership = utNewAInitFirst(saMembership, (saAllocatedUser()));
    saUsers.MembershipTableIndex_ = utNewAInitFirst(uint32, (saAllocatedUser()));
    saUsers.NumMembershipTable = utNewAInitFirst(uint32, (saAllocatedUser()));
    saSetUsedUserMembershipTable(0);
    saSetAllocatedUserMembershipTable(2);
    saSetFreeUserMembershipTable(0);
    saUsers.MembershipTable = utNewAInitFirst(saMembership, saAllocatedUserMembershipTable());
    saUsers.NumMembership = utNewAInitFirst(uint32, (saAllocatedUser()));
    saUsers.FirstOutTransaction = utNewAInitFirst(saTransaction, (saAllocatedUser()));
    saUsers.LastOutTransaction = utNewAInitFirst(saTransaction, (saAllocatedUser()));
    saUsers.FirstInTransaction = utNewAInitFirst(saTransaction, (saAllocatedUser()));
    saUsers.LastInTransaction = utNewAInitFirst(saTransaction, (saAllocatedUser()));
    saUsers.FirstListing = utNewAInitFirst(saListing, (saAllocatedUser()));
    saUsers.LastListing = utNewAInitFirst(saListing, (saAllocatedUser()));
    saUsers.FirstPost = utNewAInitFirst(saPost, (saAllocatedUser()));
    saUsers.LastPost = utNewAInitFirst(saPost, (saAllocatedUser()));
    saUsers.PostTableIndex_ = utNewAInitFirst(uint32, (saAllocatedUser()));
    saUsers.NumPostTable = utNewAInitFirst(uint32, (saAllocatedUser()));
    saSetUsedUserPostTable(0);
    saSetAllocatedUserPostTable(2);
    saSetFreeUserPostTable(0);
    saUsers.PostTable = utNewAInitFirst(saPost, saAllocatedUserPostTable());
    saUsers.NumPost = utNewAInitFirst(uint32, (saAllocatedUser()));
    saUsers.FirstSubscription = utNewAInitFirst(saSubscription, (saAllocatedUser()));
    saUsers.LastSubscription = utNewAInitFirst(saSubscription, (saAllocatedUser()));
    saUsers.FirstAnnouncement = utNewAInitFirst(saAnnouncement, (saAllocatedUser()));
    saUsers.LastAnnouncement = utNewAInitFirst(saAnnouncement, (saAllocatedUser()));
}

/*----------------------------------------------------------------------------------------
  Realloc the arrays of properties for class User.
----------------------------------------------------------------------------------------*/
static void reallocUsers(
    uint32 newSize)
{
    utResizeArray(saUsers.Sym, (newSize));
    utResizeArray(saUsers.ShownNameIndex_, (newSize));
    utResizeArray(saUsers.NumShownName, (newSize));
    utResizeArray(saUsers.Email, (newSize));
    utResizeArray(saUsers.JoinDate, (newSize));
    utResizeArray(saUsers.Active, (newSize));
    utResizeArray(saUsers.HashedPasswordIndex_, (newSize));
    utResizeArray(saUsers.NumHashedPassword, (newSize));
    utResizeArray(saUsers.NounceIndex_, (newSize));
    utResizeArray(saUsers.NumNounce, (newSize));
    utResizeArray(saUsers.LoggedIn, (newSize));
    utResizeArray(saUsers.LastActivity, (newSize));
    utResizeArray(saUsers.Balance, (newSize));
    utResizeArray(saUsers.Validated, (newSize));
    utResizeArray(saUsers.ValidationKeyIndex_, (newSize));
    utResizeArray(saUsers.NumValidationKey, (newSize));
    utResizeArray(saUsers.SupremeLeader, (newSize));
    utResizeArray(saUsers.RegionManager, (newSize));
    utResizeArray(saUsers.Root, (newSize));
    utResizeArray(saUsers.NextRootUser, (newSize));
    utResizeArray(saUsers.PrevRootUser, (newSize));
    utResizeArray(saUsers.NextTableRootUser, (newSize));
    utResizeArray(saUsers.ByEmailRoot, (newSize));
    utResizeArray(saUsers.NextRootByEmailUser, (newSize));
    utResizeArray(saUsers.PrevRootByEmailUser, (newSize));
    utResizeArray(saUsers.NextTableRootByEmailUser, (newSize));
    utResizeArray(saUsers.Region, (newSize));
    utResizeArray(saUsers.NextRegionUser, (newSize));
    utResizeArray(saUsers.PrevRegionUser, (newSize));
    utResizeArray(saUsers.Session, (newSize));
    utResizeArray(saUsers.Charity, (newSize));
    utResizeArray(saUsers.FirstMembership, (newSize));
    utResizeArray(saUsers.LastMembership, (newSize));
    utResizeArray(saUsers.MembershipTableIndex_, (newSize));
    utResizeArray(saUsers.NumMembershipTable, (newSize));
    utResizeArray(saUsers.NumMembership, (newSize));
    utResizeArray(saUsers.FirstOutTransaction, (newSize));
    utResizeArray(saUsers.LastOutTransaction, (newSize));
    utResizeArray(saUsers.FirstInTransaction, (newSize));
    utResizeArray(saUsers.LastInTransaction, (newSize));
    utResizeArray(saUsers.FirstListing, (newSize));
    utResizeArray(saUsers.LastListing, (newSize));
    utResizeArray(saUsers.FirstPost, (newSize));
    utResizeArray(saUsers.LastPost, (newSize));
    utResizeArray(saUsers.PostTableIndex_, (newSize));
    utResizeArray(saUsers.NumPostTable, (newSize));
    utResizeArray(saUsers.NumPost, (newSize));
    utResizeArray(saUsers.FirstSubscription, (newSize));
    utResizeArray(saUsers.LastSubscription, (newSize));
    utResizeArray(saUsers.FirstAnnouncement, (newSize));
    utResizeArray(saUsers.LastAnnouncement, (newSize));
    saSetAllocatedUser(newSize);
}

/*----------------------------------------------------------------------------------------
  Allocate more Users.
----------------------------------------------------------------------------------------*/
void saUserAllocMore(void)
{
    reallocUsers((uint32)(saAllocatedUser() + (saAllocatedUser() >> 1)));
}

/*----------------------------------------------------------------------------------------
  Compact the User.ShownName heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactUserShownNames(void)
{
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    char *toPtr = saUsers.ShownName;
    char *fromPtr = toPtr;
    saUser User;
    uint32 size;

    while(fromPtr < saUsers.ShownName + saUsedUserShownName()) {
        User = *(saUser *)(void *)fromPtr;
        if(User != saUserNull) {
            /* Need to move it to toPtr */
            size = utMax(saUserGetNumShownName(User) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saUserSetShownNameIndex_(User, toPtr - saUsers.ShownName + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saUser *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedUserShownName(toPtr - saUsers.ShownName);
    saSetFreeUserShownName(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the User.ShownName heap.
----------------------------------------------------------------------------------------*/
static void allocMoreUserShownNames(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedUserShownName() - saUsedUserShownName();

    if((saFreeUserShownName() << 2) > saUsedUserShownName()) {
        saCompactUserShownNames();
        freeSpace = saAllocatedUserShownName() - saUsedUserShownName();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedUserShownName(saAllocatedUserShownName() + spaceNeeded - freeSpace +
            (saAllocatedUserShownName() >> 1));
        utResizeArray(saUsers.ShownName, saAllocatedUserShownName());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new User.ShownName array.
----------------------------------------------------------------------------------------*/
void saUserAllocShownNames(
    saUser User,
    uint32 numShownNames)
{
    uint32 freeSpace = saAllocatedUserShownName() - saUsedUserShownName();
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numShownNames + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saUserGetNumShownName(User) == 0);
#endif
    if(numShownNames == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMoreUserShownNames(spaceNeeded);
    }
    saUserSetShownNameIndex_(User, saUsedUserShownName() + usedHeaderSize);
    saUserSetNumShownName(User, numShownNames);
    *(saUser *)(void *)(saUsers.ShownName + saUsedUserShownName()) = User;
    memset(saUsers.ShownName + saUserGetShownNameIndex_(User), 0, ((numShownNames))*sizeof(char));
    saSetUsedUserShownName(saUsedUserShownName() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saUserGetShownNames for the database manager.
----------------------------------------------------------------------------------------*/
static void *getUserShownNames(
    uint64 objectNumber,
    uint32 *numValues)
{
    saUser User = saIndex2User((uint32)objectNumber);

    *numValues = saUserGetNumShownName(User);
    return saUserGetShownNames(User);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saUserAllocShownNames for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocUserShownNames(
    uint64 objectNumber,
    uint32 numValues)
{
    saUser User = saIndex2User((uint32)objectNumber);

    saUserSetShownNameIndex_(User, 0);
    saUserSetNumShownName(User, 0);
    if(numValues == 0) {
        return NULL;
    }
    saUserAllocShownNames(User, numValues);
    return saUserGetShownNames(User);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the User.ShownName array.
----------------------------------------------------------------------------------------*/
void saUserFreeShownNames(
    saUser User)
{
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saUserGetNumShownName(User) + usedHeaderSize, freeHeaderSize);
    char *dataPtr = saUserGetShownNames(User) - usedHeaderSize;

    if(saUserGetNumShownName(User) == 0) {
        return;
    }
    *(saUser *)(void *)(dataPtr) = saUserNull;
    *(uint32 *)(void *)(((saUser *)(void *)dataPtr) + 1) = size;
    saUserSetNumShownName(User, 0);
    saSetFreeUserShownName(saFreeUserShownName() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the User.ShownName array.
----------------------------------------------------------------------------------------*/
void saUserResizeShownNames(
    saUser User,
    uint32 numShownNames)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numShownNames + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saUserGetNumShownName(User) + usedHeaderSize, freeHeaderSize);
    char *dataPtr;

    if(numShownNames == 0) {
        if(saUserGetNumShownName(User) != 0) {
            saUserFreeShownNames(User);
        }
        return;
    }
    if(saUserGetNumShownName(User) == 0) {
        saUserAllocShownNames(User, numShownNames);
        return;
    }
    freeSpace = saAllocatedUserShownName() - saUsedUserShownName();
    if(freeSpace < newSize) {
        allocMoreUserShownNames(newSize);
    }
    dataPtr = saUserGetShownNames(User) - usedHeaderSize;
    memcpy((void *)(saUsers.ShownName + saUsedUserShownName()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        memset(saUsers.ShownName + saUsedUserShownName() + oldSize, 0, ((newSize - oldSize))*sizeof(char));
    }
    *(saUser *)(void *)dataPtr = saUserNull;
    *(uint32 *)(void *)(((saUser *)(void *)dataPtr) + 1) = oldSize;
    saSetFreeUserShownName(saFreeUserShownName() + oldSize);
    saUserSetShownNameIndex_(User, saUsedUserShownName() + usedHeaderSize);
    saUserSetNumShownName(User, numShownNames);
    saSetUsedUserShownName(saUsedUserShownName() + newSize);
}

/*----------------------------------------------------------------------------------------
  Compact the User.HashedPassword heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactUserHashedPasswords(void)
{
    uint32 elementSize = sizeof(uint8);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint8 *toPtr = saUsers.HashedPassword;
    uint8 *fromPtr = toPtr;
    saUser User;
    uint32 size;

    while(fromPtr < saUsers.HashedPassword + saUsedUserHashedPassword()) {
        User = *(saUser *)(void *)fromPtr;
        if(User != saUserNull) {
            /* Need to move it to toPtr */
            size = utMax(saUserGetNumHashedPassword(User) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saUserSetHashedPasswordIndex_(User, toPtr - saUsers.HashedPassword + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saUser *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedUserHashedPassword(toPtr - saUsers.HashedPassword);
    saSetFreeUserHashedPassword(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the User.HashedPassword heap.
----------------------------------------------------------------------------------------*/
static void allocMoreUserHashedPasswords(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedUserHashedPassword() - saUsedUserHashedPassword();

    if((saFreeUserHashedPassword() << 2) > saUsedUserHashedPassword()) {
        saCompactUserHashedPasswords();
        freeSpace = saAllocatedUserHashedPassword() - saUsedUserHashedPassword();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedUserHashedPassword(saAllocatedUserHashedPassword() + spaceNeeded - freeSpace +
            (saAllocatedUserHashedPassword() >> 1));
        utResizeArray(saUsers.HashedPassword, saAllocatedUserHashedPassword());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new User.HashedPassword array.
----------------------------------------------------------------------------------------*/
void saUserAllocHashedPasswords(
    saUser User,
    uint32 numHashedPasswords)
{
    uint32 freeSpace = saAllocatedUserHashedPassword() - saUsedUserHashedPassword();
    uint32 elementSize = sizeof(uint8);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numHashedPasswords + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saUserGetNumHashedPassword(User) == 0);
#endif
    if(numHashedPasswords == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMoreUserHashedPasswords(spaceNeeded);
    }
    saUserSetHashedPasswordIndex_(User, saUsedUserHashedPassword() + usedHeaderSize);
    saUserSetNumHashedPassword(User, numHashedPasswords);
    *(saUser *)(void *)(saUsers.HashedPassword + saUsedUserHashedPassword()) = User;
    memset(saUsers.HashedPassword + saUserGetHashedPasswordIndex_(User), 0, ((numHashedPasswords))*sizeof(uint8));
    saSetUsedUserHashedPassword(saUsedUserHashedPassword() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saUserGetHashedPasswords for the database manager.
----------------------------------------------------------------------------------------*/
static void *getUserHashedPasswords(
    uint64 objectNumber,
    uint32 *numValues)
{
    saUser User = saIndex2User((uint32)objectNumber);

    *numValues = saUserGetNumHashedPassword(User);
    return saUserGetHashedPasswords(User);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saUserAllocHashedPasswords for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocUserHashedPasswords(
    uint64 objectNumber,
    uint32 numValues)
{
    saUser User = saIndex2User((uint32)objectNumber);

    saUserSetHashedPasswordIndex_(User, 0);
    saUserSetNumHashedPassword(User, 0);
    if(numValues == 0) {
        return NULL;
    }
    saUserAllocHashedPasswords(User, numValues);
    return saUserGetHashedPasswords(User);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the User.HashedPassword array.
----------------------------------------------------------------------------------------*/
void saUserFreeHashedPasswords(
    saUser User)
{
    uint32 elementSize = sizeof(uint8);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saUserGetNumHashedPassword(User) + usedHeaderSize, freeHeaderSize);
    uint8 *dataPtr = saUserGetHashedPasswords(User) - usedHeaderSize;

    if(saUserGetNumHashedPassword(User) == 0) {
        return;
    }
    *(saUser *)(void *)(dataPtr) = saUserNull;
    *(uint32 *)(void *)(((saUser *)(void *)dataPtr) + 1) = size;
    saUserSetNumHashedPassword(User, 0);
    saSetFreeUserHashedPassword(saFreeUserHashedPassword() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the User.HashedPassword array.
----------------------------------------------------------------------------------------*/
void saUserResizeHashedPasswords(
    saUser User,
    uint32 numHashedPasswords)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(uint8);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numHashedPasswords + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saUserGetNumHashedPassword(User) + usedHeaderSize, freeHeaderSize);
    uint8 *dataPtr;

    if(numHashedPasswords == 0) {
        if(saUserGetNumHashedPassword(User) != 0) {
            saUserFreeHashedPasswords(User);
        }
        return;
    }
    if(saUserGetNumHashedPassword(User) == 0) {
        saUserAllocHashedPasswords(User, numHashedPasswords);
        return;
    }
    freeSpace = saAllocatedUserHashedPassword() - saUsedUserHashedPassword();
    if(freeSpace < newSize) {
        allocMoreUserHashedPasswords(newSize);
    }
    dataPtr = saUserGetHashedPasswords(User) - usedHeaderSize;
    memcpy((void *)(saUsers.HashedPassword + saUsedUserHashedPassword()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        memset(saUsers.HashedPassword + saUsedUserHashedPassword() + oldSize, 0, ((newSize - oldSize))*sizeof(uint8));
    }
    *(saUser *)(void *)dataPtr = saUserNull;
    *(uint32 *)(void *)(((saUser *)(void *)dataPtr) + 1) = oldSize;
    saSetFreeUserHashedPassword(saFreeUserHashedPassword() + oldSize);
    saUserSetHashedPasswordIndex_(User, saUsedUserHashedPassword() + usedHeaderSize);
    saUserSetNumHashedPassword(User, numHashedPasswords);
    saSetUsedUserHashedPassword(saUsedUserHashedPassword() + newSize);
}

/*----------------------------------------------------------------------------------------
  Compact the User.Nounce heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactUserNounces(void)
{
    uint32 elementSize = sizeof(uint8);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint8 *toPtr = saUsers.Nounce;
    uint8 *fromPtr = toPtr;
    saUser User;
    uint32 size;

    while(fromPtr < saUsers.Nounce + saUsedUserNounce()) {
        User = *(saUser *)(void *)fromPtr;
        if(User != saUserNull) {
            /* Need to move it to toPtr */
            size = utMax(saUserGetNumNounce(User) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saUserSetNounceIndex_(User, toPtr - saUsers.Nounce + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saUser *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedUserNounce(toPtr - saUsers.Nounce);
    saSetFreeUserNounce(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the User.Nounce heap.
----------------------------------------------------------------------------------------*/
static void allocMoreUserNounces(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedUserNounce() - saUsedUserNounce();

    if((saFreeUserNounce() << 2) > saUsedUserNounce()) {
        saCompactUserNounces();
        freeSpace = saAllocatedUserNounce() - saUsedUserNounce();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedUserNounce(saAllocatedUserNounce() + spaceNeeded - freeSpace +
            (saAllocatedUserNounce() >> 1));
        utResizeArray(saUsers.Nounce, saAllocatedUserNounce());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new User.Nounce array.
----------------------------------------------------------------------------------------*/
void saUserAllocNounces(
    saUser User,
    uint32 numNounces)
{
    uint32 freeSpace = saAllocatedUserNounce() - saUsedUserNounce();
    uint32 elementSize = sizeof(uint8);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numNounces + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saUserGetNumNounce(User) == 0);
#endif
    if(numNounces == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMoreUserNounces(spaceNeeded);
    }
    saUserSetNounceIndex_(User, saUsedUserNounce() + usedHeaderSize);
    saUserSetNumNounce(User, numNounces);
    *(saUser *)(void *)(saUsers.Nounce + saUsedUserNounce()) = User;
    memset(saUsers.Nounce + saUserGetNounceIndex_(User), 0, ((numNounces))*sizeof(uint8));
    saSetUsedUserNounce(saUsedUserNounce() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saUserGetNounces for the database manager.
----------------------------------------------------------------------------------------*/
static void *getUserNounces(
    uint64 objectNumber,
    uint32 *numValues)
{
    saUser User = saIndex2User((uint32)objectNumber);

    *numValues = saUserGetNumNounce(User);
    return saUserGetNounces(User);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saUserAllocNounces for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocUserNounces(
    uint64 objectNumber,
    uint32 numValues)
{
    saUser User = saIndex2User((uint32)objectNumber);

    saUserSetNounceIndex_(User, 0);
    saUserSetNumNounce(User, 0);
    if(numValues == 0) {
        return NULL;
    }
    saUserAllocNounces(User, numValues);
    return saUserGetNounces(User);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the User.Nounce array.
----------------------------------------------------------------------------------------*/
void saUserFreeNounces(
    saUser User)
{
    uint32 elementSize = sizeof(uint8);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saUserGetNumNounce(User) + usedHeaderSize, freeHeaderSize);
    uint8 *dataPtr = saUserGetNounces(User) - usedHeaderSize;

    if(saUserGetNumNounce(User) == 0) {
        return;
    }
    *(saUser *)(void *)(dataPtr) = saUserNull;
    *(uint32 *)(void *)(((saUser *)(void *)dataPtr) + 1) = size;
    saUserSetNumNounce(User, 0);
    saSetFreeUserNounce(saFreeUserNounce() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the User.Nounce array.
----------------------------------------------------------------------------------------*/
void saUserResizeNounces(
    saUser User,
    uint32 numNounces)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(uint8);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numNounces + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saUserGetNumNounce(User) + usedHeaderSize, freeHeaderSize);
    uint8 *dataPtr;

    if(numNounces == 0) {
        if(saUserGetNumNounce(User) != 0) {
            saUserFreeNounces(User);
        }
        return;
    }
    if(saUserGetNumNounce(User) == 0) {
        saUserAllocNounces(User, numNounces);
        return;
    }
    freeSpace = saAllocatedUserNounce() - saUsedUserNounce();
    if(freeSpace < newSize) {
        allocMoreUserNounces(newSize);
    }
    dataPtr = saUserGetNounces(User) - usedHeaderSize;
    memcpy((void *)(saUsers.Nounce + saUsedUserNounce()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        memset(saUsers.Nounce + saUsedUserNounce() + oldSize, 0, ((newSize - oldSize))*sizeof(uint8));
    }
    *(saUser *)(void *)dataPtr = saUserNull;
    *(uint32 *)(void *)(((saUser *)(void *)dataPtr) + 1) = oldSize;
    saSetFreeUserNounce(saFreeUserNounce() + oldSize);
    saUserSetNounceIndex_(User, saUsedUserNounce() + usedHeaderSize);
    saUserSetNumNounce(User, numNounces);
    saSetUsedUserNounce(saUsedUserNounce() + newSize);
}

/*----------------------------------------------------------------------------------------
  Compact the User.ValidationKey heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactUserValidationKeys(void)
{
    uint32 elementSize = sizeof(uint8);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint8 *toPtr = saUsers.ValidationKey;
    uint8 *fromPtr = toPtr;
    saUser User;
    uint32 size;

    while(fromPtr < saUsers.ValidationKey + saUsedUserValidationKey()) {
        User = *(saUser *)(void *)fromPtr;
        if(User != saUserNull) {
            /* Need to move it to toPtr */
            size = utMax(saUserGetNumValidationKey(User) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saUserSetValidationKeyIndex_(User, toPtr - saUsers.ValidationKey + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saUser *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedUserValidationKey(toPtr - saUsers.ValidationKey);
    saSetFreeUserValidationKey(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the User.ValidationKey heap.
----------------------------------------------------------------------------------------*/
static void allocMoreUserValidationKeys(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedUserValidationKey() - saUsedUserValidationKey();

    if((saFreeUserValidationKey() << 2) > saUsedUserValidationKey()) {
        saCompactUserValidationKeys();
        freeSpace = saAllocatedUserValidationKey() - saUsedUserValidationKey();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedUserValidationKey(saAllocatedUserValidationKey() + spaceNeeded - freeSpace +
            (saAllocatedUserValidationKey() >> 1));
        utResizeArray(saUsers.ValidationKey, saAllocatedUserValidationKey());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new User.ValidationKey array.
----------------------------------------------------------------------------------------*/
void saUserAllocValidationKeys(
    saUser User,
    uint32 numValidationKeys)
{
    uint32 freeSpace = saAllocatedUserValidationKey() - saUsedUserValidationKey();
    uint32 elementSize = sizeof(uint8);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numValidationKeys + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saUserGetNumValidationKey(User) == 0);
#endif
    if(numValidationKeys == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMoreUserValidationKeys(spaceNeeded);
    }
    saUserSetValidationKeyIndex_(User, saUsedUserValidationKey() + usedHeaderSize);
    saUserSetNumValidationKey(User, numValidationKeys);
    *(saUser *)(void *)(saUsers.ValidationKey + saUsedUserValidationKey()) = User;
    memset(saUsers.ValidationKey + saUserGetValidationKeyIndex_(User), 0, ((numValidationKeys))*sizeof(uint8));
    saSetUsedUserValidationKey(saUsedUserValidationKey() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saUserGetValidationKeys for the database manager.
----------------------------------------------------------------------------------------*/
static void *getUserValidationKeys(
    uint64 objectNumber,
    uint32 *numValues)
{
    saUser User = saIndex2User((uint32)objectNumber);

    *numValues = saUserGetNumValidationKey(User);
    return saUserGetValidationKeys(User);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saUserAllocValidationKeys for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocUserValidationKeys(
    uint64 objectNumber,
    uint32 numValues)
{
    saUser User = saIndex2User((uint32)objectNumber);

    saUserSetValidationKeyIndex_(User, 0);
    saUserSetNumValidationKey(User, 0);
    if(numValues == 0) {
        return NULL;
    }
    saUserAllocValidationKeys(User, numValues);
    return saUserGetValidationKeys(User);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the User.ValidationKey array.
----------------------------------------------------------------------------------------*/
void saUserFreeValidationKeys(
    saUser User)
{
    uint32 elementSize = sizeof(uint8);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saUserGetNumValidationKey(User) + usedHeaderSize, freeHeaderSize);
    uint8 *dataPtr = saUserGetValidationKeys(User) - usedHeaderSize;

    if(saUserGetNumValidationKey(User) == 0) {
        return;
    }
    *(saUser *)(void *)(dataPtr) = saUserNull;
    *(uint32 *)(void *)(((saUser *)(void *)dataPtr) + 1) = size;
    saUserSetNumValidationKey(User, 0);
    saSetFreeUserValidationKey(saFreeUserValidationKey() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the User.ValidationKey array.
----------------------------------------------------------------------------------------*/
void saUserResizeValidationKeys(
    saUser User,
    uint32 numValidationKeys)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(uint8);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numValidationKeys + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saUserGetNumValidationKey(User) + usedHeaderSize, freeHeaderSize);
    uint8 *dataPtr;

    if(numValidationKeys == 0) {
        if(saUserGetNumValidationKey(User) != 0) {
            saUserFreeValidationKeys(User);
        }
        return;
    }
    if(saUserGetNumValidationKey(User) == 0) {
        saUserAllocValidationKeys(User, numValidationKeys);
        return;
    }
    freeSpace = saAllocatedUserValidationKey() - saUsedUserValidationKey();
    if(freeSpace < newSize) {
        allocMoreUserValidationKeys(newSize);
    }
    dataPtr = saUserGetValidationKeys(User) - usedHeaderSize;
    memcpy((void *)(saUsers.ValidationKey + saUsedUserValidationKey()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        memset(saUsers.ValidationKey + saUsedUserValidationKey() + oldSize, 0, ((newSize - oldSize))*sizeof(uint8));
    }
    *(saUser *)(void *)dataPtr = saUserNull;
    *(uint32 *)(void *)(((saUser *)(void *)dataPtr) + 1) = oldSize;
    saSetFreeUserValidationKey(saFreeUserValidationKey() + oldSize);
    saUserSetValidationKeyIndex_(User, saUsedUserValidationKey() + usedHeaderSize);
    saUserSetNumValidationKey(User, numValidationKeys);
    saSetUsedUserValidationKey(saUsedUserValidationKey() + newSize);
}

/*----------------------------------------------------------------------------------------
  Compact the User.MembershipTable heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactUserMembershipTables(void)
{
    uint32 elementSize = sizeof(saMembership);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    saMembership *toPtr = saUsers.MembershipTable;
    saMembership *fromPtr = toPtr;
    saUser User;
    uint32 size;

    while(fromPtr < saUsers.MembershipTable + saUsedUserMembershipTable()) {
        User = *(saUser *)(void *)fromPtr;
        if(User != saUserNull) {
            /* Need to move it to toPtr */
            size = utMax(saUserGetNumMembershipTable(User) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saUserSetMembershipTableIndex_(User, toPtr - saUsers.MembershipTable + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saUser *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedUserMembershipTable(toPtr - saUsers.MembershipTable);
    saSetFreeUserMembershipTable(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the User.MembershipTable heap.
----------------------------------------------------------------------------------------*/
static void allocMoreUserMembershipTables(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedUserMembershipTable() - saUsedUserMembershipTable();

    if((saFreeUserMembershipTable() << 2) > saUsedUserMembershipTable()) {
        saCompactUserMembershipTables();
        freeSpace = saAllocatedUserMembershipTable() - saUsedUserMembershipTable();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedUserMembershipTable(saAllocatedUserMembershipTable() + spaceNeeded - freeSpace +
            (saAllocatedUserMembershipTable() >> 1));
        utResizeArray(saUsers.MembershipTable, saAllocatedUserMembershipTable());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new User.MembershipTable array.
----------------------------------------------------------------------------------------*/
void saUserAllocMembershipTables(
    saUser User,
    uint32 numMembershipTables)
{
    uint32 freeSpace = saAllocatedUserMembershipTable() - saUsedUserMembershipTable();
    uint32 elementSize = sizeof(saMembership);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numMembershipTables + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saUserGetNumMembershipTable(User) == 0);
#endif
    if(numMembershipTables == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMoreUserMembershipTables(spaceNeeded);
    }
    saUserSetMembershipTableIndex_(User, saUsedUserMembershipTable() + usedHeaderSize);
    saUserSetNumMembershipTable(User, numMembershipTables);
    *(saUser *)(void *)(saUsers.MembershipTable + saUsedUserMembershipTable()) = User;
    {
        uint32 xValue;
        for(xValue = (uint32)(saUserGetMembershipTableIndex_(User)); xValue < saUserGetMembershipTableIndex_(User) + numMembershipTables; xValue++) {
            saUsers.MembershipTable[xValue] = saMembershipNull;
        }
    }
    saSetUsedUserMembershipTable(saUsedUserMembershipTable() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saUserGetMembershipTables for the database manager.
----------------------------------------------------------------------------------------*/
static void *getUserMembershipTables(
    uint64 objectNumber,
    uint32 *numValues)
{
    saUser User = saIndex2User((uint32)objectNumber);

    *numValues = saUserGetNumMembershipTable(User);
    return saUserGetMembershipTables(User);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saUserAllocMembershipTables for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocUserMembershipTables(
    uint64 objectNumber,
    uint32 numValues)
{
    saUser User = saIndex2User((uint32)objectNumber);

    saUserSetMembershipTableIndex_(User, 0);
    saUserSetNumMembershipTable(User, 0);
    if(numValues == 0) {
        return NULL;
    }
    saUserAllocMembershipTables(User, numValues);
    return saUserGetMembershipTables(User);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the User.MembershipTable array.
----------------------------------------------------------------------------------------*/
void saUserFreeMembershipTables(
    saUser User)
{
    uint32 elementSize = sizeof(saMembership);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saUserGetNumMembershipTable(User) + usedHeaderSize, freeHeaderSize);
    saMembership *dataPtr = saUserGetMembershipTables(User) - usedHeaderSize;

    if(saUserGetNumMembershipTable(User) == 0) {
        return;
    }
    *(saUser *)(void *)(dataPtr) = saUserNull;
    *(uint32 *)(void *)(((saUser *)(void *)dataPtr) + 1) = size;
    saUserSetNumMembershipTable(User, 0);
    saSetFreeUserMembershipTable(saFreeUserMembershipTable() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the User.MembershipTable array.
----------------------------------------------------------------------------------------*/
void saUserResizeMembershipTables(
    saUser User,
    uint32 numMembershipTables)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(saMembership);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numMembershipTables + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saUserGetNumMembershipTable(User) + usedHeaderSize, freeHeaderSize);
    saMembership *dataPtr;

    if(numMembershipTables == 0) {
        if(saUserGetNumMembershipTable(User) != 0) {
            saUserFreeMembershipTables(User);
        }
        return;
    }
    if(saUserGetNumMembershipTable(User) == 0) {
        saUserAllocMembershipTables(User, numMembershipTables);
        return;
    }
    freeSpace = saAllocatedUserMembershipTable() - saUsedUserMembershipTable();
    if(freeSpace < newSize) {
        allocMoreUserMembershipTables(newSize);
    }
    dataPtr = saUserGetMembershipTables(User) - usedHeaderSize;
    memcpy((void *)(saUsers.MembershipTable + saUsedUserMembershipTable()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        {
            uint32 xValue;
            for(xValue = (uint32)(saUsedUserMembershipTable() + oldSize); xValue < saUsedUserMembershipTable() + oldSize + newSize - oldSize; xValue++) {
                saUsers.MembershipTable[xValue] = saMembershipNull;
            }
        }
    }
    *(saUser *)(void *)dataPtr = saUserNull;
    *(uint32 *)(void *)(((saUser *)(void *)dataPtr) + 1) = oldSize;
    saSetFreeUserMembershipTable(saFreeUserMembershipTable() + oldSize);
    saUserSetMembershipTableIndex_(User, saUsedUserMembershipTable() + usedHeaderSize);
    saUserSetNumMembershipTable(User, numMembershipTables);
    saSetUsedUserMembershipTable(saUsedUserMembershipTable() + newSize);
}

/*----------------------------------------------------------------------------------------
  Compact the User.PostTable heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactUserPostTables(void)
{
    uint32 elementSize = sizeof(saPost);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    saPost *toPtr = saUsers.PostTable;
    saPost *fromPtr = toPtr;
    saUser User;
    uint32 size;

    while(fromPtr < saUsers.PostTable + saUsedUserPostTable()) {
        User = *(saUser *)(void *)fromPtr;
        if(User != saUserNull) {
            /* Need to move it to toPtr */
            size = utMax(saUserGetNumPostTable(User) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saUserSetPostTableIndex_(User, toPtr - saUsers.PostTable + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saUser *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedUserPostTable(toPtr - saUsers.PostTable);
    saSetFreeUserPostTable(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the User.PostTable heap.
----------------------------------------------------------------------------------------*/
static void allocMoreUserPostTables(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedUserPostTable() - saUsedUserPostTable();

    if((saFreeUserPostTable() << 2) > saUsedUserPostTable()) {
        saCompactUserPostTables();
        freeSpace = saAllocatedUserPostTable() - saUsedUserPostTable();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedUserPostTable(saAllocatedUserPostTable() + spaceNeeded - freeSpace +
            (saAllocatedUserPostTable() >> 1));
        utResizeArray(saUsers.PostTable, saAllocatedUserPostTable());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new User.PostTable array.
----------------------------------------------------------------------------------------*/
void saUserAllocPostTables(
    saUser User,
    uint32 numPostTables)
{
    uint32 freeSpace = saAllocatedUserPostTable() - saUsedUserPostTable();
    uint32 elementSize = sizeof(saPost);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numPostTables + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saUserGetNumPostTable(User) == 0);
#endif
    if(numPostTables == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMoreUserPostTables(spaceNeeded);
    }
    saUserSetPostTableIndex_(User, saUsedUserPostTable() + usedHeaderSize);
    saUserSetNumPostTable(User, numPostTables);
    *(saUser *)(void *)(saUsers.PostTable + saUsedUserPostTable()) = User;
    {
        uint32 xValue;
        for(xValue = (uint32)(saUserGetPostTableIndex_(User)); xValue < saUserGetPostTableIndex_(User) + numPostTables; xValue++) {
            saUsers.PostTable[xValue] = saPostNull;
        }
    }
    saSetUsedUserPostTable(saUsedUserPostTable() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saUserGetPostTables for the database manager.
----------------------------------------------------------------------------------------*/
static void *getUserPostTables(
    uint64 objectNumber,
    uint32 *numValues)
{
    saUser User = saIndex2User((uint32)objectNumber);

    *numValues = saUserGetNumPostTable(User);
    return saUserGetPostTables(User);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saUserAllocPostTables for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocUserPostTables(
    uint64 objectNumber,
    uint32 numValues)
{
    saUser User = saIndex2User((uint32)objectNumber);

    saUserSetPostTableIndex_(User, 0);
    saUserSetNumPostTable(User, 0);
    if(numValues == 0) {
        return NULL;
    }
    saUserAllocPostTables(User, numValues);
    return saUserGetPostTables(User);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the User.PostTable array.
----------------------------------------------------------------------------------------*/
void saUserFreePostTables(
    saUser User)
{
    uint32 elementSize = sizeof(saPost);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saUserGetNumPostTable(User) + usedHeaderSize, freeHeaderSize);
    saPost *dataPtr = saUserGetPostTables(User) - usedHeaderSize;

    if(saUserGetNumPostTable(User) == 0) {
        return;
    }
    *(saUser *)(void *)(dataPtr) = saUserNull;
    *(uint32 *)(void *)(((saUser *)(void *)dataPtr) + 1) = size;
    saUserSetNumPostTable(User, 0);
    saSetFreeUserPostTable(saFreeUserPostTable() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the User.PostTable array.
----------------------------------------------------------------------------------------*/
void saUserResizePostTables(
    saUser User,
    uint32 numPostTables)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(saPost);
    uint32 usedHeaderSize = (sizeof(saUser) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saUser) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numPostTables + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saUserGetNumPostTable(User) + usedHeaderSize, freeHeaderSize);
    saPost *dataPtr;

    if(numPostTables == 0) {
        if(saUserGetNumPostTable(User) != 0) {
            saUserFreePostTables(User);
        }
        return;
    }
    if(saUserGetNumPostTable(User) == 0) {
        saUserAllocPostTables(User, numPostTables);
        return;
    }
    freeSpace = saAllocatedUserPostTable() - saUsedUserPostTable();
    if(freeSpace < newSize) {
        allocMoreUserPostTables(newSize);
    }
    dataPtr = saUserGetPostTables(User) - usedHeaderSize;
    memcpy((void *)(saUsers.PostTable + saUsedUserPostTable()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        {
            uint32 xValue;
            for(xValue = (uint32)(saUsedUserPostTable() + oldSize); xValue < saUsedUserPostTable() + oldSize + newSize - oldSize; xValue++) {
                saUsers.PostTable[xValue] = saPostNull;
            }
        }
    }
    *(saUser *)(void *)dataPtr = saUserNull;
    *(uint32 *)(void *)(((saUser *)(void *)dataPtr) + 1) = oldSize;
    saSetFreeUserPostTable(saFreeUserPostTable() + oldSize);
    saUserSetPostTableIndex_(User, saUsedUserPostTable() + usedHeaderSize);
    saUserSetNumPostTable(User, numPostTables);
    saSetUsedUserPostTable(saUsedUserPostTable() + newSize);
}

/*----------------------------------------------------------------------------------------
  Copy the properties of User.
----------------------------------------------------------------------------------------*/
void saUserCopyProps(
    saUser oldUser,
    saUser newUser)
{
    saUserSetEmail(newUser, saUserGetEmail(oldUser));
    saUserSetJoinDate(newUser, saUserGetJoinDate(oldUser));
    saUserSetActive(newUser, saUserActive(oldUser));
    saUserSetLoggedIn(newUser, saUserLoggedIn(oldUser));
    saUserSetLastActivity(newUser, saUserGetLastActivity(oldUser));
    saUserSetBalance(newUser, saUserGetBalance(oldUser));
    saUserSetValidated(newUser, saUserValidated(oldUser));
    saUserSetSupremeLeader(newUser, saUserSupremeLeader(oldUser));
    saUserSetRegionManager(newUser, saUserRegionManager(oldUser));
}

/*----------------------------------------------------------------------------------------
  Increase the size of the hash table.
----------------------------------------------------------------------------------------*/
static void resizeUserMembershipHashTable(
    saUser User)
{
    saMembership _Membership, prevMembership, nextMembership;
    uint32 oldNumMemberships = saUserGetNumMembershipTable(User);
    uint32 newNumMemberships = oldNumMemberships << 1;
    uint32 xMembership, index;

    if(newNumMemberships == 0) {
        newNumMemberships = 2;
        saUserAllocMembershipTables(User, 2);
    } else {
        saUserResizeMembershipTables(User, newNumMemberships);
    }
    for(xMembership = 0; xMembership < oldNumMemberships; xMembership++) {
        _Membership = saUserGetiMembershipTable(User, xMembership);
        prevMembership = saMembershipNull;
        while(_Membership != saMembershipNull) {
            nextMembership = saMembershipGetNextTableUserMembership(_Membership);
            index = (newNumMemberships - 1) & saCharity2Index(saMembershipGetCharity(_Membership));
            if(index != xMembership) {
                if(prevMembership == saMembershipNull) {
                    saUserSetiMembershipTable(User, xMembership, nextMembership);
                } else {
                    saMembershipSetNextTableUserMembership(prevMembership, nextMembership);
                }
                saMembershipSetNextTableUserMembership(_Membership, saUserGetiMembershipTable(User, index));
                saUserSetiMembershipTable(User, index, _Membership);
            } else {
                prevMembership = _Membership;
            }
            _Membership = nextMembership;
        }
    }
}

/*----------------------------------------------------------------------------------------
  Add the Membership to the User.  If the table is near full, build a new one twice
  as big, delete the old one, and return the new one.
----------------------------------------------------------------------------------------*/
static void addUserMembershipToHashTable(
    saUser User,
    saMembership _Membership)
{
    saMembership nextMembership;
    uint32 index;

    if(saUserGetNumMembership(User) >> 1 >= saUserGetNumMembershipTable(User)) {
        resizeUserMembershipHashTable(User);
    }
    index = (saUserGetNumMembershipTable(User) - 1) & saCharity2Index(saMembershipGetCharity(_Membership));
    nextMembership = saUserGetiMembershipTable(User, index);
    saMembershipSetNextTableUserMembership(_Membership, nextMembership);
    saUserSetiMembershipTable(User, index, _Membership);
    saUserSetNumMembership(User, saUserGetNumMembership(User) + 1);
}

/*----------------------------------------------------------------------------------------
  Remove the Membership from the hash table.
----------------------------------------------------------------------------------------*/
static void removeUserMembershipFromHashTable(
    saUser User,
    saMembership _Membership)
{
    uint32 index = (saUserGetNumMembershipTable(User) - 1) & saCharity2Index(saMembershipGetCharity(_Membership));
    saMembership prevMembership, nextMembership;
    
    nextMembership = saUserGetiMembershipTable(User, index);
    if(nextMembership == _Membership) {
        saUserSetiMembershipTable(User, index, saMembershipGetNextTableUserMembership(nextMembership));
    } else {
        do {
            prevMembership = nextMembership;
            nextMembership = saMembershipGetNextTableUserMembership(nextMembership);
        } while(nextMembership != _Membership);
        saMembershipSetNextTableUserMembership(prevMembership, saMembershipGetNextTableUserMembership(_Membership));
    }
    saUserSetNumMembership(User, saUserGetNumMembership(User) - 1);
    saMembershipSetNextTableUserMembership(_Membership, saMembershipNull);
}

/*----------------------------------------------------------------------------------------
  Find the Membership from the User and its hash key.
----------------------------------------------------------------------------------------*/
saMembership saUserFindMembership(
    saUser User,
    saCharity Charity)
{
    uint32 mask = saUserGetNumMembershipTable(User) - 1;
    saMembership _Membership;

    if(mask + 1 != 0) {
        _Membership = saUserGetiMembershipTable(User, saCharity2Index(Charity) & mask);
        while(_Membership != saMembershipNull) {
            if(saMembershipGetCharity(_Membership) == Charity) {
                return _Membership;
            }
            _Membership = saMembershipGetNextTableUserMembership(_Membership);
        }
    }
    return saMembershipNull;
}

/*----------------------------------------------------------------------------------------
  Add the Membership to the head of the list on the User.
----------------------------------------------------------------------------------------*/
void saUserInsertMembership(
    saUser User,
    saMembership _Membership)
{
#if defined(DD_DEBUG)
    if(User == saUserNull) {
        utExit("Non-existent User");
    }
    if(_Membership == saMembershipNull) {
        utExit("Non-existent Membership");
    }
    if(saMembershipGetUser(_Membership) != saUserNull) {
        utExit("Attempting to add Membership to User twice");
    }
#endif
    saMembershipSetNextUserMembership(_Membership, saUserGetFirstMembership(User));
    if(saUserGetFirstMembership(User) != saMembershipNull) {
        saMembershipSetPrevUserMembership(saUserGetFirstMembership(User), _Membership);
    }
    saUserSetFirstMembership(User, _Membership);
    saMembershipSetPrevUserMembership(_Membership, saMembershipNull);
    if(saUserGetLastMembership(User) == saMembershipNull) {
        saUserSetLastMembership(User, _Membership);
    }
    saMembershipSetUser(_Membership, User);
    addUserMembershipToHashTable(User, _Membership);
}

/*----------------------------------------------------------------------------------------
  Add the Membership to the end of the list on the User.
----------------------------------------------------------------------------------------*/
void saUserAppendMembership(
    saUser User,
    saMembership _Membership)
{
#if defined(DD_DEBUG)
    if(User == saUserNull) {
        utExit("Non-existent User");
    }
    if(_Membership == saMembershipNull) {
        utExit("Non-existent Membership");
    }
    if(saMembershipGetUser(_Membership) != saUserNull) {
        utExit("Attempting to add Membership to User twice");
    }
#endif
    saMembershipSetPrevUserMembership(_Membership, saUserGetLastMembership(User));
    if(saUserGetLastMembership(User) != saMembershipNull) {
        saMembershipSetNextUserMembership(saUserGetLastMembership(User), _Membership);
    }
    saUserSetLastMembership(User, _Membership);
    saMembershipSetNextUserMembership(_Membership, saMembershipNull);
    if(saUserGetFirstMembership(User) == saMembershipNull) {
        saUserSetFirstMembership(User, _Membership);
    }
    saMembershipSetUser(_Membership, User);
    addUserMembershipToHashTable(User, _Membership);
}

/*----------------------------------------------------------------------------------------
  Insert the Membership to the User after the previous Membership.
----------------------------------------------------------------------------------------*/
void saUserInsertAfterMembership(
    saUser User,
    saMembership prevMembership,
    saMembership _Membership)
{
    saMembership nextMembership = saMembershipGetNextUserMembership(prevMembership);

#if defined(DD_DEBUG)
    if(User == saUserNull) {
        utExit("Non-existent User");
    }
    if(_Membership == saMembershipNull) {
        utExit("Non-existent Membership");
    }
    if(saMembershipGetUser(_Membership) != saUserNull) {
        utExit("Attempting to add Membership to User twice");
    }
#endif
    saMembershipSetNextUserMembership(_Membership, nextMembership);
    saMembershipSetNextUserMembership(prevMembership, _Membership);
    saMembershipSetPrevUserMembership(_Membership, prevMembership);
    if(nextMembership != saMembershipNull) {
        saMembershipSetPrevUserMembership(nextMembership, _Membership);
    }
    if(saUserGetLastMembership(User) == prevMembership) {
        saUserSetLastMembership(User, _Membership);
    }
    saMembershipSetUser(_Membership, User);
    addUserMembershipToHashTable(User, _Membership);
}

/*----------------------------------------------------------------------------------------
 Remove the Membership from the User.
----------------------------------------------------------------------------------------*/
void saUserRemoveMembership(
    saUser User,
    saMembership _Membership)
{
    saMembership pMembership, nMembership;

#if defined(DD_DEBUG)
    if(_Membership == saMembershipNull) {
        utExit("Non-existent Membership");
    }
    if(saMembershipGetUser(_Membership) != saUserNull && saMembershipGetUser(_Membership) != User) {
        utExit("Delete Membership from non-owning User");
    }
#endif
    nMembership = saMembershipGetNextUserMembership(_Membership);
    pMembership = saMembershipGetPrevUserMembership(_Membership);
    if(pMembership != saMembershipNull) {
        saMembershipSetNextUserMembership(pMembership, nMembership);
    } else if(saUserGetFirstMembership(User) == _Membership) {
        saUserSetFirstMembership(User, nMembership);
    }
    if(nMembership != saMembershipNull) {
        saMembershipSetPrevUserMembership(nMembership, pMembership);
    } else if(saUserGetLastMembership(User) == _Membership) {
        saUserSetLastMembership(User, pMembership);
    }
    saMembershipSetNextUserMembership(_Membership, saMembershipNull);
    saMembershipSetPrevUserMembership(_Membership, saMembershipNull);
    saMembershipSetUser(_Membership, saUserNull);
    removeUserMembershipFromHashTable(User, _Membership);
}

/*----------------------------------------------------------------------------------------
  Add the OutTransaction to the head of the list on the User.
----------------------------------------------------------------------------------------*/
void saUserInsertOutTransaction(
    saUser User,
    saTransaction _Transaction)
{
#if defined(DD_DEBUG)
    if(User == saUserNull) {
        utExit("Non-existent User");
    }
    if(_Transaction == saTransactionNull) {
        utExit("Non-existent Transaction");
    }
    if(saTransactionGetFromUser(_Transaction) != saUserNull) {
        utExit("Attempting to add Transaction to User twice");
    }
#endif
    saTransactionSetNextUserOutTransaction(_Transaction, saUserGetFirstOutTransaction(User));
    if(saUserGetFirstOutTransaction(User) != saTransactionNull) {
        saTransactionSetPrevUserOutTransaction(saUserGetFirstOutTransaction(User), _Transaction);
    }
    saUserSetFirstOutTransaction(User, _Transaction);
    saTransactionSetPrevUserOutTransaction(_Transaction, saTransactionNull);
    if(saUserGetLastOutTransaction(User) == saTransactionNull) {
        saUserSetLastOutTransaction(User, _Transaction);
    }
    saTransactionSetFromUser(_Transaction, User);
}

/*----------------------------------------------------------------------------------------
  Add the OutTransaction to the end of the list on the User.
----------------------------------------------------------------------------------------*/
void saUserAppendOutTransaction(
    saUser User,
    saTransaction _Transaction)
{
#if defined(DD_DEBUG)
    if(User == saUserNull) {
        utExit("Non-existent User");
    }
    if(_Transaction == saTransactionNull) {
        utExit("Non-existent Transaction");
    }
    if(saTransactionGetFromUser(_Transaction) != saUserNull) {
        utExit("Attempting to add Transaction to User twice");
    }
#endif
    saTransactionSetPrevUserOutTransaction(_Transaction, saUserGetLastOutTransaction(User));
    if(saUserGetLastOutTransaction(User) != saTransactionNull) {
        saTransactionSetNextUserOutTransaction(saUserGetLastOutTransaction(User), _Transaction);
    }
    saUserSetLastOutTransaction(User, _Transaction);
    saTransactionSetNextUserOutTransaction(_Transaction, saTransactionNull);
    if(saUserGetFirstOutTransaction(User) == saTransactionNull) {
        saUserSetFirstOutTransaction(User, _Transaction);
    }
    saTransactionSetFromUser(_Transaction, User);
}

/*----------------------------------------------------------------------------------------
  Insert the OutTransaction to the User after the previous OutTransaction.
----------------------------------------------------------------------------------------*/
void saUserInsertAfterOutTransaction(
    saUser User,
    saTransaction prevTransaction,
    saTransaction _Transaction)
{
    saTransaction nextTransaction = saTransactionGetNextUserOutTransaction(prevTransaction);

#if defined(DD_DEBUG)
    if(User == saUserNull) {
        utExit("Non-existent User");
    }
    if(_Transaction == saTransactionNull) {
        utExit("Non-existent Transaction");
    }
    if(saTransactionGetFromUser(_Transaction) != saUserNull) {
        utExit("Attempting to add Transaction to User twice");
    }
#endif
    saTransactionSetNextUserOutTransaction(_Transaction, nextTransaction);
    saTransactionSetNextUserOutTransaction(prevTransaction, _Transaction);
    saTransactionSetPrevUserOutTransaction(_Transaction, prevTransaction);
    if(nextTransaction != saTransactionNull) {
        saTransactionSetPrevUserOutTransaction(nextTransaction, _Transaction);
    }
    if(saUserGetLastOutTransaction(User) == prevTransaction) {
        saUserSetLastOutTransaction(User, _Transaction);
    }
    saTransactionSetFromUser(_Transaction, User);
}

/*----------------------------------------------------------------------------------------
 Remove the OutTransaction from the User.
----------------------------------------------------------------------------------------*/
void saUserRemoveOutTransaction(
    saUser User,
    saTransaction _Transaction)
{
    saTransaction pTransaction, nTransaction;

#if defined(DD_DEBUG)
    if(_Transaction == saTransactionNull) {
        utExit("Non-existent Transaction");
    }
    if(saTransactionGetFromUser(_Transaction) != saUserNull && saTransactionGetFromUser(_Transaction) != User) {
        utExit("Delete Transaction from non-owning User");
    }
#endif
    nTransaction = saTransactionGetNextUserOutTransaction(_Transaction);
    pTransaction = saTransactionGetPrevUserOutTransaction(_Transaction);
    if(pTransaction != saTransactionNull) {
        saTransactionSetNextUserOutTransaction(pTransaction, nTransaction);
    } else if(saUserGetFirstOutTransaction(User) == _Transaction) {
        saUserSetFirstOutTransaction(User, nTransaction);
    }
    if(nTransaction != saTransactionNull) {
        saTransactionSetPrevUserOutTransaction(nTransaction, pTransaction);
    } else if(saUserGetLastOutTransaction(User) == _Transaction) {
        saUserSetLastOutTransaction(User, pTransaction);
    }
    saTransactionSetNextUserOutTransaction(_Transaction, saTransactionNull);
    saTransactionSetPrevUserOutTransaction(_Transaction, saTransactionNull);
    saTransactionSetFromUser(_Transaction, saUserNull);
}

/*----------------------------------------------------------------------------------------
  Add the InTransaction to the head of the list on the User.
----------------------------------------------------------------------------------------*/
void saUserInsertInTransaction(
    saUser User,
    saTransaction _Transaction)
{
#if defined(DD_DEBUG)
    if(User == saUserNull) {
        utExit("Non-existent User");
    }
    if(_Transaction == saTransactionNull) {
        utExit("Non-existent Transaction");
    }
    if(saTransactionGetToUser(_Transaction) != saUserNull) {
        utExit("Attempting to add Transaction to User twice");
    }
#endif
    saTransactionSetNextUserInTransaction(_Transaction, saUserGetFirstInTransaction(User));
    if(saUserGetFirstInTransaction(User) != saTransactionNull) {
        saTransactionSetPrevUserInTransaction(saUserGetFirstInTransaction(User), _Transaction);
    }
    saUserSetFirstInTransaction(User, _Transaction);
    saTransactionSetPrevUserInTransaction(_Transaction, saTransactionNull);
    if(saUserGetLastInTransaction(User) == saTransactionNull) {
        saUserSetLastInTransaction(User, _Transaction);
    }
    saTransactionSetToUser(_Transaction, User);
}

/*----------------------------------------------------------------------------------------
  Add the InTransaction to the end of the list on the User.
----------------------------------------------------------------------------------------*/
void saUserAppendInTransaction(
    saUser User,
    saTransaction _Transaction)
{
#if defined(DD_DEBUG)
    if(User == saUserNull) {
        utExit("Non-existent User");
    }
    if(_Transaction == saTransactionNull) {
        utExit("Non-existent Transaction");
    }
    if(saTransactionGetToUser(_Transaction) != saUserNull) {
        utExit("Attempting to add Transaction to User twice");
    }
#endif
    saTransactionSetPrevUserInTransaction(_Transaction, saUserGetLastInTransaction(User));
    if(saUserGetLastInTransaction(User) != saTransactionNull) {
        saTransactionSetNextUserInTransaction(saUserGetLastInTransaction(User), _Transaction);
    }
    saUserSetLastInTransaction(User, _Transaction);
    saTransactionSetNextUserInTransaction(_Transaction, saTransactionNull);
    if(saUserGetFirstInTransaction(User) == saTransactionNull) {
        saUserSetFirstInTransaction(User, _Transaction);
    }
    saTransactionSetToUser(_Transaction, User);
}

/*----------------------------------------------------------------------------------------
  Insert the InTransaction to the User after the previous InTransaction.
----------------------------------------------------------------------------------------*/
void saUserInsertAfterInTransaction(
    saUser User,
    saTransaction prevTransaction,
    saTransaction _Transaction)
{
    saTransaction nextTransaction = saTransactionGetNextUserInTransaction(prevTransaction);

#if defined(DD_DEBUG)
    if(User == saUserNull) {
        utExit("Non-existent User");
    }
    if(_Transaction == saTransactionNull) {
        utExit("Non-existent Transaction");
    }
    if(saTransactionGetToUser(_Transaction) != saUserNull) {
        utExit("Attempting to add Transaction to User twice");
    }
#endif
    saTransactionSetNextUserInTransaction(_Transaction, nextTransaction);
    saTransactionSetNextUserInTransaction(prevTransaction, _Transaction);
    saTransactionSetPrevUserInTransaction(_Transaction, prevTransaction);
    if(nextTransaction != saTransactionNull) {
        saTransactionSetPrevUserInTransaction(nextTransaction, _Transaction);
    }
    if(saUserGetLastInTransaction(User) == prevTransaction) {
        saUserSetLastInTransaction(User, _Transaction);
    }
    saTransactionSetToUser(_Transaction, User);
}

/*----------------------------------------------------------------------------------------
 Remove the InTransaction from the User.
----------------------------------------------------------------------------------------*/
void saUserRemoveInTransaction(
    saUser User,
    saTransaction _Transaction)
{
    saTransaction pTransaction, nTransaction;

#if defined(DD_DEBUG)
    if(_Transaction == saTransactionNull) {
        utExit("Non-existent Transaction");
    }
    if(saTransactionGetToUser(_Transaction) != saUserNull && saTransactionGetToUser(_Transaction) != User) {
        utExit("Delete Transaction from non-owning User");
    }
#endif
    nTransaction = saTransactionGetNextUserInTransaction(_Transaction);
    pTransaction = saTransactionGetPrevUserInTransaction(_Transaction);
    if(pTransaction != saTransactionNull) {
        saTransactionSetNextUserInTransaction(pTransaction, nTransaction);
    } else if(saUserGetFirstInTransaction(User) == _Transaction) {
        saUserSetFirstInTransaction(User, nTransaction);
    }
    if(nTransaction != saTransactionNull) {
        saTransactionSetPrevUserInTransaction(nTransaction, pTransaction);
    } else if(saUserGetLastInTransaction(User) == _Transaction) {
        saUserSetLastInTransaction(User, pTransaction);
    }
    saTransactionSetNextUserInTransaction(_Transaction, saTransactionNull);
    saTransactionSetPrevUserInTransaction(_Transaction, saTransactionNull);
    saTransactionSetToUser(_Transaction, saUserNull);
}

/*----------------------------------------------------------------------------------------
  Add the Listing to the head of the list on the User.
----------------------------------------------------------------------------------------*/
void saUserInsertListing(
    saUser User,
    saListing _Listing)
{
#if defined(DD_DEBUG)
    if(User == saUserNull) {
        utExit("Non-existent User");
    }
    if(_Listing == saListingNull) {
        utExit("Non-existent Listing");
    }
    if(saListingGetUser(_Listing) != saUserNull) {
        utExit("Attempting to add Listing to User twice");
    }
#endif
    saListingSetNextUserListing(_Listing, saUserGetFirstListing(User));
    if(saUserGetFirstListing(User) != saListingNull) {
        saListingSetPrevUserListing(saUserGetFirstListing(User), _Listing);
    }
    saUserSetFirstListing(User, _Listing);
    saListingSetPrevUserListing(_Listing, saListingNull);
    if(saUserGetLastListing(User) == saListingNull) {
        saUserSetLastListing(User, _Listing);
    }
    saListingSetUser(_Listing, User);
}

/*----------------------------------------------------------------------------------------
  Add the Listing to the end of the list on the User.
----------------------------------------------------------------------------------------*/
void saUserAppendListing(
    saUser User,
    saListing _Listing)
{
#if defined(DD_DEBUG)
    if(User == saUserNull) {
        utExit("Non-existent User");
    }
    if(_Listing == saListingNull) {
        utExit("Non-existent Listing");
    }
    if(saListingGetUser(_Listing) != saUserNull) {
        utExit("Attempting to add Listing to User twice");
    }
#endif
    saListingSetPrevUserListing(_Listing, saUserGetLastListing(User));
    if(saUserGetLastListing(User) != saListingNull) {
        saListingSetNextUserListing(saUserGetLastListing(User), _Listing);
    }
    saUserSetLastListing(User, _Listing);
    saListingSetNextUserListing(_Listing, saListingNull);
    if(saUserGetFirstListing(User) == saListingNull) {
        saUserSetFirstListing(User, _Listing);
    }
    saListingSetUser(_Listing, User);
}

/*----------------------------------------------------------------------------------------
  Insert the Listing to the User after the previous Listing.
----------------------------------------------------------------------------------------*/
void saUserInsertAfterListing(
    saUser User,
    saListing prevListing,
    saListing _Listing)
{
    saListing nextListing = saListingGetNextUserListing(prevListing);

#if defined(DD_DEBUG)
    if(User == saUserNull) {
        utExit("Non-existent User");
    }
    if(_Listing == saListingNull) {
        utExit("Non-existent Listing");
    }
    if(saListingGetUser(_Listing) != saUserNull) {
        utExit("Attempting to add Listing to User twice");
    }
#endif
    saListingSetNextUserListing(_Listing, nextListing);
    saListingSetNextUserListing(prevListing, _Listing);
    saListingSetPrevUserListing(_Listing, prevListing);
    if(nextListing != saListingNull) {
        saListingSetPrevUserListing(nextListing, _Listing);
    }
    if(saUserGetLastListing(User) == prevListing) {
        saUserSetLastListing(User, _Listing);
    }
    saListingSetUser(_Listing, User);
}

/*----------------------------------------------------------------------------------------
 Remove the Listing from the User.
----------------------------------------------------------------------------------------*/
void saUserRemoveListing(
    saUser User,
    saListing _Listing)
{
    saListing pListing, nListing;

#if defined(DD_DEBUG)
    if(_Listing == saListingNull) {
        utExit("Non-existent Listing");
    }
    if(saListingGetUser(_Listing) != saUserNull && saListingGetUser(_Listing) != User) {
        utExit("Delete Listing from non-owning User");
    }
#endif
    nListing = saListingGetNextUserListing(_Listing);
    pListing = saListingGetPrevUserListing(_Listing);
    if(pListing != saListingNull) {
        saListingSetNextUserListing(pListing, nListing);
    } else if(saUserGetFirstListing(User) == _Listing) {
        saUserSetFirstListing(User, nListing);
    }
    if(nListing != saListingNull) {
        saListingSetPrevUserListing(nListing, pListing);
    } else if(saUserGetLastListing(User) == _Listing) {
        saUserSetLastListing(User, pListing);
    }
    saListingSetNextUserListing(_Listing, saListingNull);
    saListingSetPrevUserListing(_Listing, saListingNull);
    saListingSetUser(_Listing, saUserNull);
}

/*----------------------------------------------------------------------------------------
  Increase the size of the hash table.
----------------------------------------------------------------------------------------*/
static void resizeUserPostHashTable(
    saUser User)
{
    saPost _Post, prevPost, nextPost;
    uint32 oldNumPosts = saUserGetNumPostTable(User);
    uint32 newNumPosts = oldNumPosts << 1;
    uint32 xPost, index;

    if(newNumPosts == 0) {
        newNumPosts = 2;
        saUserAllocPostTables(User, 2);
    } else {
        saUserResizePostTables(User, newNumPosts);
    }
    for(xPost = 0; xPost < oldNumPosts; xPost++) {
        _Post = saUserGetiPostTable(User, xPost);
        prevPost = saPostNull;
        while(_Post != saPostNull) {
            nextPost = saPostGetNextTableUserPost(_Post);
            index = (newNumPosts - 1) & (uint32)saPostGetID(_Post);
            if(index != xPost) {
                if(prevPost == saPostNull) {
                    saUserSetiPostTable(User, xPost, nextPost);
                } else {
                    saPostSetNextTableUserPost(prevPost, nextPost);
                }
                saPostSetNextTableUserPost(_Post, saUserGetiPostTable(User, index));
                saUserSetiPostTable(User, index, _Post);
            } else {
                prevPost = _Post;
            }
            _Post = nextPost;
        }
    }
}

/*----------------------------------------------------------------------------------------
  Add the Post to the User.  If the table is near full, build a new one twice
  as big, delete the old one, and return the new one.
----------------------------------------------------------------------------------------*/
static void addUserPostToHashTable(
    saUser User,
    saPost _Post)
{
    saPost nextPost;
    uint32 index;

    if(saUserGetNumPost(User) >> 1 >= saUserGetNumPostTable(User)) {
        resizeUserPostHashTable(User);
    }
    index = (saUserGetNumPostTable(User) - 1) & (uint32)saPostGetID(_Post);
    nextPost = saUserGetiPostTable(User, index);
    saPostSetNextTableUserPost(_Post, nextPost);
    saUserSetiPostTable(User, index, _Post);
    saUserSetNumPost(User, saUserGetNumPost(User) + 1);
}

/*----------------------------------------------------------------------------------------
  Remove the Post from the hash table.
----------------------------------------------------------------------------------------*/
static void removeUserPostFromHashTable(
    saUser User,
    saPost _Post)
{
    uint32 index = (saUserGetNumPostTable(User) - 1) & (uint32)saPostGetID(_Post);
    saPost prevPost, nextPost;
    
    nextPost = saUserGetiPostTable(User, index);
    if(nextPost == _Post) {
        saUserSetiPostTable(User, index, saPostGetNextTableUserPost(nextPost));
    } else {
        do {
            prevPost = nextPost;
            nextPost = saPostGetNextTableUserPost(nextPost);
        } while(nextPost != _Post);
        saPostSetNextTableUserPost(prevPost, saPostGetNextTableUserPost(_Post));
    }
    saUserSetNumPost(User, saUserGetNumPost(User) - 1);
    saPostSetNextTableUserPost(_Post, saPostNull);
}

/*----------------------------------------------------------------------------------------
  Find the Post from the User and its hash key.
----------------------------------------------------------------------------------------*/
saPost saUserFindPost(
    saUser User,
    uint32 ID)
{
    uint32 mask = saUserGetNumPostTable(User) - 1;
    saPost _Post;

    if(mask + 1 != 0) {
        _Post = saUserGetiPostTable(User, (uint32)ID & mask);
        while(_Post != saPostNull) {
            if(saPostGetID(_Post) == ID) {
                return _Post;
            }
            _Post = saPostGetNextTableUserPost(_Post);
        }
    }
    return saPostNull;
}

/*----------------------------------------------------------------------------------------
  Add the Post to the head of the list on the User.
----------------------------------------------------------------------------------------*/
void saUserInsertPost(
    saUser User,
    saPost _Post)
{
#if defined(DD_DEBUG)
    if(User == saUserNull) {
        utExit("Non-existent User");
    }
    if(_Post == saPostNull) {
        utExit("Non-existent Post");
    }
    if(saPostGetUser(_Post) != saUserNull) {
        utExit("Attempting to add Post to User twice");
    }
#endif
    saPostSetNextUserPost(_Post, saUserGetFirstPost(User));
    if(saUserGetFirstPost(User) != saPostNull) {
        saPostSetPrevUserPost(saUserGetFirstPost(User), _Post);
    }
    saUserSetFirstPost(User, _Post);
    saPostSetPrevUserPost(_Post, saPostNull);
    if(saUserGetLastPost(User) == saPostNull) {
        saUserSetLastPost(User, _Post);
    }
    saPostSetUser(_Post, User);
    addUserPostToHashTable(User, _Post);
}

/*----------------------------------------------------------------------------------------
  Add the Post to the end of the list on the User.
----------------------------------------------------------------------------------------*/
void saUserAppendPost(
    saUser User,
    saPost _Post)
{
#if defined(DD_DEBUG)
    if(User == saUserNull) {
        utExit("Non-existent User");
    }
    if(_Post == saPostNull) {
        utExit("Non-existent Post");
    }
    if(saPostGetUser(_Post) != saUserNull) {
        utExit("Attempting to add Post to User twice");
    }
#endif
    saPostSetPrevUserPost(_Post, saUserGetLastPost(User));
    if(saUserGetLastPost(User) != saPostNull) {
        saPostSetNextUserPost(saUserGetLastPost(User), _Post);
    }
    saUserSetLastPost(User, _Post);
    saPostSetNextUserPost(_Post, saPostNull);
    if(saUserGetFirstPost(User) == saPostNull) {
        saUserSetFirstPost(User, _Post);
    }
    saPostSetUser(_Post, User);
    addUserPostToHashTable(User, _Post);
}

/*----------------------------------------------------------------------------------------
  Insert the Post to the User after the previous Post.
----------------------------------------------------------------------------------------*/
void saUserInsertAfterPost(
    saUser User,
    saPost prevPost,
    saPost _Post)
{
    saPost nextPost = saPostGetNextUserPost(prevPost);

#if defined(DD_DEBUG)
    if(User == saUserNull) {
        utExit("Non-existent User");
    }
    if(_Post == saPostNull) {
        utExit("Non-existent Post");
    }
    if(saPostGetUser(_Post) != saUserNull) {
        utExit("Attempting to add Post to User twice");
    }
#endif
    saPostSetNextUserPost(_Post, nextPost);
    saPostSetNextUserPost(prevPost, _Post);
    saPostSetPrevUserPost(_Post, prevPost);
    if(nextPost != saPostNull) {
        saPostSetPrevUserPost(nextPost, _Post);
    }
    if(saUserGetLastPost(User) == prevPost) {
        saUserSetLastPost(User, _Post);
    }
    saPostSetUser(_Post, User);
    addUserPostToHashTable(User, _Post);
}

/*----------------------------------------------------------------------------------------
 Remove the Post from the User.
----------------------------------------------------------------------------------------*/
void saUserRemovePost(
    saUser User,
    saPost _Post)
{
    saPost pPost, nPost;

#if defined(DD_DEBUG)
    if(_Post == saPostNull) {
        utExit("Non-existent Post");
    }
    if(saPostGetUser(_Post) != saUserNull && saPostGetUser(_Post) != User) {
        utExit("Delete Post from non-owning User");
    }
#endif
    nPost = saPostGetNextUserPost(_Post);
    pPost = saPostGetPrevUserPost(_Post);
    if(pPost != saPostNull) {
        saPostSetNextUserPost(pPost, nPost);
    } else if(saUserGetFirstPost(User) == _Post) {
        saUserSetFirstPost(User, nPost);
    }
    if(nPost != saPostNull) {
        saPostSetPrevUserPost(nPost, pPost);
    } else if(saUserGetLastPost(User) == _Post) {
        saUserSetLastPost(User, pPost);
    }
    saPostSetNextUserPost(_Post, saPostNull);
    saPostSetPrevUserPost(_Post, saPostNull);
    saPostSetUser(_Post, saUserNull);
    removeUserPostFromHashTable(User, _Post);
}

/*----------------------------------------------------------------------------------------
  Add the Subscription to the head of the list on the User.
----------------------------------------------------------------------------------------*/
void saUserInsertSubscription(
    saUser User,
    saSubscription _Subscription)
{
#if defined(DD_DEBUG)
    if(User == saUserNull) {
        utExit("Non-existent User");
    }
    if(_Subscription == saSubscriptionNull) {
        utExit("Non-existent Subscription");
    }
    if(saSubscriptionGetUser(_Subscription) != saUserNull) {
        utExit("Attempting to add Subscription to User twice");
    }
#endif
    saSubscriptionSetNextUserSubscription(_Subscription, saUserGetFirstSubscription(User));
    if(saUserGetFirstSubscription(User) != saSubscriptionNull) {
        saSubscriptionSetPrevUserSubscription(saUserGetFirstSubscription(User), _Subscription);
    }
    saUserSetFirstSubscription(User, _Subscription);
    saSubscriptionSetPrevUserSubscription(_Subscription, saSubscriptionNull);
    if(saUserGetLastSubscription(User) == saSubscriptionNull) {
        saUserSetLastSubscription(User, _Subscription);
    }
    saSubscriptionSetUser(_Subscription, User);
}

/*----------------------------------------------------------------------------------------
  Add the Subscription to the end of the list on the User.
----------------------------------------------------------------------------------------*/
void saUserAppendSubscription(
    saUser User,
    saSubscription _Subscription)
{
#if defined(DD_DEBUG)
    if(User == saUserNull) {
        utExit("Non-existent User");
    }
    if(_Subscription == saSubscriptionNull) {
        utExit("Non-existent Subscription");
    }
    if(saSubscriptionGetUser(_Subscription) != saUserNull) {
        utExit("Attempting to add Subscription to User twice");
    }
#endif
    saSubscriptionSetPrevUserSubscription(_Subscription, saUserGetLastSubscription(User));
    if(saUserGetLastSubscription(User) != saSubscriptionNull) {
        saSubscriptionSetNextUserSubscription(saUserGetLastSubscription(User), _Subscription);
    }
    saUserSetLastSubscription(User, _Subscription);
    saSubscriptionSetNextUserSubscription(_Subscription, saSubscriptionNull);
    if(saUserGetFirstSubscription(User) == saSubscriptionNull) {
        saUserSetFirstSubscription(User, _Subscription);
    }
    saSubscriptionSetUser(_Subscription, User);
}

/*----------------------------------------------------------------------------------------
  Insert the Subscription to the User after the previous Subscription.
----------------------------------------------------------------------------------------*/
void saUserInsertAfterSubscription(
    saUser User,
    saSubscription prevSubscription,
    saSubscription _Subscription)
{
    saSubscription nextSubscription = saSubscriptionGetNextUserSubscription(prevSubscription);

#if defined(DD_DEBUG)
    if(User == saUserNull) {
        utExit("Non-existent User");
    }
    if(_Subscription == saSubscriptionNull) {
        utExit("Non-existent Subscription");
    }
    if(saSubscriptionGetUser(_Subscription) != saUserNull) {
        utExit("Attempting to add Subscription to User twice");
    }
#endif
    saSubscriptionSetNextUserSubscription(_Subscription, nextSubscription);
    saSubscriptionSetNextUserSubscription(prevSubscription, _Subscription);
    saSubscriptionSetPrevUserSubscription(_Subscription, prevSubscription);
    if(nextSubscription != saSubscriptionNull) {
        saSubscriptionSetPrevUserSubscription(nextSubscription, _Subscription);
    }
    if(saUserGetLastSubscription(User) == prevSubscription) {
        saUserSetLastSubscription(User, _Subscription);
    }
    saSubscriptionSetUser(_Subscription, User);
}

/*----------------------------------------------------------------------------------------
 Remove the Subscription from the User.
----------------------------------------------------------------------------------------*/
void saUserRemoveSubscription(
    saUser User,
    saSubscription _Subscription)
{
    saSubscription pSubscription, nSubscription;

#if defined(DD_DEBUG)
    if(_Subscription == saSubscriptionNull) {
        utExit("Non-existent Subscription");
    }
    if(saSubscriptionGetUser(_Subscription) != saUserNull && saSubscriptionGetUser(_Subscription) != User) {
        utExit("Delete Subscription from non-owning User");
    }
#endif
    nSubscription = saSubscriptionGetNextUserSubscription(_Subscription);
    pSubscription = saSubscriptionGetPrevUserSubscription(_Subscription);
    if(pSubscription != saSubscriptionNull) {
        saSubscriptionSetNextUserSubscription(pSubscription, nSubscription);
    } else if(saUserGetFirstSubscription(User) == _Subscription) {
        saUserSetFirstSubscription(User, nSubscription);
    }
    if(nSubscription != saSubscriptionNull) {
        saSubscriptionSetPrevUserSubscription(nSubscription, pSubscription);
    } else if(saUserGetLastSubscription(User) == _Subscription) {
        saUserSetLastSubscription(User, pSubscription);
    }
    saSubscriptionSetNextUserSubscription(_Subscription, saSubscriptionNull);
    saSubscriptionSetPrevUserSubscription(_Subscription, saSubscriptionNull);
    saSubscriptionSetUser(_Subscription, saUserNull);
}

/*----------------------------------------------------------------------------------------
  Add the Announcement to the head of the list on the User.
----------------------------------------------------------------------------------------*/
void saUserInsertAnnouncement(
    saUser User,
    saAnnouncement _Announcement)
{
#if defined(DD_DEBUG)
    if(User == saUserNull) {
        utExit("Non-existent User");
    }
    if(_Announcement == saAnnouncementNull) {
        utExit("Non-existent Announcement");
    }
    if(saAnnouncementGetUser(_Announcement) != saUserNull) {
        utExit("Attempting to add Announcement to User twice");
    }
#endif
    saAnnouncementSetNextUserAnnouncement(_Announcement, saUserGetFirstAnnouncement(User));
    if(saUserGetFirstAnnouncement(User) != saAnnouncementNull) {
        saAnnouncementSetPrevUserAnnouncement(saUserGetFirstAnnouncement(User), _Announcement);
    }
    saUserSetFirstAnnouncement(User, _Announcement);
    saAnnouncementSetPrevUserAnnouncement(_Announcement, saAnnouncementNull);
    if(saUserGetLastAnnouncement(User) == saAnnouncementNull) {
        saUserSetLastAnnouncement(User, _Announcement);
    }
    saAnnouncementSetUser(_Announcement, User);
}

/*----------------------------------------------------------------------------------------
  Add the Announcement to the end of the list on the User.
----------------------------------------------------------------------------------------*/
void saUserAppendAnnouncement(
    saUser User,
    saAnnouncement _Announcement)
{
#if defined(DD_DEBUG)
    if(User == saUserNull) {
        utExit("Non-existent User");
    }
    if(_Announcement == saAnnouncementNull) {
        utExit("Non-existent Announcement");
    }
    if(saAnnouncementGetUser(_Announcement) != saUserNull) {
        utExit("Attempting to add Announcement to User twice");
    }
#endif
    saAnnouncementSetPrevUserAnnouncement(_Announcement, saUserGetLastAnnouncement(User));
    if(saUserGetLastAnnouncement(User) != saAnnouncementNull) {
        saAnnouncementSetNextUserAnnouncement(saUserGetLastAnnouncement(User), _Announcement);
    }
    saUserSetLastAnnouncement(User, _Announcement);
    saAnnouncementSetNextUserAnnouncement(_Announcement, saAnnouncementNull);
    if(saUserGetFirstAnnouncement(User) == saAnnouncementNull) {
        saUserSetFirstAnnouncement(User, _Announcement);
    }
    saAnnouncementSetUser(_Announcement, User);
}

/*----------------------------------------------------------------------------------------
  Insert the Announcement to the User after the previous Announcement.
----------------------------------------------------------------------------------------*/
void saUserInsertAfterAnnouncement(
    saUser User,
    saAnnouncement prevAnnouncement,
    saAnnouncement _Announcement)
{
    saAnnouncement nextAnnouncement = saAnnouncementGetNextUserAnnouncement(prevAnnouncement);

#if defined(DD_DEBUG)
    if(User == saUserNull) {
        utExit("Non-existent User");
    }
    if(_Announcement == saAnnouncementNull) {
        utExit("Non-existent Announcement");
    }
    if(saAnnouncementGetUser(_Announcement) != saUserNull) {
        utExit("Attempting to add Announcement to User twice");
    }
#endif
    saAnnouncementSetNextUserAnnouncement(_Announcement, nextAnnouncement);
    saAnnouncementSetNextUserAnnouncement(prevAnnouncement, _Announcement);
    saAnnouncementSetPrevUserAnnouncement(_Announcement, prevAnnouncement);
    if(nextAnnouncement != saAnnouncementNull) {
        saAnnouncementSetPrevUserAnnouncement(nextAnnouncement, _Announcement);
    }
    if(saUserGetLastAnnouncement(User) == prevAnnouncement) {
        saUserSetLastAnnouncement(User, _Announcement);
    }
    saAnnouncementSetUser(_Announcement, User);
}

/*----------------------------------------------------------------------------------------
 Remove the Announcement from the User.
----------------------------------------------------------------------------------------*/
void saUserRemoveAnnouncement(
    saUser User,
    saAnnouncement _Announcement)
{
    saAnnouncement pAnnouncement, nAnnouncement;

#if defined(DD_DEBUG)
    if(_Announcement == saAnnouncementNull) {
        utExit("Non-existent Announcement");
    }
    if(saAnnouncementGetUser(_Announcement) != saUserNull && saAnnouncementGetUser(_Announcement) != User) {
        utExit("Delete Announcement from non-owning User");
    }
#endif
    nAnnouncement = saAnnouncementGetNextUserAnnouncement(_Announcement);
    pAnnouncement = saAnnouncementGetPrevUserAnnouncement(_Announcement);
    if(pAnnouncement != saAnnouncementNull) {
        saAnnouncementSetNextUserAnnouncement(pAnnouncement, nAnnouncement);
    } else if(saUserGetFirstAnnouncement(User) == _Announcement) {
        saUserSetFirstAnnouncement(User, nAnnouncement);
    }
    if(nAnnouncement != saAnnouncementNull) {
        saAnnouncementSetPrevUserAnnouncement(nAnnouncement, pAnnouncement);
    } else if(saUserGetLastAnnouncement(User) == _Announcement) {
        saUserSetLastAnnouncement(User, pAnnouncement);
    }
    saAnnouncementSetNextUserAnnouncement(_Announcement, saAnnouncementNull);
    saAnnouncementSetPrevUserAnnouncement(_Announcement, saAnnouncementNull);
    saAnnouncementSetUser(_Announcement, saUserNull);
}

#if defined(DD_DEBUG)
/*----------------------------------------------------------------------------------------
  Write out all the fields of an object.
----------------------------------------------------------------------------------------*/
void saShowUser(
    saUser User)
{
    utDatabaseShowObject("sa", "User", saUser2Index(User));
}
#endif

/*----------------------------------------------------------------------------------------
  Destroy Transaction including everything in it. Remove from parents.
----------------------------------------------------------------------------------------*/
void saTransactionDestroy(
    saTransaction Transaction)
{
    saUser owningFromUser = saTransactionGetFromUser(Transaction);
    saUser owningToUser = saTransactionGetToUser(Transaction);

    if(saTransactionDestructorCallback != NULL) {
        saTransactionDestructorCallback(Transaction);
    }
    if(owningFromUser != saUserNull) {
        saUserRemoveOutTransaction(owningFromUser, Transaction);
#if defined(DD_DEBUG)
    } else {
        utExit("Transaction without owning User");
#endif
    }
    if(owningToUser != saUserNull) {
        saUserRemoveInTransaction(owningToUser, Transaction);
#if defined(DD_DEBUG)
    } else {
        utExit("Transaction without owning User");
#endif
    }
    saTransactionFree(Transaction);
}

/*----------------------------------------------------------------------------------------
  Default constructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static uint64 allocTransaction(void)
{
    saTransaction Transaction = saTransactionAlloc();

    return saTransaction2Index(Transaction);
}

/*----------------------------------------------------------------------------------------
  Destructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static void destroyTransaction(
    uint64 objectIndex)
{
    saTransactionDestroy(saIndex2Transaction((uint32)objectIndex));
}

/*----------------------------------------------------------------------------------------
  Allocate the field arrays of Transaction.
----------------------------------------------------------------------------------------*/
static void allocTransactions(void)
{
    saSetAllocatedTransaction(2);
    saSetUsedTransaction(1);
    saSetFirstFreeTransaction(saTransactionNull);
    saTransactions.Date = utNewAInitFirst(uint64, (saAllocatedTransaction()));
    saTransactions.Amount = utNewAInitFirst(uint32, (saAllocatedTransaction()));
    saTransactions.DescriptionIndex_ = utNewAInitFirst(uint32, (saAllocatedTransaction()));
    saTransactions.NumDescription = utNewAInitFirst(uint32, (saAllocatedTransaction()));
    saSetUsedTransactionDescription(0);
    saSetAllocatedTransactionDescription(2);
    saSetFreeTransactionDescription(0);
    saTransactions.Description = utNewAInitFirst(char, saAllocatedTransactionDescription());
    saTransactions.FromUser = utNewAInitFirst(saUser, (saAllocatedTransaction()));
    saTransactions.NextUserOutTransaction = utNewAInitFirst(saTransaction, (saAllocatedTransaction()));
    saTransactions.PrevUserOutTransaction = utNewAInitFirst(saTransaction, (saAllocatedTransaction()));
    saTransactions.ToUser = utNewAInitFirst(saUser, (saAllocatedTransaction()));
    saTransactions.NextUserInTransaction = utNewAInitFirst(saTransaction, (saAllocatedTransaction()));
    saTransactions.PrevUserInTransaction = utNewAInitFirst(saTransaction, (saAllocatedTransaction()));
}

/*----------------------------------------------------------------------------------------
  Realloc the arrays of properties for class Transaction.
----------------------------------------------------------------------------------------*/
static void reallocTransactions(
    uint32 newSize)
{
    utResizeArray(saTransactions.Date, (newSize));
    utResizeArray(saTransactions.Amount, (newSize));
    utResizeArray(saTransactions.DescriptionIndex_, (newSize));
    utResizeArray(saTransactions.NumDescription, (newSize));
    utResizeArray(saTransactions.FromUser, (newSize));
    utResizeArray(saTransactions.NextUserOutTransaction, (newSize));
    utResizeArray(saTransactions.PrevUserOutTransaction, (newSize));
    utResizeArray(saTransactions.ToUser, (newSize));
    utResizeArray(saTransactions.NextUserInTransaction, (newSize));
    utResizeArray(saTransactions.PrevUserInTransaction, (newSize));
    saSetAllocatedTransaction(newSize);
}

/*----------------------------------------------------------------------------------------
  Allocate more Transactions.
----------------------------------------------------------------------------------------*/
void saTransactionAllocMore(void)
{
    reallocTransactions((uint32)(saAllocatedTransaction() + (saAllocatedTransaction() >> 1)));
}

/*----------------------------------------------------------------------------------------
  Compact the Transaction.Description heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactTransactionDescriptions(void)
{
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saTransaction) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saTransaction) + sizeof(uint32) + elementSize - 1)/elementSize;
    char *toPtr = saTransactions.Description;
    char *fromPtr = toPtr;
    saTransaction Transaction;
    uint32 size;

    while(fromPtr < saTransactions.Description + saUsedTransactionDescription()) {
        Transaction = *(saTransaction *)(void *)fromPtr;
        if(Transaction != saTransactionNull) {
            /* Need to move it to toPtr */
            size = utMax(saTransactionGetNumDescription(Transaction) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saTransactionSetDescriptionIndex_(Transaction, toPtr - saTransactions.Description + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saTransaction *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedTransactionDescription(toPtr - saTransactions.Description);
    saSetFreeTransactionDescription(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the Transaction.Description heap.
----------------------------------------------------------------------------------------*/
static void allocMoreTransactionDescriptions(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedTransactionDescription() - saUsedTransactionDescription();

    if((saFreeTransactionDescription() << 2) > saUsedTransactionDescription()) {
        saCompactTransactionDescriptions();
        freeSpace = saAllocatedTransactionDescription() - saUsedTransactionDescription();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedTransactionDescription(saAllocatedTransactionDescription() + spaceNeeded - freeSpace +
            (saAllocatedTransactionDescription() >> 1));
        utResizeArray(saTransactions.Description, saAllocatedTransactionDescription());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new Transaction.Description array.
----------------------------------------------------------------------------------------*/
void saTransactionAllocDescriptions(
    saTransaction Transaction,
    uint32 numDescriptions)
{
    uint32 freeSpace = saAllocatedTransactionDescription() - saUsedTransactionDescription();
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saTransaction) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saTransaction) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numDescriptions + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saTransactionGetNumDescription(Transaction) == 0);
#endif
    if(numDescriptions == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMoreTransactionDescriptions(spaceNeeded);
    }
    saTransactionSetDescriptionIndex_(Transaction, saUsedTransactionDescription() + usedHeaderSize);
    saTransactionSetNumDescription(Transaction, numDescriptions);
    *(saTransaction *)(void *)(saTransactions.Description + saUsedTransactionDescription()) = Transaction;
    memset(saTransactions.Description + saTransactionGetDescriptionIndex_(Transaction), 0, ((numDescriptions))*sizeof(char));
    saSetUsedTransactionDescription(saUsedTransactionDescription() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saTransactionGetDescriptions for the database manager.
----------------------------------------------------------------------------------------*/
static void *getTransactionDescriptions(
    uint64 objectNumber,
    uint32 *numValues)
{
    saTransaction Transaction = saIndex2Transaction((uint32)objectNumber);

    *numValues = saTransactionGetNumDescription(Transaction);
    return saTransactionGetDescriptions(Transaction);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saTransactionAllocDescriptions for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocTransactionDescriptions(
    uint64 objectNumber,
    uint32 numValues)
{
    saTransaction Transaction = saIndex2Transaction((uint32)objectNumber);

    saTransactionSetDescriptionIndex_(Transaction, 0);
    saTransactionSetNumDescription(Transaction, 0);
    if(numValues == 0) {
        return NULL;
    }
    saTransactionAllocDescriptions(Transaction, numValues);
    return saTransactionGetDescriptions(Transaction);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the Transaction.Description array.
----------------------------------------------------------------------------------------*/
void saTransactionFreeDescriptions(
    saTransaction Transaction)
{
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saTransaction) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saTransaction) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saTransactionGetNumDescription(Transaction) + usedHeaderSize, freeHeaderSize);
    char *dataPtr = saTransactionGetDescriptions(Transaction) - usedHeaderSize;

    if(saTransactionGetNumDescription(Transaction) == 0) {
        return;
    }
    *(saTransaction *)(void *)(dataPtr) = saTransactionNull;
    *(uint32 *)(void *)(((saTransaction *)(void *)dataPtr) + 1) = size;
    saTransactionSetNumDescription(Transaction, 0);
    saSetFreeTransactionDescription(saFreeTransactionDescription() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the Transaction.Description array.
----------------------------------------------------------------------------------------*/
void saTransactionResizeDescriptions(
    saTransaction Transaction,
    uint32 numDescriptions)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saTransaction) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saTransaction) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numDescriptions + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saTransactionGetNumDescription(Transaction) + usedHeaderSize, freeHeaderSize);
    char *dataPtr;

    if(numDescriptions == 0) {
        if(saTransactionGetNumDescription(Transaction) != 0) {
            saTransactionFreeDescriptions(Transaction);
        }
        return;
    }
    if(saTransactionGetNumDescription(Transaction) == 0) {
        saTransactionAllocDescriptions(Transaction, numDescriptions);
        return;
    }
    freeSpace = saAllocatedTransactionDescription() - saUsedTransactionDescription();
    if(freeSpace < newSize) {
        allocMoreTransactionDescriptions(newSize);
    }
    dataPtr = saTransactionGetDescriptions(Transaction) - usedHeaderSize;
    memcpy((void *)(saTransactions.Description + saUsedTransactionDescription()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        memset(saTransactions.Description + saUsedTransactionDescription() + oldSize, 0, ((newSize - oldSize))*sizeof(char));
    }
    *(saTransaction *)(void *)dataPtr = saTransactionNull;
    *(uint32 *)(void *)(((saTransaction *)(void *)dataPtr) + 1) = oldSize;
    saSetFreeTransactionDescription(saFreeTransactionDescription() + oldSize);
    saTransactionSetDescriptionIndex_(Transaction, saUsedTransactionDescription() + usedHeaderSize);
    saTransactionSetNumDescription(Transaction, numDescriptions);
    saSetUsedTransactionDescription(saUsedTransactionDescription() + newSize);
}

/*----------------------------------------------------------------------------------------
  Copy the properties of Transaction.
----------------------------------------------------------------------------------------*/
void saTransactionCopyProps(
    saTransaction oldTransaction,
    saTransaction newTransaction)
{
    saTransactionSetDate(newTransaction, saTransactionGetDate(oldTransaction));
    saTransactionSetAmount(newTransaction, saTransactionGetAmount(oldTransaction));
}

#if defined(DD_DEBUG)
/*----------------------------------------------------------------------------------------
  Write out all the fields of an object.
----------------------------------------------------------------------------------------*/
void saShowTransaction(
    saTransaction Transaction)
{
    utDatabaseShowObject("sa", "Transaction", saTransaction2Index(Transaction));
}
#endif

/*----------------------------------------------------------------------------------------
  Destroy Charity including everything in it. Remove from parents.
----------------------------------------------------------------------------------------*/
void saCharityDestroy(
    saCharity Charity)
{
    saMembership Membership_;
    saRoot owningRoot = saCharityGetRoot(Charity);
    saUser owningUser = saCharityGetUser(Charity);

    if(saCharityDestructorCallback != NULL) {
        saCharityDestructorCallback(Charity);
    }
    saSafeForeachCharityMembership(Charity, Membership_) {
        saMembershipDestroy(Membership_);
    } saEndSafeCharityMembership;
    if(owningRoot != saRootNull) {
        saRootRemoveCharity(owningRoot, Charity);
    }
    if(owningUser != saUserNull) {
        saUserSetCharity(owningUser, saCharityNull);
#if defined(DD_DEBUG)
    } else {
        utExit("Charity without owning User");
#endif
    }
    saCharityFree(Charity);
}

/*----------------------------------------------------------------------------------------
  Default constructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static uint64 allocCharity(void)
{
    saCharity Charity = saCharityAlloc();

    return saCharity2Index(Charity);
}

/*----------------------------------------------------------------------------------------
  Destructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static void destroyCharity(
    uint64 objectIndex)
{
    saCharityDestroy(saIndex2Charity((uint32)objectIndex));
}

/*----------------------------------------------------------------------------------------
  Allocate the field arrays of Charity.
----------------------------------------------------------------------------------------*/
static void allocCharitys(void)
{
    saSetAllocatedCharity(2);
    saSetUsedCharity(1);
    saSetFirstFreeCharity(saCharityNull);
    saCharitys.Root = utNewAInitFirst(saRoot, (saAllocatedCharity()));
    saCharitys.NextRootCharity = utNewAInitFirst(saCharity, (saAllocatedCharity()));
    saCharitys.PrevRootCharity = utNewAInitFirst(saCharity, (saAllocatedCharity()));
    saCharitys.User = utNewAInitFirst(saUser, (saAllocatedCharity()));
    saCharitys.FirstMembership = utNewAInitFirst(saMembership, (saAllocatedCharity()));
    saCharitys.LastMembership = utNewAInitFirst(saMembership, (saAllocatedCharity()));
}

/*----------------------------------------------------------------------------------------
  Realloc the arrays of properties for class Charity.
----------------------------------------------------------------------------------------*/
static void reallocCharitys(
    uint32 newSize)
{
    utResizeArray(saCharitys.Root, (newSize));
    utResizeArray(saCharitys.NextRootCharity, (newSize));
    utResizeArray(saCharitys.PrevRootCharity, (newSize));
    utResizeArray(saCharitys.User, (newSize));
    utResizeArray(saCharitys.FirstMembership, (newSize));
    utResizeArray(saCharitys.LastMembership, (newSize));
    saSetAllocatedCharity(newSize);
}

/*----------------------------------------------------------------------------------------
  Allocate more Charitys.
----------------------------------------------------------------------------------------*/
void saCharityAllocMore(void)
{
    reallocCharitys((uint32)(saAllocatedCharity() + (saAllocatedCharity() >> 1)));
}

/*----------------------------------------------------------------------------------------
  Copy the properties of Charity.
----------------------------------------------------------------------------------------*/
void saCharityCopyProps(
    saCharity oldCharity,
    saCharity newCharity)
{
}

/*----------------------------------------------------------------------------------------
  Add the Membership to the head of the list on the Charity.
----------------------------------------------------------------------------------------*/
void saCharityInsertMembership(
    saCharity Charity,
    saMembership _Membership)
{
#if defined(DD_DEBUG)
    if(Charity == saCharityNull) {
        utExit("Non-existent Charity");
    }
    if(_Membership == saMembershipNull) {
        utExit("Non-existent Membership");
    }
    if(saMembershipGetCharity(_Membership) != saCharityNull) {
        utExit("Attempting to add Membership to Charity twice");
    }
#endif
    saMembershipSetNextCharityMembership(_Membership, saCharityGetFirstMembership(Charity));
    if(saCharityGetFirstMembership(Charity) != saMembershipNull) {
        saMembershipSetPrevCharityMembership(saCharityGetFirstMembership(Charity), _Membership);
    }
    saCharitySetFirstMembership(Charity, _Membership);
    saMembershipSetPrevCharityMembership(_Membership, saMembershipNull);
    if(saCharityGetLastMembership(Charity) == saMembershipNull) {
        saCharitySetLastMembership(Charity, _Membership);
    }
    saMembershipSetCharity(_Membership, Charity);
}

/*----------------------------------------------------------------------------------------
  Add the Membership to the end of the list on the Charity.
----------------------------------------------------------------------------------------*/
void saCharityAppendMembership(
    saCharity Charity,
    saMembership _Membership)
{
#if defined(DD_DEBUG)
    if(Charity == saCharityNull) {
        utExit("Non-existent Charity");
    }
    if(_Membership == saMembershipNull) {
        utExit("Non-existent Membership");
    }
    if(saMembershipGetCharity(_Membership) != saCharityNull) {
        utExit("Attempting to add Membership to Charity twice");
    }
#endif
    saMembershipSetPrevCharityMembership(_Membership, saCharityGetLastMembership(Charity));
    if(saCharityGetLastMembership(Charity) != saMembershipNull) {
        saMembershipSetNextCharityMembership(saCharityGetLastMembership(Charity), _Membership);
    }
    saCharitySetLastMembership(Charity, _Membership);
    saMembershipSetNextCharityMembership(_Membership, saMembershipNull);
    if(saCharityGetFirstMembership(Charity) == saMembershipNull) {
        saCharitySetFirstMembership(Charity, _Membership);
    }
    saMembershipSetCharity(_Membership, Charity);
}

/*----------------------------------------------------------------------------------------
  Insert the Membership to the Charity after the previous Membership.
----------------------------------------------------------------------------------------*/
void saCharityInsertAfterMembership(
    saCharity Charity,
    saMembership prevMembership,
    saMembership _Membership)
{
    saMembership nextMembership = saMembershipGetNextCharityMembership(prevMembership);

#if defined(DD_DEBUG)
    if(Charity == saCharityNull) {
        utExit("Non-existent Charity");
    }
    if(_Membership == saMembershipNull) {
        utExit("Non-existent Membership");
    }
    if(saMembershipGetCharity(_Membership) != saCharityNull) {
        utExit("Attempting to add Membership to Charity twice");
    }
#endif
    saMembershipSetNextCharityMembership(_Membership, nextMembership);
    saMembershipSetNextCharityMembership(prevMembership, _Membership);
    saMembershipSetPrevCharityMembership(_Membership, prevMembership);
    if(nextMembership != saMembershipNull) {
        saMembershipSetPrevCharityMembership(nextMembership, _Membership);
    }
    if(saCharityGetLastMembership(Charity) == prevMembership) {
        saCharitySetLastMembership(Charity, _Membership);
    }
    saMembershipSetCharity(_Membership, Charity);
}

/*----------------------------------------------------------------------------------------
 Remove the Membership from the Charity.
----------------------------------------------------------------------------------------*/
void saCharityRemoveMembership(
    saCharity Charity,
    saMembership _Membership)
{
    saMembership pMembership, nMembership;

#if defined(DD_DEBUG)
    if(_Membership == saMembershipNull) {
        utExit("Non-existent Membership");
    }
    if(saMembershipGetCharity(_Membership) != saCharityNull && saMembershipGetCharity(_Membership) != Charity) {
        utExit("Delete Membership from non-owning Charity");
    }
#endif
    nMembership = saMembershipGetNextCharityMembership(_Membership);
    pMembership = saMembershipGetPrevCharityMembership(_Membership);
    if(pMembership != saMembershipNull) {
        saMembershipSetNextCharityMembership(pMembership, nMembership);
    } else if(saCharityGetFirstMembership(Charity) == _Membership) {
        saCharitySetFirstMembership(Charity, nMembership);
    }
    if(nMembership != saMembershipNull) {
        saMembershipSetPrevCharityMembership(nMembership, pMembership);
    } else if(saCharityGetLastMembership(Charity) == _Membership) {
        saCharitySetLastMembership(Charity, pMembership);
    }
    saMembershipSetNextCharityMembership(_Membership, saMembershipNull);
    saMembershipSetPrevCharityMembership(_Membership, saMembershipNull);
    saMembershipSetCharity(_Membership, saCharityNull);
}

#if defined(DD_DEBUG)
/*----------------------------------------------------------------------------------------
  Write out all the fields of an object.
----------------------------------------------------------------------------------------*/
void saShowCharity(
    saCharity Charity)
{
    utDatabaseShowObject("sa", "Charity", saCharity2Index(Charity));
}
#endif

/*----------------------------------------------------------------------------------------
  Destroy Membership including everything in it. Remove from parents.
----------------------------------------------------------------------------------------*/
void saMembershipDestroy(
    saMembership Membership)
{
    saCharity owningCharity = saMembershipGetCharity(Membership);
    saUser owningUser = saMembershipGetUser(Membership);

    if(saMembershipDestructorCallback != NULL) {
        saMembershipDestructorCallback(Membership);
    }
    if(owningUser != saUserNull) {
        saUserRemoveMembership(owningUser, Membership);
#if defined(DD_DEBUG)
    } else {
        utExit("Membership without owning User");
#endif
    }
    if(owningCharity != saCharityNull) {
        saCharityRemoveMembership(owningCharity, Membership);
#if defined(DD_DEBUG)
    } else {
        utExit("Membership without owning Charity");
#endif
    }
    saMembershipFree(Membership);
}

/*----------------------------------------------------------------------------------------
  Default constructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static uint64 allocMembership(void)
{
    saMembership Membership = saMembershipAlloc();

    return saMembership2Index(Membership);
}

/*----------------------------------------------------------------------------------------
  Destructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static void destroyMembership(
    uint64 objectIndex)
{
    saMembershipDestroy(saIndex2Membership((uint32)objectIndex));
}

/*----------------------------------------------------------------------------------------
  Allocate the field arrays of Membership.
----------------------------------------------------------------------------------------*/
static void allocMemberships(void)
{
    saSetAllocatedMembership(2);
    saSetUsedMembership(1);
    saSetFirstFreeMembership(saMembershipNull);
    saMemberships.User = utNewAInitFirst(saUser, (saAllocatedMembership()));
    saMemberships.NextUserMembership = utNewAInitFirst(saMembership, (saAllocatedMembership()));
    saMemberships.PrevUserMembership = utNewAInitFirst(saMembership, (saAllocatedMembership()));
    saMemberships.NextTableUserMembership = utNewAInitFirst(saMembership, (saAllocatedMembership()));
    saMemberships.Charity = utNewAInitFirst(saCharity, (saAllocatedMembership()));
    saMemberships.NextCharityMembership = utNewAInitFirst(saMembership, (saAllocatedMembership()));
    saMemberships.PrevCharityMembership = utNewAInitFirst(saMembership, (saAllocatedMembership()));
}

/*----------------------------------------------------------------------------------------
  Realloc the arrays of properties for class Membership.
----------------------------------------------------------------------------------------*/
static void reallocMemberships(
    uint32 newSize)
{
    utResizeArray(saMemberships.User, (newSize));
    utResizeArray(saMemberships.NextUserMembership, (newSize));
    utResizeArray(saMemberships.PrevUserMembership, (newSize));
    utResizeArray(saMemberships.NextTableUserMembership, (newSize));
    utResizeArray(saMemberships.Charity, (newSize));
    utResizeArray(saMemberships.NextCharityMembership, (newSize));
    utResizeArray(saMemberships.PrevCharityMembership, (newSize));
    saSetAllocatedMembership(newSize);
}

/*----------------------------------------------------------------------------------------
  Allocate more Memberships.
----------------------------------------------------------------------------------------*/
void saMembershipAllocMore(void)
{
    reallocMemberships((uint32)(saAllocatedMembership() + (saAllocatedMembership() >> 1)));
}

/*----------------------------------------------------------------------------------------
  Copy the properties of Membership.
----------------------------------------------------------------------------------------*/
void saMembershipCopyProps(
    saMembership oldMembership,
    saMembership newMembership)
{
}

#if defined(DD_DEBUG)
/*----------------------------------------------------------------------------------------
  Write out all the fields of an object.
----------------------------------------------------------------------------------------*/
void saShowMembership(
    saMembership Membership)
{
    utDatabaseShowObject("sa", "Membership", saMembership2Index(Membership));
}
#endif

/*----------------------------------------------------------------------------------------
  Destroy Category including everything in it. Remove from parents.
----------------------------------------------------------------------------------------*/
void saCategoryDestroy(
    saCategory Category)
{
    saListing Listing_;
    saRoot owningRoot = saCategoryGetRoot(Category);

    if(saCategoryDestructorCallback != NULL) {
        saCategoryDestructorCallback(Category);
    }
    saSafeForeachCategoryListing(Category, Listing_) {
        saListingDestroy(Listing_);
    } saEndSafeCategoryListing;
    if(owningRoot != saRootNull) {
        saRootRemoveCategory(owningRoot, Category);
    }
    saCategoryFree(Category);
}

/*----------------------------------------------------------------------------------------
  Default constructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static uint64 allocCategory(void)
{
    saCategory Category = saCategoryAlloc();

    return saCategory2Index(Category);
}

/*----------------------------------------------------------------------------------------
  Destructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static void destroyCategory(
    uint64 objectIndex)
{
    saCategoryDestroy(saIndex2Category((uint32)objectIndex));
}

/*----------------------------------------------------------------------------------------
  Allocate the field arrays of Category.
----------------------------------------------------------------------------------------*/
static void allocCategorys(void)
{
    saSetAllocatedCategory(2);
    saSetUsedCategory(1);
    saSetFirstFreeCategory(saCategoryNull);
    saCategorys.Sym = utNewAInitFirst(utSym, (saAllocatedCategory()));
    saCategorys.NumListings = utNewAInitFirst(uint32, (saAllocatedCategory()));
    saCategorys.Root = utNewAInitFirst(saRoot, (saAllocatedCategory()));
    saCategorys.NextRootCategory = utNewAInitFirst(saCategory, (saAllocatedCategory()));
    saCategorys.PrevRootCategory = utNewAInitFirst(saCategory, (saAllocatedCategory()));
    saCategorys.NextTableRootCategory = utNewAInitFirst(saCategory, (saAllocatedCategory()));
    saCategorys.FirstListing = utNewAInitFirst(saListing, (saAllocatedCategory()));
    saCategorys.LastListing = utNewAInitFirst(saListing, (saAllocatedCategory()));
}

/*----------------------------------------------------------------------------------------
  Realloc the arrays of properties for class Category.
----------------------------------------------------------------------------------------*/
static void reallocCategorys(
    uint32 newSize)
{
    utResizeArray(saCategorys.Sym, (newSize));
    utResizeArray(saCategorys.NumListings, (newSize));
    utResizeArray(saCategorys.Root, (newSize));
    utResizeArray(saCategorys.NextRootCategory, (newSize));
    utResizeArray(saCategorys.PrevRootCategory, (newSize));
    utResizeArray(saCategorys.NextTableRootCategory, (newSize));
    utResizeArray(saCategorys.FirstListing, (newSize));
    utResizeArray(saCategorys.LastListing, (newSize));
    saSetAllocatedCategory(newSize);
}

/*----------------------------------------------------------------------------------------
  Allocate more Categorys.
----------------------------------------------------------------------------------------*/
void saCategoryAllocMore(void)
{
    reallocCategorys((uint32)(saAllocatedCategory() + (saAllocatedCategory() >> 1)));
}

/*----------------------------------------------------------------------------------------
  Copy the properties of Category.
----------------------------------------------------------------------------------------*/
void saCategoryCopyProps(
    saCategory oldCategory,
    saCategory newCategory)
{
    saCategorySetNumListings(newCategory, saCategoryGetNumListings(oldCategory));
}

/*----------------------------------------------------------------------------------------
  Add the Listing to the head of the list on the Category.
----------------------------------------------------------------------------------------*/
void saCategoryInsertListing(
    saCategory Category,
    saListing _Listing)
{
#if defined(DD_DEBUG)
    if(Category == saCategoryNull) {
        utExit("Non-existent Category");
    }
    if(_Listing == saListingNull) {
        utExit("Non-existent Listing");
    }
    if(saListingGetCategory(_Listing) != saCategoryNull) {
        utExit("Attempting to add Listing to Category twice");
    }
#endif
    saListingSetNextCategoryListing(_Listing, saCategoryGetFirstListing(Category));
    if(saCategoryGetFirstListing(Category) != saListingNull) {
        saListingSetPrevCategoryListing(saCategoryGetFirstListing(Category), _Listing);
    }
    saCategorySetFirstListing(Category, _Listing);
    saListingSetPrevCategoryListing(_Listing, saListingNull);
    if(saCategoryGetLastListing(Category) == saListingNull) {
        saCategorySetLastListing(Category, _Listing);
    }
    saListingSetCategory(_Listing, Category);
}

/*----------------------------------------------------------------------------------------
  Add the Listing to the end of the list on the Category.
----------------------------------------------------------------------------------------*/
void saCategoryAppendListing(
    saCategory Category,
    saListing _Listing)
{
#if defined(DD_DEBUG)
    if(Category == saCategoryNull) {
        utExit("Non-existent Category");
    }
    if(_Listing == saListingNull) {
        utExit("Non-existent Listing");
    }
    if(saListingGetCategory(_Listing) != saCategoryNull) {
        utExit("Attempting to add Listing to Category twice");
    }
#endif
    saListingSetPrevCategoryListing(_Listing, saCategoryGetLastListing(Category));
    if(saCategoryGetLastListing(Category) != saListingNull) {
        saListingSetNextCategoryListing(saCategoryGetLastListing(Category), _Listing);
    }
    saCategorySetLastListing(Category, _Listing);
    saListingSetNextCategoryListing(_Listing, saListingNull);
    if(saCategoryGetFirstListing(Category) == saListingNull) {
        saCategorySetFirstListing(Category, _Listing);
    }
    saListingSetCategory(_Listing, Category);
}

/*----------------------------------------------------------------------------------------
  Insert the Listing to the Category after the previous Listing.
----------------------------------------------------------------------------------------*/
void saCategoryInsertAfterListing(
    saCategory Category,
    saListing prevListing,
    saListing _Listing)
{
    saListing nextListing = saListingGetNextCategoryListing(prevListing);

#if defined(DD_DEBUG)
    if(Category == saCategoryNull) {
        utExit("Non-existent Category");
    }
    if(_Listing == saListingNull) {
        utExit("Non-existent Listing");
    }
    if(saListingGetCategory(_Listing) != saCategoryNull) {
        utExit("Attempting to add Listing to Category twice");
    }
#endif
    saListingSetNextCategoryListing(_Listing, nextListing);
    saListingSetNextCategoryListing(prevListing, _Listing);
    saListingSetPrevCategoryListing(_Listing, prevListing);
    if(nextListing != saListingNull) {
        saListingSetPrevCategoryListing(nextListing, _Listing);
    }
    if(saCategoryGetLastListing(Category) == prevListing) {
        saCategorySetLastListing(Category, _Listing);
    }
    saListingSetCategory(_Listing, Category);
}

/*----------------------------------------------------------------------------------------
 Remove the Listing from the Category.
----------------------------------------------------------------------------------------*/
void saCategoryRemoveListing(
    saCategory Category,
    saListing _Listing)
{
    saListing pListing, nListing;

#if defined(DD_DEBUG)
    if(_Listing == saListingNull) {
        utExit("Non-existent Listing");
    }
    if(saListingGetCategory(_Listing) != saCategoryNull && saListingGetCategory(_Listing) != Category) {
        utExit("Delete Listing from non-owning Category");
    }
#endif
    nListing = saListingGetNextCategoryListing(_Listing);
    pListing = saListingGetPrevCategoryListing(_Listing);
    if(pListing != saListingNull) {
        saListingSetNextCategoryListing(pListing, nListing);
    } else if(saCategoryGetFirstListing(Category) == _Listing) {
        saCategorySetFirstListing(Category, nListing);
    }
    if(nListing != saListingNull) {
        saListingSetPrevCategoryListing(nListing, pListing);
    } else if(saCategoryGetLastListing(Category) == _Listing) {
        saCategorySetLastListing(Category, pListing);
    }
    saListingSetNextCategoryListing(_Listing, saListingNull);
    saListingSetPrevCategoryListing(_Listing, saListingNull);
    saListingSetCategory(_Listing, saCategoryNull);
}

#if defined(DD_DEBUG)
/*----------------------------------------------------------------------------------------
  Write out all the fields of an object.
----------------------------------------------------------------------------------------*/
void saShowCategory(
    saCategory Category)
{
    utDatabaseShowObject("sa", "Category", saCategory2Index(Category));
}
#endif

/*----------------------------------------------------------------------------------------
  Destroy Listing including everything in it. Remove from parents.
----------------------------------------------------------------------------------------*/
void saListingDestroy(
    saListing Listing)
{
    saUser owningUser = saListingGetUser(Listing);
    saCategory owningCategory = saListingGetCategory(Listing);
    saRoot owningRoot = saListingGetRoot(Listing);

    if(saListingDestructorCallback != NULL) {
        saListingDestructorCallback(Listing);
    }
    if(owningRoot != saRootNull) {
        saRootRemoveListing(owningRoot, Listing);
    }
    if(owningUser != saUserNull) {
        saUserRemoveListing(owningUser, Listing);
#if defined(DD_DEBUG)
    } else {
        utExit("Listing without owning User");
#endif
    }
    if(owningCategory != saCategoryNull) {
        saCategoryRemoveListing(owningCategory, Listing);
#if defined(DD_DEBUG)
    } else {
        utExit("Listing without owning Category");
#endif
    }
    saListingFree(Listing);
}

/*----------------------------------------------------------------------------------------
  Default constructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static uint64 allocListing(void)
{
    saListing Listing = saListingAlloc();

    return saListing2Index(Listing);
}

/*----------------------------------------------------------------------------------------
  Destructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static void destroyListing(
    uint64 objectIndex)
{
    saListingDestroy(saIndex2Listing((uint32)objectIndex));
}

/*----------------------------------------------------------------------------------------
  Allocate the field arrays of Listing.
----------------------------------------------------------------------------------------*/
static void allocListings(void)
{
    saSetAllocatedListing(2);
    saSetUsedListing(1);
    saSetFirstFreeListing(saListingNull);
    saListings.TitleIndex_ = utNewAInitFirst(uint32, (saAllocatedListing()));
    saListings.NumTitle = utNewAInitFirst(uint32, (saAllocatedListing()));
    saSetUsedListingTitle(0);
    saSetAllocatedListingTitle(2);
    saSetFreeListingTitle(0);
    saListings.Title = utNewAInitFirst(char, saAllocatedListingTitle());
    saListings.DescriptionIndex_ = utNewAInitFirst(uint32, (saAllocatedListing()));
    saListings.NumDescription = utNewAInitFirst(uint32, (saAllocatedListing()));
    saSetUsedListingDescription(0);
    saSetAllocatedListingDescription(2);
    saSetFreeListingDescription(0);
    saListings.Description = utNewAInitFirst(char, saAllocatedListingDescription());
    saListings.ID = utNewAInitFirst(uint64, (saAllocatedListing()));
    saListings.Rate = utNewAInitFirst(uint32, (saAllocatedListing()));
    saListings.Offered = utNewAInitFirst(uint8, (saAllocatedListing()));
    saListings.FixedPrice = utNewAInitFirst(uint8, (saAllocatedListing()));
    saListings.Root = utNewAInitFirst(saRoot, (saAllocatedListing()));
    saListings.NextRootListing = utNewAInitFirst(saListing, (saAllocatedListing()));
    saListings.PrevRootListing = utNewAInitFirst(saListing, (saAllocatedListing()));
    saListings.NextTableRootListing = utNewAInitFirst(saListing, (saAllocatedListing()));
    saListings.User = utNewAInitFirst(saUser, (saAllocatedListing()));
    saListings.NextUserListing = utNewAInitFirst(saListing, (saAllocatedListing()));
    saListings.PrevUserListing = utNewAInitFirst(saListing, (saAllocatedListing()));
    saListings.Category = utNewAInitFirst(saCategory, (saAllocatedListing()));
    saListings.NextCategoryListing = utNewAInitFirst(saListing, (saAllocatedListing()));
    saListings.PrevCategoryListing = utNewAInitFirst(saListing, (saAllocatedListing()));
}

/*----------------------------------------------------------------------------------------
  Realloc the arrays of properties for class Listing.
----------------------------------------------------------------------------------------*/
static void reallocListings(
    uint32 newSize)
{
    utResizeArray(saListings.TitleIndex_, (newSize));
    utResizeArray(saListings.NumTitle, (newSize));
    utResizeArray(saListings.DescriptionIndex_, (newSize));
    utResizeArray(saListings.NumDescription, (newSize));
    utResizeArray(saListings.ID, (newSize));
    utResizeArray(saListings.Rate, (newSize));
    utResizeArray(saListings.Offered, (newSize));
    utResizeArray(saListings.FixedPrice, (newSize));
    utResizeArray(saListings.Root, (newSize));
    utResizeArray(saListings.NextRootListing, (newSize));
    utResizeArray(saListings.PrevRootListing, (newSize));
    utResizeArray(saListings.NextTableRootListing, (newSize));
    utResizeArray(saListings.User, (newSize));
    utResizeArray(saListings.NextUserListing, (newSize));
    utResizeArray(saListings.PrevUserListing, (newSize));
    utResizeArray(saListings.Category, (newSize));
    utResizeArray(saListings.NextCategoryListing, (newSize));
    utResizeArray(saListings.PrevCategoryListing, (newSize));
    saSetAllocatedListing(newSize);
}

/*----------------------------------------------------------------------------------------
  Allocate more Listings.
----------------------------------------------------------------------------------------*/
void saListingAllocMore(void)
{
    reallocListings((uint32)(saAllocatedListing() + (saAllocatedListing() >> 1)));
}

/*----------------------------------------------------------------------------------------
  Compact the Listing.Title heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactListingTitles(void)
{
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saListing) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saListing) + sizeof(uint32) + elementSize - 1)/elementSize;
    char *toPtr = saListings.Title;
    char *fromPtr = toPtr;
    saListing Listing;
    uint32 size;

    while(fromPtr < saListings.Title + saUsedListingTitle()) {
        Listing = *(saListing *)(void *)fromPtr;
        if(Listing != saListingNull) {
            /* Need to move it to toPtr */
            size = utMax(saListingGetNumTitle(Listing) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saListingSetTitleIndex_(Listing, toPtr - saListings.Title + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saListing *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedListingTitle(toPtr - saListings.Title);
    saSetFreeListingTitle(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the Listing.Title heap.
----------------------------------------------------------------------------------------*/
static void allocMoreListingTitles(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedListingTitle() - saUsedListingTitle();

    if((saFreeListingTitle() << 2) > saUsedListingTitle()) {
        saCompactListingTitles();
        freeSpace = saAllocatedListingTitle() - saUsedListingTitle();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedListingTitle(saAllocatedListingTitle() + spaceNeeded - freeSpace +
            (saAllocatedListingTitle() >> 1));
        utResizeArray(saListings.Title, saAllocatedListingTitle());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new Listing.Title array.
----------------------------------------------------------------------------------------*/
void saListingAllocTitles(
    saListing Listing,
    uint32 numTitles)
{
    uint32 freeSpace = saAllocatedListingTitle() - saUsedListingTitle();
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saListing) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saListing) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numTitles + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saListingGetNumTitle(Listing) == 0);
#endif
    if(numTitles == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMoreListingTitles(spaceNeeded);
    }
    saListingSetTitleIndex_(Listing, saUsedListingTitle() + usedHeaderSize);
    saListingSetNumTitle(Listing, numTitles);
    *(saListing *)(void *)(saListings.Title + saUsedListingTitle()) = Listing;
    memset(saListings.Title + saListingGetTitleIndex_(Listing), 0, ((numTitles))*sizeof(char));
    saSetUsedListingTitle(saUsedListingTitle() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saListingGetTitles for the database manager.
----------------------------------------------------------------------------------------*/
static void *getListingTitles(
    uint64 objectNumber,
    uint32 *numValues)
{
    saListing Listing = saIndex2Listing((uint32)objectNumber);

    *numValues = saListingGetNumTitle(Listing);
    return saListingGetTitles(Listing);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saListingAllocTitles for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocListingTitles(
    uint64 objectNumber,
    uint32 numValues)
{
    saListing Listing = saIndex2Listing((uint32)objectNumber);

    saListingSetTitleIndex_(Listing, 0);
    saListingSetNumTitle(Listing, 0);
    if(numValues == 0) {
        return NULL;
    }
    saListingAllocTitles(Listing, numValues);
    return saListingGetTitles(Listing);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the Listing.Title array.
----------------------------------------------------------------------------------------*/
void saListingFreeTitles(
    saListing Listing)
{
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saListing) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saListing) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saListingGetNumTitle(Listing) + usedHeaderSize, freeHeaderSize);
    char *dataPtr = saListingGetTitles(Listing) - usedHeaderSize;

    if(saListingGetNumTitle(Listing) == 0) {
        return;
    }
    *(saListing *)(void *)(dataPtr) = saListingNull;
    *(uint32 *)(void *)(((saListing *)(void *)dataPtr) + 1) = size;
    saListingSetNumTitle(Listing, 0);
    saSetFreeListingTitle(saFreeListingTitle() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the Listing.Title array.
----------------------------------------------------------------------------------------*/
void saListingResizeTitles(
    saListing Listing,
    uint32 numTitles)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saListing) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saListing) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numTitles + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saListingGetNumTitle(Listing) + usedHeaderSize, freeHeaderSize);
    char *dataPtr;

    if(numTitles == 0) {
        if(saListingGetNumTitle(Listing) != 0) {
            saListingFreeTitles(Listing);
        }
        return;
    }
    if(saListingGetNumTitle(Listing) == 0) {
        saListingAllocTitles(Listing, numTitles);
        return;
    }
    freeSpace = saAllocatedListingTitle() - saUsedListingTitle();
    if(freeSpace < newSize) {
        allocMoreListingTitles(newSize);
    }
    dataPtr = saListingGetTitles(Listing) - usedHeaderSize;
    memcpy((void *)(saListings.Title + saUsedListingTitle()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        memset(saListings.Title + saUsedListingTitle() + oldSize, 0, ((newSize - oldSize))*sizeof(char));
    }
    *(saListing *)(void *)dataPtr = saListingNull;
    *(uint32 *)(void *)(((saListing *)(void *)dataPtr) + 1) = oldSize;
    saSetFreeListingTitle(saFreeListingTitle() + oldSize);
    saListingSetTitleIndex_(Listing, saUsedListingTitle() + usedHeaderSize);
    saListingSetNumTitle(Listing, numTitles);
    saSetUsedListingTitle(saUsedListingTitle() + newSize);
}

/*----------------------------------------------------------------------------------------
  Compact the Listing.Description heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactListingDescriptions(void)
{
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saListing) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saListing) + sizeof(uint32) + elementSize - 1)/elementSize;
    char *toPtr = saListings.Description;
    char *fromPtr = toPtr;
    saListing Listing;
    uint32 size;

    while(fromPtr < saListings.Description + saUsedListingDescription()) {
        Listing = *(saListing *)(void *)fromPtr;
        if(Listing != saListingNull) {
            /* Need to move it to toPtr */
            size = utMax(saListingGetNumDescription(Listing) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saListingSetDescriptionIndex_(Listing, toPtr - saListings.Description + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saListing *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedListingDescription(toPtr - saListings.Description);
    saSetFreeListingDescription(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the Listing.Description heap.
----------------------------------------------------------------------------------------*/
static void allocMoreListingDescriptions(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedListingDescription() - saUsedListingDescription();

    if((saFreeListingDescription() << 2) > saUsedListingDescription()) {
        saCompactListingDescriptions();
        freeSpace = saAllocatedListingDescription() - saUsedListingDescription();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedListingDescription(saAllocatedListingDescription() + spaceNeeded - freeSpace +
            (saAllocatedListingDescription() >> 1));
        utResizeArray(saListings.Description, saAllocatedListingDescription());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new Listing.Description array.
----------------------------------------------------------------------------------------*/
void saListingAllocDescriptions(
    saListing Listing,
    uint32 numDescriptions)
{
    uint32 freeSpace = saAllocatedListingDescription() - saUsedListingDescription();
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saListing) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saListing) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numDescriptions + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saListingGetNumDescription(Listing) == 0);
#endif
    if(numDescriptions == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMoreListingDescriptions(spaceNeeded);
    }
    saListingSetDescriptionIndex_(Listing, saUsedListingDescription() + usedHeaderSize);
    saListingSetNumDescription(Listing, numDescriptions);
    *(saListing *)(void *)(saListings.Description + saUsedListingDescription()) = Listing;
    memset(saListings.Description + saListingGetDescriptionIndex_(Listing), 0, ((numDescriptions))*sizeof(char));
    saSetUsedListingDescription(saUsedListingDescription() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saListingGetDescriptions for the database manager.
----------------------------------------------------------------------------------------*/
static void *getListingDescriptions(
    uint64 objectNumber,
    uint32 *numValues)
{
    saListing Listing = saIndex2Listing((uint32)objectNumber);

    *numValues = saListingGetNumDescription(Listing);
    return saListingGetDescriptions(Listing);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saListingAllocDescriptions for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocListingDescriptions(
    uint64 objectNumber,
    uint32 numValues)
{
    saListing Listing = saIndex2Listing((uint32)objectNumber);

    saListingSetDescriptionIndex_(Listing, 0);
    saListingSetNumDescription(Listing, 0);
    if(numValues == 0) {
        return NULL;
    }
    saListingAllocDescriptions(Listing, numValues);
    return saListingGetDescriptions(Listing);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the Listing.Description array.
----------------------------------------------------------------------------------------*/
void saListingFreeDescriptions(
    saListing Listing)
{
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saListing) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saListing) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saListingGetNumDescription(Listing) + usedHeaderSize, freeHeaderSize);
    char *dataPtr = saListingGetDescriptions(Listing) - usedHeaderSize;

    if(saListingGetNumDescription(Listing) == 0) {
        return;
    }
    *(saListing *)(void *)(dataPtr) = saListingNull;
    *(uint32 *)(void *)(((saListing *)(void *)dataPtr) + 1) = size;
    saListingSetNumDescription(Listing, 0);
    saSetFreeListingDescription(saFreeListingDescription() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the Listing.Description array.
----------------------------------------------------------------------------------------*/
void saListingResizeDescriptions(
    saListing Listing,
    uint32 numDescriptions)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saListing) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saListing) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numDescriptions + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saListingGetNumDescription(Listing) + usedHeaderSize, freeHeaderSize);
    char *dataPtr;

    if(numDescriptions == 0) {
        if(saListingGetNumDescription(Listing) != 0) {
            saListingFreeDescriptions(Listing);
        }
        return;
    }
    if(saListingGetNumDescription(Listing) == 0) {
        saListingAllocDescriptions(Listing, numDescriptions);
        return;
    }
    freeSpace = saAllocatedListingDescription() - saUsedListingDescription();
    if(freeSpace < newSize) {
        allocMoreListingDescriptions(newSize);
    }
    dataPtr = saListingGetDescriptions(Listing) - usedHeaderSize;
    memcpy((void *)(saListings.Description + saUsedListingDescription()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        memset(saListings.Description + saUsedListingDescription() + oldSize, 0, ((newSize - oldSize))*sizeof(char));
    }
    *(saListing *)(void *)dataPtr = saListingNull;
    *(uint32 *)(void *)(((saListing *)(void *)dataPtr) + 1) = oldSize;
    saSetFreeListingDescription(saFreeListingDescription() + oldSize);
    saListingSetDescriptionIndex_(Listing, saUsedListingDescription() + usedHeaderSize);
    saListingSetNumDescription(Listing, numDescriptions);
    saSetUsedListingDescription(saUsedListingDescription() + newSize);
}

/*----------------------------------------------------------------------------------------
  Copy the properties of Listing.
----------------------------------------------------------------------------------------*/
void saListingCopyProps(
    saListing oldListing,
    saListing newListing)
{
    saListingSetID(newListing, saListingGetID(oldListing));
    saListingSetRate(newListing, saListingGetRate(oldListing));
    saListingSetOffered(newListing, saListingOffered(oldListing));
    saListingSetFixedPrice(newListing, saListingFixedPrice(oldListing));
}

#if defined(DD_DEBUG)
/*----------------------------------------------------------------------------------------
  Write out all the fields of an object.
----------------------------------------------------------------------------------------*/
void saShowListing(
    saListing Listing)
{
    utDatabaseShowObject("sa", "Listing", saListing2Index(Listing));
}
#endif

/*----------------------------------------------------------------------------------------
  Destroy Thread including everything in it. Remove from parents.
----------------------------------------------------------------------------------------*/
void saThreadDestroy(
    saThread Thread)
{
    saPost Post_;
    saSubscription Subscription_;
    saRoot owningRoot = saThreadGetRoot(Thread);

    if(saThreadDestructorCallback != NULL) {
        saThreadDestructorCallback(Thread);
    }
    saSafeForeachThreadPost(Thread, Post_) {
        saPostDestroy(Post_);
    } saEndSafeThreadPost;
    saSafeForeachThreadSubscription(Thread, Subscription_) {
        saSubscriptionDestroy(Subscription_);
    } saEndSafeThreadSubscription;
    if(owningRoot != saRootNull) {
        saRootRemoveThread(owningRoot, Thread);
    }
    saThreadFree(Thread);
}

/*----------------------------------------------------------------------------------------
  Default constructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static uint64 allocThread(void)
{
    saThread Thread = saThreadAlloc();

    return saThread2Index(Thread);
}

/*----------------------------------------------------------------------------------------
  Destructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static void destroyThread(
    uint64 objectIndex)
{
    saThreadDestroy(saIndex2Thread((uint32)objectIndex));
}

/*----------------------------------------------------------------------------------------
  Allocate the field arrays of Thread.
----------------------------------------------------------------------------------------*/
static void allocThreads(void)
{
    saSetAllocatedThread(2);
    saSetUsedThread(1);
    saSetFirstFreeThread(saThreadNull);
    saThreads.SubjectIndex_ = utNewAInitFirst(uint32, (saAllocatedThread()));
    saThreads.NumSubject = utNewAInitFirst(uint32, (saAllocatedThread()));
    saSetUsedThreadSubject(0);
    saSetAllocatedThreadSubject(2);
    saSetFreeThreadSubject(0);
    saThreads.Subject = utNewAInitFirst(char, saAllocatedThreadSubject());
    saThreads.ID = utNewAInitFirst(uint32, (saAllocatedThread()));
    saThreads.NumPosts = utNewAInitFirst(uint32, (saAllocatedThread()));
    saThreads.Root = utNewAInitFirst(saRoot, (saAllocatedThread()));
    saThreads.NextRootThread = utNewAInitFirst(saThread, (saAllocatedThread()));
    saThreads.PrevRootThread = utNewAInitFirst(saThread, (saAllocatedThread()));
    saThreads.NextTableRootThread = utNewAInitFirst(saThread, (saAllocatedThread()));
    saThreads.FirstPost = utNewAInitFirst(saPost, (saAllocatedThread()));
    saThreads.LastPost = utNewAInitFirst(saPost, (saAllocatedThread()));
    saThreads.FirstSubscription = utNewAInitFirst(saSubscription, (saAllocatedThread()));
    saThreads.LastSubscription = utNewAInitFirst(saSubscription, (saAllocatedThread()));
}

/*----------------------------------------------------------------------------------------
  Realloc the arrays of properties for class Thread.
----------------------------------------------------------------------------------------*/
static void reallocThreads(
    uint32 newSize)
{
    utResizeArray(saThreads.SubjectIndex_, (newSize));
    utResizeArray(saThreads.NumSubject, (newSize));
    utResizeArray(saThreads.ID, (newSize));
    utResizeArray(saThreads.NumPosts, (newSize));
    utResizeArray(saThreads.Root, (newSize));
    utResizeArray(saThreads.NextRootThread, (newSize));
    utResizeArray(saThreads.PrevRootThread, (newSize));
    utResizeArray(saThreads.NextTableRootThread, (newSize));
    utResizeArray(saThreads.FirstPost, (newSize));
    utResizeArray(saThreads.LastPost, (newSize));
    utResizeArray(saThreads.FirstSubscription, (newSize));
    utResizeArray(saThreads.LastSubscription, (newSize));
    saSetAllocatedThread(newSize);
}

/*----------------------------------------------------------------------------------------
  Allocate more Threads.
----------------------------------------------------------------------------------------*/
void saThreadAllocMore(void)
{
    reallocThreads((uint32)(saAllocatedThread() + (saAllocatedThread() >> 1)));
}

/*----------------------------------------------------------------------------------------
  Compact the Thread.Subject heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactThreadSubjects(void)
{
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saThread) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saThread) + sizeof(uint32) + elementSize - 1)/elementSize;
    char *toPtr = saThreads.Subject;
    char *fromPtr = toPtr;
    saThread Thread;
    uint32 size;

    while(fromPtr < saThreads.Subject + saUsedThreadSubject()) {
        Thread = *(saThread *)(void *)fromPtr;
        if(Thread != saThreadNull) {
            /* Need to move it to toPtr */
            size = utMax(saThreadGetNumSubject(Thread) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saThreadSetSubjectIndex_(Thread, toPtr - saThreads.Subject + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saThread *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedThreadSubject(toPtr - saThreads.Subject);
    saSetFreeThreadSubject(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the Thread.Subject heap.
----------------------------------------------------------------------------------------*/
static void allocMoreThreadSubjects(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedThreadSubject() - saUsedThreadSubject();

    if((saFreeThreadSubject() << 2) > saUsedThreadSubject()) {
        saCompactThreadSubjects();
        freeSpace = saAllocatedThreadSubject() - saUsedThreadSubject();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedThreadSubject(saAllocatedThreadSubject() + spaceNeeded - freeSpace +
            (saAllocatedThreadSubject() >> 1));
        utResizeArray(saThreads.Subject, saAllocatedThreadSubject());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new Thread.Subject array.
----------------------------------------------------------------------------------------*/
void saThreadAllocSubjects(
    saThread Thread,
    uint32 numSubjects)
{
    uint32 freeSpace = saAllocatedThreadSubject() - saUsedThreadSubject();
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saThread) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saThread) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numSubjects + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saThreadGetNumSubject(Thread) == 0);
#endif
    if(numSubjects == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMoreThreadSubjects(spaceNeeded);
    }
    saThreadSetSubjectIndex_(Thread, saUsedThreadSubject() + usedHeaderSize);
    saThreadSetNumSubject(Thread, numSubjects);
    *(saThread *)(void *)(saThreads.Subject + saUsedThreadSubject()) = Thread;
    memset(saThreads.Subject + saThreadGetSubjectIndex_(Thread), 0, ((numSubjects))*sizeof(char));
    saSetUsedThreadSubject(saUsedThreadSubject() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saThreadGetSubjects for the database manager.
----------------------------------------------------------------------------------------*/
static void *getThreadSubjects(
    uint64 objectNumber,
    uint32 *numValues)
{
    saThread Thread = saIndex2Thread((uint32)objectNumber);

    *numValues = saThreadGetNumSubject(Thread);
    return saThreadGetSubjects(Thread);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saThreadAllocSubjects for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocThreadSubjects(
    uint64 objectNumber,
    uint32 numValues)
{
    saThread Thread = saIndex2Thread((uint32)objectNumber);

    saThreadSetSubjectIndex_(Thread, 0);
    saThreadSetNumSubject(Thread, 0);
    if(numValues == 0) {
        return NULL;
    }
    saThreadAllocSubjects(Thread, numValues);
    return saThreadGetSubjects(Thread);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the Thread.Subject array.
----------------------------------------------------------------------------------------*/
void saThreadFreeSubjects(
    saThread Thread)
{
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saThread) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saThread) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saThreadGetNumSubject(Thread) + usedHeaderSize, freeHeaderSize);
    char *dataPtr = saThreadGetSubjects(Thread) - usedHeaderSize;

    if(saThreadGetNumSubject(Thread) == 0) {
        return;
    }
    *(saThread *)(void *)(dataPtr) = saThreadNull;
    *(uint32 *)(void *)(((saThread *)(void *)dataPtr) + 1) = size;
    saThreadSetNumSubject(Thread, 0);
    saSetFreeThreadSubject(saFreeThreadSubject() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the Thread.Subject array.
----------------------------------------------------------------------------------------*/
void saThreadResizeSubjects(
    saThread Thread,
    uint32 numSubjects)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saThread) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saThread) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numSubjects + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saThreadGetNumSubject(Thread) + usedHeaderSize, freeHeaderSize);
    char *dataPtr;

    if(numSubjects == 0) {
        if(saThreadGetNumSubject(Thread) != 0) {
            saThreadFreeSubjects(Thread);
        }
        return;
    }
    if(saThreadGetNumSubject(Thread) == 0) {
        saThreadAllocSubjects(Thread, numSubjects);
        return;
    }
    freeSpace = saAllocatedThreadSubject() - saUsedThreadSubject();
    if(freeSpace < newSize) {
        allocMoreThreadSubjects(newSize);
    }
    dataPtr = saThreadGetSubjects(Thread) - usedHeaderSize;
    memcpy((void *)(saThreads.Subject + saUsedThreadSubject()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        memset(saThreads.Subject + saUsedThreadSubject() + oldSize, 0, ((newSize - oldSize))*sizeof(char));
    }
    *(saThread *)(void *)dataPtr = saThreadNull;
    *(uint32 *)(void *)(((saThread *)(void *)dataPtr) + 1) = oldSize;
    saSetFreeThreadSubject(saFreeThreadSubject() + oldSize);
    saThreadSetSubjectIndex_(Thread, saUsedThreadSubject() + usedHeaderSize);
    saThreadSetNumSubject(Thread, numSubjects);
    saSetUsedThreadSubject(saUsedThreadSubject() + newSize);
}

/*----------------------------------------------------------------------------------------
  Copy the properties of Thread.
----------------------------------------------------------------------------------------*/
void saThreadCopyProps(
    saThread oldThread,
    saThread newThread)
{
    saThreadSetID(newThread, saThreadGetID(oldThread));
    saThreadSetNumPosts(newThread, saThreadGetNumPosts(oldThread));
}

/*----------------------------------------------------------------------------------------
  Add the Post to the head of the list on the Thread.
----------------------------------------------------------------------------------------*/
void saThreadInsertPost(
    saThread Thread,
    saPost _Post)
{
#if defined(DD_DEBUG)
    if(Thread == saThreadNull) {
        utExit("Non-existent Thread");
    }
    if(_Post == saPostNull) {
        utExit("Non-existent Post");
    }
    if(saPostGetThread(_Post) != saThreadNull) {
        utExit("Attempting to add Post to Thread twice");
    }
#endif
    saPostSetNextThreadPost(_Post, saThreadGetFirstPost(Thread));
    if(saThreadGetFirstPost(Thread) != saPostNull) {
        saPostSetPrevThreadPost(saThreadGetFirstPost(Thread), _Post);
    }
    saThreadSetFirstPost(Thread, _Post);
    saPostSetPrevThreadPost(_Post, saPostNull);
    if(saThreadGetLastPost(Thread) == saPostNull) {
        saThreadSetLastPost(Thread, _Post);
    }
    saPostSetThread(_Post, Thread);
}

/*----------------------------------------------------------------------------------------
  Add the Post to the end of the list on the Thread.
----------------------------------------------------------------------------------------*/
void saThreadAppendPost(
    saThread Thread,
    saPost _Post)
{
#if defined(DD_DEBUG)
    if(Thread == saThreadNull) {
        utExit("Non-existent Thread");
    }
    if(_Post == saPostNull) {
        utExit("Non-existent Post");
    }
    if(saPostGetThread(_Post) != saThreadNull) {
        utExit("Attempting to add Post to Thread twice");
    }
#endif
    saPostSetPrevThreadPost(_Post, saThreadGetLastPost(Thread));
    if(saThreadGetLastPost(Thread) != saPostNull) {
        saPostSetNextThreadPost(saThreadGetLastPost(Thread), _Post);
    }
    saThreadSetLastPost(Thread, _Post);
    saPostSetNextThreadPost(_Post, saPostNull);
    if(saThreadGetFirstPost(Thread) == saPostNull) {
        saThreadSetFirstPost(Thread, _Post);
    }
    saPostSetThread(_Post, Thread);
}

/*----------------------------------------------------------------------------------------
  Insert the Post to the Thread after the previous Post.
----------------------------------------------------------------------------------------*/
void saThreadInsertAfterPost(
    saThread Thread,
    saPost prevPost,
    saPost _Post)
{
    saPost nextPost = saPostGetNextThreadPost(prevPost);

#if defined(DD_DEBUG)
    if(Thread == saThreadNull) {
        utExit("Non-existent Thread");
    }
    if(_Post == saPostNull) {
        utExit("Non-existent Post");
    }
    if(saPostGetThread(_Post) != saThreadNull) {
        utExit("Attempting to add Post to Thread twice");
    }
#endif
    saPostSetNextThreadPost(_Post, nextPost);
    saPostSetNextThreadPost(prevPost, _Post);
    saPostSetPrevThreadPost(_Post, prevPost);
    if(nextPost != saPostNull) {
        saPostSetPrevThreadPost(nextPost, _Post);
    }
    if(saThreadGetLastPost(Thread) == prevPost) {
        saThreadSetLastPost(Thread, _Post);
    }
    saPostSetThread(_Post, Thread);
}

/*----------------------------------------------------------------------------------------
 Remove the Post from the Thread.
----------------------------------------------------------------------------------------*/
void saThreadRemovePost(
    saThread Thread,
    saPost _Post)
{
    saPost pPost, nPost;

#if defined(DD_DEBUG)
    if(_Post == saPostNull) {
        utExit("Non-existent Post");
    }
    if(saPostGetThread(_Post) != saThreadNull && saPostGetThread(_Post) != Thread) {
        utExit("Delete Post from non-owning Thread");
    }
#endif
    nPost = saPostGetNextThreadPost(_Post);
    pPost = saPostGetPrevThreadPost(_Post);
    if(pPost != saPostNull) {
        saPostSetNextThreadPost(pPost, nPost);
    } else if(saThreadGetFirstPost(Thread) == _Post) {
        saThreadSetFirstPost(Thread, nPost);
    }
    if(nPost != saPostNull) {
        saPostSetPrevThreadPost(nPost, pPost);
    } else if(saThreadGetLastPost(Thread) == _Post) {
        saThreadSetLastPost(Thread, pPost);
    }
    saPostSetNextThreadPost(_Post, saPostNull);
    saPostSetPrevThreadPost(_Post, saPostNull);
    saPostSetThread(_Post, saThreadNull);
}

/*----------------------------------------------------------------------------------------
  Add the Subscription to the head of the list on the Thread.
----------------------------------------------------------------------------------------*/
void saThreadInsertSubscription(
    saThread Thread,
    saSubscription _Subscription)
{
#if defined(DD_DEBUG)
    if(Thread == saThreadNull) {
        utExit("Non-existent Thread");
    }
    if(_Subscription == saSubscriptionNull) {
        utExit("Non-existent Subscription");
    }
    if(saSubscriptionGetThread(_Subscription) != saThreadNull) {
        utExit("Attempting to add Subscription to Thread twice");
    }
#endif
    saSubscriptionSetNextThreadSubscription(_Subscription, saThreadGetFirstSubscription(Thread));
    if(saThreadGetFirstSubscription(Thread) != saSubscriptionNull) {
        saSubscriptionSetPrevThreadSubscription(saThreadGetFirstSubscription(Thread), _Subscription);
    }
    saThreadSetFirstSubscription(Thread, _Subscription);
    saSubscriptionSetPrevThreadSubscription(_Subscription, saSubscriptionNull);
    if(saThreadGetLastSubscription(Thread) == saSubscriptionNull) {
        saThreadSetLastSubscription(Thread, _Subscription);
    }
    saSubscriptionSetThread(_Subscription, Thread);
}

/*----------------------------------------------------------------------------------------
  Add the Subscription to the end of the list on the Thread.
----------------------------------------------------------------------------------------*/
void saThreadAppendSubscription(
    saThread Thread,
    saSubscription _Subscription)
{
#if defined(DD_DEBUG)
    if(Thread == saThreadNull) {
        utExit("Non-existent Thread");
    }
    if(_Subscription == saSubscriptionNull) {
        utExit("Non-existent Subscription");
    }
    if(saSubscriptionGetThread(_Subscription) != saThreadNull) {
        utExit("Attempting to add Subscription to Thread twice");
    }
#endif
    saSubscriptionSetPrevThreadSubscription(_Subscription, saThreadGetLastSubscription(Thread));
    if(saThreadGetLastSubscription(Thread) != saSubscriptionNull) {
        saSubscriptionSetNextThreadSubscription(saThreadGetLastSubscription(Thread), _Subscription);
    }
    saThreadSetLastSubscription(Thread, _Subscription);
    saSubscriptionSetNextThreadSubscription(_Subscription, saSubscriptionNull);
    if(saThreadGetFirstSubscription(Thread) == saSubscriptionNull) {
        saThreadSetFirstSubscription(Thread, _Subscription);
    }
    saSubscriptionSetThread(_Subscription, Thread);
}

/*----------------------------------------------------------------------------------------
  Insert the Subscription to the Thread after the previous Subscription.
----------------------------------------------------------------------------------------*/
void saThreadInsertAfterSubscription(
    saThread Thread,
    saSubscription prevSubscription,
    saSubscription _Subscription)
{
    saSubscription nextSubscription = saSubscriptionGetNextThreadSubscription(prevSubscription);

#if defined(DD_DEBUG)
    if(Thread == saThreadNull) {
        utExit("Non-existent Thread");
    }
    if(_Subscription == saSubscriptionNull) {
        utExit("Non-existent Subscription");
    }
    if(saSubscriptionGetThread(_Subscription) != saThreadNull) {
        utExit("Attempting to add Subscription to Thread twice");
    }
#endif
    saSubscriptionSetNextThreadSubscription(_Subscription, nextSubscription);
    saSubscriptionSetNextThreadSubscription(prevSubscription, _Subscription);
    saSubscriptionSetPrevThreadSubscription(_Subscription, prevSubscription);
    if(nextSubscription != saSubscriptionNull) {
        saSubscriptionSetPrevThreadSubscription(nextSubscription, _Subscription);
    }
    if(saThreadGetLastSubscription(Thread) == prevSubscription) {
        saThreadSetLastSubscription(Thread, _Subscription);
    }
    saSubscriptionSetThread(_Subscription, Thread);
}

/*----------------------------------------------------------------------------------------
 Remove the Subscription from the Thread.
----------------------------------------------------------------------------------------*/
void saThreadRemoveSubscription(
    saThread Thread,
    saSubscription _Subscription)
{
    saSubscription pSubscription, nSubscription;

#if defined(DD_DEBUG)
    if(_Subscription == saSubscriptionNull) {
        utExit("Non-existent Subscription");
    }
    if(saSubscriptionGetThread(_Subscription) != saThreadNull && saSubscriptionGetThread(_Subscription) != Thread) {
        utExit("Delete Subscription from non-owning Thread");
    }
#endif
    nSubscription = saSubscriptionGetNextThreadSubscription(_Subscription);
    pSubscription = saSubscriptionGetPrevThreadSubscription(_Subscription);
    if(pSubscription != saSubscriptionNull) {
        saSubscriptionSetNextThreadSubscription(pSubscription, nSubscription);
    } else if(saThreadGetFirstSubscription(Thread) == _Subscription) {
        saThreadSetFirstSubscription(Thread, nSubscription);
    }
    if(nSubscription != saSubscriptionNull) {
        saSubscriptionSetPrevThreadSubscription(nSubscription, pSubscription);
    } else if(saThreadGetLastSubscription(Thread) == _Subscription) {
        saThreadSetLastSubscription(Thread, pSubscription);
    }
    saSubscriptionSetNextThreadSubscription(_Subscription, saSubscriptionNull);
    saSubscriptionSetPrevThreadSubscription(_Subscription, saSubscriptionNull);
    saSubscriptionSetThread(_Subscription, saThreadNull);
}

#if defined(DD_DEBUG)
/*----------------------------------------------------------------------------------------
  Write out all the fields of an object.
----------------------------------------------------------------------------------------*/
void saShowThread(
    saThread Thread)
{
    utDatabaseShowObject("sa", "Thread", saThread2Index(Thread));
}
#endif

/*----------------------------------------------------------------------------------------
  Destroy Post including everything in it. Remove from parents.
----------------------------------------------------------------------------------------*/
void saPostDestroy(
    saPost Post)
{
    saRoot owningRoot = saPostGetRoot(Post);
    saThread owningThread = saPostGetThread(Post);
    saUser owningUser = saPostGetUser(Post);

    if(saPostDestructorCallback != NULL) {
        saPostDestructorCallback(Post);
    }
    if(owningRoot != saRootNull) {
        saRootRemovePost(owningRoot, Post);
    }
    if(owningUser != saUserNull) {
        saUserRemovePost(owningUser, Post);
#if defined(DD_DEBUG)
    } else {
        utExit("Post without owning User");
#endif
    }
    if(owningThread != saThreadNull) {
        saThreadRemovePost(owningThread, Post);
#if defined(DD_DEBUG)
    } else {
        utExit("Post without owning Thread");
#endif
    }
    saPostFree(Post);
}

/*----------------------------------------------------------------------------------------
  Default constructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static uint64 allocPost(void)
{
    saPost Post = saPostAlloc();

    return saPost2Index(Post);
}

/*----------------------------------------------------------------------------------------
  Destructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static void destroyPost(
    uint64 objectIndex)
{
    saPostDestroy(saIndex2Post((uint32)objectIndex));
}

/*----------------------------------------------------------------------------------------
  Allocate the field arrays of Post.
----------------------------------------------------------------------------------------*/
static void allocPosts(void)
{
    saSetAllocatedPost(2);
    saSetUsedPost(1);
    saSetFirstFreePost(saPostNull);
    saPosts.Date = utNewAInitFirst(uint64, (saAllocatedPost()));
    saPosts.MessageIndex_ = utNewAInitFirst(uint32, (saAllocatedPost()));
    saPosts.NumMessage = utNewAInitFirst(uint32, (saAllocatedPost()));
    saSetUsedPostMessage(0);
    saSetAllocatedPostMessage(2);
    saSetFreePostMessage(0);
    saPosts.Message = utNewAInitFirst(char, saAllocatedPostMessage());
    saPosts.ID = utNewAInitFirst(uint32, (saAllocatedPost()));
    saPosts.Root = utNewAInitFirst(saRoot, (saAllocatedPost()));
    saPosts.NextRootPost = utNewAInitFirst(saPost, (saAllocatedPost()));
    saPosts.PrevRootPost = utNewAInitFirst(saPost, (saAllocatedPost()));
    saPosts.NextTableRootPost = utNewAInitFirst(saPost, (saAllocatedPost()));
    saPosts.User = utNewAInitFirst(saUser, (saAllocatedPost()));
    saPosts.NextUserPost = utNewAInitFirst(saPost, (saAllocatedPost()));
    saPosts.PrevUserPost = utNewAInitFirst(saPost, (saAllocatedPost()));
    saPosts.NextTableUserPost = utNewAInitFirst(saPost, (saAllocatedPost()));
    saPosts.Thread = utNewAInitFirst(saThread, (saAllocatedPost()));
    saPosts.NextThreadPost = utNewAInitFirst(saPost, (saAllocatedPost()));
    saPosts.PrevThreadPost = utNewAInitFirst(saPost, (saAllocatedPost()));
}

/*----------------------------------------------------------------------------------------
  Realloc the arrays of properties for class Post.
----------------------------------------------------------------------------------------*/
static void reallocPosts(
    uint32 newSize)
{
    utResizeArray(saPosts.Date, (newSize));
    utResizeArray(saPosts.MessageIndex_, (newSize));
    utResizeArray(saPosts.NumMessage, (newSize));
    utResizeArray(saPosts.ID, (newSize));
    utResizeArray(saPosts.Root, (newSize));
    utResizeArray(saPosts.NextRootPost, (newSize));
    utResizeArray(saPosts.PrevRootPost, (newSize));
    utResizeArray(saPosts.NextTableRootPost, (newSize));
    utResizeArray(saPosts.User, (newSize));
    utResizeArray(saPosts.NextUserPost, (newSize));
    utResizeArray(saPosts.PrevUserPost, (newSize));
    utResizeArray(saPosts.NextTableUserPost, (newSize));
    utResizeArray(saPosts.Thread, (newSize));
    utResizeArray(saPosts.NextThreadPost, (newSize));
    utResizeArray(saPosts.PrevThreadPost, (newSize));
    saSetAllocatedPost(newSize);
}

/*----------------------------------------------------------------------------------------
  Allocate more Posts.
----------------------------------------------------------------------------------------*/
void saPostAllocMore(void)
{
    reallocPosts((uint32)(saAllocatedPost() + (saAllocatedPost() >> 1)));
}

/*----------------------------------------------------------------------------------------
  Compact the Post.Message heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactPostMessages(void)
{
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saPost) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saPost) + sizeof(uint32) + elementSize - 1)/elementSize;
    char *toPtr = saPosts.Message;
    char *fromPtr = toPtr;
    saPost Post;
    uint32 size;

    while(fromPtr < saPosts.Message + saUsedPostMessage()) {
        Post = *(saPost *)(void *)fromPtr;
        if(Post != saPostNull) {
            /* Need to move it to toPtr */
            size = utMax(saPostGetNumMessage(Post) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saPostSetMessageIndex_(Post, toPtr - saPosts.Message + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saPost *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedPostMessage(toPtr - saPosts.Message);
    saSetFreePostMessage(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the Post.Message heap.
----------------------------------------------------------------------------------------*/
static void allocMorePostMessages(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedPostMessage() - saUsedPostMessage();

    if((saFreePostMessage() << 2) > saUsedPostMessage()) {
        saCompactPostMessages();
        freeSpace = saAllocatedPostMessage() - saUsedPostMessage();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedPostMessage(saAllocatedPostMessage() + spaceNeeded - freeSpace +
            (saAllocatedPostMessage() >> 1));
        utResizeArray(saPosts.Message, saAllocatedPostMessage());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new Post.Message array.
----------------------------------------------------------------------------------------*/
void saPostAllocMessages(
    saPost Post,
    uint32 numMessages)
{
    uint32 freeSpace = saAllocatedPostMessage() - saUsedPostMessage();
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saPost) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saPost) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numMessages + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saPostGetNumMessage(Post) == 0);
#endif
    if(numMessages == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMorePostMessages(spaceNeeded);
    }
    saPostSetMessageIndex_(Post, saUsedPostMessage() + usedHeaderSize);
    saPostSetNumMessage(Post, numMessages);
    *(saPost *)(void *)(saPosts.Message + saUsedPostMessage()) = Post;
    memset(saPosts.Message + saPostGetMessageIndex_(Post), 0, ((numMessages))*sizeof(char));
    saSetUsedPostMessage(saUsedPostMessage() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saPostGetMessages for the database manager.
----------------------------------------------------------------------------------------*/
static void *getPostMessages(
    uint64 objectNumber,
    uint32 *numValues)
{
    saPost Post = saIndex2Post((uint32)objectNumber);

    *numValues = saPostGetNumMessage(Post);
    return saPostGetMessages(Post);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saPostAllocMessages for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocPostMessages(
    uint64 objectNumber,
    uint32 numValues)
{
    saPost Post = saIndex2Post((uint32)objectNumber);

    saPostSetMessageIndex_(Post, 0);
    saPostSetNumMessage(Post, 0);
    if(numValues == 0) {
        return NULL;
    }
    saPostAllocMessages(Post, numValues);
    return saPostGetMessages(Post);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the Post.Message array.
----------------------------------------------------------------------------------------*/
void saPostFreeMessages(
    saPost Post)
{
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saPost) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saPost) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saPostGetNumMessage(Post) + usedHeaderSize, freeHeaderSize);
    char *dataPtr = saPostGetMessages(Post) - usedHeaderSize;

    if(saPostGetNumMessage(Post) == 0) {
        return;
    }
    *(saPost *)(void *)(dataPtr) = saPostNull;
    *(uint32 *)(void *)(((saPost *)(void *)dataPtr) + 1) = size;
    saPostSetNumMessage(Post, 0);
    saSetFreePostMessage(saFreePostMessage() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the Post.Message array.
----------------------------------------------------------------------------------------*/
void saPostResizeMessages(
    saPost Post,
    uint32 numMessages)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saPost) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saPost) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numMessages + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saPostGetNumMessage(Post) + usedHeaderSize, freeHeaderSize);
    char *dataPtr;

    if(numMessages == 0) {
        if(saPostGetNumMessage(Post) != 0) {
            saPostFreeMessages(Post);
        }
        return;
    }
    if(saPostGetNumMessage(Post) == 0) {
        saPostAllocMessages(Post, numMessages);
        return;
    }
    freeSpace = saAllocatedPostMessage() - saUsedPostMessage();
    if(freeSpace < newSize) {
        allocMorePostMessages(newSize);
    }
    dataPtr = saPostGetMessages(Post) - usedHeaderSize;
    memcpy((void *)(saPosts.Message + saUsedPostMessage()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        memset(saPosts.Message + saUsedPostMessage() + oldSize, 0, ((newSize - oldSize))*sizeof(char));
    }
    *(saPost *)(void *)dataPtr = saPostNull;
    *(uint32 *)(void *)(((saPost *)(void *)dataPtr) + 1) = oldSize;
    saSetFreePostMessage(saFreePostMessage() + oldSize);
    saPostSetMessageIndex_(Post, saUsedPostMessage() + usedHeaderSize);
    saPostSetNumMessage(Post, numMessages);
    saSetUsedPostMessage(saUsedPostMessage() + newSize);
}

/*----------------------------------------------------------------------------------------
  Copy the properties of Post.
----------------------------------------------------------------------------------------*/
void saPostCopyProps(
    saPost oldPost,
    saPost newPost)
{
    saPostSetDate(newPost, saPostGetDate(oldPost));
    saPostSetID(newPost, saPostGetID(oldPost));
}

#if defined(DD_DEBUG)
/*----------------------------------------------------------------------------------------
  Write out all the fields of an object.
----------------------------------------------------------------------------------------*/
void saShowPost(
    saPost Post)
{
    utDatabaseShowObject("sa", "Post", saPost2Index(Post));
}
#endif

/*----------------------------------------------------------------------------------------
  Destroy Subscription including everything in it. Remove from parents.
----------------------------------------------------------------------------------------*/
void saSubscriptionDestroy(
    saSubscription Subscription)
{
    saThread owningThread = saSubscriptionGetThread(Subscription);
    saUser owningUser = saSubscriptionGetUser(Subscription);

    if(saSubscriptionDestructorCallback != NULL) {
        saSubscriptionDestructorCallback(Subscription);
    }
    if(owningThread != saThreadNull) {
        saThreadRemoveSubscription(owningThread, Subscription);
#if defined(DD_DEBUG)
    } else {
        utExit("Subscription without owning Thread");
#endif
    }
    if(owningUser != saUserNull) {
        saUserRemoveSubscription(owningUser, Subscription);
#if defined(DD_DEBUG)
    } else {
        utExit("Subscription without owning User");
#endif
    }
    saSubscriptionFree(Subscription);
}

/*----------------------------------------------------------------------------------------
  Default constructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static uint64 allocSubscription(void)
{
    saSubscription Subscription = saSubscriptionAlloc();

    return saSubscription2Index(Subscription);
}

/*----------------------------------------------------------------------------------------
  Destructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static void destroySubscription(
    uint64 objectIndex)
{
    saSubscriptionDestroy(saIndex2Subscription((uint32)objectIndex));
}

/*----------------------------------------------------------------------------------------
  Allocate the field arrays of Subscription.
----------------------------------------------------------------------------------------*/
static void allocSubscriptions(void)
{
    saSetAllocatedSubscription(2);
    saSetUsedSubscription(1);
    saSetFirstFreeSubscription(saSubscriptionNull);
    saSubscriptions.User = utNewAInitFirst(saUser, (saAllocatedSubscription()));
    saSubscriptions.NextUserSubscription = utNewAInitFirst(saSubscription, (saAllocatedSubscription()));
    saSubscriptions.PrevUserSubscription = utNewAInitFirst(saSubscription, (saAllocatedSubscription()));
    saSubscriptions.Thread = utNewAInitFirst(saThread, (saAllocatedSubscription()));
    saSubscriptions.NextThreadSubscription = utNewAInitFirst(saSubscription, (saAllocatedSubscription()));
    saSubscriptions.PrevThreadSubscription = utNewAInitFirst(saSubscription, (saAllocatedSubscription()));
}

/*----------------------------------------------------------------------------------------
  Realloc the arrays of properties for class Subscription.
----------------------------------------------------------------------------------------*/
static void reallocSubscriptions(
    uint32 newSize)
{
    utResizeArray(saSubscriptions.User, (newSize));
    utResizeArray(saSubscriptions.NextUserSubscription, (newSize));
    utResizeArray(saSubscriptions.PrevUserSubscription, (newSize));
    utResizeArray(saSubscriptions.Thread, (newSize));
    utResizeArray(saSubscriptions.NextThreadSubscription, (newSize));
    utResizeArray(saSubscriptions.PrevThreadSubscription, (newSize));
    saSetAllocatedSubscription(newSize);
}

/*----------------------------------------------------------------------------------------
  Allocate more Subscriptions.
----------------------------------------------------------------------------------------*/
void saSubscriptionAllocMore(void)
{
    reallocSubscriptions((uint32)(saAllocatedSubscription() + (saAllocatedSubscription() >> 1)));
}

/*----------------------------------------------------------------------------------------
  Copy the properties of Subscription.
----------------------------------------------------------------------------------------*/
void saSubscriptionCopyProps(
    saSubscription oldSubscription,
    saSubscription newSubscription)
{
}

#if defined(DD_DEBUG)
/*----------------------------------------------------------------------------------------
  Write out all the fields of an object.
----------------------------------------------------------------------------------------*/
void saShowSubscription(
    saSubscription Subscription)
{
    utDatabaseShowObject("sa", "Subscription", saSubscription2Index(Subscription));
}
#endif

/*----------------------------------------------------------------------------------------
  Destroy Announcement including everything in it. Remove from parents.
----------------------------------------------------------------------------------------*/
void saAnnouncementDestroy(
    saAnnouncement Announcement)
{
    saUser owningUser = saAnnouncementGetUser(Announcement);
    saRoot owningRoot = saAnnouncementGetRoot(Announcement);

    if(saAnnouncementDestructorCallback != NULL) {
        saAnnouncementDestructorCallback(Announcement);
    }
    if(owningRoot != saRootNull) {
        saRootRemoveAnnouncement(owningRoot, Announcement);
#if defined(DD_DEBUG)
    } else {
        utExit("Announcement without owning Root");
#endif
    }
    if(owningUser != saUserNull) {
        saUserRemoveAnnouncement(owningUser, Announcement);
#if defined(DD_DEBUG)
    } else {
        utExit("Announcement without owning User");
#endif
    }
    saAnnouncementFree(Announcement);
}

/*----------------------------------------------------------------------------------------
  Default constructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static uint64 allocAnnouncement(void)
{
    saAnnouncement Announcement = saAnnouncementAlloc();

    return saAnnouncement2Index(Announcement);
}

/*----------------------------------------------------------------------------------------
  Destructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static void destroyAnnouncement(
    uint64 objectIndex)
{
    saAnnouncementDestroy(saIndex2Announcement((uint32)objectIndex));
}

/*----------------------------------------------------------------------------------------
  Allocate the field arrays of Announcement.
----------------------------------------------------------------------------------------*/
static void allocAnnouncements(void)
{
    saSetAllocatedAnnouncement(2);
    saSetUsedAnnouncement(1);
    saSetFirstFreeAnnouncement(saAnnouncementNull);
    saAnnouncements.Date = utNewAInitFirst(uint64, (saAllocatedAnnouncement()));
    saAnnouncements.SubjectIndex_ = utNewAInitFirst(uint32, (saAllocatedAnnouncement()));
    saAnnouncements.NumSubject = utNewAInitFirst(uint32, (saAllocatedAnnouncement()));
    saSetUsedAnnouncementSubject(0);
    saSetAllocatedAnnouncementSubject(2);
    saSetFreeAnnouncementSubject(0);
    saAnnouncements.Subject = utNewAInitFirst(char, saAllocatedAnnouncementSubject());
    saAnnouncements.MessageIndex_ = utNewAInitFirst(uint32, (saAllocatedAnnouncement()));
    saAnnouncements.NumMessage = utNewAInitFirst(uint32, (saAllocatedAnnouncement()));
    saSetUsedAnnouncementMessage(0);
    saSetAllocatedAnnouncementMessage(2);
    saSetFreeAnnouncementMessage(0);
    saAnnouncements.Message = utNewAInitFirst(char, saAllocatedAnnouncementMessage());
    saAnnouncements.ID = utNewAInitFirst(uint32, (saAllocatedAnnouncement()));
    saAnnouncements.Root = utNewAInitFirst(saRoot, (saAllocatedAnnouncement()));
    saAnnouncements.NextRootAnnouncement = utNewAInitFirst(saAnnouncement, (saAllocatedAnnouncement()));
    saAnnouncements.PrevRootAnnouncement = utNewAInitFirst(saAnnouncement, (saAllocatedAnnouncement()));
    saAnnouncements.NextTableRootAnnouncement = utNewAInitFirst(saAnnouncement, (saAllocatedAnnouncement()));
    saAnnouncements.User = utNewAInitFirst(saUser, (saAllocatedAnnouncement()));
    saAnnouncements.NextUserAnnouncement = utNewAInitFirst(saAnnouncement, (saAllocatedAnnouncement()));
    saAnnouncements.PrevUserAnnouncement = utNewAInitFirst(saAnnouncement, (saAllocatedAnnouncement()));
}

/*----------------------------------------------------------------------------------------
  Realloc the arrays of properties for class Announcement.
----------------------------------------------------------------------------------------*/
static void reallocAnnouncements(
    uint32 newSize)
{
    utResizeArray(saAnnouncements.Date, (newSize));
    utResizeArray(saAnnouncements.SubjectIndex_, (newSize));
    utResizeArray(saAnnouncements.NumSubject, (newSize));
    utResizeArray(saAnnouncements.MessageIndex_, (newSize));
    utResizeArray(saAnnouncements.NumMessage, (newSize));
    utResizeArray(saAnnouncements.ID, (newSize));
    utResizeArray(saAnnouncements.Root, (newSize));
    utResizeArray(saAnnouncements.NextRootAnnouncement, (newSize));
    utResizeArray(saAnnouncements.PrevRootAnnouncement, (newSize));
    utResizeArray(saAnnouncements.NextTableRootAnnouncement, (newSize));
    utResizeArray(saAnnouncements.User, (newSize));
    utResizeArray(saAnnouncements.NextUserAnnouncement, (newSize));
    utResizeArray(saAnnouncements.PrevUserAnnouncement, (newSize));
    saSetAllocatedAnnouncement(newSize);
}

/*----------------------------------------------------------------------------------------
  Allocate more Announcements.
----------------------------------------------------------------------------------------*/
void saAnnouncementAllocMore(void)
{
    reallocAnnouncements((uint32)(saAllocatedAnnouncement() + (saAllocatedAnnouncement() >> 1)));
}

/*----------------------------------------------------------------------------------------
  Compact the Announcement.Subject heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactAnnouncementSubjects(void)
{
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saAnnouncement) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saAnnouncement) + sizeof(uint32) + elementSize - 1)/elementSize;
    char *toPtr = saAnnouncements.Subject;
    char *fromPtr = toPtr;
    saAnnouncement Announcement;
    uint32 size;

    while(fromPtr < saAnnouncements.Subject + saUsedAnnouncementSubject()) {
        Announcement = *(saAnnouncement *)(void *)fromPtr;
        if(Announcement != saAnnouncementNull) {
            /* Need to move it to toPtr */
            size = utMax(saAnnouncementGetNumSubject(Announcement) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saAnnouncementSetSubjectIndex_(Announcement, toPtr - saAnnouncements.Subject + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saAnnouncement *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedAnnouncementSubject(toPtr - saAnnouncements.Subject);
    saSetFreeAnnouncementSubject(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the Announcement.Subject heap.
----------------------------------------------------------------------------------------*/
static void allocMoreAnnouncementSubjects(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedAnnouncementSubject() - saUsedAnnouncementSubject();

    if((saFreeAnnouncementSubject() << 2) > saUsedAnnouncementSubject()) {
        saCompactAnnouncementSubjects();
        freeSpace = saAllocatedAnnouncementSubject() - saUsedAnnouncementSubject();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedAnnouncementSubject(saAllocatedAnnouncementSubject() + spaceNeeded - freeSpace +
            (saAllocatedAnnouncementSubject() >> 1));
        utResizeArray(saAnnouncements.Subject, saAllocatedAnnouncementSubject());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new Announcement.Subject array.
----------------------------------------------------------------------------------------*/
void saAnnouncementAllocSubjects(
    saAnnouncement Announcement,
    uint32 numSubjects)
{
    uint32 freeSpace = saAllocatedAnnouncementSubject() - saUsedAnnouncementSubject();
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saAnnouncement) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saAnnouncement) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numSubjects + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saAnnouncementGetNumSubject(Announcement) == 0);
#endif
    if(numSubjects == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMoreAnnouncementSubjects(spaceNeeded);
    }
    saAnnouncementSetSubjectIndex_(Announcement, saUsedAnnouncementSubject() + usedHeaderSize);
    saAnnouncementSetNumSubject(Announcement, numSubjects);
    *(saAnnouncement *)(void *)(saAnnouncements.Subject + saUsedAnnouncementSubject()) = Announcement;
    memset(saAnnouncements.Subject + saAnnouncementGetSubjectIndex_(Announcement), 0, ((numSubjects))*sizeof(char));
    saSetUsedAnnouncementSubject(saUsedAnnouncementSubject() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saAnnouncementGetSubjects for the database manager.
----------------------------------------------------------------------------------------*/
static void *getAnnouncementSubjects(
    uint64 objectNumber,
    uint32 *numValues)
{
    saAnnouncement Announcement = saIndex2Announcement((uint32)objectNumber);

    *numValues = saAnnouncementGetNumSubject(Announcement);
    return saAnnouncementGetSubjects(Announcement);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saAnnouncementAllocSubjects for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocAnnouncementSubjects(
    uint64 objectNumber,
    uint32 numValues)
{
    saAnnouncement Announcement = saIndex2Announcement((uint32)objectNumber);

    saAnnouncementSetSubjectIndex_(Announcement, 0);
    saAnnouncementSetNumSubject(Announcement, 0);
    if(numValues == 0) {
        return NULL;
    }
    saAnnouncementAllocSubjects(Announcement, numValues);
    return saAnnouncementGetSubjects(Announcement);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the Announcement.Subject array.
----------------------------------------------------------------------------------------*/
void saAnnouncementFreeSubjects(
    saAnnouncement Announcement)
{
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saAnnouncement) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saAnnouncement) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saAnnouncementGetNumSubject(Announcement) + usedHeaderSize, freeHeaderSize);
    char *dataPtr = saAnnouncementGetSubjects(Announcement) - usedHeaderSize;

    if(saAnnouncementGetNumSubject(Announcement) == 0) {
        return;
    }
    *(saAnnouncement *)(void *)(dataPtr) = saAnnouncementNull;
    *(uint32 *)(void *)(((saAnnouncement *)(void *)dataPtr) + 1) = size;
    saAnnouncementSetNumSubject(Announcement, 0);
    saSetFreeAnnouncementSubject(saFreeAnnouncementSubject() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the Announcement.Subject array.
----------------------------------------------------------------------------------------*/
void saAnnouncementResizeSubjects(
    saAnnouncement Announcement,
    uint32 numSubjects)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saAnnouncement) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saAnnouncement) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numSubjects + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saAnnouncementGetNumSubject(Announcement) + usedHeaderSize, freeHeaderSize);
    char *dataPtr;

    if(numSubjects == 0) {
        if(saAnnouncementGetNumSubject(Announcement) != 0) {
            saAnnouncementFreeSubjects(Announcement);
        }
        return;
    }
    if(saAnnouncementGetNumSubject(Announcement) == 0) {
        saAnnouncementAllocSubjects(Announcement, numSubjects);
        return;
    }
    freeSpace = saAllocatedAnnouncementSubject() - saUsedAnnouncementSubject();
    if(freeSpace < newSize) {
        allocMoreAnnouncementSubjects(newSize);
    }
    dataPtr = saAnnouncementGetSubjects(Announcement) - usedHeaderSize;
    memcpy((void *)(saAnnouncements.Subject + saUsedAnnouncementSubject()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        memset(saAnnouncements.Subject + saUsedAnnouncementSubject() + oldSize, 0, ((newSize - oldSize))*sizeof(char));
    }
    *(saAnnouncement *)(void *)dataPtr = saAnnouncementNull;
    *(uint32 *)(void *)(((saAnnouncement *)(void *)dataPtr) + 1) = oldSize;
    saSetFreeAnnouncementSubject(saFreeAnnouncementSubject() + oldSize);
    saAnnouncementSetSubjectIndex_(Announcement, saUsedAnnouncementSubject() + usedHeaderSize);
    saAnnouncementSetNumSubject(Announcement, numSubjects);
    saSetUsedAnnouncementSubject(saUsedAnnouncementSubject() + newSize);
}

/*----------------------------------------------------------------------------------------
  Compact the Announcement.Message heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactAnnouncementMessages(void)
{
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saAnnouncement) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saAnnouncement) + sizeof(uint32) + elementSize - 1)/elementSize;
    char *toPtr = saAnnouncements.Message;
    char *fromPtr = toPtr;
    saAnnouncement Announcement;
    uint32 size;

    while(fromPtr < saAnnouncements.Message + saUsedAnnouncementMessage()) {
        Announcement = *(saAnnouncement *)(void *)fromPtr;
        if(Announcement != saAnnouncementNull) {
            /* Need to move it to toPtr */
            size = utMax(saAnnouncementGetNumMessage(Announcement) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saAnnouncementSetMessageIndex_(Announcement, toPtr - saAnnouncements.Message + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saAnnouncement *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedAnnouncementMessage(toPtr - saAnnouncements.Message);
    saSetFreeAnnouncementMessage(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the Announcement.Message heap.
----------------------------------------------------------------------------------------*/
static void allocMoreAnnouncementMessages(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedAnnouncementMessage() - saUsedAnnouncementMessage();

    if((saFreeAnnouncementMessage() << 2) > saUsedAnnouncementMessage()) {
        saCompactAnnouncementMessages();
        freeSpace = saAllocatedAnnouncementMessage() - saUsedAnnouncementMessage();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedAnnouncementMessage(saAllocatedAnnouncementMessage() + spaceNeeded - freeSpace +
            (saAllocatedAnnouncementMessage() >> 1));
        utResizeArray(saAnnouncements.Message, saAllocatedAnnouncementMessage());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new Announcement.Message array.
----------------------------------------------------------------------------------------*/
void saAnnouncementAllocMessages(
    saAnnouncement Announcement,
    uint32 numMessages)
{
    uint32 freeSpace = saAllocatedAnnouncementMessage() - saUsedAnnouncementMessage();
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saAnnouncement) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saAnnouncement) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numMessages + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saAnnouncementGetNumMessage(Announcement) == 0);
#endif
    if(numMessages == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMoreAnnouncementMessages(spaceNeeded);
    }
    saAnnouncementSetMessageIndex_(Announcement, saUsedAnnouncementMessage() + usedHeaderSize);
    saAnnouncementSetNumMessage(Announcement, numMessages);
    *(saAnnouncement *)(void *)(saAnnouncements.Message + saUsedAnnouncementMessage()) = Announcement;
    memset(saAnnouncements.Message + saAnnouncementGetMessageIndex_(Announcement), 0, ((numMessages))*sizeof(char));
    saSetUsedAnnouncementMessage(saUsedAnnouncementMessage() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saAnnouncementGetMessages for the database manager.
----------------------------------------------------------------------------------------*/
static void *getAnnouncementMessages(
    uint64 objectNumber,
    uint32 *numValues)
{
    saAnnouncement Announcement = saIndex2Announcement((uint32)objectNumber);

    *numValues = saAnnouncementGetNumMessage(Announcement);
    return saAnnouncementGetMessages(Announcement);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saAnnouncementAllocMessages for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocAnnouncementMessages(
    uint64 objectNumber,
    uint32 numValues)
{
    saAnnouncement Announcement = saIndex2Announcement((uint32)objectNumber);

    saAnnouncementSetMessageIndex_(Announcement, 0);
    saAnnouncementSetNumMessage(Announcement, 0);
    if(numValues == 0) {
        return NULL;
    }
    saAnnouncementAllocMessages(Announcement, numValues);
    return saAnnouncementGetMessages(Announcement);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the Announcement.Message array.
----------------------------------------------------------------------------------------*/
void saAnnouncementFreeMessages(
    saAnnouncement Announcement)
{
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saAnnouncement) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saAnnouncement) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saAnnouncementGetNumMessage(Announcement) + usedHeaderSize, freeHeaderSize);
    char *dataPtr = saAnnouncementGetMessages(Announcement) - usedHeaderSize;

    if(saAnnouncementGetNumMessage(Announcement) == 0) {
        return;
    }
    *(saAnnouncement *)(void *)(dataPtr) = saAnnouncementNull;
    *(uint32 *)(void *)(((saAnnouncement *)(void *)dataPtr) + 1) = size;
    saAnnouncementSetNumMessage(Announcement, 0);
    saSetFreeAnnouncementMessage(saFreeAnnouncementMessage() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the Announcement.Message array.
----------------------------------------------------------------------------------------*/
void saAnnouncementResizeMessages(
    saAnnouncement Announcement,
    uint32 numMessages)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(char);
    uint32 usedHeaderSize = (sizeof(saAnnouncement) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saAnnouncement) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numMessages + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saAnnouncementGetNumMessage(Announcement) + usedHeaderSize, freeHeaderSize);
    char *dataPtr;

    if(numMessages == 0) {
        if(saAnnouncementGetNumMessage(Announcement) != 0) {
            saAnnouncementFreeMessages(Announcement);
        }
        return;
    }
    if(saAnnouncementGetNumMessage(Announcement) == 0) {
        saAnnouncementAllocMessages(Announcement, numMessages);
        return;
    }
    freeSpace = saAllocatedAnnouncementMessage() - saUsedAnnouncementMessage();
    if(freeSpace < newSize) {
        allocMoreAnnouncementMessages(newSize);
    }
    dataPtr = saAnnouncementGetMessages(Announcement) - usedHeaderSize;
    memcpy((void *)(saAnnouncements.Message + saUsedAnnouncementMessage()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        memset(saAnnouncements.Message + saUsedAnnouncementMessage() + oldSize, 0, ((newSize - oldSize))*sizeof(char));
    }
    *(saAnnouncement *)(void *)dataPtr = saAnnouncementNull;
    *(uint32 *)(void *)(((saAnnouncement *)(void *)dataPtr) + 1) = oldSize;
    saSetFreeAnnouncementMessage(saFreeAnnouncementMessage() + oldSize);
    saAnnouncementSetMessageIndex_(Announcement, saUsedAnnouncementMessage() + usedHeaderSize);
    saAnnouncementSetNumMessage(Announcement, numMessages);
    saSetUsedAnnouncementMessage(saUsedAnnouncementMessage() + newSize);
}

/*----------------------------------------------------------------------------------------
  Copy the properties of Announcement.
----------------------------------------------------------------------------------------*/
void saAnnouncementCopyProps(
    saAnnouncement oldAnnouncement,
    saAnnouncement newAnnouncement)
{
    saAnnouncementSetDate(newAnnouncement, saAnnouncementGetDate(oldAnnouncement));
    saAnnouncementSetID(newAnnouncement, saAnnouncementGetID(oldAnnouncement));
}

#if defined(DD_DEBUG)
/*----------------------------------------------------------------------------------------
  Write out all the fields of an object.
----------------------------------------------------------------------------------------*/
void saShowAnnouncement(
    saAnnouncement Announcement)
{
    utDatabaseShowObject("sa", "Announcement", saAnnouncement2Index(Announcement));
}
#endif

/*----------------------------------------------------------------------------------------
  Destroy TransactionArray including everything in it. Remove from parents.
----------------------------------------------------------------------------------------*/
void saTransactionArrayDestroy(
    saTransactionArray TransactionArray)
{
    if(saTransactionArrayDestructorCallback != NULL) {
        saTransactionArrayDestructorCallback(TransactionArray);
    }
    saTransactionArrayFree(TransactionArray);
}

/*----------------------------------------------------------------------------------------
  Default constructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static uint64 allocTransactionArray(void)
{
    saTransactionArray TransactionArray = saTransactionArrayAlloc();

    return saTransactionArray2Index(TransactionArray);
}

/*----------------------------------------------------------------------------------------
  Destructor wrapper for the database manager.
----------------------------------------------------------------------------------------*/
static void destroyTransactionArray(
    uint64 objectIndex)
{
    saTransactionArrayDestroy(saIndex2TransactionArray((uint32)objectIndex));
}

/*----------------------------------------------------------------------------------------
  Allocate the field arrays of TransactionArray.
----------------------------------------------------------------------------------------*/
static void allocTransactionArrays(void)
{
    saSetAllocatedTransactionArray(2);
    saSetUsedTransactionArray(1);
    saSetFirstFreeTransactionArray(saTransactionArrayNull);
    saTransactionArrays.TransactionIndex_ = utNewAInitFirst(uint32, (saAllocatedTransactionArray()));
    saTransactionArrays.NumTransaction = utNewAInitFirst(uint32, (saAllocatedTransactionArray()));
    saSetUsedTransactionArrayTransaction(0);
    saSetAllocatedTransactionArrayTransaction(2);
    saSetFreeTransactionArrayTransaction(0);
    saTransactionArrays.Transaction = utNewAInitFirst(saTransaction, saAllocatedTransactionArrayTransaction());
    saTransactionArrays.UsedTransaction = utNewAInitFirst(uint32, (saAllocatedTransactionArray()));
    saTransactionArrays.FreeList = utNewAInitFirst(saTransactionArray, (saAllocatedTransactionArray()));
}

/*----------------------------------------------------------------------------------------
  Realloc the arrays of properties for class TransactionArray.
----------------------------------------------------------------------------------------*/
static void reallocTransactionArrays(
    uint32 newSize)
{
    utResizeArray(saTransactionArrays.TransactionIndex_, (newSize));
    utResizeArray(saTransactionArrays.NumTransaction, (newSize));
    utResizeArray(saTransactionArrays.UsedTransaction, (newSize));
    utResizeArray(saTransactionArrays.FreeList, (newSize));
    saSetAllocatedTransactionArray(newSize);
}

/*----------------------------------------------------------------------------------------
  Allocate more TransactionArrays.
----------------------------------------------------------------------------------------*/
void saTransactionArrayAllocMore(void)
{
    reallocTransactionArrays((uint32)(saAllocatedTransactionArray() + (saAllocatedTransactionArray() >> 1)));
}

/*----------------------------------------------------------------------------------------
  Compact the TransactionArray.Transaction heap to free memory.
----------------------------------------------------------------------------------------*/
void saCompactTransactionArrayTransactions(void)
{
    uint32 elementSize = sizeof(saTransaction);
    uint32 usedHeaderSize = (sizeof(saTransactionArray) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saTransactionArray) + sizeof(uint32) + elementSize - 1)/elementSize;
    saTransaction *toPtr = saTransactionArrays.Transaction;
    saTransaction *fromPtr = toPtr;
    saTransactionArray TransactionArray;
    uint32 size;

    while(fromPtr < saTransactionArrays.Transaction + saUsedTransactionArrayTransaction()) {
        TransactionArray = *(saTransactionArray *)(void *)fromPtr;
        if(TransactionArray != saTransactionArrayNull) {
            /* Need to move it to toPtr */
            size = utMax(saTransactionArrayGetNumTransaction(TransactionArray) + usedHeaderSize, freeHeaderSize);
            memmove((void *)toPtr, (void *)fromPtr, size*elementSize);
            saTransactionArraySetTransactionIndex_(TransactionArray, toPtr - saTransactionArrays.Transaction + usedHeaderSize);
            toPtr += size;
        } else {
            /* Just skip it */
            size = *(uint32 *)(void *)(((saTransactionArray *)(void *)fromPtr) + 1);
        }
        fromPtr += size;
    }
    saSetUsedTransactionArrayTransaction(toPtr - saTransactionArrays.Transaction);
    saSetFreeTransactionArrayTransaction(0);
}

/*----------------------------------------------------------------------------------------
  Allocate more memory for the TransactionArray.Transaction heap.
----------------------------------------------------------------------------------------*/
static void allocMoreTransactionArrayTransactions(
    uint32 spaceNeeded)
{
    uint32 freeSpace = saAllocatedTransactionArrayTransaction() - saUsedTransactionArrayTransaction();

    if((saFreeTransactionArrayTransaction() << 2) > saUsedTransactionArrayTransaction()) {
        saCompactTransactionArrayTransactions();
        freeSpace = saAllocatedTransactionArrayTransaction() - saUsedTransactionArrayTransaction();
    }
    if(freeSpace < spaceNeeded) {
        saSetAllocatedTransactionArrayTransaction(saAllocatedTransactionArrayTransaction() + spaceNeeded - freeSpace +
            (saAllocatedTransactionArrayTransaction() >> 1));
        utResizeArray(saTransactionArrays.Transaction, saAllocatedTransactionArrayTransaction());
    }
}

/*----------------------------------------------------------------------------------------
  Allocate memory for a new TransactionArray.Transaction array.
----------------------------------------------------------------------------------------*/
void saTransactionArrayAllocTransactions(
    saTransactionArray TransactionArray,
    uint32 numTransactions)
{
    uint32 freeSpace = saAllocatedTransactionArrayTransaction() - saUsedTransactionArrayTransaction();
    uint32 elementSize = sizeof(saTransaction);
    uint32 usedHeaderSize = (sizeof(saTransactionArray) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saTransactionArray) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 spaceNeeded = utMax(numTransactions + usedHeaderSize, freeHeaderSize);

#if defined(DD_DEBUG)
    utAssert(saTransactionArrayGetNumTransaction(TransactionArray) == 0);
#endif
    if(numTransactions == 0) {
        return;
    }
    if(freeSpace < spaceNeeded) {
        allocMoreTransactionArrayTransactions(spaceNeeded);
    }
    saTransactionArraySetTransactionIndex_(TransactionArray, saUsedTransactionArrayTransaction() + usedHeaderSize);
    saTransactionArraySetNumTransaction(TransactionArray, numTransactions);
    *(saTransactionArray *)(void *)(saTransactionArrays.Transaction + saUsedTransactionArrayTransaction()) = TransactionArray;
    {
        uint32 xValue;
        for(xValue = (uint32)(saTransactionArrayGetTransactionIndex_(TransactionArray)); xValue < saTransactionArrayGetTransactionIndex_(TransactionArray) + numTransactions; xValue++) {
            saTransactionArrays.Transaction[xValue] = saTransactionNull;
        }
    }
    saSetUsedTransactionArrayTransaction(saUsedTransactionArrayTransaction() + spaceNeeded);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saTransactionArrayGetTransactions for the database manager.
----------------------------------------------------------------------------------------*/
static void *getTransactionArrayTransactions(
    uint64 objectNumber,
    uint32 *numValues)
{
    saTransactionArray TransactionArray = saIndex2TransactionArray((uint32)objectNumber);

    *numValues = saTransactionArrayGetNumTransaction(TransactionArray);
    return saTransactionArrayGetTransactions(TransactionArray);
}

/*----------------------------------------------------------------------------------------
  Wrapper around saTransactionArrayAllocTransactions for the database manager.
----------------------------------------------------------------------------------------*/
static void *allocTransactionArrayTransactions(
    uint64 objectNumber,
    uint32 numValues)
{
    saTransactionArray TransactionArray = saIndex2TransactionArray((uint32)objectNumber);

    saTransactionArraySetTransactionIndex_(TransactionArray, 0);
    saTransactionArraySetNumTransaction(TransactionArray, 0);
    if(numValues == 0) {
        return NULL;
    }
    saTransactionArrayAllocTransactions(TransactionArray, numValues);
    return saTransactionArrayGetTransactions(TransactionArray);
}

/*----------------------------------------------------------------------------------------
  Free memory used by the TransactionArray.Transaction array.
----------------------------------------------------------------------------------------*/
void saTransactionArrayFreeTransactions(
    saTransactionArray TransactionArray)
{
    uint32 elementSize = sizeof(saTransaction);
    uint32 usedHeaderSize = (sizeof(saTransactionArray) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saTransactionArray) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 size = utMax(saTransactionArrayGetNumTransaction(TransactionArray) + usedHeaderSize, freeHeaderSize);
    saTransaction *dataPtr = saTransactionArrayGetTransactions(TransactionArray) - usedHeaderSize;

    if(saTransactionArrayGetNumTransaction(TransactionArray) == 0) {
        return;
    }
    *(saTransactionArray *)(void *)(dataPtr) = saTransactionArrayNull;
    *(uint32 *)(void *)(((saTransactionArray *)(void *)dataPtr) + 1) = size;
    saTransactionArraySetNumTransaction(TransactionArray, 0);
    saSetFreeTransactionArrayTransaction(saFreeTransactionArrayTransaction() + size);
}

/*----------------------------------------------------------------------------------------
  Resize the TransactionArray.Transaction array.
----------------------------------------------------------------------------------------*/
void saTransactionArrayResizeTransactions(
    saTransactionArray TransactionArray,
    uint32 numTransactions)
{
    uint32 freeSpace;
    uint32 elementSize = sizeof(saTransaction);
    uint32 usedHeaderSize = (sizeof(saTransactionArray) + elementSize - 1)/elementSize;
    uint32 freeHeaderSize = (sizeof(saTransactionArray) + sizeof(uint32) + elementSize - 1)/elementSize;
    uint32 newSize = utMax(numTransactions + usedHeaderSize, freeHeaderSize);
    uint32 oldSize = utMax(saTransactionArrayGetNumTransaction(TransactionArray) + usedHeaderSize, freeHeaderSize);
    saTransaction *dataPtr;

    if(numTransactions == 0) {
        if(saTransactionArrayGetNumTransaction(TransactionArray) != 0) {
            saTransactionArrayFreeTransactions(TransactionArray);
        }
        return;
    }
    if(saTransactionArrayGetNumTransaction(TransactionArray) == 0) {
        saTransactionArrayAllocTransactions(TransactionArray, numTransactions);
        return;
    }
    freeSpace = saAllocatedTransactionArrayTransaction() - saUsedTransactionArrayTransaction();
    if(freeSpace < newSize) {
        allocMoreTransactionArrayTransactions(newSize);
    }
    dataPtr = saTransactionArrayGetTransactions(TransactionArray) - usedHeaderSize;
    memcpy((void *)(saTransactionArrays.Transaction + saUsedTransactionArrayTransaction()), dataPtr,
        elementSize*utMin(oldSize, newSize));
    if(newSize > oldSize) {
        {
            uint32 xValue;
            for(xValue = (uint32)(saUsedTransactionArrayTransaction() + oldSize); xValue < saUsedTransactionArrayTransaction() + oldSize + newSize - oldSize; xValue++) {
                saTransactionArrays.Transaction[xValue] = saTransactionNull;
            }
        }
    }
    *(saTransactionArray *)(void *)dataPtr = saTransactionArrayNull;
    *(uint32 *)(void *)(((saTransactionArray *)(void *)dataPtr) + 1) = oldSize;
    saSetFreeTransactionArrayTransaction(saFreeTransactionArrayTransaction() + oldSize);
    saTransactionArraySetTransactionIndex_(TransactionArray, saUsedTransactionArrayTransaction() + usedHeaderSize);
    saTransactionArraySetNumTransaction(TransactionArray, numTransactions);
    saSetUsedTransactionArrayTransaction(saUsedTransactionArrayTransaction() + newSize);
}

/*----------------------------------------------------------------------------------------
  Copy the properties of TransactionArray.
----------------------------------------------------------------------------------------*/
void saTransactionArrayCopyProps(
    saTransactionArray oldTransactionArray,
    saTransactionArray newTransactionArray)
{
}

/*----------------------------------------------------------------------------------------
  Add the indexed Transaction to the TransactionArray.
----------------------------------------------------------------------------------------*/
void saTransactionArrayInsertTransaction(
    saTransactionArray TransactionArray,
    uint32 x,
    saTransaction _Transaction)
{
#if defined(DD_DEBUG)
    if(TransactionArray == saTransactionArrayNull) {
        utExit("Non existent TransactionArray");
    }
#endif
    saTransactionArraySetiTransaction(TransactionArray, x, _Transaction);
    saTransactionArraySetUsedTransaction(TransactionArray, utMax(saTransactionArrayGetUsedTransaction(TransactionArray), x + 1));
}

/*----------------------------------------------------------------------------------------
  Add the Transaction to the end of the TransactionArrayTransaction array.
----------------------------------------------------------------------------------------*/
void saTransactionArrayAppendTransaction(
    saTransactionArray TransactionArray,
    saTransaction _Transaction)
{
    uint32 usedTransaction = saTransactionArrayGetUsedTransaction(TransactionArray);

#if defined(DD_DEBUG)
    if(TransactionArray == saTransactionArrayNull) {
        utExit("Non existent TransactionArray");
    }
#endif
    if(usedTransaction >= saTransactionArrayGetNumTransaction(TransactionArray)) {
        saTransactionArrayResizeTransactions(TransactionArray, usedTransaction + (usedTransaction << 1) + 1);
    }
    saTransactionArraySetiTransaction(TransactionArray, usedTransaction, _Transaction);
    saTransactionArraySetUsedTransaction(TransactionArray, usedTransaction + 1);
}

#if defined(DD_DEBUG)
/*----------------------------------------------------------------------------------------
  Write out all the fields of an object.
----------------------------------------------------------------------------------------*/
void saShowTransactionArray(
    saTransactionArray TransactionArray)
{
    utDatabaseShowObject("sa", "TransactionArray", saTransactionArray2Index(TransactionArray));
}
#endif

/*----------------------------------------------------------------------------------------
  Free memory used by the sa database.
----------------------------------------------------------------------------------------*/
void saDatabaseStop(void)
{
    utFree(saRoots.NextListingID);
    utFree(saRoots.NextThreadID);
    utFree(saRoots.NextPostID);
    utFree(saRoots.NextAnnouncementID);
    utFree(saRoots.FirstRegion);
    utFree(saRoots.LastRegion);
    utFree(saRoots.RegionTableIndex_);
    utFree(saRoots.NumRegionTable);
    utFree(saRoots.RegionTable);
    utFree(saRoots.NumRegion);
    utFree(saRoots.FirstUser);
    utFree(saRoots.LastUser);
    utFree(saRoots.UserTableIndex_);
    utFree(saRoots.NumUserTable);
    utFree(saRoots.UserTable);
    utFree(saRoots.NumUser);
    utFree(saRoots.FirstByEmailUser);
    utFree(saRoots.LastByEmailUser);
    utFree(saRoots.ByEmailUserTableIndex_);
    utFree(saRoots.NumByEmailUserTable);
    utFree(saRoots.ByEmailUserTable);
    utFree(saRoots.NumByEmailUser);
    utFree(saRoots.FirstCharity);
    utFree(saRoots.LastCharity);
    utFree(saRoots.FirstCategory);
    utFree(saRoots.LastCategory);
    utFree(saRoots.CategoryTableIndex_);
    utFree(saRoots.NumCategoryTable);
    utFree(saRoots.CategoryTable);
    utFree(saRoots.NumCategory);
    utFree(saRoots.FirstListing);
    utFree(saRoots.LastListing);
    utFree(saRoots.ListingTableIndex_);
    utFree(saRoots.NumListingTable);
    utFree(saRoots.ListingTable);
    utFree(saRoots.NumListing);
    utFree(saRoots.FirstSession);
    utFree(saRoots.LastSession);
    utFree(saRoots.SessionTableIndex_);
    utFree(saRoots.NumSessionTable);
    utFree(saRoots.SessionTable);
    utFree(saRoots.NumSession);
    utFree(saRoots.FirstThread);
    utFree(saRoots.LastThread);
    utFree(saRoots.ThreadTableIndex_);
    utFree(saRoots.NumThreadTable);
    utFree(saRoots.ThreadTable);
    utFree(saRoots.NumThread);
    utFree(saRoots.FirstPost);
    utFree(saRoots.LastPost);
    utFree(saRoots.PostTableIndex_);
    utFree(saRoots.NumPostTable);
    utFree(saRoots.PostTable);
    utFree(saRoots.NumPost);
    utFree(saRoots.FirstAnnouncement);
    utFree(saRoots.LastAnnouncement);
    utFree(saRoots.AnnouncementTableIndex_);
    utFree(saRoots.NumAnnouncementTable);
    utFree(saRoots.AnnouncementTable);
    utFree(saRoots.NumAnnouncement);
    utFree(saRegions.Sym);
    utFree(saRegions.NumListings);
    utFree(saRegions.Root);
    utFree(saRegions.NextRootRegion);
    utFree(saRegions.PrevRootRegion);
    utFree(saRegions.NextTableRootRegion);
    utFree(saRegions.FirstUser);
    utFree(saRegions.LastUser);
    utFree(saRegions.Session);
    utFree(saSessions.Sym);
    utFree(saSessions.Root);
    utFree(saSessions.NextRootSession);
    utFree(saSessions.PrevRootSession);
    utFree(saSessions.NextTableRootSession);
    utFree(saSessions.User);
    utFree(saSessions.Region);
    utFree(saUsers.Sym);
    utFree(saUsers.ShownNameIndex_);
    utFree(saUsers.NumShownName);
    utFree(saUsers.ShownName);
    utFree(saUsers.Email);
    utFree(saUsers.JoinDate);
    utFree(saUsers.Active);
    utFree(saUsers.HashedPasswordIndex_);
    utFree(saUsers.NumHashedPassword);
    utFree(saUsers.HashedPassword);
    utFree(saUsers.NounceIndex_);
    utFree(saUsers.NumNounce);
    utFree(saUsers.Nounce);
    utFree(saUsers.LoggedIn);
    utFree(saUsers.LastActivity);
    utFree(saUsers.Balance);
    utFree(saUsers.Validated);
    utFree(saUsers.ValidationKeyIndex_);
    utFree(saUsers.NumValidationKey);
    utFree(saUsers.ValidationKey);
    utFree(saUsers.SupremeLeader);
    utFree(saUsers.RegionManager);
    utFree(saUsers.Root);
    utFree(saUsers.NextRootUser);
    utFree(saUsers.PrevRootUser);
    utFree(saUsers.NextTableRootUser);
    utFree(saUsers.ByEmailRoot);
    utFree(saUsers.NextRootByEmailUser);
    utFree(saUsers.PrevRootByEmailUser);
    utFree(saUsers.NextTableRootByEmailUser);
    utFree(saUsers.Region);
    utFree(saUsers.NextRegionUser);
    utFree(saUsers.PrevRegionUser);
    utFree(saUsers.Session);
    utFree(saUsers.Charity);
    utFree(saUsers.FirstMembership);
    utFree(saUsers.LastMembership);
    utFree(saUsers.MembershipTableIndex_);
    utFree(saUsers.NumMembershipTable);
    utFree(saUsers.MembershipTable);
    utFree(saUsers.NumMembership);
    utFree(saUsers.FirstOutTransaction);
    utFree(saUsers.LastOutTransaction);
    utFree(saUsers.FirstInTransaction);
    utFree(saUsers.LastInTransaction);
    utFree(saUsers.FirstListing);
    utFree(saUsers.LastListing);
    utFree(saUsers.FirstPost);
    utFree(saUsers.LastPost);
    utFree(saUsers.PostTableIndex_);
    utFree(saUsers.NumPostTable);
    utFree(saUsers.PostTable);
    utFree(saUsers.NumPost);
    utFree(saUsers.FirstSubscription);
    utFree(saUsers.LastSubscription);
    utFree(saUsers.FirstAnnouncement);
    utFree(saUsers.LastAnnouncement);
    utFree(saTransactions.Date);
    utFree(saTransactions.Amount);
    utFree(saTransactions.DescriptionIndex_);
    utFree(saTransactions.NumDescription);
    utFree(saTransactions.Description);
    utFree(saTransactions.FromUser);
    utFree(saTransactions.NextUserOutTransaction);
    utFree(saTransactions.PrevUserOutTransaction);
    utFree(saTransactions.ToUser);
    utFree(saTransactions.NextUserInTransaction);
    utFree(saTransactions.PrevUserInTransaction);
    utFree(saCharitys.Root);
    utFree(saCharitys.NextRootCharity);
    utFree(saCharitys.PrevRootCharity);
    utFree(saCharitys.User);
    utFree(saCharitys.FirstMembership);
    utFree(saCharitys.LastMembership);
    utFree(saMemberships.User);
    utFree(saMemberships.NextUserMembership);
    utFree(saMemberships.PrevUserMembership);
    utFree(saMemberships.NextTableUserMembership);
    utFree(saMemberships.Charity);
    utFree(saMemberships.NextCharityMembership);
    utFree(saMemberships.PrevCharityMembership);
    utFree(saCategorys.Sym);
    utFree(saCategorys.NumListings);
    utFree(saCategorys.Root);
    utFree(saCategorys.NextRootCategory);
    utFree(saCategorys.PrevRootCategory);
    utFree(saCategorys.NextTableRootCategory);
    utFree(saCategorys.FirstListing);
    utFree(saCategorys.LastListing);
    utFree(saListings.TitleIndex_);
    utFree(saListings.NumTitle);
    utFree(saListings.Title);
    utFree(saListings.DescriptionIndex_);
    utFree(saListings.NumDescription);
    utFree(saListings.Description);
    utFree(saListings.ID);
    utFree(saListings.Rate);
    utFree(saListings.Offered);
    utFree(saListings.FixedPrice);
    utFree(saListings.Root);
    utFree(saListings.NextRootListing);
    utFree(saListings.PrevRootListing);
    utFree(saListings.NextTableRootListing);
    utFree(saListings.User);
    utFree(saListings.NextUserListing);
    utFree(saListings.PrevUserListing);
    utFree(saListings.Category);
    utFree(saListings.NextCategoryListing);
    utFree(saListings.PrevCategoryListing);
    utFree(saThreads.SubjectIndex_);
    utFree(saThreads.NumSubject);
    utFree(saThreads.Subject);
    utFree(saThreads.ID);
    utFree(saThreads.NumPosts);
    utFree(saThreads.Root);
    utFree(saThreads.NextRootThread);
    utFree(saThreads.PrevRootThread);
    utFree(saThreads.NextTableRootThread);
    utFree(saThreads.FirstPost);
    utFree(saThreads.LastPost);
    utFree(saThreads.FirstSubscription);
    utFree(saThreads.LastSubscription);
    utFree(saPosts.Date);
    utFree(saPosts.MessageIndex_);
    utFree(saPosts.NumMessage);
    utFree(saPosts.Message);
    utFree(saPosts.ID);
    utFree(saPosts.Root);
    utFree(saPosts.NextRootPost);
    utFree(saPosts.PrevRootPost);
    utFree(saPosts.NextTableRootPost);
    utFree(saPosts.User);
    utFree(saPosts.NextUserPost);
    utFree(saPosts.PrevUserPost);
    utFree(saPosts.NextTableUserPost);
    utFree(saPosts.Thread);
    utFree(saPosts.NextThreadPost);
    utFree(saPosts.PrevThreadPost);
    utFree(saSubscriptions.User);
    utFree(saSubscriptions.NextUserSubscription);
    utFree(saSubscriptions.PrevUserSubscription);
    utFree(saSubscriptions.Thread);
    utFree(saSubscriptions.NextThreadSubscription);
    utFree(saSubscriptions.PrevThreadSubscription);
    utFree(saAnnouncements.Date);
    utFree(saAnnouncements.SubjectIndex_);
    utFree(saAnnouncements.NumSubject);
    utFree(saAnnouncements.Subject);
    utFree(saAnnouncements.MessageIndex_);
    utFree(saAnnouncements.NumMessage);
    utFree(saAnnouncements.Message);
    utFree(saAnnouncements.ID);
    utFree(saAnnouncements.Root);
    utFree(saAnnouncements.NextRootAnnouncement);
    utFree(saAnnouncements.PrevRootAnnouncement);
    utFree(saAnnouncements.NextTableRootAnnouncement);
    utFree(saAnnouncements.User);
    utFree(saAnnouncements.NextUserAnnouncement);
    utFree(saAnnouncements.PrevUserAnnouncement);
    utFree(saTransactionArrays.TransactionIndex_);
    utFree(saTransactionArrays.NumTransaction);
    utFree(saTransactionArrays.Transaction);
    utFree(saTransactionArrays.UsedTransaction);
    utFree(saTransactionArrays.FreeList);
    utUnregisterModule(saModuleID);
}

/*----------------------------------------------------------------------------------------
  Allocate memory used by the sa database.
----------------------------------------------------------------------------------------*/
void saDatabaseStart(void)
{
    if(!utInitialized()) {
        utStart();
    }
    saRootData.hash = 0x70a2e135;
    saModuleID = utRegisterModule("sa", false, saHash(), 14, 240, 0, sizeof(struct saRootType_),
        &saRootData, saDatabaseStart, saDatabaseStop);
    utRegisterClass("Root", 60, &saRootData.usedRoot, &saRootData.allocatedRoot,
        &saRootData.firstFreeRoot, 4, 4, allocRoot, destroyRoot);
    utRegisterField("NextListingID", &saRoots.NextListingID, sizeof(uint64), UT_UINT, NULL);
    utRegisterField("NextThreadID", &saRoots.NextThreadID, sizeof(uint64), UT_UINT, NULL);
    utRegisterField("NextPostID", &saRoots.NextPostID, sizeof(uint64), UT_UINT, NULL);
    utRegisterField("NextAnnouncementID", &saRoots.NextAnnouncementID, sizeof(uint64), UT_UINT, NULL);
    utRegisterField("FirstRegion", &saRoots.FirstRegion, sizeof(saRegion), UT_POINTER, "Region");
    utRegisterField("LastRegion", &saRoots.LastRegion, sizeof(saRegion), UT_POINTER, "Region");
    utRegisterField("RegionTableIndex_", &saRoots.RegionTableIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumRegionTable", &saRoots.NumRegionTable, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("RegionTable", &saRoots.RegionTable, sizeof(saRegion), UT_POINTER, "Region");
    utRegisterArray(&saRootData.usedRootRegionTable, &saRootData.allocatedRootRegionTable,
        getRootRegionTables, allocRootRegionTables, saCompactRootRegionTables);
    utRegisterField("NumRegion", &saRoots.NumRegion, sizeof(uint32), UT_UINT, NULL);
    utRegisterField("FirstUser", &saRoots.FirstUser, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("LastUser", &saRoots.LastUser, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("UserTableIndex_", &saRoots.UserTableIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumUserTable", &saRoots.NumUserTable, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("UserTable", &saRoots.UserTable, sizeof(saUser), UT_POINTER, "User");
    utRegisterArray(&saRootData.usedRootUserTable, &saRootData.allocatedRootUserTable,
        getRootUserTables, allocRootUserTables, saCompactRootUserTables);
    utRegisterField("NumUser", &saRoots.NumUser, sizeof(uint32), UT_UINT, NULL);
    utRegisterField("FirstByEmailUser", &saRoots.FirstByEmailUser, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("LastByEmailUser", &saRoots.LastByEmailUser, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("ByEmailUserTableIndex_", &saRoots.ByEmailUserTableIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumByEmailUserTable", &saRoots.NumByEmailUserTable, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("ByEmailUserTable", &saRoots.ByEmailUserTable, sizeof(saUser), UT_POINTER, "User");
    utRegisterArray(&saRootData.usedRootByEmailUserTable, &saRootData.allocatedRootByEmailUserTable,
        getRootByEmailUserTables, allocRootByEmailUserTables, saCompactRootByEmailUserTables);
    utRegisterField("NumByEmailUser", &saRoots.NumByEmailUser, sizeof(uint32), UT_UINT, NULL);
    utRegisterField("FirstCharity", &saRoots.FirstCharity, sizeof(saCharity), UT_POINTER, "Charity");
    utRegisterField("LastCharity", &saRoots.LastCharity, sizeof(saCharity), UT_POINTER, "Charity");
    utRegisterField("FirstCategory", &saRoots.FirstCategory, sizeof(saCategory), UT_POINTER, "Category");
    utRegisterField("LastCategory", &saRoots.LastCategory, sizeof(saCategory), UT_POINTER, "Category");
    utRegisterField("CategoryTableIndex_", &saRoots.CategoryTableIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumCategoryTable", &saRoots.NumCategoryTable, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("CategoryTable", &saRoots.CategoryTable, sizeof(saCategory), UT_POINTER, "Category");
    utRegisterArray(&saRootData.usedRootCategoryTable, &saRootData.allocatedRootCategoryTable,
        getRootCategoryTables, allocRootCategoryTables, saCompactRootCategoryTables);
    utRegisterField("NumCategory", &saRoots.NumCategory, sizeof(uint32), UT_UINT, NULL);
    utRegisterField("FirstListing", &saRoots.FirstListing, sizeof(saListing), UT_POINTER, "Listing");
    utRegisterField("LastListing", &saRoots.LastListing, sizeof(saListing), UT_POINTER, "Listing");
    utRegisterField("ListingTableIndex_", &saRoots.ListingTableIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumListingTable", &saRoots.NumListingTable, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("ListingTable", &saRoots.ListingTable, sizeof(saListing), UT_POINTER, "Listing");
    utRegisterArray(&saRootData.usedRootListingTable, &saRootData.allocatedRootListingTable,
        getRootListingTables, allocRootListingTables, saCompactRootListingTables);
    utRegisterField("NumListing", &saRoots.NumListing, sizeof(uint32), UT_UINT, NULL);
    utRegisterField("FirstSession", &saRoots.FirstSession, sizeof(saSession), UT_POINTER, "Session");
    utRegisterField("LastSession", &saRoots.LastSession, sizeof(saSession), UT_POINTER, "Session");
    utRegisterField("SessionTableIndex_", &saRoots.SessionTableIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumSessionTable", &saRoots.NumSessionTable, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("SessionTable", &saRoots.SessionTable, sizeof(saSession), UT_POINTER, "Session");
    utRegisterArray(&saRootData.usedRootSessionTable, &saRootData.allocatedRootSessionTable,
        getRootSessionTables, allocRootSessionTables, saCompactRootSessionTables);
    utRegisterField("NumSession", &saRoots.NumSession, sizeof(uint32), UT_UINT, NULL);
    utRegisterField("FirstThread", &saRoots.FirstThread, sizeof(saThread), UT_POINTER, "Thread");
    utRegisterField("LastThread", &saRoots.LastThread, sizeof(saThread), UT_POINTER, "Thread");
    utRegisterField("ThreadTableIndex_", &saRoots.ThreadTableIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumThreadTable", &saRoots.NumThreadTable, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("ThreadTable", &saRoots.ThreadTable, sizeof(saThread), UT_POINTER, "Thread");
    utRegisterArray(&saRootData.usedRootThreadTable, &saRootData.allocatedRootThreadTable,
        getRootThreadTables, allocRootThreadTables, saCompactRootThreadTables);
    utRegisterField("NumThread", &saRoots.NumThread, sizeof(uint32), UT_UINT, NULL);
    utRegisterField("FirstPost", &saRoots.FirstPost, sizeof(saPost), UT_POINTER, "Post");
    utRegisterField("LastPost", &saRoots.LastPost, sizeof(saPost), UT_POINTER, "Post");
    utRegisterField("PostTableIndex_", &saRoots.PostTableIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumPostTable", &saRoots.NumPostTable, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("PostTable", &saRoots.PostTable, sizeof(saPost), UT_POINTER, "Post");
    utRegisterArray(&saRootData.usedRootPostTable, &saRootData.allocatedRootPostTable,
        getRootPostTables, allocRootPostTables, saCompactRootPostTables);
    utRegisterField("NumPost", &saRoots.NumPost, sizeof(uint32), UT_UINT, NULL);
    utRegisterField("FirstAnnouncement", &saRoots.FirstAnnouncement, sizeof(saAnnouncement), UT_POINTER, "Announcement");
    utRegisterField("LastAnnouncement", &saRoots.LastAnnouncement, sizeof(saAnnouncement), UT_POINTER, "Announcement");
    utRegisterField("AnnouncementTableIndex_", &saRoots.AnnouncementTableIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumAnnouncementTable", &saRoots.NumAnnouncementTable, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("AnnouncementTable", &saRoots.AnnouncementTable, sizeof(saAnnouncement), UT_POINTER, "Announcement");
    utRegisterArray(&saRootData.usedRootAnnouncementTable, &saRootData.allocatedRootAnnouncementTable,
        getRootAnnouncementTables, allocRootAnnouncementTables, saCompactRootAnnouncementTables);
    utRegisterField("NumAnnouncement", &saRoots.NumAnnouncement, sizeof(uint32), UT_UINT, NULL);
    utRegisterClass("Region", 9, &saRootData.usedRegion, &saRootData.allocatedRegion,
        &saRootData.firstFreeRegion, 60, 4, allocRegion, destroyRegion);
    utRegisterField("Sym", &saRegions.Sym, sizeof(utSym), UT_SYM, NULL);
    utRegisterField("NumListings", &saRegions.NumListings, sizeof(uint32), UT_UINT, NULL);
    utRegisterField("Root", &saRegions.Root, sizeof(saRoot), UT_POINTER, "Root");
    utRegisterField("NextRootRegion", &saRegions.NextRootRegion, sizeof(saRegion), UT_POINTER, "Region");
    utRegisterField("PrevRootRegion", &saRegions.PrevRootRegion, sizeof(saRegion), UT_POINTER, "Region");
    utRegisterField("NextTableRootRegion", &saRegions.NextTableRootRegion, sizeof(saRegion), UT_POINTER, "Region");
    utRegisterField("FirstUser", &saRegions.FirstUser, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("LastUser", &saRegions.LastUser, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("Session", &saRegions.Session, sizeof(saSession), UT_POINTER, "Session");
    utRegisterClass("Session", 7, &saRootData.usedSession, &saRootData.allocatedSession,
        &saRootData.firstFreeSession, 69, 4, allocSession, destroySession);
    utRegisterField("Sym", &saSessions.Sym, sizeof(utSym), UT_SYM, NULL);
    utRegisterField("Root", &saSessions.Root, sizeof(saRoot), UT_POINTER, "Root");
    utRegisterField("NextRootSession", &saSessions.NextRootSession, sizeof(saSession), UT_POINTER, "Session");
    utRegisterField("PrevRootSession", &saSessions.PrevRootSession, sizeof(saSession), UT_POINTER, "Session");
    utRegisterField("NextTableRootSession", &saSessions.NextTableRootSession, sizeof(saSession), UT_POINTER, "Session");
    utRegisterField("User", &saSessions.User, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("Region", &saSessions.Region, sizeof(saRegion), UT_POINTER, "Region");
    utRegisterClass("User", 57, &saRootData.usedUser, &saRootData.allocatedUser,
        &saRootData.firstFreeUser, 76, 4, allocUser, destroyUser);
    utRegisterField("Sym", &saUsers.Sym, sizeof(utSym), UT_SYM, NULL);
    utRegisterField("ShownNameIndex_", &saUsers.ShownNameIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumShownName", &saUsers.NumShownName, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("ShownName", &saUsers.ShownName, sizeof(char), UT_CHAR, NULL);
    utRegisterArray(&saRootData.usedUserShownName, &saRootData.allocatedUserShownName,
        getUserShownNames, allocUserShownNames, saCompactUserShownNames);
    utRegisterField("Email", &saUsers.Email, sizeof(utSym), UT_SYM, NULL);
    utRegisterField("JoinDate", &saUsers.JoinDate, sizeof(uint64), UT_UINT, NULL);
    utRegisterField("Active", &saUsers.Active, sizeof(uint8), UT_BOOL, NULL);
    utRegisterField("HashedPasswordIndex_", &saUsers.HashedPasswordIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumHashedPassword", &saUsers.NumHashedPassword, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("HashedPassword", &saUsers.HashedPassword, sizeof(uint8), UT_UINT, NULL);
    utRegisterArray(&saRootData.usedUserHashedPassword, &saRootData.allocatedUserHashedPassword,
        getUserHashedPasswords, allocUserHashedPasswords, saCompactUserHashedPasswords);
    utRegisterField("NounceIndex_", &saUsers.NounceIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumNounce", &saUsers.NumNounce, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("Nounce", &saUsers.Nounce, sizeof(uint8), UT_UINT, NULL);
    utRegisterArray(&saRootData.usedUserNounce, &saRootData.allocatedUserNounce,
        getUserNounces, allocUserNounces, saCompactUserNounces);
    utRegisterField("LoggedIn", &saUsers.LoggedIn, sizeof(uint8), UT_BOOL, NULL);
    utRegisterField("LastActivity", &saUsers.LastActivity, sizeof(uint64), UT_UINT, NULL);
    utRegisterField("Balance", &saUsers.Balance, sizeof(uint32), UT_UINT, NULL);
    utRegisterField("Validated", &saUsers.Validated, sizeof(uint8), UT_BOOL, NULL);
    utRegisterField("ValidationKeyIndex_", &saUsers.ValidationKeyIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumValidationKey", &saUsers.NumValidationKey, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("ValidationKey", &saUsers.ValidationKey, sizeof(uint8), UT_UINT, NULL);
    utRegisterArray(&saRootData.usedUserValidationKey, &saRootData.allocatedUserValidationKey,
        getUserValidationKeys, allocUserValidationKeys, saCompactUserValidationKeys);
    utRegisterField("SupremeLeader", &saUsers.SupremeLeader, sizeof(uint8), UT_BOOL, NULL);
    utRegisterField("RegionManager", &saUsers.RegionManager, sizeof(uint8), UT_BOOL, NULL);
    utRegisterField("Root", &saUsers.Root, sizeof(saRoot), UT_POINTER, "Root");
    utRegisterField("NextRootUser", &saUsers.NextRootUser, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("PrevRootUser", &saUsers.PrevRootUser, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("NextTableRootUser", &saUsers.NextTableRootUser, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("ByEmailRoot", &saUsers.ByEmailRoot, sizeof(saRoot), UT_POINTER, "Root");
    utRegisterField("NextRootByEmailUser", &saUsers.NextRootByEmailUser, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("PrevRootByEmailUser", &saUsers.PrevRootByEmailUser, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("NextTableRootByEmailUser", &saUsers.NextTableRootByEmailUser, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("Region", &saUsers.Region, sizeof(saRegion), UT_POINTER, "Region");
    utRegisterField("NextRegionUser", &saUsers.NextRegionUser, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("PrevRegionUser", &saUsers.PrevRegionUser, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("Session", &saUsers.Session, sizeof(saSession), UT_POINTER, "Session");
    utRegisterField("Charity", &saUsers.Charity, sizeof(saCharity), UT_POINTER, "Charity");
    utRegisterField("FirstMembership", &saUsers.FirstMembership, sizeof(saMembership), UT_POINTER, "Membership");
    utRegisterField("LastMembership", &saUsers.LastMembership, sizeof(saMembership), UT_POINTER, "Membership");
    utRegisterField("MembershipTableIndex_", &saUsers.MembershipTableIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumMembershipTable", &saUsers.NumMembershipTable, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("MembershipTable", &saUsers.MembershipTable, sizeof(saMembership), UT_POINTER, "Membership");
    utRegisterArray(&saRootData.usedUserMembershipTable, &saRootData.allocatedUserMembershipTable,
        getUserMembershipTables, allocUserMembershipTables, saCompactUserMembershipTables);
    utRegisterField("NumMembership", &saUsers.NumMembership, sizeof(uint32), UT_UINT, NULL);
    utRegisterField("FirstOutTransaction", &saUsers.FirstOutTransaction, sizeof(saTransaction), UT_POINTER, "Transaction");
    utRegisterField("LastOutTransaction", &saUsers.LastOutTransaction, sizeof(saTransaction), UT_POINTER, "Transaction");
    utRegisterField("FirstInTransaction", &saUsers.FirstInTransaction, sizeof(saTransaction), UT_POINTER, "Transaction");
    utRegisterField("LastInTransaction", &saUsers.LastInTransaction, sizeof(saTransaction), UT_POINTER, "Transaction");
    utRegisterField("FirstListing", &saUsers.FirstListing, sizeof(saListing), UT_POINTER, "Listing");
    utRegisterField("LastListing", &saUsers.LastListing, sizeof(saListing), UT_POINTER, "Listing");
    utRegisterField("FirstPost", &saUsers.FirstPost, sizeof(saPost), UT_POINTER, "Post");
    utRegisterField("LastPost", &saUsers.LastPost, sizeof(saPost), UT_POINTER, "Post");
    utRegisterField("PostTableIndex_", &saUsers.PostTableIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumPostTable", &saUsers.NumPostTable, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("PostTable", &saUsers.PostTable, sizeof(saPost), UT_POINTER, "Post");
    utRegisterArray(&saRootData.usedUserPostTable, &saRootData.allocatedUserPostTable,
        getUserPostTables, allocUserPostTables, saCompactUserPostTables);
    utRegisterField("NumPost", &saUsers.NumPost, sizeof(uint32), UT_UINT, NULL);
    utRegisterField("FirstSubscription", &saUsers.FirstSubscription, sizeof(saSubscription), UT_POINTER, "Subscription");
    utRegisterField("LastSubscription", &saUsers.LastSubscription, sizeof(saSubscription), UT_POINTER, "Subscription");
    utRegisterField("FirstAnnouncement", &saUsers.FirstAnnouncement, sizeof(saAnnouncement), UT_POINTER, "Announcement");
    utRegisterField("LastAnnouncement", &saUsers.LastAnnouncement, sizeof(saAnnouncement), UT_POINTER, "Announcement");
    utRegisterClass("Transaction", 11, &saRootData.usedTransaction, &saRootData.allocatedTransaction,
        &saRootData.firstFreeTransaction, 138, 4, allocTransaction, destroyTransaction);
    utRegisterField("Date", &saTransactions.Date, sizeof(uint64), UT_UINT, NULL);
    utRegisterField("Amount", &saTransactions.Amount, sizeof(uint32), UT_UINT, NULL);
    utRegisterField("DescriptionIndex_", &saTransactions.DescriptionIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumDescription", &saTransactions.NumDescription, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("Description", &saTransactions.Description, sizeof(char), UT_CHAR, NULL);
    utRegisterArray(&saRootData.usedTransactionDescription, &saRootData.allocatedTransactionDescription,
        getTransactionDescriptions, allocTransactionDescriptions, saCompactTransactionDescriptions);
    utRegisterField("FromUser", &saTransactions.FromUser, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("NextUserOutTransaction", &saTransactions.NextUserOutTransaction, sizeof(saTransaction), UT_POINTER, "Transaction");
    utRegisterField("PrevUserOutTransaction", &saTransactions.PrevUserOutTransaction, sizeof(saTransaction), UT_POINTER, "Transaction");
    utRegisterField("ToUser", &saTransactions.ToUser, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("NextUserInTransaction", &saTransactions.NextUserInTransaction, sizeof(saTransaction), UT_POINTER, "Transaction");
    utRegisterField("PrevUserInTransaction", &saTransactions.PrevUserInTransaction, sizeof(saTransaction), UT_POINTER, "Transaction");
    utRegisterClass("Charity", 6, &saRootData.usedCharity, &saRootData.allocatedCharity,
        &saRootData.firstFreeCharity, 144, 4, allocCharity, destroyCharity);
    utRegisterField("Root", &saCharitys.Root, sizeof(saRoot), UT_POINTER, "Root");
    utRegisterField("NextRootCharity", &saCharitys.NextRootCharity, sizeof(saCharity), UT_POINTER, "Charity");
    utRegisterField("PrevRootCharity", &saCharitys.PrevRootCharity, sizeof(saCharity), UT_POINTER, "Charity");
    utRegisterField("User", &saCharitys.User, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("FirstMembership", &saCharitys.FirstMembership, sizeof(saMembership), UT_POINTER, "Membership");
    utRegisterField("LastMembership", &saCharitys.LastMembership, sizeof(saMembership), UT_POINTER, "Membership");
    utRegisterClass("Membership", 7, &saRootData.usedMembership, &saRootData.allocatedMembership,
        &saRootData.firstFreeMembership, 150, 4, allocMembership, destroyMembership);
    utRegisterField("User", &saMemberships.User, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("NextUserMembership", &saMemberships.NextUserMembership, sizeof(saMembership), UT_POINTER, "Membership");
    utRegisterField("PrevUserMembership", &saMemberships.PrevUserMembership, sizeof(saMembership), UT_POINTER, "Membership");
    utRegisterField("NextTableUserMembership", &saMemberships.NextTableUserMembership, sizeof(saMembership), UT_POINTER, "Membership");
    utRegisterField("Charity", &saMemberships.Charity, sizeof(saCharity), UT_POINTER, "Charity");
    utRegisterField("NextCharityMembership", &saMemberships.NextCharityMembership, sizeof(saMembership), UT_POINTER, "Membership");
    utRegisterField("PrevCharityMembership", &saMemberships.PrevCharityMembership, sizeof(saMembership), UT_POINTER, "Membership");
    utRegisterClass("Category", 8, &saRootData.usedCategory, &saRootData.allocatedCategory,
        &saRootData.firstFreeCategory, 157, 4, allocCategory, destroyCategory);
    utRegisterField("Sym", &saCategorys.Sym, sizeof(utSym), UT_SYM, NULL);
    utRegisterField("NumListings", &saCategorys.NumListings, sizeof(uint32), UT_UINT, NULL);
    utRegisterField("Root", &saCategorys.Root, sizeof(saRoot), UT_POINTER, "Root");
    utRegisterField("NextRootCategory", &saCategorys.NextRootCategory, sizeof(saCategory), UT_POINTER, "Category");
    utRegisterField("PrevRootCategory", &saCategorys.PrevRootCategory, sizeof(saCategory), UT_POINTER, "Category");
    utRegisterField("NextTableRootCategory", &saCategorys.NextTableRootCategory, sizeof(saCategory), UT_POINTER, "Category");
    utRegisterField("FirstListing", &saCategorys.FirstListing, sizeof(saListing), UT_POINTER, "Listing");
    utRegisterField("LastListing", &saCategorys.LastListing, sizeof(saListing), UT_POINTER, "Listing");
    utRegisterClass("Listing", 20, &saRootData.usedListing, &saRootData.allocatedListing,
        &saRootData.firstFreeListing, 175, 4, allocListing, destroyListing);
    utRegisterField("TitleIndex_", &saListings.TitleIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumTitle", &saListings.NumTitle, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("Title", &saListings.Title, sizeof(char), UT_CHAR, NULL);
    utRegisterArray(&saRootData.usedListingTitle, &saRootData.allocatedListingTitle,
        getListingTitles, allocListingTitles, saCompactListingTitles);
    utRegisterField("DescriptionIndex_", &saListings.DescriptionIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumDescription", &saListings.NumDescription, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("Description", &saListings.Description, sizeof(char), UT_CHAR, NULL);
    utRegisterArray(&saRootData.usedListingDescription, &saRootData.allocatedListingDescription,
        getListingDescriptions, allocListingDescriptions, saCompactListingDescriptions);
    utRegisterField("ID", &saListings.ID, sizeof(uint64), UT_UINT, NULL);
    utRegisterField("Rate", &saListings.Rate, sizeof(uint32), UT_UINT, NULL);
    utRegisterField("Offered", &saListings.Offered, sizeof(uint8), UT_BOOL, NULL);
    utRegisterField("FixedPrice", &saListings.FixedPrice, sizeof(uint8), UT_BOOL, NULL);
    utRegisterField("Root", &saListings.Root, sizeof(saRoot), UT_POINTER, "Root");
    utRegisterField("NextRootListing", &saListings.NextRootListing, sizeof(saListing), UT_POINTER, "Listing");
    utRegisterField("PrevRootListing", &saListings.PrevRootListing, sizeof(saListing), UT_POINTER, "Listing");
    utRegisterField("NextTableRootListing", &saListings.NextTableRootListing, sizeof(saListing), UT_POINTER, "Listing");
    utRegisterField("User", &saListings.User, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("NextUserListing", &saListings.NextUserListing, sizeof(saListing), UT_POINTER, "Listing");
    utRegisterField("PrevUserListing", &saListings.PrevUserListing, sizeof(saListing), UT_POINTER, "Listing");
    utRegisterField("Category", &saListings.Category, sizeof(saCategory), UT_POINTER, "Category");
    utRegisterField("NextCategoryListing", &saListings.NextCategoryListing, sizeof(saListing), UT_POINTER, "Listing");
    utRegisterField("PrevCategoryListing", &saListings.PrevCategoryListing, sizeof(saListing), UT_POINTER, "Listing");
    utRegisterClass("Thread", 13, &saRootData.usedThread, &saRootData.allocatedThread,
        &saRootData.firstFreeThread, 190, 4, allocThread, destroyThread);
    utRegisterField("SubjectIndex_", &saThreads.SubjectIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumSubject", &saThreads.NumSubject, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("Subject", &saThreads.Subject, sizeof(char), UT_CHAR, NULL);
    utRegisterArray(&saRootData.usedThreadSubject, &saRootData.allocatedThreadSubject,
        getThreadSubjects, allocThreadSubjects, saCompactThreadSubjects);
    utRegisterField("ID", &saThreads.ID, sizeof(uint32), UT_UINT, NULL);
    utRegisterField("NumPosts", &saThreads.NumPosts, sizeof(uint32), UT_UINT, NULL);
    utRegisterField("Root", &saThreads.Root, sizeof(saRoot), UT_POINTER, "Root");
    utRegisterField("NextRootThread", &saThreads.NextRootThread, sizeof(saThread), UT_POINTER, "Thread");
    utRegisterField("PrevRootThread", &saThreads.PrevRootThread, sizeof(saThread), UT_POINTER, "Thread");
    utRegisterField("NextTableRootThread", &saThreads.NextTableRootThread, sizeof(saThread), UT_POINTER, "Thread");
    utRegisterField("FirstPost", &saThreads.FirstPost, sizeof(saPost), UT_POINTER, "Post");
    utRegisterField("LastPost", &saThreads.LastPost, sizeof(saPost), UT_POINTER, "Post");
    utRegisterField("FirstSubscription", &saThreads.FirstSubscription, sizeof(saSubscription), UT_POINTER, "Subscription");
    utRegisterField("LastSubscription", &saThreads.LastSubscription, sizeof(saSubscription), UT_POINTER, "Subscription");
    utRegisterClass("Post", 16, &saRootData.usedPost, &saRootData.allocatedPost,
        &saRootData.firstFreePost, 203, 4, allocPost, destroyPost);
    utRegisterField("Date", &saPosts.Date, sizeof(uint64), UT_UINT, NULL);
    utRegisterField("MessageIndex_", &saPosts.MessageIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumMessage", &saPosts.NumMessage, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("Message", &saPosts.Message, sizeof(char), UT_CHAR, NULL);
    utRegisterArray(&saRootData.usedPostMessage, &saRootData.allocatedPostMessage,
        getPostMessages, allocPostMessages, saCompactPostMessages);
    utRegisterField("ID", &saPosts.ID, sizeof(uint32), UT_UINT, NULL);
    utRegisterField("Root", &saPosts.Root, sizeof(saRoot), UT_POINTER, "Root");
    utRegisterField("NextRootPost", &saPosts.NextRootPost, sizeof(saPost), UT_POINTER, "Post");
    utRegisterField("PrevRootPost", &saPosts.PrevRootPost, sizeof(saPost), UT_POINTER, "Post");
    utRegisterField("NextTableRootPost", &saPosts.NextTableRootPost, sizeof(saPost), UT_POINTER, "Post");
    utRegisterField("User", &saPosts.User, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("NextUserPost", &saPosts.NextUserPost, sizeof(saPost), UT_POINTER, "Post");
    utRegisterField("PrevUserPost", &saPosts.PrevUserPost, sizeof(saPost), UT_POINTER, "Post");
    utRegisterField("NextTableUserPost", &saPosts.NextTableUserPost, sizeof(saPost), UT_POINTER, "Post");
    utRegisterField("Thread", &saPosts.Thread, sizeof(saThread), UT_POINTER, "Thread");
    utRegisterField("NextThreadPost", &saPosts.NextThreadPost, sizeof(saPost), UT_POINTER, "Post");
    utRegisterField("PrevThreadPost", &saPosts.PrevThreadPost, sizeof(saPost), UT_POINTER, "Post");
    utRegisterClass("Subscription", 6, &saRootData.usedSubscription, &saRootData.allocatedSubscription,
        &saRootData.firstFreeSubscription, 214, 4, allocSubscription, destroySubscription);
    utRegisterField("User", &saSubscriptions.User, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("NextUserSubscription", &saSubscriptions.NextUserSubscription, sizeof(saSubscription), UT_POINTER, "Subscription");
    utRegisterField("PrevUserSubscription", &saSubscriptions.PrevUserSubscription, sizeof(saSubscription), UT_POINTER, "Subscription");
    utRegisterField("Thread", &saSubscriptions.Thread, sizeof(saThread), UT_POINTER, "Thread");
    utRegisterField("NextThreadSubscription", &saSubscriptions.NextThreadSubscription, sizeof(saSubscription), UT_POINTER, "Subscription");
    utRegisterField("PrevThreadSubscription", &saSubscriptions.PrevThreadSubscription, sizeof(saSubscription), UT_POINTER, "Subscription");
    utRegisterClass("Announcement", 15, &saRootData.usedAnnouncement, &saRootData.allocatedAnnouncement,
        &saRootData.firstFreeAnnouncement, 228, 4, allocAnnouncement, destroyAnnouncement);
    utRegisterField("Date", &saAnnouncements.Date, sizeof(uint64), UT_UINT, NULL);
    utRegisterField("SubjectIndex_", &saAnnouncements.SubjectIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumSubject", &saAnnouncements.NumSubject, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("Subject", &saAnnouncements.Subject, sizeof(char), UT_CHAR, NULL);
    utRegisterArray(&saRootData.usedAnnouncementSubject, &saRootData.allocatedAnnouncementSubject,
        getAnnouncementSubjects, allocAnnouncementSubjects, saCompactAnnouncementSubjects);
    utRegisterField("MessageIndex_", &saAnnouncements.MessageIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumMessage", &saAnnouncements.NumMessage, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("Message", &saAnnouncements.Message, sizeof(char), UT_CHAR, NULL);
    utRegisterArray(&saRootData.usedAnnouncementMessage, &saRootData.allocatedAnnouncementMessage,
        getAnnouncementMessages, allocAnnouncementMessages, saCompactAnnouncementMessages);
    utRegisterField("ID", &saAnnouncements.ID, sizeof(uint32), UT_UINT, NULL);
    utRegisterField("Root", &saAnnouncements.Root, sizeof(saRoot), UT_POINTER, "Root");
    utRegisterField("NextRootAnnouncement", &saAnnouncements.NextRootAnnouncement, sizeof(saAnnouncement), UT_POINTER, "Announcement");
    utRegisterField("PrevRootAnnouncement", &saAnnouncements.PrevRootAnnouncement, sizeof(saAnnouncement), UT_POINTER, "Announcement");
    utRegisterField("NextTableRootAnnouncement", &saAnnouncements.NextTableRootAnnouncement, sizeof(saAnnouncement), UT_POINTER, "Announcement");
    utRegisterField("User", &saAnnouncements.User, sizeof(saUser), UT_POINTER, "User");
    utRegisterField("NextUserAnnouncement", &saAnnouncements.NextUserAnnouncement, sizeof(saAnnouncement), UT_POINTER, "Announcement");
    utRegisterField("PrevUserAnnouncement", &saAnnouncements.PrevUserAnnouncement, sizeof(saAnnouncement), UT_POINTER, "Announcement");
    utRegisterClass("TransactionArray", 5, &saRootData.usedTransactionArray, &saRootData.allocatedTransactionArray,
        &saRootData.firstFreeTransactionArray, 239, 4, allocTransactionArray, destroyTransactionArray);
    utRegisterField("TransactionIndex_", &saTransactionArrays.TransactionIndex_, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("NumTransaction", &saTransactionArrays.NumTransaction, sizeof(uint32), UT_UINT, NULL);
    utSetFieldHidden();
    utRegisterField("Transaction", &saTransactionArrays.Transaction, sizeof(saTransaction), UT_POINTER, "Transaction");
    utRegisterArray(&saRootData.usedTransactionArrayTransaction, &saRootData.allocatedTransactionArrayTransaction,
        getTransactionArrayTransactions, allocTransactionArrayTransactions, saCompactTransactionArrayTransactions);
    utRegisterField("UsedTransaction", &saTransactionArrays.UsedTransaction, sizeof(uint32), UT_UINT, NULL);
    utRegisterField("FreeList", &saTransactionArrays.FreeList, sizeof(saTransactionArray), UT_POINTER, "TransactionArray");
    utSetFieldHidden();
    allocRoots();
    allocRegions();
    allocSessions();
    allocUsers();
    allocTransactions();
    allocCharitys();
    allocMemberships();
    allocCategorys();
    allocListings();
    allocThreads();
    allocPosts();
    allocSubscriptions();
    allocAnnouncements();
    allocTransactionArrays();
}

