#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //
#include <conio.h>   //getch 사용
#include <windows.h> //gotoxy 사용
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ESC 27
#define ENTER 13
#define SPACE 32
#define BACKSPACE 8
#define COL GetStdHandle(STD_OUTPUT_HANDLE)              // 콘솔창의 핸들정보 받기
#define BLACK SetConsoleTextAttribute(COL, 0x0000);      // 검정색
#define DARK_BLUE SetConsoleTextAttribute(COL, 0x0001);  // 파란색
#define GREEN SetConsoleTextAttribute(COL, 0x0002);      // 녹색
#define BLUE_GREEN SetConsoleTextAttribute(COL, 0x0003); // 청녹색
#define BLOOD SetConsoleTextAttribute(COL, 0x0004);      // 검붉은색
#define PURPLE SetConsoleTextAttribute(COL, 0x0005);     // 보라색
#define GOLD SetConsoleTextAttribute(COL, 0x0006);       // 금색
#define ORIGINAL SetConsoleTextAttribute(COL, 0x0007);   // 밝은 회색 (ORIGINAL CONSOLE COLOR)
#define BLUE SetConsoleTextAttribute(COL, 0x0009);       // 파란색
#define HIGH_GREEN SetConsoleTextAttribute(COL, 0x000a); // 연두색
#define SKY_BLUE SetConsoleTextAttribute(COL, 0x000b);   // 하늘색
#define RED SetConsoleTextAttribute(COL, 0x000c);        // 빨간색
#define PLUM SetConsoleTextAttribute(COL, 0x000d);       // 자주색
#define YELLOW SetConsoleTextAttribute(COL, 0x000e);     // 노란색
#define WHITE SetConsoleTextAttribute(COL, 0x000f);      // 흰색

