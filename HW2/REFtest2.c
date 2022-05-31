#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //
#include <conio.h>   //getch ���
#include <windows.h> //gotoxy ���
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ESC 27
#define ENTER 13
#define SPACE 32
#define BACKSPACE 8
#define COL GetStdHandle(STD_OUTPUT_HANDLE)              // �ܼ�â�� �ڵ����� �ޱ�
#define BLACK SetConsoleTextAttribute(COL, 0x0000);      // ������
#define DARK_BLUE SetConsoleTextAttribute(COL, 0x0001);  // �Ķ���
#define GREEN SetConsoleTextAttribute(COL, 0x0002);      // ���
#define BLUE_GREEN SetConsoleTextAttribute(COL, 0x0003); // û���
#define BLOOD SetConsoleTextAttribute(COL, 0x0004);      // �˺�����
#define PURPLE SetConsoleTextAttribute(COL, 0x0005);     // �����
#define GOLD SetConsoleTextAttribute(COL, 0x0006);       // �ݻ�
#define ORIGINAL SetConsoleTextAttribute(COL, 0x0007);   // ���� ȸ�� (ORIGINAL CONSOLE COLOR)
#define BLUE SetConsoleTextAttribute(COL, 0x0009);       // �Ķ���
#define HIGH_GREEN SetConsoleTextAttribute(COL, 0x000a); // ���λ�
#define SKY_BLUE SetConsoleTextAttribute(COL, 0x000b);   // �ϴû�
#define RED SetConsoleTextAttribute(COL, 0x000c);        // ������
#define PLUM SetConsoleTextAttribute(COL, 0x000d);       // ���ֻ�
#define YELLOW SetConsoleTextAttribute(COL, 0x000e);     // �����
#define WHITE SetConsoleTextAttribute(COL, 0x000f);      // ���

