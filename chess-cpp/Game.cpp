/**
   Akash Chaurasia - achaura1
   akashc@jhu.edu
*/  


#include <iostream>
#include <cassert>
#include <cctype>
#include "Terminal.h"
#include "Game.h"
#include "Prompts.h"
#include "Piece.h"
#include <string>
#include <sstream>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::istringstream;

Game::~Game() {

    // Delete the factories used to generate pieces
    for (size_t i = 0; i < _registered_factories.size(); i++) {
        delete _registered_factories[i];
    }

    // Delete any other dynamically-allocated resources here

    // Delete pieces on board
    for (unsigned int i = 0; i < (_width * _height); i++) {
      if (_pieces[i] != nullptr) {
	delete _pieces[i];
      }
    }

}

// Create a Piece on the board using the appropriate factory.
// Returns true if the piece was successfully placed on the board.
bool Game::init_piece(int piece_type, Player owner, Position pos) {
    Piece* piece = new_piece(piece_type, owner);
    if (!piece) return false;

    // Fail if the position is out of bounds
    if (!valid_position(pos)) {
        Prompts::out_of_bounds();
        return false;
    }

    // Fail if the position is occupied
    if (get_piece(pos)) {
        Prompts::blocked();
        return false;
    }
    _pieces[index(pos)] = piece;
    return true;
}

// Get the Piece at a specified Position.  Returns nullptr if no
// Piece at that Position or if Position is out of bounds.
Piece* Game::get_piece(Position pos) const {
    if (valid_position(pos))
      return _pieces[index(pos)];
    else {
        Prompts::out_of_bounds();
        return nullptr;
    }
}

char boardHelper(int type){
  if (type == 0){std::cout << "♙" << " ";}
  if (type == 1){std::cout << "♖" << " ";}
  if (type == 2){std::cout << "♘" << " ";}
  if (type == 3){std::cout << "♗" << " ";}
  if (type == 4){std::cout << "♕" << " ";}
  if (type == 5){std::cout << "♔" << " ";}
  if (type == 6){std::cout << "☭" << " ";}
  return '0';
}

void Game::run(){
}

void Game::save(string filename, int gameType) {
  (void)filename;
  (void)gameType;
}

//Draw the board
void Game::setup_board(){
  Terminal t;
  int flipper = 1;
  for (unsigned int i = 0; i < _height; i++) {
    std::cout <<(8-i) << " ";
    for (unsigned int j = 0; j < _width; j++) {

      if (flipper == 1) {
	t.color_bg(t.GREEN);
      } else {
	t.color_bg(t.RED);
      }
      Position location(j,i);
      if (get_piece(location) != NULL) {
	int p_type = get_piece(location)->piece_type();
	int p_own = get_piece(location)->owner();
	if (p_own == 0) {
	  t.color_fg(1, t.WHITE);
	} else if (p_own == 2) {
	  t.color_fg(1, t.BLUE);
	} else {
	  t.color_fg(0, t.BLACK);
	}

	boardHelper(p_type);
      } else {
	std::cout << "  ";
      } 
      t.set_default();
      flipper *= -1;
    }
    std::cout << "\n";
    flipper *= -1;
  }
    char col = 'a';
    std::cout<<" ";
    for (unsigned int z = 0; z < _width; z++) {
      std::cout << " ";
      std::cout << col;
      col++;
    }
  std::cout << "\n";
}


// Search the factories to find a factory that can translate
//`piece_type' into a Piece, and use it to create the Piece.
// Returns nullptr if factory not found.
Piece* Game::new_piece(int piece_type, Player owner) {

    PieceGenMap::iterator it = _registered_factories.find(piece_type);

    if (it == _registered_factories.end()) { // not found
        std::cout << "Piece type " << piece_type << " has no generator\n";
        return nullptr;
    } else {
        return it->second->new_piece(owner);
    }
}



// Add a factory to the Board to enable producing
// a certain type of piece. Returns whether factory
// was successfully added or not.
bool Game::add_factory(AbstractPieceFactory* piece_gen) {
    // Temporary piece to get the ID
    Piece* p = piece_gen->new_piece(WHITE);
    int piece_type = p->piece_type();
    delete p;

    PieceGenMap::iterator it = _registered_factories.find(piece_type);
    if (it == _registered_factories.end()) { // not found, so add it
        _registered_factories[piece_type] = piece_gen;
        return true;
    } else {
        std::cout << "Piece type " << piece_type << " already has a generator\n";
        return false;
    }
}

int Game::isBlocked(std::vector<Position> trajectory) {
  int isBlocked = 0;
  for (std::vector<Position>::iterator it = trajectory.begin();
                it != trajectory.end(); it++) {
    if (_pieces[index(*it)] != nullptr) {
      isBlocked = 1;
    }
  }
  return isBlocked;
}

int Game::in_check(Player pl) {

  // Get this player's king's position
  Position king_pos;

  for (unsigned int i = 0; i < (height() * width()); i++) {
    if (_pieces[i] != nullptr) {
      if (_pieces[i]->owner() == pl && _pieces[i]->piece_type() == KING_ENUM) {
	king_pos = Position(i % _width, i / _width);
      }
    }
  } 
  // iterate through pieces, and if they are an enemy piece,
  // check if it is possible to capture the king
  for (unsigned int i = 0; i < (height() * width()); i++) {

    // find all enemy pieces
    if (_pieces[i] != nullptr && _pieces[i]->owner() != pl
            && _pieces[i]->owner() != NO_ONE) {
      Position from_pos = Position(i % _width, i / _width);
      std::vector<Position> trajectory;

      //find all enemy pieces whose move shapes permit them to capture the king
      int can_capture = _pieces[i]->valid_move_shape(from_pos, king_pos, trajectory);
      if (can_capture > 0) {

	if (_pieces[i]->piece_type() == PAWN_ENUM) {
	  //	  if (can_capture == 6) {
	  return 1;
	    //}
	}
	
	if (!isBlocked(trajectory)) {
	  return 1;
	}
      }      
    }
  }

  // No pieces capture the king, return 0
  return 0;
}
