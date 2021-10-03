#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"header.h"

void initialise_dbs(bk_tree_node **head,member_tree_node **faculty,member_tree_node **student,member_tree_node **fine_tree,member_tree_node **borrow_tree_root)
{
    *head=NULL;
    *student=NULL;
    *faculty=NULL;
    *fine_tree=NULL;
    *borrow_tree_root=NULL;
}


/*---------------------------------------------------- INSERTION ALGORITHM FOR B TREE OF BOOKS----------------------------------------------------------*/

//function to split a node of book tree node
void split_bktree_node(bk_tree_node* node_to_split,bk_tree_node** splitted_node,struct book_detail** splitted_book)
{
    (*splitted_node)=(bk_tree_node*)malloc(sizeof(bk_tree_node));
    (*splitted_node)->leaf=node_to_split->leaf;
    (*splitted_node)->size=ORDER-ORDER/2-1;  //fixing size on new node
    node_to_split->size=ORDER/2;
    int i,j;
    j=ORDER/2+1;
    for(i=0;i+j<ORDER;i++)
    {
        (*splitted_node)->books[i]=node_to_split->books[i+j];
    }
    if(!node_to_split->leaf)
    {
        for(i=0;i+j<=ORDER;i++)
        {
            (*splitted_node)->children[i]=node_to_split->children[i+j];
        }
    }
    *splitted_book=node_to_split->books[ORDER/2];
}

bk_tree_node* insert_bktree(bk_tree_node *root,struct book_detail* bk) 
{
    bk_tree_node* splitted_node=NULL;
    struct book_detail* splitted_book=NULL;
    if(root==NULL)
    {
        root=(bk_tree_node*)malloc(sizeof(bk_tree_node));
        root->size=1;
        root->leaf=true;
        root->books[0]=bk;
    }
    else
    {
        insert_into_bktree(root,&splitted_node,&splitted_book,bk);
        if(splitted_node!=NULL)
        {
            bk_tree_node* new_root=(bk_tree_node*)malloc(sizeof(bk_tree_node));
            new_root->leaf=false;
            new_root->size=1;
            new_root->books[0]=splitted_book;
            new_root->children[0]=root;
            new_root->children[1]=splitted_node;
            root=new_root;
        }
    }
    return root;  
}
void insert_into_bktree(bk_tree_node *node,bk_tree_node** splitted_node,struct book_detail** splitted_book,struct book_detail* bk)
{
    int i,j;
    if(node->leaf)
    {
        i=node->size-1;
        while(i>=0 && strcmp(bk->key,node->books[i]->key)<0)
        {
            node->books[i+1]=node->books[i];
            i--;
        }
        node->books[i+1]=bk;
        node->size+=1;
        if(node->size==ORDER)
        {
            split_bktree_node(node,splitted_node,splitted_book);
        }
    }
    else
    {
        i=0;
        while(i<node->size && strcmp(node->books[i]->key,bk->key)<0)
        {
            i++;
        }
        insert_into_bktree(node->children[i],splitted_node,splitted_book,bk);
        if((*splitted_node)==NULL) //do not do anything if there was no splitting in its children node
        {
            return;
        }
        for(j=i;j<node->size;j++)
        {
            node->books[j+1]=node->books[j];
        }
        node->books[i]=*splitted_book;
        for(j=i+1;j<=node->size;j++)
        {
            node->children[j+1]=node->children[j];
        }
        node->children[i+1]=*splitted_node;
        *splitted_node=NULL;
        node->size+=1;
        if(node->size==ORDER)   //split if it exceeds the capacity
        {
            split_bktree_node(node,splitted_node,splitted_book);
        }
    }
}


/*---------------------------------------------------- END OF INSERTION ALGORITHM FOR B TREE OF BOOKS ----------------------------------------------------------*/

//function to make key for each book
void make_book_key(struct book_detail* bk)
{
    char str[30];
    strcat(bk->key,bk->subject);
    strcat(bk->key,bk->title);
    for(int i=0;i<bk->no_of_authors;i++)
    {
        strcat(bk->key,bk->authors[i]);
    }
    sprintf(str,"%d",bk->print_year);
    strcat(bk->key,str);
}
//Function to insert books into data base of books in given order
bk_tree_node* insertbook(bk_tree_node *root) 
{
    int j,found;
    char t;
    struct book_detail* new_book=(struct book_detail*)malloc(sizeof(struct book_detail));
    printf("\t\t\t\tBOOK INSERTION");
    printf("\n\nENTER THE TITLE:");
    gets(new_book->title);   /*USING TWO gets(),because when i put one gets(),
                        it does not allow me to enter data
                        instead it takes value from input buffer*/
    gets(new_book->title); 
    printf("\nENTER THE SUBJECT:");
    gets(new_book->subject);
    printf("\nENTER THE NUMBER OF AUTHORS:");
    scanf("%d",&new_book->no_of_authors);
    printf("\nENTER THE SURNAMES OF AUTHORS:");
    for(j=0;j<new_book->no_of_authors;j++)
    {
       scanf("%s",new_book->authors[j]);
    }
    new_book->status=1;
    printf("\nENTER THE ACCESSION NUMBER:");
    scanf("%d",&new_book->accno);
    printf("\nENTER THE YEAR OF PRINT:");
    scanf("%d",&new_book->print_year);
    printf("\nENTER THE LOCATION OF BOOK:");
    scanf("%d %d %d",&new_book->hall,&new_book->rack,&new_book->row);
    new_book->key[0]='\0';
    make_book_key(new_book);
    root=insert_bktree(root,new_book);
    printf("\nBOOK SUCCESSFULLY INSERTED(press any key to continue)");
    scanf("%c",&t);
    scanf("%c",&t);
    return root;
}
//function to display details of the bool
void displaybook(struct book_detail* book)  
{       
    int j;
    printf("\nSUBJECT:%s",book->subject);
    printf("\nTITLE:%s",book->title);
    printf("\nAUTHORS:");
    for(j=0;j<book->no_of_authors;j++)
    {
        printf("%s,",book->authors[j]);
    }
    printf("\nACCESSION NUMBER:%d",book->accno);
    printf("\nYEAR OF PRINT:%d",book->print_year);
    printf("\nHALL:%d RACK:%d ROW:%d",book->hall,book->rack,book->row);
    if(book->status==1)
    {
       printf("\nSTATUS:AVAILABALE\n\n");
    }
    else
    {
       printf("\nSTATUS:NOT AVAILABLE\n\n");
    }
}
//function to search for book by title 
void search_by_title(bk_tree_node* root,char title[title_size])
{
    int i;
    for(i=0;i<root->size;i++)
    {
        if(strcmp(root->books[i]->title,title)==0)
        {
            displaybook(root->books[i]);
        }
        if(!root->leaf)
        {
            search_by_title(root->children[i],title);
        }
    }
    if(!root->leaf)
    {
        search_by_title(root->children[i],title);
    }
}

