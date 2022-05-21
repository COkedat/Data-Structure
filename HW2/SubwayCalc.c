// 지하철 최단거리 최소환승 프로그램

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct element{ // 구조체
    char from; // 출발지
    char to; // 목적지
    int data; // 가는 시간
    int ic; // 환승 여부
} element;

char *csvLists[20]={"1호선","2호선","3호선","4호선","5호선","6호선",
                    "7호선","8호선","9호선","1호선","2호선","5호선",
                    "경의선","경춘선","공항철도","분당선","인천1선","중앙선","역이름","환승정보"};


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