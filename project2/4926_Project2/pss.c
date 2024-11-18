/***************************************************************
 *
 * file: pss.h
 *
 * @Author  Nikolaos Vasilikopoulos (nvasilik@csd.uoc.gr), John Petropoulos (johnpetr@csd.uoc.gr)
 * @Version 30-11-2022
 *
 * @e-mail       hy240-list@csd.uoc.gr
 *
 * @brief   Implementation of the "pss.h" header file for the Public Subscribe System,
 * function definitions
 *
 *
 ***************************************************************
 */

#include <stdio.h>
#include <stdlib.h>

#include "pss.h"


Group  G[MG];
int M,P;
SubInfo **Hashtable;
Infolist *helplist,*h;


/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @param m Size of the hash table.
 * @param p Prime number for the universal hash functions.
 *
 * @return 0 on success
 *         1 on failure
 */
int initialize(int m, int p){
    P=p;
    M=m;
    Hashtable = (SubInfo**) malloc(M*sizeof(SubInfo*));
    int i=0;
    for (i = 0; i < MG; i++)
    {
        G[i].gr=NULL;
    }
    return EXIT_SUCCESS;
}

/**
 * @brief Free resources
 *
 * @return 0 on success
 *         1 on failure
 */
int free_all(void){
    return EXIT_SUCCESS;
}

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
int Insert_Info(int iTM,int iId,int* gids_arr,int size_of_gids_arr){
    int i = 0;
    for (i = 0; i < size_of_gids_arr-1; i++)
    {
        G[gids_arr[i]].gr=BST_Insert(G[gids_arr[i]].gr,iId,iTM); 
        
    }

    for (i = 0; i < size_of_gids_arr-1; i++){
        printf("\tGROUPID = <%d>",gids_arr[i]);
        printf(", INFOLIST = < ");
        BST_Print(G[gids_arr[i]].gr);
        printf(" >\n");
    }

    return EXIT_SUCCESS;
}
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
int Subscriber_Registration(int sTM,int sId,int* gids_arr,int size_of_gids_arr){
    int i=0;
    SubInfo *current=NULL;
    Hash_Insert(sId,sTM);

    
    current=Hash_LookUp(sId);  /*GIA TO CONSUME*/
    
    
    SubInfo *check;


    
    for (i = 0; i < size_of_gids_arr-1; i++){
        G[gids_arr[i]].gsub=L_Insert(G[gids_arr[i]].gsub,sId);
        current->sgp[gids_arr[i]]=NULL;  /*HOLDING THE OLDEST INFO*/
        current->tgp[gids_arr[i]]=NULL;
    }
    
    Hash_print(Hashtable);
    printf("\n");

    for (i = 0; i < size_of_gids_arr-1; i++){
        printf("\tGROUPID = <%d>",gids_arr[i]);
        printf(", SUBLIST = < ");
        print_list(G[gids_arr[i]].gsub); 
        printf(" >\n");
    }
    
    


    return EXIT_SUCCESS;
}
/**
 * @brief Prune Information from server and forward it to client
 *
 * @param tm Information timestamp of arrival
 * @return 0 on success
 *          1 on failure
 */
