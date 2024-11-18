#include<stdio.h>
#include<stdlib.h>
#define ISLEAF(ptr) ptr->tlc== NULL && ptr->trc==NULL

typedef struct TreeInfo {
    int tId;
    int ttm;
    struct TreeInfo *tlc;
    struct TreeInfo *trc;
    struct TreeInfo *tp;
    struct TreeInfo *next;
}TreeInfo;

TreeInfo *BST_Insert(TreeInfo *root, int id, int tm ){
    TreeInfo *new_info=malloc(sizeof(struct TreeInfo));
    new_info->tId=id;
    new_info->ttm=tm;
    new_info->tlc=NULL;
    new_info->trc=NULL;

    TreeInfo *ptr=root;
    TreeInfo *tmp=NULL;

    if(root == NULL){
        root=new_info;
    }else{
        while (ptr!=NULL){
            tmp=ptr;
            if (id>ptr->tId){
                ptr=ptr->trc;
            }else{
                ptr=ptr->tlc;
            }
        }

        new_info->tp=tmp;
        if (id>tmp->tId){
            tmp->trc=new_info;
        }else{
            tmp->tlc=new_info;
        }
    }

    return root;
}

int main(){}