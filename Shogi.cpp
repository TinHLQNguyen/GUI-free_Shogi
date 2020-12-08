#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
using namespace std;

/* ---------------------Chess piece format--------------------- */
#define RED_PIECE   " \033[1;31m\033[1;43m"
#define BLUE_PIECE  " \033[1;34m\033[1;43m"
#define RESET_COLOR "\x1B[0m"
/* ---------------------Piece index format--------------------- */
#define KING    0
#define GOLD    1
#define SILVER  2
#define KNIGHT  3
#define LANCE   4
#define BISHOP  5
#define ROOK    6
#define PAWN    7
#define INVALID_NAME 99

#define KING_CHAR   "玉"
#define GOLD_CHAR   "金"
#define SILVER_CHAR "銀"
#define KNIGHT_CHAR "桂"
#define LANCE_CHAR  "香"
#define BISHOP_CHAR "角"
#define ROOK_CHAR   "飛"
#define PAWN_CHAR   "歩"

#define SILVER_PROMOTED_CHAR    "S"
#define KNIGHT_PROMOTED_CHAR    "K"
#define LANCE_PROMOTED_CHAR     "L"
#define PAWN_PROMOTED_CHAR      "と"
#define BISHOP_PROMOTED_CHAR    "馬"
#define ROOK_PROMOTED_CHAR      "竜"

#define RED_KING RED_PIECE KING_CHAR RESET_COLOR
#define RED_GOLD RED_PIECE GOLD_CHAR RESET_COLOR
#define RED_SILVER RED_PIECE SILVER_CHAR RESET_COLOR
#define RED_KNIGHT RED_PIECE KNIGHT_CHAR RESET_COLOR
#define RED_LANCE RED_PIECE LANCE_CHAR RESET_COLOR
#define RED_BISHOP RED_PIECE BISHOP_CHAR RESET_COLOR
#define RED_ROOK RED_PIECE ROOK_CHAR RESET_COLOR
#define RED_PAWN RED_PIECE PAWN_CHAR RESET_COLOR

#define RED_PROMOTED_SILVER RED_PIECE SILVER_PROMOTED_CHAR RESET_COLOR
#define RED_PROMOTED_KNIGHT RED_PIECE KNIGHT_PROMOTED_CHAR RESET_COLOR
#define RED_PROMOTED_LANCE RED_PIECE LANCE_PROMOTED_CHAR RESET_COLOR
#define RED_PROMOTED_BISHOP RED_PIECE BISHOP_PROMOTED_CHAR RESET_COLOR
#define RED_PROMOTED_ROOK RED_PIECE ROOK_PROMOTED_CHAR RESET_COLOR
#define RED_PROMOTED_PAWN RED_PIECE PAWN_PROMOTED_CHAR RESET_COLOR

#define BLUE_KING BLUE_PIECE KING_CHAR RESET_COLOR
#define BLUE_GOLD BLUE_PIECE GOLD_CHAR RESET_COLOR
#define BLUE_SILVER BLUE_PIECE SILVER_CHAR RESET_COLOR
#define BLUE_KNIGHT BLUE_PIECE KNIGHT_CHAR RESET_COLOR
#define BLUE_LANCE BLUE_PIECE LANCE_CHAR RESET_COLOR
#define BLUE_BISHOP BLUE_PIECE BISHOP_CHAR RESET_COLOR
#define BLUE_ROOK BLUE_PIECE ROOK_CHAR RESET_COLOR
#define BLUE_PAWN BLUE_PIECE PAWN_CHAR RESET_COLOR

#define BLUE_PROMOTED_SILVER BLUE_PIECE SILVER_PROMOTED_CHAR RESET_COLOR
#define BLUE_PROMOTED_KNIGHT BLUE_PIECE KNIGHT_PROMOTED_CHAR RESET_COLOR
#define BLUE_PROMOTED_LANCE BLUE_PIECE LANCE_PROMOTED_CHAR RESET_COLOR
#define BLUE_PROMOTED_BISHOP BLUE_PIECE BISHOP_PROMOTED_CHAR RESET_COLOR
#define BLUE_PROMOTED_ROOK BLUE_PIECE ROOK_PROMOTED_CHAR RESET_COLOR
#define BLUE_PROMOTED_PAWN BLUE_PIECE PAWN_PROMOTED_CHAR RESET_COLOR

#define EMPTY_SQUARE "   "
/* ---------------------Other constants--------------------- */
#define BOARDSIZE   9

#define KINGNUM     1
#define GOLDNUM     2
#define SILVERNUM   2
#define KNIGHTNUM   2
#define LANCENUM    2
#define BISHOPNUM   1
#define ROOKNUM     1
#define PAWNNUM     9

#define RED_TURN    true
#define BLUE_TURN   false
#define RED     true
#define BLUE    false

