

#include<string.h>
#include "CSetNode.h"
#include<vector>
#include<iostream>
#define ONESTONE 100
#define TWOSTONE 500
#define THREESTONE 1400
CSetNode::CSetNode()
{
}


CSetNode::~CSetNode()
{
}

int CSetNode::evaluate(int player, int map[8][8])
{
	int result = -1;
	int depth = 6;
	int switchedPlayer = (player == 1) ? 2 : 1;
	CMyNode root(switchedPlayer, map);

	CMyNode moveWIN(switchedPlayer, map);
	moveWIN.GenChild();
	for (int i = 0; (unsigned)i < moveWIN.children.size(); i++)
	{
		CMyNode *d = &moveWIN.children.at(i);
		d->score = getScore(player, d->map);
		if (d->score == pos_INF)
			return d->updatedCol;
	}

	CMyNode moveLOSE(player, map);
	moveLOSE.GenChild();
	for (int i = 0; (unsigned)i < moveLOSE.children.size(); i++)
	{
		CMyNode *d = &moveLOSE.children.at(i);
		d->score = getScore(player, d->map);
		if (d->score == neg_INF)
			return d->updatedCol;
	}

	Minimax(&root, depth);
	if (root.children.size() != 0) {
		int maxScore = INT_MIN;
		int scoreList[8] = { 0 };
		for (int i = 0; (unsigned)i < root.children.size(); i++)
		{
			CMyNode d = root.children.at(i);
			scoreList[d.updatedCol] = d.score;
			if (d.score > maxScore) {
				maxScore = d.score;
				result = d.updatedCol;
			}
			else if (d.score == maxScore) {
				if (d.score == neg_INF) {
					maxScore = d.score;
					result = d.updatedCol;
				}
			}
		}

		bool bNextLose = true;
		int count = 0;
		while (bNextLose)
		{
			int cp[8][8];
			CopyMap(cp, map);
			PutInCol(cp, player, result);
			PutInCol(cp, switchedPlayer, result);
			int scoreAfter = getScore(player, cp);
			if (scoreAfter == neg_INF)
			{
				bNextLose = true;
				maxScore = INT_MIN;
				for (int i = 0; (unsigned)i < root.children.size(); i++)
				{
					CMyNode d = root.children.at(i);
					if (d.score == scoreList[result] && i != result)
					{
						maxScore = d.score;
						result = d.updatedCol;
					}
					else if(d.score != scoreList[result])
					{
						if (d.score > maxScore) {
							maxScore = d.score;
							result = d.updatedCol;
						}
						else if (d.score == maxScore) {
							if (d.score == neg_INF) {
								maxScore = d.score;
								result = d.updatedCol;
							}
						}
					}
				}
			}
			else
				bNextLose = false;
			if (count++ > 10)
			{
				cout << "점수가 모두 같아 무한루프 돌다가 카운트 오버로 빠져나옴" << endl;
				if (result >= 0 && result < 7) {
					while (!CanPutInCol(map, result))
						result++;
				}
				else {
					while (!CanPutInCol(map, result))
						result = 0;
				}
				break;
			}
		}
	}
	return result;
}

void CSetNode::Minimax(CMyNode *root, int depth)
{
	int alpha = INT_MIN;
	int beta = INT_MAX;
	int player = root->player;
	int switchedPlayer = (player == 1) ? 2 : 1;
	ABPruning(root, switchedPlayer, alpha, beta, depth);

}

int CSetNode::ABPruning(CMyNode *Parent, int player, int alpha, int beta, int depth)
{
	int score;
	if (depth == 0) {
		score = getScore(player, Parent->map);
		Parent->score = score;
	/*	for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				cout << Parent->map[i][j] << " ";
			}
			cout << endl;
		}*/

		return score;
	}
	else {
		Parent->GenChild();
		if (Parent->player == player) {
			for (int i = 0; (unsigned)i < Parent->children.size(); i++)
			{
				CMyNode *d = &Parent->children.at(i);
				
				score= ABPruning(d, player, alpha, beta, depth-1);
				if (beta > score)
					beta = score;
				if (beta <= alpha)
					break;
			}
			Parent->score = beta;
			return beta;
		}
		else {
			for (int i = 0; (unsigned)i < Parent->children.size(); i++)
			{
				CMyNode *d = &Parent->children.at(i);
				
				score = ABPruning(d, player, alpha, beta, depth-1);
				if (alpha < score)
					alpha = score;
				if (beta <= alpha)
					break;
			}
			Parent->score = alpha;
			return alpha;
		}
	}

	return 0;
}

