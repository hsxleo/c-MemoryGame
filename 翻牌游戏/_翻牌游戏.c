#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <string.h>

//�û��Ľṹ�� 
struct user {
	char account[20];
	char password[20];
	int attempts[50];
	int time[50]; 
};


int readFile(struct user users[]);//��ȡ�ļ�
void saveFile(struct user users[], int users_len);// �����ļ�
int loginUser(struct user users[], int users_len);//�û���¼
int createUser(struct user users[], int users_len);//�û�ע��
void gameMenu(struct user users[], int users_len, int user_index);//��Ϸ�˵�
void playGame(struct user users[], int users_len, int user_index);//��Ϸ����
void showGame(char b[]);//��ʾ��Ϸ
void checkHistory(struct user users[], int user_index);//�鿴��Ϸ��ʷ
void clearHistory(struct user users[], int user_index);//�����Ϸ��ʷ
void logoutUser(struct user users[], int users_len, int user_index);//ע���˺�
//��ʼ�˵�
int main() {
	struct user users[200];  //�û��ǵĽṹ������
	int users_len = 0;//�û��ṹ��ĳ���
	int user_index = 0;//��ǰ�û��Ľṹ���±�

	//�ṹ���������ݳ�ʼ��
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

	int option;  //ѡ����
	do {
		system("cls");  //��������
		printf("Welcome to Find the Pairs, Test your memory:\n\n");
		printf("Select from the following options:\n\n");
		printf("\t'1' Login to your account\n");
		printf("\t'2' Create new account\n");
		printf("\t'3' Quit\n\n\n");
		printf("\tOption:");
		scanf("%d", &option);
		rewind(stdin);   // ��ջ�����  ��ֹ�û�������ĸ
		if (option == 1) {
			user_index = loginUser(users, users_len);
			gameMenu(users, users_len, user_index);
		}
		else if (option == 2) {
			users_len = createUser(users, users_len);
			saveFile(users,users_len);  //����һ��
			user_index = users_len - 1;  //��ǰ�û�Ϊ���һ���û�
			gameMenu(users, users_len, user_index);
		}
	} while (option != 3);  //�˳���Ϸ
}

//��ȡ�ļ�
int readFile(struct user users[]) {
	FILE *fp;  //�ļ�ָ��
	if ((fp = fopen("users.bin", "rb")) == NULL) {  //�ж��ļ��Ƿ����   �������ھ��½�һ���ļ�
		fp = fopen("users.bin", "wb");
	}

	int users_len = 0;
	while (fread(&users[users_len++], sizeof(struct user), 1, fp) != 0);  //��ȡ�ļ�����   �����ļ�����

	fclose(fp);   //�ر��ļ�
	return users_len - 1;
}

//�����ļ�
void saveFile(struct user users[],int users_len) {
	FILE* fp;  //�ļ�ָ��
	fp = fopen("users.bin", "wb");   //���ļ�

	
	int i;
	for (i = 0; i < users_len; i++) {
		fwrite(users, sizeof(struct user), 1, fp);  //д���ļ�
	}

	fclose(fp);   //�ر��ļ�
}

//�û���¼
int loginUser(struct user users[], int users_len) {
	char a[20], b[20];
	system("cls"); //��������
	printf("\tLogin screen\n\n");
	do {
		printf("\tPlease enter your account number:");
		scanf("%s", a);
		printf("\tPlease enter your password:");
		scanf("%s", b);

		int i;
		for (i = 0; i < users_len; i++) {
			if (strcmp(users[i].account, a) == 0 && strcmp(users[i].password, b) == 0) {  //�˺����붼��ȷ
				return i;  //���ظ��û����±�
			}
		}

		printf("\n\tYour account or password is incorrect. Please re-enter it!\n");  
	} while (1);
}

