// ����ö �ִܰŸ� �ּ�ȯ�� ���α׷�

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#define TRUE 1
#define FALSE 0
#define INF	9999
#define MAX_VERTICES 552 //����(��)�� ��

int Sub_Time=0; // �̵� �ð� �������� (ȯ�� �ð� ����)
int Sub_Cnt=0; // ������ Ƚ�� �������� (ȯ�� �ð� ����)
int IC_Time=0; // ȯ�� �ð� ��������
int option=1; // �켱��� ���� �������� (1=�ִܰŸ�, 2=�ּ�ȯ��)
int trans_done=0; // ����ġ �ΰ� ���� (�̹� ������� 1)

const int R=MAX_VERTICES; // ��� ���
const int C=MAX_VERTICES; // ��� ���
const int IC=150; // ȯ�¿� ���� ���

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

int distance[MAX_VERTICES]; // ���� �����κ����� �ִ� ��� �Ÿ�
int found[MAX_VERTICES]; // �湮�� �� ǥ��
int path[MAX_VERTICES]; // �ִܰŸ� ������ ��ġ�� ������ ����
int check[MAX_VERTICES];// �� ������ ���� ���� ǥ��


char *csvLists[20]={"1ȣ��","1����","2ȣ��","2����","3ȣ��","4ȣ��",
                    "5ȣ��","5����","6ȣ��","7ȣ��","8ȣ��","9ȣ��",
                    "�д缱","��õ1��","�߾Ӽ�","���ἱ","���Ǽ�","����ö��","���̸�","ȯ������"}; //�� ���ϸ� ����


/*
* �Լ���: makeArray()
* �� �� : ����
* ������: element**
* �� �� : ����ü �迭�� ���� �����ϴ� �Լ�
*/
element** makeArray(){
    element **arr = malloc(sizeof(element *) * R);   // ���� �����Ϳ� (element ������ ũ�� * row)��ŭ ���� �޸� �Ҵ�. �迭�� ����
    for (int i = 0; i < R; i++){            // ���� ũ�⸸ŭ �ݺ�
        arr[i] = malloc(sizeof(element) * C);    // (element�� ũ�� * col)��ŭ ���� �޸� �Ҵ�. �迭�� ����
    }
    return arr;
}


/*
* �Լ���: initArray(arr)
* �� �� : element**
* ������: void
* �� �� : ����ü �迭 ���θ� �ʱ�ȭ�ϴ� �Լ�
*/
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
/*
* �Լ���: killArray(arr)
* �� �� : element**
* ������: void
* �� �� : ����ü �迭�� �Ҵ� �����ϴ� �Լ�
*/
void killArray(element** arr){

    for (int i = 0; i < R; i++) {
        free(arr[i]);
    }
    free(arr);
}

/*
* �Լ���: readCSV(i)
* �� �� : int
* ������: FILE*
* �� �� : i��° ������ �о FILE*�� �����ϴ� �Լ�
*/
FILE* readCSV(int i){
        char fileDir[50] = "./data/"; // ���� ���
        strcat(fileDir, csvLists[i]); // i��° ���ϸ��� ���� ��ο� ����
        strcat(fileDir, ".csv"); // .csv�� ���� ��ο� ����
        FILE *stream = fopen(fileDir, "r"); // ���� ����� ������ �ҷ���
        if (stream != NULL){ //������ ������� ������
            return stream;
        }
        else{ //������ ����ְų� ������ �ִٸ�
            printf("Failed to load %s !!!", fileDir); //���� ���
            exit(1); //����
        }
}

