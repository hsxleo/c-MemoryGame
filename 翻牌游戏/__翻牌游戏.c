//1. 用户可以创建账户。账户应为包含用户名，密码，游戏历史的结构体变量。所有的帐户存储在一个数据文件中
//2. 登录游戏后的选项：（1）开始新游戏（2）回顾游戏历史（3）清除历史（4）注销
//3. 尽量避免使用全局变量
//4. 开始前显示牌面给用户一段时间
//5. 用户选择的两张牌相同则保持翻开状态，不相同则在几秒后再扣住
//6. 记录尝试次数（翻了几次才结束游戏）

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>

struct player {
	char account[100];
	char password[100];
	int attempts[100];  //尝试次数
    int time[100];  //用时
};
//players[0].attempts[0]  用来记录玩家人数
//players[0].attempts[1]  用来记录当前玩家下标
//players[0].attempts[2]  用来记录当前玩家attempts[]的下标

void Menu(struct player players[]);
//读取文件
void Read(struct player players[]) {
    FILE* fp;
    if ((fp = fopen("players.text", "r")) == NULL) {
        fp = fopen("players.text", "w");
    }
    int i,j;
    for (i = 0; i <= players[0].attempts[0]; i++) {
        fread(&players[i], sizeof(struct player), 1, fp);
    }
    if (i == 0) {  //没有实际数据   全部清零
        for (i = 0; i < 100; i++) {
            strcpy(players[i].account, "");
            strcpy(players[i].password, "");
            for (j = 0; j < 100; j++) {
                players[i].attempts[j] = 0;
                players[i].time[j] = 0;
            }
        }
    }
    fclose(fp);
}

//保存文件
void Save(struct player players[]) {
    FILE* fp;
    fp = fopen("players.text", "r+");
    int i;
    for (i = 0; i <= players[0].attempts[0]; i++) {
        fwrite(&players[i], sizeof(struct player), 1, fp);
    }
    fclose(fp);
}

//判断用户是否存在  存在返回该玩家的数组下标   不存在返回-1
int IsExist(struct player players[],char account[],char password[]) {
    int i;
    for (i = 1; i <= players[0].attempts[0]; i++) {
        if (strcmp(players[i].account, account) == 0 && strcmp(players[i].password, password) == 0) {
            players[0].attempts[1] = i;   //用这个来记录当前玩家的下标
            return i;
        }
    }
    return -1;
}

//登录
void Login(struct player players[]) {
    char account[100], password[100];
    system("cls");
    printf("LoginForm\n\n");
    printf("Enter your account:");
    scanf("%s", account);
    printf("Enter your password:");
    scanf("%s", password);
    int i, j;
    while (IsExist(players,account,password) == -1) {
        printf("Incorrect account or password!\n");
        printf("Re-enter your account:");
        scanf("%s", account);
        printf("Re-enter your password:");
        scanf("%s", password);
    }

    //更新当前用户的索引信息
    players[0].attempts[1] = IsExist(players, account, password);
    for (i = 0; i < 100; i++) {
        if (players[players[0].attempts[1]].attempts[i] == 0) {
            players[0].attempts[2] == i;
        }
    }
}

//注册
void Register(struct player players[]) {
    char account[100], password[100],code[100];
    int i, j;
    system("cls");
    printf("RegisterForm\n\n");
    while(1) {
        printf("Enter the name of the account you want:");
        scanf("%s", account);
        while (1) {
            printf("Set your password:");
            scanf("%s", password);
            printf("Set your password again:");
            scanf("%s", code);
            if (strcmp(password, code) == 0) {
                break;
            }
            else {
                printf("\nThe two passwords are different!\n");
            }
        }
        if (IsExist(players, account, password) == -1) {
            break;
        }
        else {
            printf("\nThe account already exists!\n");
        }
    } 
    
    //账号密码合法 
    //创建账号
    players[0].attempts[0]++;
    players[0].attempts[1] = players[0].attempts[0];
    players[0].attempts[2] = 0;
    strcpy(players[players[0].attempts[0]].account, account);
    strcpy(players[players[0].attempts[0]].password, password);

    printf("\n\nAfter successful account registration, press any key to enter the game...");
    getch();
}

//绘制游戏
void DrawGame(struct player players[],char a[][4]) {
    system("cls");
    int i, j;
    for (i = 0; i < 4; i++) {
        printf("\t\t");
        for (j = 0; j < 4; j++) {
            printf("%d\t", 4 * i + j + 1);
        }
        printf("\n\n");
    }

    for (i = 0; i < 4; i++) {
        printf("\t\t");
        for (j = 0; j < 4; j++) {
            printf("%c\t", a[i][j]);
        }
        printf("\n\n");
    }

}