void CSetNode::CopyMap(int copyMap[8][8],int map[8][8])
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			copyMap[i][j] = map[i][j];
		}
	}
	return;
}

void CSetNode::PutInCol(int map[8][8], int player, int col)
{
	for (int i = 7; i >=0 ; i--)
	{
		if (map[i][col] == 0) {
			map[i][col] = player;
			return;
		}
	}
}

bool CSetNode::CanPutInCol(int map[8][8], int col)
{
	bool b = false;
	for (int i = 7; i >= 0; i--)
	{
		if (map[i][col] == 0)
		{
			b = true;
			break;
		}
	}
	return b;
}

int CSetNode::getScore(int player, int map[8][8])
{
	int resultScore = 0;
	int temp = 0;

	temp = checkHorizontalScore(player, map);
	if (temp == pos_INF || temp == neg_INF)
		return temp;

	resultScore += temp;

	temp = checkVerticalScore(player, map);
	if (temp == pos_INF || temp == neg_INF)
		return temp;

	resultScore += temp;

	temp = checkDiaRLDScore(player, map);
	if (temp == pos_INF || temp == neg_INF)
		return temp;

	resultScore += temp;

	temp = checkDiaLRDScore(player, map);
	if (temp == pos_INF || temp == neg_INF)
		return temp;

	resultScore += temp;
	
	return resultScore;
}