//�û�ע��
int createUser(struct user users[], int users_len) {
	char a[20], b[20], c[20];
	system("cls");  //��������
	printf("\tRegistration Page\n\n");
	printf("\tPlease enter the account name you would like to use:");
	scanf("%s", a);
	do {  //��֤�û��Ƿ����
		printf("\n\n\tSearching database this may take a moment, please wait......\n\n");
		int i;
		for (i = 0; i < users_len; i++) {
			if (strcmp(users[i].account, a) == 0) {   //�û�����
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
	users_len++; //�û���������1
	printf("\n\n\tyou have successfully created and credited your account, you are now logged in...");
	Sleep(1000);  //��ͣ1��
	return users_len;  //�����û�����
	
}

//��Ϸ�˵�
void gameMenu(struct user users[], int users_len,int user_index) {
	
	int option;  //ѡ����
	do {
		system("cls");  //��������
		printf("Select from the following options:\n\n");
		printf("\t1. Start Playing\n\n");
		printf("\t2. Check your game history\n\n");
		printf("\t3. Clear game history\n\n");
		printf("\t4. Logout this account\n\n");
		printf("\t5. Return to the login screen\n\n\n");
		printf("\tOption:");
		scanf("%d", &option);
		rewind(stdin);   // ��ջ�����  ��ֹ�û�������ĸ
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
	} while (option != 5);  //���ص���¼�˵�
}

//��Ϸ����
void playGame(struct user users[], int users_len, int user_index) {
	char a[20], b[20];
	int i;
	for (i = 0; i < 8; i++) {  //����8���ַ�   
		a[i] = '!' + i;
		a[i+8] = '!' + i;
	}

	int n = 100;
	srand((unsigned)(time(NULL)));  //���ʱ������
	while (n--) {   //�������100���ַ�˳��   �������������
		int t1 = rand() % 16;  //�������[0,15]������
		int t2 = rand() % 16;
		char tmp;
		tmp = a[t1];
		a[t1] = a[t2];
		a[t2] = tmp;
	}

	for (i = 0; i < 16; i++) {  //������ʾ������
		b[i] = '-';
	}

	showGame(a);   //չʾ5����
	printf("The game starts right away, and now you have 5 seconds to memorize it!");
	Sleep(5000);  //��ͣ5��

	int attempts = 0;//���ԵĴ���
	time_t start, end;
	n = 8;   //һ��8���ַ�

	start = time(NULL);  //��ʱ��ʼ
	do {
		showGame(b);
		printf("Please enter the two positions you want to turn over:");
		int t1, t2;
		scanf("%d%d", &t1, &t2);
		t1--;  //��������0��ʼ��  ����Ҫ��ȥ1  
		t2--;
		attempts++;   //���ԵĴ�������
		rewind(stdin);   // ��ջ�����  ��ֹ�û�������ĸ

		//ѡ���������ֵ��b[]  ����ʾ����
		b[t1] = a[t1];
		b[t2] = a[t2];
		showGame(b);
		Sleep(700);

		//�ж��Ƿ�һ��
		if (a[t1] != a[t2]) {  //�����һ�����á�-������ԭ���ĸ�ֵ
			b[t1] = '-';
			b[t2] = '-';
		}
		else {
			n--;  //����һ��
		}
	} while (n > 0);  //8��ȫ������
	end = time(NULL);  //��ʱ����

	//��Ϸ��¼���浽�ṹ����
	i = 0;
	while (users[user_index].attempts[i++] != 0);    //ѭ����iΪ0  ���ǵ�ǰ�û��ĵڼ�����Ϸ���±�
	i--;
	users[user_index].attempts[i] = attempts;
	users[user_index].time[i] = end - start;

	//��ʾ���
	printf("Congratulations on winning the game, attempts: %d,time: %ds\n", attempts, end - start);
	printf("Press any key to return to the game menu.");
	getch();
}

//��ʾ��Ϸ
void showGame(char b[]) {
	system("cls"); //��������
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

//�鿴��Ϸ��ʷ
void checkHistory(struct user users[], int user_index) {
	system("cls");  //��������
	int i= 0;
	printf("\tGame\tAttempts\ttime\n");
	while (users[user_index].attempts[i] != 0) {
		printf("\t%d\t%d\t\t%d\n", i + 1, users[user_index].attempts[i], users[user_index].time[i]);
		i++;
	}
	printf("\n\nPress any key to return to the game menu.");
	getch();
}

//�����Ϸ��ʷ
void clearHistory(struct user users[], int user_index) {
	system("cls");  //��������
	int i;
	for (i = 0; i < 50; i++) {
		users[user_index].attempts[i] = 0;
		users[user_index].time[i] = 0;
	}
	printf("\nThe game has a clear record of success\n");
	printf("\n\nPress any key to return to the game menu.");
	getch();
}

//ע���˺�
void logoutUser(struct user users[], int users_len,int user_index) {
	int i;
	for (i = user_index; i < users_len; i++) {   //���û�����Ľṹ����ǰ��һλ
		users[i] = users[i + 1];
	}
	users_len--;
	saveFile(users, users_len);  //����һ��
	printf("\n\nThe account has been cancelled successfully,please press any key to return to the login screen.");
	getch();
}