//玩游戏
void PlayGame(struct player players[]) {

    srand((unsigned)(time(NULL)));
    char a[4][4], b[4][4];
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            a[i][j] = '-';
            b[i][j] = '-';
        }
    }

    //生成牌
    int n = 8;
    while (n) {
        int x1 = rand() % 4;
        int y1 = rand() % 4;
        int x2 = rand() % 4;
        int y2 = rand() % 4;
        if ((x1 != x2 || y1 != y2 )&& b[x1][y1] == '-' && b[x2][y2] == '-') {
            n--;
            b[x1][y1] = '!' + n;
            b[x2][y2] = '!' + n;
        }
    }

    //显示5秒中
    DrawGame(players, b);
    printf("You have five seconds to remember!");
    Sleep(5000);

    //开始游戏
    n = 8;
    int attempts = 0;
    time_t start, end;
    
    start = time(NULL);
    while (n) {
        DrawGame(players, a);
        printf("Please enter two numbers:");
        int p, q;
        scanf("%d%d", &p, &q);
        attempts++;
        int x1, x2, y1, y2;
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (p == 4 * i + j + 1) {
                    x1 = i;
                    y1 = j;
                }
                if (q == 4 * i + j + 1) {
                    x2 = i;
                    y2 = j;
                }
            }
        }

        //显示选择那两张牌
        a[x1][y1] = b[x1][y1];
        a[x2][y2] = b[x2][y2];
        DrawGame(players, a);
        //不相同暂停1s继续
        if (b[x1][y1] != b[x2][y2]) {
            Sleep(1000);
            a[x1][y1] = '-';
            a[x2][y2] = '-';
        }
        else {  //相同
            n--;
        }
    }
    end = time(NULL);

    //游戏记录储存
    players[players[0].attempts[1]].attempts[players[0].attempts[2]] = attempts;
    players[players[0].attempts[1]].time[players[0].attempts[2]] = end-start;
    players[0].attempts[2]++;


    //游戏完成返回主菜单
    printf("Congratulations on your game victory,attempts: %d time: %ds\n", attempts, end - start);
    printf("Please press any key to return to the game menu...\n");
    getch();
}

//查看历史
void Check(struct player players[]) {
    system("cls");
    printf("Game history\n\n");
    printf("\t\tid\t\tattempts\t\ttime\n\n");
    int i;
    for (i = 0; i < players[0].attempts[2]; i++) {
        printf("\t\t%d\t\t%d\t\t\t%ds\n", i + 1, players[players[0].attempts[1]].attempts[i], players[players[0].attempts[1]].time[i]);
    }

    printf("Press any key to return to the game menu...\n");
    getch();
}

//清空历史
void Clear(struct  player players[]){
    system("cls");
    printf("Clear history\n\n");
    int i;
    for (i = 0; i < players[0].attempts[2]; i++) {
        players[players[0].attempts[1]].attempts[i] = 0;
        players[players[0].attempts[1]].time[i] = 0;
    }
    players[0].attempts[2] = 0;
    printf("The game has a clear record of success!\n");
    printf("Press any key to return to the game menu...\n");
    getch();
};

//注销账号
void Logout(struct player players[]) {
    system("cls");
    printf("Logout account\n\n");
    int i;
    for (i = players[0].attempts[1]; i < players[0].attempts[0]; i++) {
        players[i] = players[i + 1];
    }
    printf("Account cancelled successfully!\n");
    printf("Please press any key to exit the game...\n");
    getch();
    exit(0);
}

//菜单
void Menu(struct player players[]) {
    while (1) {
        system("cls");
        printf("Select from the following options:\n\n");
        printf("\t\t1. Start Playing\n\n");
        printf("\t\t2. Check your game history\n\n");
        printf("\t\t3. Clear game history\n\n");
        printf("\t\t4. The cancellation of account\n\n");
        printf("\t\tOption:");
        int a;
        scanf("%d", &a);
        switch (a) {
        case 1:
            PlayGame(players);
            Save(players);
            break;
        case 2:
            Check(players);
            Save(players);
            break;
        case 3:
            Clear(players);
            Save(players);
            break;
        case 4:
            Logout(players);
            break;
        }
    }
}

int main() {
	struct player players[100];
    //初始化结构体
    int i,j;
    for (i = 0; i < 100; i++) {
        strcpy(players[i].account, "");
        strcpy(players[i].password, "");
        for (j = 0; j < 100; j++) {
            players[i].attempts[j] = 0;
            players[i].time[j] = 0;
        }
    }

    Read(players);
    printf("Welcome to Find the Pairs, Test your memory:\n\n");
    printf("Select from the following options:\n\n");
    printf("\t\t'1' Login to your account\n");
    printf("\t\t'2' Create new account\n");
    printf("\t\t'3' Quit\n\n\n");
    printf("\t\tOption:");
    int a;
    scanf("%d", &a);
    switch (a) {
    case 1:
        Login(players);
        Menu(players);
        break;
    case 2:
        Register(players);
        Menu(players);
        break;
    case 3:
        return 0;
    }
    
}