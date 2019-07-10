

#include "CMyNode.h"
#include"CSetNode.h"
using namespace std;
CMyNode::CMyNode()
{
}

CMyNode::CMyNode(int player, int map[8][8]):player(player),updatedCol(-1)
{
	this->score = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			this->map[i][j] = map[i][j];
		}
	}
}

CMyNode::CMyNode(int player, int updatedCol, int  map[8][8]):player(player),updatedCol(updatedCol)
{

	this->score = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			this->map[i][j] = map[i][j];
		}
	}
}

vector<CMyNode> CMyNode::GenChild()
{
	for (int i = 0; i < 8; i++)
	{
		if (CSetNode::CanPutInCol(map, i))
		{
			int copyMap[8][8] = { 0 };
			int temp[8][8];
			int switchedPlayer;
			switchedPlayer = player % 2 + 1;
			CSetNode::CopyMap(copyMap, map);

			CSetNode::PutInCol(copyMap, switchedPlayer, i);
			CSetNode::CopyMap(temp, copyMap);
			if (temp != nullptr) {
				CMyNode child(switchedPlayer, i, temp);
				children.push_back(child);
			}
		}
	}
	return children;
}

CMyNode::~CMyNode()
{
}