#define HELP "In ones' turn, start by selecting either moving a piece (m) or dropping a piece (d).\nTo move a piece, input when prompted with the follwoing  format: <PIECE> <start suare> <end square> (Exp: PAWN G1 F1).\nTo drop a piece, input when prompted with the follwoing format: <PIECE> <location square> (Exp: PAWN E5).\n<PIECE>'s keywords: \n - KING\n - GOLD\n - SILVER\n - KNIGHT\n - LANCE\n - BISHOP\n - ROOK\n - PAWN\n\n *note: keywords are case-sensitive. \n ";

#define MOVE_SUCCESS    0
#define DROP_SUCCESS    0
#define INVALID_PIECE   1
#define INVALID_START   2
#define INVALID_END     3
#define INVALID_DROP    2

#define VALID_MOVE      1
#define INVALID_MOVE    0

#define NOT_BLOCKED     1
#define BLOCKED         0

#define DEBUG true
/* ---------------------Structure prototype-------------------- */
struct SHOGI_GAME {
    string s_p1_name;
    string s_p2_name;
    string s_board[BOARDSIZE][BOARDSIZE];
    int i_turns; // No. of turns of the game so far
    string s_command; // either 'm': move a piece or 'd': drop a piece
    string s_ctrl_piece;
    char square1[2];
    char square2[2];
    // red pieces:
    string a_redPieces[14];
    string a_bluePieces[14];
    // No. of pieces of each side
    int a_rPieceNum[8], a_bPieceNum[8];
    // No. of captured of each side
    int a_rPieceCap[8], a_bPieceCap[8];
    // Repetition check
    bool Repetition_FLAG;
    // Impasse check
    bool Impasse_FLAG;
    // not yet implement promotion
} mygame;

/* ---------------------Function Prototypes-------------------- */
void initialization(SHOGI_GAME* p_Game);
void display(SHOGI_GAME* p_Game);
void results(SHOGI_GAME* p_Game);
void game();
void game_loop(SHOGI_GAME* p_Game);
int move_piece(SHOGI_GAME* p_Game);
int drop_piece(SHOGI_GAME* p_Game);
int encode_piece(string piece_name);
int check_valid_move(int piece, bool player, int start_row, int start_col, int end_row, int end_col);
int is_blocked(SHOGI_GAME* p_Game, int piece, int start_row, int start_col, int end_row, int end_col);

int main()
{
    game();
    return 0;
}