int Prune(int tm){
    int i;
    int j;
      
    for (i=0;i<MG;i++){
        SubInfo *sptr;
        Subscription *ptr=G[i].gsub;
        Infolist *iptr;
        Info *in=G[i].gr;
        TreeInfo *k;
        
        if (in==NULL){continue;}
        if (in!=NULL){            
            Inorder(in,tm);
        }
        while (ptr!=NULL){
            sptr=Hash_LookUp(ptr->sId);
            k=NULL;
            iptr=helplist;
            while (iptr!=NULL){
                k=LO_Insert(k,iptr->id,iptr->time);
                G[i].gr=BST_Delete(G[i].gr,iptr->id);
                sptr->tgp[i]=k;
                iptr=iptr->next;
            }
            ptr=ptr->snext;
        }
                
        helplist=NULL;

    }

    

    for (i = 0; i < MG; i++){
        if (G[i].gr==NULL && G[i].gsub==NULL){continue;}
        printf("\tGROUPID = <%d>",i);
        printf(", INFOLIST = < ");
        BST_Print(G[i].gr);
        printf(" >,  SUBLIST = <");
        print_list(G[i].gsub); 
        printf(" >\n");
        
    }
    printf("\n");
    for(i=0;i<M;i++){
        SubInfo *sptr=Hashtable[i];
        Subscription *ptr=NULL;
        if(sptr==NULL){continue;}
        while(sptr!=NULL){
            printf("\n\tSUBSCRIBERID<%d>,GROUPID =\n\t",sptr->sId);
            for (j = 0; j < MG; j++){
                if(G[j].gsub==NULL){continue;}
                ptr=G[j].gsub;
                while (ptr!=NULL){
                    if(ptr->sId==sptr->sId && sptr->tgp[j]!=NULL){
                        printf("\t<%d> = ",j);
                        LO_BST_Print(sptr->tgp[j]);
                        printf("\n\t");
                    }
                    ptr=ptr->snext;
                }
                
            }
            sptr=sptr->snext;
        }
    }

    
    
    
    return EXIT_SUCCESS;
}
/**
 * @brief Consume Information for subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Consume(int sId){
    int i;
    int j;
      
    for (i=0;i<MG;i++){
        SubInfo *sptr;
        Subscription *ptr=G[i].gsub;
        Infolist *iptr;
        Info *in=G[i].gr;
        TreeInfo *k;
        
        if (in==NULL || ptr==NULL){continue;}

        
            sptr=Hash_LookUp(ptr->sId);
            if(sptr->tgp[i]==NULL){
                sptr->sgp[i]=NULL;
            }else{
            sptr->sgp[i]=FindLast(sptr->tgp[i]);
            }

            printf("\tGROUPID = <%d>",i);
            LO_BST_Print(sptr->tgp[i]);
            if(sptr->sgp[i]==NULL){
                printf("NEWSGP = <>\n");
                continue;
            }
            printf("NEWSGP = <%d>\n",sptr->sgp[i]->tId);
        }

    

    

    return EXIT_SUCCESS;
}
/**
 * @brief Delete subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Delete_Subscriber(int sId){
    SubInfo *ptr1=NULL;
    int arr[MG]={0};
    int i;

    ptr1=Hash_LookUp(sId);

    for ( i = 0; i < MG; i++)
    {
        if(ptr1->sgp[i]!=(TreeInfo*)1){  /*IM NOT LOOKING FOR IT IN EVERY GROUP */
            G[i].gsub=L_Delete(G[i].gsub,sId);
            arr[i]=1;
        }    
    }

    Hash_Delete(Hashtable, sId);

    Hash_print(Hashtable);
    printf("\n");

    for (i = 0; i < MG; i++){
        if(arr[i]==1){
            printf("\tGROUPID = <%d>",i);
            printf(", SUBLIST = < ");
            print_list(G[i].gsub);
            printf(" >\n");
        }    
    }

    return EXIT_SUCCESS;
}
/**
 * @brief Print Data Structures of the system
 *
 * @return 0 on success
 *          1 on failure
 */
int Print_all(void){
    int i,j;
    for (i = 0; i < MG; i++){
        if (G[i].gr==NULL && G[i].gsub==NULL){continue;}
        printf("\tGROUPID = <%d>",i);
        printf(", INFOLIST = < ");
        BST_Print(G[i].gr);
        printf(" >,  SUBLIST = <");
        print_list(G[i].gsub); 
        printf(" >\n");

    }

    for(i=0;i<M;i++){
        SubInfo *sptr=Hashtable[i];
        Subscription *ptr=NULL;
        if(sptr==NULL){continue;}
        while(sptr!=NULL){
            printf("\n\tSUBSCRIBERID<%d>,GROUPID =\n\t",sptr->sId);
            for (j = 0; j < MG; j++){
                if(G[j].gsub==NULL){continue;}
                ptr=G[j].gsub;
                while (ptr!=NULL){
                    if(ptr->sId==sptr->sId && sptr->tgp[j]!=NULL){
                        printf("\t<%d> = ",j);
                        LO_BST_Print(sptr->tgp[j]);
                        printf("\n\t");
                    }
                    ptr=ptr->snext;
                }
                
            }
            sptr=sptr->snext;
        }
    }
    
    return EXIT_SUCCESS;
}

/*=============BST=============*/


Info *BST_Insert(Info *root, int id, int tm ){
    Info *new_info=malloc(sizeof(struct Info));
    new_info->iId=id;
    new_info->itm=tm;
    new_info->ilc=NULL;
    new_info->irc=NULL;

    Info *ptr=root;
    Info *tmp=NULL;

    if(root == NULL){
        root=new_info;
    }else{
        while (ptr!=NULL){
            tmp=ptr;
            if (id>ptr->iId){
                ptr=ptr->irc;
            }else{
                ptr=ptr->ilc;
            }
        }

        new_info->ip=tmp;
        if (id>tmp->iId){
            tmp->irc=new_info;
        }else{
            tmp->ilc=new_info;
        }
    }
    return root;
}

