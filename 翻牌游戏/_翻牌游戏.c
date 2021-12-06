#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <string.h>

//用户的结构体 
struct user {
	char account[20];
	char password[20];
	int attempts[50];
	int time[50]; 
};


int readFile(struct user users[]);//读取文件
void saveFile(struct user users[], int users_len);// 保存文件
int loginUser(struct user users[], int users_len);//用户登录
int createUser(struct user users[], int users_len);//用户注册
void gameMenu(struct user users[], int users_len, int user_index);//游戏菜单
void playGame(struct user users[], int users_len, int user_index);//游戏进行
void showGame(char b[]);//显示游戏
void checkHistory(struct user users[], int user_index);//查看游戏历史
void clearHistory(struct user users[], int user_index);//清空游戏历史
void logoutUser(struct user users[], int users_len, int user_index);//注销账号
//开始菜单
int main() {
	struct user users[200];  //用户们的结构体数组
	int users_len = 0;//用户结构体的长度
	int user_index = 0;//当前用户的结构体下标

	//结构体所有内容初始化
	int i, j;
	for (i = 0; i < 200; i++) {
		strcpy(users[i].account, "");
		strcpy(users[i].password, "");
		for (j = 0; j < 50; j++) {
			users[i].attempts[j] = 0;
			users[i].time[j] = 0;
		}
	}

	users_len = readFile(users);

	int option;  //选择结果
	do {
		system("cls");  //清屏函数
		printf("Welcome to Find the Pairs, Test your memory:\n\n");
		printf("Select from the following options:\n\n");
		printf("\t'1' Login to your account\n");
		printf("\t'2' Create new account\n");
		printf("\t'3' Quit\n\n\n");
		printf("\tOption:");
		scanf("%d", &option);
		rewind(stdin);   // 清空缓冲区  防止用户输入字母
		if (option == 1) {
			user_index = loginUser(users, users_len);
			gameMenu(users, users_len, user_index);
		}
		else if (option == 2) {
			users_len = createUser(users, users_len);
			saveFile(users,users_len);  //保存一下
			user_index = users_len - 1;  //当前用户为最后一个用户
			gameMenu(users, users_len, user_index);
		}
	} while (option != 3);  //退出游戏
}

//读取文件
int readFile(struct user users[]) {
	FILE *fp;  //文件指针
	if ((fp = fopen("users.bin", "rb")) == NULL) {  //判断文件是否存在   若不存在就新建一个文件
		fp = fopen("users.bin", "wb");
	}

	int users_len = 0;
	while (fread(&users[users_len++], sizeof(struct user), 1, fp) != 0);  //读取文件内容   读到文件结束

	fclose(fp);   //关闭文件
	return users_len - 1;
}

//保存文件
void saveFile(struct user users[],int users_len) {
	FILE* fp;  //文件指针
	fp = fopen("users.bin", "wb");   //打开文件

	
	int i;
	for (i = 0; i < users_len; i++) {
		fwrite(users, sizeof(struct user), 1, fp);  //写入文件
	}

	fclose(fp);   //关闭文件
}

//用户登录
int loginUser(struct user users[], int users_len) {
	char a[20], b[20];
	system("cls"); //清屏函数
	printf("\tLogin screen\n\n");
	do {
		printf("\tPlease enter your account number:");
		scanf("%s", a);
		printf("\tPlease enter your password:");
		scanf("%s", b);

		int i;
		for (i = 0; i < users_len; i++) {
			if (strcmp(users[i].account, a) == 0 && strcmp(users[i].password, b) == 0) {  //账号密码都正确
				return i;  //返回该用户的下标
			}
		}

		printf("\n\tYour account or password is incorrect. Please re-enter it!\n");  
	} while (1);
}

//用户注册
int createUser(struct user users[], int users_len) {
	char a[20], b[20], c[20];
	system("cls");  //清屏函数
	printf("\tRegistration Page\n\n");
	printf("\tPlease enter the account name you would like to use:");
	scanf("%s", a);
	do {  //验证用户是否存在
		printf("\n\n\tSearching database this may take a moment, please wait......\n\n");
		int i;
		for (i = 0; i < users_len; i++) {
			if (strcmp(users[i].account, a) == 0) {   //用户存在
				printf("\n\tThe user name already exists, please re-create:");
				scanf("%s", a);
				break;
			}
		}
		if (i == users_len) {
			break;
		}
	} while (1);

	do {
		printf("\tPlease enter the password you would like to use:");
		scanf("%s", b);
		printf("\tPlease enter your password again:");
		scanf("%s", c);
	} while (strcmp(b, c) != 0);
	strcpy(users[users_len].account, a);
	strcpy(users[users_len].password, b);
	users_len++; //用户数量增加1
	printf("\n\n\tyou have successfully created and credited your account, you are now logged in...");
	Sleep(1000);  //暂停1秒
	return users_len;  //返回用户数量
	
}

