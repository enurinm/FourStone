#pragma once
#include<vector>
using namespace std;

class CMyNode
{
public:
	int player;
	int updatedCol;
	int score;
	int map[8][8];
	vector<CMyNode> children;
	CMyNode();
	CMyNode(int player, int map[8][8]);
	CMyNode(int player,int updatedCol, int map[8][8]);

	vector<CMyNode> GenChild();

	~CMyNode();
	
};

