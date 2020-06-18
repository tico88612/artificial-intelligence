#include "MyAI.h"

#include <limits>

#include "State.h"

#define DLIMIT 5

MyAI::MyAI(void) { this->Color = 2; }

MyAI::~MyAI(void) {}

// 0 protocol_version 回傳通訊協定版本 (Done)
bool MyAI::protocol_version(const char* data[], char* response) {
    strcpy(response, "1.0.0");
    return 0;
}

// 1 name 回傳 AI 程式名稱 (Done)
bool MyAI::name(const char* data[], char* response) {
    strcpy(response, "tico88612");
    return 0;
}

// 2 version 回傳 AI 程式版本 (Done)
bool MyAI::version(const char* data[], char* response) {
    strcpy(response, "0.54.87");
    return 0;
}

// 3 known_command 是否支援此指令 (Done)
bool MyAI::known_command(const char* data[], char* response) {
    for (int i = 0; i < COMMAND_NUM; i++) {
        if (!strcmp(data[0], commands_name[i])) {
            strcpy(response, "true");
            return 0;
        }
    }
    strcpy(response, "false");
    return 0;
}

// 4 list_commands 列出所支援的指令 (Done)
bool MyAI::list_commands(const char* data[], char* response) {
    for (int i = 0; i < COMMAND_NUM; i++) {
        strcat(response, commands_name[i]);
        if (i < COMMAND_NUM - 1) {
            strcat(response, "\n");
        }
    }
    return 0;
}

// 5 quit 要求程式終止 (Done)
bool MyAI::quit(const char* data[], char* response) {
    fprintf(stderr, "Bye\n");
    return 0;
}

// 6 boardsize 設定盤面大小 (Done)
bool MyAI::boardsize(const char* data[], char* response) {
    fprintf(stderr, "BoardSize: %s x %s\n", data[0], data[1]);
    return 0;
}

// 7 reset_board 要求重置盤面
bool MyAI::reset_board(const char* data[], char* response) {
    this->Color = 2;
    this->Red_Time = -1;    // known
    this->Black_Time = -1;  // known
    this->initBoardState();
    return 0;
}

// 8 num_repetition 設定最多盤面重複次數
bool MyAI::num_repetition(const char* data[], char* response) {
    return 0;
}

// 9 num_moves_to_draw 設定最多無吃翻次數
bool MyAI::num_moves_to_draw(const char* data[], char* response) {
    return 0;
}

// 10 move 傳送走步
bool MyAI::move(const char* data[], char* response) {
    char move[6];
    sprintf(move, "%s-%s", data[0], data[1]);
    this->MakeMove(move);
    return 0;
}

// 11 flip 傳送翻棋
bool MyAI::flip(const char* data[], char* response) {
    char move[6];
    sprintf(move, "%s(%s)", data[0], data[1]);
    this->MakeMove(move);
    return 0;
}

// 12 genmove 要求產生走步
bool MyAI::genmove(const char* data[], char* response) {
    // set color
    if (!strcmp(data[0], "red")) {
        this->Color = RED;
    } else if (!strcmp(data[0], "black")) {
        this->Color = BLACK;
    } else {
        this->Color = 2;
    }
    // genmove
    char move[6];
    this->generateMove(move);
    sprintf(response, "%c%c %c%c", move[0], move[1], move[3], move[4]);
    return 0;
}

// 13 game_over 傳送遊戲結果
bool MyAI::game_over(const char* data[], char* response) {
    fprintf(stderr, "Game Result: %s\n", data[0]);
    return 0;
}

// 14 ready 詢問是否準備完成
bool MyAI::ready(const char* data[], char* response) {
    return 0;
}

// 15 time_settings 設定時間
bool MyAI::time_settings(const char* data[], char* response) {
    return 0;
}

// 16 time_left 傳送剩餘時間
bool MyAI::time_left(const char* data[], char* response) {
    if (!strcmp(data[0], "red")) {
        sscanf(data[1], "%d", &(this->Red_Time));
    } else {
        sscanf(data[1], "%d", &(this->Black_Time));
    }
    fprintf(stderr, "Time Left(%s): %s\n", data[0], data[1]);
    return 0;
}

