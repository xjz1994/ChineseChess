#include "stdafx.h"
#include "math.h"
#include "MoveGenerator.h"

#ifdef  _DEGUG
#undef THIS_FILE
static BYTE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif //  _DEGUG

CMoveGenerator::CMoveGenerator() {}

CMoveGenerator::~CMoveGenerator() {}

//判断局面 position上，从From 到To的走法是否合法，返回bool
bool CMoveGenerator::IsValidMove(BYTE position[10][9], int nFromX, int nFromY, int nToX, int nToY) {
    int i, j;
    int nMoveChessID, nTargetID;
    if (nFromX == nToX&&nFromY == nToY) {//起始坐标与目的坐标相同
        return false;
    }
    nMoveChessID = position[nFromX][nFromY];
    nTargetID = position[nToX][nToY];
    if (IsSameSide(nMoveChessID, nTargetID)) {//目标位置有己方棋子
        return false;
    }
    switch (nMoveChessID) {
        case B_KING:
            if (nTargetID == R_KING) {
                if (nFromX != nToX) {//判断将的X坐标是否相同
                    return false;
                }
                for (i = nFromY + 1; i < nToY; ++i) {
                    if (position[i][nFromX] != NOCHESS) {//判断将之间是否隔着棋子
                        return false;
                    }
                }
            }else {
                if (nToY > 2 || nToX > 5 || nToX < 3) {//判断是否在九宫格
                    return false;
                }
                if (abs(nFromY - nToY) + abs(nToX - nFromX) > 1) {//判断在九宫格内是否只走一步
                    return false;
                }
            }
            break;
        case R_KING:
            if (nTargetID == B_KING) {
                if (nFromX != nToX) {//判断将的X坐标是否相同
                    return false;
                }
                for (i = nFromY - 1; i > nToY; --i) {
                    if (position[i][nFromX] != NOCHESS) {//判断将之间是否隔着棋子
                        return false;
                    }
                }
            }else {
                if (nToY < 7 || nToX>5 || nToX < 3) {//判断是否在九宫格
                    return false;
                }
                if (abs(nFromY - nToY) + abs(nToX - nFromX) > 1) {//判断在九宫格内是否只走一步
                    return false;
                }
            }
            break;
        case B_BISHOP:
            if (nToY > 2 || nToX > 5 || nToX < 3) {//判断是否在九宫格
                return false;
            }
            if (abs(nFromY - nToY) != 1 || abs(nToX - nFromX) != 1) {//士只能走斜线
                return false;
            }
            break;
        case R_BISHOP:
            if (nToY < 7 || nToX > 5 || nToY < 3) {//判断是否在九宫格
                return false;
            }
            if (abs(nFromY - nToY) != 1 || abs(nToX - nFromX) != 1) {//士只能走斜线
                return false;
            }
            break;
        case B_ELEPHANT:
            if (nToY < 5) {//象不能过河
                return false;
            }
            if (abs(nFromY - nToY) != 2 || abs(nToX - nFromX) != 2) {//象只能走田字
                return false;
            }
            if (position[(nFromY + nToY) / 2][(nFromX + nToX) / 2]!=NOCHESS) {//象眼不能有棋子
                return false;
            }
            break;
        case R_ELEPHANT:
            if (nToY > 4) {//象不能过河
                return false;
            }
            if (abs(nFromY - nToY) != 2 || abs(nToX - nFromX) != 2) {//象只能走田字
                return false;
            }
            if (position[(nFromY + nToY) / 2][(nFromX + nToX) / 2] != NOCHESS) {//象眼不能有棋子
                return false;
            }
            break;
        case B_PAWN:
            if (nToY < nFromY) {//兵不能回头
                return false;
            }
            if (nToY < 5 && nFromY == nToY) {//过河前只能只走
                return false;
            }
            if (nToY - nFromY + abs(nToX - nFromX) > 1) {//只能走一步
                return false;
            }
            break;
        case R_PAWN:
            if (nToY > nFromY) {//兵不能回头
                return false;
            }
            if (nToY >4 && nFromY == nToY) {//过河前只能只走
                return false;
            }
            if (nFromY - nToY + abs(nToX - nFromX) > 1) {//只能走一步
                return false;
            }
            break;
        case B_CAR:
		case R_CAR:
            if (nFromY != nToY && nFromX != nToX) {//車只能走直线
                return false;
            }
            if (nFromY == nToY) {//横走
                if (nFromX < nToX) {//往右走，路径上不能有棋子
                    for (i = nFromX + 1; i < nToX; ++i) {
                        if (position[nFromY][i] != NOCHESS) {
                            return false;
                        }
                    }
                }else {//往左走，路径上不能有棋子
                    for (i = nToX + 1; i<nFromX ; i++) {
						if (position[nFromY][i] != NOCHESS) {
							return false;
						}
                    }
                }
            }else {//竖走
				if (nFromY < nToY) {//往下走
					for (j = nFromY + 1; j < nToY; j++) {
						if (position[j][nFromX] != NOCHESS) {
							return false;
						}
					}
				}else {//往上走
					for (j = nToY + 1; j < nFromY; j++) {
						if (position[j][nFromX] != NOCHESS) {
							return false;
						}
					}
				}
            }
            break;
		case B_HORSE:
		case R_HORSE:
			if (!((abs(nToX - nFromX) == 1 && abs(nToY - nFromY) == 2) || (abs(nToX - nFromX) == 2 && abs(nToY - nFromY) == 1))) {//马走日字
				return false;
			}
			if (nToX - nFromX == 2) {
				i = nFromX + 1;
				j = nFromY;
			} else if (nFromX - nToX == 2) {
				i = nFromX-1;
				j = nFromY;
			}else if (nToY - nFromY == 2) {
				i = nFromX;
				j = nFromY + 1;
			}else if (nFromY - nToY == 2) {
				i = nFromX;
				j = nFromY - 1;
			}
			if (position[j][i] != NOCHESS) {
				return false;
			}
			break;
		case B_CANON:
		case R_CANON:
			if (nFromY != nToY && nFromX != nToX) {//炮只能走直线
				return false;
			}
			if (position[nToY][nToX] == NOCHESS) {//炮不吃子时，中间不能有子
				if (nFromY == nToY) {//横走
					if (nFromX < nToX) {//往右走，路径上不能有棋子
						for (i = nFromX + 1; i < nToX; ++i) {
							if (position[nFromY][i] != NOCHESS) {
								return false;
							}
						}
					}
					else {//往左走，路径上不能有棋子
						for (i = nToX + 1; i<nFromX; i++) {
							if (position[nFromY][i] != NOCHESS) {
								return false;
							}
						}
					}
				}
				else {//竖走
					if (nFromY < nToY) {//往下走
						for (j = nFromY + 1; j < nToY; j++) {
							if (position[j][nFromX] != NOCHESS) {
								return false;
							}
						}
					}
					else {//往上走
						for (j = nToY + 1; j < nFromY; j++) {
							if (position[j][nFromX] != NOCHESS) {
								return false;
							}
						}
					}
				}
			}else {//炮吃子时，中间要有子
				int count = 0;
				if (nFromY == nToY) {//横走
					if (nFromX < nToX) {//往右走
						for (i = nFromX + 1; i < nToX; ++i) {
							if (position[nFromY][i] != NOCHESS) {
								count++;
							}
						}
						if (count != 1) {
							return false;
						}
					}
					else {//往左走
						for (i = nToX + 1; i<nFromX; i++) {
							if (position[nFromY][i] != NOCHESS) {
								count++;
							}
						}
						if (count != 1) {
							return false;
						}
					}
				}
				else {//竖走
					if (nFromY < nToY) {//往下走
						for (j = nFromY + 1; j < nToY; j++) {
							if (position[j][nFromX] != NOCHESS) {
								count++;
							}
						}
						if (count != 1) {
							return false;
						}
					}
					else {//往上走
						for (j = nToY + 1; j < nFromY; j++) {
							if (position[j][nFromX] != NOCHESS) {
								count++;
							}
						}
						if (count != 1) {
							return false;
						}
					}
				}
			}
			break;
		default:
			return false;
    }
	return true;
}

