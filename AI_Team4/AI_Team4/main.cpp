

#include<string>
#include<vector>
#include<time.h>
#include "CSetNode.h"
#include <fstream>
#include<stdio.h>
#include<iostream>
#include<Windows.h>
#include<time.h>
#include<random>
#define _AFXDLL

using namespace std;
static int map[8][8] = { 0 }; // 현재 플레이중인 맵
void put(int player, int col, int *x);
void put(int player, int x, int y);

int main() {
	char bufAn[500]; // an.sgf파일 내용
	int fsize; // an.sgf파일크기
	string seq; // 기록된 플레이 순서
	

	// an.sgf에서 현재 플레이 상태 읽어서 map의 8by8 배열로 저장
	FILE *fp;
	fopen_s(&fp,"an.sgf", "r");
	char buf[10] = { 0 };

	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if (fsize != 0) {
		while (fread(bufAn, sizeof(char), fsize, fp));
		fseek(fp, 0, SEEK_SET);

		for (int i = 0; i < fsize / 6; i++) {
			fread(buf, sizeof(char), 1, fp);
			fread(buf, sizeof(char), 1, fp);
			int state;
			if (buf[0] == 'B')
				state = 1;
			else
				state = 2;
			fread(buf, sizeof(char), 1, fp);
			fread(buf, sizeof(char), 1, fp);
			int xindex;
			switch (buf[0]) {
			case 'a':
				xindex = 0;
				seq.append("1");
				break;
			case 'b':
				xindex = 1;
				seq.append("2");
				break;
			case 'c':
				xindex = 2;
				seq.append("3");
				break;
			case 'd':
				xindex = 3;
				seq.append("4");
				break;
			case 'e':
				xindex = 4;
				seq.append("5");
				break;
			case 'f':
				xindex = 5;
				seq.append("6");
				break;
			case 'g':
				xindex = 6;
				seq.append("7");
				break;
			case 'h':
				xindex = 7;
				seq.append("8");
				break;
			}
			fread(buf, sizeof(char), 1, fp);
			int yindex;
			switch (buf[0]) {
			case 'a':
				yindex = 0;
				break;
			case 'b':
				yindex = 1;
				break;
			case 'c':
				yindex = 2;
				break;
			case 'd':
				yindex = 3;
				break;
			case 'e':
				yindex = 4;
				break;
			case 'f':
				yindex = 5;
				break;
			case 'g':
				yindex = 6;
				break;
			case 'h':
				yindex = 7;
				break;
			}
			fread(buf, sizeof(char), 1, fp);
			map[yindex][xindex] = state;

		}
	}
	int player = seq.length() % 2 + 1;
	fclose(fp);
	//////////////////////////////////////
	
	clock_t timeStart, timeEnd;
	timeStart = clock();
	srand((unsigned)time(NULL));
	int x=rand()%6 + 1; // 만약 첫 수라면 랜덤(1~6)
	int y = 0;
	if(seq.length()!=0) {
		x = CSetNode::evaluate(player, map);
	}

	put(player, x,&y);
	//노드 위치
	timeEnd = clock();
	string str= "수행 시간 : "+to_string(((double)(timeEnd - timeStart)) / CLOCKS_PER_SEC);
	string str2 = "x:" + to_string(x+1) + " y:" + to_string(y+1);
	cout << str << endl;
	cout << str2 << endl;

	//////////////////////////////////////

	// re.sgf파일에 결과 쓰기
	int playedNum = seq.length();
	buf[0] = ';';
	if (playedNum % 2 == 0)
		buf[1] = 'B';
	else
		buf[1] = 'W';
	buf[2] = '[';
	switch (x)
	{
	case 0:
		buf[3] = 'a';
		break;
	case 1:
		buf[3] = 'b';
		break;
	case 2:
		buf[3] = 'c';
		break;
	case 3:
		buf[3] = 'd';
		break;
	case 4:
		buf[3] = 'e';
		break;
	case 5:
		buf[3] = 'f';
		break;
	case 6:
		buf[3] = 'g';
		break;
	case 7:
		buf[3] = 'h';
		break;
	default:
		break;
	}
	switch (y)
	{
	case 0:
		buf[4] = 'a';
		break;
	case 1:
		buf[4] = 'b';
		break;
	case 2:
		buf[4] = 'c';
		break;
	case 3:
		buf[4] = 'd';
		break;
	case 4:
		buf[4] = 'e';
		break;
	case 5:
		buf[4] = 'f';
		break;
	case 6:
		buf[4] = 'g';
		break;
	case 7:
		buf[4] = 'h';
		break;
	default:
		break;
	}
	buf[5] = ']';

	fopen_s(&fp, "re.sgf", "wt");
	fseek(fp, 0, SEEK_SET);

	while (fsize != fwrite(bufAn, sizeof(char), fsize, fp));
	fwrite(buf, sizeof(char), 6, fp);
	fclose(fp);
	return 0;
}


 void put(int player, int col,int *x) {  // col에서 둘 수 있는 가장 아래에 돌을 둠
	for (int i = 7; i >= 0; i--) {
		if (map[i][col] == 0) {
			put(player, i, col);
			*x = i;
			break;
		}
	}
}


 void put(int player, int x, int y) { // x, y에 돌을 둠

	if (player != 1 && player != 2)
		return;
	map[x][y] = player;

}