/*
* �Լ���: readSubArray(arr,subinfo[])
* �� �� : element** , sublist
* ������: void
* �� �� : ������� ����ü�� ������ ����ö ���� ����ü ������ ȣ�� �ε����� ä��� �Լ�
*/
void readSubArray(element** arr,sublist subinfo[]){
    int cRow=0; // ���� �࿭ ���� �����ϴ� ����
    char *tmp, *line; //�ٰ� �ܾ� ���� ����
    char buf[2048]; // ���� ����
    srand(time(NULL));

    //������� ���� �Է�
    for (int i = 0; i < 18; i++){
        FILE *stream = readCSV(i);
        char *line, *tmp;
        line = fgets(buf, 2048, stream); // ���� �̸� ���� (�Ⱦ��� ĭ �Ÿ���)
        int tmpR = 0, tmpC = 0; //�ӽ� �� ��
        while ((line = fgets(buf, 2048, stream)) != NULL){ //���پ� ����
            tmpC = 0; //�ӽ� ���� 0���� �ʱ�ȭ
            tmp = strtok(line, ","); // ,�� ������ line�� ù �ܾ ����
            tmp = strtok(NULL, ","); //���� �ܾ ���� ( ù��° �� �ڵ� �پ�ѱ� )
            while (tmp != NULL){ // ���� �ܾ NULL�� �ƴϸ�
                subinfo[cRow + tmpR].num=i; // ~ȣ�� �ε����� ���� ����ü�� ����
                arr[cRow + tmpR][cRow + tmpC].data = atoi(tmp); // ������� ����ü�� cRow + tmpR,C�� �ε����� �ش� �� ����
                tmp = strtok(NULL, ","); //���� �ܾ ����
                tmpC++; // �ӽ� �� �߰�
            }
            tmpR++; // �ӽ��� �߰�
        }
        cRow = cRow + tmpR; // ���� ���� �ӽ� �ุŭ �����༭ ����
        fclose(stream); //���� �ݱ�
    }

    //���⼭���ʹ� ����ü�� from to ����
    for (int i = 0; i < R; i++){
        for (int j = 0; j < R; j++){
            strcpy(arr[i][j].from,subinfo[i].code); //arr�� i�࿡ ��� ���ڵ� ����
            strcpy(arr[i][j].to,subinfo[j].code); //arr�� j�࿡ ���� ���ڵ� ����
        }
    }
    
    //���⼭���ʹ� ȯ���ϴ� �ڵ��� �ε��� ����
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
                //printf("%d) %s %d �ε��� [%s]\n", tmpCnt,tmp,i,subinfo[i].name);
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
                //printf("%d(%s), ",n,subinfo[n].name);
                arr[m][n].data = (rand() % atoi(tmp)) + 1; // 1 ~ �ش� ������ ���� ������ ����
                arr[m][n].ic = TRUE;
            }
            if (j<IC)
                j++;
            tmp = strtok(NULL, ",");
        }
        //printf("[%d:%s]\n",m,subinfo[m].name);
        if (i<IC-2)
            i++;
    }
    fclose(stream);
}

/*
* �Լ���: readSubInfo(subinfo[])
* �� �� : sublist
* ������: void
* �� �� : ����ö ���� ����ü ������ ����� �ڵ带 ä��� �Լ�
*/
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

/*
* �Լ���: choose(distance[], n, found[])
* �� �� : int 3��
* ������: int
* �� �� : �Ÿ��� ���� ���� �ε����� �����ϴ� �Լ�
*/
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

