#include <stdio.h>
#include <stdlib.h>

typedef struct Info {
    int iId;
    int itm;
    int igp[64];
    struct Info *ilc;
    struct Info *irc;
    struct Info *ip;
}Info;

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

void BST_Print(Info *root){
    if(root==NULL){
        return;
    }
    BST_Print(root->ilc);
    printf("%d, ", root->iId);
    BST_Print(root->irc);
}


Info *BST_LookUP(Info *root, int id){
    Info *ptr = root;
    while (ptr!=NULL){
        if(ptr->iId==id){return ptr;}
        if (id>ptr->iId){
            ptr=ptr->irc;
        }else{
            ptr=ptr->ilc;
        }
    }
    
} 

int isleaf(Info *node){
    if (node->ilc==NULL && node->irc==NULL){
       return 0;
    }else {
        return 1;
    }
}

Info *BST_Delete(Info *root, int id){
    Info *ptr=BST_LookUP(root, id);
    Info *tmp1=NULL;
    Info *tmp2=NULL;

    if(ptr==root){
        tmp1=ptr->irc;     
        while (tmp1!=NULL){
            tmp2=tmp1;
            tmp1=tmp1->ilc;
        }
        tmp2->ilc=ptr->ilc;
        root=ptr->irc;
        return root;
    }

    if(ptr->ilc==NULL && ptr->irc==NULL){
        ptr->ip->ilc=NULL;
        ptr->ip->irc=NULL;
        ptr->ip=NULL;
        return root;
    }

    if(ptr->ilc==NULL || ptr->irc==NULL){
        if(ptr->ilc!=NULL){ 
            if(ptr->ip->ilc==ptr){
                ptr->ip->ilc=ptr->ilc;
            }else{
                ptr->ip->irc=ptr->ilc;
            }
        }
        else{
            if(ptr->ip->ilc==ptr){
                ptr->ip->ilc=ptr->irc;
                ptr->irc->ip=ptr->ip;
            }else{
                ptr->ip->irc=ptr->irc;
            }
        }
        return root;
    }

    if(ptr->ilc!=NULL && ptr->irc!=NULL){
            if(ptr->ip->ilc==ptr){
                tmp1=ptr->ilc; 
                
                while (tmp1!=NULL){
                    tmp2=tmp1;
                    tmp1=tmp1->irc;
                }
                tmp2->irc=ptr->irc;
                ptr->ip->ilc=ptr->ilc;
            }else{
                tmp1=ptr->irc; 
                
                while (tmp1!=NULL){
                    tmp2=tmp1;
                   tmp1=tmp1->ilc;
                }
                tmp2->ilc=ptr->ilc;
                ptr->ip->irc=ptr->irc;
                
            }
    }

    return root;


}






int main(){
    Info *root,*f;
    root=BST_Insert(root,50,5);
    root=BST_Insert(root,30,6);
    root=BST_Insert(root,60,0);
    root=BST_Insert(root,20,6);
    root=BST_Insert(root,40,7);
    root=BST_Insert(root,55,6);
    root=BST_Insert(root,70,6);
    root=BST_Insert(root,51,6);
    root=BST_Insert(root,57,6);
    root=BST_Insert(root,68,0);
    root=BST_Insert(root,71,0);
    root=BST_Insert(root,69,0);
    root=BST_Insert(root,65,0);
    root=BST_Insert(root,15,0);
    root=BST_Insert(root,25,0);


    
    



    f=BST_LookUP(root,68);
    printf("%d\n",f->iId);
    
    BST_Print(root);
    printf("\n");


    root=BST_Delete(root,50);    

    
    BST_Print(root);
    printf("\n");

}