#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //Ŀ������ �Լ��� ���Ǵ� ������ 

void gotoxy(int x, int y) //gotoxy �Լ�
{
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void setcursortype(CURSOR_TYPE c) { //Ŀ������ �Լ� 
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void clear_GUI();

void draw_menuUI();
int select_menu();

void draw_gamerule();

void draw_levUI();
int select_lev();

void flag(int, int, char**);

void set_custom(int*, int*, int*);

void draw_gameUI(int*, int*);

void detect_mine(int, int, int, int, char**, char**);

void game_over(int, int, char**);
void game_clear(int, int, char**);


int main(void)
{
	setcursortype(NOCURSOR); //Ŀ�� ����

	while (1) //��ü���� �ݺ���
	{
		//�޴�ȭ�� --------------------------------------------------------------------------------------
		
		while (1)
		{
			//-----------�׷��� ����
			draw_menuUI();

			//-----------���� �Լ�
			int menu = select_menu();

			//-----------���� ����� ���� ���� �ܰ� ����
			//1.���ӽ��� ���� ��
			if (menu == 1) 
			{
				break; //�޴�ȭ�� �ݺ��� ����
			}

			//2.���Ӽ��� ���� ��
			else if (menu == 2)
			{
				draw_gamerule(); //���� �� ���� �׷��� ����
			}

			//�� ��(3.��������) ���� ��
			else 
			{
				exit(0); //���α׷� ����
			}
		}

		//���� ���� ȭ��	--------------------------------------------------------------------------------
		
		//-----------�׷��� ����
		draw_levUI();

		//-----------���� �Լ�
		int lev = select_lev();

		//-----------���� ����� ���� ���� �ܰ� ����
		int row, col, mine; //���� ������(��, ��, ���� ����)
		
		//1.���� ���� �� 
		if (lev == 1) 
		{
			row = 9, col = 9, mine = 10; //9x9 ���ڰ���:10
		}

		//2.���� ���� �� 
		else if (lev == 2)
		{
			row = 16, col = 16, mine = 40; //16x16 ���ڰ���:40
		}

		//3.����� ���� �� 
		else if (lev == 3)
		{
			row = 16, col = 30, mine = 49; //16x30 ���ڰ���:49
		}

		//�� ��(4.Ŀ����) ���� ��
		else
		{
			set_custom(&row, &col, &mine); //Ŀ���� ���� ����, ����, ���ڰ��� ����
		}

		
		//���� ȭ�� -------------------------------------------------------------------------------------
		
		//-----------�׷��� ����
		draw_gameUI(&row, &col);
		
		//-----------���� ����
		int i, j, k;

		srand(time(NULL)); //����ǥ ����

		//2���� �迭(�������� �迭) ���� �Ҵ�
		char** arr_mine = malloc(sizeof(char*) * row);
		for (i = 0; i < row; i++)
		{
			arr_mine[i] = malloc(sizeof(char) * col);
		}

		//�������� �迭 'o'�� �ʱ�ȭ
		for (i = 0; i < row; i++)
		{
			for (j = 0; j < col; j++)
			{
				arr_mine[i][j] = 'o';
			}
		}

		//2���� �迭(Ŭ�� ���� �迭) ���� �Ҵ�
		char** click = malloc(sizeof(char*) * row);
		for (i = 0; i < row; i++)
		{
			click[i] = malloc(sizeof(char) * col);
		}

		//Ŭ�� ���� �迭 'o'�� �ʱ�ȭ
		for (i = 0; i < row; i++)
		{
			for (j = 0; j < col; j++)
			{
				click[i][j] = 'n';
			}
		}



		//���� ��ġ
		for (i = 0; i < mine; i++) //������ �� ������ŭ �ݺ�
		{
			int mine_count = 0; //��ġ�� ���� ����
			arr_mine[rand() % row][rand() % col] = 'x'; //���� �ڸ��� ���� ��ġ('x'�Է�)

			for (j = 0; j < row; j++) //���� �迭 ��ü �ݺ�
			{
				for (k = 0; k < col; k++)
				{
					if (arr_mine[j][k] == 'x') //���� ���ڰ� �ִٸ�
					{
						mine_count++; //��ġ�� ���� ������ +1 
					}
				}
			}

			if (mine_count - 1 != i) //���� ��ġ�� ���� ������ ��ġ�� ���� ����(i)�� �ٸ���
			{
				i--; //�ѹ� �� �����Ͽ���
			}
		}

		setcursortype(SOLIDCURSOR); //Ŀ�� ����


//------�׽�Ʈ--------------------------------
//		for (i = 0; i < row; i++)
//		{
//			printf("\n");
//			for (j = 0; j < col; j++)
//			{
//				printf("%c ", arr_mine[i][j]);
//			}
//			
//		}
//		_getch();
//		draw_gameUI(&row, &col);
//-------------------------------------------


		//----------------------------------------------------------------------------------------------------
		// ���⼭���� �ּ� �ۼ� �ʿ�
		//----------------------------------------------------------------------------------------------------

		int point_r = 1;
		int point_c = 1;

		while (1)
		{
			while (1)
			{
				gotoxy(point_c, point_r);
				int key = _getch();
				if (key == 224) //����Ű�� ��
				{
					key = _getch();
					if (key == 80) //�Ʒ� ����Ű�� ���
					{
						point_r++;
					}
					else if (key == 72) //�� ����Ű�� ���
					{
						point_r--;
					}
					else if (key == 77) //���� ����Ű�� ���
					{
						point_c++;
					}
					else if (key == 75) //���� ����Ű�� ���
					{
						point_c--;
					}
				}
				else
				{
					if (key == 13) //enterŰ �� ���
					{
						if (click[point_r - 1][point_c - 1] == 'f');
						else
							break; //���ð� ���
					}
					else if (key == 32)
						flag(point_c, point_r, click);
				}

				if (point_r == 0) //Ŀ�� ��ġ�� ������ �Ѿ ���
				{
					point_r = row; //���������� �̵�
				}

				if (point_r == row + 1) //Ŀ�� ��ġ�� �Ʒ����� �Ѿ ���
				{
					point_r = 1; //ó������ �̵�
				}

				if (point_c == col + 1) //Ŀ�� ��ġ�� �������� �Ѿ ���
				{
					point_c = 1; //�������� �̵�
				}

				if (point_c == 0) //Ŀ�� ��ġ�� ������ �Ѿ ���
				{
					point_c = col; //���������� �̵�
				}
			}
			
			if (arr_mine[point_r - 1][point_c - 1] == 'x')
			{
				game_over(row, col, arr_mine);
				break;
			}

			detect_mine(row, col, point_r, point_c, arr_mine, click);

			int mine_left = 0;
			for (i = 0; i < row; i++)
			{
				for (j = 0; j < col; j++)
				{
					if (click[i][j] == 'n')
					{
						mine_left++;
					}
					if (click[i][j] == 'f')
					{
						mine_left++;
					}
				}
			}

			if (mine_left == mine)
			{
				game_clear(row, col, arr_mine);
				break;
			}
		}
		

		//���� �迭 �޸� ����
		for (i = 0; i < row - 1; i++)
		{
			free(arr_mine[i]);
		}
		free(arr_mine);

		_getch(); //�Ͻ����� �Լ�(���� ����)
	}
}

void clear_GUI() //ȭ�� �ʱ�ȭ �Լ�
{
	int i, j;
	for (i = 1; i < 37; i++)
	{
		for (j = 1; j < 19; j++)
		{
			gotoxy(i, j); printf("  ");
		}
	}
}

void draw_menuUI() //�޴�ȭ�� �׷��� ���� �Լ�
{
	int i;
	system("mode con cols=78 lines=21"); //â ũ�� ����

	for (i = 0; i < 39; i++) //��, �Ʒ� �׵θ� ����
	{
		gotoxy(i, 0); printf("��");
		gotoxy(i, 20); printf("��");
	}
	for (i = 1; i < 20; i++) //����, ���� �׵θ� ����
	{
		gotoxy(0, i); printf("��");
		gotoxy(38, i); printf("��");
	}

	gotoxy(15, 5); printf("��  ����ã��  ��"); 
	gotoxy(17, 10); printf("���ӽ���");
	gotoxy(17, 12); printf("���Ӽ���");
	gotoxy(17, 14); printf("��������");
}
int select_menu()
{
	int key = 0, cursor = 1, i;

	while (1)
	{
		//-------------------------------------------- ���� Ŀ�� �̵�
		for (i = 0; i < 5; i += 2)
		{
			gotoxy(16, 10 + i); printf(" ");
			gotoxy(21, 10 + i); printf("  ");
		}

		gotoxy(16, 10 + 2 * (cursor - 1)); printf(">");
		gotoxy(21, 10 + 2 * (cursor - 1)); printf(" <");
		//--------------------------------------------

		//-------------------------------------------- Ű���� �Է� ����
		key = _getch();
		if (key == 224) //����Ű�� ��
		{
			key = _getch();
			if (key == 80 || key == 77) //�Ʒ� ����Ű �Ǵ� ���� ����Ű�� ���
			{
				cursor++; //Ŀ���� 1�� ����
			}
			else if (key == 72 || key == 75) //�� ����Ű �Ǵ� ���� ����Ű�� ���
			{
				cursor--; //Ŀ���� 1�� �ø�
			}
		}
		else
		{
			if (key == 13) //enterŰ �� ���
				break; //���ð� ���
		}

		if (cursor == 4) //Ŀ�� ��ġ�� ���� �Ѿ ���
		{
			cursor = 1; //ù��°�� �̵�
		}

		if (cursor == 0) //Ŀ�� ��ġ�� ó���� �Ѿ ���
		{
			cursor = 3; //���������� �̵�
		}
	}
	return cursor;
}

void draw_gamerule()
{
	int i;
	for (i = 0; i < 39; i++)
	{
		gotoxy(i, 0); printf("��");
		gotoxy(i, 20); printf("��");
	}
	for (i = 1; i < 20; i++)
	{
		gotoxy(0, i); printf("��");
		gotoxy(38, i); printf("��");
	}

	gotoxy(15, 5); printf("��  ���� ����  ��");
	gotoxy(16, 12); printf(" ");
	gotoxy(21, 12); printf("  ");
	gotoxy(17, 10); printf("��������");
	gotoxy(17, 12); printf("�����ϱ�");
	gotoxy(8, 14); printf("��Ģ ���Ⱑ ������ �������� �����ϰ��� ��");
	gotoxy(10, 16); printf("�ƹ� ��ư�̳� ������ ��������");

	_getch();
}

void draw_levUI() //����ȭ�� �׷��� ���� �Լ�
{
	clear_GUI(); //ȭ�� �ʱ�ȭ

	gotoxy(15, 5); printf("��  ��������  ��");
	gotoxy(17, 9); printf("  ����  ");
	gotoxy(17, 11); printf("  ����  ");
	gotoxy(17, 13); printf(" ����� ");
	gotoxy(17, 15); printf(" Ŀ���� ");
}
int select_lev()
{
	int key = 0, lev = 1;
	int i;

	while (1)
	{
		//-------------------------------------------- ���� Ŀ�� �̵�
		for (i = 0; i < 7; i += 2)
		{
			gotoxy(16, 9 + i); printf(" ");
			gotoxy(21, 9 + i); printf("  ");
		}

		gotoxy(16, 9 + 2 * (lev - 1)); printf(">");
		gotoxy(21, 9 + 2 * (lev - 1)); printf(" <");
		//--------------------------------------------

		//-------------------------------------------- Ű���� �Է� ����
		key = _getch();
		if (key == 224) //����Ű�� ��
		{
			key = _getch();
			if (key == 80 || key == 77) //�Ʒ� ����Ű �Ǵ� ���� ����Ű�� ���
			{
				lev++; //Ŀ���� 1�� ����
			}
			else if (key == 72 || key == 75) //�� ����Ű �Ǵ� ���� ����Ű�� ���
			{
				lev--; //Ŀ���� 1�� �ø�
			}
		}
		else
		{
			if (key == 13) //enterŰ �� ���
				break; //���ð� ���
		}

		if (lev == 5) //Ŀ�� ��ġ�� ���� �Ѿ ���
		{
			lev = 1; //ù��°�� �̵�
		}

		if (lev == 0) //Ŀ�� ��ġ�� ó���� �Ѿ ���
		{
			lev = 4; //���������� �̵�
		}
	}
	return lev;
}

void set_custom(int* row, int* col, int* mine) //Ŀ���� ���� ����, ����, ���ڰ��� ���� �Լ�
{
	clear_GUI(); //ȭ�� �ʱ�ȭ

	//�׷��� ����
	gotoxy(15, 5); printf("�� Ŀ���Ұ��� ��");
	gotoxy(13, 10); printf("����ũ��:");
	gotoxy(13, 12); printf("����ũ��:");
	gotoxy(13, 14); printf("���ڰ���:");
	
	setcursortype(NORMALCURSOR); //Ŀ�� ����

	//����, ����, ���� ���� �Է�
	gotoxy(18, 10); scanf_s("%d", col);
	gotoxy(18, 12); scanf_s("%d", row);
	gotoxy(18, 14); scanf_s("%d", mine);

	setcursortype(NOCURSOR); //Ŀ�� ����
}

void draw_gameUI(int* row, int* col) //����ȭ�� �׷��� ���� �Լ�
{
	clear_GUI(); //ȭ�� �ʱ�ȭ

	//ȭ�� ũ�� ����
	char command[30] = { '\0' };
	sprintf_s(command, sizeof(command), "mode con: lines=%d cols=%d", *row + 2, (*col + 2) * 2);
	system(command);

	//���� UI��ġ
	int i, j;
	for (i = 0; i < *col; i++)
	{
		for (j = 0; j < *row; j++)
		{
			gotoxy(1 + i, 1 + j); printf("��");
		}
	}
}

void detect_mine(int row, int col, int bpt_r, int bpt_c, char** arr_mine, char** click)
{
	int mine_number = 0;
	int pt_r = bpt_r - 1, pt_c = bpt_c - 1;
	click[pt_r][pt_c] = 'y';

	if (pt_r == 0)
	{
		if (pt_c == 0)
		{
			//���� ���
			if (arr_mine[pt_r + 1][pt_c] == 'x') mine_number++;
			if (arr_mine[pt_r + 1][pt_c + 1] == 'x') mine_number++;
			if (arr_mine[pt_r][pt_c + 1] == 'x') mine_number++;

			if (mine_number == 0)
			{
				if (click[pt_r + 1][pt_c] == 'n')	detect_mine(row, col, bpt_r + 1, bpt_c, arr_mine, click);
				if (click[pt_r + 1][pt_c + 1] == 'n') detect_mine(row, col, bpt_r + 1, bpt_c + 1, arr_mine, click);
				if (click[pt_r][pt_c + 1] == 'n') detect_mine(row, col, bpt_r, bpt_c + 1, arr_mine, click);
			}
		}

		else if (pt_c == col - 1)
		{
			//���� ���
			if (arr_mine[pt_r + 1][pt_c] == 'x') mine_number++;
			if (arr_mine[pt_r + 1][pt_c - 1] == 'x') mine_number++;
			if (arr_mine[pt_r][pt_c - 1] == 'x') mine_number++;

			if (mine_number == 0)
			{
				if (click[pt_r + 1][pt_c] == 'n') detect_mine(row, col, bpt_r + 1, bpt_c, arr_mine, click);
				if (click[pt_r + 1][pt_c - 1] == 'n') detect_mine(row, col, bpt_r + 1, bpt_c - 1, arr_mine, click);
				if (click[pt_r][pt_c - 1] == 'n') detect_mine(row, col, bpt_r, bpt_c - 1, arr_mine, click);
			}
		}

		else
		{
			//���
			if (arr_mine[pt_r + 1][pt_c] == 'x') mine_number++;
			if (arr_mine[pt_r + 1][pt_c - 1] == 'x') mine_number++;
			if (arr_mine[pt_r + 1][pt_c + 1] == 'x') mine_number++;
			if (arr_mine[pt_r][pt_c + 1] == 'x') mine_number++;
			if (arr_mine[pt_r][pt_c - 1] == 'x') mine_number++;

			if (mine_number == 0)
			{
				if (click[pt_r + 1][pt_c] == 'n') detect_mine(row, col, bpt_r + 1, bpt_c, arr_mine, click);
				if (click[pt_r + 1][pt_c - 1] == 'n') detect_mine(row, col, bpt_r + 1, bpt_c - 1, arr_mine, click);
				if (click[pt_r + 1][pt_c + 1] == 'n') detect_mine(row, col, bpt_r + 1, bpt_c + 1, arr_mine, click);
				if (click[pt_r][pt_c + 1] == 'n') detect_mine(row, col, bpt_r, bpt_c + 1, arr_mine, click);
				if (click[pt_r][pt_c - 1] == 'n') detect_mine(row, col, bpt_r, bpt_c - 1, arr_mine, click);
			}
		}
	}

	else if (pt_r == row - 1)
	{
		if (pt_c == 0)
		{
			//���� �ϴ�
			if (arr_mine[pt_r - 1][pt_c] == 'x') mine_number++;
			if (arr_mine[pt_r - 1][pt_c + 1] == 'x') mine_number++;
			if (arr_mine[pt_r][pt_c + 1] == 'x') mine_number++;

			if (mine_number == 0)
			{
				if (click[pt_r - 1][pt_c] == 'n') detect_mine(row, col, bpt_r - 1, bpt_c, arr_mine, click);
				if (click[pt_r - 1][pt_c + 1] == 'n') detect_mine(row, col, bpt_r - 1, bpt_c + 1, arr_mine, click);
				if (click[pt_r][pt_c + 1] == 'n') detect_mine(row, col, bpt_r, bpt_c + 1, arr_mine, click);
			}
		}

		else if (pt_c == col - 1)
		{
			//���� �ϴ�
			if (arr_mine[pt_r - 1][pt_c] == 'x') mine_number++;
			if (arr_mine[pt_r - 1][pt_c - 1] == 'x') mine_number++;
			if (arr_mine[pt_r][pt_c - 1] == 'x') mine_number++;

			if (mine_number == 0)
			{
				if (click[pt_r - 1][pt_c] == 'n') detect_mine(row, col, bpt_r - 1, bpt_c, arr_mine, click);
				if (click[pt_r - 1][pt_c - 1] == 'n') detect_mine(row, col, bpt_r - 1, bpt_c - 1, arr_mine, click);
				if (click[pt_r][pt_c - 1] == 'n') detect_mine(row, col, bpt_r, bpt_c - 1, arr_mine, click);
			}
		}

		else
		{
			//�ϴ�
			if (arr_mine[pt_r - 1][pt_c] == 'x') mine_number++;
			if (arr_mine[pt_r - 1][pt_c - 1] == 'x') mine_number++;
			if (arr_mine[pt_r - 1][pt_c + 1] == 'x') mine_number++;
			if (arr_mine[pt_r][pt_c + 1] == 'x') mine_number++;
			if (arr_mine[pt_r][pt_c - 1] == 'x') mine_number++;

			if (mine_number == 0)
			{
				if (click[pt_r - 1][pt_c] == 'n') detect_mine(row, col, bpt_r - 1, bpt_c, arr_mine, click);
				if (click[pt_r - 1][pt_c - 1] == 'n') detect_mine(row, col, bpt_r - 1, bpt_c - 1, arr_mine, click);
				if (click[pt_r - 1][pt_c + 1] == 'n') detect_mine(row, col, bpt_r - 1, bpt_c + 1, arr_mine, click);
				if (click[pt_r][pt_c + 1] == 'n') detect_mine(row, col, bpt_r, bpt_c + 1, arr_mine, click);
				if (click[pt_r][pt_c - 1] == 'n') detect_mine(row, col, bpt_r, bpt_c - 1, arr_mine, click);
			}
		}
	}

	else if (pt_c == 0)
	{
		//����
		if (arr_mine[pt_r + 1][pt_c] == 'x') mine_number++;
		if (arr_mine[pt_r + 1][pt_c + 1] == 'x') mine_number++;
		if (arr_mine[pt_r - 1][pt_c] == 'x') mine_number++;
		if (arr_mine[pt_r - 1][pt_c + 1] == 'x') mine_number++;
		if (arr_mine[pt_r][pt_c + 1] == 'x') mine_number++;

		if (mine_number == 0)
		{
			if (click[pt_r + 1][pt_c] == 'n') detect_mine(row, col, bpt_r + 1, bpt_c, arr_mine, click);
			if (click[pt_r + 1][pt_c + 1] == 'n') detect_mine(row, col, bpt_r + 1, bpt_c + 1, arr_mine, click);
			if (click[pt_r - 1][pt_c] == 'n') detect_mine(row, col, bpt_r - 1, bpt_c, arr_mine, click);
			if (click[pt_r - 1][pt_c + 1] == 'n') detect_mine(row, col, bpt_r - 1, bpt_c + 1, arr_mine, click);
			if (click[pt_r][pt_c + 1] == 'n') detect_mine(row, col, bpt_r, bpt_c + 1, arr_mine, click);
		}
	}

	else if (pt_c == col - 1)
	{
		//����
		if (arr_mine[pt_r + 1][pt_c] == 'x') mine_number++;
		if (arr_mine[pt_r + 1][pt_c - 1] == 'x') mine_number++;
		if (arr_mine[pt_r - 1][pt_c] == 'x') mine_number++;
		if (arr_mine[pt_r - 1][pt_c - 1] == 'x') mine_number++;
		if (arr_mine[pt_r][pt_c - 1] == 'x') mine_number++;

		if (mine_number == 0)
		{
			if (click[pt_r + 1][pt_c] == 'n') detect_mine(row, col, bpt_r + 1, bpt_c, arr_mine, click);
			if (click[pt_r + 1][pt_c - 1] == 'n') detect_mine(row, col, bpt_r + 1, bpt_c - 1, arr_mine, click);
			if (click[pt_r - 1][pt_c] == 'n') detect_mine(row, col, bpt_r - 1, bpt_c, arr_mine, click);
			if (click[pt_r - 1][pt_c - 1] == 'n') detect_mine(row, col, bpt_r - 1, bpt_c - 1, arr_mine, click);
			if (click[pt_r][pt_c - 1] == 'n') detect_mine(row, col, bpt_r, bpt_c - 1, arr_mine, click);
		}
	}

	else
	{
		//�߰�
		if (arr_mine[pt_r + 1][pt_c] == 'x') mine_number++;
		if (arr_mine[pt_r + 1][pt_c - 1] == 'x') mine_number++;
		if (arr_mine[pt_r + 1][pt_c + 1] == 'x') mine_number++;
		if (arr_mine[pt_r - 1][pt_c] == 'x') mine_number++;
		if (arr_mine[pt_r - 1][pt_c - 1] == 'x') mine_number++;
		if (arr_mine[pt_r - 1][pt_c + 1] == 'x') mine_number++;
		if (arr_mine[pt_r][pt_c + 1] == 'x') mine_number++;
		if (arr_mine[pt_r][pt_c - 1] == 'x') mine_number++;

		if (mine_number == 0)
		{
			if (click[pt_r + 1][pt_c] == 'n') detect_mine(row, col, bpt_r + 1, bpt_c, arr_mine, click);
			if (click[pt_r + 1][pt_c - 1] == 'n') detect_mine(row, col, bpt_r + 1, bpt_c - 1, arr_mine, click);
			if (click[pt_r + 1][pt_c + 1] == 'n') detect_mine(row, col, bpt_r + 1, bpt_c + 1, arr_mine, click);
			if (click[pt_r - 1][pt_c] == 'n') detect_mine(row, col, bpt_r - 1, bpt_c, arr_mine, click);
			if (click[pt_r - 1][pt_c - 1] == 'n') detect_mine(row, col, bpt_r - 1, bpt_c - 1, arr_mine, click);
			if (click[pt_r - 1][pt_c + 1] == 'n') detect_mine(row, col, bpt_r - 1, bpt_c + 1, arr_mine, click);
			if (click[pt_r][pt_c + 1] == 'n') detect_mine(row, col, bpt_r, bpt_c + 1, arr_mine, click);
			if (click[pt_r][pt_c - 1] == 'n') detect_mine(row, col, bpt_r, bpt_c - 1, arr_mine, click);
		}
	}

	if (mine_number == 0)
	{
		gotoxy(bpt_c, bpt_r); printf("  ");
	}
	else
	{
		gotoxy(bpt_c, bpt_r); printf("%d ", mine_number);
	}
}

void game_clear(int row, int col, char** arr_mine)
{
	system("cls");
	setcursortype(NOCURSOR);

	int i, j;
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			gotoxy(j + 1, i + 1);
			if (arr_mine[i][j] == 'o')
			{
				printf("  ");
			}
			else
			{
				printf("��");
				Sleep(50);
			}
		}
		printf("\n");
	}

	gotoxy(0, 0);
	Sleep(500);
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			gotoxy(j + 1, i + 1);
			if (arr_mine[i][j] == 'o')
			{
				printf("  ");
			}
			else
			{
				printf("  ");
				Sleep(50);
			}
		}
		printf("\n");
	}
	Sleep(1000);
	for (i = 0; i < 3; i++)
	{
		gotoxy((col + 2) / 2 - 2, (row + 2) / 2);
		printf("game clear!");
		Sleep(150);
		gotoxy((col + 2) / 2 - 2, (row + 2) / 2);
		printf("           ");
		Sleep(150);
	}
	gotoxy((col + 2) / 2 - 2, (row + 2) / 2);
	printf("game clear!");

}

