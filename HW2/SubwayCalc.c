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
const int IC=145; // ȯ�¿� ���� ���

//int tmp[R][C]; // �ӽ÷� ���� �������

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
        arr[i] =malloc(sizeof(element) * C);    // (element�� ũ�� * col)��ŭ ���� �޸� �Ҵ�. �迭�� ����
    }
    return arr;
}

void initArray(element** arr){
    for (int i = 0; i < R; i++){
        for (int j = 0; j < C; j++){
                strcpy(arr[i][j].from,""); // ����� ���
                strcpy(arr[i][j].to,""); // ������ ���
                arr[i][j].data=9999; // ����ġ 9999�� �ʱ�ȭ
                arr[i][j].ic=FALSE; // ȯ�� X�� �ʱ�ȭ
            }
    }
}

void killArray(element** arr){

    for (int i = 0; i < R; i++) {
        free(arr[i]);
    }
    free(arr);
}


FILE* readCSV(int i){
        char fileDir[50] = "./data/";
        strcat(fileDir, csvLists[i]);
        strcat(fileDir, ".csv");
        //printf("%s \n", fileDir);
        FILE *stream = fopen(fileDir, "r");
        if (stream != NULL){
            return stream;
        }
        else{
            printf("Failed to load %s !!!", fileDir);
            exit(1);
        }
}

void readSubArray(element** arr,sublist subinfo[]){
    int cRow=0;
    char *tmp, *line;
    char buf[2048];

    //������� ���� �Է�
    for (int i = 0; i < 18; i++){
        FILE *stream = readCSV(i);
        char *line, *tmp;
        line = fgets(buf, 2048, stream); // ���� �̸� ���� (�Ⱦ��� ĭ �Ÿ���)
        int tmpR = 0, tmpC = 0;
        while ((line = fgets(buf, 2048, stream)) != NULL){
            tmpC = 0;
            tmp = strtok(line, ",");
            tmp = strtok(NULL, ",");
            while (tmp != NULL){
                subinfo[cRow + tmpR].num=i;
                arr[cRow + tmpR][cRow + tmpC].data = atoi(tmp);
                tmp = strtok(NULL, ",");
                tmpC++;
            }
            tmpR++;
        }
        cRow = cRow + tmpR;
        fclose(stream);
    }

    //���⼭���ʹ� ȯ���ϴ� �ε��� ����
    FILE *stream = readCSV(19);
    int tmpCnt = 0;
    int tmpIC[IC];
    line = fgets(buf, 2048, stream);
    tmp = strtok(line, ",");
    //tmp = strtok(NULL, ","); //ȯ�� �ܾ� ��ŵ
    while (tmp != NULL){
        if(tmpCnt==IC-1) // ������ �ܾ��
                tmp[strlen(tmp) - 1] = '\0'; // \n�� �������ش�.
        for (int i = 0; i < R; i++){
            if (strcmp(tmp, subinfo[i].code) == 0){
                tmpIC[tmpCnt] = i;
                printf("%d) %s %d �ε��� [%s]\n", tmpCnt,tmp,i,subinfo[i].name);
                tmpCnt++;
            }
        }
        tmp = strtok(NULL, ",");
    }

    //���⼭���ʹ� ȯ�������� ������Ŀ� ����
    tmpCnt=0;
    int m;
    int n;
    int i=0;
    int j=0;
    //int tmpMtx[IC][IC];
    while ((line = fgets(buf, 2048, stream)) != NULL){
        j=0;
        tmp = strtok(line, ",");
        tmp = strtok(NULL, ",");
        while (tmp != NULL){
            if(j==IC-1) // ������ �ܾ��
                tmp[strlen(tmp) - 1] = '\0'; // \n�� �������ش�.
            m = tmpIC[i];
            n = tmpIC[j];
            if (atoi(tmp) != 9999){
                printf("%d(%s), ",n,subinfo[n].name);
                arr[m][n].data = atoi(tmp);
                arr[m][n].ic = TRUE;
            }
            if (j<IC)
                j++;
            tmp = strtok(NULL, ",");
        }
        printf("[%d:%s]\n",m,subinfo[m].name);
        //if (i<IC)
            i++;
    }
    fclose(stream);
    
}

void readSubInfo(sublist subinfo[]){
        char buf[2048];
        FILE *stream = readCSV(18);
        char *line, *tmp;
        line = fgets(buf, 2048, stream); // ���� �̸� ���� (�Ⱦ��� ĭ �Ÿ���)
        int idx=0;
        while ((line = fgets(buf, 2048, stream))!=NULL) {
            int i=0;
            tmp = strtok(buf, ","); //
            while (tmp != NULL){
                if (i == 0){
                    strcpy(subinfo[idx].code, tmp);
                }
                else if (i == 1){
                    if(idx!=R-1) // ���������� �ƴϸ�
                        tmp[strlen(tmp) - 1] = '\0'; // \n�� �������ش�.
                    strcpy(subinfo[idx].name, tmp);
                }
                i++;
                tmp = strtok(NULL, "");
            }
            idx++;
        }

        fclose(stream);
}




int main(){
    printf("Start \n");
    element** test= makeArray();
    initArray(test);
    
    sublist subinfo[R];

    readSubInfo(subinfo);
    readSubArray(test,subinfo);

    int debug=0;
    if (debug==1){
        
        for(int i=0;i<R;i++){
            for (int j = 0; j < R; j++){
                if(test[i][j].data!=9999&&test[i][j].data!=0){
                    if(test[i][j].ic==TRUE){
                        printf("[%d -> %d] : %d ", i,j,test[i][j].data);
                        printf("[%s (%s) -> %s (%s)] ȯ��\n", subinfo[i],csvLists[subinfo[i].num],subinfo[j],csvLists[subinfo[j].num]);
                    }
                    else{
                        /*
                        printf("[%d -> %d] : %d ", i,j,test[i][j].data);
                        printf("[%s -> %s]\n", subinfo[i],subinfo[j]);
                        */
                    }
                }
            }
        }
        /*
        for(int i=0;i<R;i++){
            printf("%d) %s - %s (%s)\n",i,subinfo[i].code,subinfo[i].name,csvLists[subinfo[i].num]);
        }*/

        printf("%s \n",test[0][0].from);
    }

    //system("PAUSE");
    killArray(test);

    printf("End \n");
    //system("pause");
    return 0;
}