/* ---------------------Function definition-------------------- */
//////////////////////// initialization() ////////////////////////
// Purpose                                                      //
//////////////////////////////////////////////////////////////////
void initialization(SHOGI_GAME* p_Game){
    // Initialize the board
    p_Game->i_turns = 0;
    int row = 0;
    int col = 0;
    for (col = 0; col < BOARDSIZE; col ++ ){
        for (row = 0; row < BOARDSIZE; row ++){
            p_Game->s_board[row][col] = EMPTY_SQUARE; // blank square
        }
    }

    p_Game->s_board[0][0] = RED_LANCE;  p_Game->s_board[0][8] = RED_LANCE;
    p_Game->s_board[0][1] = RED_KNIGHT; p_Game->s_board[0][7] = RED_KNIGHT;
    p_Game->s_board[0][2] = RED_SILVER; p_Game->s_board[0][6] = RED_SILVER;
    p_Game->s_board[0][3] = RED_GOLD;   p_Game->s_board[0][5] = RED_GOLD;
    p_Game->s_board[0][4] = RED_KING;
    p_Game->s_board[1][1] = RED_ROOK;
    p_Game->s_board[1][7] = RED_BISHOP;
    for (col = 0; col < BOARDSIZE; col++)
        p_Game->s_board[2][col] = RED_PAWN;

    p_Game->s_board[8][0] = BLUE_LANCE;  p_Game->s_board[8][8] = BLUE_LANCE;
    p_Game->s_board[8][1] = BLUE_KNIGHT; p_Game->s_board[8][7] = BLUE_KNIGHT;
    p_Game->s_board[8][2] = BLUE_SILVER; p_Game->s_board[8][6] = BLUE_SILVER;
    p_Game->s_board[8][3] = BLUE_GOLD;   p_Game->s_board[8][5] = BLUE_GOLD;
    p_Game->s_board[8][4] = BLUE_KING;
    p_Game->s_board[7][1] = BLUE_BISHOP;
    p_Game->s_board[7][7] = BLUE_ROOK;
    for (col = 0; col < BOARDSIZE; col++)
        p_Game->s_board[6][col] = BLUE_PAWN;

    // Initialize the list of pieces
    p_Game->a_redPieces[KING] = RED_KING; 
    p_Game->a_redPieces[GOLD] = RED_GOLD;
    p_Game->a_redPieces[SILVER] = RED_SILVER;
    p_Game->a_redPieces[KNIGHT] = RED_KNIGHT;
    p_Game->a_redPieces[LANCE] = RED_LANCE;
    p_Game->a_redPieces[BISHOP] = RED_BISHOP;
    p_Game->a_redPieces[ROOK] = RED_ROOK;
    p_Game->a_redPieces[PAWN] = RED_PAWN;
    p_Game->a_redPieces[SILVER+6] = RED_PROMOTED_SILVER;
    p_Game->a_redPieces[KNIGHT+6] = RED_PROMOTED_KNIGHT;
    p_Game->a_redPieces[LANCE+6] = RED_PROMOTED_LANCE;
    p_Game->a_redPieces[BISHOP+6] = RED_PROMOTED_BISHOP;
    p_Game->a_redPieces[ROOK+6] = RED_PROMOTED_ROOK;
    p_Game->a_redPieces[PAWN+6] = RED_PROMOTED_PAWN;

    p_Game->a_bluePieces[KING] = BLUE_KING; 
    p_Game->a_bluePieces[GOLD] = BLUE_GOLD;
    p_Game->a_bluePieces[SILVER] = BLUE_SILVER;
    p_Game->a_bluePieces[KNIGHT] = BLUE_KNIGHT;
    p_Game->a_bluePieces[LANCE] = BLUE_LANCE;
    p_Game->a_bluePieces[BISHOP] = BLUE_BISHOP;
    p_Game->a_bluePieces[ROOK] = BLUE_ROOK;
    p_Game->a_bluePieces[PAWN] = BLUE_PAWN;
    p_Game->a_bluePieces[SILVER+6] = BLUE_PROMOTED_SILVER;
    p_Game->a_bluePieces[KNIGHT+6] = BLUE_PROMOTED_KNIGHT;
    p_Game->a_bluePieces[LANCE+6] = BLUE_PROMOTED_LANCE;
    p_Game->a_bluePieces[BISHOP+6] = BLUE_PROMOTED_BISHOP;
    p_Game->a_bluePieces[ROOK+6] = BLUE_PROMOTED_ROOK;
    p_Game->a_bluePieces[PAWN+6] = BLUE_PROMOTED_PAWN;
    
    // initialize the number of pieces
    p_Game->a_rPieceNum[KING] = KINGNUM;        p_Game->a_bPieceNum[KING] = KINGNUM;
    p_Game->a_rPieceNum[GOLD] = GOLDNUM;        p_Game->a_bPieceNum[GOLD] = GOLDNUM;
    p_Game->a_rPieceNum[SILVER] = SILVERNUM;    p_Game->a_bPieceNum[SILVER] = SILVERNUM;
    p_Game->a_rPieceNum[KNIGHT] = KNIGHTNUM;    p_Game->a_bPieceNum[KNIGHT] = KNIGHTNUM;
    p_Game->a_rPieceNum[LANCE] = LANCENUM;      p_Game->a_bPieceNum[LANCE] = LANCENUM;
    p_Game->a_rPieceNum[BISHOP] = BISHOPNUM;    p_Game->a_bPieceNum[BISHOP] = BISHOPNUM;
    p_Game->a_rPieceNum[ROOK] = ROOKNUM;        p_Game->a_bPieceNum[ROOK] = ROOKNUM;
    p_Game->a_rPieceNum[PAWN] = PAWNNUM;        p_Game->a_bPieceNum[PAWN] = PAWNNUM;
    // initialize the number of captured pieces
    p_Game->a_rPieceCap[KING] = KINGNUM - p_Game->a_rPieceNum[KING];
    p_Game->a_bPieceCap[KING] = KINGNUM - p_Game->a_bPieceNum[KING];
    p_Game->a_rPieceCap[GOLD] = GOLDNUM - p_Game->a_rPieceNum[GOLD];
    p_Game->a_bPieceCap[GOLD] = GOLDNUM - p_Game->a_bPieceNum[GOLD];   
    p_Game->a_rPieceCap[SILVER] = SILVERNUM - p_Game->a_rPieceNum[SILVER];
    p_Game->a_bPieceCap[SILVER] = SILVERNUM - p_Game->a_bPieceNum[SILVER];  
    p_Game->a_rPieceCap[KNIGHT] = KNIGHTNUM - p_Game->a_rPieceNum[KNIGHT];
    p_Game->a_bPieceCap[KNIGHT] = KNIGHTNUM - p_Game->a_bPieceNum[KNIGHT];
    p_Game->a_rPieceCap[LANCE] = LANCENUM - p_Game->a_rPieceNum[LANCE];
    p_Game->a_bPieceCap[LANCE] = LANCENUM - p_Game->a_bPieceNum[LANCE];
    p_Game->a_rPieceCap[BISHOP] = BISHOPNUM - p_Game->a_rPieceNum[BISHOP];
    p_Game->a_bPieceCap[BISHOP] = BISHOPNUM - p_Game->a_bPieceNum[BISHOP];
    p_Game->a_rPieceCap[ROOK] = ROOKNUM - p_Game->a_rPieceNum[ROOK];
    p_Game->a_bPieceCap[ROOK] = ROOKNUM - p_Game->a_bPieceNum[ROOK];
    p_Game->a_rPieceCap[PAWN] = PAWNNUM - p_Game->a_rPieceNum[PAWN];
    p_Game->a_bPieceCap[PAWN] = PAWNNUM - p_Game->a_bPieceNum[PAWN];
    // Initialize repetition check
    p_Game->Repetition_FLAG = false;
    // Initialize impasse check
    p_Game->Impasse_FLAG = false;
}

