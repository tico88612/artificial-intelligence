
#ifndef STATE_INCLUDED
#define STATE_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "MyAI.h"

static const double INF = 2147483647;
static const double WIN = 1000000000;

struct State {
    int Board[32];
    int Color;
    int CloseChess[14];
    State(MyAI &ai) {
        for (int i = 0; i < 32; i++) {
            Board[i] = ai.getBoard(i);
        }
        for (int i = 0; i < 14; i++) {
            CloseChess[i] = ai.getClose(i);
        }
        Color = ai.getColor();
    }

    State(const State& n) : Color(n.Color) {
        for (int i = 0; i < 32; i++) {
            Board[i] = n.Board[i];
        }
        for (int i = 0; i < 14; i++) {
            CloseChess[i] = n.CloseChess[i];
        }
    }

    int Expand(int* Board, int color, int* Result) {
        int ResultCount = 0;
        for (int i = 0; i < 32; i++) {
            if (Board[i] >= 0 && Board[i] / 7 == color) {
                //Gun
                if (Board[i] % 7 == 1) {
                    int row = i / 4;
                    int col = i % 4;
                    for (int rowCount = row * 4; rowCount < (row + 1) * 4; rowCount++) {
                        if (Referee(Board, i, rowCount, color)) {
                            Result[ResultCount] = i * 100 + rowCount;
                            ResultCount++;
                        }
                    }
                    for (int colCount = col; colCount < 32; colCount += 4) {
                        if (Referee(Board, i, colCount, color)) {
                            Result[ResultCount] = i * 100 + colCount;
                            ResultCount++;
                        }
                    }
                } else {
                    int Move[4] = {i - 4, i + 1, i + 4, i - 1};
                    for (int k = 0; k < 4; k++) {
                        if (Move[k] >= 0 && Move[k] < 32 && Referee(Board, i, Move[k], color)) {
                            Result[ResultCount] = i * 100 + Move[k];
                            ResultCount++;
                        }
                    }
                }
            };
        }
        return ResultCount;
    }
    
    bool Referee(int* chess, int from_location_no, int to_location_no, int UserId) {
        int MessageNo = 0;
        bool IsCurrent = true;
        int from_chess_no = chess[from_location_no];
        int to_chess_no = chess[to_location_no];
        int from_row = from_location_no / 4;
        int to_row = to_location_no / 4;
        int from_col = from_location_no % 4;
        int to_col = to_location_no % 4;

        if (from_chess_no < 0 || (to_chess_no < 0 && to_chess_no != CHESS_EMPTY)) {
            MessageNo = 1;
            //strcat(Message,"**no chess can move**");
            //strcat(Message,"**can't move darkchess**");
            IsCurrent = false;
        } else if (from_chess_no >= 0 && from_chess_no / 7 != UserId) {
            MessageNo = 2;
            //strcat(Message,"**not my chess**");
            IsCurrent = false;
        } else if ((from_chess_no / 7 == to_chess_no / 7) && to_chess_no >= 0) {
            MessageNo = 3;
            //strcat(Message,"**can't eat my self**");
            IsCurrent = false;
        }
        //check attack
        else if (to_chess_no == CHESS_EMPTY && abs(from_row - to_row) + abs(from_col - to_col) == 1)  //legal move
        {
            IsCurrent = true;
        } else if (from_chess_no % 7 == 1)  //judge gun
        {
            int row_gap = from_row - to_row;
            int col_gap = from_col - to_col;
            int between_Count = 0;
            //slant
            if (from_row - to_row == 0 || from_col - to_col == 0) {
                //row
                if (row_gap == 0) {
                    for (int i = 1; i < abs(col_gap); i++) {
                        int between_chess;
                        if (col_gap > 0)
                            between_chess = chess[from_location_no - i];
                        else
                            between_chess = chess[from_location_no + i];
                        if (between_chess != CHESS_EMPTY)
                            between_Count++;
                    }
                }
                //column
                else {
                    for (int i = 1; i < abs(row_gap); i++) {
                        int between_chess;
                        if (row_gap > 0)
                            between_chess = chess[from_location_no - 4 * i];
                        else
                            between_chess = chess[from_location_no + 4 * i];
                        if (between_chess != CHESS_EMPTY)
                            between_Count++;
                    }
                }

                if (between_Count != 1) {
                    MessageNo = 4;
                    //strcat(Message,"**gun can't eat opp without between one piece**");
                    IsCurrent = false;
                } else if (to_chess_no == CHESS_EMPTY) {
                    MessageNo = 5;
                    //strcat(Message,"**gun can't eat opp without between one piece**");
                    IsCurrent = false;
                }
            }
            //slide
            else {
                MessageNo = 6;
                //strcat(Message,"**cant slide**");
                IsCurrent = false;
            }
        } else  // non gun
        {
            //judge pawn or king

            //distance
            if (abs(from_row - to_row) + abs(from_col - to_col) > 1) {
                MessageNo = 7;
                //strcat(Message,"**cant eat**");
                IsCurrent = false;
            }
            //judge pawn
            else if (from_chess_no % 7 == 0) {
                if (to_chess_no % 7 != 0 && to_chess_no % 7 != 6) {
                    MessageNo = 8;
                    //strcat(Message,"**pawn only eat pawn and king**");
                    IsCurrent = false;
                }
            }
            //judge king
            else if (from_chess_no % 7 == 6 && to_chess_no % 7 == 0) {
                MessageNo = 9;
                //strcat(Message,"**king can't eat pawn**");
                IsCurrent = false;
            } else if (from_chess_no % 7 < to_chess_no % 7) {
                MessageNo = 10;
                //strcat(Message,"**cant eat**");
                IsCurrent = false;
            }
        }
        return IsCurrent;
    }
    
};

double Eval(const State& B) {
    double t = 0;
    int ind = 0;
    int myChess[32];

    return t + rand() % 10;
}
double NegaScout(State& B, int dep, int cut, double alpha, double beta, int& BestMove) {
    if (cut == 0) {
        return Eval(B);
    }
}
#endif