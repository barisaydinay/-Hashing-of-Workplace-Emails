// BARIS AYDINAY 2452977

// I ASSUMED ALL MAIL ID'S ARE UNIQUE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct myData
{
    int mailID;
    char sender[50];
    char receipt[50];
    int day;
    char content[200];
    int noOfWords;
};


void menu();
struct myData* readMail(char *,int,int,int *);
int hash(int,int);
void FindEmptySlot(struct myData*,int ,struct myData,int,int,int);
int hashLinear(int);
void printHashTable(struct myData*,int);
int hashDouble(int,int);
struct myData* rehash(int ,struct myData*,int ,int ,struct myData,int);
void findHash(struct myData*,int,int,int,int,int);


int main()
{
    int number=0,option=-1;
    char filePath[50];
    printf("Enter data path: ");
    scanf("%s",filePath);
    fflush(stdin);
    printf("How many files: ");
    scanf("%d",&number);
    strcat(filePath,"/");
    int adressingChoice;
    printf("Please enter the addressing choice (1: Linear Probing, 2: Double Hashing): ");
    scanf("%d",&adressingChoice);
    struct myData *myTable;
    int mySize = 0;
    myTable = readMail(filePath, number,adressingChoice,&mySize); // Mails have been read according to the addressing choice!

    while(option!=3) {

        menu();
        printf("\n");
        printf("Enter option: ");
        scanf("%d",&option);
        printf("\n");

        if(option == 1)
        {
            int mailid;
            char name[50];
            printf("Enter unique identifier: ");
            scanf("%d",&mailid);
            printf("Enter sender: ");
            scanf("%s",name);
            fflush(stdin);
            int myKey;
            int id_name0 = mailid + name[0];
            myKey = (mailid + name[0]) -65;
            findHash(myTable,mySize,myKey,mailid,adressingChoice,id_name0);
        }

        else if(option == 2)
        {
            printf("\nTable Displayed: \n");
            printHashTable(myTable,mySize);
        }

        else printf("\n** GoodBye! **");

    }


}



void menu()
{
    printf("\nPlease choose one of the following options:\n");
    printf("(1) Search an email\n");
    printf("(2) Print Table\n");
    printf("(3) Exit\n");

}



struct myData* readMail(char filePath[50],int number,int adressingChoice,int *mySize)
{

    FILE *inFile1;
    char fileName[50]="";
    char fileNumber[3] = "";
    int i;
    int validity=0;

    int size = 11;
    *mySize = size;
    int key,value,temporarySize,n;

    struct myData *table = (struct myData*)malloc(size *sizeof(struct myData));
    struct myData *maintable;
    maintable = table;
    table[0].mailID = 0;
    int j=1;
    int count=0;
    int k;
     for(k=0;k<size;k++){

        table[k].day = -1;
    }



    for(i=0; i < number; i++)
    {
        struct myData dummy;



        char str[200] = "";
        strcpy(fileName,filePath);

        itoa(i+1, fileNumber, 10);


        strcat(fileName,fileNumber);
        strcat(fileName,".txt");
        inFile1 = fopen(fileName,"r");

        if(inFile1 != NULL)
        {
            validity = 1;
        }

        if(inFile1 == NULL)
        {
            if(validity == 1)
            {
                printf("\n*** You have entered excess number of files, so entire folder have been read ! ***\n");
                i = number-1;
                continue;
            }
            else
            {
                printf("Directory that contains the mails can not be found !");
                exit(0);
            }
        }


        fseek(inFile1, 0, SEEK_SET);

        while(fgets(str,200,inFile1))
        {


            if(j==1)
            {
                int id = atoi(str);
                dummy.mailID = id;
            }

            else if(j==2)
            {
                if(str[0] == ' ') {
                    int subsize = strlen(str) -1;
                    char sub[subsize];
                    strncpy(sub,&str[1],subsize-1);
                    sub[subsize-1] = '\0';
                    strcpy(dummy.sender,sub);
                }
                else {
                        int subsize = strlen(str);
                        char sub[subsize];
                        strncpy(sub,&str[0],subsize-1);
                        sub[subsize-1] = '\0';
                        strcpy(dummy.sender,sub);
                }



            }
            else if(j==3)
            {

                if(str[0] == ' ') {
                    int subsize = strlen(str) -1;
                    char sub[subsize];
                    strncpy(sub,&str[1],subsize-1);
                    sub[subsize-1] = '\0';
                    strcpy(dummy.receipt,sub);
                }
                else {
                        int subsize = strlen(str);
                        char sub[subsize];
                        strncpy(sub,&str[0],subsize-1);
                        sub[subsize-1] = '\0';
                        strcpy(dummy.receipt,sub);
                }

            }

            else if(j==4)
            {
                int dayNo = atoi(str);
                dummy.day = dayNo;
            }
            else if(j==5)
            {
                strcpy(dummy.content,str);
                int k,words=0;

                for(k=0; k<strlen(str); k++)
                {
                    if(str[k] == ' ') words++;
                }
                dummy.noOfWords = words+1;

            }


            j++;
            if(j==4) fseek(inFile1, 6, SEEK_CUR);
            if(j==2) fseek(inFile1, 6,SEEK_CUR);
            if(j==3) fseek(inFile1,4,SEEK_CUR);
            count++;

        }
        printf("\nEmail Read!\n-----------");



        float lambda;
        if(dummy.sender[0] == ' ') key = (dummy.mailID + dummy.sender[1]) -65;
        else key = (dummy.mailID + dummy.sender[0]) -65;


        value = hash(size,key);

        lambda = ((1.0)*(i+1))/((1.0)*size) ;


        if(lambda > 0.5)
        {


            struct myData *tempTable;
            tempTable = (struct myData*)malloc(size*sizeof(struct myData));
            int b;
            for(b=0;b<size;b++) {
                tempTable[b].mailID = maintable[b].mailID;
                tempTable[b].day = maintable[b].day;
                tempTable[b].noOfWords = maintable[b].noOfWords;
                strcpy(tempTable[b].sender,maintable[b].sender);
                strcpy(tempTable[b].receipt,maintable[b].receipt);
            }

            free (maintable);
            maintable = tempTable;


            temporarySize = size;
            size = newSize(size);
            *mySize = size;
            struct myData *newTable;
            newTable = (struct myData*)malloc(size*sizeof(struct myData));
            newTable[0].mailID = 0;
            int a;
            for(a=0;a<size;a++){
             newTable[a].day = -1;
            }

            for(n=0;n<temporarySize;n++) {

                if(maintable[n].mailID != NULL) {

                    if(maintable[n].sender[0] == ' ') key = (maintable[n].mailID + maintable[n].sender[1]) -65;
                    else key = (maintable[n].mailID + maintable[n].sender[0]) -65;
                    value = hash(size,key);
                   FindEmptySlot(newTable,size,maintable[n],value,adressingChoice,key);
                }

            }

            if(dummy.sender[0] == ' ') key = (dummy.mailID + dummy.sender[1]) -65;
            else key = (dummy.mailID + dummy.sender[0]) -65;
            value = hash(size,key);
            FindEmptySlot(newTable,size,dummy,value,adressingChoice,key);
            free(maintable);
            maintable = newTable;
        }

        else
        {

         FindEmptySlot(maintable,size,dummy,value,adressingChoice,key);

        }


        printHashTable(maintable,size);


        j=1;
        strcpy(fileName,"");
        fclose(inFile1);
    }
    return maintable;


}