/////////////////////////// display() ////////////////////////////
// Purpose: display the current state of the board              //
//////////////////////////////////////////////////////////////////
void display(SHOGI_GAME* p_Game){
    int loop_var = 0; // exclusive use for looping counter
    int col = 0;
    int row = 0;
    // Displaying the 2 first rows
    cout << "   1   2   3   4   5   6   7   8   9 ";
    cout << "\n ";
    for (loop_var = 0; loop_var < 9; loop_var++){
        cout << " ---";
    }
    cout << "\n";
    // Displaying the rest
    for (row = 0; row < BOARDSIZE; row++)
    {
        cout << (char)(65+row); //65: ASCII of letter A
        cout << "|";
        for (col = 0; col < BOARDSIZE; col++){
            cout << p_Game->s_board[row][col];
            cout << "|";
        }
        cout << "\n ";
        // separate line
        for (loop_var = 0; loop_var < 9; loop_var++){
            cout << " ---";
        }
        cout << "\n";
    }

    if (p_Game->i_turns == 0){
        cout << "Red player goes first! \n";
        cout << "----------------------------- \n";
        p_Game->i_turns = 1;
    }
}

///////////////////////// game_loop() ////////////////////////////
// Purpose: Take commands from players according to turn to     //
// either move or drop the captured piece. Command's legitimacy //
// are checked. User need to re-input if they're not.           //
//////////////////////////////////////////////////////////////////
void game_loop(SHOGI_GAME* p_Game){
    bool correct_command = false;
    int command_result;
    if (p_Game->i_turns%2 == RED_TURN){
            cout << RED_PIECE << "Red player " << p_Game->s_p1_name << RESET_COLOR << "'s turn. Turn number " << p_Game->i_turns << endl;
        }else{
            cout << BLUE_PIECE << "Blue player " << p_Game->s_p2_name << RESET_COLOR << "'s turn. Turn number " << p_Game->i_turns << endl;
        }

    // The first input of a turn
    correct_command = false;
    cout << "Select your desired command: move (m) or drop (d) (""-h"" for help):";
    cin >> p_Game->s_command;

    while (!correct_command){
        if (p_Game->s_command == "m"){
            cout << "Enter move command (Type -q -q -q to return to command type): ";
            cin >> p_Game->s_ctrl_piece >> p_Game->square1 >> p_Game->square2;
            command_result = move_piece(p_Game);
            switch (command_result){
            case MOVE_SUCCESS:
                correct_command = true;
                break;
            case INVALID_PIECE:
                cout << "Invalid piece to move, please try again \n";
                cout << "Select your desired command: move (m) or drop (d) (""-h"" for help):";
                cin >> p_Game->s_command;
                break;
            case INVALID_START:
                cout << "Invalid starting square, please try again \n";
                cout << "Select your desired command: move (m) or drop (d) (""-h"" for help):";
                cin >> p_Game->s_command;
                break;
            case INVALID_END:
                cout << "Invalid destination square, please try again \n";
                cout << "Select your desired command: move (m) or drop (d) (""-h"" for help):";
                cin >> p_Game->s_command;
                break;
            }

        }else if (p_Game->s_command == "d"){
            // show the available drop
            cout << "Available drop pieces: \n";
            cout << "|";
            if (p_Game->i_turns%2 == RED_TURN){
                for (int loop_var = 0; loop_var < PAWN+1; loop_var++){
                    cout << p_Game->a_redPieces[loop_var];
                    cout << "|";
                }
                cout << endl;
                cout << "|";
                for (int loop_var = 0; loop_var < PAWN+1; loop_var++){
                    cout << " " << p_Game->a_rPieceCap[loop_var] << " ";
                    cout << "|";
                }
                cout << endl;
            }else{
                for (int loop_var = 0; loop_var < PAWN+1; loop_var++){
                    cout <<p_Game->a_bluePieces[loop_var];
                    cout << "|";
                }
                cout << endl;
                cout << "|";
                for (int loop_var = 0; loop_var < PAWN+1; loop_var++){
                    cout << " " << p_Game->a_bPieceCap[loop_var] << " ";
                    cout << "|";
                }
                cout << endl;
            }
            // input the drop command
            cout << "Enter drop command (Type -q -q to return to command type): ";
            cin >> p_Game->s_ctrl_piece >> p_Game->square1;
            command_result = drop_piece(p_Game);
            switch (command_result){
            case DROP_SUCCESS:
                correct_command = true;
                break;
            case INVALID_PIECE:
                cout << "Invalid piece to drop, please try again \n";
                cout << "Select your desired command: move (m) or drop (d) (""-h"" for help):";
                cin >> p_Game->s_command;
                break;
            case INVALID_DROP:
                cout << "Invalid square to drop, please try again \n";
                cout << "Select your desired command: move (m) or drop (d) (""-h"" for help):";
                cin >> p_Game->s_command;
                break;
            }

        }else if (p_Game->s_command == "-h"){
            cout << HELP;
            cout << "Select your desired command: move (m) or drop (d):";
            cin >> p_Game->s_command;
        }else{
            cout << "Invalid command, please try again \n";
            cout << "Select your desired command: move (m) or drop (d):";
            cin >> p_Game->s_command;
        }
    }
    
    // Proceed to next turn
    p_Game->i_turns++;
}
///////////////////////// move_piece() ///////////////////////////
// Purpose: check the legitimacy and check the command to move a//
// piece.                                                       //
//////////////////////////////////////////////////////////////////
int move_piece(SHOGI_GAME* p_Game){
    int i_ctrl_piece = encode_piece(p_Game->s_ctrl_piece);
    int i_target_piece = 0;
    bool b_target_color = RED;
    bool player;
    if (p_Game->i_turns%2 == RED_TURN)
        player = RED_TURN;
    else
        player = BLUE_TURN;

    int i_start_row = (int)p_Game->square1[0]-65; //Convertinting the char to int from ASCII
    int i_start_col = (int)p_Game->square1[1]-49; //Convertinting the char to int from ASCII, starting from 0
    int i_end_row = (int)p_Game->square2[0]-65;
    int i_end_col = (int)p_Game->square2[1]-49;

    bool match_Start = false;
    int red_search_var = 0;
    int blue_search_var = 0;

    // check input name of the piece
    if (i_ctrl_piece == INVALID_NAME)
        return INVALID_PIECE;

    // check valid square inputs, not outside the board.
    if ((i_start_row < 0) ||  (i_start_row >BOARDSIZE ))
        return INVALID_START;
    if ((i_start_col < 0 ) ||  (i_start_col >BOARDSIZE ))
        return INVALID_START;
        
    if ((i_end_row < 0 ) ||  (i_end_row >BOARDSIZE ))
        return INVALID_END;
    if ((i_end_col < 0 ) ||  (i_end_col >BOARDSIZE ))
        return INVALID_END;

    if ((i_end_col == i_start_col ) &&  (i_end_row == i_start_row )){
        cout << "Could not move to the same location" << endl;
        return INVALID_END;
    }
        

    // check matching piece at start square
    if (player == RED_TURN){ // RED player's turn
        if ((i_ctrl_piece == KING) || (i_ctrl_piece == GOLD)){
            if (p_Game->a_redPieces[i_ctrl_piece] == p_Game->s_board[i_start_row][i_start_col]){
                match_Start = true;
            }
        }else{
            if (p_Game->a_redPieces[i_ctrl_piece] == p_Game->s_board[i_start_row][i_start_col]){ //check for normal and promoted piece
                match_Start = true;
            }
            if (p_Game->a_redPieces[i_ctrl_piece + 6] == p_Game->s_board[i_start_row][i_start_col]){
                i_ctrl_piece = i_ctrl_piece + 6; // use this to know if we are controlling a promoted piece
                match_Start = true;
            }
        }
    }else{
        if ((i_ctrl_piece == KING) || (i_ctrl_piece == GOLD)){
            if (p_Game->a_bluePieces[i_ctrl_piece] == p_Game->s_board[i_start_row][i_start_col]){
                match_Start = true;
            }
        }else{
            if (p_Game->a_bluePieces[i_ctrl_piece] == p_Game->s_board[i_start_row][i_start_col]){ //check for normal and promoted piece
                match_Start = true;
            }
            if (p_Game->a_bluePieces[i_ctrl_piece + 6] == p_Game->s_board[i_start_row][i_start_col]){
                i_ctrl_piece = i_ctrl_piece + 6; // use this to know if we are controlling a promoted piece
                match_Start = true;
            }
        }
    }

    if (!match_Start)
        return INVALID_START;
    else{
        // here, i_ctrl_piece could detect promoted piece
        if (check_valid_move(i_ctrl_piece, player, i_start_row, i_start_col, i_end_row, i_end_col) == VALID_MOVE){
            // check if blocked in case of BISHOP, ROOK, BISHOP+6, ROOK+6.
            // at this stage, all destinations square are VALID_MOVE
            if ((i_ctrl_piece == BISHOP) || (i_ctrl_piece == ROOK) || (i_ctrl_piece == BISHOP+6) || (i_ctrl_piece == ROOK+6)){
                if (is_blocked(p_Game, i_ctrl_piece, i_start_row, i_start_col, i_end_row, i_end_col) == BLOCKED){
                    cout << "This move is blocked by some pieces" << endl;
                    return INVALID_END;
                }
            }

            if (p_Game->s_board[i_end_row][i_end_col] == EMPTY_SQUARE){ // empty destination location, move normally
                if (player == RED_TURN)
                    p_Game->s_board[i_end_row][i_end_col] = p_Game->a_redPieces[i_ctrl_piece];
                else
                    p_Game->s_board[i_end_row][i_end_col] = p_Game->a_bluePieces[i_ctrl_piece];
                p_Game->s_board[i_start_row][i_start_col] = EMPTY_SQUARE;
            } else{

                // check which piece is at the destination square
                for (red_search_var = 0; red_search_var < *(&(p_Game->a_redPieces) + 1)- p_Game->a_redPieces; red_search_var++){
                    if (p_Game->a_redPieces[red_search_var] == p_Game->s_board[i_end_row][i_end_col]){
                        i_target_piece = red_search_var;
                        b_target_color = RED;
                    } 
                }
                for (blue_search_var = 0; blue_search_var < *(&(p_Game->a_bluePieces) + 1)- p_Game->a_bluePieces; blue_search_var++){
                    if (p_Game->a_bluePieces[blue_search_var] == p_Game->s_board[i_end_row][i_end_col]){
                        i_target_piece = blue_search_var;
                        b_target_color = BLUE;
                    }
                }

                // if ally, return INVALID_END
                if (player == b_target_color){
                    cout << "Cannot capture ally\n";
                    return INVALID_END;
                }else{ // else, capture enemy piece
                    if (i_target_piece > PAWN)
                        i_target_piece -= 6; // because at this point i_target_piece encoded promotion so it needs to be reduce to identify which piece
                    // remove from number array, increase capture array
                    if (b_target_color == RED){
                        p_Game->a_rPieceNum[i_target_piece] -=1;
                        p_Game->a_rPieceCap[i_target_piece] +=1;
                    }else{
                        p_Game->a_bPieceNum[i_target_piece] -=1;
                        p_Game->a_bPieceCap[i_target_piece] +=1;
                    }
                    // mover command piece there
                    if (player == RED_TURN)
                        p_Game->s_board[i_end_row][i_end_col] = p_Game->a_redPieces[i_ctrl_piece];
                    else
                        p_Game->s_board[i_end_row][i_end_col] = p_Game->a_bluePieces[i_ctrl_piece];
                    p_Game->s_board[i_start_row][i_start_col] = EMPTY_SQUARE;
                }

                // implement promotion
                if ((i_ctrl_piece != KING) && (i_ctrl_piece != GOLD)){ // this i_target_piece does not encode promotion info anymore. Promotion does not apply to KING and GOLD
                    if (player == RED_TURN){
                        if (i_end_row >= 6){ // row G, the piece is promoted
                            p_Game->s_board[i_end_row][i_end_col] = p_Game->a_redPieces[i_ctrl_piece+6];
                            cout << "The piece " << p_Game->a_redPieces[i_ctrl_piece] << " was promoted to " << p_Game->a_redPieces[i_ctrl_piece+6] << endl;
                        }
                    }else{
                        if (i_end_row <= 2){ // row G, the piece is promoted
                            p_Game->s_board[i_end_row][i_end_col] = p_Game->a_bluePieces[i_ctrl_piece+6];
                            cout << "The piece " << p_Game->a_redPieces[i_ctrl_piece] << " was promoted to " << p_Game->a_redPieces[i_ctrl_piece+6] << endl;
                        }
                    }
                }
                
            }
        }else{
            cout << "Invalid move for this piece \n";
            return INVALID_END;
        }
    }
    return MOVE_SUCCESS;
}

