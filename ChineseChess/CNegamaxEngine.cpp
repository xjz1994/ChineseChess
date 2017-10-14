#include "stdafx.h"
#include "chess.h"
#include "CNegamaxEngine.h"

CNegamaxEngine::CNegamaxEngine() {}

CNegamaxEngine::~CNegamaxEngine() {}

//传入position，找出一步最佳走法，并且修改棋盘数据为走过后的状态
void CNegamaxEngine::SearchAGoodMove(BYTE position[10][9]) {
	//设定搜索层数为m_nSearchDepth
	m_nMaxDepth = m_nSearchDepth;
	//将传入的棋盘复制到成员变量CurPosition中
	memcpy(CurPosition, position, 90);
	//寻找最佳走法
	NegaMax(m_nMaxDepth);
	//修改棋盘
	MakeMove(&m_cmBestMove);
	//将修改过的棋盘复制的传入棋盘中
	memcpy(position, CurPosition, 90);
}

//负极大值搜索函数,depth表示节点离叶子节点的层数
int CNegamaxEngine::NegaMax(int depth) {
	int current = -20000;
	int score;
	int Count, i;
	BYTE type;
	i = IsGameOver(CurPosition, depth);//检查棋盘是否结束
	if (i != 0) {
		return i;//棋局结束，返回极大或极小值
	}
	if (depth <= 0) {//叶子节点取估值
		return m_pEval->Eveluate(CurPosition, (m_nMaxDepth - depth) % 2);//返回估值
	}
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, (m_nMaxDepth - depth) % 2);//列出当前局面下一步的所有可能走法
	for (i = 0; i < Count; i++) {
		//根据走法产生新局面
		type = MakeMove(&m_pMG->m_MoveList[depth][i]);
		//递归调用负极大值搜索下一层的节点
		score = -NegaMax(depth - 1);
		//恢复当前局面
		UnMakeMove(&m_pMG->m_MoveList[depth][i], type);
		if (score > current) {
			current = score;//修改当前最大值为score
			if (depth == m_nMaxDepth) {
				//靠近根部是保存最佳走法
				m_cmBestMove = m_pMG->m_MoveList[depth][i];
			}
		}
	}
	return current;
}