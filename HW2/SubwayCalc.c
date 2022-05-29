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
int Sub_Cnt=0; // 정거장 횟수 전역변수 (환승 시간 제외)
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
int path[MAX_VERTICES]; // 최단거리 역까지 거치는 노드들을 저장
int check[MAX_VERTICES];// 한 역으로 가는 역을 표시


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

    //여기서부터는 구조체의 from to 저장
    for (int i = 0; i < R; i++){
        for (int j = 0; j < R; j++){
            strcpy(arr[i][j].from,subinfo[i].code);
            strcpy(arr[i][j].to,subinfo[j].code);
        }
    }
    /*
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
    
    fclose(stream);*/
    
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
	for (i = 0; i<R; i++){  /* 초기화 */
		distance[i] = arr[start][i].data;
		found[i] = FALSE;
		path[i] = start;
	}
	path[start] = -1;

	found[start] = TRUE;    /* 시작 정점 방문 표시 */
	distance[start] = 0;
	for (i = 0; i<R-1; i++) {
		u = choose(distance, R, found);
		found[u] = TRUE;
		for (w = 0; w<R; w++)
			if (!found[w])
				if (distance[u] + arr[u][w].data < distance[w]){
					path[w]=u; //인덱스 저장
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
    
	printf("<출발>\n");
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
            printf("-><환승 : 소요시간 %d 분> %s\n", arr[q][q-1].data,subinfo[way[q-1]].name);
        }
	}
	printf("\n\n");
    printf("소요시간 : %d (%d + 환승 소요시간 %d) 분\n",Sub_Time+IC_Time,Sub_Time,IC_Time);
    printf("정거장 수 : %d 개\n",Sub_Cnt);
}


int main(){
    printf("프로그램 시작 \n");
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
                        printf("[%s (%s) -> %s (%s)] 환승\n", subarray[i][j].from,csvLists[subinfo[i].num],subarray[i][j].to,csvLists[subinfo[j].num]);
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
        printf("출발역을 입력해주세요: ");
        fgets(sub1,sizeof(sub1),stdin); //출발역 입력
        //sub1이 비어있지 않고 0번째가 \0이 아니면서 끝부분이 \n일경우
        if(sub1 !=NULL && sub1[0]!='\0' && sub1[strlen(sub1)-1]=='\n')
            sub1[strlen(sub1)-1]='\0';

        printf("도착역을 입력해주세요: ");
        fgets(sub2,sizeof(sub2),stdin); //도착역 입력 
        //sub2가 비어있지 않고 0번째가 \0이 아니면서 끝부분이 \n일경우
        if(sub2 !=NULL && sub2[0]!='\0' && sub2[strlen(sub2)-1]=='\n') 
            sub2[strlen(sub2)-1]='\0';

        if(strcmp(sub1,sub2)==0){
            printf("출발역과 도착역이 동일합니다! \n");
            printf("다시 입력해주세요! \n");
            continue;
        }
        sub1_idx=subChk(subinfo,sub1);
        sub2_idx=subChk(subinfo,sub2);
        if (sub1_idx==-1||sub2_idx==-1){
            printf("출발역또는 도착역이 정확하지 않습니다! \n");
            printf("다시 입력해주세요! \n");
            continue;
        }
        printf("[%s (%s) -> %s (%s)] \n", subinfo[sub1_idx].name,csvLists[subinfo[sub1_idx].num],subinfo[sub2_idx].name,csvLists[subinfo[sub2_idx].num]);
        ok=TRUE;
    }

    shortest_path(subarray, sub1_idx);
	print_path(sub1_idx, sub2_idx, subinfo,subarray);

    killArray(subarray);
    printf("프로그램 종료 \n");

    return 0;
}