///////////////////////// drop_piece() ///////////////////////////
// Purpose: check the legitimacy and check the command to drop a//
// piece.                                                       //
//////////////////////////////////////////////////////////////////
int drop_piece(SHOGI_GAME* p_Game){
    int i_drop_piece = encode_piece(p_Game->s_ctrl_piece);
    if (DEBUG){
        cout << "i_drop_piece " << i_drop_piece << endl;
        cout << "p_Game->s_ctrl_piece " << p_Game->s_ctrl_piece << endl;
    }
    bool player;
    if (p_Game->i_turns%2 == RED_TURN)
        player = RED_TURN;
    else
        player = BLUE_TURN;

    int i_drop_row = (int)p_Game->square1[0]-65; //Convertinting the char to int from ASCII
    int i_drop_col = (int)p_Game->square1[1]-49; //Convertinting the char to int from ASCII, starting from 0

    // check input name of the piece
    if (i_drop_piece == INVALID_NAME)
        return INVALID_PIECE;

    // check valid square inputs, not outside the board.
    if ((i_drop_row < 0) ||  (i_drop_row >BOARDSIZE ))
        return INVALID_DROP;
    if ((i_drop_col < 0 ) ||  (i_drop_col >BOARDSIZE ))
        return INVALID_DROP;

    // check if the drop square is an empty square, else 
    if (p_Game->s_board[i_drop_row][i_drop_col] == EMPTY_SQUARE){
        // check if the input piece is a captured piece
        if (player == RED_TURN){
            if(p_Game->a_rPieceCap[i_drop_piece] == 0){ //no captured piece to be collected
                cout << "No captured piece of this type\n";
                return INVALID_PIECE;
            }else{
                p_Game->a_rPieceNum[i_drop_piece] +=1;
                p_Game->a_rPieceCap[i_drop_piece] -=1;
                p_Game->s_board[i_drop_row][i_drop_col] = p_Game->a_redPieces[i_drop_piece];
            }
        }else{
            if(p_Game->a_bPieceCap[i_drop_piece] == 0){ //no captured piece to be collected
                cout << "No captured piece of this type\n";
                return INVALID_PIECE;
            }else{
                p_Game->a_bPieceNum[i_drop_piece] +=1;
                p_Game->a_bPieceCap[i_drop_piece] -=1;
                p_Game->s_board[i_drop_row][i_drop_col] = p_Game->a_bluePieces[i_drop_piece];
            }
        }
    }else{
        cout << "There's a piece at this location already\n";
        return INVALID_DROP;
    }

    return DROP_SUCCESS;
}

