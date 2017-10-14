#pragma once

//定义每种棋子的价值
#define BASEVELUE_PAWN 100;
#define BASEVELUE_BISHOP 250;
#define BASEVELUE_ELEPHANT 250;
#define BASEVELUE_CAR 500;
#define BASEVELUE_HORSE 350;
#define BASEVELUE_CANON 350;
#define BASEVELUE_KING 10000;
//定义棋子灵活性
#define FLEXIBILITY_PAWN 15;
#define FLEXIBILITY_BISHOP 1;
#define FLEXIBILITY_ELEPHANT 1;
#define FLEXIBILITY_CAR 6;
#define FLEXIBILITY_HORSE 12;
#define FLEXIBILITY_CANON 6;
#define FLEXIBILITY_KING 0;

class CEveluation
{
public:
	CEveluation();
	~CEveluation();
	//估值函数，对传统的棋盘打分，BIsRedTurn表示轮到谁走棋
	virtual int Eveluate(BYTE position[10][9], bool bIsRenTurn);

protected:
	//列举与指定位置的棋子相关的棋子
	int GetRelatePiece(BYTE position[10][9], int j, int i);
	//判断位置From的棋子是否能走到位置To
	bool CanTouch(BYTE position[10][9], int nFromX, int nFromY, int nToX, int nToY);
	//将一个位置加入相关位置的队列
	void AddPoint(int x, int y);
	int m_BaseValue[15];//存放棋子基本价值
	int m_FlexValue[15];//存放棋子灵活性
	short m_AttackPos[10][9];//存放每一个位置被威胁的信息
	BYTE m_GuardPos[10][9];//存放每一位置被保护的信息
	BYTE m_FlexBilityPos[10][9];//存放每一位置上棋子的灵活性分数
	int m_chessValue[10][9];//存放每一位置上棋子的总价值
	int nPosCount;//记录一棋子的相关位置个数
	CHESSMANPOS RelatePos[20];//记录一个棋子相关位置的数组
};
