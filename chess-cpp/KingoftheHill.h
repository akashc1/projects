/**
   # Akash Chaurasia - achaura1
   # akashc@jhu.edu
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