//////////////////////// encode_piece() //////////////////////////
// Purpose: convert the input string from user into processing- //
// friendly int values.                                         //
//////////////////////////////////////////////////////////////////
int encode_piece(string piece_name){
    if (piece_name == "KING")
        return KING;
    else if (piece_name == "GOLD")
        return GOLD;
    else if (piece_name == "SILVER")
        return SILVER;
    else if (piece_name == "KNIGHT")
        return KNIGHT;
    else if (piece_name == "LANCE")
        return LANCE;
    else if (piece_name == "BISHOP")
        return BISHOP;
    else if (piece_name == "ROOK")
        return ROOK;
    else if (piece_name == "PAWN")
        return PAWN;
    else
        return INVALID_NAME;
}

////////////////////// check_valid_move() ////////////////////////
// Purpose: Depending on the commanded piece, check if the      //
// destination square conforms with the piece's ability to move //
//////////////////////////////////////////////////////////////////
int check_valid_move(int piece, bool player, int start_row, int start_col, int end_row, int end_col){ 
    int direction = 1;
    if (player == RED_TURN)
            direction = 1;
    else if (player == BLUE_TURN)
            direction = -1;

    switch (piece)
    {
    case KING:
        if ((end_row <= start_row+1) && (end_row >= start_row+1) && (end_col <= start_col+1) && (end_col >= start_col+1))
            return VALID_MOVE;
        else
            return INVALID_MOVE;
        break;
    
    case GOLD: // GOLD piece and other promoted pieces that will behave like a GOLD
    case PAWN+6: 
    case SILVER+6: 
    case KNIGHT+6:
    case LANCE+6:
        if ((end_row <= start_row+1) && (end_row >= start_row+1) && (end_col <= start_col+1) && (end_col >= start_col+1))
            if ((end_row == start_row - direction*1) && ((end_col == start_col - 1) || (end_col == start_col + 1)))
                return INVALID_MOVE;
            else
                return VALID_MOVE;
        else
            return INVALID_MOVE;
        break;

    case SILVER:
        if ((end_row <= start_row+1) && (end_row >= start_row+1) && (end_col <= start_col+1) && (end_col >= start_col+1))
            if ((end_row == start_row - direction*1) && (end_col == start_col))
                return INVALID_MOVE;
            else if ((end_row == start_row) && ((end_col == start_col-1) || (end_col == start_col+1)))
                return INVALID_MOVE;
            else
                return VALID_MOVE;
        else
            return INVALID_MOVE;
        break;

    case KNIGHT:
        if ((end_row == start_row + direction*2) && ((end_col == start_col-1) || (end_col == start_col+1)))
            return VALID_MOVE;
        else
            return INVALID_MOVE;
        break;

    case LANCE: // need to check for piece(s) in between start and destination
        if (end_col == start_col)
            if ( direction*(end_row - start_row) > 0 )
                return VALID_MOVE;
            else
                return INVALID_MOVE;
        else
            return INVALID_MOVE;
        break;

    case PAWN:
        if ((end_row == start_row + direction*1) && (end_col == start_col))
            return VALID_MOVE;
        else
            return INVALID_MOVE;
        break;

    case BISHOP: // need to check for piece(s) in between start and destination
        if (abs(end_row - start_row) == abs(end_col - start_col))
            return VALID_MOVE;
        else
            return INVALID_MOVE;
        break;

    case ROOK: // need to check for piece(s) in between start and destination
        if ((end_row == start_row) || (end_col == start_col))
            return VALID_MOVE;
        else
            return INVALID_MOVE;
        break;

    case BISHOP+6: //promoted bishop // need to check for piece(s) in between start and destination
        if (((end_row <= start_row+1) && (end_row >= start_row+1) && (end_col <= start_col+1) && (end_col >= start_col+1))
         || (abs(end_row - start_row) == abs(end_col - start_col)))
            return VALID_MOVE;
        else
            return INVALID_MOVE;
        break;

    case ROOK+6: //promoted rook // need to check for piece(s) in between start and destination
        if (((end_row <= start_row+1) && (end_row >= start_row+1) && (end_col <= start_col+1) && (end_col >= start_col+1))
         || ((end_row == start_row) || (end_col == start_col)))
            return VALID_MOVE;
        else
            return INVALID_MOVE;
        break;
    }
    return VALID_MOVE;
}