int CSetNode::checkHorizontalScore(int player, int board[8][8])
{

	int resultScore = 0;
	int enemy = (player == 1) ? 2 : 1;

	for (int row = 0; row < 8; row++) {
		for (int col = 0; col <= 4; col++) {

			// 돌 1개 놓인 것 체크
			if (board[row][col] == enemy && board[row][col + 1] == 0 && board[row][col + 2] == 0
				&& board[row][col + 3] == 0)
				resultScore = resultScore - ONESTONE;

			if (board[row][col] == 0 && board[row][col + 1] == enemy && board[row][col + 2] == 0
				&& board[row][col + 3] == 0)
				resultScore = resultScore - ONESTONE;

			if (board[row][col] == 0 && board[row][col + 1] == 0 && board[row][col + 2] == enemy
				&& board[row][col + 3] == 0)
				resultScore = resultScore - ONESTONE;

			if (board[row][col] == 0 && board[row][col + 1] == 0 && board[row][col + 2] == 0
				&& board[row][col + 3] == enemy)
				resultScore = resultScore - ONESTONE;

			if (board[row][col] == player && board[row][col + 1] == 0 && board[row][col + 2] == 0
				&& board[row][col + 3] == 0)
				resultScore = resultScore + ONESTONE;

			if (board[row][col] == 0 && board[row][col + 1] == player && board[row][col + 2] == 0
				&& board[row][col + 3] == 0)
				resultScore = resultScore + ONESTONE;

			if (board[row][col] == 0 && board[row][col + 1] == 0 && board[row][col + 2] == player
				&& board[row][col + 3] == 0)
				resultScore = resultScore + ONESTONE;

			if (board[row][col] == 0 && board[row][col + 1] == 0 && board[row][col + 2] == 0
				&& board[row][col + 3] == player)
				resultScore = resultScore + ONESTONE;

			// 돌 2개 놓인 것 체크
			if (board[row][col] == enemy && board[row][col + 1] == enemy && board[row][col + 2] == 0
				&& board[row][col + 3] == 0)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == 0 && board[row][col + 1] == enemy && board[row][col + 2] == enemy
				&& board[row][col + 3] == 0)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == 0 && board[row][col + 1] == 0 && board[row][col + 2] == enemy
				&& board[row][col + 3] == enemy)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == enemy && board[row][col + 1] == 0 && board[row][col + 2] == enemy
				&& board[row][col + 3] == 0)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == 0 && board[row][col + 1] == enemy && board[row][col + 2] == 0
				&& board[row][col + 3] == enemy)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == enemy && board[row][col + 1] == 0 && board[row][col + 2] == 0
				&& board[row][col + 3] == enemy)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == player && board[row][col + 1] == player && board[row][col + 2] == 0
				&& board[row][col + 3] == 0)
				resultScore = resultScore + TWOSTONE;

			if (board[row][col] == 0 && board[row][col + 1] == player && board[row][col + 2] == player
				&& board[row][col + 3] == 0)
				resultScore = resultScore + TWOSTONE;

			if (board[row][col] == 0 && board[row][col + 1] == 0 && board[row][col + 2] == player
				&& board[row][col + 3] == player)
				resultScore = resultScore + TWOSTONE;

			if (board[row][col] == player && board[row][col + 1] == 0 && board[row][col + 2] == player
				&& board[row][col + 3] == 0)
				resultScore = resultScore + TWOSTONE;

			if (board[row][col] == 0 && board[row][col + 1] == player && board[row][col + 2] == 0
				&& board[row][col + 3] == player)
				resultScore = resultScore + TWOSTONE;

			if (board[row][col] == player && board[row][col + 1] == 0 && board[row][col + 2] == 0
				&& board[row][col + 3] == player)
				resultScore = resultScore + TWOSTONE;

			// 돌 3개 놓인 것 체크
			if (board[row][col] == enemy && board[row][col + 1] == enemy && board[row][col + 2] == enemy
				&& board[row][col + 3] == 0)
				resultScore = resultScore - THREESTONE;

			if (board[row][col] == enemy && board[row][col + 1] == enemy && board[row][col + 2] == 0
				&& board[row][col + 3] == enemy)
				resultScore = resultScore - THREESTONE;

			if (board[row][col] == enemy && board[row][col + 1] == 0 && board[row][col + 2] == enemy
				&& board[row][col + 3] == enemy)
				resultScore = resultScore - THREESTONE;

			if (board[row][col] == 0 && board[row][col + 1] == enemy && board[row][col + 2] == enemy
				&& board[row][col + 3] == enemy)
				resultScore = resultScore - THREESTONE;

			if (board[row][col] == player && board[row][col + 1] == player && board[row][col + 2] == player
				&& board[row][col + 3] == 0)
				resultScore = resultScore + THREESTONE;

			if (board[row][col] == player && board[row][col + 1] == player && board[row][col + 2] == 0
				&& board[row][col + 3] == player)
				resultScore = resultScore + THREESTONE;

			if (board[row][col] == player && board[row][col + 1] == 0 && board[row][col + 2] == player
				&& board[row][col + 3] == player)
				resultScore = resultScore + THREESTONE;

			if (board[row][col] == 0 && board[row][col + 1] == player && board[row][col + 2] == player
				&& board[row][col + 3] == player)
				resultScore = resultScore + THREESTONE;

			if (board[row][col] == player && board[row][col + 1] == player && board[row][col + 2] == player
				&& board[row][col + 3] == player)
				return pos_INF;

			if (board[row][col] == enemy && board[row][col + 1] == enemy && board[row][col + 2] == enemy
				&& board[row][col + 3] == enemy)
				return neg_INF;

		}
	}

	return resultScore;
}

