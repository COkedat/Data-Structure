// ����ö �ִܰŸ� �ּ�ȯ�� ���α׷�

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define TRUE 1
#define FALSE 0
#define INF	9999
#define MAX_VERTICES 552 //����(��)�� ��

int Sub_Time=0; // �̵� �ð� �������� (ȯ�� �ð� ����)
int IC_Time=0; // ȯ�� �ð� ��������
int IC_Cnt=0; // ȯ�� Ƚ�� ��������
const int R=MAX_VERTICES; // ��� ���
const int C=MAX_VERTICES; // ��� ���
int R_sav=0; // ��� �������� (���Կ�)
int C_sav=0; // ��� �������� (���Կ�)

int tmp[R][C]; // �ӽ÷� ���� �������

typedef struct sublist{ // ���� ����ü
    char name[30]; // ����
    char code[10]; // �� �ڵ�
    int num; // ȣ�� ��ȣ
} sublist;

typedef struct element{ // ������� ����ü
    //char* name; // ���̸�?
    char from[10]; // �����
    char to[10]; // ������
    int data; // ���� �ð�
    int ic; // ȯ�� ����
} element;

int distance[MAX_VERTICES]; //���� �����κ����� �ִ� ��� �Ÿ�
int found[MAX_VERTICES]; // �湮�� �� ǥ��
int path[MAX_VERTICES][MAX_VERTICES]; // �ִܰŸ� ������ ��ġ�� ������ ����
int check[MAX_VERTICES];// �� ������ ���� ���� ǥ��

//�Ⱦ��Ͱ���
/*
typedef struct GraphType {
	int n;	// ������ ����
	element** weight[][]; // ����ü ���
} GraphType;	
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