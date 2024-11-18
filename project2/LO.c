#include <stdio.h>
#include <stdlib.h>

typedef struct TreeInfo {
    int tId;
    int ttm;
    struct TreeInfo *tlc;
    struct TreeInfo *trc;
    struct TreeInfo *tp;
    struct TreeInfo *next;
    struct TreeInfo *prev;

}TreeInfo;

TreeInfo *LO_Insert(TreeInfo *root, int id, int tm){
    //TreeInfo *e=LO_BST_LookUp(root,id,tm);
    //if(e!=NULL){return root;}
    TreeInfo *newsub = (TreeInfo*) malloc(sizeof(TreeInfo));
    TreeInfo *helper = malloc(sizeof(TreeInfo));
    newsub->tId = id;
    newsub->ttm = tm;

    newsub->next=NULL;
    newsub->prev=NULL;
    newsub->tp =NULL;

    helper->next=NULL;
    helper->prev=NULL;
    helper->tp=NULL;

    TreeInfo *ptr = root;
    //ptr=root;
    //printf("ptr.ttm%d ",ptr);
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
        //printf("tmp(%d)\n",tmp->tId);
        //tmp->tp->tId=id;
        //printf("s(%d)\n",id);
        //printf("p(%d)\n",tmp->tp->tId);
        //tmp->tp->ttm=tm;
        tmp->trc=helper;
        tmp->next=NULL;
        tmp->prev= NULL;
        helper->tp=tmp;
    }
    return root;
}

void LO_BST_Print(TreeInfo *T){
    TreeInfo *p=T;
    while(p->tlc!=NULL){p=p->tlc;}
    //printf("(%d)\n",p->tId);
    while(p!=NULL){
        printf("%d(%d)\n",p->tId, p->ttm);
        p=p->next;
    }
}

TreeInfo *LO_LookUp(TreeInfo *root,int id,int tm){
    TreeInfo *ptr=root;
    if (ptr==root){return root;}
    while (ptr!=NULL){
        if(ptr->tId==id){return ptr;}
        if (tm>ptr->ttm){
            ptr=ptr->trc;
        }else{
            ptr=ptr->tlc;
        }
    }
}

int main(){
    TreeInfo *kelly=NULL;
    TreeInfo *k;
    kelly=LO_Insert(kelly,32,2070);
    //k=kelly;
    //printf("(%d)\n",k->tId);
    kelly=LO_Insert(kelly,31,2089);
    // kelly=LO_Insert(kelly,11,60);
    //  kelly=LO_Insert(kelly,15,3);
    // kelly=LO_Insert(kelly,155,0);
    // kelly=LO_Insert(kelly,140,4005);
    
    // k=LO_LookUp(kelly,14,33);
    // printf("%d %d\n\n", k->tId,k->ttm);

  
    LO_BST_Print(kelly);
}