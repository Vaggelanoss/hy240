/***************************************************************
 *
 * file: pss.h
 *
 * @Author  Nikolaos Vasilikopoulos (nvasilik@csd.uoc.gr)
 * @Student Lempesis Evangelos (csd4926@csd.uoc.gr)
 * @Version 20-10-2020
 * @LastEdit 20/11/22
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
SubInfo *S;
/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @return 0 on success
 *         1 on failure
 */
int initialize(void){
    int i=0;
    for (i = 0; i < MG; i++)
    {
        G[i].gfirst=NULL;
        G[i].glast=NULL;
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
    Info *ptr=NULL;
    Info *check=NULL;


    for (i = 0; i < size_of_gids_arr-1; i++)
    {
        G[gids_arr[i]].gfirst=DL_Insert(G[gids_arr[i]].gfirst,iId,iTM,gids_arr); /*inserts a new info to a specific place*/
        G[gids_arr[i]].glast=FindLast(G[gids_arr[i]].gfirst); /*Find last node*/
    }


    for (i = 0; i < size_of_gids_arr-1; i++){
        printf("\tGROUPID = <%d>",gids_arr[i]);
        printf(", INFOLIST = < ");
        DL_print_list(G[gids_arr[i]].gfirst);
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
    S=SL_Insert(S,sId,sTM);
    current=SL_LookUp(S,sId);  /*GIA TO CONSUME*/
    
    
    SubInfo *check;


    
    for (i = 0; i < size_of_gids_arr-1; i++){
        G[gids_arr[i]].ggsub=L_Insert(G[gids_arr[i]].ggsub,sId);
        current->sgp[gids_arr[i]]=G[gids_arr[i]].glast;  /*HOLDING THE OLDEST INFO*/
    }    


    printf("\t");
    SL_print_list(S);
     
   
    for (i = 0; i < size_of_gids_arr-1; i++){
        printf("\tGROUPID = <%d>",gids_arr[i]);
        printf(", SUBLIST = < ");
        print_list(G[gids_arr[i]].ggsub);
        printf(" >\n");
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
    SubInfo *current=NULL;
    current=SL_LookUp(S,sId); 


    for(i=0;i<MG;i++){
        if (current->sgp[i] != (Info *)1) {
            if (current->sgp[i]==NULL){ /*CHECKS IF THERE ARE ANY INFOS IN THIS GROUP AT THIS MOMENT*/
                if (G[i].gfirst==NULL){
                    printf("\tGROUPID = <%d>, INFOLIST = <>,NEWSGP = <>\n",i);
                } else {
                    printf("\tGROUPID = <%d>",i);
                    printf(", INFOLIST = < ");
                    DL_print_listReverse(G[i].glast);
                    printf(" >");
                    current->sgp[i]=G[i].gfirst;  /*UPDATE(AFTER CONSUME HOLDING THE NEWEST INFO)*/
                    printf(",NEWSGP=<%d>\n",current->sgp[i]->iId);

                }
            } else  {
                printf("GROUPID = <%d>", i); 
                if (current->sgp[i]==G[i].gfirst){/*CHECKING IF IS ALREADY UPDATED*/
                    printf(",INFOLIST = < >");
                    printf(",NEWSGP=<%d>\n",current->sgp[i]->iId);

                }else {
                DL_print_listReverse(current->sgp[i]);
                current->sgp[i]=G[i].gfirst;
                printf(",NEWSGP=<%d>\n",current->sgp[i]->iId);

                }
            }
        }
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
    SubInfo *ptr1=NULL;   /*global list*/
    Subscription *ptr2=NULL;
    int arr[MG]={0};   /*THIS IS INITIALIZED WITH ZERO*/

    int i;
    ptr1=SL_LookUp(S,sId);  /*HOLDING IN A PTR THE SUB WE WANT TO DELETE*/
    if(ptr1==NULL){
        printf("!!!NO SUBSCRIBER WITH THIS ID!!!!\n");
        return EXIT_FAILURE;
    }

    for ( i = 0; i < MG; i++)
    {
        if(ptr1->sgp[i]!=(Info*)1){  /*IM NOT LOOKING FOR IT IN EVERY GROUP */
            G[i].ggsub=L_Delete(G[i].ggsub,sId);
            arr[i]=1;
        }    
    }
    S=SL_Delete(S,sId);
    printf("\t");
    SL_print_list(S);
     
   
    for (i = 0; i < MG; i++){
        if(arr[i]==1){
            printf("\tGROUPID = <%d>",i);
            printf(", SUBLIST = < ");
            print_list(G[i].ggsub);
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
    int i,c1=0,c2=0;

    for ( i = 0; i < MG; i++){
        
        if (G[i].gfirst!=NULL){
            c1++;
        }

        printf("\tGROUPID = <%d>",i);
        printf(", INFOLIST = < ");
        DL_print_list(G[i].gfirst);
        printf(" >, SUBLIST = < ");
        print_list(G[i].ggsub);
        printf(" >\n");
    }

    printf("\t");
    SL_print_list(S);
    SubInfo *Stmp=S;
    

    while (Stmp!=NULL){

        printf("\tSUBSCRIBERID = <%d>",Stmp->sId);
        printf(", GROUPLIST = < ");
        for (i = 0; i < MG; i++){
            if (Stmp->sgp[i]!=(Info*)1){
                printf("%d, ",i);
            }
        }
        
        printf(" >\n");
        
        Stmp=Stmp->snext;
        c2++;
    }

    printf("\tNO_GROUPS = <%d>, NO_SUBSCRIBERS = <%d>\n",c1,c2);
    

    return EXIT_SUCCESS;
}





/*========SL===========*/

SubInfo *SL_Insert(SubInfo *head, int sId, int Stm) {
    SubInfo *newSub = malloc(sizeof(struct SubInfo));

    int i=0;
    newSub->sId = sId;
    newSub->stm = Stm;

    newSub->snext = NULL;

    
    for(i=0;i<MG;i++){
        newSub->sgp[i]=(Info*) 1;
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



void SL_print_list(SubInfo *list) {
    SubInfo *ptr = list;
    printf("SUBSCRIBERLIST = <");
    while (ptr != NULL) {
        printf("%d, ", ptr->sId);
        ptr = ptr->snext;
    }
    printf(" >\n");
}



/*=================DL===================*/


Info *head= NULL;


Info *DL_Insert(Info *head,int iId,int itm, int* p){
    Info *new_info=malloc(sizeof(struct Info));
    int i=0;
    new_info->iId=iId;
    new_info->itm=itm;

    new_info->inext=NULL;
    new_info->iprev=NULL;

    for ( i = 0; i <MG; i++){
        new_info->igp[i]=0;
    }

    for (i = 0; p[i]!=-1; i++){
        
        new_info->igp[p[i]]=1;

    }
    
      

    if (head == NULL) { 
        head=new_info;
        return head;
    } else{
        Info *ptr=head;
        while (ptr->itm>itm){
            if (ptr->inext==NULL){
                ptr->inext=new_info;
                new_info->iprev=ptr;
                return head;
            }
            
            ptr=ptr->inext;

        }

        if (ptr->iprev!= NULL){
            ptr->iprev->inext=new_info;
            new_info->iprev=ptr->iprev;
            ptr->iprev=new_info;
            new_info->inext=ptr;

        }else{
            ptr->iprev=new_info;
            new_info->inext=ptr;
            new_info->iprev=NULL;
            head=new_info;

        }
        
    }

    return head;
}


Info *DL_Delete(Info *head,int iId){
    Info *ptr=head;
     

    while (ptr->inext!=NULL && ptr->iId!=iId)
    {
        ptr=ptr->inext;
    }

    if (ptr->iprev==NULL)
    {
        head=ptr->inext;
        return head;
    }
    
    if (ptr->inext!=NULL){
        ptr->iprev->inext=ptr->inext;
        ptr->inext->iprev=ptr->iprev;
    }
    
    else {
        ptr->iprev->inext=NULL;
    }
    free(ptr);
    return head;
    
}


void DL_print_list(Info *list) {
    Info *ptr = list;
    int i;
    while (ptr != NULL) {
        printf(" %d, ", ptr->iId);
        ptr = ptr->inext;

    }
}


void DL_print_listReverse(Info *last) {
    Info *ptr = last;
    if (ptr==NULL)
    {
       return;
    }
    
    int i;
    while (ptr != NULL) {
 
        printf("%d, ",ptr->iId);
        
        
        ptr = ptr->iprev;

    }
}


Info *DL_LookUp(Info *head, int id){
    
    Info *ptr = head;
    while (ptr->inext!=NULL ){
        if (ptr->iId==id){
            return ptr;
        }
        ptr=ptr->inext;
    }

    return NULL;
    

}


Info *FindLast(Info *head){
    
    Info *ptr = head;
    while(ptr->inext!=NULL){
        ptr=ptr->inext;
    }

    return ptr;
    

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
