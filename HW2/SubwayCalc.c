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

    strcat(fileDir,csvLists[i]);
    strcat(fileDir,".csv");
    
    printf("%s \n",fileDir);

    FILE* stream=NULL;
    stream=fopen(fileDir,"r");
    if( stream != NULL ){   
        while( !feof( stream ) ){
            fgets( buf, 2048, stream );          
            printf( "%s\n", buf );            
        }       
    }
    else{
        printf("Failed to load %s !!!",fileDir);
    }
    fclose(stream);
}


int main(){
    printf("pootis \n");
    readCSV(0);

    system("PAUSE");
    return 0;
}