Info *BST_Delete(Info *root, int id){
    Info *ptr=NULL;
    if (root == NULL) {
        return NULL;
    }

    if (id>root->iId){
        root->irc = BST_Delete(root->irc, id);
    }else if (id<root->iId){
        root->ilc = BST_Delete(root->ilc, id);
    }else{
        if (root->ilc==NULL&& root->irc==NULL) {
            return NULL;  
        }else if (root->ilc == NULL){
            ptr = root->irc;
            free(root);
            return ptr;
        }else if (root->irc == NULL) {
            ptr = root->ilc;
            free(root);
            return ptr;
        }
        
        Info *ptr2=NULL;
        ptr2=root->irc;
        if (ptr2 == NULL) {
        return NULL;
        }

        while (ptr2->ilc != NULL) {
            ptr2 = ptr2->ilc;
        }

        ptr=ptr2;

        root->iId = ptr->iId;
        root->irc = BST_Delete(root->irc, ptr->iId);
    }
    return root;
}


Info *BST_LookUP(Info *root, int id){
    Info *ptr = NULL;
    ptr = root;
    while (ptr!=NULL){
        if(ptr->iId==id){return ptr;}
        if (id>ptr->iId){
            ptr=ptr->irc;
        }else{
            ptr=ptr->ilc;
        }
    }
} 

void BST_Print(Info *root){
    if(root==NULL){
        return;
    }
    BST_Print(root->ilc);
    printf("%d, ", root->itm);
    BST_Print(root->irc);
}

void Inorder(Info *root,int tm){
    if (root == NULL){return;}
        
 
    if (root->itm<=tm){
        helplist=IL_Insert(helplist,root->iId,root->itm);
    }
    
    Inorder(root->irc,tm);
    Inorder(root->ilc,tm);
}

/*=========hepl list============*/

Infolist *IL_Insert(Infolist *head, int id,int time){
    Infolist *newSub = malloc(sizeof(struct Infolist));

    newSub->id = id;
    newSub->time = time;
    
    if (head==NULL){
        head=newSub;
        head->next=NULL;
    }else{
        newSub->next=head;
        head=newSub;
    }
}

Infolist *IL_Delete(Infolist *head,int sid){
    Infolist *ptr=head;
    Infolist *prev=NULL;
    
    while (ptr->next!=NULL && ptr->id!=sid)
    {
        prev=ptr;
        ptr=ptr->next;
    }    

    

    if (prev==NULL)
    {
        head=ptr->next;
        free(ptr);
        return head;
    }

    if (ptr->next!=NULL){
        prev->next=ptr->next;
    }
    
    else {
        prev->next=NULL;
        
    }
    
    
    free(ptr);

    return head;
}

void Iprint_list(Infolist *list) {
    Infolist *ptr = list;
    while (ptr != NULL) {
        printf("%d(%d),", ptr->id,ptr->time);
        ptr = ptr->next;
    }
}

Infolist *IL_LookUp(Infolist *list,int sid) {
    Infolist *ptr = list;
    while (ptr != NULL) {
        if (ptr->id==sid)
        {
           return ptr;
        }
        ptr = ptr->next;
    }

    return NULL;
}

/*===============SL===============*/

SubInfo *SL_Insert(SubInfo *head, int sId, int Stm) {
    SubInfo *newSub = malloc(sizeof(struct SubInfo));

    int i=0;
    newSub->sId = sId;
    newSub->stm = Stm;

    newSub->snext = NULL;

    for(i=0;i<MG;i++){
        newSub->sgp[i]=(TreeInfo*) 1;
    }


    if (head == NULL) {
        head = newSub;
    } else {
        SubInfo *ptr=head;
        SubInfo *prev=NULL;

        while (ptr!=NULL && ptr->stm>Stm)
        {
            prev=ptr;
            ptr=ptr->snext;
        }

        if (ptr==NULL)
        {
            prev->snext=newSub;
        }
        
        if (prev==NULL)
        {
            newSub->snext=head;
            head=newSub;

        } else{

            prev->snext=newSub;
            newSub->snext=ptr;
        }
        
    }

    return head;
       
}

