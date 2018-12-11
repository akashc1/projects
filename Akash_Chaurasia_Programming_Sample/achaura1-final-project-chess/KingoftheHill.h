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


#ifndef KING_OF_THE_HILL_H
#define KING_OF_THE_HILL_H

#include "Game.h"
#include "ChessPiece.h"
#include "ChessGame.h"
#include <sstream>


class KingoftheHill : public ChessGame {
 public:
 int game_over() override;
 KingoftheHill() : ChessGame() {gametype = 2;};
 KingoftheHill(std::string filename, int gameType) : ChessGame(filename, gameType) {gametype = 2;};
  
};

#endif
