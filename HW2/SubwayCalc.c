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
int R_sav=0; // 행렬 전역변수 (삽입용)
int C_sav=0; // 행렬 전역변수 (삽입용)

int tmp[R][C]; // 임시로 받을 전역행렬

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
        arr[i] =malloc(sizeof(element) * C);    // (int의 크기 * col)만큼 동적 메모리 할당. 배열의 가로
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



//해야할거) 구조체 행렬 제작하기


int main(){
    printf("pootis \n");
    readCSV(18);

    element** test= makeArray();
    initArray(test);
    strcpy(test[0][0].from,"아싸좋아요");
    printf("%s \n",test[0][0].from);
    //printf("%d",test[0][0].data);

    //system("PAUSE");
    killArray(test);
    printf("sus \n");
    return 0;
}