SubInfo *SL_Delete(SubInfo *head, int sId){
    SubInfo *ptr=head;
    SubInfo *prev=NULL;
    
    while (ptr->snext!=NULL && ptr->sId!=sId)
    {
        prev=ptr;
        ptr=ptr->snext;
    }

    if (prev==NULL)
    {
        head=ptr->snext;
        free(ptr);
        return head;
    }

    if (ptr->snext!=NULL){
        prev->snext=ptr->snext;
    }
    
    else {
        prev->snext=NULL;
        
    }
    
    
    free(ptr);

    return head;
}

void SL_print_list(SubInfo *list) {
    SubInfo *ptr = list;
    printf("SUBSCRIBERLIST = <");
    while (ptr != NULL) {
        printf("%d, ", ptr->sId);
        ptr = ptr->snext;
    }
    printf(" >\n");
}

SubInfo *SL_LookUp(SubInfo *list,int sid) {
    SubInfo *ptr = list;
    while (ptr != NULL) {
        if (ptr->sId==sid)
        {
           return ptr;
        }
        ptr = ptr->snext;
    }

    return NULL;
}

/*==============Hash Table==============*/

int hashfunction(int id){
    int h = (P + id )% M;
    return h;
}

SubInfo Hash_Insert(int sid, int stm){
    Hashtable[hashfunction(sid)]=SL_Insert(Hashtable[hashfunction(sid)],sid,stm);
}

void Hash_Delete(SubInfo **table,int sid){
    table[hashfunction(sid)]=SL_Delete(Hashtable[hashfunction(sid)],sid);
}

SubInfo *Hash_LookUp(int sid){
    return SL_LookUp(Hashtable[hashfunction(sid)],sid);
}

void Hash_print(SubInfo **table){
    int i;
    SubInfo* curr=NULL;
    for(i=0;i<M;i++){
        if(table[i]!=NULL){
            printf("\tHASHTABLE[%d", i);
            printf("] ");
            printf("SUBSCRIBERS:< ");
            curr=table[i];
            while(curr!=NULL){
                printf("%d, ", curr->sId);
                curr=curr->snext;
            }
            printf("> ");
            printf("\n");
        }
    }
}

/*============L====================*/


Subscription *L_Insert(Subscription *head, int sid){
    Subscription *newSub = malloc(sizeof(struct Subscription));

    newSub->sId = sid;
    
    if (head==NULL){
        head=newSub;
        head->snext=NULL;
    }else{
        newSub->snext=head;
        head=newSub;
    }
}

Subscription *L_Delete(Subscription *head,int sid){
    Subscription *ptr=head;
    Subscription *prev=NULL;
    
    while (ptr->snext!=NULL && ptr->sId!=sid)
    {
        prev=ptr;
        ptr=ptr->snext;
    }    

    

    if (prev==NULL)
    {
        head=ptr->snext;
        free(ptr);
        return head;
    }

    if (ptr->snext!=NULL){
        prev->snext=ptr->snext;
    }
    
    else {
        prev->snext=NULL;
        
    }
    
    
    free(ptr);

    return head;
}

void print_list(Subscription *list) {
    Subscription *ptr = list;
    while (ptr != NULL) {
        printf("%d,", ptr->sId);
        ptr = ptr->snext;
    }
}

Subscription *L_LookUp(Subscription *list,int sid) {
    Subscription *ptr = list;
    while (ptr != NULL) {
        if (ptr->sId==sid)
        {
           return ptr;
        }
        ptr = ptr->snext;
    }

    return NULL;
}

