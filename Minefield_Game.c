#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>


// Macros
#define		TABLE_SIZE		6
#define		NUMBER_OF_MINES		4
#define		WIN			1
#define		CONTINUE		0
#define		GAME_OVER	       -1


// Global Variables
char table[TABLE_SIZE + 2][TABLE_SIZE + 2];
int sub_table[TABLE_SIZE + 2][TABLE_SIZE + 2];


// Functions
void initialize_sub_table();				// Alt tabloya ilk deger verilir
void initialize_table();				// Tabloya ilk deger verilir
void place_mines();					// Mayinlar yerlestirilir
void display_sub_table();				// Alt tablo yazdirilir
void display_table();					// Tablo yazdirilir
void explorer(int row, int column);			// Mayin kesfeder
int dig(int row, int column);				// Istenen yerin acilmasini saglar
int is_winning();					// Kazanilip kazanilmadigini kontrol eder


int main() {

	printf("1 -> to win the game the flags must be put by pressing 'f' on all mines!!!\n");
	printf("2 -> table size and number of mines can be changed by changing TABLE_SIZE and NUMBER_OF_MINES macros\n");
	printf("3 -> for instance 2nd. row and 1st. column wanted to be dug then as a input 'd,2,1' should be entered\n");
	printf("     Addition flag 'f,2,1' and deletion flag 'r,2,1'\n");
	printf("Good Luck...\n\n\n");

	initialize_sub_table();
	initialize_table();

	char choice;
	int status = CONTINUE;
	int row, column;
	int number_of_flags = 0;
	int play_again;

	while (1) {

		printf("*******************************************MINEFIELD GAME*******************************************\n\n");
		printf("Table size : %d x %d\n", TABLE_SIZE, TABLE_SIZE);
		printf("Added flags : %d\n", number_of_flags);
		printf("Number of mines : %d\n", NUMBER_OF_MINES);
		display_table();
		printf("\ndig ->\t\t    'd,row,column'\nadd flag ->\t    'f,row,column'\nremove added flag ->'r,row,column'\n");
		scanf("%c,%d,%d", &choice, &row, &column);

		if (choice == 'd') {

			if (table[row][column] == '#')
				status = dig(row, column);

			else {
				system("cls");
				printf("It can not be dug...\n");
			}
		}

		else if (choice == 'f') {

			if ((NUMBER_OF_MINES > number_of_flags) && (table[row][column] == '#')) {

				system("cls");
				number_of_flags++;
				printf("%dx%d flag added...\n", row, column);
				table[row][column] = 'F';
				status = is_winning();
			}

			else {
				system("cls");
				printf("Flag can not be added...\n");
			}
		}

		else if (choice == 'r') {

			if ((number_of_flags > 0) && (table[row][column] == 'F')) {
				system("cls");
				printf("%dx%d deletion flag is successful...\n", row, column);
				number_of_flags--;
				table[row][column] = '#';
			}
			else {
				system("cls");
				printf("Deletion flag is unsuccessfull!!!!!!!\n");
			}
		}


		if (status == GAME_OVER) {

			printf("GAME OVER!!!!! \n");
			display_sub_table();
		invalid1:
			printf("PLAY AGAIN ? [1 -> YES][0 -> NO] \n");
			scanf("%d", &play_again);

			switch (play_again) {

			case 0:
				system("cls");
				printf("EXIT IS SUCCESSFUL...\n");
				return 0;

			case 1:
				number_of_flags = 0;
				status = CONTINUE;
				system("cls");
				initialize_sub_table();
				initialize_table();
				break;

			default:
				system("cls");
				printf("INVALID VALUE TRY AGAIN...\n");
				goto invalid1;
			}
		}

		else if (status == WIN) {

			printf("CONGRATS YOU WON THE GAME!!!!!\n");
			display_sub_table();
		invalid2:
			printf("PLAY AGAIN ? [1 -> YES][0 -> NO] \n");
			scanf("%d", &play_again);

			switch (play_again) {

			case 0:
				system("cls");
				printf("EXIT IS SUCCESSFUL...\n");
				return 0;

			case 1:
				number_of_flags = 0;
				status = CONTINUE;
				system("cls");
				initialize_sub_table();
				initialize_table();
				break;

			default:
				system("cls");
				printf("INVALID VALUE TRY AGAIN...\n");
				goto invalid2;
			}
		}
	}
}



void initialize_sub_table() {

	for (int i = 0; i <= TABLE_SIZE + 1; i++)
		for (int j = 0; j <= TABLE_SIZE + 1; j++)
			sub_table[i][j] = 0;

	for (int i = 0; i < TABLE_SIZE + 2; i++) {
		sub_table[i][0] = 1;
		sub_table[i][TABLE_SIZE + 1] = 1;
	}

	for (int i = 1; i < TABLE_SIZE + 1; i++) {
		sub_table[0][i] = 1;
		sub_table[TABLE_SIZE + 1][i] = 1;
	}

	place_mines();
}