////////////////////////// is_blocked() //////////////////////////
// Purpose: BISHOP, ROOK, BISHOP+6, ROOK+6 has large free range //
// so they need to be checked if there's any piece between their//
// start and destination square to confirm it's a legit move.   //
//////////////////////////////////////////////////////////////////
int is_blocked(SHOGI_GAME* p_Game, int piece, int start_row, int start_col, int end_row, int end_col){
    int loop_var_col = 0;
    int loop_var_row = 0;
    int incre_col = 1;
    int incre_row = 1;
    switch (piece)
    {
    case BISHOP:
    case BISHOP+6:
        if (end_col >= start_col)
            incre_col = 1;
        else
            incre_col = -1;

        if (end_row >= start_row)
            incre_row = 1;
        else
            incre_row = -1;

       loop_var_col = start_col+incre_col;
        loop_var_row = start_row+incre_row;
        while((loop_var_col != end_col) || (loop_var_row != end_row)){
            if (p_Game->s_board[loop_var_row][loop_var_col] != EMPTY_SQUARE)
                return BLOCKED;
            loop_var_col += incre_col;
            loop_var_row += incre_row;
        }
        break;
    
    case ROOK:
    case ROOK+6:
        if (end_col > start_col)
            incre_col = 1;
        else if (end_col < start_col)
            incre_col = -1;
        else
            incre_col = 0;

        if (end_row > start_row)
            incre_row = 1;
        else if (end_row < start_row)
            incre_row = -1;
        else
            incre_row = 0;

        loop_var_col = start_col+incre_col;
        loop_var_row = start_row+incre_row;
        while((loop_var_col != end_col) || (loop_var_row != end_row)){
            if (p_Game->s_board[loop_var_row][loop_var_col] != EMPTY_SQUARE)
                return BLOCKED;
            loop_var_col += incre_col;
            loop_var_row += incre_row;
        }
        break;
    }
    return NOT_BLOCKED;
}