// 17 showboard 顯示當前盤面 (Done)
bool MyAI::showboard(const char* data[], char* response) {
    Pirnf_Chessboard();
    return 0;
}

int GetFin(char c) {
    static const char skind[] = {'-', 'K', 'G', 'M', 'R', 'N', 'C', 'P', 'X', 'k', 'g', 'm', 'r', 'n', 'c', 'p'};
    for (int f = 0; f < 16; f++)
        if (c == skind[f]) return f;
    return -1;
}

void MyAI::initBoardState() {
    /*

	iPieceCount[14] (index):  [00] [01] [02] [03] [04] [05] [06] [07] [08] [09] [10] [11] [12] [13]
	                           RK   RG   RM   RR   RN   RC   RP   BK   BG   BM   BR   BN   BC   BP 
    	iCurrentPosition[32] (value):  [00] [01] [02] [03] [04] [05] [06] [07] [08] [09] [10] [11] [12] [13] [14] [15]  
	                            	empty  RK   RR   RN   RN   RR   RC   RP  dark  BK   BG   BM   BR   BN   BC   BP 
	*/
    // initial board
    char iCurrentPosition[32];
    for (int i = 0; i < 32; i++) {
        iCurrentPosition[i] = 'X';
    }
    int iPieceCount[14] = {1, 2, 2, 2, 2, 2, 5, 1, 2, 2, 2, 2, 2, 5};

    puts("\niPieceCount[14]:");
    for (int i = 0; i < 14; i++) printf("[%2d]", i);
    puts("");
    for (int i = 0; i < 14; i++) printf("%4d", iPieceCount[i]);
    puts("\n");
    puts("iCurrentPosition[32]:");
    for (int i = 28; i >= 0; i -= 4) {
        for (int j = 0; j < 4; j++)
            printf("%2c ", iCurrentPosition[i + j]);
        printf(" | ");
        for (int j = 0; j < 4; j++)
            printf("%2d ", i + j);
        if (i % 4 == 0) puts("");
    }
    puts("");

    printf("\n\n\n");

    memcpy(this->CloseChess, iPieceCount, sizeof(int) * 14);

    //convert to my format
    int Index = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 4; j++) {
            this->Board[Index] = ConvertChessNo(GetFin(iCurrentPosition[Index]));
            Index++;
        }
    }
    this->Pirnf_Chessboard();
}

// Start Implementation Alpha-Beta Pruning

void MyAI::generateMove(char move[6]) {
    // fprintf(stderr, "Generating moves...\n");
    
    
}
/*
void MyAI::generateMove(char move[6]) {
    // generateMove Call by reference: change src,dst 

    int Result[100];
    int count = this->Expand(this->Board, this->Color, Result);
    int startPoint = 0;
    int EndPount = 0;

    int CoverIndex[32];
    int CoverCount = 0;
    for (int i = 0; i < 32; i++)
        if (this->Board[i] == CHESS_COVER) {
            CoverIndex[CoverCount] = i;
            CoverCount++;
        }
    int Answer;
    if (CoverCount > 0) {
        Result[count] = CoverIndex[rand() % CoverCount];
        Result[count] = Result[count] * 100 + Result[count];
        count++;
    }

    //randon choose legal move
    if (count > 0) {
        Answer = Result[rand() % count];
        startPoint = Answer / 100;
        EndPount = Answer % 100;
        sprintf(move, "%c%c-%c%c", 'a' + (startPoint % 4), '1' + (7 - startPoint / 4), 'a' + (EndPount % 4), '1' + (7 - EndPount / 4));
    }
    //no legal move -> flip chess
    else {
        sprintf(move, "NAN");
    }
    char chess_Start[4] = "";
    char chess_End[4] = "";
    Pirnf_Chess(Board[startPoint], chess_Start);
    Pirnf_Chess(Board[EndPount], chess_End);
    printf("My result: \n--------------------------\n");
    printf("(%d) -> (%d)\n", startPoint, EndPount);
    printf("<%s> -> <%s>\n", chess_Start, chess_End);
    printf("move:%s\n", move);
    printf("--------------------------\n");
    this->Pirnf_Chessboard();
}
*/