//function to search for book by author name
void search_by_author(bk_tree_node* root,char str[author_name_size])
{
    int i;
    bool found;
    for(i=0;i<root->size;i++)
    {
        found=false;
        for(int j=0;j<root->books[i]->no_of_authors && !found;j++)
        {
            if(strcmp(root->books[i]->authors[j],str)==0)
            {
               displaybook(root->books[i]);
               found=true;
            }
        }
        if(!root->leaf)
        {
            search_by_author(root->children[i],str);
        }
    }
    if(!root->leaf)
    {
        search_by_author(root->children[i],str);
    }
}
// Function do search for a book either by author name or by title string
void searchbook(bk_tree_node *root)
{
    char choice,str[title_size];
    int i,j;
    char t;
    struct book_detail *ptr;
    printf("\t\t\tBOOK SEARCH");
    printf("\n\nA-Search by author name\nT-Search by title string\nENTER YOUR CHOICE:");
    scanf("%c",&choice);
    scanf("%c",&choice);
    switch(choice)
    {
       case 'T' :{
                    printf("\nENTER THE TITLE STRING:");
                    gets(str);
                    gets(str);
                    printf("BOOKS ARE :\n");
                    search_by_title(root,str);
                    break;
                 }
       case 'A' :{
                    printf("\nENTER AUTHOR NAME:");
                    scanf("%s",str);
                    printf("BOOKS ARE :\n");
                    search_by_author(root,str);
                    break;
                 }
      default  :{
                    printf("\nINVALID CHOICE");
                    break;
                }
    }
    printf("\n\npress any key to continue");
    scanf("%c",&t); 
    scanf("%c",&t); 
}


/*---------------------------------------------------- INSERTION ALGORITHM FOR B TREE OF MEMBERS ----------------------------------------------------------*/

//function to split a member tree node
void split_memtree_node(member_tree_node* node_to_split,member_tree_node** splitted_node,struct member_detail** splitted_member)
{
    (*splitted_node)=(member_tree_node*)malloc(sizeof(member_tree_node));
    (*splitted_node)->leaf=node_to_split->leaf;
    (*splitted_node)->size=ORDER-ORDER/2-1;  //fixing size on new node
    node_to_split->size=ORDER/2;
    int i,j;
    j=ORDER/2+1;
    for(i=0;i+j<ORDER;i++)
    {
        (*splitted_node)->members[i]=node_to_split->members[i+j];
    }
    if(!node_to_split->leaf)
    {
        for(i=0;i+j<=ORDER;i++)
        {
            (*splitted_node)->children[i]=node_to_split->children[i+j];
        }
    }
    *splitted_member=node_to_split->members[ORDER/2];
}

member_tree_node* insert_memtree(member_tree_node *root,struct member_detail* mem) 
{
    member_tree_node* splitted_node=NULL;
    struct member_detail* splitted_member=NULL;
    if(root==NULL)
    {
        root=(member_tree_node*)malloc(sizeof(member_tree_node));
        root->size=1;
        root->leaf=true;
        root->members[0]=mem;
    }
    else
    {
        insert_into_memtree(root,&splitted_node,&splitted_member,mem);
        if(splitted_node!=NULL)
        {
            member_tree_node* new_root=(member_tree_node*)malloc(sizeof(member_tree_node));
            new_root->leaf=false;
            new_root->size=1;
            new_root->members[0]=splitted_member;
            new_root->children[0]=root;
            new_root->children[1]=splitted_node;
            root=new_root;
        }
    }
    return root;  
}

void insert_into_memtree(member_tree_node *node,member_tree_node** splitted_node,struct member_detail** splitted_member,struct member_detail* mem)
{
    int i,j;
    if(node->leaf)
    {
        i=node->size-1;
        while(i>=0 && strcmp(mem->key,node->members[i]->key)<0)
        {
            node->members[i+1]=node->members[i];
            i--;
        }
        node->members[i+1]=mem;
        node->size+=1;
        if(node->size==ORDER)
        {
            split_memtree_node(node,splitted_node,splitted_member);
        }
    }
    else
    {
        i=0;
        while(i<node->size && strcmp(node->members[i]->key,mem->key)<0)
        {
            i++;
        }
        insert_into_memtree(node->children[i],splitted_node,splitted_member,mem);
        if((*splitted_node)==NULL) //do not do anything if there was no splitting in its children node
        {
            return;
        }
        for(j=i;j<node->size;j++)
        {
            node->members[j+1]=node->members[j];
        }
        node->members[i]=*splitted_member;
        for(j=i+1;j<=node->size;j++)
        {
            node->children[j+1]=node->children[j];
        }
        node->children[i+1]=*splitted_node;
        *splitted_node=NULL;
        *splitted_member=NULL;
        node->size+=1;
        if(node->size==ORDER)   //split if it exceeds the capacity
        {
            split_memtree_node(node,splitted_node,splitted_member);
        }
    }
}


/*---------------------------------------------------- END OF INSERTION ALGORITHM FOR B TREE OF MEMBERS ----------------------------------------------------------*/

member_tree_node* appendmember(member_tree_node *root,member_tree_node **borrow_tree_root) 
{
   char t;
   struct member_detail *new_member=(struct member_detail*)malloc(sizeof(struct member_detail));
   if(new_member!=NULL)
   {
       printf("\nENTER THE NAME:");
       gets(new_member->name);    //two gets() function due to same reason mentioned in line 116
       gets(new_member->name);
       printf("\nENTER THE DEPARTMENT:");
       gets(new_member->dept);
       printf("\nENTER THE DESIGNATION:");
       scanf("%s",new_member->desgn);
       printf("\nENTER ID:");
       scanf("%d",&new_member->id);
       new_member->fine=0.0;
       new_member->no_of_issues=0;
       new_member->no_of_fines=0;
       new_member->book_lists=NULL;
       new_member->key[0]='\0';
       new_member->alt_key[0]='\0';
       new_member->alt_key2[0]='\0';
       make_member_key(new_member);
       make_member_alt_key2(new_member);
       root=insert_memtree(root,new_member);
       *borrow_tree_root=insert_memtree3(*borrow_tree_root,new_member);
       printf("\nMEMBER ADDED SUCCESFULLY(press any key to continue)");
       scanf("%c",&t);
       scanf("%c",&t); 
   }
   else
   {
       printf("\nDATABASE IS FULL(press any key to continue)");
       scanf("%c",&t);
       scanf("%c",&t); 
   }
   return root;
}

