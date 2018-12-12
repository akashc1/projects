/** 
   Akash Chaurasia - achaura1
   akashc@jhu.edu
*/  

//This is where the Chess Pieces get functions written for them
#include "ChessPiece.h"
#include "Enumerations.h"
#include <vector>
#include <iostream>

// will need to add more stuff here since a pawn can move 2 pieces
// as its first move and also can move diagonally to capture a piece
int Pawn::valid_move_shape(Position start, Position end,
                                std::vector<Position>& trajectory) const{
  // Pawns can only go forward so have to get the direction
  // that they should move in

  if (_owner == WHITE) {
    if (start.y == 6) {
      if (end.y == 5) {
	if (start.x == end.x) {
	  return SUCCESS;
	} else if (abs((int)(start.x - end.x)) == 1) {
	  return 6;
	}
      } else if (end.y == 4) {
	trajectory.push_back(Position(start.x, 5));
	return SUCCESS;
      }
    } else {
      if ((int)(end.y - start.y) == -1  && start.x == end.x) {
	return SUCCESS;
      } else if ((int)(end.y - start.y) == -1 && abs((int)(start.x - end.x)) == 1) {
	return 6;
      }
    }
  }

  if (_owner == BLACK) {
    if (start.y == 1) {
      if (end.y == 2 && start.x == end.x) {
	return SUCCESS;
      } else if (end.y == 2 && abs((int)(start.x - end.x)) == 1) {
	return 6;
      } else if (end.y == 3 && start.x == end.x) {
	trajectory.push_back(Position(start.x, 2));
	return SUCCESS;
      }
    } else {
      if (end.y - start.y == 1  && start.x == end.x) { 
	return SUCCESS;
      } else if (end.y - start.y == 1 && abs((int)(start.x-end.x)) == 1) {
	return 6;
      }
    }
  }
  
  return 0;
}

// check if the given move is valid for a rook
int Rook::valid_move_shape(Position start, Position end,
                                std::vector<Position>& trajectory) const {
  // Two possibilites for rook movement, either the X coordinate stays the same
  // or the Y coordinate stays the same
  if (start.x == end.x) {
    // Get the direction that we should be checking in
    // if have to look up the board, iteration must be negative
    int add = 1;
    if (start.y > end.y) {
      add = -1;
    }
    // Iterate along the distance between the two positions
    // push back each position and return success
    for(int i = 1; i < abs((int)(end.y - start.y)); i++) {
      trajectory.push_back(Position(start.x, start.y + add*i));
    }
    return SUCCESS;
  } else if (start.y == end.y) {
    // Get the direction that we should be checking in
    // if have to look up the board, iteration must be negative
    int add = 1;
    if (start.x > end.x) {
      add = -1;
    }
    // Iterate along the distance between the two positions
    // push back each position and return success
    for(int i = 1; i < abs((int)(end.x - start.x)); i++) {
      trajectory.push_back(Position(start.x + add*i, start.y));
    }
    return SUCCESS;
  }  else {
    // Everything failed! return 0...
    return 0;
  }
}

// Check withether the given move is valid for the ghost piece
int Ghost::valid_move_shape(Position start, Position end,
                                std::vector<Position>& trajectory) const {
  // No ability to see the board and what is occupied
  // Thus, must do this step outside of this
  (void)start;
  (void)end;
  (void)trajectory;
  return SUCCESS;
}

// check whether the given move is valid for a knight
int Knight::valid_move_shape(Position start, Position end,
                                std::vector<Position>& trajectory) const {
  // A knight does not worry about trajectory
  // since it can hop over pieces - no need to fill the vector
  (void)trajectory;
  if ((abs((int)(end.y-start.y)) == 1 && abs((int)(end.x-start.x)) == 2)
          || (abs((int)(end.y-start.y)) == 2 && abs((int)(end.x-start.x)) == 1)) {

    return SUCCESS;
  }
  return 0;
}

// check whether the given move is valid for a bishop
int Bishop::valid_move_shape(Position start, Position end,
                                std::vector<Position>& trajectory) const {
  // Check for purely diagonal movement
  if (abs((int)(end.x - start.x)) == abs((int)(end.y - start.y))) {
    // Getting 2 directions, since bishops have 2D movement
    // if looking up and/or left on the board, iteration must be negative
    int add_x = 1, add_y = 1;
    if (start.x > end.x) {
      add_x *= -1;
    }
    if (start.y > end.y) {
      add_y *= -1;
    }
    //Iterate between two positions and push back each one in vector
    for (int i = 1; i < abs((int)(start.x - end.x)); i++) {
      trajectory.push_back(Position(start.x + add_x * i, start.y + add_y * i));
    }    
    return SUCCESS;
    
  } else {
    //Movement not diagonal, return 0
    return 0;
  }
}

// Queen moves like a bishop or rook, so using those functions to our advantage here
int Queen::valid_move_shape(Position start, Position end,
                                std::vector<Position>& trajectory) const {

  if (abs((int)(end.x - start.x)) == abs((int)(end.y - start.y))) {
    // use the bishop's valid_move_shape function
    Bishop b(WHITE, BISHOP_ENUM);
    return b.valid_move_shape(start, end, trajectory);
  }
  if ((start.x == end.x) || (start.y == end.y)) {
    // this is rook-like movement, so use the rook's valid_move_shape function
    Rook r(WHITE, ROOK_ENUM);
    return r.valid_move_shape(start, end, trajectory);
  }
  //move shape is not valid, return 0
  return 0;
}

// The king can only move one space in any direction
// no need to populate the trajectory vector
// since inappropriately occupied spots are checked elsewhere
int King::valid_move_shape(Position start, Position end,
                                std::vector<Position>& trajectory) const {
  (void)trajectory;
  if ((start.x == end.x) && (abs((int)(start.y - end.y)) == 1)) {
    return SUCCESS;
  } else if ((start.y == end.y) && (abs((int)(start.x - end.x)) == 1)) {
    return SUCCESS;
  } else if ((abs((int)(start.x - end.x)) == 1) && (abs((int)(start.y - end.y) == 1))) {
    return SUCCESS;
  }
  //moves more than one space (which is invalid), return 0
  return 0;
}