#define N 600  // 노드의 갯수
#define m 5000 // 최대 값
FILE *fp;
int data[N][N];
typedef struct Node
{
    char numbername[N][20];
    int distance[N]; // 정점 까지의 거리
    int via[N];      // 이전 정점을 가리키는 포인터
    int startnumber;
    int destnumber;
} node;
void select(node subway, int ccolor, int v1, int v2);
int namecheck(char insert[], node *Subway); //노선도 역 이름과 입력받은 역 이름 비교하는 함수
void input(node *Subway, char start[], char dest[], int option);
void shortfind(node *Subway, int option);
void print(node subway, char start[], char dest[], int option);
void color(int path[], int i);
void main_print();         //초기 메뉴화면 보여주는 함수
void gotoxy(int x, int y); //커서의 위치를 제어하는 함수
int main()
{                         // GUI, 메뉴화면 커서와 키보드 제어
    node subway, *Subway; //구조체선언
    Subway = &subway;     //구조체포인터 Subway초기화
    int option;
    char start[30];
    char dest[30];
    unsigned int y = 10, y1 = 0;
    WHITE;
    main_print();
    gotoxy(15, 8);
    printf("▶");
    char cursor;
    while (1)
    {
        cursor = getch();
        switch (cursor)
        {
        case UP:
            system("cls");
            main_print();
            y1 = ((--y) % 3 + 8);
            gotoxy(15, y1);
            printf("▶");
            break;
        case DOWN:
            system("cls");
            main_print();
            y1 = ((++y) % 3 + 8);
            gotoxy(15, y1);
            printf("▶");
            break;
        case ENTER:
            system("cls");
            switch (y1)
            {
            case 8: // 첫번째 메뉴
                input(Subway, start, dest, 0);
                shortfind(Subway, 0);
                print(subway, start, dest, 0);
                break;
            case 9: // 두번째 메뉴
                input(Subway, start, dest, 0);
                shortfind(Subway, 1);
                print(subway, start, dest, 1);
                break;

            case 10: // 세번째 메뉴
                exit(0);
                break;
            }
            break;
        }
    }
}
void gotoxy(int x, int y) //커서의 위치를 제어하는 함수
{
    COORD Pos = {x - 1, y - 1}; // x와 y는 좌푤를 의미함
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
void main_print()
{
    GREEN;
    printf("┌─────────────────────────────────────────────┓\n");
    printf("│                                             ┃\n");
    printf("│                 ");
    WHITE;
    printf("지하철  노선도");
    GREEN;
    printf("              ┃\n");
    printf("│                                             ┃\n");
    printf("└─────────────────────────────────────────────┛\n\n\n");
    BLUE;
    printf("\t\t 1. 최단거리 검색\n");
    printf("\t\t 2. 최소환승 검색\n");
    printf("\t\t 3. 종료\n\n");
}
void input(node *Subway, char start[], char dest[], int option)
{
    int weight, num1, num2, w = 0;
    int i = 0, s, e, temp = 0;
    // 데이터 초기화
    for (s = 0; s < N; s++)
    {
        for (e = 0; e < N; e++)
        {
            if (s == e)
                data[s][e] = 0;
            else
                data[s][e] = m;
        }
    }
    fp = fopen("subway.txt", "r"); // subway파일불러오기
    while (w != EOF)
    {                                                      // eof는 파일의 끝을 의미
        w = fscanf(fp, "%d %d %d", &num1, &num2, &weight); // data이차행렬에 가중치넣기
        w = fscanf(fp, "%s", &Subway->numbername[num1]);
        data[num1 - 1][num2 - 1] = weight;
        data[num2 - 1][num1 - 1] = weight;
    }
    fclose(fp);
    if (option == 0)
    {
        GREEN;
        printf("┌─────────────────────────────────────────────┓\n");
        printf("│                                             ┃\n");
        printf("│                 ");
        WHITE;
        printf("지하철  노선도");
        GREEN;
        printf("              ┃\n");
        printf("│                                             ┃\n");
        printf("└─────────────────────────────────────────────┛\n\n\n");
        BLUE;
        //출발 및 도착역 고유번호 찾기
        while (1)
        {
            printf("\t 출발역을 입력하시오. : ");
            scanf("%s", start);
            if (namecheck(start, Subway))
                break;
            else
                printf("\t 잘못 입력하셨습니다. \n");
        }
        while (1)
        {

            printf("\n\t 도착역을 입력하시오. : ");
            scanf("%s", dest);
            if (namecheck(dest, Subway))
                break;
            else
                printf("\t 잘못 입력하셨습니다. \n");
        }
    }
    for (i = 0; i < N; i++)
    { //출발역이름에 따른 고유번호찾기
        if (!strcmp(Subway->numbername[temp], start))
        {
            Subway->startnumber = temp;
            break;
        }
        temp++;
    }
    temp = 0;
    for (i = 0; i < N; i++)
    { //도착역이름에 따른 고유번호찾기
        if (!strcmp(Subway->numbername[temp], dest))
        {
            Subway->destnumber = temp;
            break;
        }
        temp++;
    }
}
void shortfind(node *Subway, int option)
{                         //구조체안에 배열값을 변경하기위해
    int i = 0, j, k, min; //구조체포인터 사용(Call by reference)
    int v[N];
    //최소 환승일경우 데이터 초기화
    if (option == 1)
    {
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                if (i == j)
                {
                    data[i][j] = 0;
                }
                if (!strcmp(Subway->numbername[i + 1], Subway->numbername[j + 1]))
                {
                    data[i][j] += 500; //환승역일경우 가중치 500을 더해서 최소환승을 함
                }
            }
        }
    }
    // 시작지점으로 부터 각 지점까지의 최단거리 구하기
    for (j = 0; j < N; j++)
    {
        v[j] = 0;
        Subway->distance[j] = m;
    }
    // 2.시작 지점부터 시작 하도록 지정
    Subway->distance[Subway->startnumber - 1] = 0;
    // 3.정점의 수만큼 돈다.
    for (i = 0; i < N; i++)
    {
        // 4. 최단거리인 정점을 찾는다.
        min = m;
        for (j = 0; j < N; j++)
        {
            if (v[j] == 0 && Subway->distance[j] < min)
            {
                k = j;
                min = Subway->distance[j];
            }
        }
        v[k] = 1; // 최단거리가 확인된 정점
        if (min == m)
            break; // error 그래프가 연결되어있지 않음
                   // k를 경유해서 j에 이르는 경로가 최단거리이면 갱신
        for (j = 0; j < N; j++)
        {
            if (Subway->distance[j] > Subway->distance[k] + data[k][j])
            {
                Subway->distance[j] = Subway->distance[k] + data[k][j];
                Subway->via[j] = k;
            }
        }
    }
}
void print(node subway, char start[], char dest[], int option)
{ //값 변경이 없으므로 구조체를 매개변수로 사용 (call by value)
    int path[N], path_cnt = 0;
    int i = 0, k, temp = 600, j = 0;
    int count = 0;
    //이동경로 저장
    k = subway.destnumber - 1;
    while (1)
    {
        path[path_cnt++] = k; // path[]에 이동 경로 저장
        if (k == (subway.startnumber - 1))
            break;
        k = subway.via[k];
    }
    //이동 경로 출력
    WHITE;
    printf("\n 경로는 : ");
    while (!(strcmp(start, subway.numbername[path[path_cnt - 1] + 1])))
    { //출발역이 환승역일경우 예외처리
        path_cnt--;
        if (option == 1 && !(strcmp(start, subway.numbername[path[path_cnt - 1] + 1]))) //최소환승이며 출발역이 환승역일경우
        {
            subway.distance[subway.destnumber - 1] -= 500;
        }
    }
    for (i = path_cnt; i >= 1; i--)
    {
        if (!(strcmp(subway.numbername[temp], subway.numbername[path[i] + 1])))
        { //환승역 두번출력 제거
            count++;
            WHITE;
            j++;
            if (j % 4 == 0)
            {
                j = 0;
                printf("\n\t  ");
            }
            printf(" [환승] ");
            printf("─▶ ");
            if (option == 1)
            {
                subway.distance[subway.destnumber - 1] -= 500;
            }
            continue;
        }
        if (!(strcmp(dest, subway.numbername[path[i] + 1]))) //도착역이 환승역일경우 예외처리
        {
            if (option == 1 && !(strcmp(dest, subway.numbername[path[i] + 1]))) //최소환승이며 도착역이 환승역일경우
            {
                subway.distance[subway.destnumber - 1] -= 500;
            }
            break;
        }
        color(path, i);
        j++;
        if (j % 4 == 0)
        {
            j = 0;
            printf("\n\t  ");
        }
        printf("%s ", &subway.numbername[path[i] + 1]);
        WHITE;
        printf("─▶ ");
        temp = path[i] + 1;
    }
    j++;
    if (j % 4 == 0)
    {
        j = 0;
        printf("\n\t  ");
    }
    color(path, i);
    printf("%s\n\n", &subway.numbername[path[i] + 1]);
    WHITE;
    printf(" 환     승 : %d번\n", count);
    printf(" 소요 시간 : %d시간 %d분\n", (subway.distance[subway.destnumber - 1] / 60), (subway.distance[subway.destnumber - 1] % 60));
    getch();
    system("cls");
    /*if (option != 11) {
     save_input(start, dest);
    }*/
}
void color(int path[], int i)
{
    if ((path[i] + 1) <= 99)
    {
        BLUE;
    }
    else if ((path[i] + 1) <= 151)
    {
        GREEN;
    }
    else if ((path[i] + 1) <= 194)
    {
        RED;
    }
    else if ((path[i] + 1) <= 241)
    {
        DARK_BLUE;
    }
    else if ((path[i] + 1) <= 292)
    {
        PURPLE;
    }
    else if ((path[i] + 1) <= 330)
    {
        BLOOD;
    }
    else if ((path[i] + 1) <= 372)
    {
        HIGH_GREEN;
    }
    else if ((path[i] + 1) <= 389)
    {
        PLUM;
    }
    else if ((path[i] + 1) <= 413)
    {
        GOLD;
    }
    else if ((path[i] + 1) <= 433)
    {
        YELLOW;
    }
    else if ((path[i] + 1) <= 472)
    {
        SKY_BLUE;
    }
    else if ((path[i] + 1) <= 500)
    {
        BLUE_GREEN;
    }
    else if ((path[i] + 1) <= 520)
    {
        BLUE_GREEN;
    }
    else if ((path[i] + 1) <= 529)
    {
        WHITE;
    }
    else if ((path[i] + 1) <= 547)
    {
        HIGH_GREEN;
    }
}
int namecheck(char insert[], node *Subway)
{ //문자열 비교 함수 (subway.txt파일에서의 이름과 입력받은 문자열 비교
    int i;
    for (i = 0; i < N; i++)
    {
        if (!strcmp(insert, Subway->numbername[i])) // strcmp가 문자열 비교 함수이다, 일치하지 않을 때 1을 반환
            return 1;
    }
    return 0;
}
void select(node subway, int ccolor, int v1, int v2)
{
    int i = 0, count = 0;
    for (i = v1; i < v2; i++)
    {
        switch (ccolor)
        {
        case 1:
            DARK_BLUE;
            break;
        case 2:
            GREEN;
            break;
        case 3:
            BLUE_GREEN;
            break;
        case 4:
            BLOOD;
            break;
        case 5:
            PURPLE;
            break;
        case 6:
            GOLD;
            break;
        case 7:
            BLUE;
            break;
        case 8:
            HIGH_GREEN;
            break;
        case 9:
            SKY_BLUE;
            break;
        case 10:
            RED;
            break;
        case 11:
            PLUM;
            break;
        case 12:
            YELLOW;
            break;
        case 13:
            WHITE;
            break;
        }
        printf("%s ", &subway.numbername[i]);
        WHITE;

        printf("◀─▶");
        count++;
        if ((count % 5) == 0)
            printf("\n");
    }
    switch (ccolor)
    {
    case 1:
        DARK_BLUE;
        break;
    case 2:
        GREEN;
        break;
    case 3:
        BLUE_GREEN;
        break;
    case 4:
        BLOOD;
        break;
    case 5:
        PURPLE;
        break;
    case 6:
        GOLD;
        break;
    case 7:
        BLUE;
        break;
    case 8:
        HIGH_GREEN;
        break;
    case 9:
        SKY_BLUE;
        break;
    case 10:
        RED;
        break;
    case 11:
        PLUM;
        break;
    case 12:
        YELLOW;
        break;
    case 13:
        WHITE;
        break;
    }
    printf("%s", &subway.numbername[v2]);
}