#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>

struct user   //�û��˺�����
{
    char num[40]; //�˺�
    char code[30];//����
    int score44[100];   //4*4�ķ���
    int time44[100];  
    int index44;
    int score66[100];   //6*6�ķ���
    int time66[100];
    int index66;
    int score88[100];   //8*8�ķ���
    int time88[100];
    int index88;
};

int readFile(struct user users[]);//��ȡ�ļ�
void saveFile(struct user users[], int usersLen); //�����ļ�
int loginUser(struct user users[], int usersLen); //�û���¼
int createUser(struct user users[], int& usersLen); //�������û�
void clearScore(struct user users[], int index);  //�����Ϸ��ʷ
void gamePlay(struct user users[], int& usersLen, int index, int size);//��Ϸ����
void checkGame(struct user users[], int& usersLen, int &index);  //�鿴��¼
void logoutUser(struct user users[], int& usersLen, int index);  //ע���˺�

//�����Ϸ��ʷ
void clearScore(struct user users[], int index) {
    memset(users[index].score44, 0, 100); 
    memset(users[index].score66, 0, 100); 
    memset(users[index].score88, 0, 100); 
    memset(users[index].time44, 0, 100);
    memset(users[index].time66, 0, 100);
    memset(users[index].time88, 0, 100);
    users[index].index44 = 0;
    users[index].index66 = 0;
    users[index].index88 = 0;

}

//�������û�
int createUser(struct user users[],int &usersLen) {
    char num[40], code1[30], code2[30];
    int i, j;
    system("cls");
    printf("        Registration Page\n\n");
    printf("        Please enter the account name you would like to use:");
    scanf("%s", num);
    printf("\n\n\nSearching database this may take a moment, please wait...\n\n");
    while (1) {
        int flag = 0;
        for (i = 0; i < usersLen; i++) {
            if (strcmp(users[i].num, num) == 0) {
                flag = 1;
                printf("The user name already exists, please re-create:");
                scanf("%s", num);
                break;
            }
        }
        if (flag == 0) {
            break;
        }
    }
    printf("Please enter the password you would like to use:");
    scanf("%s", code1);
    printf("\n\n\nPlease enter your password again:");
    scanf("%s", code2);
    printf("\n\n\nyou have successfully created and credited your account, you are now logged in...");

    //�����û����ݵĴ���
    clearScore(users, usersLen);
    strcpy(users[usersLen].num, num);
    strcpy(users[usersLen].code, code1);
    usersLen++;

    _getch();
    return usersLen - 1;
}

//��ȡ�ļ�
int readFile(struct user users[]) {
    FILE* fp;   //�ļ�ָ��
    int len = 0;  //�洢�ṹ�峤��
    struct user tmp;
    //�ж��˺������ļ��Ƿ���ڣ������ڴ��ļ������򴴽�һ���ļ�
    if ((fp = fopen("account_code.bin", "rb")) == NULL) {
        if ((fp = fopen("account_code.bin", "wb")) == NULL) {
            printf("Failed to create account password file!\n");
            exit(-1);
        }
    }
    //���ļ����ݶ����û��ṹ�����飬���ر��ļ�
    else {
        //���ļ�����ȫ�������û��ṹ��������
        while (1) {
            fread(&tmp, sizeof(struct user), 1, fp);
            if (feof(fp) == 0) {
                fseek(fp, -sizeof(struct user), SEEK_CUR);
                fread(&users[len++], sizeof(struct user), 1, fp);
            }
            else {
                break;
            }

        }
    }
    fclose(fp);
    return len;
}

//�����ļ�
void saveFile(struct user users[], int usersLen) {
    FILE* fp; //�ļ�ָ��
    if ((fp = fopen("account_code.bin", "rb+")) == NULL) {
        printf("Failed to save account password file!\n");
        exit(-1);
    }
    rewind(fp);  //�ļ�ָ��ص��ļ�ͷ��
    int i;    
    //����Ʒ�ṹ�������ȫ������д���û��ļ�
    for (i = 0; i < usersLen; i++) {
        fwrite(&users[i], sizeof(struct user), 1, fp);
    }
    fclose(fp);
}

//��ʼ�˵�
int beginMenu() {
    printf("Welcome to Find the Pairs, Test your memory:\n\n");
    printf("Select from the following options:\n\n");
    printf("      '1' Login to your account\n");
    printf("      '2' Create new account\n");
    printf("      '3' Quit\n\n\n");
    printf("      Option:");
    int a,b;
	while (1) {
		b = scanf("%d", &a);    //�����û����������
		if (b == 1)       //�������ݺϷ�ʱ
		{
			if (a >= 1 && a <= 3) {
				return a;  //�����û�ѡ��Ĺ������
			}
			else {
				printf("      There is an error in the input data, please re-enter:");
			}
		}
		else           //�������ݲ��Ϸ���ʱ��
		{
			rewind(stdin);   // ��ջ�����
			printf("      The input data is not valid. Please re-enter:");
		}
	}
	return a;
}

