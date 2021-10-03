#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define title_size 100
#define subject_size 100
#define author_size 5
#define author_name_size 100
#define dept_size 50
#define member_name_size 100
#define desgn_size 10
#define key_size 200
#define ORDER 3

typedef enum{false,true} bool;

extern float max_fine; //declaration of variable which keeps track of maximum fine among all the members
extern int t; //minimum degree of node in a b tree

struct date
{
   int d,m,y;
};

struct book_detail
{
   char title[title_size];
   char subject[subject_size];
   char authors[author_size][author_name_size];
   int no_of_authors;
   int accno;
   int print_year;
   int status;
   int hall,rack,row;
   char key[key_size];
};

struct member_detail
{
   char name[member_name_size];
   char dept[dept_size];
   char desgn[desgn_size];
   int id;
   int no_of_issues;
   int no_of_fines;
   struct book_issued *book_lists; //circular linked list of all the books issued by this member 
   float fine;
   char key[key_size]; //key based on subject,title,author's name and print year
   char alt_key[key_size]; //key based on number of fines and member id
   char alt_key2[key_size];//key based on number of book issued currently
};

struct book_issued
{
   struct book_detail* bk;
   struct date issue_date;
   struct date exp_date;
   int renew_count;  //variable which keeps track of how many times this book was renewed by the member
   struct book_issued *next;
};

typedef struct books_node_tag{
    struct book_detail* books[ORDER];  //order of b tree is maximum number of children a node can have
    struct books_node_tag* children[ORDER+1] ;
    int size;  //number of books inthe node
    bool leaf ; // tells whether node is leaf node or not
}bk_tree_node;

typedef struct members_node_tag{
    struct member_detail* members[ORDER];   //order of b tree is maximum number of children a node can have
    struct members_node_tag* children[ORDER+1]; 
    int size; //number of members in the node
    bool leaf; //tells whether node is a leaf node or nots
}member_tree_node;

//this structure is for a linked list which is used to accumulate members having maximum book issued inside the maxbook function
typedef struct member_node_tag{
    struct member_detail *mem;
    struct member_node_tag *next;
}member_list;

void initialise_dbs(bk_tree_node **head,member_tree_node **faculty,member_tree_node **student,member_tree_node **fine_tree,member_tree_node **borrow_tree_root);

//functions used for insertion operation of book tree
bk_tree_node* insert_bktree(bk_tree_node *root,struct book_detail* bk);

void split_bktree_node(bk_tree_node * node_to_split,bk_tree_node** splitted_node,struct book_detail** splitted_book);

void insert_into_bktree(bk_tree_node* node,bk_tree_node** splitted_node,struct book_detail** splitted_book,struct book_detail* bk);

bk_tree_node* insertbook(bk_tree_node *root);

void make_book_key(struct book_detail* bk);

//function used for searching book by title
void search_by_title(bk_tree_node* root,char title[title_size]); 

//function used for searching book by author
void search_by_author(bk_tree_node* root,char str[author_name_size]);

void searchbook(bk_tree_node *root);

//function to search and get the data of a book using key
struct book_detail* search_book_by_key(bk_tree_node* root,char key[key_size]);

//function used to display book
void displaybook(struct book_detail* book) ;

//functions used for insertion operation of member tree
member_tree_node* insert_memtree(member_tree_node *root,struct member_detail* mem);

void split_memtree_node(member_tree_node* node_to_split,member_tree_node** splitted_node,struct member_detail** splitted_member);

void insert_into_memtree(member_tree_node *node,member_tree_node** splitted_node,struct member_detail** splitted_member,struct member_detail* mem);

member_tree_node* appendmember(member_tree_node *root,member_tree_node **borrow_tree_root);

void make_member_key(struct member_detail *mem);

//function used to check if a year is leap year or not
int check_leap(int year);

// This function will return a specific date after given number of days from the given date.
struct date setdate(int days,struct date today);

