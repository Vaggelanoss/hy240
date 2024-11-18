#include<stdio.h>
#include<stdlib.h>

typedef struct SubInfo {
    int sId;
    int stm;
    struct TreeInfo *tgp[64];
    struct TreeInfo *sgp[64];
    struct SubInfo *snext;
}SubInfo;

int M=10;
int P=11;

int hashfunction(int id){
    int h = (P + id )% M;
    return h;
}

SubInfo **Hashtable; //= malloc(sizeof(struct SubInfo)*M);

SubInfo *SL_Insert(SubInfo *head, int sId, int Stm) {
    SubInfo *newSub = malloc(sizeof(struct SubInfo));

    int i=0;
    newSub->sId = sId;
    newSub->stm = Stm;

    newSub->snext = NULL;


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

SubInfo Hash_Insert(int sid, int stm){
    Hashtable[hashfunction(sid)]=SL_Insert(Hashtable[hashfunction(sid)],sid,stm);
    
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

void Hash_Delete(SubInfo **table, int sid){
    table[hashfunction(sid)]=SL_Delete(Hashtable[hashfunction(sid)],sid);
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

SubInfo *Hash_LookUp(int sid){
    return SL_LookUp(Hashtable[hashfunction(sid)],sid); 
}

void Hash_print(){
    SubInfo* curr=NULL;
    for(int i=0;i<M;i++){
        printf("HASHTABLE INDEX : %d ", i);
        if(Hashtable[i]!=NULL){
            printf("ELEMENTS: ");
            curr=Hashtable[i];
            while(curr!=NULL){
                printf("%d, ", curr->sId);
                curr=curr->snext;
            }
        }
        printf("\n");
    }
}



int main(){
    Hashtable = (SubInfo**) malloc(M*sizeof(SubInfo*));
    for(int i=0;i<M;i++){
        Hashtable[i]=NULL;
    }
   
    Hash_Insert(10,10000);
    Hash_Insert(12,1000);
    Hash_Insert(22,1000);
    Hash_Delete(Hashtable, 10);
    Hash_print();
    
    printf("\n\n\n");
    
    printf("Did i found him  :  %d\n", Hash_LookUp(12)->stm);
}

