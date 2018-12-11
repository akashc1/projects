/**
   # Finn Banks - fbanks2
   # Akash Chaurasia - achaura1
   # Nikita Shtarkman - nshtark1
   # 601.220
   # December 8th 2017
   # Final Project
   # fbanks2@jhu.edu
   # achaura1@jhu.edu
   # nshtark1@jhu.edu
*/

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "Game.h"
#include "ChessPiece.h"
#include <sstream>
#include <iostream>
#include <vector>
class ChessGame : public Game {

public:

    // Creates new game in standard start-of-game state 
    ChessGame();

    // Creates game with state indicated in specified file
    ChessGame(std::string filename, int gameType);
    ChessGame(const ChessGame &old);
    
    void save(std::string filename, int gameType);
    void run() override;

    // Perform a move from the start Position to the end Position
    // The method returns an integer with the status
    // >= 0 is SUCCESS, < 0 is failure
    int make_move(Position start, Position end, int speak) override;


    
    int checkHelper(Position start, Position end);
    // Reports whether the chess game is over
    int game_over()  override;

    bool inCheckMate(Player inCheck);

    int stalemate(Player pl);
    
protected:

    // Create all needed factories for the kinds of pieces
    // used in chess (doesn't make the actual pieces)
    virtual void initialize_factories();

    int _numRandomed;
    int gametype = 1;
};

#endif // CHESS_GAME_H