/*
* �Լ���: shortest_path(arr, start)
* �� �� : element**, int
* ������: void
* �� �� : �ִܰŸ��� Ž���ϴ� �Լ�
*/
void shortest_path(element** arr, int start){
	int i, u, w;
	for (i = 0; i<R; i++){  /* �ʱ�ȭ */
		distance[i] = arr[start][i].data;
		found[i] = FALSE;
		path[i] = start;
	}
	path[start] = -1;
    if(option==2&&trans_done==0){ //�ּ�ȯ�� �ɼ��� ���
        for (i = 0; i<R; i++){
		    for (int j = 0; j<R; j++){
                if(arr[i][j].ic==1)// ȯ�¿��̸�
                    arr[i][j].data+=1000; //����ġ�� ũ�� ������Ŵ
	        }
	    }
        trans_done=1;
    }

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

/*
* �Լ���: subChk(subinfo[],chk[])
* �� �� : sublist,char
* ������: int
* �� �� : ���� �ε����� Ž���ϴ� �Լ�
*/
int subChk(sublist subinfo[],char chk[]){
    for (int i=0;i<R;i++){
        if (strcmp(chk,subinfo[i].name)==0) //�ش��ϴ� ���ڿ��� ã����
            return i; //�ش� �ε��� ���
    }
    return -1; //��Ͽ� ������ -1 ���
}

/*
* �Լ���: print_path(int start, int end,sublist subinfo[],element** arr)
* �� �� : int, int, sublist, element**
* ������: void
* �� �� : path[]�� Ž���ϸ鼭 ������� �̵� �ð��� ����ϴ� �Լ�
*/
void print_path(int start, int end,sublist subinfo[],element** arr){
    //�ʱ�ȭ �ܰ�
    Sub_Time=1;
    IC_Time=0;
    Sub_Cnt=1;
	int i = end;
	int k = 0;
    int limit = 0;
	int way[R];
    int Now;
	while (path[i] != -1){ // -1�� �����Ҷ����� �ε������� ����
		way[k++]=i; // �ε��� ����
		way[k++]=path[i]; // i �������� �̾����� �ε��� ����
		i = path[i]; // ����
	}
    //printf("(%d �ε���)", k-1);
    if(strcmp(subinfo[way[0]].name,subinfo[way[1]].name)==0) limit+=1; // �������� �������� �̸��� �����ϴٸ� = ȯ���̸� �Ѱ��� �� �а� �����.
    printf("<���>\n");
    printf("-><%s> %s\n", csvLists[subinfo[way[k-1]].num],subinfo[way[k-1]].name);
	for(int q = k-1; q > limit; q=q-2){
        if(arr[way[q]][way[q-1]].ic==0){ 
            // ȯ���� �ƴҰ��
            if(q<4||arr[way[q-2]][way[q-3]].ic==0){ // �������� ȯ�¿��� �ƴϸ� ����, ��Ʈ��Ŷ���� �߸��� ���� ����
                Now=arr[way[q]][way[q-1]].data;
                Sub_Time+=Now;
                //printf("(%d��)", Sub_Time);
                printf("-><%s> %s\n", csvLists[subinfo[way[q-1]].num],subinfo[way[q-1]].name);
            }
            Sub_Cnt++;
        }
        else{
            // ȯ���� ���
            if(q<4||arr[way[q-2]][way[q-3]].ic==0){ // �������� ȯ�¿�(�ߺ�)�� �ƴϸ� ����, ��Ʈ��Ŷ���� �߸��� ���� ����
                Now=arr[way[q]][way[q-1]].data;
                if(option==2) Now=arr[way[q]][way[q-1]].data-1000; // �ּ�ȯ���ϰ�� ����ġ�� ������ �����Ƿ� 1000�� ����
                IC_Time+=Now;
                printf("-><ȯ�� : �ҿ�ð� %d ��> %s\n", Now,subinfo[way[q-1]].name);
            }
        }
	}
	printf("\n");
    printf("�ҿ�ð� : %d (%d + ȯ�� �ҿ�ð� %d) ��\n",Sub_Time+IC_Time,Sub_Time,IC_Time);
    printf("������ �� : %d ��\n\n",Sub_Cnt);
}

/*
* �Լ���: calc_path(int start, int end,sublist subinfo[],element** arr)
* �� �� : int, int, sublist, element**
* ������: int
* �� �� : path[]�� Ž���ϸ鼭 �� �̵� �ð��� �����ϴ� �Լ�
*/
int calc_path(int start, int end,sublist subinfo[],element** arr){
    //�ʱ�ȭ �ܰ�
    shortest_path(arr, start);
    Sub_Time=1;
    IC_Time=0;
    Sub_Cnt=1;
	int i = end;
	int k = 0;
    int limit = 0;
	int way[R];
    int Now;
	while (path[i] != -1){ // -1�� �����Ҷ����� �ε������� ����
		way[k++]=i; // �ε��� ����
		way[k++]=path[i]; // i �������� �̾����� �ε��� ����
		i = path[i]; // ����
	}
    if(strcmp(subinfo[way[0]].name,subinfo[way[1]].name)==0) limit+=1; // �������� �������� �̸��� �����ϴٸ� = ȯ���̸� �Ѱ��� �� �а� �����.
	for(int q = k-1; q > limit; q=q-2){
        if(arr[way[q]][way[q-1]].ic==0){ 
            // ȯ���� �ƴҰ��
            if(q<4||arr[way[q-2]][way[q-3]].ic==0){ // �������� ȯ�¿��� �ƴϸ� ����, ��Ʈ��Ŷ���� �߸��� ���� ����
                Now=arr[way[q]][way[q-1]].data;
                Sub_Time+=Now;
            }
            Sub_Cnt++;
        }
        else{
            // ȯ���� ���
            if(q<4||arr[way[q-2]][way[q-3]].ic==0){ // �������� ȯ�¿�(�ߺ�)�� �ƴϸ� ����, ��Ʈ��Ŷ���� �߸��� ���� ����
                Now=arr[way[q]][way[q-1]].data;
                if(option==2) Now=arr[way[q]][way[q-1]].data-1000; // �ּ�ȯ���ϰ�� ����ġ�� ������ �����Ƿ� 1000�� ����?
                IC_Time+=Now;
            }
        }
	}
    return Sub_Time+IC_Time;
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
    
    char sub1[100]; // ��߿� �Է�
    char sub2[100]; // ������ �Է�
    int sub1_idx; // ��߿� �ε���
    int sub2_idx; // ������ �ε���
    int ov_idx[R]; // ��߿� ȯ�� �ߺ� �ε��� ���� �迭
    int ov_time[R]; // ��߿� ȯ�� �ߺ� �ð� ���� �迭
    int curIdx=0; // ��߿� ȯ�� �ߺ� �ε��� �迭 �ε���
    int min_index; // ��߿��� ȯ���ϰ�� ȯ�� ���� ���� �ִ��� �ε���
    int min_time; // ��߿��� ȯ���ϰ�� ȯ�� ���� ���� �ִ��� �ð�

    while(1){ // �� �̸��� �Է��ϴ� �κ� 
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
        break;
    }
    while(1){ // �켱 ����� �Է��ϴ� �κ� 
        printf("���? 1. �ִܰ�� 2. �ּ�ȯ��\n");
        printf(": ");
        char Chk_num[5]; // �켱��� ����
        fgets(Chk_num,sizeof(Chk_num),stdin); //��߿� �Է�
        //sub1�� ������� �ʰ� 0��°�� \0�� �ƴϸ鼭 ���κ��� \n�ϰ��
        if(Chk_num !=NULL && Chk_num[0]!='\0' && Chk_num[strlen(Chk_num)-1]=='\n')
            Chk_num[strlen(Chk_num)-1]='\0';

        if(atoi(Chk_num)!=1&&atoi(Chk_num)!=2){
            printf("�ùٸ��� ���� ���Դϴ�! \n");
            printf("�ٽ� �Է����ּ���! \n");
            continue;
        }
        else if(atoi(Chk_num)==1){
            option=1;
        }
        else{
            option=2;
        }
        break;
    }

    for(int i=0;i<R;i++){
        if(strcmp(sub1,subinfo[i].name)==0){ // �̸��� �����ϴٸ�
            ov_idx[curIdx]=i; // �ش� ��߿� �ε����� ����
            ov_time[curIdx++]=calc_path(i, sub2_idx,subinfo,subarray); // �ش� ��߿� �ҿ�ð� ����
        }
    }
    min_time = ov_time[0];// 0��° �迭�� ����
    min_index = ov_idx[0];
    if(curIdx>1){ //ȯ���̶��()
        for (int i = 1; i < curIdx; i++){
            if (ov_time[i] < min_time){ //�ּ��� ��� ȯ�¿� ã��
                min_time = ov_time[i];
                min_index = ov_idx[i];
            }
        }
    }
    
    shortest_path(subarray, min_index); // ���ͽ�Ʈ��
	print_path(min_index, sub2_idx, subinfo, subarray); // ���� ��� �Լ�

    killArray(subarray); //������� free
    printf("���α׷� ���� \n");

    return 0;
}