//在m_MoveList中插入一个走法，nPly是此走法所在的层次
int CMoveGenerator::AddMove(int nFromX,int nFromY,int nToX,int nToY,int nPly) {
	m_MoveList[nPly][m_nMoveCount].From.x = nFromX;
	m_MoveList[nPly][m_nMoveCount].From.y = nFromY;
	m_MoveList[nPly][m_nMoveCount].To.x = nToX;
	m_MoveList[nPly][m_nMoveCount].To.y = nToY;
	m_nMoveCount++;
	return m_nMoveCount;
}

//产生当前局面position中所有可能的走法
//nPly表示当前搜索的层数，每层将走法存在不同的位置，以免覆盖
//nSide代表哪一方的走法，true为红方，false为黑方
int CMoveGenerator::CreatePossibleMove(BYTE position[10][9], int nPly, int nSide) {
	int x, y, nChessID;
	bool flag;
	int i, j;
	m_nMoveCount = 0;
	for (j = 0; j < 9; j++) {//col
		for (i = 0; i < 10; i++) {//row
			if (position[j][i] != NOCHESS) {
				nChessID = position[j][i];
				if (!nSide&&IsRed(nChessID)) {
					continue;
				}
				if (nSide&&IsBlack(nChessID)) {
					continue;
				}
				switch (nChessID){
					case R_KING:
					case B_KING:
						Gen_KingMove(position, i, j,nPly);
						break;
					case R_BISHOP:
						Gen_RBishopMove(position, i, j, nPly);
						break;
					case B_BISHOP:
						Gen_BBishopMove(position, i, j, nPly);
						break;
					case R_ELEPHANT:
					case B_ELEPHANT:
						Gen_ElephantMove(position, i, j, nPly);
						break;
					case R_HORSE:
					case B_HORSE:
						Gen_HorseMove(position, i, j, nPly);
						break;
					case R_CAR:
					case B_CAR:
						Gen_CarMove(position, i, j, nPly);
						break;
					case R_PAWN:
						Gen_RPawnMove(position, i, j, nPly);
						break;
					case B_PAWN:
						Gen_BPawnMove(position, i, j, nPly);
						break;
					case B_CANON:
					case R_CANON:
						Gen_CanonMove(position, i, j, nPly);
						break;
					default:
						break;
				}
			}
		}
	}
	return m_nMoveCount;
}

