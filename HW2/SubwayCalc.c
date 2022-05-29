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
int Sub_Cnt=0; // ������ Ƚ�� �������� (ȯ�� �ð� ����)
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
int path[MAX_VERTICES]; // �ִܰŸ� ������ ��ġ�� ������ ����
int check[MAX_VERTICES];// �� ������ ���� ���� ǥ��


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

    //���⼭���ʹ� ����ü�� from to ����
    for (int i = 0; i < R; i++){
        for (int j = 0; j < R; j++){
            strcpy(arr[i][j].from,subinfo[i].code);
            strcpy(arr[i][j].to,subinfo[j].code);
        }
    }
    /*
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
    
    fclose(stream);*/
    
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

int choose(int distance[], int n, int found[]){
	int i, min, minpos;
	min = INT_MAX;
	minpos = -1;
	for (i = 0; i<n; i++)
		if (distance[i]< min && !found[i]) {
			min = distance[i];
			minpos = i;
		}
	return minpos;
}

void shortest_path(element** arr, int start){
	int i, u, w;
	for (i = 0; i<R; i++){  /* �ʱ�ȭ */
		distance[i] = arr[start][i].data;
		found[i] = FALSE;
		path[i] = start;
	}
	path[start] = -1;

	found[start] = TRUE;    /* ���� ���� �湮 ǥ�� */
	distance[start] = 0;
	for (i = 0; i<R-1; i++) {
		u = choose(distance, R, found);
		found[u] = TRUE;
		for (w = 0; w<R; w++)
			if (!found[w])
				if (distance[u] + arr[u][w].data < distance[w]){
					path[w]=u; //�ε��� ����
					distance[w] = distance[u] + arr[u][w].data;
				}
	}
}

int subChk(sublist subinfo[],char chk[]){
    for (int i=0;i<R;i++){
        if (strcmp(chk,subinfo[i].name)==0)
            return i;
    }
    return -1;
}

void print_path(int start, int end,sublist subinfo[],element** arr){
    
	printf("<���>\n");
	int i = end;
	int k = 0;
	int way[R];
	while (path[i] != -1){
		way[k++]=i;
		way[k++]=path[i];
		i = path[i];
	}
    printf("-><%s> %s\n", csvLists[subinfo[way[k-1]].num],subinfo[way[k-1]].name);
	for(int q = k - 1; q > 0; q=q-2){
        if(arr[q][q-1].ic==0){
            Sub_Time+=arr[q][q-1].data;
            Sub_Cnt++;
            printf("-><%s> %s\n", csvLists[subinfo[way[q-1]].num],subinfo[way[q-1]].name);
        }
        else{
            IC_Time+=arr[q][q-1].data;
            printf("-><ȯ�� : �ҿ�ð� %d ��> %s\n", arr[q][q-1].data,subinfo[way[q-1]].name);
        }
	}
	printf("\n\n");
    printf("�ҿ�ð� : %d (%d + ȯ�� �ҿ�ð� %d) ��\n",Sub_Time+IC_Time,Sub_Time,IC_Time);
    printf("������ �� : %d ��\n",Sub_Cnt);
}


int main(){
    printf("���α׷� ���� \n");
    element** subarray= makeArray();
    initArray(subarray);
    
    sublist subinfo[R];

    readSubInfo(subinfo);
    readSubArray(subarray,subinfo);

    int debug=0;
    if (debug==1){
        
        for(int i=0;i<R;i++){
            for (int j = 0; j < R; j++){
                if(subarray[i][j].data!=9999&&subarray[i][j].data!=0){
                    if(subarray[i][j].ic==TRUE){
                        printf("[%d -> %d] : %d ", i,j,subarray[i][j].data);
                        printf("[%s (%s) -> %s (%s)] ȯ��\n", subarray[i][j].from,csvLists[subinfo[i].num],subarray[i][j].to,csvLists[subinfo[j].num]);
                    }
                    else{
                        printf("[%d -> %d] : %d ", i,j,subarray[i][j].data);
                        printf("[%s -> %s]\n", subarray[i][j].from,subarray[i][j].to);
                    }
                }
            }
        }
        /*
        for(int i=0;i<R;i++){
            printf("%d) %s - %s (%s)\n",i,subinfo[i].code,subinfo[i].name,csvLists[subinfo[i].num]);
        }*/
    }
    int ok=FALSE;
    char sub1[100];
    char sub2[100];
    int sub1_idx;
    int sub2_idx;
    while(ok!=TRUE){
        printf("��߿��� �Է����ּ���: ");
        fgets(sub1,sizeof(sub1),stdin); //��߿� �Է�
        //sub1�� ������� �ʰ� 0��°�� \0�� �ƴϸ鼭 ���κ��� \n�ϰ��
        if(sub1 !=NULL && sub1[0]!='\0' && sub1[strlen(sub1)-1]=='\n')
            sub1[strlen(sub1)-1]='\0';

        printf("�������� �Է����ּ���: ");
        fgets(sub2,sizeof(sub2),stdin); //������ �Է� 
        //sub2�� ������� �ʰ� 0��°�� \0�� �ƴϸ鼭 ���κ��� \n�ϰ��
        if(sub2 !=NULL && sub2[0]!='\0' && sub2[strlen(sub2)-1]=='\n') 
            sub2[strlen(sub2)-1]='\0';

        if(strcmp(sub1,sub2)==0){
            printf("��߿��� �������� �����մϴ�! \n");
            printf("�ٽ� �Է����ּ���! \n");
            continue;
        }
        sub1_idx=subChk(subinfo,sub1);
        sub2_idx=subChk(subinfo,sub2);
        if (sub1_idx==-1||sub2_idx==-1){
            printf("��߿��Ǵ� �������� ��Ȯ���� �ʽ��ϴ�! \n");
            printf("�ٽ� �Է����ּ���! \n");
            continue;
        }
        printf("[%s (%s) -> %s (%s)] \n", subinfo[sub1_idx].name,csvLists[subinfo[sub1_idx].num],subinfo[sub2_idx].name,csvLists[subinfo[sub2_idx].num]);
        ok=TRUE;
    }

    shortest_path(subarray, sub1_idx);
	print_path(sub1_idx, sub2_idx, subinfo,subarray);

    killArray(subarray);
    printf("���α׷� ���� \n");

    return 0;
}