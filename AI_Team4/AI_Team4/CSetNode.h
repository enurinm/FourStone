#pragma once
#include "CMyNode.h"
#include<vector>
#include<string>
using namespace std;
class CSetNode
{
public:
	const static int pos_INF = INT_MAX;
	const static int neg_INF = INT_MIN;
	CSetNode();
	~CSetNode();
	static int evaluate(int player, int map[8][8]);
	static void Minimax(CMyNode* root, int depth);
	static int ABPruning(CMyNode* Parent, int player, int alpha, int beta, int depth);
	static void CopyMap(int copyMap[8][8],int  map[8][8]);
	static void PutInCol(int map[8][8],int player,int col);
	static bool CanPutInCol(int map[8][8], int col);
	//가중치 계산
	static int getScore(int player, int  board[8][8]);
	static int checkHorizontalScore(int player, int board[8][8]);
	static int checkVerticalScore(int player, int board[8][8]);
	static int checkDiaRLDScore(int player, int board[8][8]);
	static int checkDiaLRDScore(int player, int board[8][8]);
};