//�û���¼
int loginUser(struct user users[], int usersLen) {
    char num[40], code[30];
    int i, j;
    printf("        Login screen\n\n");
    printf("        Please enter your account number:");
    scanf("%s", num);
    printf("        Please enter your password:");
    scanf("%s", code);
    while (1) {
        for (i = 0; i < usersLen; i++) {
            if (strcmp(users[i].num, num) == 0 && strcmp(users[i].code,code) == 0) {   //�˺����붼��ͬ
                return i;
            }
        }
        printf("Incorrect account or password!\n");
        printf("        Please enter your account number:");
        scanf("%s", num);
        printf("        Please enter your password:");
        scanf("%s", code);
    }
}

//���˵�
void mainMenu(struct user users[], int &usersLen,int &index) {
    system("cls");
    printf("Select from the following options:\n\n");
    printf("      1. Start Playing\n\n");
    printf("      2. Check your game history\n\n");
    printf("      3. Save the file\n\n");
    printf("      4. Clear game history\n\n");
    printf("      5. Logout this account\n\n\n");
    printf("      Option:");
    int a, b;
    while (1) {
        b = scanf("%d", &a);    //�����û����������
        if (b == 1)       //�������ݺϷ�ʱ
        {
            if (a >= 1 && a <= 5) {
                break;  //�����û�ѡ��Ĺ������
            }
            else {
                printf("      There is an error in the input data, please re-enter:");
            }
        }
        else           //�������ݲ��Ϸ���ʱ��
        {
            rewind(stdin);   // ��ջ�����
            printf("      The input data is not valid. Please re-enter:");
        }
    }
    switch (a) {
    case 1:
        printf("Would you like to play:\n");
        printf("          1 Beginner(4*4)\n");
        printf("          2 Intermediate(6*6)\n");
        printf("          3 Expert(8*8)\n");
        printf("selection:");
        int c;
        scanf("%d", &c);
        if (c == 1) {  //Beginner(4*4)
            gamePlay(users,usersLen,index,4);
        }
        else if (c == 2) {  //Intermediate(6*6)
            gamePlay(users, usersLen,index,6);
        }
        else if (c == 3) {  //Expert(8*8)
            gamePlay(users, usersLen,index,8);
        }
        break;
    case 2:
        checkGame(users, usersLen, index);
        break;
    case 3:
        saveFile(users, usersLen);
        printf("File saved successfully!");
        printf("Press any key to continue:");
        _getch();
        mainMenu(users, usersLen, index);
        break;
    case 4:
        clearScore(users, index);
        saveFile(users, usersLen);
        printf("Press any key to continue:");
        _getch();
        mainMenu(users, usersLen, index);
        break;
    case 5:
        logoutUser(users, usersLen,index);
        saveFile(users, usersLen);
        printf("Account logout successful, the game is being launched...");
        _getch();
        break;
    }
}

int main() {
	struct user users[100];   //�û�
	int usersLen; //�û�����
	int answer;   //�û��ڿ�ʼ�˵���ѡ��
    int index;   //��ǰ�û����±�
    usersLen = readFile(users);   //��ȡ�ļ�
    answer = beginMenu();   //��ʼ�˵�
	switch (answer) {   //��ʼ�˵���ѡ��
	case 1:
        index = loginUser(users, usersLen);  //�û���¼
        mainMenu(users, usersLen,index);
		break;
	case 2:
        index = createUser(users, usersLen);  //�������û�
        mainMenu(users, usersLen,index);
		break;
	case 3:
		exit(0);
		break;
	}
}

//��Ϸ����
void printGame(char ch[][10], int size) {
    int i, j;
    system("cls");  //����
    //��ӡ��ʾ����
    for (i = 0; i < size; i++) {
        printf("\n      ");
        for (j = 0; j < size; j++) {
            printf("%-5d", i * 4 + j + 1);
        }
        printf("\n");
    }
    printf("\n");
    for (i = 0; i < size; i++) {
        printf("\n      ");
        for (j = 0; j < size; j++) {
            printf("%-5c", ch[i][j]);
        }
        printf("\n");
    }
}

//��Ϸʤ��
bool isWin(char ch[][10], int size)     {
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (ch[i][j] == '~') {
                return false;
            }
        }
    }
    return true;
}

