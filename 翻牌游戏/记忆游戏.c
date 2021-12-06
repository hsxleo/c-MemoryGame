#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>

struct user {
	char account[10];
	char password[10];
	int attempts[50];
	int time[50];
	int length;  //总玩家数量
	int player;  //当前玩家
};
void read(struct user users[]);
void save(struct user users[]);
int find(struct user users[], char account[10], char password[10]);
void login(struct user users[]);
void create(struct user users[]); 
void menu(struct user users[]);
void bulider(char ch[]);
void draw(char ch[], int isShow[]);
void playing(struct user users[]);
void record(struct user users[], int attempts, int time);
void check(struct user users[]);
void clear(struct user users[]);
void logout(struct user users[]);

int main() {
	struct user users[100];
	//结构体初始化  全初始化为0
	int i, j;
	for (i = 0; i < 100; i++) {
		strcpy(users[i].account,"");
		strcpy(users[i].password,"");
		users[i].length = 0;
		users[i].player = 0;
		for (j = 0; j < 50; j++) {
			users[i].attempts[j] = 0;
			users[i].time[j] = 0;
		}
	}
	
	read(users);
	//printf("length = %d,player = %d", users[0].length, users[0].player);
	//for (i = 0; i < 4; i++) {
	//	printf("account = %s\n", users[i].account);
	//	printf("password = %s\n", users[i].password);
	//	printf("attempts = %d\n", users[i].attempts[0]);
	//	printf("time = %d\n", users[i].time[0]);
	//}
	//getch();
	int option;
	printf("Welcome to Find the Pairs, Test your memory:\n\n");
	printf("Select from the following options:\n\n");
	printf("      '1' Login to your account\n");
	printf("      '2' Create new account\n");
	printf("      '3' Quit\n\n\n");
	printf("      Option:");
	scanf("%d", &option);
	switch (option) {
	case 1:
		login(users);
		menu(users);
		break;
	case 2:
		create(users);
		save(users);
		menu(users);
		break;
	case 3:
		break;
	}
	return 0;
}
void read(struct user users[]) {
	FILE *fp;
	//打开文件
	if ((fp = fopen("users.txt", "r")) == NULL) { 
		fp = fopen("users.txt", "w");
	}
	//int flg = 1;
	int i = 0;
	while (fread(&users[i], sizeof(struct user), 1, fp)) {
		i++;
	}
	users[0].length = i;
	fclose(fp);
	/*while (feof(fp) == 0) {
		if (flg == 1) {
			fread(&users[i], sizeof(struct user), 1, fp);
			fseek(fp, -sizeof(struct user), SEEK_CUR);
			flg = 0;
		}
		else {
			i++;
			fread(&users[i], sizeof(struct user), 1, fp);
		}
	}*/
}
void save(struct user users[]) {
	FILE* fp;
	//打开文件
	fp = fopen("users.txt", "w");
	//int k;
	//printf("length = %d,player = %d", users[0].length, users[0].player);
	//for (k = 0; k < 4; k++) {
	//	printf("account = %s\n", users[k].account);
	//	printf("password = %s\n", users[k].password);
	//}
	//getch();
	int i = 0;
	for (i = 0; i < users[0].length; i++) {
		fwrite(&users[i], sizeof(struct user), 1, fp);
	}
	
	fclose(fp);
}
int find(struct user users[],char account[],char password[]) {
	int i;
	for (i = 0; i < users[0].length; i++) {
		if (strcmp(users[i].account, account) == 0 && strcmp(users[i].password,password) == 0) {
			return i;
		}
	}
	return -1;
}
void login(struct user users[]) {
	char account[10], password[10];
	printf("        Login screen\n\n");
	printf("        Please enter your account number:");
	scanf("%s", account);
	printf("        Please enter your password:");
	scanf("%s", password);
	while (1) {
		int i = find(users,account, password);
		if (i == -1) {
			printf("Incorrect account or password!\n");
			printf("        Please enter your account number:");
			scanf("%s", account);
			printf("        Please enter your password:");
			scanf("%s", password);
		}
		else {
			users[0].player = i;  //当前用户
			break;
		}
	}
}
void create(struct user users[]){
	char account[10], password[10], password2[10];
	printf("        Registration Page\n\n");
	do {
		printf("        Please enter the account name you would like to use:");
		scanf("%s", account);
		do {  //输入两次密码并判断
			printf("        Please enter your password:");
			scanf("%s", password);
			printf("        Please enter your password again:");
			scanf("%s", password2);
			if (strcmp(password, password2) != 0) {
				printf("        The two password inputs are different!\n");
			}
		} while (strcmp(password, password2) != 0);
		if (find(users, account, password) != -1) {
			printf("        The user already exists!\n");
		}
	} while (find(users, account, password) != -1);

	printf("        Searching database this may take a moment, please wait...\n");
	printf("        you have successfully created and credited your account, you are now logged in...");
	strcpy(users[users[0].length].account, account);
	strcpy(users[users[0].length].password, password);
	users[0].player = users[0].length;
	users[0].length++;
}
void menu(struct user users[]) {
	while (1) {
		int option;
		system("cls");
		printf("Select from the following options:\n\n");
		printf("      1. Start Playing\n\n");
		printf("      2. Check your game history\n\n");
		printf("      3. Clear game history\n\n");
		printf("      4. Logout this account\n\n");
		printf("      5. Return to the Start menu\n\n");
		printf("      Option:");
		scanf("%d", &option);
		switch (option) {
		case 1:
			playing(users);
			save(users);
			break;
		case 2:
			check(users);
			break;
		case 3:
			clear(users);
			save(users);
			break;
		case 4:
			logout(users);
			save(users);
			main();
			break;
		case 5:
			main();
			break;
		}
	}
}
void bulider(char ch[]) {
	srand((unsigned)time(NULL));
	int i;
	for (i = 1; i <= 8; i++) {
		ch[i] = '!' + i;
		ch[i + 8] = '!' + i;
	}
	//打乱  随意的两两交换
	for (i = 0; i < 20; i++) {
		int t1 = rand() % 16 + 1;
		int t2 = rand() % 16 + 1;
		char tmp = ch[t1];
		ch[t1] = ch[t2];
		ch[t2] = tmp;
	}
}
void draw(char ch[], int isShow[]) {
	int i, j;
	system("cls");
	for (i = 1; i <= 16; i++) {
		printf("\t%d", i);
		if (i % 4 == 0) {
			printf("\n\n");
		}
	}
	printf("\n\n");
	for (i = 1; i <= 16; i++) {
		if (isShow[i] == 0) {
			printf("\t%c", ch[i]);
		}
		else {
			printf("\t-");
		}
		if (i % 4 == 0) {
			printf("\n\n");
		}
	}
}
void playing(struct user users[]) {
	char ch[20];
	int isShow[20] = { 0 };
	bulider(ch);
	draw(ch,isShow);
	printf("You have five seconds to remember it!");
	Sleep(5000);
	int i;
	for (i = 1; i <= 16; i++) {
		isShow[i] = 1;
	}

	time_t start, end;
	int n = 8;
	int attempts = 0;
	int num1, num2;
	start = time(NULL);
	while (n != 0) {
		draw(ch, isShow);
		printf("Please enter two integers:");
		scanf("%d%d", &num1, &num2);
		attempts++;
		isShow[num1] = 0;
		isShow[num2] = 0;
		draw(ch, isShow);
		Sleep(1000);
		if (ch[num1] != ch[num2]) {
			isShow[num1] = 1;
			isShow[num2] = 1;
		}
		else {
			n--;
		}
	}
	end = time(NULL);

	//讲游戏成绩写到结构体中
	record(users, attempts, end - start);

	printf("The game victory! attempts:%d,time:%ds", attempts, end - start);
	printf("\n\nAny key returns to the menu.");
	getch();
}
void record(struct user users[],int attempts,int time) {
	int i, j = 0;
	for (i = 0; i < 50; i++) {
		if (users[users[0].player].attempts[i] == 0) {
			j = i;
			break;
		}
	}
	users[users[0].player].attempts[j] = attempts;
	users[users[0].player].time[j] = time;
}
void check(struct user users[]) {
	system("cls"); 
	int i = 0;
	printf("\t\tAttempts\ttime\n");
	while (users[users[0].player].attempts[i] != 0) {
		printf("\t%d\t%d\t\t%d\n", i + 1, users[users[0].player].attempts[i], users[users[0].player].time[i]);
		i++;
	}
	printf("\n\nAny key returns to the menu.");
	getch();
}
void clear(struct user users[]) {
	system("cls");  
	int i;
	for (i = 0; i < 50; i++) {
		users[users[0].player].attempts[i] = 0;
		users[users[0].player].time[i] = 0;
	}
	printf("\n\nClear success,any key returns to the menu.");
	getch();
}
void logout(struct user users[]) {
	int i;
	for (i = users[0].player; i < users[0].length -1; i++) {
		users[i] = users[i + 1];
	}
	users[0].length--;
	printf("Account logout successful,press any key to return to the login menu.");
	getch();
}