int check_leap(int year)
{
   int retvalue=0;
   if((year%4==0 && year%100!=0 )|| year%400==0)
   {
      retvalue=1;
   }
   return retvalue;
}

// This function will return a specific date after given number of days from the given date.
struct date setdate(int days,struct date today) 
{
   int rem,retvalue;
   struct date retdate;
   int calendar[12]={31,28,31,30,31,30,31,31,30,31,30,31};
   retvalue=check_leap(today.y);
   if(retvalue==1)
   {
      calendar[1]=29;
   }
   while((today.d+days)>calendar[today.m-1])
   {
      rem=calendar[today.m-1]-today.d+1;
      days=days-rem;
      today.m++;
      today.d=1;
      if(today.m==13)
      {
         today.m=1;
         today.y++;
         today.d=1;
         retvalue=check_leap(today.y);
         if(retvalue==1)
         {
            calendar[1]=29;
         }
         else
         {
            calendar[1]=28;
         }
      }
   }
   retdate.d=today.d+days;
   retdate.m=today.m;
   retdate.y=today.y;
   return retdate;
}

//function to make alt_key2 for member
void make_member_alt_key2(struct member_detail *mem)
{
    char str[20];
    sprintf(str,"%d",mem->no_of_issues);
    strcat(mem->alt_key2,str);
    if(strcmp(mem->desgn,"student")==0)
    {
        strcat(mem->alt_key2,"a");  //since student is coming below faculty if number of issues are same
    }
    else
    {
        strcat(mem->alt_key2,"b");
    }
    strcat(mem->alt_key2,mem->name);
}

//function to make key for member
void make_member_key(struct member_detail *mem)
{
    char str[20];
    strcat(mem->key,mem->dept);
    sprintf(str,"%d",mem->id);
    strcat(mem->key,str);
}

//function to search and get the data of a member using key
struct member_detail* search_member_by_key(member_tree_node* root,char key[key_size])
{
    struct member_detail* ret=NULL;
    int i=0;
    while(i<root->size && strcmp(root->members[i]->key,key)<0)
    {
        i++;
    }
    if(i<root->size  && strcmp(root->members[i]->key,key)==0)
    {
        ret=root->members[i];
    }
    else if(!root->leaf)
    {
        ret=search_member_by_key(root->children[i],key);
    }
    return ret;
}

//function to search and get the data of a book using key
struct book_detail* search_book_by_key(bk_tree_node* root,char key[key_size])
{
    struct book_detail* ret=NULL;
    int i=0;
    while(i<root->size && strcmp(root->books[i]->key,key)<0)
    {
        i++;
    }
    if(i<root->size  && strcmp(root->books[i]->key,key)==0)
    {
        ret=root->books[i];
    }
    else if(!root->leaf)
    {
        ret=search_book_by_key(root->children[i],key);
    }
    return ret;
}

// Function used to issue a book by scanning the accesion number of the book
//make sure last is not null
void issuebook(bk_tree_node *book_root,member_tree_node *mem_root,int maxissue,member_tree_node **borrow_tree_root)
{
    int id,k,n;
    struct date today;
    char t;
    char dept[dept_size];
    char title[title_size],subject[subject_size];
    int print_year;
    char authors[author_name_size][author_name_size];
    char str[20];
    char key[key_size]={0};
    struct member_detail* mem=NULL;
    struct book_detail* bk=NULL;
    printf("\nENTER ID:");
    scanf("%d",&id);
    printf("\nENTER DEPARTMENT:");
    gets(dept);
    gets(dept);
    strcat(key,dept);
    sprintf(str,"%d",id);
    strcat(key,str);
    mem=search_member_by_key(mem_root,key);
    if(mem!=NULL)
    {  
        if(mem->no_of_issues < maxissue)
        {
            printf("\nENTER THE SUBJECT:");
            gets(subject);
            printf("\nENTER THE TITLE:");
            gets(title); 
            printf("\nENTER THE NUMBER OF AUTHORS:");
            scanf("%d",&n);
            printf("\nENTER THE SURNAMES OF AUTHORS:");
            for(int j=0;j<n;j++)
            {
               scanf("%s",authors[j]);
            }
            printf("\nENTER THE YEAR OF PRINT:");
            scanf("%d",&print_year);
            key[0]='\0';
            strcat(key,subject);
            strcat(key,title);
            for(int j=0;j<n;j++)
            {
               strcat(key,authors[j]);
            }
            sprintf(str,"%d",print_year);
            strcat(key,str);
            bk=search_book_by_key(book_root,key);
            if(bk!=NULL)
            {
                printf("\nENTER TODAY'S DATE(DD/MM/YYYY):");
                scanf("%d %d %d",&today.d,&today.m,&today.y);
                struct book_issued *newissue=(struct book_issued*)malloc(sizeof(struct book_issued));
                newissue->issue_date=today;
                newissue->bk=bk;
                newissue->renew_count=3;
                if(strcmp(mem->desgn,"faculty")==0)
                {
                    newissue->exp_date=setdate(60,today);
                }
                else
                {
                    newissue->exp_date=setdate(14,today);
                }
                *borrow_tree_root=deletion2(*borrow_tree_root,mem->alt_key2);
                mem->no_of_issues++;
                mem->alt_key2[0]='\0';
                make_member_alt_key2(mem);
                *borrow_tree_root=insert_memtree3(*borrow_tree_root,mem);
                bk->status=0;
                if(mem->book_lists==NULL)
                {
                    mem->book_lists=newissue;
                    newissue->next=newissue;
                }
                else
                {
                    newissue->next=mem->book_lists->next;
                    mem->book_lists->next=newissue;
                    mem->book_lists=newissue;
                }
                k=1;
                printf("\nLIST OF BOOKS CURRENTLY ISSUED:");
                printf("\nS.NO\tACCNO.\tTITLE\t\tISSUE DATE\tEXPIRY DATE\n");
                struct book_issued *ptr3=mem->book_lists->next;
                printf("%d\t%d\t%s\t\t%d-%d-%d\t%d-%d-%d\n",k,ptr3->bk->accno,ptr3->bk->title,ptr3->issue_date.d,ptr3->issue_date.m,ptr3->issue_date.y,ptr3->exp_date.d,ptr3->exp_date.m,ptr3->exp_date.y);
                ptr3=ptr3->next;
                while(ptr3!=mem->book_lists->next)
                {
                    k++;
                    printf("%d\t%d\t%s\t\t%d-%d-%d\t%d-%d-%d\n",k,ptr3->bk->accno,ptr3->bk->title,ptr3->issue_date.d,ptr3->issue_date.m,ptr3->issue_date.y,ptr3->exp_date.d,ptr3->exp_date.m,ptr3->exp_date.y);
                    ptr3=ptr3->next;
                }
            }
            else
            {
                printf("\nBOOK NOT AVAILABLE");
            }
         }
         else   
         {
             printf("\nCANNNOT ISSUE MORE BOOKS");
         }
      }
      else
      {
         printf("\nMEMBER NOT FOUND");
      }
      printf("\n\npress any key to continue");
      scanf("%c",&t); 
      scanf("%c",&t);
}

