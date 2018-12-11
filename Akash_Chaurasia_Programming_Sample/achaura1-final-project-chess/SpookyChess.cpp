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

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "Game.h"
#include "ChessGame.h"
#include "Prompts.h"
#include <sstream>
#include <fstream>
#include "SpookyChess.h"
using std::string;
using std::istringstream;
using std::cin;
using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;


SpookyChess::SpookyChess(){
  add_factory(new PieceFactory<Ghost>(GHOST_ENUM));
  // Creates a standard vector of all of the variations of the non pawn pieces                         
  std::vector<int> pieces {
    ROOK_ENUM, KNIGHT_ENUM, BISHOP_ENUM, QUEEN_ENUM,
      KING_ENUM, BISHOP_ENUM, KNIGHT_ENUM, ROOK_ENUM
      };
  srand(322);
  gametype = 3;
  _numRandomed = 0;
  // Loops through and places the the pawns and the other pieces on the board
  /*
  for (size_t i = 0; i < pieces.size(); ++i) {
    init_piece(PAWN_ENUM, WHITE, Position(i, 6));
    init_piece(pieces[i], WHITE, Position(i, 7));
    init_piece(pieces[i], BLACK, Position(i, 0));
    init_piece(PAWN_ENUM, BLACK, Position(i, 1));
  }
  */
  init_piece(GHOST_ENUM, NO_ONE, Position(0, 3));
}

// Saves the game

void SpookyChess::save(string filename, int gameType){
  ofstream newFile;
  string gamemode[] = {"filler", "chess\n", "king\n", "spooky\n"};
  char lookup[] = {'a','b','c','d','e','f','g','h'};
  newFile.open(filename);
  newFile << gamemode[gameType];
  newFile << turn() << endl;
  if(gameType == 3){
    newFile << _numRandomed << endl;
  }
  for (unsigned int i = 0; i < height(); i++){
    for (unsigned int j = 0; j < width(); j++){
      if (get_piece(Position(j,i)) != nullptr){
        newFile << (get_piece(Position(j,i)))->owner() << " ";
        newFile << lookup[j] << i + 1 << " ";
        newFile << (get_piece(Position(j,i)))->piece_type();
        newFile << endl;
      }
    }
  }
}

SpookyChess::SpookyChess(string filename, int gameType): ChessGame(filename, gameType){
  srand(322);
  int g;
  _numRandomed = 0;
  for(int i = 0; i < _numRandomed + 1; i++){
    g = rand()%64; 
  }
  gametype = 3;
  make_move(Position(0, 3), Position(g % _width, g / _width), 0);
}

int SpookyChess::make_move(Position start, Position end, int speak){
  return ChessGame::make_move(start,end, speak);  
}




void SpookyChess::ghost_mover(){
  //GHOST TEXT
  Position ghost_pos; 
  for (unsigned int i = 0; i < (height() * width()); i++) {
    if (_pieces[i] != nullptr) {
      if (_pieces[i]->owner() == NO_ONE && _pieces[i]->piece_type() == GHOST_ENUM) {
	ghost_pos = Position(i % _width, i / _width);
      }
    }
  }
  int g = rand()%64;
  while(make_move(Position(ghost_pos.x, ghost_pos.y), Position(g % _width, 7 - (g / _width)), 0) == 800){
    _numRandomed++;
    g = rand()%64;
  }
}
