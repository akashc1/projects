/**
   Finn Banks - fbanks2
   Akash Chaurasia - achaura1
   Nikita Shtarkman - nshtark1
   601.220
   December 8th 2017
   Final Project
   fbanks2@jhu.edu
   achaura1@jhu.edu
   nshtark1@jhu.edu
*/


#ifndef SPOOKY_CHESS_H
#define SPOOKY_CHESS_H

#include "Game.h"
#include "ChessPiece.h"
#include "ChessGame.h"
#include <sstream>
#include <string>

class SpookyChess : public ChessGame {

public:
  
  SpookyChess();

 SpookyChess(std::string filename, int gameType);

  // Perform a move from the start Position to the end Position                                        
  // The method returns an integer with the status                                                     
  // >= 0 is SUCCESS, < 0 is failure                                                                   
  int make_move(Position start, Position end, int speak);
  void save(std::string filename, int gameType);
  // Reports whether the chess game is over                                                            
   void move_parsing(std::string thing);
   void ghost_mover() override;
 protected:

   int _numRandomed;
};

#endif // SPOOKY_CHESS_H 

