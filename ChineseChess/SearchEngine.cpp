#include "stdafx.h"
#include "chess.h"
#include "SearchEngine.h"

CSearchEngine::CSearchEngine() {}

CSearchEngine::~CSearchEngine() {
	delete m_pMG;//删除走法生产器
	delete m_pEval;//删除估值引擎
}
//根据传入的走法，改变棋盘
BYTE CSearchEngine::MakeMove(CHESSMOVE * move) {
	BYTE nChessID;
	nChessID = CurPosition[move->To.y][move->To.x];//取目标位置棋子
	CurPosition[move->To.y][move->To.x] = CurPosition[move->From.y][move->From.x];//把棋子移动到目标位置
	CurPosition[move->From.y][move->From.x] = NOCHESS;//清空原来的位置
	return nChessID;//返回被吃掉的棋子
}
//根据传入的走法，回复棋盘
void CSearchEngine::UnMakeMove(CHESSMOVE * move,BYTE nChessID) {
	CurPosition[move->From.y][move->From.x] = CurPosition[move->To.y][move->To.x];//把目标位置和棋子拷贝回到原位置
	CurPosition[move->To.y][move->To.x] = nChessID;//回复目标位置原来的棋子
}
//检测游戏是否已经结束,若结束，返回极大值或极小值，若未结束，返回0
int CSearchEngine::IsGameOver(BYTE position[10][9], int nDepth) {
	int i, j;
	bool redLive = false, blackLive = false;
	//检测红黑双方九宫格内是否有将帅
	for (i = 7; i < 10; i++) {
		for (j = 3; j < 6; j++) {
			if (position[i][j] == B_KING) {
				blackLive = true;
			}
			if (position[i][j] == R_KING) {
				redLive = true;
			}
		}
	}
	for (i = 0; i < 3; i++) {
		for (j = 3; j < 6; j++) {
			if (position[i][j] == B_KING) {
				blackLive = true;
			}
			if (position[i][j] == R_KING) {
				redLive = true;
			}
		}
	}
	i = (m_nMaxDepth - nDepth - 1) % 2;//计算当前层数是否奇偶
	if (!redLive) {//红将不在了
		if (i) {
			return 19990 + nDepth;//偶数层，返回极大值
		}else {
			return -19990 - nDepth;//奇数层，返回极小值
		}
	}
	if (!blackLive) {//黑帅不在了
		if (i) {
			return -19990 - nDepth;//偶数层，返回极小值
		}else {
			return 19990 + nDepth;//奇数层，返回极大值
		}
	}
	return 0;
}