/////////////////////////// results() ////////////////////////////
// Purpose                                                      //
//////////////////////////////////////////////////////////////////
void results(SHOGI_GAME* p_Game){
    int loop_var = 0;
    if ((p_Game->i_turns - 1)%2 == RED_TURN){
            cout << RED_PIECE << "Red player " << RESET_COLOR << "WINS!!! Using" << p_Game->i_turns - 1 << " turns" << endl;
        }else{
            cout << BLUE_PIECE << "Blue player " << RESET_COLOR << "WINS!!! Using" << p_Game->i_turns - 1 << " turns" << endl;
        }

    cout << "Capture pieces summary: \n";
    cout << RED_PIECE << "Red's captured pieces' " << RESET_COLOR << endl;
    cout << "|";
    for (loop_var = 0; loop_var < PAWN+1; loop_var++){
        cout << p_Game->a_redPieces[loop_var];
        cout << "|";
    }
    cout << endl;
    cout << "|";
    for (loop_var = 0; loop_var < PAWN+1; loop_var++){
        cout << " " << p_Game->a_rPieceCap[loop_var] << " ";
        cout << "|";
    }
    cout << endl;

    cout << BLUE_PIECE << "Blue's captured pieces' " << RESET_COLOR << endl;
    cout << "|";
    for (loop_var = 0; loop_var < PAWN+1; loop_var++){
        cout <<p_Game->a_bluePieces[loop_var];
        cout << "|";
    }
    cout << endl;
    cout << "|";
    for (loop_var = 0; loop_var < PAWN+1; loop_var++){
        cout << " " << p_Game->a_bPieceCap[loop_var] << " ";
        cout << "|";
    }
    cout << endl;
    
}

///////////////////////////// game() /////////////////////////////
// Purpose                                                      //
//////////////////////////////////////////////////////////////////
void game(){
    SHOGI_GAME* p_mygame = &mygame;
    cout << "******** Welcome to the SHOGI GAME ********\n";
    cout << "Here is the way to play the game. In ones' turn, start by selecting either moving a piece (m) or dropping a piece (d).\n";
    cout << "To move a piece, input when prompted with the follwoing  format: <PIECE> <start suare> <end square> (Exp: PAWN G1 F1).\n";
    cout << "To drop a piece, input when prompted with the follwoing  format: <PIECE> <location square> (Exp: PAWN E5).\n";
    cout << "<PIECE>'s keywords: \n - KING\n - GOLD\n - SILVER\n - KNIGHT\n - LANCE\n - BISHOP\n - ROOK\n - PAWN\n";
    cout << "\n *note: keywords are case-sensitive. \n ";
    cout << "The help message could be display from any prompted using: ""-h"" input.\n";
    cout << "To start the game, please enter players' names below: \n";
    cout << "Player 1's (" << RED_PIECE << "RED" << RESET_COLOR <<" ) name (Enter to confirm): ";
    getline (cin,mygame.s_p1_name);
    cout << "Player 2's (" << BLUE_PIECE << "BLUE" << RESET_COLOR <<" ) name (Enter to confirm): ";
    getline (cin,mygame.s_p2_name);

    initialization(p_mygame);
    display(p_mygame);

    while(p_mygame->a_rPieceNum[KING] * p_mygame->a_bPieceNum[KING] == 1){ // as long as both Kings alive
        // Game loop
        game_loop(p_mygame);
        cout << "Turn's result: \n";
        display(p_mygame);
        cout << "----------------------------- \n \n";
        
    }
    results(p_mygame);
}