/***************************************************************
 *
 * file: pss.h
 *
 * @Authors  Nikolaos Vasilikopoulos (nvasilik@csd.uoc.gr), John Petropoulos (johnpetr@csd.uoc.gr)
 * @Version 30-11-2022
 *
 * @e-mail       hy240-list@csd.uoc.gr
 *
 * @brief   Header file for the Public Subscribe System,
 * with the structures and function prototypes
 *
 ***************************************************************
 */

#ifndef pss_h
#define pss_h
#define MG 64

typedef struct Info {
    int iId;
    int itm;
    int igp[MG];
    struct Info *ilc;
    struct Info *irc;
    struct Info *ip;
}Info;
typedef struct Subscription {
    int sId;
    struct Subscription *snext;
}Subscription;
typedef struct Group {
    int gId;
    struct Subscription *gsub;
    struct Info *gr;
}Group;
typedef struct SubInfo {
    int sId;
    int stm;
    struct TreeInfo *tgp[MG];
    struct TreeInfo *sgp[MG];
    struct SubInfo *snext;
}SubInfo;
typedef struct TreeInfo {
    int tId;
    int ttm;
    struct TreeInfo *tlc;
    struct TreeInfo *trc;
    struct TreeInfo *tp;
    struct TreeInfo *next;
    struct TreeInfo *prev;
}TreeInfo;

typedef struct Infolist{
    int id;
    int time;
    struct Infolist *next;
}Infolist;


/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @param m Size of hash table
 * @param p Prime number for the universal hash function
 *
 * @return 0 on success
 *         1 on failure
 */
int initialize(int m, int p);

/**
 * @brief Free resources
 *
 * @return 0 on success
 *         1 on failure
 */
int free_all(void);

/**
 * @brief Insert info
 *
 * @param iTM Timestamp of arrival
 * @param iId Identifier of information
 * @param gids_arr Pointer to array containing the gids of the Event.
 * @param size_of_gids_arr Size of gids_arr including -1
 * @return 0 on success
 *          1 on failure
 */
int Insert_Info(int iTM,int iId,int* gids_arr,int size_of_gids_arr);

/**
 * @brief Subsriber Registration
 *
 * @param sTM Timestamp of arrival
 * @param sId Identifier of subscriber
 * @param gids_arr Pointer to array containing the gids of the Event.
 * @param size_of_gids_arr Size of gids_arr including -1
 * @return 0 on success
 *          1 on failure
 */
int Subscriber_Registration(int sTM,int sId,int* gids_arr,int size_of_gids_arr);

/**
 * @brief Prune Information from server and forward it to client
 *
 * @param tm Information timestamp of arrival
 * @return 0 on success
 *          1 on failure
 */
int Prune(int tm);

/**
 * @brief Consume Information for subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Consume(int sId);

/**
 * @brief Delete subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Delete_Subscriber(int sId);

/**
 * @brief Print Data Structures of the system
 *
 * @return 0 on success
 *          1 on failure
 */
int Print_all(void);

void BST_Print(Info *root);
Info *BST_LookUP(Info *root, int id);
Info *BST_Insert(Info *root, int id, int tm );
Info *BST_Delete(Info *root, int id);
void Inorder(Info *root,int tm);

SubInfo *SL_Insert(SubInfo *head, int sId, int Stm);
SubInfo *SL_Delete(SubInfo *head, int sId);
void SL_print_list(SubInfo *list); 
SubInfo *SL_LookUp(SubInfo *list,int sid);

int hashfunction(int id);
SubInfo Hash_Insert(int sid, int stm);
void Hash_Delete(SubInfo **table,int sid);
SubInfo *Hash_LookUp(int sid);
void Hash_print();

Subscription *L_Insert(Subscription *head, int sid);
void print_list(Subscription *list);
Subscription *L_Delete(Subscription *head,int sid);

TreeInfo *LO_BST_Insert(TreeInfo *root, int id, int tm);
TreeInfo *LO_Insert(TreeInfo *root, int id, int tm);
void LO_BST_Print(TreeInfo *T);


Infolist *IL_LookUp(Infolist *list,int sid);
void Iprint_list(Infolist *list);
Infolist *IL_Delete(Infolist *head,int sid);
Infolist *IL_Insert(Infolist *head, int id,int time);

TreeInfo* LO_BST_LookUp(TreeInfo* root, int id,int itm);
int isleaf(TreeInfo *node);
TreeInfo *FindLast(TreeInfo *head);

#endif /* pss_h */

