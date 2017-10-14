#pragma once
#include "chess.h"

class CMoveGenerator 
{
public:
    CMoveGenerator();
    virtual ~CMoveGenerator();

    //检测走法是否合法
    static bool IsValidMove(BYTE position[10][9], int nFromX, int nToX, int nFromY, int nToY);

    //产生给点棋盘上的所有合法的走法
    int CreatePossibleMove(BYTE position[10][9], int nPly, int nSide);

    //存放CreatePossibleMove产生的所有走法队列
    CHESSMOVE m_MoveList[8][80];

protected:
    //在m_MoveList中插入一个走法
    int AddMove(int nFromX, int nToX, int nFromY, int nToY, int nPly);
    //产生将、帅的走法
    void Gen_KingMove(BYTE position[10][9], int i, int j, int nPly);
    //产生红士的走法
    void Gen_RBishopMove(BYTE position[10][9], int i, int j, int nPly);
    //产生黑士的走法
    void Gen_BBishopMove(BYTE position[10][9], int i, int j, int nPly);
    //产生象的走法
    void Gen_ElephantMove(BYTE position[10][9], int i, int j, int nPly);
    //产生马的走法
    void Gen_HorseMove(BYTE position[10][9], int i, int j, int nPly);
    //产生車的走法
    void Gen_CarMove(BYTE position[10][9], int i, int j, int nPly);
    //产生红卒的走法
    void Gen_RPawnMove(BYTE position[10][9], int i, int j, int nPly);
    //产生黑兵的走法
    void Gen_BPawnMove(BYTE position[10][9], int i, int j, int nPly);
    //产生炮的走法
    void Gen_CanonMove(BYTE position[10][9], int i, int j, int nPly);
    //记录m_moveList中走法的数量
    int m_nMoveCount;
};