// Function used to calculate fine amd return which uses date of return/reissue and expiry date
float calculatefine(struct date today,struct date exp_date)
{
   int calendar[12]={31,28,31,30,31,30,31,31,30,31,30,31};
   int no_of_days1=0,no_of_days2=0;
   int no_of_leapyears,i,diff;
   float retvalue;
   no_of_days1+=today.d+365*today.y;
   if(today.m>2)
   {
      no_of_leapyears=today.y/4-today.y/100+today.y/400;
   }
   else
   {
      today.y--;
      no_of_leapyears=today.y/4-today.y/100+today.y/400;
   }
   no_of_days1+=no_of_leapyears;
   for(i=0;i<today.m-1;i++)
   {
      no_of_days1+=calendar[i];
   }
   no_of_days2+=exp_date.d+exp_date.y*365;
   if(exp_date.m>2)
   {
      no_of_leapyears=exp_date.y/4-exp_date.y/100+exp_date.y/400;
   }
   else
   {
      exp_date.y--;
      no_of_leapyears=exp_date.y/4-exp_date.y/100+exp_date.y/400;
   }
   no_of_days2+=no_of_leapyears;
   for(i=0;i<exp_date.m-1;i++)
   {
      no_of_days2+=calendar[i];
   }
   diff=no_of_days1-no_of_days2;
   if(diff>0)
   {
      retvalue=0.5*diff;
   }
   else
   {
      retvalue=0.0;
   }
   return retvalue;
}

/*Function used when user wants to return a book
 it calls calculatefine() to add fine if there is any */   //make sure last is not null
void returnbook(member_tree_node *root,member_tree_node **fine_tree_root,member_tree_node **borrow_tree_root)
{   
    int id,acc,found;
    struct date today;
    char t;
    float fine;
    char dept[dept_size];
    char key[key_size]={0};
    char str[30]={0};
    struct member_detail *mem=NULL;
    printf("\nENTER ID:");
    scanf("%d",&id);
    printf("\nENTER DEPARTMENT:");
    gets(dept);
    gets(dept);
    strcat(key,dept);
    sprintf(str,"%d",id);
    strcat(key,str);
    mem=search_member_by_key(root,key);
    if(mem!=NULL)
    {
       printf("\nENTER ACCESSION NUMBER OF THE BOOK:");
       scanf("%d",&acc);
       struct book_issued *ptr3=mem->book_lists->next;
       struct book_issued *prev=NULL;
       found=0;
       while( ptr3!=mem->book_lists && !found)
       {
            if(ptr3->bk->accno==acc)
            {
               found=1;
            }
            else
            {
               prev=ptr3;
               ptr3=ptr3->next;
            }
       }
       if(ptr3->bk->accno==acc)
       {
            found=1;
       }   
       if(found==1)
       {   
           printf("\nENTER TODAY'S DATE(DD/MM/YYYY):");
           scanf("%d %d %d",&today.d,&today.m,&today.y);
           fine=calculatefine(today,ptr3->exp_date);
           /*Here if the fine is not zero then we add a new data into the fine tree with key being(no: of fines+member id)
           we also delete the previous record of the same user if it was presnet.
           For eg:Initially user has no fine,after he return or renew a book beyonf the expiry date,  a fine is added and 
           number of fines is incremented to one.now we insert details of this member into th fine tree with key being(no: of fines+member id)
           Later on each time he gets fined,we delete the previous record and add new record with updated value of number of fines */
           if(fine)
           {
                if(mem->no_of_fines!=0)
                {
                    *fine_tree_root=deletion(*fine_tree_root,mem->alt_key);
                }
                mem->no_of_fines++;
                sprintf(str,"%d",mem->no_of_fines);
                mem->alt_key[0]='\0';
                strcat(mem->alt_key,str);
                sprintf(str,"%d",mem->id);
                strcat(mem->alt_key,str);
                *fine_tree_root=insert_memtree2(*fine_tree_root,mem);
           }
           mem->fine+=fine;
           if(mem->fine>max_fine)
           {
               max_fine=mem->fine;
           }
           *borrow_tree_root=deletion2(*borrow_tree_root,mem->alt_key2);
           mem->no_of_issues--;
           mem->alt_key2[0]='\0';
           make_member_alt_key2(mem);
           *borrow_tree_root=insert_memtree3(*borrow_tree_root,mem);
           ptr3->bk->status=1;
           if(ptr3==mem->book_lists)
           {
               if(prev)
               {
                   prev->next=ptr3->next;
               }
               mem->book_lists=prev;
           }
           else if(ptr3==mem->book_lists->next)
           {
               mem->book_lists->next=ptr3->next;
           }
           else
           {
               prev->next=ptr3->next;
           }
           free(ptr3);
           ptr3=NULL;
           printf("\nBOOK SUCCESFULLY RETURNED");
        }
        else
        {
            printf("\nTHIS BOOKS HAS NOT BEEN ISSUED");
        }
    }
    else
    { 
       printf("\nMEMBER NOT FOUND");
    }
    printf("\npress any key to continue");
    scanf("%c",&t);
    scanf("%c",&t);
}