void MyAI::MakeMove(const char move[6]) {
    int src, dst;
    src = ('8' - move[1]) * 4 + (move[0] - 'a');
    if (move[2] == '(') {
        printf("# call flip(): flip(%d,%d) = %d\n", src, src, GetFin(move[3]));
        this->Board[src] = ConvertChessNo(GetFin(move[3]));
        --CloseChess[ConvertChessNo(GetFin(move[3]))];
        Pirnf_Chessboard();
    } else {
        dst = ('8' - move[4]) * 4 + (move[3] - 'a');
        printf("# Search call move(): move : %d-%d \n", src, dst);
        this->Board[dst] = this->Board[src];
        this->Board[src] = CHESS_EMPTY;
        Pirnf_Chessboard();
    }
    /* init time */
}

//---------------------------------------------my search function---------------------------------------------
int MyAI::ConvertChessNo(int input) {
    switch (input) {
        case 0:
            return CHESS_EMPTY;
            break;
        case 8:
            return CHESS_COVER;
            break;
        case 1:
            return 6;
            break;
        case 2:
            return 5;
            break;
        case 3:
            return 4;
            break;
        case 4:
            return 3;
            break;
        case 5:
            return 2;
            break;
        case 6:
            return 1;
            break;
        case 7:
            return 0;
            break;
        case 9:
            return 13;
            break;
        case 10:
            return 12;
            break;
        case 11:
            return 11;
            break;
        case 12:
            return 10;
            break;
        case 13:
            return 9;
            break;
        case 14:
            return 8;
            break;
        case 15:
            return 7;
            break;
    }
    return -1;
}

// 產生有效盤面
int MyAI::Expand(int* Board, int color, int* Result) {
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

//---------------------------- Display --------------------------------
//Display chess board
void MyAI::Pirnf_Chessboard() {
    char Mes[1024] = "";
    char temp[1024];
    char myColor[10] = "";
    if (Color == -99)
        strcpy(myColor, "Unknown");
    else if (this->Color == RED)
        strcpy(myColor, "Red");
    else
        strcpy(myColor, "Black");
    sprintf(temp, "------------%s-------------\n", myColor);
    strcat(Mes, temp);
    strcat(Mes, "<8> ");
    for (int i = 0; i < 32; i++) {
        if (i != 0 && i % 4 == 0) {
            sprintf(temp, "\n<%d> ", 8 - (i / 4));
            strcat(Mes, temp);
        }
        char chess_name[4] = "";
        Pirnf_Chess(this->Board[i], chess_name);
        sprintf(temp, "%5s", chess_name);
        strcat(Mes, temp);
    }
    strcat(Mes, "\n\n     ");
    for (int i = 0; i < 4; i++) {
        sprintf(temp, " <%c> ", 'a' + i);
        strcat(Mes, temp);
    }
    strcat(Mes, "\n\n");
    printf("%s", Mes);
}

//Print chess
void MyAI::Pirnf_Chess(int chess_no, char* Result) {
    // XX -> Empty
    if (chess_no == CHESS_EMPTY) {
        strcat(Result, " - ");
        return;
    }
    //OO -> DarkChess
    else if (chess_no == CHESS_COVER) {
        strcat(Result, " X ");
        return;
    }
    /*if(Color == RED )
				strcat(Result, "R");
		else
				strcat(Result, "B");*/
    switch (chess_no) {
        case 0:
            strcat(Result, " P ");
            break;
        case 1:
            strcat(Result, " C ");
            break;
        case 2:
            strcat(Result, " N ");
            break;
        case 3:
            strcat(Result, " R ");
            break;
        case 4:
            strcat(Result, " M ");
            break;
        case 5:
            strcat(Result, " G ");
            break;
        case 6:
            strcat(Result, " K ");
            break;
        case 7:
            strcat(Result, " p ");
            break;
        case 8:
            strcat(Result, " c ");
            break;
        case 9:
            strcat(Result, " n ");
            break;
        case 10:
            strcat(Result, " r ");
            break;
        case 11:
            strcat(Result, " m ");
            break;
        case 12:
            strcat(Result, " g ");
            break;
        case 13:
            strcat(Result, " k ");
            break;
    }
}

// Referee
bool MyAI::Referee(int* chess, int from_location_no, int to_location_no, int UserId) {
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
