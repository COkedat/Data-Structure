// ����ö �ִܰŸ� �ּ�ȯ�� ���α׷�

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct element{ // ����ü
    char from; // �����
    char to; // ������
    int data; // ���� �ð�
    int ic; // ȯ�� ����
} element;

char *csvLists[20]={"1ȣ��","2ȣ��","3ȣ��","4ȣ��","5ȣ��","6ȣ��",
                    "7ȣ��","8ȣ��","9ȣ��","1ȣ��","2ȣ��","5ȣ��",
                    "���Ǽ�","���ἱ","����ö��","�д缱","��õ1��","�߾Ӽ�","���̸�","ȯ������"};


void readCSV(int i){
    char buf[2048];
    char fileDir[50]="./data/";
    //char* pLine;
    printf("%c \n",csvLists[i]);

    strcat(fileDir,csvLists[i]);
    strcat(fileDir,".csv");
    FILE* stream=NULL;

    stream=fopen(fileDir,"r");
    if( stream != NULL )
    {   
        while( !feof( stream ) ){
            fgets( buf, 2048, stream );          
            printf( "%s", buf );            
        }       
    }
    fclose(stream);
}

int test=1;

int main(){
    printf("pootis \n");
    printf("strange %c %d \n",csvLists[0][1],test);
    readCSV(1);

    return 0;
}