// Function used to issue a book by scanning the accesion number of the book
//make sure last is not null
void issuebook(bk_tree_node *book_root,member_tree_node *mem_root,int maxissue,member_tree_node **borrow_tree_root);

//function to search and get the data of a member using key
struct member_detail* search_member_by_key(member_tree_node* root,char key[key_size]);

//function to search and display those members having a particular fine
void search_member_by_fine(member_tree_node *root,float fine);

// Function used to calculate fine amd return which uses date of return/reissue and expiry date
float calculatefine(struct date today,struct date exp_date);

/*Function used when user wants to return a book
 it calls calculatefine() to add fine if there is any */   //make sure last is not null
void returnbook(member_tree_node *root,member_tree_node **fine_tree_root,member_tree_node **borrow_tree_root);

/* Function called when user wants to renew his/her book further
   it calcultes if there is any fine and ensures that user does not exceed the maximum number 
   of times he/she can renew*/
void renewbook(member_tree_node *root ,member_tree_node **fine_tree_root);

//function to display details of the member
void display_member(struct member_detail* mem);

/* Solution to subdivision v 
   This function will display fine of a particular member by entering his/her id
   This function will also display names of members who currently have maximum fines */
void checkfine_and_displaymaxfine(member_tree_node *student,member_tree_node *faculty);

//this function connects all the members who have issued maximum books at present into a linked list
void get_members_with_maxbooks(member_tree_node *root,member_list **head,member_list **ptr);

/* Subdivsion iv solution
   This function will print the names of students and faculty who issued maximum books at present*/
void maxbooks(member_tree_node *student,member_tree_node *faculty);

//function to free linked list nodes, used inside get_members_with_maxbooks function
void free_nodes(member_list *ptr);

//function to delete a data from the tree
member_tree_node * deletion(member_tree_node* root,char key[key_size]);

//function to remove a data from a node
void Remove(member_tree_node *root,char key[key_size]);

//function to remove data from a leaf
void remove_from_leaf(member_tree_node *root,int pos);

//function to remove data from a non leaf.
//this function recursively deletes data from its children untill it reaches leaf
void remove_from_nonleaf(member_tree_node*root,int pos);

//function to get predecessor
struct member_detail* get_pred(member_tree_node *node,int pos);

//function to get successor
struct member_detail* get_succ(member_tree_node *node,int pos);

void fill(member_tree_node *node,int pos);

//function to borrow a data from the left sibling and gives it to the parent and parent gives one data to the child in requirement
void borrow_from_prev(member_tree_node *node,int pos);

//function which borrows a data from the right sibling and gives it to the parent and parent gives one data to the child in requirement
void borrow_from_next(member_tree_node *node,int pos);

//function to merge to siblings i.e child[pos] and child[pos+1]
void merge(member_tree_node *node,int pos);

member_tree_node* insert_memtree2(member_tree_node *root,struct member_detail* mem) ;

//function used to insert a data into tree which keeps track of number of fines of members
void insert_into_memtree2(member_tree_node *node,member_tree_node** splitted_node,struct member_detail** splitted_member,struct member_detail* mem);

//this function searches through the fine tree and displays members having more than 5 fines
void search_fine_tree(member_tree_node *root);

//function to make alt_key2 for member
void make_member_alt_key2(struct member_detail *mem);

//function used for inserting into tree which keeps track of details of members based on the number of borrows
member_tree_node* insert_memtree3(member_tree_node *root,struct member_detail* mem);

//function used for inserting into tree which keeps track of details of members based on the number of borrows
void insert_into_memtree3(member_tree_node *node,member_tree_node** splitted_node,struct member_detail** splitted_member,struct member_detail* mem);

//function to remove a data from a node
void Remove2(member_tree_node *root,char key[key_size]);

member_tree_node * deletion2(member_tree_node* root,char key[key_size]);

//function to remove data from a non leaf.
//this function recursively deletes data from its children untill it reaches leaf
void remove_from_nonleaf2(member_tree_node*root,int pos);

//function to display borrowers in descending order of number of borrows
void display_borrowers(member_tree_node *node);