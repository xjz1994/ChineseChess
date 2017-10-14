#include "stdafx.h"
#include "math.h"
#include "chess.h"
#include "Eveluation.h"


const int BA0[10][9] =
{
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{90,90,110,120,120,120,110,90,90},
	{90,90,110,120,120,120,110,90,90},
	{70,90,110,110,110,110,110,90,70},
	{70,70,70,70,70,70,70,70,70},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const int BA1[10][9] = 
{
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 70,70,70,70,70,70,70,70,70 },
	{ 70,90,110,110,110,110,110,90,70 },
	{ 90,90,110,120,120,120,110,90,90 },
	{ 90,90,110,120,120,120,110,90,90 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

int GetPawnValue(int x, int y, BYTE CurSiuation[10][9]) {

	if (CurSiuation[y][x] == R_PAWN) {
		return BA0[y][x];
	}

	if (CurSiuation[y][x] == B_PAWN) {
		return BA1[y][x];
	}
	return 0;
}

CEveluation::CEveluation() {

	m_BaseValue[B_KING] = BASEVELUE_KING;
	m_BaseValue[B_CAR] = BASEVELUE_CAR;
	m_BaseValue[B_HORSE] = BASEVELUE_HORSE;
	m_BaseValue[B_BISHOP] = BASEVELUE_BISHOP;
	m_BaseValue[B_ELEPHANT] = BASEVELUE_ELEPHANT;
	m_BaseValue[B_CANON] = BASEVELUE_CANON;
	m_BaseValue[B_PAWN] = BASEVELUE_PAWN;
	m_BaseValue[R_KING] = BASEVELUE_KING;
	m_BaseValue[R_CAR] = BASEVELUE_CAR;
	m_BaseValue[R_HORSE] = BASEVELUE_HORSE;
	m_BaseValue[R_BISHOP] = BASEVELUE_BISHOP;
	m_BaseValue[R_ELEPHANT] = BASEVELUE_ELEPHANT;
	m_BaseValue[R_CANON] = BASEVELUE_CANON;
	m_BaseValue[R_PAWN] = BASEVELUE_PAWN;

	m_FlexValue[B_KING] = FLEXIBILITY_KING;
	m_FlexValue[B_CAR] = FLEXIBILITY_CAR;
	m_FlexValue[B_HORSE] = FLEXIBILITY_HORSE;
	m_FlexValue[B_BISHOP] = FLEXIBILITY_BISHOP;
	m_FlexValue[B_ELEPHANT] = FLEXIBILITY_ELEPHANT;
	m_FlexValue[B_CANON] = FLEXIBILITY_CANON;
	m_FlexValue[B_PAWN] = FLEXIBILITY_PAWN;
	m_FlexValue[R_KING] = FLEXIBILITY_KING;
	m_FlexValue[R_CAR] = FLEXIBILITY_CAR;
	m_FlexValue[R_HORSE] = FLEXIBILITY_HORSE;
	m_FlexValue[R_BISHOP] = FLEXIBILITY_BISHOP;
	m_FlexValue[R_ELEPHANT] = FLEXIBILITY_ELEPHANT;
	m_FlexValue[R_CANON] = FLEXIBILITY_CANON;
	m_FlexValue[R_PAWN] = FLEXIBILITY_PAWN;
}
CEveluation::~CEveluation() {}

int count = 0;


int CEveluation::Eveluate(BYTE position[10][9], bool bIsRedTurn) {
	int i, j, k;
	int nChessType, nTargetType;
	count++;
	memset(m_chessValue, 0, 360);
	memset(m_AttackPos, 0, 180);
	memset(m_GuardPos, 0, 90);
	memset(m_FlexBilityPos, 0, 90);

	for (i = 0; i < 10; i++) {
		for (j = 0; j < 9; j++) {
			if (position[i][j] != NOCHESS) {
				nChessType = position[i][j];
				GetRelatePiece(position, j, i);
				for (k = 0; k < nPosCount; k++) {
					nTargetType = position[RelatePos[k].y][RelatePos[k].x];
					if (nTargetType == NOCHESS) {
						m_FlexBilityPos[i][j]++;
					}
					else {
						if (IsSameSide(nChessType, nTargetType)) {
							m_GuardPos[RelatePos[k].y][RelatePos[k].x]++;
						}
						else {
							m_AttackPos[RelatePos[k].y][RelatePos[k].x]++;;
							m_FlexBilityPos[i][j]++;
							switch (nTargetType) {
							case R_KING:
								if (!bIsRedTurn) {
									return 18888;
								}
								break;
							case B_KING:
								if (bIsRedTurn) {
									return -18888;
								}
							default:
								
								m_AttackPos[RelatePos[k].y][RelatePos[k].x] += (30 + (m_BaseValue[nTargetType] - m_BaseValue[nChessType]) / 10) / 10;
								break;
							}
						}
					}
				}
			}
		}
	}

	for (i = 0; i < 10; i++) {
		for (j = 0; j < 9; j++) {
			if (position[i][j] != NOCHESS) {
				nChessType = position[i][j];
				m_chessValue[i][j]++;

				m_chessValue[i][j] += m_FlexValue[nChessType] * m_FlexBilityPos[i][j];

				m_chessValue[i][j] += GetPawnValue(j, i, position);
			}
		}
	}

	int nHalfvalue;
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 9; j++) {
			if (position[i][j] != NOCHESS) {
				nChessType = position[i][j];
				nHalfvalue = m_BaseValue[nChessType] / 16;
				m_chessValue[i][j] += m_BaseValue[nChessType];
				if (IsRed(nChessType)) {
					if (m_AttackPos[i][j]) {
						if (bIsRedTurn) {
							if (nChessType == R_KING) {
								m_chessValue[i][j] -= 20;
							}else {
								m_chessValue[i][j] -= nHalfvalue * 2;
								if (m_GuardPos[i][j]) {
									m_chessValue[i][j] += nHalfvalue;
								}
							}
						}else {
							if (nChessType == R_KING) {
								return 18888;
							}
							m_chessValue[i][j] -= nHalfvalue * 10;
							if (m_GuardPos[i][j]) {
								m_chessValue[i][j] += nHalfvalue * 9;
							}
						}
						m_chessValue[i][j] -= m_AttackPos[i][j];
					}else {
						if (m_GuardPos[i][j]) {
							m_chessValue[i][j] += 5;
						}
					}
				}else {
					if (m_AttackPos[i][j]) {
						if (!bIsRedTurn) {
							if (nChessType == B_KING) {
								m_chessValue[i][j] -= 20;
							}else {
								m_chessValue[i][j] -= nHalfvalue * 2;
								if (m_GuardPos) {
									m_chessValue[i][j] += nHalfvalue;
								}
							}
						}else {
							if (nChessType == B_KING) {
								return 18888;
							}
							m_chessValue[i][j] -= nHalfvalue * 10;
							if (m_GuardPos[i][j]) {
								m_chessValue[i][j] += nHalfvalue * 9;
							}
						}
						m_chessValue[i][j] -= m_AttackPos[i][j];
					}
					else {
						if (m_GuardPos[i][j]) {
							m_chessValue[i][j] += 5;
						}
					}
				}
			}
		}
	}

	int nRedValue = 0, nBlackValue = 0;
	for (i = 0; i < 10; i++) {
		for (i = 0; i < 9; i++) {
			nChessType = position[i][j];
			if (nChessType != NOCHESS) {
				if (IsRed(nChessType)) {
					nRedValue += m_chessValue[i][j];
				}else {
					nBlackValue += m_chessValue[i][j];
				}
			}
		}
	}
	if (bIsRedTurn) {
		return nRedValue - nBlackValue;
	}else {
		return nBlackValue - nRedValue;
	}
}

