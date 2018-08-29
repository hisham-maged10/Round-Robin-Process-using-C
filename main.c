#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

typedef struct // Queue's NODE
{
    int element;
    struct Node *next;
} Node;
typedef struct // Queue Data Structure
{
    Node *head;
    Node *tail;
} Queue;
typedef struct // Used in reading from and writing to files
{
    char name [25];
    int out,enter;

}Operation;

int dequeue(Queue*s);
void enqueue(Queue*s,int value);
void proccessing(Queue*s);
Node* createNode(int value);
void initialize(Queue*s);
int isEmpty(Queue*s);
int index=0;

int main()
{
    Queue Q;                           //For Creating The Queue
    initialize(&Q);                    //initializing the Queue
    proccessing(&Q);                   //To do the Operations
    return 0;
}
void initialize(Queue *Q)
{
    Q->tail=NULL; //make the tail equal to null
    Q->head=NULL; //make the head equal to null
}
Node* createNode(int value)
{
    Node *n=malloc(sizeof(Node)); //save memory of size Node place in memory
    n->next=NULL;//initialize next to null
    n->element=value; //take value and assign node with it
    return n;
}
int isEmpty(Queue *Q)
{
    if((Q->head)&&(Q->tail))//if queue is empty return 1 and else return 0
        return 0;
    else
        return 1;
}
void enqueue(Queue *Q,int value)
{
    Node*n=createNode(value);//gets the value and put it in the new node u created
    Node*temp=Q->tail;// create temp node and put tail of Queue from it
    Q->tail=n; //change queue tail to point to new node
    if (temp)
    {
    temp->next=n;// if temp != null then temp ==> old tail will point to the new node
    }
    else
    {
    Q->head=n; // if == null point the head of queue to the pointer mean that it is the first element input
    }
}
int dequeue(Queue *Q)
{
    Node*temp=(Node*)Q->head;//create pointer to temporary node and points to head of the queue
    if (temp) // if temp isn't equal to null
    {
        int value;
        value=temp->element; // takes the value of the queue's head
        Q->head=(Node*)temp->next; //make head point to the next node in Queue Q
        free(temp);  //free memory of the temporary node
        if (!Q->head)
        {
        Q->tail=NULL; //if the head points to the node, then tail will point to null
        }
        return value;
    }
    puts("\nThe queue is empty!") ;//if the queue was empty i will print error
    exit(1);
}

void proccessing(Queue *Q)
{
    FILE *file;
    Operation operation[10]; //create variable operation that inputs the read files' info
    int inoperation[10]; //it loads the indicies and put them in the stack
    int i=0,j=0,flag=0; //counters
    int temp;
    char c;
    int noofOperation=0;
    file = fopen("Operations.txt","r");
    if (file==NULL)//if reading from file equal to null then the file does not exist
    {
        printf("file isn't found");
        exit(1);
    }
    else
    {
        file = fopen("Operations.txt","r");
        while(!feof(file)) //Loop until EOF
        {
            c=getc(file);//get the character from the file and store it in c
            printf("%c",c);
            if(c=='=')//if the c is equal to = then the number after it the time
            {
                flag++;//assign the flag counter to 0
                while(!feof(file)) //loop until EOF
                {
                    c=getc(file); //get character from the file
                     printf("%c",c);
                    if(c==' ') //if it is equal to white space then don't proceed with the code
                        continue;
                    else if(c=='\n') //if it is equal to \n then enter the next line
                    {
                       noofOperation=noofOperation/10; // take variable noofOperationx that indacte the time and divide by 10
                        operation[0].enter=0; // initialize enter and out to zero
                       operation[0].out=0;
                        break;
                    }
                    else // get the number * 10 to traverse
                    {
                           noofOperation= noofOperation+(c-'0');
                           noofOperation*=10;
                    }
                }
            }
            else if(flag>0) // flag >0 means that the  time is processing is taken
            {
                operation[0].name[0]=c;
                i=0; //reset the values
                flag=i;
                j++;

                while(!feof(file))
                {
                    c=getc(file);
                     printf("%c",c);
                    if(c==' ') // if the character taken was whitespace increase flag and continue to the next iteration of the loop
                    {
                        flag++;
                        continue;
                    }
                    else if(c>='0'&&c<='9')
                    {
                        if(flag==2) // if it is equal to two then c has the out time
                        {
                             operation[i].out+=(c-'0'); // take value and subtract '0' to get it's value since zero ascii is 47 and each number is up by one
                            operation[i].out*=10;// then multiply by 10

                        }
                        else if(flag==1)// if it is equal to one then c has the in time
                        {
                          operation[i].enter= operation[i].enter+(c-'0');
                            operation[i].enter*=10;
                            index++;

                        }
                        else// else flag == 0 then c now is reading number which beside the process
                        {
                            operation[i].name[j]=c; // read the number and get the name from it
                            j++;
                        }
                    }
                    else if(c=='\n') // if it read new line from the file
                    {
                       operation[i].enter/=10; // it will divide the enter and out
                       operation[i].out/=10;
                        flag=0; // make counter =0
                        operation[i].name[j]='\0';
                        j=0;// reset counter
                        i++;
                        operation[i].enter=0; // initialize
                        operation[i].out=0;
                    }
                    else // name of the process
                    {
                        operation[i].name[j]=c;
                        j++;
                    }
                }
                if (feof(file))
                {
                    operation[i].name[j-1]='\0';
                    operation[i].enter/=10;
                    operation[i].out/=10;
                    flag=0;
                }
                flag++;
            }
        }
        fclose(file);
        flag=0;
        i=0;
    }
puts("\nOperating");

    while(i<noofOperation) // loop until the processing time that was read from file
    {
        if(i==0)
        {
        for(j=0; j<index; j++)
        {
            if( (operation[j].enter==i)&& (operation[j].name!=NULL)) // if name is not equal to null then that means means that is info
            {
               inoperation[flag]=j;
                enqueue(Q,j); //enqueue Q
                flag++;
            }
        }
        }

        if(!isEmpty(Q)) // the queue is not empty then
        {
            temp=dequeue(Q); // dequeue the first element and put in the temp
            operation[temp].out--; // decrease the out time
            if(operation[temp].out==0) // if the out time is equal to zero
            {
                printf("%s %d to %d aborted due to time  \n",operation[temp].name,i,i+1);
                i++;
                for(j=0; j<4; j++) //check if the time increases
                {
                    if(operation[j].enter == i&&operation[j].name!=NULL)
                    {
                        enqueue(Q,j);
                        inoperation[flag++]=j;
                    }
                }
            }
            else if (operation[temp].out !=0)
            {
                printf("%s %d to %d\n",operation[temp].name,i,i+1);
                i++;
                for(j=0; j<index; j++)
                {
                    if(operation[j].enter == i&&operation[j].name!=NULL)
                    {
                        inoperation[flag++]=j;
                        enqueue(Q,j);
                    }
                }
                enqueue(Q,temp); // make the element enter the queue
            }

        }
        else // if the queue was empty
        {
            printf( "Idle %d to %d\n",i,i+1);
            i++;
            for(j=0; j<index; j++)
            {
                if(operation[j].enter == i&&operation[j].name!=NULL)
                {
                    inoperation[flag++]=j;
                    enqueue(Q,j);
                }
            }
        }
    }
    printf("The END");
}