#define N 600  // ����� ����
#define m 5000 // �ִ� ��
FILE *fp;
int data[N][N];
typedef struct Node
{
    char numbername[N][20];
    int distance[N]; // ���� ������ �Ÿ�
    int via[N];      // ���� ������ ����Ű�� ������
    int startnumber;
    int destnumber;
} node;
void select(node subway, int ccolor, int v1, int v2);
int namecheck(char insert[], node *Subway); //�뼱�� �� �̸��� �Է¹��� �� �̸� ���ϴ� �Լ�
void input(node *Subway, char start[], char dest[], int option);
void shortfind(node *Subway, int option);
void print(node subway, char start[], char dest[], int option);
void color(int path[], int i);
void main_print();         //�ʱ� �޴�ȭ�� �����ִ� �Լ�
void gotoxy(int x, int y); //Ŀ���� ��ġ�� �����ϴ� �Լ�
int main()
{                         // GUI, �޴�ȭ�� Ŀ���� Ű���� ����
    node subway, *Subway; //����ü����
    Subway = &subway;     //����ü������ Subway�ʱ�ȭ
    int option;
    char start[30];
    char dest[30];
    unsigned int y = 10, y1 = 0;
    WHITE;
    main_print();
    gotoxy(15, 8);
    printf("��");
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
            printf("��");
            break;
        case DOWN:
            system("cls");
            main_print();
            y1 = ((++y) % 3 + 8);
            gotoxy(15, y1);
            printf("��");
            break;
        case ENTER:
            system("cls");
            switch (y1)
            {
            case 8: // ù��° �޴�
                input(Subway, start, dest, 0);
                shortfind(Subway, 0);
                print(subway, start, dest, 0);
                break;
            case 9: // �ι�° �޴�
                input(Subway, start, dest, 0);
                shortfind(Subway, 1);
                print(subway, start, dest, 1);
                break;

            case 10: // ����° �޴�
                exit(0);
                break;
            }
            break;
        }
    }
}
void gotoxy(int x, int y) //Ŀ���� ��ġ�� �����ϴ� �Լ�
{
    COORD Pos = {x - 1, y - 1}; // x�� y�� ��ǧ�� �ǹ���
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
void main_print()
{
    GREEN;
    printf("����������������������������������������������������������������������������������������������\n");
    printf("��                                             ��\n");
    printf("��                 ");
    WHITE;
    printf("����ö  �뼱��");
    GREEN;
    printf("              ��\n");
    printf("��                                             ��\n");
    printf("����������������������������������������������������������������������������������������������\n\n\n");
    BLUE;
    printf("\t\t 1. �ִܰŸ� �˻�\n");
    printf("\t\t 2. �ּ�ȯ�� �˻�\n");
    printf("\t\t 3. ����\n\n");
}
void input(node *Subway, char start[], char dest[], int option)
{
    int weight, num1, num2, w = 0;
    int i = 0, s, e, temp = 0;
    // ������ �ʱ�ȭ
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
    fp = fopen("subway.txt", "r"); // subway���Ϻҷ�����
    while (w != EOF)
    {                                                      // eof�� ������ ���� �ǹ�
        w = fscanf(fp, "%d %d %d", &num1, &num2, &weight); // data������Ŀ� ����ġ�ֱ�
        w = fscanf(fp, "%s", &Subway->numbername[num1]);
        data[num1 - 1][num2 - 1] = weight;
        data[num2 - 1][num1 - 1] = weight;
    }
    fclose(fp);
    if (option == 0)
    {
        GREEN;
        printf("����������������������������������������������������������������������������������������������\n");
        printf("��                                             ��\n");
        printf("��                 ");
        WHITE;
        printf("����ö  �뼱��");
        GREEN;
        printf("              ��\n");
        printf("��                                             ��\n");
        printf("����������������������������������������������������������������������������������������������\n\n\n");
        BLUE;
        //��� �� ������ ������ȣ ã��
        while (1)
        {
            printf("\t ��߿��� �Է��Ͻÿ�. : ");
            scanf("%s", start);
            if (namecheck(start, Subway))
                break;
            else
                printf("\t �߸� �Է��ϼ̽��ϴ�. \n");
        }
        while (1)
        {

            printf("\n\t �������� �Է��Ͻÿ�. : ");
            scanf("%s", dest);
            if (namecheck(dest, Subway))
                break;
            else
                printf("\t �߸� �Է��ϼ̽��ϴ�. \n");
        }
    }
    for (i = 0; i < N; i++)
    { //��߿��̸��� ���� ������ȣã��
        if (!strcmp(Subway->numbername[temp], start))
        {
            Subway->startnumber = temp;
            break;
        }
        temp++;
    }
    temp = 0;
    for (i = 0; i < N; i++)
    { //�������̸��� ���� ������ȣã��
        if (!strcmp(Subway->numbername[temp], dest))
        {
            Subway->destnumber = temp;
            break;
        }
        temp++;
    }
}
void shortfind(node *Subway, int option)
{                         //����ü�ȿ� �迭���� �����ϱ�����
    int i = 0, j, k, min; //����ü������ ���(Call by reference)
    int v[N];
    //�ּ� ȯ���ϰ�� ������ �ʱ�ȭ
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
                    data[i][j] += 500; //ȯ�¿��ϰ�� ����ġ 500�� ���ؼ� �ּ�ȯ���� ��
                }
            }
        }
    }
    // ������������ ���� �� ���������� �ִܰŸ� ���ϱ�
    for (j = 0; j < N; j++)
    {
        v[j] = 0;
        Subway->distance[j] = m;
    }
    // 2.���� �������� ���� �ϵ��� ����
    Subway->distance[Subway->startnumber - 1] = 0;
    // 3.������ ����ŭ ����.
    for (i = 0; i < N; i++)
    {
        // 4. �ִܰŸ��� ������ ã�´�.
        min = m;
        for (j = 0; j < N; j++)
        {
            if (v[j] == 0 && Subway->distance[j] < min)
            {
                k = j;
                min = Subway->distance[j];
            }
        }
        v[k] = 1; // �ִܰŸ��� Ȯ�ε� ����
        if (min == m)
            break; // error �׷����� ����Ǿ����� ����
                   // k�� �����ؼ� j�� �̸��� ��ΰ� �ִܰŸ��̸� ����
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
{ //�� ������ �����Ƿ� ����ü�� �Ű������� ��� (call by value)
    int path[N], path_cnt = 0;
    int i = 0, k, temp = 600, j = 0;
    int count = 0;
    //�̵���� ����
    k = subway.destnumber - 1;
    while (1)
    {
        path[path_cnt++] = k; // path[]�� �̵� ��� ����
        if (k == (subway.startnumber - 1))
            break;
        k = subway.via[k];
    }
    //�̵� ��� ���
    WHITE;
    printf("\n ��δ� : ");
    while (!(strcmp(start, subway.numbername[path[path_cnt - 1] + 1])))
    { //��߿��� ȯ�¿��ϰ�� ����ó��
        path_cnt--;
        if (option == 1 && !(strcmp(start, subway.numbername[path[path_cnt - 1] + 1]))) //�ּ�ȯ���̸� ��߿��� ȯ�¿��ϰ��
        {
            subway.distance[subway.destnumber - 1] -= 500;
        }
    }
    for (i = path_cnt; i >= 1; i--)
    {
        if (!(strcmp(subway.numbername[temp], subway.numbername[path[i] + 1])))
        { //ȯ�¿� �ι���� ����
            count++;
            WHITE;
            j++;
            if (j % 4 == 0)
            {
                j = 0;
                printf("\n\t  ");
            }
            printf(" [ȯ��] ");
            printf("���� ");
            if (option == 1)
            {
                subway.distance[subway.destnumber - 1] -= 500;
            }
            continue;
        }
        if (!(strcmp(dest, subway.numbername[path[i] + 1]))) //�������� ȯ�¿��ϰ�� ����ó��
        {
            if (option == 1 && !(strcmp(dest, subway.numbername[path[i] + 1]))) //�ּ�ȯ���̸� �������� ȯ�¿��ϰ��
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
        printf("���� ");
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
    printf(" ȯ     �� : %d��\n", count);
    printf(" �ҿ� �ð� : %d�ð� %d��\n", (subway.distance[subway.destnumber - 1] / 60), (subway.distance[subway.destnumber - 1] % 60));
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
{ //���ڿ� �� �Լ� (subway.txt���Ͽ����� �̸��� �Է¹��� ���ڿ� ��
    int i;
    for (i = 0; i < N; i++)
    {
        if (!strcmp(insert, Subway->numbername[i])) // strcmp�� ���ڿ� �� �Լ��̴�, ��ġ���� ���� �� 1�� ��ȯ
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

        printf("������");
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