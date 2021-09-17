/*
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 2
Due Date: 9/18/2020
*/

#include<stdio.h>
#include<stdlib.h>

struct node
{
    int data;
    struct node *next;
}*head;



void append(int num)
{
    struct node *temp,*right;
    temp= (struct node *)malloc(sizeof(struct node));
    temp->data=num;
    right=(struct node *)head;
    while(right->next != NULL)
	right=right->next;
    right->next =temp;
    right=temp;
    right->next=NULL;
}



void add( int num )
{
    struct node *temp;
    temp=(struct node *)malloc(sizeof(struct node));
    temp->data=num;
    if (head== NULL)
    {
	head=temp;
	head->next=NULL;
    }
    else
    {
	temp->next=head;
	head=temp;
    }
}
void addafter(int num, int loc)
{
    int i;
    struct node *temp,*left,*right;
    right=head;
    for(i=1;i<loc;i++)
    {
	left=right;
	right=right->next;
    }
    temp=(struct node *)malloc(sizeof(struct node));
    temp->data=num;
    left->next=temp;
    left=temp;
    left->next=right;
    return;
}



void insert(int num)
{
    int c=0;
    struct node *temp;
    temp=head;
    if(temp==NULL)
    {
	add(num);
    }
    else
    {
	while(temp!=NULL)
	{
	    if(temp->data<num)
		c++;
	    temp=temp->next;
	}
	if(c==0)
	    add(num);
	else if(c<count())
	    addafter(num,++c);
	else
	    append(num);
    }
}



void delete(int num)
{
    struct node **temp, *erase;//temp is used to traverse, erase holds the value to be erased
    temp=&head;

    while(*temp != NULL){
    	if((*temp)->data == num){
	    erase = *temp; // used to hold value to be erased, erase did not like being a pptr, works file with ptr
	    *temp = (*temp)->next; // remove node
	    free(erase);//free old memory
    	}//if
    	else{
	    temp = &((*temp)->next); // move on
    	}//else
    }//while
    printf("Deletion Completed\n");
}//delete


void  display(struct node *r)
{
    r=head;
    while(r!=NULL)
    {
	printf("%d ",r->data);
	r=r->next;
    }
    printf("\n");
}


int count()
{
    struct node *n;
    int c=0;
    n=head;
    while(n!=NULL)
    {
	n=n->next;
	c++;
    }
    return c;
}

// Used to free remaining memory of items not deleted
void freeMem(){
	struct node *prev; // prev pointer to lag behind and free up memory
	
	//traversing the list to free up memory
	while(head != NULL){
		prev = head;
		head = head->next;
		free(prev);
	}//while
}//freeMem

int  main()
{
    int i,num;
    struct node *n;
    head=NULL;
    while(1)
    {
	printf("\nList Operations\n");
	printf("===============\n");
	printf("1.Insert\n");
	printf("2.Display\n");
	printf("3.Size\n");
	printf("4.Delete\n");
	printf("5.Exit\n");
	printf("Enter your choice : ");
	if(scanf("%d",&i)<=0){
	    printf("Enter only an Integer\n");
	    exit(0);
	} else {
	    switch(i)
	    {
		case 1:      printf("Enter the number to insert : ");
			     scanf("%d",&num);
			     insert(num);
			     break;
		case 2:     if(head==NULL)
			    {
				printf("List is Empty\n");
			    }
			    else
			    {
				printf("Element(s) in the list are : ");
			    }
			    display(n);
			    break;
		case 3:     printf("Size of the list is %d\n",count());
			    break;
		case 4:     if(head==NULL)
				printf("List is Empty\n");
			    else{
				printf("Deletion In Progress:\nEnter the number to delete : ");
				scanf("%d",&num);
				delete(num);
			    }
			    break;
		case 5: freeMem();//used to clean up rest of old memory    
			return 0;
		default:    printf("Invalid option\n");
	    }
	}
    }
    return 0;
}
