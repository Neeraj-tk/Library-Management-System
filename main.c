#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include"header.h"

float max_fine=0; //variable which keeps track of maxfine among all the members
int t; //minimum degree of tree which tracks members having fines to pay

void main()
{
    bk_tree_node *root;
    member_tree_node* student;
    member_tree_node* faculty;
    member_tree_node* fine_tree_root; //tree which store members based on number of fines
    member_tree_node* borrow_tree_root; //tree which store members based on number of borrows
    initialise_dbs(&root,&student,&faculty,&fine_tree_root,&borrow_tree_root);
    int i,n,j,size,choice,status,p;
    char ch;
    t=ceil(ORDER/2)-1;
    do
    {
       printf("\n\t\t\t\tLIBRARY SYSTEM");
       printf("\n\n1.ADD BOOKS");
       printf("\n2.ADD MEMBER");
       printf("\n3.ISSUE BOOK");
       printf("\n4.SEARCH FOR BOOK");
       printf("\n5.RETURN BOOK");
       printf("\n6.RENEW BOOK");
       printf("\n7.FIND STUDENTS AND FACULTIES WHO ISSUED MAXIMUM BOOKS AT PRESENT");
       printf("\n8.FIND FINE OF GIVEN MEMBER AND DISPLAY MEMBER(S) WITH MAX FINE");
       printf("\n9.DISPLAY BORROWERS IN DESCENDING ORDER OF NO: OF BOOKS CURRENTLY ISSUED");
       printf("\n10.DISPLAY MEMBERS HAVING MORE THAN FIVE FINES");
       printf("\n11.EXIT");
       printf("\nEnter your choice:");
       scanf("%d",&choice);
       system("clear");
       switch(choice)
       {
          case 1 : {
                      
                      root=insertbook(root);
                      break;
                   }
          case 2 : {
                      printf("\t\t\t\tAPPEND MEMBER");
                      printf("\nSELECT TYPE OF MEMBER:\nF-FACULTY\nS-STUDENT\n");
                      scanf("%c",&ch);   //scanf reading input from keyboard buffer and not letting me read.hence using 2 scanf
                      scanf("%c",&ch);
                      switch(ch)
                      {
                          case 'F' : {
                                        faculty=appendmember(faculty,&borrow_tree_root);
                                        break;
                                     }
                          case 'S' : {
                                        student=appendmember(student,&borrow_tree_root);
                                        break;
                                     }
                          default  : {
                                        printf("\nINVALID CHOICE(press any key to continue)");
                                        scanf("%c",&ch);
                                        break;
                                     }
                      }
                      break;
                  }
         case 3 : { 
                      printf("\t\t\tISSUE BOOK");
                      printf("\nSELECT TYPE OF MEMBER:\nF-FACULTY\nS-STUDENT\n");
                      scanf("%c",&ch);
                      scanf("%c",&ch);
                      switch(ch)
                      {
                          case 'F' : {
                                        if(faculty)
                                        {
                                          issuebook(root,faculty,4,&borrow_tree_root);
                                        }
                                        break;
                                     }
                          case 'S' : {
                                        if(student)
                                        {
                                          issuebook(root,student,2,&borrow_tree_root);
                                        } 
                                        break;
                                     }
                          default :  {
                                        printf("\nINVALID CHOICE");
                                        break;
                                     }
                      }
                      break;
                  }
          case 4 : {
                     if(root)
                     {
                       searchbook(root);
                     } 
                     break;
                  }
          case 5 : {   
                      printf("\t\t\t\tRETURN BOOK");
                      printf("\nSELECT TYPE OF MEMBER:\nF-FACULTY\nS-STUDENT\n");
                      scanf("%c",&ch);
                      scanf("%c",&ch);
                      switch(ch)
                      {
                          case 'F' : {
                                       if(faculty)
                                       {
                                         returnbook(faculty,&fine_tree_root,&borrow_tree_root);
                                       }
                                       break;
                                     }
                          case 'S' : { 
                                        if(student)
                                        {
                                           returnbook(student,&fine_tree_root,&borrow_tree_root); 
                                        }
                                        break;
                                     }
                          default :  {
                                        printf("\nINVALID CHOICE");
                                        break;
                                     }
                      }
                      break;
                  }
          case 6 : {   
                      printf("\t\t\t\tRENEW BOOK");
                      printf("\nSELECT TYPE OF MEMBER:\nF-FACULTY\nS-STUDENT\n");
                      scanf("%c",&ch);
                      scanf("%c",&ch);
                      switch(ch)
                      {
                          case 'F' : {
                                        if(faculty)
                                        {
                                          renewbook(faculty,&fine_tree_root);
                                        } 
                                        break;
                                     }
                          case 'S' : { 
                                        if(student)
                                        {
                                          renewbook(student,&fine_tree_root);
                                        }
                                        break;
                                     }
                          default :  {
                                        printf("\nINVALID CHOICE");
                                        break;
                                     }
                      }
                      break;
                  }
         case 7 : {   
                    if(student || faculty)
                    {
                      maxbooks(student,faculty);
                    }
                    break;
                  }
         case 8 : {
                    if(student || faculty)
                    {
                      checkfine_and_displaymaxfine(student,faculty);
                    }
                    break;
                  }
         case 9 : {
                    printf("\nMEMBERS IN THE DESCENDING ORDER OF NUMBER OF BORROWS:\n");
                    if(borrow_tree_root)
                    {
                      display_borrowers(borrow_tree_root);
                    }
                    printf("\npress any key to continue");
                    scanf("%c",&ch);
                    scanf("%c",&ch);
                    break;
                  }
         case 10: {
                    printf("\nMEMBERS HAVING MORE THAN 5 FINES ARE :\n");
                    if(fine_tree_root)
                    {
                       search_fine_tree(fine_tree_root);
                    }
                    printf("\npress any key to continue");
                    scanf("%c",&ch);
                    scanf("%c",&ch);
                    break;
                  }
         case 11: {
                    break; 
                  }          
         default :{
                    printf("\nINVALID CHOICE");
                    break;
                  } 
      }
      system("clear");
      
  }while(choice!=11);
}