/* Function called when user wants to renew his/her book further
   it calcultes if there is any fine and ensures that user does not exceed the maximum number 
   of times he/she can renew*/
void renewbook(member_tree_node *root ,member_tree_node **fine_tree_root)
{  
   int k,found,id,acc;
   char t;
   struct date today;
   char dept[dept_size];
   char key[key_size]={0};
   char str[30]={0};
   float fine;
   struct member_detail *mem=NULL;
   printf("\nENTER ID:");
   scanf("%d",&id);
   printf("\nENTER DEPARTMENT:");
   gets(dept);
   gets(dept);
   strcat(key,dept);
   sprintf(str,"%d",id);
   strcat(key,str);
   mem=search_member_by_key(root,key);
   if(mem!=NULL)
   {
      printf("\nENTER THE ACCESSION NUMBER:");
      scanf("%d",&acc);
      found=0;
      struct book_issued *ptr2=mem->book_lists->next;
      while(ptr2 !=mem->book_lists && !found)
      {
        if(ptr2->bk->accno==acc)
        {
           found=1;
        }
        else
        {
           ptr2=ptr2->next;
        }
      }
      if(ptr2->bk->accno==acc)
      {
        found=1;
      }
      if(found==1)
      {
        printf("\nENTER TODAY'S DATE:(DD/MM/YYYY):");
        scanf("%d %d %d",&today.d,&today.m,&today.y);
        if(--(ptr2->renew_count)>=0)
        {
            fine=calculatefine(today,ptr2->exp_date);
            /*Here if the fine is not zero then we add a new data into the fine tree with key being(no: of fines+member id)
            we also delete the previous record of the same user if it was presnet.
            For eg:Initially user has no fine,after he return or renew a book beyonf the expiry date,  a fine is added and 
            number of fines is incremented to one.now we insert details of this member into th fine tree with key being(no: of fines+member id)
            Later on each time he gets fined,we delete the previous record and add new record with updated value of number of fines */
            if(fine)
            {
                if(mem->no_of_fines!=0)
                {
                    *fine_tree_root=deletion(*fine_tree_root,mem->alt_key);
                }
                mem->no_of_fines++;
                sprintf(str,"%d",mem->no_of_fines);
                mem->alt_key[0]='\0';
                strcat(mem->alt_key,str);
                sprintf(str,"%d",mem->id);
                strcat(mem->alt_key,str);
                *fine_tree_root=insert_memtree2(*fine_tree_root,mem);
            }
            mem->fine+=fine;
            if(mem->fine>max_fine)
            {
                max_fine=mem->fine;
            }
            ptr2->issue_date=today;
            if(strcmp(mem->desgn,"faculty")==0)
            {
               ptr2->exp_date=setdate(60,today);
            }
            else
            {
               ptr2->exp_date=setdate(14,today);
            }
            k=1;
            printf("\nLIST OF BOOKS CURRENTLY ISSUED:");
            printf("\nS.NO\tACCNO.\tTITLE\t\tISSUE DATE\tEXPIRY DATE\n");
            struct book_issued *ptr3=mem->book_lists->next;
            printf("%d\t%d\t%s\t\t%d-%d-%d\t%d-%d-%d\n",k,ptr3->bk->accno,ptr3->bk->title,ptr3->issue_date.d,ptr3->issue_date.m,ptr3->issue_date.y,ptr3->exp_date.d,ptr3->exp_date.m,ptr3->exp_date.y);
            ptr3=ptr3->next;
            while(ptr3!=mem->book_lists->next)
            {
                k++;
                printf("%d\t%d\t%s\t\t%d-%d-%d\t%d-%d-%d\n",k,ptr3->bk->accno,ptr3->bk->title,ptr3->issue_date.d,ptr3->issue_date.m,ptr3->issue_date.y,ptr3->exp_date.d,ptr3->exp_date.m,ptr3->exp_date.y);
                ptr3=ptr3->next;
            }
             printf("\nRENEW SUCCESFULL");
        }
        else
        {
           printf("\nRENEW LIMIT REACHED");
        }
      }
      else
      {
        printf("\nTHIS BOOK HAS NOT BEEN ISSUED");  
      }
   }
   else
   {  
      printf("\nMEMBER DOES NOT EXIST");
   }
   printf("\npress any key to continue");
   scanf("%c",&t);
   scanf("%c",&t);
}

/* Solution to subdivision v 
   This function will display fine of a particular member by entering his/her id
   This function will also display names of members who currently have maximum fines */
void checkfine_and_displaymaxfine(member_tree_node *student,member_tree_node *faculty)
{
   printf("\t\t\tDISPLAY FINE");
   int id;
   char ch;
   char dept[dept_size];
   char key[key_size]={0};
   char str[30]={0};
   struct member_detail *mem=NULL;
   printf("\nENTER ID:");
   scanf("%d",&id);
   printf("\nENTER DEPARTMENT:");
   gets(dept);
   gets(dept);
   strcat(key,dept);
   sprintf(str,"%d",id);
   strcat(key,str);
   printf("\nSELECT TYPE OF MEMBER:\nF-FACULTY\nS-STUDENT\n");
   scanf("%c",&ch);
   switch(ch)     
   {
       case 'F' : {  
                    if(faculty)
                    {
                        mem=search_member_by_key(faculty,key);
                        if(mem!=NULL)
                        {
                           printf("\nNAME:%s\nID:%d\nFINE:%f\n",mem->name,id,mem->fine);
                        }
                        else
                        {
                           printf("\nMEMBER DOES NOT EXIST");
                        }
                    }
                    else
                    {
                        printf("\nNo Faculty member");
                    }
                    break;
                  }
       case 'S' : {
                    if(student)
                    {
                        mem=search_member_by_key(student,key);
                        if(mem!=NULL)
                        {
                            printf("\nNAME:%s\nID:%d\nFINE:%f\n",mem->name,id,mem->fine);
                        }
                        else
                        {
                            printf("\nMEMBER DOES NOT EXIST");
                        }
                    }
                    else
                    {
                        printf("\nNo Student member");
                    }
                    break;
                  }
                          
       default :  {
                    printf("\nINVALID CHOICE");
                    break;
                  }
   }
   printf("\nMEMBERS HAVING MAXIMUM FINE ARE:");
   if(student)
   {
       search_member_by_fine(student,max_fine);
   }
   if(faculty)
   {
       search_member_by_fine(faculty,max_fine);
   }
   printf("\npress any key to continue");
   scanf("%c",&ch);
   scanf("%c",&ch);
}