//��Ϸ����
void gamePlay(struct user users[], int& usersLen,int index,int size) {
    int attempts = 0;  //����
    int _time;  //ʱ��
    char ch[10][10];
    char ch_rand[10][10];  //������ɵ���������
    time_t start, end;
    //��ʼ��ch����
    int i, j;
    int a, b;
    srand(time(NULL));
    //��ʼ����������
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            ch[i][j] = '~';
            ch_rand[i][j] = '~';
        }
    }
    int rand_count = 0;
    char card = '!';  //���Ƶ�ֵ
    while (!isWin(ch_rand,size)) {
        //������ɵ������±�
        int tx1 = rand() % size;  
        int ty1 = rand() % size;
        int tx2 = rand() % size;
        int ty2 = rand() % size;
        //������û��ֵ �Ҳ���ͬһ����
        if (tx1 != tx2 || ty1 != ty2) {
            if (ch_rand[tx1][ty1] == '~' && ch_rand[tx2][ty2] == '~') {
                ch_rand[tx1][ty1] = card + rand_count;
                ch_rand[tx2][ty2] = card + rand_count;
                rand_count++;
            }
        }
        
    }
    printGame(ch, size);
    start = time(NULL);  //��ʼ��ʱ
    while (!isWin(ch, size)) {
        printf("Please select two Numbers:");
        scanf("%d%d", &a, &b);
        int tx1 = (a - 1) / size;
        int ty1 = a % size - 1;
        if (a % size == 0) {
            ty1 = size - 1;
        }
        int tx2 = (b - 1) / size;
        int ty2 = b % size - 1;
        if (b % size == 0) {
            ty2 = size - 1;
        }
        attempts++;
        //�ж��������Ƿ����
        if (ch_rand[tx1][ty1] == ch_rand[tx2][ty2]) {
            ch[tx1][ty1] = ch_rand[tx1][ty1];
            ch[tx2][ty2] = ch_rand[tx2][ty2];
            printGame(ch, size);  //��ʾ����
            rand_count--;
        }
        else {
            ch[tx1][ty1] = ch_rand[tx1][ty1];
            ch[tx2][ty2] = ch_rand[tx2][ty2];
            printGame(ch, size); //��ʾ����
            Sleep(800);  //��ͣһС��
            //��ԭ����
            ch[tx1][ty1] = '~';
            ch[tx2][ty2] = '~';
            printGame(ch, size); //��ʾ����
        }
    }
    end = time(NULL);
    _time = end - start;  //ʱ����

    //д���ļ�
    if (size == 4) {
        users[index].score44[users[index].index44] = attempts;
        users[index].time44[users[index].index44] = _time;
        users[index].index44++;
    }
    else if (size == 6) {
        users[index].score66[users[index].index66] = attempts;
        users[index].time66[users[index].index66] = _time;
        users[index].index66++;
    }
    else if (size == 8) {
        users[index].score88[users[index].index88] = attempts;
        users[index].time88[users[index].index88] = _time;
        users[index].index88++;
    }
    saveFile(users,usersLen);  //���浽�ļ�����


    printf("\n\nCongratulations you have completed the board in %d attempts and %d seconds\n\n\n",attempts,_time);
    printf("Press 'n' to exit or any other key to play again:");
    char choose;
    rewind(stdin);   // ��ջ�����
    scanf("%c", &choose);
    if (choose == 'n') {
        mainMenu(users, usersLen, index);
    }
    else {
        gamePlay(users, usersLen, index, size);
    }
}

//�鿴��Ϸ��¼
void checkGame(struct user users[], int &usersLen, int &index) {
    int i;
    printf("Beginner(4*4):\n");
    printf("      Game      Attempts      Time\n");
    for (i = 0; i < users[index].index44;i++) {
        printf("      %d            %d            %d\n", i + 1, users[i].score44[index], users[i].time44[index]);
        i++;
    }
    printf("Intermediate(6*6):\n");
    printf("      Game      Attempts      Time\n");
    for (i = 0; i < users[index].index66; i++) {
        printf("      %d            %d            %d\n", i + 1, users[i].score66[index], users[i].time66[index]);
        i++;
    }
    printf("Expert(8*8):\n");
    printf("      Game      Attempts      Time\n");
    for (i = 0; i < users[index].index88; i++) {
        printf("      %d            %d            %d\n", i + 1, users[i].score88[index], users[i].time88[index]);
        i++;
    }
    printf("Press any key to continue:");
    _getch();
    mainMenu(users, usersLen, index);
}

//ע���˺�
void logoutUser(struct user users[], int& usersLen,int index) {
    int i;
    usersLen--;
    if (usersLen == 0) {  //ֻ��һ���˺�
        strcpy(users[0].num,"");
        strcpy(users[0].code,"");
        clearScore(users, 0);
    }
    else {
        for (i = index; i < usersLen; i++) {
            users[i] = users[i + 1];
        }
    }
    
}