int CSetNode::checkVerticalScore(int player, int board[8][8])
{
	int resultScore = 0;
	int enemy = (player == 1) ? 2 : 1;

	for (int row = 7; row >= 3; row--) {
		for (int col = 0; col < 8; col++) {

			// 돌 1개 놓인 것 체크
			if (board[row][col] == enemy && board[row - 1][col] == 0 && board[row - 2][col] == 0
				&& board[row - 3][col] == 0)
				resultScore = resultScore - ONESTONE;

			if (board[row][col] == 0 && board[row - 1][col] == enemy && board[row - 2][col] == 0
				&& board[row - 3][col] == 0)
				resultScore = resultScore - ONESTONE;

			if (board[row][col] == 0 && board[row - 1][col] == 0 && board[row - 2][col] == enemy
				&& board[row - 3][col] == 0)
				resultScore = resultScore - ONESTONE;

			if (board[row][col] == 0 && board[row - 1][col] == 0 && board[row - 2][col] == 0
				&& board[row - 3][col] == enemy)
				resultScore = resultScore - ONESTONE;

			if (board[row][col] == player && board[row - 1][col] == 0 && board[row - 2][col] == 0
				&& board[row - 3][col] == 0)
				resultScore = resultScore + ONESTONE;

			if (board[row][col] == 0 && board[row - 1][col] == player && board[row - 2][col] == 0
				&& board[row - 3][col] == 0)
				resultScore = resultScore + ONESTONE;

			if (board[row][col] == 0 && board[row - 1][col] == 0 && board[row - 2][col] == player
				&& board[row - 3][col] == 0)
				resultScore = resultScore + ONESTONE;

			if (board[row][col] == 0 && board[row - 1][col] == 0 && board[row - 2][col] == 0
				&& board[row - 3][col] == player)
				resultScore = resultScore + ONESTONE;

			// 돌 2개 놓인 것 체크
			if (board[row][col] == enemy && board[row - 1][col] == enemy && board[row - 2][col] == 0
				&& board[row - 3][col] == 0)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == 0 && board[row - 1][col] == enemy && board[row - 2][col] == enemy
				&& board[row - 3][col] == 0)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == 0 && board[row - 1][col] == 0 && board[row - 2][col] == enemy
				&& board[row - 3][col] == enemy)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == enemy && board[row - 1][col] == 0 && board[row - 2][col] == enemy
				&& board[row - 3][col] == 0)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == 0 && board[row - 1][col] == enemy && board[row - 2][col] == 0
				&& board[row - 3][col] == enemy)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == enemy && board[row - 1][col] == 0 && board[row - 2][col] == 0
				&& board[row - 3][col] == enemy)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == player && board[row - 1][col] == player && board[row - 2][col] == 0
				&& board[row - 3][col] == 0)
				resultScore = resultScore + TWOSTONE;

			if (board[row][col] == 0 && board[row - 1][col] == player && board[row - 2][col] == player
				&& board[row - 3][col] == 0)
				resultScore = resultScore + TWOSTONE;

			if (board[row][col] == 0 && board[row - 1][col] == 0 && board[row - 2][col] == player
				&& board[row - 3][col] == player)
				resultScore = resultScore + TWOSTONE;

			if (board[row][col] == player && board[row - 1][col] == 0 && board[row - 2][col] == player
				&& board[row - 3][col] == 0)
				resultScore = resultScore + TWOSTONE;

			if (board[row][col] == 0 && board[row - 1][col] == player && board[row - 2][col] == 0
				&& board[row - 3][col] == player)
				resultScore = resultScore + TWOSTONE;

			if (board[row][col] == player && board[row - 1][col] == 0 && board[row - 2][col] == 0
				&& board[row - 3][col] == player)
				resultScore = resultScore + TWOSTONE;

			// 돌 3개 놓인 것 체크
			if (board[row][col] == enemy && board[row - 1][col] == enemy && board[row - 2][col] == enemy
				&& board[row - 3][col] == 0)
				resultScore = resultScore - THREESTONE;

			if (board[row][col] == enemy && board[row - 1][col] == enemy && board[row - 2][col] == 0
				&& board[row - 3][col] == enemy)
				resultScore = resultScore - THREESTONE;

			if (board[row][col] == enemy && board[row - 1][col] == 0 && board[row - 2][col] == enemy
				&& board[row - 3][col] == enemy)
				resultScore = resultScore - THREESTONE;

			if (board[row][col] == 0 && board[row - 1][col] == enemy && board[row - 2][col] == enemy
				&& board[row - 3][col] == enemy)
				resultScore = resultScore - THREESTONE;

			if (board[row][col] == player && board[row - 1][col] == player && board[row - 2][col] == player
				&& board[row - 3][col] == 0)
				resultScore = resultScore + THREESTONE;

			if (board[row][col] == player && board[row - 1][col] == player && board[row - 2][col] == 0
				&& board[row - 3][col] == player)
				resultScore = resultScore + THREESTONE;

			if (board[row][col] == player && board[row - 1][col] == 0 && board[row - 2][col] == player
				&& board[row - 3][col] == player)
				resultScore = resultScore + THREESTONE;

			if (board[row][col] == 0 && board[row - 1][col] == player && board[row - 2][col] == player
				&& board[row - 3][col] == player)
				resultScore = resultScore + THREESTONE;

			if (board[row][col] == player && board[row - 1][col] == player && board[row - 2][col] == player
				&& board[row - 3][col] == player)
				return pos_INF;

			if (board[row][col] == enemy && board[row - 1][col] == enemy && board[row - 2][col] == enemy
				&& board[row - 3][col] == enemy)
				return neg_INF;

		}
	}

	return resultScore;
}