//游戏菜单
void gameMenu(struct user users[], int users_len,int user_index) {
	
	int option;  //选择结果
	do {
		system("cls");  //清屏函数
		printf("Select from the following options:\n\n");
		printf("\t1. Start Playing\n\n");
		printf("\t2. Check your game history\n\n");
		printf("\t3. Clear game history\n\n");
		printf("\t4. Logout this account\n\n");
		printf("\t5. Return to the login screen\n\n\n");
		printf("\tOption:");
		scanf("%d", &option);
		rewind(stdin);   // 清空缓冲区  防止用户输入字母
		switch (option) {
		case 1:
			playGame(users, users_len, user_index);
			saveFile(users, users_len);
			break;
		case 2:
			checkHistory(users, user_index);
			break;
		case 3:
			clearHistory(users, user_index);
			saveFile(users, users_len);
			break;
		case 4:
			logoutUser(users, users_len, user_index);
			main();
			break;
		}
	} while (option != 5);  //返回到登录菜单
}

//游戏进行
void playGame(struct user users[], int users_len, int user_index) {
	char a[20], b[20];
	int i;
	for (i = 0; i < 8; i++) {  //生成8对字符   
		a[i] = '!' + i;
		a[i+8] = '!' + i;
	}

	int n = 100;
	srand((unsigned)(time(NULL)));  //随机时间种子
	while (n--) {   //随机打乱100次字符顺序   随机的两两交换
		int t1 = rand() % 16;  //随机生成[0,15]的整数
		int t2 = rand() % 16;
		char tmp;
		tmp = a[t1];
		a[t1] = a[t2];
		a[t2] = tmp;
	}

	for (i = 0; i < 16; i++) {  //用来显示的数组
		b[i] = '-';
	}

	showGame(a);   //展示5秒钟
	printf("The game starts right away, and now you have 5 seconds to memorize it!");
	Sleep(5000);  //暂停5秒

	int attempts = 0;//尝试的次数
	time_t start, end;
	n = 8;   //一共8对字符

	start = time(NULL);  //计时开始
	do {
		showGame(b);
		printf("Please enter the two positions you want to turn over:");
		int t1, t2;
		scanf("%d%d", &t1, &t2);
		t1--;  //数组是由0开始的  所以要减去1  
		t2--;
		attempts++;   //尝试的次数增加
		rewind(stdin);   // 清空缓冲区  防止用户输入字母

		//选择的两个赋值给b[]  并显示出来
		b[t1] = a[t1];
		b[t2] = a[t2];
		showGame(b);
		Sleep(700);

		//判断是否一样
		if (a[t1] != a[t2]) {  //如果不一样就用‘-’覆盖原来的赋值
			b[t1] = '-';
			b[t2] = '-';
		}
		else {
			n--;  //减少一对
		}
	} while (n > 0);  //8对全部翻出
	end = time(NULL);  //计时结束

	//游戏记录保存到结构体中
	i = 0;
	while (users[user_index].attempts[i++] != 0);    //循环到i为0  即是当前用户的第几次游戏的下标
	i--;
	users[user_index].attempts[i] = attempts;
	users[user_index].time[i] = end - start;

	//显示结果
	printf("Congratulations on winning the game, attempts: %d,time: %ds\n", attempts, end - start);
	printf("Press any key to return to the game menu.");
	getch();
}

//显示游戏
void showGame(char b[]) {
	system("cls"); //清屏函数
	int i;
	for (i = 1; i <= 16; i++) {
		printf("\t%d", i);
		if (i % 4 == 0) {
			printf("\n\n");
		}
	}

	printf("\n");

	for (i = 0; i < 16; i++) {
		printf("\t%c", b[i]);
		if ((i + 1) % 4 == 0) {
			printf("\n\n");
		}
	}
}

//查看游戏历史
void checkHistory(struct user users[], int user_index) {
	system("cls");  //清屏函数
	int i= 0;
	printf("\tGame\tAttempts\ttime\n");
	while (users[user_index].attempts[i] != 0) {
		printf("\t%d\t%d\t\t%d\n", i + 1, users[user_index].attempts[i], users[user_index].time[i]);
		i++;
	}
	printf("\n\nPress any key to return to the game menu.");
	getch();
}

//清空游戏历史
void clearHistory(struct user users[], int user_index) {
	system("cls");  //清屏函数
	int i;
	for (i = 0; i < 50; i++) {
		users[user_index].attempts[i] = 0;
		users[user_index].time[i] = 0;
	}
	printf("\nThe game has a clear record of success\n");
	printf("\n\nPress any key to return to the game menu.");
	getch();
}

//注销账号
void logoutUser(struct user users[], int users_len,int user_index) {
	int i;
	for (i = user_index; i < users_len; i++) {   //该用户往后的结构体往前移一位
		users[i] = users[i + 1];
	}
	users_len--;
	saveFile(users, users_len);  //保存一下
	printf("\n\nThe account has been cancelled successfully,please press any key to return to the login screen.");
	getch();
}