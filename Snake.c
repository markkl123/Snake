#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>
#include <time.h>

#define UP 72  // Up Arrow 
#define DOWN 80 // Down Arrow 
#define LEFT 75  // Left Arrow 
#define RIGHT 77  // Right Arrow 
#define ESC 27  // Escape Key

typedef struct Point
{
	int x;
	int y;
} Point;

void gotoxy(int y, int x);
int getKey();
void printsnake(Point* snake, int size);
void move(Point* snake, int size, char* c);
void init_snake(Point** snake, int* size, char* c, Point* food);
void getnewpoint(Point* point, char* c);
void newpoint(Point* point, char* c);
bool ischarvalid(char c);
int menu();
bool islose(Point* snake, int size);
bool iswin(int size);
void printframe();
void _setfoodrandomly(Point* snake, int size, Point* food);
bool is_food_eaten(Point* snake, Point food);
void snakaegrowth(Point** snake, int* size, Point* food);

void main()
{
	char c;
	int size, option = menu();
	Point food, *snake;
	srand(time(NULL));
	while (option != 0)
	{
		init_snake(&snake, &size, &c, &food);

		while (c != ESC)
		{
			move(snake, size, &c);

			if (is_food_eaten(snake,food))
				snakaegrowth(&snake, &size, &food);

			if (islose(snake, size) || iswin(size))
					break;
		}
		if (c == ESC)
		system("cls");
		option = menu();
	}
}

bool is_food_eaten(Point* snake, Point food)
{
	return (snake->x == food.x && snake->y == food.y);
}

void snakaegrowth(Point** snake, int* size, Point* food)
{
	(*size)++;

	Point* newsnake = malloc((*size) * sizeof(Point));
	if (newsnake == NULL) { printf("allocation error"); exit(0); }

	int i;
	for (i = 0; i < (*size) - 1; i++)
		*(newsnake + i) = *((*snake) + i);

	*snake = malloc((*size) * sizeof(Point));
	if (*snake == NULL) { printf("allocation error"); exit(0); }

	for (i = 0; i < (*size) - 1; i++)
		*((*snake) + i) = *(newsnake + i);

	free(newsnake);

	if (*size != 20)
	_setfoodrandomly(*snake,*size, food);
}

void _setfoodrandomly(Point* snake, int size, Point* food)
{
	int i;
	Point newfood;
	bool validlocation = false;
	while (!validlocation)
	{
		newfood.x = (rand() % 74) + 1;
		newfood.y = (rand() % 24) + 1;
		validlocation = true;
		for (i = 0; i < size; i++)
			if ((snake+i)->x == newfood.x && (snake + i)->y == newfood.y)
				validlocation = false;
	}
	*food = newfood;
	gotoxy(food->x, food->y);
	printf("$");
}

void printframe()
{
	int i;
	for (i = 0; i < 25; i++) {
		gotoxy(75, i);
		printf("#");
		gotoxy(0, i);
		printf("#");
	}
	for (i = 0; i <= 75; i++) {
		gotoxy(i, 25);
		printf("#");
		gotoxy(i, 0);
		printf("#");
	}
	gotoxy(26, 81);
}

bool islose(Point* snake,int size)
{
	if (snake->x >= 75 || snake->x <= 0 || snake->y <= 0 || snake->y >= 25) {
		system("cls");
		printf("##################################\n           GAME OVER\n##################################\n");
		return true;
	}
	int i;
	for (i = 1; i < size; i++)
		if (snake->x == (snake + i)->x && snake->y == (snake + i)->y) {
			system("cls");
			printf("##################################\n           GAME OVER\n##################################\n");
			return true;
		}
	return false;
}

int menu()
{
	
	int choice;
	printf("*******************************************************\n* Pick your level, to exit press 0\n* 0 - Exit\n* 1 - Basic level\n* 2 - Intermidiate level\n* 3 - Advanced level\n*******************************************************\n");
	scanf("%d", &choice);
	switch (choice)
	{
	case 0:
		return 0;
	case 1:
		system("cls");
		return 1;
		break;
/*	case 2:
		return 2;
		break;
	case 3:
		return 3;
		break;*/
	default:
		return menu();
	}
	
}

bool ischarvalid(char c)
{
	return (c == RIGHT || c == DOWN || c == LEFT || c == UP || c == ESC);
}

void getnewpoint(Point* snake,char *c)
{
	char newchar;
	if (_kbhit())
	{
		newchar = (char)getKey();
		if (ischarvalid(newchar) && newchar != ESC) 
			*c = newchar;

		newpoint(snake, c);

		if (newchar == ESC)
			*c = newchar;
	}
	else
	newpoint(snake, c);
}

void newpoint(Point* point, char* c)
{
	switch (*c)
	{
	case RIGHT:
		(*point).x++;
		break;
	case DOWN:
		(*point).y++;
		break;
	case LEFT:
		(*point).x--;
		break;
	case UP:
		(*point).y--;
		break;
	}
}

void init_snake(Point** snake, int* size, char* c, Point* food)
{
	*size = 10;
	printframe();
	*c = 0;
	*snake = malloc((*size) * sizeof(Point));
	if (*snake == NULL) { printf("allocation error"); exit(0); }

	_setfoodrandomly(*snake, *size, food);
	while (!ischarvalid(*c))
		if (_kbhit())
			*c = (char)getKey();

	int i,j;
	(*snake)->x = 40;
	(*snake)->y = 12;
	for (j = 1; j <= *size; j++)
	{
		for (i = j; i > 0; i--)
			*((*snake) + i) = *((*snake) + i - 1);
		newpoint(*snake, c);
	}
	printsnake(*snake, *size);
}

void printsnake(Point* snake, int size)
{
	//system("cls");
	int i;
	for (i = 0; i < size; i++)
	{
		gotoxy(snake[i].x, snake[i].y);
		printf("@");
	}
	gotoxy(26, 81);
}

void move(Point* snake, int size, char* c)
{
	Sleep(500);
	int i;
	Point newpoint = *snake;
	getnewpoint(&newpoint, c);
	if (*c != ESC && !(newpoint.x >= 75 || newpoint.x <= 0 || newpoint.y <= 0 || newpoint.y >= 25)) 
	{
		gotoxy(snake[size - 1].x, snake[size - 1].y);
		printf(" ");

		for (i = size - 1; i > 0; i--)
			*(snake + i) = *(snake + i - 1);

		*snake = newpoint;

		gotoxy(snake[0].x, snake[0].y);
		printf("@");
		gotoxy(26, 81);
	}
	else
		*snake = newpoint;
}

bool iswin(int size)
{
	if (size == 20) {
		system("cls");
		printf("##################################\n  Winner Winner Chicken Dinner!!\n##################################\n");
	}
	return size == 20;
}

// This function moves the cursor to the line y and column x.
// (0,0) is the upper left corner of the screen.
void gotoxy(int y, int x) {
	printf("\x1b[%d;%df", x + 1, y + 1);
}

// This function gets keyCodes for special keys such as arrows , escape and others
// It returns the Key code such as 72 for arrow up, 27 for Escape... 
int getKey()
{
	char KeyStroke = _getch();
	if (KeyStroke == 0 || KeyStroke == -32)
		KeyStroke = _getch();
	return (KeyStroke);
}