//生产将的合法走法，nPly表示插入到List第几层
void CMoveGenerator::Gen_KingMove(BYTE position[10][9],int i,int j,int nPly) {
	int x, y;
	for (y = 0; y < 3; y++) {
		for (x = 3; x < 6; x++) {
			if (IsValidMove(position, j, i, x, y)) {
				AddMove(j,i,x,y,nPly);
			}
		}
	}
	for (y = 7; y < 10; y++) {
		for (x = 3; x < 6; x++) {
			if (IsValidMove(position, j, i, x, y)) {
				AddMove(j, i, x, y, nPly);
			}
		}
	}
}
//生产红士的合法走法，nPly表示插入到List第几层
void CMoveGenerator::Gen_RBishopMove(BYTE position[10][9], int i, int j, int nPly) {
	int x, y;
	for (y = 7; y < 10; y++) {
		for (x = 3; x < 6; x++) {
			if (IsValidMove(position, j, i, x, y)) {
				AddMove(j, i, x, y, nPly);
			}
		}
	}
}
//生产黑士的合法走法，nPly表示插入到List第几层
void CMoveGenerator::Gen_BBishopMove(BYTE position[10][9], int i, int j, int nPly) {
	int x, y;
	for (y = 0; y < 3; y++) {
		for (x = 3; x < 6; x++) {
			if (IsValidMove(position, j, i, x, y)) {
				AddMove(j, i, x, y, nPly);
			}
		}
	}
}
//生产象的合法走法，nPly表示插入到List第几层
void CMoveGenerator::Gen_ElephantMove(BYTE position[10][9], int i, int j, int nPly) {
	int x, y;
	x = j + 2;
	y = i + 2;
	if (x < 9 && y < 10 && IsValidMove(position, j, i, x, y)) {
		AddMove(j, i, x, y, nPly);
	}
	x = j + 2;
	y = i - 2;
	if (x < 9 && y >= 0 && IsValidMove(position, j, i, x, y)) {
		AddMove(j, i, x, y, nPly);
	}
	x = j - 2;
	y = i - 2;
	if (x >= 0 && y >= 0 && IsValidMove(position, j, i, x, y)) {
		AddMove(j, i, x, y, nPly);
	}
	x = j - 2;
	y = i + 2;
	if (x >= 0 && y < 10 && IsValidMove(position, j, i, x, y)) {
		AddMove(j, i, x, y, nPly);
	}
}
//生产马的合法走法，nPly表示插入到List第几层
void CMoveGenerator::Gen_HorseMove(BYTE position[10][9], int i, int j, int nPly) {
	int x, y;
	x = j + 2;
	y = i + 1;
	if (x < 9 && y < 10 && IsValidMove(position, j, i, x, y)) {
		AddMove(j, i, x, y, nPly);
	}
	x = j + 1;
	y = i + 2;
	if (x < 9 && y < 10 && IsValidMove(position, j, i, x, y)) {
		AddMove(j, i, x, y, nPly);
	}
	x = j - 2;
	y = i + 1;
	if (x >= 0 && y < 10 && IsValidMove(position, j, i, x, y)) {
		AddMove(j, i, x, y, nPly);
	}
	x = j - 1;
	y = i + 2;
	if (x >= 0 && y < 10 && IsValidMove(position, j, i, x, y)) {
		AddMove(j, i, x, y, nPly);
	}
	x = j - 2;
	y = i - 1;
	if (x >= 0 && y >= 0 && IsValidMove(position, j, i, x, y)) {
		AddMove(j, i, x, y, nPly);
	}
	x = j - 1;
	y = i - 2;
	if (x >= 0 && y >= 0 && IsValidMove(position, j, i, x, y)) {
		AddMove(j, i, x, y, nPly);
	}
	x = j + 2;
	y = i - 1;
	if (x < 9 && y >= 0 && IsValidMove(position, j, i, x, y)) {
		AddMove(j, i, x, y, nPly);
	}
	x = j + 1;
	y = i - 2;
	if (x < 9 && y >= 0 && IsValidMove(position, j, i, x, y)) {
		AddMove(j, i, x, y, nPly);
	}
}
//生产红卒的合法走法，nPly表示插入到List第几层
void CMoveGenerator::Gen_RPawnMove(BYTE position[10][9], int i, int j, int nPly) {
	int x, y;
	int nChessID = position[i][j];
	y = i - 1;//向前
	x = j;
	if (y > 0 && !IsSameSide(nChessID, position[y][x])) {
		AddMove(j, i, x, y, nPly);
	}
	if (i < 5) {//是否已经过河
		y = i;
		x = j + 1;
		if (x < 9 && !IsSameSide(nChessID, position[y][x])) {//向右
			AddMove(j, i, x, y, nPly);
		}
		y = i;
		x - j - 1;
		if (x >= 0 && !IsSameSide(nChessID, position[y][x])) {//向左
			AddMove(j, i, x, y, nPly);
		}
	}
}
//产生黑兵的合法走法
void CMoveGenerator::Gen_BPawnMove(BYTE position[10][9], int i, int j, int nPly) {
	int x, y;
	int nChessID = position[i][j];
	y = i + 1;
	x = j;
	if (y < 10 && !IsSameSide(nChessID, position[y][x])) {
		AddMove(j, i, x, y, nPly);
	}
	if (i > 4) {//是否已经过河
		y = i;
		x = j + 1;
		if (x < 9 && !IsSameSide(nChessID, position[y][x])) {//向右
			AddMove(j, i, x, y, nPly);
		}
		y = i;
		x - j - 1;
		if (x >= 0 && !IsSameSide(nChessID, position[y][x])) {//向左
			AddMove(j, i, x, y, nPly);
		}
	}
}
//产生車的合法走法
void CMoveGenerator::Gen_CarMove(BYTE position[10][9], int i, int j, int nPly) {
	int x, y;
	int nChessID = position[i][j];
	x = j + 1;
	y = i;
	for (; x < 9; x++) {
		if (position[y][x] == NOCHESS) {
			AddMove(j, i, x, y, nPly);
		}else {
			if (!IsSameSide(nChessID, position[y][x])) {
				AddMove(j, i, x, y, nPly);
			}
			break;
		}
	}
	x = j - 1;
	y = i;
	for (; x >= 0; x--) {
		if (position[y][x] == NOCHESS) {
			AddMove(j, i, x, y, nPly);
		}else {
			if (!IsSameSide(nChessID, position[y][x])) {
				AddMove(j, i, x, y, nPly);
			}
			break;
		}
	}
	x = j;
	y = i + 1;
	for (; y < 10; y++) {
		if (position[y][x] == NOCHESS) {
			AddMove(j, i, x, y, nPly);
		}else {
			if (!IsSameSide(nChessID, position[y][x])) {
				AddMove(j, i, x, y, nPly);
			}
			break;
		}
	}
	x = j;
	y = i - 1;
	for (; y >= 0; y--) {
		if (position[y][x] == NOCHESS) {
			AddMove(j, i, x, y, nPly);
		}
		else {
			if (!IsSameSide(nChessID, position[y][x])) {
				AddMove(j, i, x, y, nPly);
			}
			break;
		}
	}
}
//产生炮的合法走法
void CMoveGenerator::Gen_CanonMove(BYTE position[10][9], int i, int j, int nPly) {
	int x, y;
	bool flag;
	int nChessID = position[i][j];
	//插入右方可走位置
	x = j + 1;
	y = i;
	flag = false;
	for (; x < 9; x++) {
		if (position[y][x] == NOCHESS) {
			if (!flag) {//是否隔有棋子
				AddMove(j, i, x, y, nPly);//没有隔着棋子，插入可走位置
			}
		}else {
			if (!flag){//没有隔着棋子，此棋子为第一个障碍，设置标志
				flag = true;
			}else {//隔有棋子,如果此处为敌方棋子就可以走
				if (!IsSameSide(nChessID, position[y][x])) {
					AddMove(j, i, x, y, nPly);
					break;
				}
			}
		}
	}
	//插入左方可走位置
	x = j - 1;
	y = i;
	flag = false;
	for (; x >= 0; x--) {
		if (position[y][x] == NOCHESS) {
			if (!flag) {
				AddMove(j, i, x, y, nPly);
			}
		}else {
			if (!flag) {
				flag = true;
			}else{
				if (!IsSameSide(nChessID, position[y][x])) {
					AddMove(j, i, x, y, nPly);
					break;
				}
			}
		}
	}
	//插入下方可走位置
	x = j;
	y = i + 1;
	flag = false;
	for (; y < 10; y++) {
		if (position[y][x] == NOCHESS) {
			if (!flag) {
				AddMove(j, i, x, y, nPly);
			}
		}
		else {
			if (!flag) {
				flag = true;
			}
			else {
				if (!IsSameSide(nChessID, position[y][x])) {
					AddMove(j, i, x, y, nPly);
					break;
				}
			}
		}
	}
	//插入上方可走位置
	x = j;
	y = i - 1;
	for (; y >= 0; y--) {
		if (position[y][x] == NOCHESS) {
			if (!flag) {
				AddMove(j, i, x, y, nPly);
			}
		}
		else {
			if (!flag) {
				flag = true;
			}
			else {
				if (!IsSameSide(nChessID, position[y][x])) {
					AddMove(j, i, x, y, nPly);
					break;
				}
			}
		}
	}
}
