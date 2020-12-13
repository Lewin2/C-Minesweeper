#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //커서제거 함수에 사용되는 열거형 

void gotoxy(int x, int y) //gotoxy 함수
{
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void setcursortype(CURSOR_TYPE c) { //커서제거 함수 
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
	setcursortype(NOCURSOR); //커서 제거

	while (1) //전체게임 반복문
	{
		//메뉴화면 --------------------------------------------------------------------------------------
		
		while (1)
		{
			//-----------그래픽 구현
			draw_menuUI();

			//-----------선택 함수
			int menu = select_menu();

			//-----------선택 결과에 따라 다음 단계 실행
			//1.게임실행 선택 시
			if (menu == 1) 
			{
				break; //메뉴화면 반복문 종료
			}

			//2.게임설명 선택 시
			else if (menu == 2)
			{
				draw_gamerule(); //게임 룰 설명 그래픽 구현
			}

			//그 외(3.게임종료) 선택 시
			else 
			{
				exit(0); //프로그램 종료
			}
		}

		//레벨 선택 화면	--------------------------------------------------------------------------------
		
		//-----------그래픽 구현
		draw_levUI();

		//-----------선택 함수
		int lev = select_lev();

		//-----------선택 결과에 따라 다음 단계 실행
		int row, col, mine; //지뢰 게임판(행, 열, 지뢰 개수)
		
		//1.쉬움 선택 시 
		if (lev == 1) 
		{
			row = 9, col = 9, mine = 10; //9x9 지뢰개수:10
		}

		//2.보통 선택 시 
		else if (lev == 2)
		{
			row = 16, col = 16, mine = 40; //16x16 지뢰개수:40
		}

		//3.어려움 선택 시 
		else if (lev == 3)
		{
			row = 16, col = 30, mine = 49; //16x30 지뢰개수:49
		}

		//그 외(4.커스텀) 선택 시
		else
		{
			set_custom(&row, &col, &mine); //커스텀 게임 가로, 세로, 지뢰개수 설정
		}

		
		//게임 화면 -------------------------------------------------------------------------------------
		
		//-----------그래픽 구현
		draw_gameUI(&row, &col);
		
		//-----------게임 시작
		int i, j, k;

		srand(time(NULL)); //난수표 생성

		//2차원 배열(지뢰존재 배열) 동적 할당
		char** arr_mine = malloc(sizeof(char*) * row);
		for (i = 0; i < row; i++)
		{
			arr_mine[i] = malloc(sizeof(char) * col);
		}

		//지뢰존재 배열 'o'로 초기화
		for (i = 0; i < row; i++)
		{
			for (j = 0; j < col; j++)
			{
				arr_mine[i][j] = 'o';
			}
		}

		//2차원 배열(클릭 감지 배열) 동적 할당
		char** click = malloc(sizeof(char*) * row);
		for (i = 0; i < row; i++)
		{
			click[i] = malloc(sizeof(char) * col);
		}

		//클릭 감지 배열 'o'로 초기화
		for (i = 0; i < row; i++)
		{
			for (j = 0; j < col; j++)
			{
				click[i][j] = 'n';
			}
		}



		//지뢰 배치
		for (i = 0; i < mine; i++) //지뢰의 총 개수만큼 반복
		{
			int mine_count = 0; //배치된 지뢰 개수
			arr_mine[rand() % row][rand() % col] = 'x'; //랜덤 자리에 지뢰 배치('x'입력)

			for (j = 0; j < row; j++) //지뢰 배열 전체 반복
			{
				for (k = 0; k < col; k++)
				{
					if (arr_mine[j][k] == 'x') //만약 지뢰가 있다면
					{
						mine_count++; //배치된 지뢰 개수를 +1 
					}
				}
			}

			if (mine_count - 1 != i) //만약 배치된 지뢰 개수가 배치한 지뢰 개수(i)와 다르면
			{
				i--; //한번 더 실행하여라
			}
		}

		setcursortype(SOLIDCURSOR); //커서 제거


//------테스트--------------------------------
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
		// 여기서부터 주석 작성 필요
		//----------------------------------------------------------------------------------------------------

		int point_r = 1;
		int point_c = 1;

		while (1)
		{
			while (1)
			{
				gotoxy(point_c, point_r);
				int key = _getch();
				if (key == 224) //방향키일 때
				{
					key = _getch();
					if (key == 80) //아래 방향키인 경우
					{
						point_r++;
					}
					else if (key == 72) //위 방향키인 경우
					{
						point_r--;
					}
					else if (key == 77) //우측 방향키인 경우
					{
						point_c++;
					}
					else if (key == 75) //좌측 방향키인 경우
					{
						point_c--;
					}
				}
				else
				{
					if (key == 13) //enter키 일 경우
					{
						if (click[point_r - 1][point_c - 1] == 'f');
						else
							break; //선택값 출력
					}
					else if (key == 32)
						flag(point_c, point_r, click);
				}

				if (point_r == 0) //커서 위치가 위쪽을 넘어간 경우
				{
					point_r = row; //마지막으로 이동
				}

				if (point_r == row + 1) //커서 위치가 아래쪽을 넘어간 경우
				{
					point_r = 1; //처음으로 이동
				}

				if (point_c == col + 1) //커서 위치가 오른쪽을 넘어간 경우
				{
					point_c = 1; //왼쪽으로 이동
				}

				if (point_c == 0) //커서 위치가 왼쪽을 넘어간 경우
				{
					point_c = col; //오른쪽으로 이동
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
		

		//동적 배열 메모리 해제
		for (i = 0; i < row - 1; i++)
		{
			free(arr_mine[i]);
		}
		free(arr_mine);

		_getch(); //일시정지 함수(추후 제거)
	}
}

void clear_GUI() //화면 초기화 함수
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

void draw_menuUI() //메뉴화면 그래픽 구현 함수
{
	int i;
	system("mode con cols=78 lines=21"); //창 크기 설정

	for (i = 0; i < 39; i++) //위, 아래 테두리 생성
	{
		gotoxy(i, 0); printf("■");
		gotoxy(i, 20); printf("■");
	}
	for (i = 1; i < 20; i++) //좌측, 우측 테두리 생성
	{
		gotoxy(0, i); printf("■");
		gotoxy(38, i); printf("■");
	}

	gotoxy(15, 5); printf("▩  지뢰찾기  ▩"); 
	gotoxy(17, 10); printf("게임시작");
	gotoxy(17, 12); printf("게임설명");
	gotoxy(17, 14); printf("게임종료");
}
int select_menu()
{
	int key = 0, cursor = 1, i;

	while (1)
	{
		//-------------------------------------------- 선택 커서 이동
		for (i = 0; i < 5; i += 2)
		{
			gotoxy(16, 10 + i); printf(" ");
			gotoxy(21, 10 + i); printf("  ");
		}

		gotoxy(16, 10 + 2 * (cursor - 1)); printf(">");
		gotoxy(21, 10 + 2 * (cursor - 1)); printf(" <");
		//--------------------------------------------

		//-------------------------------------------- 키보드 입력 감지
		key = _getch();
		if (key == 224) //방향키일 때
		{
			key = _getch();
			if (key == 80 || key == 77) //아래 방향키 또는 우측 방향키일 경우
			{
				cursor++; //커서를 1개 내림
			}
			else if (key == 72 || key == 75) //위 방향키 또는 좌측 방향키인 경우
			{
				cursor--; //커서를 1개 올림
			}
		}
		else
		{
			if (key == 13) //enter키 일 경우
				break; //선택값 출력
		}

		if (cursor == 4) //커서 위치가 끝을 넘어간 경우
		{
			cursor = 1; //첫번째로 이동
		}

		if (cursor == 0) //커서 위치가 처음을 넘어간 경우
		{
			cursor = 3; //마지막으로 이동
		}
	}
	return cursor;
}

void draw_gamerule()
{
	int i;
	for (i = 0; i < 39; i++)
	{
		gotoxy(i, 0); printf("■");
		gotoxy(i, 20); printf("■");
	}
	for (i = 1; i < 20; i++)
	{
		gotoxy(0, i); printf("■");
		gotoxy(38, i); printf("■");
	}

	gotoxy(15, 5); printf("▩  추후 구현  ▩");
	gotoxy(16, 12); printf(" ");
	gotoxy(21, 12); printf("  ");
	gotoxy(17, 10); printf("언젠가는");
	gotoxy(17, 12); printf("구현하기");
	gotoxy(8, 14); printf("규칙 쓰기가 귀찮다 언젠가는 구현하겠지 뭐");
	gotoxy(10, 16); printf("아무 버튼이나 누르면 나가진다");

	_getch();
}

void draw_levUI() //레벨화면 그래픽 구현 함수
{
	clear_GUI(); //화면 초기화

	gotoxy(15, 5); printf("▩  레벨선택  ▩");
	gotoxy(17, 9); printf("  쉬움  ");
	gotoxy(17, 11); printf("  보통  ");
	gotoxy(17, 13); printf(" 어려움 ");
	gotoxy(17, 15); printf(" 커스텀 ");
}
int select_lev()
{
	int key = 0, lev = 1;
	int i;

	while (1)
	{
		//-------------------------------------------- 선택 커서 이동
		for (i = 0; i < 7; i += 2)
		{
			gotoxy(16, 9 + i); printf(" ");
			gotoxy(21, 9 + i); printf("  ");
		}

		gotoxy(16, 9 + 2 * (lev - 1)); printf(">");
		gotoxy(21, 9 + 2 * (lev - 1)); printf(" <");
		//--------------------------------------------

		//-------------------------------------------- 키보드 입력 감지
		key = _getch();
		if (key == 224) //방향키일 때
		{
			key = _getch();
			if (key == 80 || key == 77) //아래 방향키 또는 우측 방향키일 경우
			{
				lev++; //커서를 1개 내림
			}
			else if (key == 72 || key == 75) //위 방향키 또는 좌측 방향키인 경우
			{
				lev--; //커서를 1개 올림
			}
		}
		else
		{
			if (key == 13) //enter키 일 경우
				break; //선택값 출력
		}

		if (lev == 5) //커서 위치가 끝을 넘어간 경우
		{
			lev = 1; //첫번째로 이동
		}

		if (lev == 0) //커서 위치가 처음을 넘어간 경우
		{
			lev = 4; //마지막으로 이동
		}
	}
	return lev;
}

void set_custom(int* row, int* col, int* mine) //커스텀 게임 가로, 세로, 지뢰개수 설정 함수
{
	clear_GUI(); //화면 초기화

	//그래픽 구현
	gotoxy(15, 5); printf("▩ 커스텀게임 ▩");
	gotoxy(13, 10); printf("가로크기:");
	gotoxy(13, 12); printf("세로크기:");
	gotoxy(13, 14); printf("지뢰개수:");
	
	setcursortype(NORMALCURSOR); //커서 생성

	//가로, 세로, 지뢰 개수 입력
	gotoxy(18, 10); scanf_s("%d", col);
	gotoxy(18, 12); scanf_s("%d", row);
	gotoxy(18, 14); scanf_s("%d", mine);

	setcursortype(NOCURSOR); //커서 제거
}

void draw_gameUI(int* row, int* col) //게임화면 그래픽 구현 함수
{
	clear_GUI(); //화면 초기화

	//화면 크기 설정
	char command[30] = { '\0' };
	sprintf_s(command, sizeof(command), "mode con: lines=%d cols=%d", *row + 2, (*col + 2) * 2);
	system(command);

	//지뢰 UI배치
	int i, j;
	for (i = 0; i < *col; i++)
	{
		for (j = 0; j < *row; j++)
		{
			gotoxy(1 + i, 1 + j); printf("■");
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
			//좌측 상단
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
			//우측 상단
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
			//상단
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
			//좌측 하단
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
			//우측 하단
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
			//하단
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
		//좌측
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
		//우측
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
		//중간
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
				printf("※");
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
				printf("※");
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
		printf("▶");
	}
	else if (click[pt_r][pt_c] == 'f')
	{
		click[pt_r][pt_c] = 'n';
		printf("■");
	}
	else;
}