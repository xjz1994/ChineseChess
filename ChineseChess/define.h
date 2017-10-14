#pragma once
#ifndef  define_h_
#define define_h_

#define BYTE unsigned char

#define NOCHESS       0
#define B_KING        1
#define B_CAR         2
#define B_HORSE       3
#define B_CANON       4
#define B_BISHOP      5
#define B_ELEPHANT    6
#define B_PAWN        7
#define B_BEGIN       B_KING
#define B_END         B_PAWN

#define R_KING        8
#define R_CAR         9
#define R_HORSE       10
#define R_CANON       11
#define R_BISHOP      12
#define R_ELEPHANT    13
#define R_PAWN        14
#define R_BEGIN       R_KING
#define R_END         R_PAWN

#define IsBlack(x)    (x>=B_BEGIN&&x<=B_END)
#define IsRed(x)      (x>=R_BEGIN&&x<=R_END)
#define IsSameSide(x,y) ((IsBlack(x)&&IsBlack(y))||(IsRed(x)&&IsRed(y)))

typedef struct _chessmanposition 
{
    BYTE x;
    BYTE y;
} CHESSMANPOS;

typedef struct _chessmove
{
    short ChessID;
    CHESSMANPOS From;
    CHESSMANPOS To;
    int Score;
} CHESSMOVE;

#endif // ! define_h_