/*============LO===============*/
TreeInfo* LO_BST_Insert(TreeInfo* root, int tId, int ttm){
    if(LO_BST_LookUp(root,tId,ttm)!=NULL){
        return root;
    }
    TreeInfo* new = (TreeInfo*) malloc(sizeof(TreeInfo));
    new->tId=tId;
    new->ttm=ttm;
    new->tlc=NULL;
    new->trc=NULL;
    new->tp=NULL;
    new->next=NULL;
    new->prev=NULL;
    if(root == NULL){
        root=new;
        return root;
    }
    TreeInfo* tmp=NULL;
    TreeInfo* curr=root;
    while(curr!=NULL){
        tmp=curr;
        if(ttm>curr->ttm){
            curr=curr->trc;
        }else{
            curr=curr->tlc;
        }
    }
    TreeInfo* new_2 = (TreeInfo*) malloc(sizeof(TreeInfo));
    new_2->tlc=NULL;
    new_2->trc=NULL;
    new_2->tp=NULL;
    new_2->prev=NULL;
    new_2->next=NULL;
    
    new->tp=tmp;
    new_2->tp=tmp;
    if(ttm<=tmp->ttm){
        new_2->ttm = tmp->ttm;
        new_2->tId = tmp->tId;
        tmp->ttm=ttm;
        tmp->tId=tId;
        tmp->tlc = new;
        tmp->trc= new_2;
        new->prev=tmp->prev;
        new->next=new_2;
        new_2->next=tmp->next;
        new_2->prev=new;
        if(tmp->prev!=NULL){
            tmp->prev->next=new;
        }
        if(tmp->next!=NULL){
            tmp->next->prev=new_2;
        }
        tmp->next=NULL;
        tmp->prev=NULL;
    }else{
        new_2->ttm=tmp->ttm;
        new_2->tId=tmp->tId;
        tmp->tlc=new_2;
        tmp->trc=new;
        new_2->next=new;
        new->next=tmp->next;
        new->prev=new_2;
        new_2->prev=tmp->prev;
        if(tmp->prev!=NULL){
            tmp->prev->next=new_2;
        }
        if(tmp->next!=NULL){
            tmp->next->prev=new;
        }
        new_2->tlc=NULL;
        new_2->trc=NULL;
        tmp->next=NULL;
        tmp->prev=NULL;
    }
    return root;
}

TreeInfo *LO_Insert(TreeInfo *root, int id, int tm){
    if(LO_BST_LookUp(root,id,tm)!=NULL){return root;}
    TreeInfo *newsub = (TreeInfo*) malloc(sizeof(TreeInfo));
    TreeInfo *helper = malloc(sizeof(TreeInfo));
    newsub->tId = id;
    newsub->ttm = tm;
    newsub->tlc = NULL;
    newsub->trc = NULL;
    newsub->next=NULL;
    newsub->prev=NULL;
    newsub->tp =NULL;

    helper->next=NULL;
    helper->prev=NULL;
    helper->tp=NULL;
    helper->tlc = NULL;
    helper->trc = NULL;

    
    TreeInfo *ptr = root;
    
    TreeInfo *tmp=NULL;
    if (root == NULL){
        root = newsub;
        return root;
    }
    while (ptr!=NULL){
        tmp=ptr;
        if (tm>ptr->ttm){
            ptr=ptr->trc;
        }else{
            ptr=ptr->tlc;
        }
    }
  
    newsub->tp=tmp;
    if (tm>tmp->ttm){
        if(tmp->prev!=NULL){
            tmp->prev->next=helper;
        }        
        helper->next=newsub;
        newsub->prev=helper;
        helper->prev=tmp->prev;
        tmp->prev=NULL;
        newsub->next=tmp->next;
        tmp->next=NULL;
        tmp->trc=newsub;
        helper->tId=tmp->tId;
        helper->ttm=tmp->ttm;
        tmp->tlc=helper;
        
        helper->tp=tmp;
    }else{
        newsub->next=helper;
        helper->prev=newsub;
        helper->next=tmp->next;
        if(tmp->prev!=NULL){
            tmp->prev->next=newsub;
        }  
        newsub->prev=tmp->prev;
        tmp->tlc=newsub; 
        helper->ttm=tmp->ttm;
        helper->tId=tmp->tId;
        tmp->trc=helper;
        tmp->next=NULL;
        tmp->prev= NULL;
        helper->tp=tmp;
    }
    return root;
}
TreeInfo* LO_BST_LookUp(TreeInfo* root, int id,int itm){
    TreeInfo* ptr= root;
    if(ptr==NULL){
        return NULL;
    }
    while(isleaf(ptr)==1){
        if(itm>ptr->ttm){
            ptr=ptr->trc;
        }else{
            ptr=ptr->tlc;
        }
    }
    if(ptr->tId==id){
        return ptr;
    }else{
        return NULL;
    }    
}

int isleaf(TreeInfo *node){
    if (node->tlc==NULL && node->trc==NULL){
       return 0;
    }else {
        return 1;
    }
}


void LO_BST_Print(TreeInfo *T){
    TreeInfo *p=T;
    printf("TREELIST = <");
    if(T==NULL){printf(">\n"); return;}
    while(p->tlc!=NULL){p=p->tlc;}
    while(p!=NULL){
        printf("%d,",p->tId);
        p=p->next;
    }
    printf("> ");
}


TreeInfo *FindLast(TreeInfo *head){
    
    TreeInfo *ptr = head;
    while(ptr->trc!=NULL){
        ptr=ptr->trc;
    }

    return ptr;
    

}