void findHash(struct myData* myTable,int mySize,int searchValue,int mailID,int addressing,int nonhashed) {
    int index;
    int flag=0;
    index = hash(mySize,searchValue);
    int temp;
    temp = index;


    if(addressing == 1) {
    while(myTable[index].day != -1) {



        if(myTable[index].mailID == mailID) {
            flag=1;
            printf("Mail Found!\n");
            printf("Recipient: %s\n",myTable[index].receipt);
            printf("Date: %d\n",myTable[index].day);
            printf("Number of Words: %d\n",myTable[index].noOfWords);
        }
        if(index == mySize-1) index = 0;
        else index++;

    }
    }

    else {
        int j=0;
        while(myTable[temp].day != -1) {
            if(myTable[temp].mailID == mailID) {
                flag=1;
                printf("---Mail Found!---\n");
                printf("Recipient: %s\n",myTable[temp].receipt);
                printf("Date: %d\n",myTable[temp].day);
                printf("Number of Words: %d\n",myTable[temp].noOfWords);
                break;
            }
            j++;
            temp = hash(mySize,index + hashDouble(j,nonhashed));

        }

    }


    if(flag==0) printf("\n--Mail Not Found!--\n");

}




void printHashTable(struct myData* table,int size) { // I used https://stackoverflow.com/questions/14420924/aligning-output-values-in-c  in order to align my output neatly.
    int i;
    printf("\nIndex ID  Sender Recipient Date Words\n\n");
    for(i=0;i<size;i++) {

            if(table[i].day == -1) {
                printf("%d\n",i);
                continue;

            }
        printf("%-6d%-4d%-8s%-9s%-6d%-6d\n",i,table[i].mailID,table[i].sender,table[i].receipt,table[i].day,table[i].noOfWords);
    }
    printf("---------------------------------------\n");

}



int newSize(int size)
{
    int i,flag=0;
    size = size*2;

    do
    {
        size++;
        flag = 1;

        for(i=2;i<=size-1;i++)
        {
                if(size % i == 0)
                {
                    flag = 0;
                    break;
                }

        }

    }while(flag==0);

    return size;


}


int hash(int size,int hashValue)
{
    int key;
    key = hashValue % size;
    return key;


}

void FindEmptySlot(struct myData* table,int size,struct myData dummy, int value, int choice,int nonhashed) {
    int i,temp;
    temp = value;


    for(i=0;i<=size;i++) {

            if(table[temp].day == -1) {
                table[temp].mailID = dummy.mailID;
                table[temp].day = dummy.day;
                table[temp].noOfWords = dummy.noOfWords;
                strcpy(table[temp].sender,dummy.sender);
                strcpy(table[temp].receipt,dummy.receipt);
                break;
            }

            else
            {
                if(choice == 1) {
                temp = hash(size,value+hashLinear(i));
                }
                else
                {
                temp = hash(size,value + hashDouble(i,nonhashed));

                }

            }



    }




}


int hashDouble(int i,int key) {
    return (i*(5-(key % 5)));

}



int hashLinear(int i) {
    return i;
}