void initialize_table() {

	for (int i = 0; i <= TABLE_SIZE + 1; i++)
		for (int j = 0; j <= TABLE_SIZE + 1; j++)
			table[i][j] = '#';

	for (int i = 0; i < TABLE_SIZE + 2; i++) {
		table[i][0] = '*';
		table[i][TABLE_SIZE + 1] = '*';
	}

	for (int i = 0; i < TABLE_SIZE + 1; i++) {
		table[0][i] = '*';
		table[TABLE_SIZE + 1][i] = '*';
	}
}



void place_mines() {

	srand(time(NULL));

	int row, column;

	for (int i = 1; i <= NUMBER_OF_MINES; i++) {

		row = 1 + (rand() % TABLE_SIZE);						// Mayin yerlestirilecek satir no random olarak alinir
		column = 1 + (rand() % TABLE_SIZE);						// Mayin yerlestirilecek sutun no random olarak alinir

		if (sub_table[row][column] == -1)						// Mayin olan bir yere tekrar mayin yerlestirildiyse
			i--;									// random olarak satir ve sutunu tekrar al ve tekrar mayin yerlestir

		sub_table[row][column] = -1;							// Mayin yerlestirilir

		for (int j = -1; j <= 1; j++)							// Mayinlarin cevresi 2x2 matris seklinde dolasilir ve cevresindeki mayin sayilari yazdirilir
			for (int k = -1; k <= 1; k++)
				if (sub_table[row][column] == -1)
					if (sub_table[row + j][column + k] != -1)		// Mayinlar haric bu islemi yap
						sub_table[row + j][column + k]++;
	}
}



void display_sub_table() {

	printf("\n");

	for (int i = 1; i <= TABLE_SIZE; i++)
		printf("%3d", i);

	printf("\n");

	for (int i = 0; i < TABLE_SIZE; i++)
		printf("---");

	printf("\n");

	for (int i = 1; i <= TABLE_SIZE; i++) {

		for (int j = 1; j <= TABLE_SIZE; j++)
			printf("%3d", sub_table[i][j]);

		printf(" |%d", i);
		printf("\n");
	}

	for (int i = 0; i < TABLE_SIZE; i++)
		printf("---");

	printf("\n");
}



void display_table() {

	printf("\n");

	for (int i = 1; i <= TABLE_SIZE; i++)
		printf("%3d", i);

	printf("\n");

	for (int i = 0; i < TABLE_SIZE; i++)
		printf("---");

	printf("\n");

	for (int i = 1; i <= TABLE_SIZE; i++) {

		for (int j = 1; j <= TABLE_SIZE; j++)
			printf("%3c", table[i][j]);

		printf(" |%d", i);
		printf("\n");
	}

	for (int i = 0; i < TABLE_SIZE; i++)
		printf("---");

	printf("\n");
}



void explorer(int row, int column) {

	table[row][column] = '0' + sub_table[row][column];

	for (int i = -1; i <= 1; i++) {					// 2x2 matris olarak cevresi dolasilir

		for (int j = -1; j <= 1; j++)
			if (sub_table[row + i][column + j] > 0 && table[row + i][column + j] == '#')
				table[row + i][column + j] = '0' + sub_table[row + i][column + j];

			else if (sub_table[row + i][column + j] == 0 && table[row + i][column + j] == '#')
				explorer(row + i, column + j);
	}
}



int dig(int row, int column) {

	system("cls");
	printf("%dx%d dig is successful...\n", row, column);

	int status = CONTINUE;

	if (sub_table[row][column] == -1)			// Eger acilan yer mayin ise
		status = GAME_OVER;

	else if (sub_table[row][column] > 0) {			// Eger acilan yer mayin degilse

		table[row][column] = ('0' + sub_table[row][column]);
		status = CONTINUE;
	}

	else											
		explorer(row, column);

	return status;
}



int is_winning() {

	int flags_on_mines = 0;							// Mayin uzerine koyulmus bayrak sayisi

	for (int i = 1; i <= TABLE_SIZE; i++)
		for (int j = 1; j <= TABLE_SIZE; j++)
			if (table[i][j] == 'F')					// Eger konumda bayrak varsa ve
				if (sub_table[i][j] == -1)			// bayrak mayinin uzerindeyse
					flags_on_mines++;			// bayragin uzerindeki mayin sayisi arttirilir

	if (flags_on_mines == NUMBER_OF_MINES)			                // Mayinin uzerine koyulan bayrak sayisi oyundaki toplam
		return WIN;							// bayrak sayisina esitse tum mayinlar bulunmustur

	else
		return CONTINUE;
}