//function to display details of the member
void display_member(struct member_detail* mem)
{
    printf("\nNAME:%s",mem->name);
    printf("\nID:%d",mem->id);
    printf("\nDEPARTMENT:%s",mem->dept);
    printf("\nDESIGNATION:%s",mem->desgn);
    printf("\nFINE:%f",mem->fine);
    printf("\nNUMBER OF BOOKS CURRENTLY ISSUED:%d",mem->no_of_issues);
    printf("\nNUMBER OF TIMES FINED:%d\n",mem->no_of_fines);
}

//function to search and display those members having a particular fine
void search_member_by_fine(member_tree_node *root,float fine)
{
    int i;
    for(i=0;i<root->size;i++)
    {
        if(root->members[i]->fine==fine)
        {
            display_member(root->members[i]);
        }
        if(!root->leaf)
        {
            search_member_by_fine(root->children[i],fine);
        }
    }
    if(!root->leaf)
    {
        search_member_by_fine(root->children[i],fine);
    }
}

//this function connects all the members who have issued maximum books at present into a linked list
void get_members_with_maxbooks(member_tree_node *root,member_list **head,member_list **ptr)
{
    int i;
    for(i=0;i<root->size;i++)
    {
        if((*head)->mem==NULL)
        {
            (*head)->mem=root->members[i];
            *ptr=*head;
        }
        else if((*ptr)->mem->no_of_issues<root->members[i]->no_of_issues)
        {
            (*head)->mem=root->members[i];
            *ptr=*head;
            free_nodes((*head)->next);
            (*ptr)->next=NULL;
        }
        else if((*ptr)->mem->no_of_issues==root->members[i]->no_of_issues)
        {
            member_list *new_node=(member_list*)malloc(sizeof(member_list));
            new_node->mem=root->members[i];
            new_node->next=NULL;
            (*ptr)->next=new_node;
            *ptr=new_node;
        }
        if(!root->leaf)
        {
            get_members_with_maxbooks(root->children[i],head,ptr);
        }
    }
    if(!root->leaf)
    {
        get_members_with_maxbooks(root->children[i],head,ptr);
    }
}

/* Subdivsion iv solution
   This function will print the names of students and faculty who issued maximum books at present*/
void maxbooks(member_tree_node *student,member_tree_node *faculty)
{
   printf("\t\t\t--DISPLAYING STUDENTS AND FACULTY WHO ISSUED MAXIMUM NO:OF BOOKS--");
   int k;
   char t;
   member_list *head,*ptr;
   head=(member_list*)malloc(sizeof(member_list));
   if(student)
   {
      head->next=NULL;
      head->mem=NULL;
      get_members_with_maxbooks(student,&head,&ptr);
      printf("\nSTUDENT(S) WHO ISSUED MAX BOOKS:");
      k=1;
      ptr=head;
      while(ptr!=NULL)
      {
        printf("\n%d.%s",k,ptr->mem->name);
        k++;
        ptr=ptr->next;
      }
   }
   if(faculty)
   {
      head->next=NULL;
      head->mem=NULL;
      get_members_with_maxbooks(faculty,&head,&ptr);
      printf("\nFACULTY(FACULTIES) WHO ISSUED MAX BOOK:");
      k=1;
      ptr=head;
      while(ptr!=NULL)
      {
        printf("\n%d.%s",k,ptr->mem->name);
        k++;
        ptr=ptr->next;
      }
   }
   free_nodes(head->next);
   free(head);
   printf("\npress any key to continue");
   scanf("%c",&t);
   scanf("%c",&t);
}

//function to free linked list nodes, used inside get_members_with_maxbooks function
void free_nodes(member_list *ptr)
{
    member_list *dptr;
    while(ptr)
    {
        dptr=ptr;
        ptr=ptr->next;
        free(dptr);
    }
}


/*---------------------------------------------------- DELETION ALGORITHM FOR B TREE OF MEMBERS(FINE TREE) ----------------------------------------------------------*/

//function to delete a data from the tree
member_tree_node * deletion(member_tree_node* root,char key[key_size])
{
    if(root==NULL)
    {
        printf("\nTREE IS EMPTY");
    }
    else
    {
        Remove(root,key);
        if((root)->size==0)
        {
            member_tree_node *dptr=root;
            if(root->leaf)
            {
                root=NULL;
            }
            else
            {
                root=root->children[0];
            }
            free(dptr);
        }
    }
    return root;
}

//function to remove a data from a node
void Remove(member_tree_node *root,char key[key_size])
{
    int i=0;
    while(i<root->size && strcmp(root->members[i]->alt_key,key)<0)
    {
        i++;
    }
    if(i<root->size && strcmp(root->members[i]->alt_key,key)==0)
    {
        if(root->leaf)
        {
            remove_from_leaf(root,i);
        }
        else
        {
            remove_from_nonleaf(root,i);
        }
    }
    else
    {
        if(root->leaf)
        {
            printf("\nKEY NOT FOUND");
        }
        else
        {
            bool flag=(i==root->size)?true:false;
            if(root->children[i]->size<t)
            {
                fill(root,i);
            }
            if(flag && root->size<i)
            {
                Remove(root->children[i-1],key);
            }
            else
            {
                Remove(root->children[i],key);
            }

        }
    }
}

//function to remove data from a leaf
void remove_from_leaf(member_tree_node *root,int pos)
{
    for(int i=pos+1;i<root->size;i++)
    {
        root->members[i-1]=root->members[i];
    }
    root->size-=1;
}

//function to remove data from a non leaf.
//this function recursively deletes data from its children untill it reaches leaf
void remove_from_nonleaf(member_tree_node*root,int pos)
{
    struct member_detail *temp=root->members[pos];
    if(root->children[pos]->size>=t)
    {
        temp=get_pred(root,pos);
        root->members[pos]=temp;
        Remove(root->children[pos],temp->alt_key);
    }
    else if(root->children[pos+1]->size>=t)
    {
        temp=get_succ(root,pos);
        root->members[pos]=temp;
        Remove(root->children[pos+1],temp->alt_key);
    }
    else
    {
        merge(root,pos);
        Remove(root->children[pos],temp->alt_key);
    }
}