void game_over(int row, int col, char** arr_mine)
{
	system("cls");
	setcursortype(NOCURSOR);
	int i, j;
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			gotoxy(j + 1, i + 1);
			if (arr_mine[i][j] == 'o')
			{
				printf("  ");
			}
			else
			{
				printf("��");
				Sleep(50);
			}
		}
		printf("\n");
	}

	gotoxy(0, 0);
	Sleep(500);
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			gotoxy(j + 1, i + 1);
			if (arr_mine[i][j] == 'o')
			{
				printf("  ");
			}
			else
			{
				printf("  ");
				Sleep(50);
			}
		}
		printf("\n");
	}
	Sleep(1000);

	for (i = 0; i < 3; i++)
	{
		gotoxy((col + 2) / 2 - 2, (row + 2) / 2);
		printf("game over!");
		Sleep(150);
		gotoxy((col + 2) / 2 - 2, (row + 2) / 2);
		printf("          ");
		Sleep(150);
	}
	gotoxy((col + 2) / 2 - 2, (row + 2) / 2);
	printf("game over!");

}

void flag(int point_c, int point_r, char** click)
{
	int pt_r = point_r - 1, pt_c = point_c - 1;
	if (click[pt_r][pt_c] == 'n')
	{
		click[pt_r][pt_c] = 'f';
		printf("��");
	}
	else if (click[pt_r][pt_c] == 'f')
	{
		click[pt_r][pt_c] = 'n';
		printf("��");
	}
	else;
}