void CEveluation::AddPoint(int x, int y) {
	RelatePos[nPosCount].x = x;
	RelatePos[nPosCount].y = y;
	nPosCount++;
}

int CEveluation::GetRelatePiece(BYTE position[10][9], int j, int i) {
	nPosCount = 0;
	BYTE nChessID, flag;
	int x, y;
	nChessID = position[i][j];
	switch (nChessID){
		case R_KING:
		case B_KING:
			for (y = 0; y < 3; y++) {
				for (x = 3; x < 6; x++) {
					if (CanTouch(position, j, i, x, y)) {
						AddPoint(x, y);
					}
				}
			}
			for (y = 7; y < 10; y++) {
				for (x = 3; x < 6; x++) {
					if (CanTouch(position, j, i, x, y)) {
						AddPoint(x, y);
					}
				}
			}
			break;
		case R_BISHOP:
			for (y = 7; y < 10; y++) {
				for (x = 3; x < 6; x++) {
					if (CanTouch(position, j, i, x, y)) {
						AddPoint(x, y);
					}
				}
			}
			break;
		case B_BISHOP:
			for (y = 0; y < 3; y++) {
				for (x = 3; x < 6; x++) {
					if (CanTouch(position, j, i, x, y)) {
						AddPoint(x, y);
					}
				}
			}
			break;
		case R_ELEPHANT:
		case B_ELEPHANT:
			x = j + 2;
			y = i + 2;
			if (x < 9 && y < 10 && CanTouch(position, j, i, x, y)) {
				AddPoint(x, y);
			}
			x = j + 2;
			y = i - 2;
			if (x < 9 && y >= 0 && CanTouch(position, j, i, x, y)) {
				AddPoint(x, y);
			}
			x = j - 2;
			y = i + 2;
			if (x >= 0 && y < 10 && CanTouch(position, j, i, x, y)) {
				AddPoint(x, y);
			}
			x = j - 2;
			y = i - 2;
			if (x >= 0 && y >= 0 && CanTouch(position, j, i, x, y)) {
				AddPoint(x, y);
			}
			break;
		case R_HORSE:
		case B_HORSE:
			x = j + 2;
			y = i + 1;
			if (x < 9 && y < 10 && CanTouch(position, j, i, x, y)) {
				AddPoint(x, y);
			}
			x = j + 2;
			y = i - 1;
			if (x < 9 && y >= 0 && CanTouch(position, j, i, x, y)) {
				AddPoint(x, y);
			}
			x = j + 1;
			y = i + 2;
			if (x < 9 && y < 10 && CanTouch(position, j, i, x, y)) {
				AddPoint(x, y);
			}
			x = j + 1;
			y = i - 2;
			if (x < 9 && y >= 0 && CanTouch(position, j, i, x, y)) {
				AddPoint(x, y);
			}
			x = j - 2;
			y = i + 1;
			if (x >= 0 && y < 10 && CanTouch(position, j, i, x, y)) {
				AddPoint(x, y);
			}
			x = j - 2;
			y = i - 1;
			if (x >= 0 && y >= 0 && CanTouch(position, j, i, x, y)) {
				AddPoint(x, y);
			}
			x = j - 1;
			y = i + 2;
			if (x >= 0 && y < 10 && CanTouch(position, j, i, x, y)) {
				AddPoint(x, y);
			}
			x = j - 1;
			y = i - 2;
			if (x >= 0 && y >= 0 && CanTouch(position, j, i, x, y)) {
				AddPoint(x, y);
			}
			break;
		case R_CAR:
		case B_CAR:
			x = j + 1;
			y = i;
			for (; x < 9; x++) {
				if (position[y][x] == NOCHESS) {
					AddPoint(x, y);
				}else {
					AddPoint(x, y);
					break;
				}
			}
			x = j - 1;
			y = i;
			for (; x >= 0; x--) {
				if (position[y][x] == NOCHESS) {
					AddPoint(x, y);
				}else {
					AddPoint(x, y);
					break;
				}
			}
			x = j;
			y = i + 1;
			for (; y < 10; y++) {
				if (position[y][x] == NOCHESS) {
					AddPoint(x, y);
				}else {
					AddPoint(x, y);
					break;
				}
			}
			x = j;
			y = i - 1;
			for (; y >= 0; y--) {
				if (position[y][x] == NOCHESS) {
					AddPoint(x, y);
				}else {
					AddPoint(x, y);
					break;
				}
			}
			break;
		case R_PAWN:
			y = i - 1;
			x = j;
			if (y > 0) {
				AddPoint(x, y);
			}
			if (i < 5) {
				y = i;
				x = j + 1;
				if (x < 9) {
					AddPoint(x, y);
				}
				x = j - 1;
				if (x >= 0) {
					AddPoint(x, y);
				}
			}
			break;
		case B_PAWN:
			y = i + 1;
			x = j;
			if (y < 10) {
				AddPoint(x, y);
			}
			if (i > 4) {
				y = i;
				x = j + 1;
				if (x < 9) {
					AddPoint(x, y);
				}
				x = j - 1;
				if (x >= 0) {
					AddPoint(x, y);
				}
			}
			break;
		case B_CANON:
		case R_CANON:
			int x, y;
			bool flag;
			int nChessID;
			x = j + 1;
			y = i;
			flag = false;
			for (; x < 9; x++) {
				if (position[y][x] == NOCHESS) {
					if (!flag) {
						AddPoint(x, y);
					}
				}
				else {
					if (!flag) {
						flag = true;
					}
					else {
						AddPoint(x, y);
						break;
					}
				}
			}
			x = j - 1;
			y = i;
			flag = false;
			for (; x >= 0; x--) {
				if (position[y][x] == NOCHESS) {
					if (!flag) {
						AddPoint(x, y);
					}
				}
				else {
					if (!flag) {
						flag = true;
					}
					else {
						AddPoint(x, y);
						break;
					}
				}
			}
			x = j;
			y = i + 1;
			flag = false;
			for (; y < 10; y++) {
				if (position[y][x] == NOCHESS) {
					if (!flag) {
						AddPoint(x, y);
					}
				}
				else {
					if (!flag) {
						flag = true;
					}
					else {
						AddPoint(x, y);
						break;
					}
				}
			}
			x = j;
			y = i - 1;
			for (; y >= 0; y--) {
				if (position[y][x] == NOCHESS) {
					if (!flag) {
						AddPoint(x, y);
					}
				}
				else {
					if (!flag) {
						flag = true;
					}
					else {
						AddPoint(x, y);
					}
				}
			}
			break;
		default:
			break;
	}
	return nPosCount;
}

