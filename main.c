#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int getFileSize(FILE *File)
{
    if(File) {fseek(File,0,SEEK_END); return ftell(File);}
    else {return 0;}
}

int getDataSize(FILE *File)
{
    int Size=0;

    if(File)
    {
        char c;
        fseek(File,0,SEEK_SET);
        while((c=fgetc(File))!=EOF) { if(c=='\n'){Size++;} }
    }

    return Size;
}

int *getLineSize(FILE *File,int dataSize)
{
    int i=0;
    int *LineSize=malloc(sizeof(int)*dataSize);
    for(i=0;i<dataSize;i++) {LineSize[i]=0;}

    char c;
    fseek(File,0,SEEK_SET);
    i=0;
    while((c=fgetc(File))!=EOF) {if(c=='\n'){LineSize[i++]++;} else{LineSize[i]++;} }

    return LineSize;
}

char *getFileData(FILE *File,int dataSize)
{
    char *buffer=NULL;
    int *LineSize = getLineSize(File,getDataSize(File));

    if(File)
    {
        fseek(File,0,SEEK_SET);
        buffer = malloc(sizeof(char)*dataSize);
        char *temp=malloc(sizeof(char*));
        int i=0;
        while(fgets(temp,LineSize[i++]+1,File)) {strcat(buffer,temp);}
    }return buffer;
}

void proc_Sort(char **Data,int dataSize)
{
    int i=0,j=0,minIndex;

    for(i=0;i<dataSize-1;i++)
    {
        minIndex=i;
        for(j=i+1;j<dataSize;j++)
        { if(strcmp(Data[j],Data[minIndex])<0) {minIndex=j;} }

        //perform swap
        if(minIndex!=i)
        {
            char *temp = Data[i];
            Data[i] = Data[minIndex];
            Data[minIndex] = temp;
        }
    }
}

void Display(char **Data)
{
    int i=0;
    while(Data[i]!=NULL) {printf("%s\n",Data[i]);i++;}
}

void storeToFile(FILE *File,char **Data,int dataSize)
{
    File = fopen("srcFile.txt","w");

    if(!File){return;}
    else
    {
        int i;
        for(i=0;i<dataSize;i++)
        {
            fputs(Data[i],File);
            fputc('\n',File);
        }
        fclose(File);
    }
}

void proc_BinarSearch(char *keyToFind,char**Data,int dataSize)
{
    int isFound=0,L=0,U=dataSize,M;

    while(isFound==0 && L<=U)
    {
        M=L+(U-L)/2;
        if(strcmp(keyToFind,Data[M])==0) {printf("%s is found at index #%d\n",keyToFind,M);isFound=1;return;}
        else if(strcmp(keyToFind,Data[M])>0) {L=M+1;}
        else {U=M-1;}
    }
    if(isFound==0) {printf("%s was not found on the specified file\n",keyToFind);}
}

int main(int argc,char* argv[])
{
    int i=0;
    FILE *srcFile;

    srcFile = fopen("srcFile.txt","r");

    if(!srcFile) { printf("File COULD NOT BE FOUND\n"); return -1; }
    else
    {
        char **Data = malloc(sizeof(char*)*getDataSize(srcFile));
        i=0;
        Data[i] = strtok(getFileData(srcFile,getFileSize(srcFile)),"\n");
        while(Data[i++]!=NULL) {Data[i] = strtok(NULL,"\n");}

        proc_Sort(Data,getDataSize(srcFile));
        storeToFile(srcFile,Data,getDataSize(srcFile));

        //BinarySearch
        char *keyToFind=malloc(sizeof(char*));
        gets(keyToFind);

        proc_BinarSearch(keyToFind,Data,getDataSize(srcFile));

    }

    return 0;
}