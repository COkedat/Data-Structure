// ����ö �ִܰŸ� �ּ�ȯ�� ���α׷�

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define TRUE 1
#define FALSE 0
#define INF	9999

int Sub_Time=0; // �̵� �ð� �������� (ȯ�� �ð� ����)
int IC_Time=0; // ȯ�� �ð� ��������
int IC_Cnt=0; // ȯ�� Ƚ�� ��������
int R=556; // �Ƹ� ��� ��������
int C=556; // �Ƹ� ��� ��������
int R_tmp=0; // �Ƹ� ��� ��������
int C_tmp=0; // �Ƹ� ��� ��������

typedef struct element{ // ����ü
    //char* name; // ���̸�?
    char from[10]; // �����
    char to[10]; // ������
    int data; // ���� �ð�
    int ic; // ȯ�� ����
} element;


//�Ⱦ���������
/*
typedef struct GraphType {
	int n;	// ������ ����
	element** weight[][]; // ����ü ���
} GraphType;

//int distance[];
//int found[];		
*/

char *csvLists[20]={"1ȣ��","1����","2ȣ��","2����","3ȣ��","4ȣ��",
                    "5ȣ��","5����","6ȣ��","7ȣ��","8ȣ��","9ȣ��",
                    "�д缱","��õ1��","�߾Ӽ�","���ἱ","���Ǽ�","����ö��","���̸�","ȯ������"};



element** makeArray(){
    element **arr = malloc(sizeof(element *) * R);   // ���� �����Ϳ� (element ������ ũ�� * row)��ŭ ���� �޸� �Ҵ�. �迭�� ����
    for (int i = 0; i < R; i++)            // ���� ũ�⸸ŭ �ݺ�
    {
        arr[i] =malloc(sizeof(element) * C);    // (int�� ũ�� * col)��ŭ ���� �޸� �Ҵ�. �迭�� ����
    }
    return arr;
}

void initArray(element** arr){
    for (int i = 0; i < R; i++){
        for (int j = 0; j < C; j++){
                strcpy(arr[i][j].from,"");
                strcpy(arr[i][j].to,"");
                arr[i][j].data=9999;
                arr[i][j].ic=FALSE;
            }
    }
}

void killArray(element** arr){

    for (int i = 0; i < R; i++) {
        free(arr[i]);
    }
    free(arr);
}

element makeElement(){

}

void readCSV(int i){
    char buf[2048];
    char fileDir[50]="./data/";

    strcat(fileDir,csvLists[i]);
    strcat(fileDir,".csv");
    
    printf("%s \n",fileDir);

    FILE* stream=NULL;
    setlocale(LC_ALL, "UTF-8");
    stream=fopen(fileDir,"r");

    if( stream != NULL ){   
        while( fgets( buf, 2048, stream )!=NULL ){ 
            fgets( buf, 2048, stream );          
            printf("%s\n", buf );             
        }
	} 
    else{
        printf("Failed to load %s !!!",fileDir);
    }
    fclose(stream);
}



//�ؾ��Ұ�) ����ü ��� �����ϱ�


int main(){
    printf("pootis \n");
    readCSV(18);

    element** test= makeArray();
    initArray(test);
    strcpy(test[0][0].from,"�ƽ����ƿ�");
    printf("%s \n",test[0][0].from);
    //printf("%d",test[0][0].data);

    //system("PAUSE");
    killArray(test);
    printf("sus \n");
    return 0;
}