//function to get predecessor
struct member_detail* get_pred(member_tree_node *node,int pos)
{
    member_tree_node *ptr=node->children[pos];
    while(!ptr->leaf)
    {
        ptr=ptr->children[ptr->size];
    }
    return ptr->members[ptr->size-1];
}

//function to get successor
struct member_detail* get_succ(member_tree_node *node,int pos)
{
    member_tree_node* ptr=node->children[pos+1];
    while(!ptr->leaf)
    {
        ptr=ptr->children[0];
    }
    return ptr->members[0];
}

void fill(member_tree_node *node,int pos)
{
    if( pos !=0 && node->children[pos-1]->size>=t)
    {
        borrow_from_prev(node,pos);
    }
    else if(pos!=node->size && node->children[pos+1]->size>=t)
    {
        borrow_from_next(node,pos);
    }
    else
    {
        if(pos!=node->size)
        {
            merge(node,pos);
        }
        else
        {
            merge(node,pos-1);
        }
    }
}

//function to borrow a data from the left sibling and gives it to the parent and parent gives one data to the child in requirement
void borrow_from_prev(member_tree_node *node,int pos)
{
    member_tree_node *child=node->children[pos];
    member_tree_node *sibling=node->children[pos-1];
    int i;
    for(i=child->size-1;i>=0;i--)
    {
        child->members[i+1]=node->members[i];
    }
    child->members[0]=node->members[pos-1];
    if(!child->leaf)
    {
        for(i=child->size;i>=0;i--)
        {
            child->children[i+1]=child->children[i];
        }
        child->children[0]=sibling->children[sibling->size];
    }
    node->members[pos-1]=sibling->members[sibling->size-1];
    sibling->size-=1;
    child->size+=1;
}

//function which borrows a data from the right sibling and gives it to the parent and parent gives one data to the child in requirement
void borrow_from_next(member_tree_node *node,int pos)
{
    member_tree_node *child=node->children[pos];
    member_tree_node *sibling=node->children[pos+1];
    int i;
    child->members[child->size]=node->members[pos];
    if(!child->leaf)
    {
        child->children[child->size+1]=sibling->children[0];
    }
    node->members[pos]=sibling->members[0];
    for(i=1;i<sibling->size;i++)
    {
        sibling->members[i-1]=sibling->members[i];
    }
    if(!sibling->leaf)
    {
        for(i=1;i<sibling->size+1;i++)
        {
            sibling->children[i-1]=sibling->children[i];
        }
    }
    sibling->size-=1;
    child->size+=1;
}

//function to merge to siblings i.e child[pos] and child[pos+1]
void merge(member_tree_node *node,int pos)
{
    member_tree_node *child=node->children[pos];
    member_tree_node *sibling=node->children[pos+1];
    int i;
    child->members[t-1]=node->members[pos];
    for(i=0;i<sibling->size;i++)
    {
        child->members[t+i]=sibling->members[i];
    }
    if(!child->leaf)
    {
        for(i=0;i<sibling->size+1;i++)
        {
            child->children[i+t]=sibling->children[i];
        }
    }
    for(i=pos+1;i<node->size;i++)
    {
        node->members[i-1]=node->members[i];
    }
    for(i=pos+2;i<node->size+1;i++)
    {
        node->children[i-1]=node->children[i];
    }
    child->size+=sibling->size;
    node->size--;
    free(sibling);
}


/*---------------------------------------------------- END OF DELETION ALGORITHM FOR B TREE OF MEMBERS (FINE TREE) ----------------------------------------------------------*/


/*---------------------------------------------------- INSERTION ALGORITHM FOR B TREE OF MEMBERS WITH NUMBER OF FINES BEING THE KEY (FINE TREE)----------------------------------------------------------*/

//function used to insert a data into tree which keeps track of number of fines of members
void insert_into_memtree2(member_tree_node *node,member_tree_node** splitted_node,struct member_detail** splitted_member,struct member_detail* mem)
{
    int i,j;
    if(node->leaf)
    {
        i=node->size-1;
        while(i>=0 && strcmp(mem->alt_key,node->members[i]->alt_key)<0)
        {
            node->members[i+1]=node->members[i];
            i--;
        }
        node->members[i+1]=mem;
        node->size+=1;
        if(node->size==ORDER)
        {
            split_memtree_node(node,splitted_node,splitted_member);
        }
    }
    else
    {
        i=0;
        while(i<node->size && strcmp(node->members[i]->alt_key,mem->alt_key)<0)
        {
            i++;
        }
        insert_into_memtree2(node->children[i],splitted_node,splitted_member,mem);
        if((*splitted_node)==NULL) //do not do anything if there was no splitting in its children node
        {
            return;
        }
        for(j=i;j<node->size;j++)
        {
            node->members[j+1]=node->members[j];
        }
        node->members[i]=*splitted_member;
        for(j=i+1;j<=node->size;j++)
        {
            node->children[j+1]=node->children[j];
        }
        node->children[i+1]=*splitted_node;
        *splitted_node=NULL;
        *splitted_member=NULL;
        node->size+=1;
        if(node->size==ORDER)   //split if it exceeds the capacity
        {
            split_memtree_node(node,splitted_node,splitted_member);
        }
    }
}

member_tree_node* insert_memtree2(member_tree_node *root,struct member_detail* mem) 
{
    member_tree_node* splitted_node=NULL;
    struct member_detail* splitted_member=NULL;
    if(root==NULL)
    {
        root=(member_tree_node*)malloc(sizeof(member_tree_node));
        root->size=1;
        root->leaf=true;
        root->members[0]=mem;
    }
    else
    {
        insert_into_memtree2(root,&splitted_node,&splitted_member,mem);
        if(splitted_node!=NULL)
        {
            member_tree_node* new_root=(member_tree_node*)malloc(sizeof(member_tree_node));
            new_root->leaf=false;
            new_root->size=1;
            new_root->members[0]=splitted_member;
            new_root->children[0]=root;
            new_root->children[1]=splitted_node;
            root=new_root;
        }
    }
    return root;  
}