bool CEveluation::CanTouch(BYTE position[10][9], int nFromX, int nFromY, int nToX, int nToY) {
	int i, j;
	int nMoveChessID, nTargetID;
	if (nFromY == nToY&&nFromX == nToX) {
		return false;
	}
	nMoveChessID = position[nFromX][nFromY];
	nTargetID = position[nToX][nToY];
	switch (nMoveChessID) {
	case B_KING:
		if (nTargetID == R_KING) {
			if (nFromX != nToX) {
				return false;
			}
			for (i = nFromY + 1; i < nToY; ++i) {
				if (position[i][nFromX] != NOCHESS) {
					return false;
				}
			}
		}
		else {
			if (nToY > 2 || nToX > 5 || nToX < 3) {
				return false;
			}
			if (abs(nFromY - nToY) + abs(nToX - nFromX) > 1) {
				return false;
			}
		}
		break;
	case R_KING:
		if (nTargetID == B_KING) {
			if (nFromX != nToX) {
				return false;
			}
			for (i = nFromY - 1; i > nToY; --i) {
				if (position[i][nFromX] != NOCHESS) {
					return false;
				}
			}
		}
		else {
			if (nToY < 7 || nToX>5 || nToX < 3) {
				return false;
			}
			if (abs(nFromY - nToY) + abs(nToX - nFromX) > 1) {
				return false;
			}
		}
		break;
	case B_BISHOP:
		if (nToY > 2 || nToX > 5 || nToX < 3) {
			return false;
		}
		if (abs(nFromY - nToY) != 1 || abs(nToX - nFromX) != 1) {
			return false;
		}
		break;
	case R_BISHOP:
		if (nToY < 7 || nToX > 5 || nToY < 3) {
			return false;
		}
		if (abs(nFromY - nToY) != 1 || abs(nToX - nFromX) != 1) {
			return false;
		}
		break;
	case B_ELEPHANT:
		if (nToY > 5) {
			return false;
		}
		if (abs(nFromY - nToY) != 2 || abs(nToX - nFromX) != 2) {
			return false;
		}
		if (position[(nFromY + nToY) / 2][(nFromX + nToX) / 2] != NOCHESS) {
			return false;
		}
		break;
	case R_ELEPHANT:
		if (nToY < 4) {
			return false;
		}
		if (abs(nFromY - nToY) != 2 || abs(nToX - nFromX) != 2) {
			return false;
		}
		if (position[(nFromY + nToY) / 2][(nFromX + nToX) / 2] != NOCHESS) {
			return false;
		}
		break;
	case B_PAWN:
		if (nToY < nFromY) {
			return false;
		}
		if (nToY < 5 && nFromY == nToY) {
			return false;
		}
		if (nToY - nFromY + abs(nToX - nFromX) > 1) {
			return false;
		}
		break;
	case R_PAWN:
		if (nToY > nFromY) {
			return false;
		}
		if (nToY >4 && nFromY == nToY) {
			return false;
		}
		if (nFromY - nToY + abs(nToX - nFromX) > 1) {
			return false;
		}
		break;
	case B_CAR:
	case R_CAR:
		if (nFromY != nToY && nFromX != nToX) {
			return false;
		}
		if (nFromY == nToY) {
			if (nFromX < nToX) {
				for (i = nFromX + 1; i < nToX; ++i) {
					if (position[nFromY][i] != NOCHESS) {
						return false;
					}
				}
			}
			else {
				for (i = nToX + 1; i<nFromX; i++) {
					if (position[nFromY][i] != NOCHESS) {
						return false;
					}
				}
			}
		}
		else {
			if (nFromY < nToY) {
				for (j = nFromY + 1; j < nToY; j++) {
					if (position[j][nFromX] != NOCHESS) {
						return false;
					}
				}
			}
			else {
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
		if (!((abs(nToX - nFromX) == 1 && abs(nToY - nFromY) == 2) || (abs(nToX - nFromX) == 2 && abs(nToY - nFromY) == 1))) {//????????
			return false;
		}
		if (nToX - nFromX == 2) {
			i = nFromX + 1;
			j = nFromY;
		}
		else if (nFromX - nToX == 2) {
			i = nFromX - 1;
			j = nFromY;
		}
		else if (nToY - nFromY == 2) {
			i = nFromX;
			j = nFromY + 1;
		}
		else if (nFromY - nToY == 2) {
			i = nFromX;
			j = nFromY - 1;
		}
		if (position[j][i] != NOCHESS) {
			return false;
		}
		break;
	case B_CANON:
	case R_CANON:
		if (nFromY != nToY && nFromX != nToX) {
			return false;
		}
		if (position[nToY][nToX] == NOCHESS) {
			if (nFromY == nToY) {
				if (nFromX < nToX) {
					for (i = nFromX + 1; i < nToX; ++i) {
						if (position[nFromY][i] != NOCHESS) {
							return false;
						}
					}
				}
				else {
					for (i = nToX + 1; i<nFromX; i++) {
						if (position[nFromY][i] != NOCHESS) {
							return false;
						}
					}
				}
			}
			else {
				if (nFromY < nToY) {
					for (j = nFromY + 1; j < nToY; j++) {
						if (position[j][nFromX] != NOCHESS) {
							return false;
						}
					}
				}
				else {
					for (j = nToY + 1; j < nFromY; j++) {
						if (position[j][nFromX] != NOCHESS) {
							return false;
						}
					}
				}
			}
		}
		else {
			int count = 0;
			if (nFromY == nToY) {
				if (nFromX < nToX) {
					for (i = nFromX + 1; i < nToX; ++i) {
						if (position[nFromY][i] != NOCHESS) {
							count++;
						}
					}
					if (count != 1) {
						return false;
					}
				}
				else {
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
			else {
				if (nFromY < nToY) {
					for (j = nFromY + 1; j < nToY; j++) {
						if (position[j][nFromX] != NOCHESS) {
							count++;
						}
					}
					if (count != 1) {
						return false;
					}
				}
				else {
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