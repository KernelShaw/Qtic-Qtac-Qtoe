#ifndef TICTACTOEAI_H
#define TICTACTOEAI_H

#include <QStringBuilder>
#include <iostream>

using namespace std;

bool isMovesLeft(QChar board[3][3])
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == '_')
                return true;
    return false;
}

int evaluate(QChar b[3][3], QChar ai, QChar pl){
    for (int row = 0; row < 3; row++)
    {
        if (b[row][0] == b[row][1] &&
            b[row][1] == b[row][2])
        {
            if (b[row][0] == ai)
                return +10;
            else if (b[row][0] == pl)
                return -10;
        }
    }

    for (int col = 0; col < 3; col++){
        if (b[0][col] == b[1][col] && b[1][col] == b[2][col]){
            if (b[0][col] == ai)
                return +10;
            else if (b[0][col] == pl)
                return -10;
        }
    }

    if (b[0][0] == b[1][1] && b[1][1] == b[2][2]){
        if (b[0][0] == ai)
            return +10;
        else if (b[0][0] == pl)
            return -10;
    }

    if (b[0][2] == b[1][1] && b[1][1] == b[2][0])
    {
        if (b[0][2] == ai)
            return +10;
        else if (b[0][2] == pl)
            return -10;
    }

    return 0;
}

int minimax(QChar board[3][3], int depth, bool isMax, QChar ai, QChar pl){
    int score = evaluate(board, ai, pl);

    if (score == 10)
        return score;

    if (score == -10)
        return score;

    if (isMovesLeft(board) == false)
        return 0;

    if (isMax) {
        int best = -1000;

        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                if (board[i][j] == '_'){
                    board[i][j] = ai;
                    best = max(best, minimax(board, depth+1, !isMax, ai, pl));
                    board[i][j] = '_';
                }
            }
        }
        return best;
    }

    else {
        int best = 1000;
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                if (board[i][j] == '_'){
                    board[i][j] = pl;
                    best = min(best, minimax(board, depth+1, !isMax, ai, pl));
                    board[i][j] = '_';
                }
            }
        }
        return best;
    }
}

int findBestMove(QChar board[3][3], QChar ai, QChar pl)
{
    int bestMove;
    int bestVal = -1000;

    for (int i = 0; i<3; i++){
        for (int j = 0; j<3; j++){
            if (board[i][j] == '_'){
                board[i][j] = ai;
                int moveVal = minimax(board, 0, false, ai, pl);
                board[i][j] = '_';

                if (moveVal > bestVal){
                    bestMove = (i * 3) + j;
                    bestVal = moveVal;
                }
            }
        }
    }

    return bestMove;
}

#endif // TICTACTOEAI_H
