// 지하철 최단거리 최소환승 프로그램

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define INF	9999

int Sub_Time=0; // 이동 시간 전역변수 (환승 시간 제외)
int IC_Time=0; // 환승 시간 전역변수
int IC_Cnt=0; // 환승 횟수 전역변수
int R=556; // 아마 행렬 전역변수
int C=556; // 아마 행렬 전역변수
int R_tmp=0; // 아마 행렬 전역변수
int C_tmp=0; // 아마 행렬 전역변수

typedef struct element{ // 구조체
    //char* name; // 역이름?
    char from[10]; // 출발지
    char to[10]; // 목적지
    int data; // 가는 시간
    int ic; // 환승 여부
} element;


//안쓸수도있음
/*
typedef struct GraphType {
	int n;	// 정점의 개수
	element** weight[][]; // 구조체 행렬
} GraphType;

//int distance[];
//int found[];		
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



//해야할거) 구조체 행렬 제작하기


int main(){
    printf("pootis \n");
    //readCSV(19);

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