int CSetNode::checkDiaRLDScore(int player, int board[8][8])
{
	int resultScore = 0;
	int enemy = (player == 1) ? 2 : 1;

	for (int row = 0; row <= 4; row++) {
		for (int col = 7; col >= 3; col--) {

			// 돌 1개 놓인 것 체크
			if (board[row][col] == enemy && board[row + 1][col - 1] == 0 && board[row + 2][col - 2] == 0
				&& board[row + 3][col - 3] == 0)
				resultScore = resultScore - ONESTONE;

			if (board[row][col] == 0 && board[row + 1][col - 1] == enemy && board[row + 2][col - 2] == 0
				&& board[row + 3][col - 3] == 0)
				resultScore = resultScore - ONESTONE;

			if (board[row][col] == 0 && board[row + 1][col - 1] == 0 && board[row + 2][col - 2] == enemy
				&& board[row + 3][col - 3] == 0)
				resultScore = resultScore - ONESTONE;

			if (board[row][col] == 0 && board[row + 1][col - 1] == 0 && board[row + 2][col - 2] == 0
				&& board[row + 3][col - 3] == enemy)
				resultScore = resultScore - ONESTONE;

			if (board[row][col] == player && board[row + 1][col - 1] == 0 && board[row + 2][col - 2] == 0
				&& board[row + 3][col - 3] == 0)
				resultScore = resultScore + ONESTONE;

			if (board[row][col] == 0 && board[row + 1][col - 1] == player && board[row + 2][col - 2] == 0
				&& board[row + 3][col - 3] == 0)
				resultScore = resultScore + ONESTONE;

			if (board[row][col] == 0 && board[row + 1][col - 1] == 0 && board[row + 2][col - 2] == player
				&& board[row + 3][col - 3] == 0)
				resultScore = resultScore + ONESTONE;

			if (board[row][col] == 0 && board[row + 1][col - 1] == 0 && board[row + 2][col - 2] == 0
				&& board[row + 3][col - 3] == player)
				resultScore = resultScore + ONESTONE;

			// 돌 2개 놓인 것 체크
			if (board[row][col] == enemy && board[row + 1][col - 1] == enemy && board[row + 2][col - 2] == 0
				&& board[row + 3][col - 3] == 0)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == 0 && board[row + 1][col - 1] == enemy && board[row + 2][col - 2] == enemy
				&& board[row + 3][col - 3] == 0)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == 0 && board[row + 1][col - 1] == 0 && board[row + 2][col - 2] == enemy
				&& board[row + 3][col - 3] == enemy)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == enemy && board[row + 1][col - 1] == 0 && board[row + 2][col - 2] == enemy
				&& board[row + 3][col - 3] == 0)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == 0 && board[row + 1][col - 1] == enemy && board[row + 2][col - 2] == 0
				&& board[row + 3][col - 3] == enemy)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == enemy && board[row + 1][col - 1] == 0 && board[row + 2][col - 2] == 0
				&& board[row + 3][col - 3] == enemy)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == player && board[row + 1][col - 1] == player && board[row + 2][col - 2] == 0
				&& board[row + 3][col - 3] == 0)
				resultScore = resultScore + TWOSTONE;

			if (board[row][col] == 0 && board[row + 1][col - 1] == player && board[row + 2][col - 2] == player
				&& board[row + 3][col - 3] == 0)
				resultScore = resultScore + TWOSTONE;

			if (board[row][col] == 0 && board[row + 1][col - 1] == 0 && board[row + 2][col - 2] == player
				&& board[row + 3][col - 3] == player)
				resultScore = resultScore + TWOSTONE;

			if (board[row][col] == player && board[row + 1][col - 1] == 0 && board[row + 2][col - 2] == player
				&& board[row + 3][col - 3] == 0)
				resultScore = resultScore + TWOSTONE;

			if (board[row][col] == 0 && board[row + 1][col - 1] == player && board[row + 2][col - 2] == 0
				&& board[row + 3][col - 3] == player)
				resultScore = resultScore + TWOSTONE;

			if (board[row][col] == player && board[row + 1][col - 1] == 0 && board[row + 2][col - 2] == 0
				&& board[row + 3][col - 3] == player)
				resultScore = resultScore + TWOSTONE;

			// 돌 3개 놓인 것 체크
			if (board[row][col] == enemy && board[row + 1][col - 1] == enemy && board[row + 2][col - 2] == enemy
				&& board[row + 3][col - 3] == 0)
				resultScore = resultScore - THREESTONE;

			if (board[row][col] == enemy && board[row + 1][col - 1] == enemy && board[row + 2][col - 2] == 0
				&& board[row + 3][col - 3] == enemy)
				resultScore = resultScore - THREESTONE;

			if (board[row][col] == enemy && board[row + 1][col - 1] == 0 && board[row + 2][col - 2] == enemy
				&& board[row + 3][col - 3] == enemy)
				resultScore = resultScore - THREESTONE;

			if (board[row][col] == 0 && board[row + 1][col - 1] == enemy && board[row + 2][col - 2] == enemy
				&& board[row + 3][col - 3] == enemy)
				resultScore = resultScore - THREESTONE;

			if (board[row][col] == player && board[row + 1][col - 1] == player && board[row + 2][col - 2] == player
				&& board[row + 3][col - 3] == 0)
				resultScore = resultScore + THREESTONE;

			if (board[row][col] == player && board[row + 1][col - 1] == player && board[row + 2][col - 2] == 0
				&& board[row + 3][col - 3] == player)
				resultScore = resultScore + THREESTONE;

			if (board[row][col] == player && board[row + 1][col - 1] == 0 && board[row + 2][col - 2] == player
				&& board[row + 3][col - 3] == player)
				resultScore = resultScore + THREESTONE;

			if (board[row][col] == 0 && board[row + 1][col - 1] == player && board[row + 2][col - 2] == player
				&& board[row + 3][col - 3] == player)
				resultScore = resultScore + THREESTONE;

			if (board[row][col] == player && board[row + 1][col - 1] == player && board[row + 2][col - 2] == player
				&& board[row + 3][col - 3] == player)
				return pos_INF;

			if (board[row][col] == enemy && board[row + 1][col - 1] == enemy && board[row + 2][col - 2] == enemy
				&& board[row + 3][col - 3] == enemy)
				return neg_INF;

		}
	}

	return resultScore;
}

