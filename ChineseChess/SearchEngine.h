#pragma once
#include "MoveGenerator.h"
#include "Eveluation.h"

class CSearchEngine {

public:
	CSearchEngine();
	virtual ~CSearchEngine();
	//博弈接口，为当前局面走出下一步
	virtual void SearchAGoodMove(BYTE position[10][9]) {};
	//设定最大搜索深度
	void SetSearchDeepth(int nDepth) { m_nSearchDepth = nDepth; };
	//设定估值引擎
	void SetEveluator(CEveluation * pEval) { m_pEval = pEval; };
	//设定走法产生器
	void SetMoveGenerator(CMoveGenerator *pMG) { m_pMG = pMG; };
protected:
	//根据某一走法产生走了之后的棋盘
	BYTE MakeMove(CHESSMOVE * move);
	//恢复某一走法所产生棋盘为走过之前的
	void UnMakeMove(CHESSMOVE *move, BYTE type);
	//是否已分出胜负 
	int IsGameOver(BYTE position[10][9], int nDepth);
	//搜索时用于当前节点棋盘状态的数组
	BYTE CurPosition[10][9];
	//记录最佳走法的变量
	CHESSMOVE m_cmBestMove;
	//走法生产器指针
	CMoveGenerator *m_pMG;
	//估值引擎指针
	CEveluation *m_pEval;
	//最大搜索深度
	int m_nSearchDepth;
	//当前搜索的大搜索深度
	int m_nMaxDepth;
};