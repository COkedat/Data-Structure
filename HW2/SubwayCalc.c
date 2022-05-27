// 지하철 최단거리 최소환승 프로그램

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define TRUE 1
#define FALSE 0
#define INF	9999
#define MAX_VERTICES 552 //정점(역)의 수

int Sub_Time=0; // 이동 시간 전역변수 (환승 시간 제외)
int IC_Time=0; // 환승 시간 전역변수
int IC_Cnt=0; // 환승 횟수 전역변수
const int R=MAX_VERTICES; // 행렬 상수
const int C=MAX_VERTICES; // 행렬 상수
const int IC=145; // 환승역 개수 상수

//int tmp[R][C]; // 임시로 받을 전역행렬

typedef struct sublist{ // 역명 구조체
    char name[30]; // 역명
    char code[10]; // 역 코드
    int num; // 호선 번호
} sublist;

typedef struct element{ // 인접행렬 구조체
    //char* name; // 역이름?
    char from[10]; // 출발지
    char to[10]; // 목적지
    int data; // 가는 시간
    int ic; // 환승 여부
} element;

int distance[MAX_VERTICES]; //시작 역으로부터의 최단 경로 거리
int found[MAX_VERTICES]; // 방문한 역 표시
int path[MAX_VERTICES][MAX_VERTICES]; // 최단거리 역까지 거치는 노드들을 저장
int check[MAX_VERTICES];// 한 역으로 가는 역을 표시

//안쓸것같음
/*
typedef struct GraphType {
	int n;	// 정점의 개수
	element** weight[][]; // 구조체 행렬
} GraphType;	
*/

char *csvLists[20]={"1호선","1지선","2호선","2지선","3호선","4호선",
                    "5호선","5지선","6호선","7호선","8호선","9호선",
                    "분당선","인천1선","중앙선","경춘선","경의선","공항철도","역이름","환승정보"};



element** makeArray(){
    element **arr = malloc(sizeof(element *) * R);   // 이중 포인터에 (element 포인터 크기 * row)만큼 동적 메모리 할당. 배열의 세로
    for (int i = 0; i < R; i++)            // 세로 크기만큼 반복
    {
        arr[i] =malloc(sizeof(element) * C);    // (element의 크기 * col)만큼 동적 메모리 할당. 배열의 가로
    }
    return arr;
}

void initArray(element** arr){
    for (int i = 0; i < R; i++){
        for (int j = 0; j < C; j++){
                strcpy(arr[i][j].from,""); // 출발지 비움
                strcpy(arr[i][j].to,""); // 목적지 비움
                arr[i][j].data=9999; // 가중치 9999로 초기화
                arr[i][j].ic=FALSE; // 환승 X로 초기화
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

    //인접행렬 정보 입력
    for (int i = 0; i < 18; i++){
        FILE *stream = readCSV(i);
        char *line, *tmp;
        line = fgets(buf, 2048, stream); // 한줄 미리 읽음 (안쓰는 칸 거르기)
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

    //여기서부터는 환승하는 인덱스 저장
    FILE *stream = readCSV(19);
    int tmpCnt = 0;
    int tmpIC[IC];
    line = fgets(buf, 2048, stream);
    tmp = strtok(line, ",");
    //tmp = strtok(NULL, ","); //환승 단어 스킵
    while (tmp != NULL){
        if(tmpCnt==IC-1) // 마지막 단어면
                tmp[strlen(tmp) - 1] = '\0'; // \n을 제거해준다.
        for (int i = 0; i < R; i++){
            if (strcmp(tmp, subinfo[i].code) == 0){
                tmpIC[tmpCnt] = i;
                printf("%d) %s %d 인덱스 [%s]\n", tmpCnt,tmp,i,subinfo[i].name);
                tmpCnt++;
            }
        }
        tmp = strtok(NULL, ",");
    }

    //여기서부터는 환승정보를 인접행렬에 저장
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
            if(j==IC-1) // 마지막 단어면
                tmp[strlen(tmp) - 1] = '\0'; // \n을 제거해준다.
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
        line = fgets(buf, 2048, stream); // 한줄 미리 읽음 (안쓰는 칸 거르기)
        int idx=0;
        while ((line = fgets(buf, 2048, stream))!=NULL) {
            int i=0;
            tmp = strtok(buf, ","); //
            while (tmp != NULL){
                if (i == 0){
                    strcpy(subinfo[idx].code, tmp);
                }
                else if (i == 1){
                    if(idx!=R-1) // 마지막줄이 아니면
                        tmp[strlen(tmp) - 1] = '\0'; // \n을 제거해준다.
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
                        printf("[%s (%s) -> %s (%s)] 환승\n", subinfo[i],csvLists[subinfo[i].num],subinfo[j],csvLists[subinfo[j].num]);
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