int CSetNode::checkDiaLRDScore(int player, int board[8][8])
{
	int resultScore = 0;
	int enemy = (player == 1) ? 2 : 1;

	for (int row = 0; row <= 4; row++) {
		for (int col = 0; col < 5; col++) {

			// 돌 1개 놓인 것 체크
			if (board[row][col] == enemy && board[row + 1][col + 1] == 0 && board[row + 2][col + 2] == 0
				&& board[row + 3][col + 3] == 0)
				resultScore = resultScore - ONESTONE;

			if (board[row][col] == 0 && board[row + 1][col + 1] == enemy && board[row + 2][col + 2] == 0
				&& board[row + 3][col + 3] == 0)
				resultScore = resultScore - ONESTONE;

			if (board[row][col] == 0 && board[row + 1][col + 1] == 0 && board[row + 2][col + 2] == enemy
				&& board[row + 3][col + 3] == 0)
				resultScore = resultScore - ONESTONE;

			if (board[row][col] == 0 && board[row + 1][col + 1] == 0 && board[row + 2][col + 2] == 0
				&& board[row + 3][col + 3] == enemy)
				resultScore = resultScore - ONESTONE;

			if (board[row][col] == player && board[row + 1][col + 1] == 0 && board[row + 2][col + 2] == 0
				&& board[row + 3][col + 3] == 0)
				resultScore = resultScore + ONESTONE;

			if (board[row][col] == 0 && board[row + 1][col + 1] == player && board[row + 2][col + 2] == 0
				&& board[row + 3][col + 3] == 0)
				resultScore = resultScore + ONESTONE;

			if (board[row][col] == 0 && board[row + 1][col + 1] == 0 && board[row + 2][col + 2] == player
				&& board[row + 3][col + 3] == 0)
				resultScore = resultScore + ONESTONE;

			if (board[row][col] == 0 && board[row + 1][col + 1] == 0 && board[row + 2][col + 2] == 0
				&& board[row + 3][col + 3] == player)
				resultScore = resultScore + ONESTONE;

			// 돌 2개 놓인 것 체크
			if (board[row][col] == enemy && board[row + 1][col + 1] == enemy && board[row + 2][col + 2] == 0
				&& board[row + 3][col + 3] == 0)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == 0 && board[row + 1][col + 1] == enemy && board[row + 2][col + 2] == enemy
				&& board[row + 3][col + 3] == 0)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == 0 && board[row + 1][col + 1] == 0 && board[row + 2][col + 2] == enemy
				&& board[row + 3][col + 3] == enemy)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == enemy && board[row + 1][col + 1] == 0 && board[row + 2][col + 2] == enemy
				&& board[row + 3][col + 3] == 0)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == 0 && board[row + 1][col + 1] == enemy && board[row + 2][col + 2] == 0
				&& board[row + 3][col + 3] == enemy)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == enemy && board[row + 1][col + 1] == 0 && board[row + 2][col + 2] == 0
				&& board[row + 3][col + 3] == enemy)
				resultScore = resultScore - TWOSTONE;

			if (board[row][col] == player && board[row + 1][col + 1] == player && board[row + 2][col + 2] == 0
				&& board[row + 3][col + 3] == 0)
				resultScore = resultScore + TWOSTONE;

			if (board[row][col] == 0 && board[row + 1][col + 1] == player && board[row + 2][col + 2] == player
				&& board[row + 3][col + 3] == 0)
				resultScore = resultScore + TWOSTONE;

			if (board[row][col] == 0 && board[row + 1][col + 1] == 0 && board[row + 2][col + 2] == player
				&& board[row + 3][col + 3] == player)
				resultScore = resultScore + TWOSTONE;

			if (board[row][col] == player && board[row + 1][col + 1] == 0 && board[row + 2][col + 2] == player
				&& board[row + 3][col + 3] == 0)
				resultScore = resultScore + TWOSTONE;

			if (board[row][col] == 0 && board[row + 1][col + 1] == player && board[row + 2][col + 2] == 0
				&& board[row + 3][col + 3] == player)
				resultScore = resultScore + TWOSTONE;

			if (board[row][col] == player && board[row + 1][col + 1] == 0 && board[row + 2][col + 2] == 0
				&& board[row + 3][col + 3] == player)
				resultScore = resultScore + TWOSTONE;

			// 돌 3개 놓인 것 체크
			if (board[row][col] == enemy && board[row + 1][col + 1] == enemy && board[row + 2][col + 2] == enemy
				&& board[row + 3][col + 3] == 0)
				resultScore = resultScore - THREESTONE;

			if (board[row][col] == enemy && board[row + 1][col + 1] == enemy && board[row + 2][col + 2] == 0
				&& board[row + 3][col + 3] == enemy)
				resultScore = resultScore - THREESTONE;

			if (board[row][col] == enemy && board[row + 1][col + 1] == 0 && board[row + 2][col + 2] == enemy
				&& board[row + 3][col + 3] == enemy)
				resultScore = resultScore - THREESTONE;

			if (board[row][col] == 0 && board[row + 1][col + 1] == enemy && board[row + 2][col + 2] == enemy
				&& board[row + 3][col + 3] == enemy)
				resultScore = resultScore - THREESTONE;

			if (board[row][col] == player && board[row + 1][col + 1] == player && board[row + 2][col + 2] == player
				&& board[row + 3][col + 3] == 0)
				resultScore = resultScore + THREESTONE;

			if (board[row][col] == player && board[row + 1][col + 1] == player && board[row + 2][col + 2] == 0
				&& board[row + 3][col + 3] == player)
				resultScore = resultScore + THREESTONE;

			if (board[row][col] == player && board[row + 1][col + 1] == 0 && board[row + 2][col + 2] == player
				&& board[row + 3][col + 3] == player)
				resultScore = resultScore + THREESTONE;

			if (board[row][col] == 0 && board[row + 1][col + 1] == player && board[row + 2][col + 2] == player
				&& board[row + 3][col + 3] == player)
				resultScore = resultScore + THREESTONE;

			if (board[row][col] == player && board[row + 1][col + 1] == player && board[row + 2][col + 2] == player
				&& board[row + 3][col + 3] == player)
				return pos_INF;

			if (board[row][col] == enemy && board[row + 1][col + 1] == enemy && board[row + 2][col + 2] == enemy
				&& board[row + 3][col + 3] == enemy)
				return neg_INF;

		}
	}

	return resultScore;
}