/*---------------------------------------------------- END OF INSERTION ALGORITHM FOR B TREE OF MEMBERS WITH NUMBER OF FINES BEING THE KEY (FINE TREE)----------------------------------------------------------*/


//this function searches through the fine tree and displays members having more than 5 fines
void search_fine_tree(member_tree_node *root)
{
    int i=0;
    char key[10]="5";
    while(i<root->size && strcmp(key,root->members[i]->alt_key)>0)
    {
        i++;
    }
    while(i<root->size)
    {
        
        if(strcmp(key,root->members[i]->alt_key)<0)
        {
            display_member(root->members[i]);
            printf("\n");
        }
        if(!root->leaf)
        {
            search_fine_tree(root->children[i]);
        }
        i++;
    }
    if(!root->leaf)
    {
        search_fine_tree(root->children[i]);
    }

}

/*---------------------------------------------------- INSERTION ALGORITHM FOR B TREE OF MEMBERS WITH NUMBER OF BORROWS BEING THE KEY (BORROW TREE)----------------------------------------------------------*/


member_tree_node* insert_memtree3(member_tree_node *root,struct member_detail* mem) 
{
    member_tree_node* splitted_node=NULL;
    struct member_detail* splitted_member=NULL;
    if(root==NULL)
    {
        root=(member_tree_node*)malloc(sizeof(member_tree_node));
        root->size=1;
        root->leaf=true;
        root->members[0]=mem;
    }
    else
    {
        insert_into_memtree3(root,&splitted_node,&splitted_member,mem);
        if(splitted_node!=NULL)
        {
            member_tree_node* new_root=(member_tree_node*)malloc(sizeof(member_tree_node));
            new_root->leaf=false;
            new_root->size=1;
            new_root->members[0]=splitted_member;
            new_root->children[0]=root;
            new_root->children[1]=splitted_node;
            root=new_root;
        }
    }
    return root;  
}

void insert_into_memtree3(member_tree_node *node,member_tree_node** splitted_node,struct member_detail** splitted_member,struct member_detail* mem)
{
    int i,j;
    if(node->leaf)
    {
        i=node->size-1;
        while(i>=0 && strcmp(mem->alt_key2,node->members[i]->alt_key2)<0)
        {
            node->members[i+1]=node->members[i];
            i--;
        }
        node->members[i+1]=mem;
        node->size+=1;
        if(node->size==ORDER)
        {
            split_memtree_node(node,splitted_node,splitted_member);
        }
    }
    else
    {
        i=0;
        while(i<node->size && strcmp(node->members[i]->alt_key2,mem->alt_key2)<0)
        {
            i++;
        }
        insert_into_memtree3(node->children[i],splitted_node,splitted_member,mem);
        if((*splitted_node)==NULL) //do not do anything if there was no splitting in its children node
        {
            return;
        }
        for(j=i;j<node->size;j++)
        {
            node->members[j+1]=node->members[j];
        }
        node->members[i]=*splitted_member;
        for(j=i+1;j<=node->size;j++)
        {
            node->children[j+1]=node->children[j];
        }
        node->children[i+1]=*splitted_node;
        *splitted_node=NULL;
        *splitted_member=NULL;
        node->size+=1;
        if(node->size==ORDER)   //split if it exceeds the capacity
        {
            split_memtree_node(node,splitted_node,splitted_member);
        }
    }
}

/*---------------------------------------------------- END OF INSERTION ALGORITHM FOR B TREE OF MEMBERS WITH NUMBER OF BORROWS BEING THE KEY (BORROW TREE)----------------------------------------------------------*/


/*---------------------------------------------------- DELETION ALGORITHM FOR B TREE OF MEMBERS WITH NUMBER OF BORROWS BEING THE KEY (BORROW TREE)----------------------------------------------------------*/


//function to remove a data from a node
void Remove2(member_tree_node *root,char key[key_size])
{
    int i=0;
    while(i<root->size && strcmp(root->members[i]->alt_key2,key)<0)
    {
        i++;
    }
    if(i<root->size && strcmp(root->members[i]->alt_key2,key)==0)
    {
        if(root->leaf)
        {
            remove_from_leaf(root,i);
        }
        else
        {
            remove_from_nonleaf2(root,i);
        }
    }
    else
    {
        if(root->leaf)
        {
            printf("\nKEY NOT FOUND");
        }
        else
        {
            bool flag=(i==root->size)?true:false;
            if(root->children[i]->size<t)
            {
                fill(root,i);
            }
            if(flag && root->size<i)
            {
                Remove2(root->children[i-1],key);
            }
            else
            {
                Remove2(root->children[i],key);
            }

        }
    }
}

//function to remove data from a non leaf.
//this function recursively deletes data from its children untill it reaches leaf
void remove_from_nonleaf2(member_tree_node*root,int pos)
{
    struct member_detail *temp=root->members[pos];
    if(root->children[pos]->size>=t)
    {
        temp=get_pred(root,pos);
        root->members[pos]=temp;
        Remove2(root->children[pos],temp->alt_key2);
    }
    else if(root->children[pos+1]->size>=t)
    {
        temp=get_succ(root,pos);
        root->members[pos]=temp;
        Remove2(root->children[pos+1],temp->alt_key2);
    }
    else
    {
        merge(root,pos);
        Remove2(root->children[pos],temp->alt_key2);
    }
}

//function to delete a data from the tree
member_tree_node * deletion2(member_tree_node* root,char key[key_size])
{
    if(root==NULL)
    {
        printf("\nTREE IS EMPTY");
    }
    else
    {
        Remove2(root,key);
        if((root)->size==0)
        {
            member_tree_node *dptr=root;
            if(root->leaf)
            {
                root=NULL;
            }
            else
            {
                root=root->children[0];
            }
            free(dptr);
        }
    }
    return root;
}

/*----------------------------------------------------END OF DELETION ALGORITHM FOR B TREE OF MEMBERS WITH NUMBER OF BORROWS BEING THE KEY (BORROW TREE)----------------------------------------------------------*/


//function to display borrowers in descending order of number of borrows
void display_borrowers(member_tree_node *node)
{
    int i;
    if(!node->leaf)
    {
        display_borrowers(node->children[node->size]);
    }
    for(i=node->size-1;i>=0;i--)
    {
        if(!node->leaf)
        {
            display_borrowers(node->children[i]);
        }
        display_member(node->members[i]);
    }
}
