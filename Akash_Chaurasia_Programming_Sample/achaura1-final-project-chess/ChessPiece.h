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


#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include "Enumerations.h"
#include "Piece.h"
#include <vector>
#include "Game.h"
class Pawn : public Piece {

protected:
    friend PieceFactory<Pawn>;
    Pawn(Player owner, int piece_type) : Piece(owner, piece_type) {}

public:
    // Returns an integer representing move shape validity
    // where a value >= 0 means valid, < 0 means invalid.
    // also populates a vector of Positions with the trajectory
    // followed by the Piece from start to end
    virtual int valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const override;
};


class Rook : public Piece {

protected:
    friend PieceFactory<Rook>;
    friend class Queen;
    Rook(Player owner, int piece_type) : Piece(owner, piece_type) {}

public:
    // Returns an integer representing move shape validity
    // where a value >= 0 means valid, < 0 means invalid.
    // also populates a vector of Positions with the trajectory
    // followed by the Piece from start to end
    virtual int valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const override;};



class Knight : public Piece {

protected:
    friend PieceFactory<Knight>;
    Knight(Player owner, int piece_type) : Piece(owner, piece_type) {}

public:
    // Returns an integer representing move shape validity
    // where a value >= 0 means valid, < 0 means invalid.
    // also populates a vector of Positions with the trajectory
    // followed by the Piece from start to end
    virtual int valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const override;
};


class Bishop : public Piece {

protected:
    friend PieceFactory<Bishop>;
    friend class Queen;
    Bishop(Player owner, int piece_type) : Piece(owner, piece_type) {}

public:
    // Returns an integer representing move shape validity
    // where a value >= 0 means valid, < 0 means invalid.
    // also populates a vector of Positions with the trajectory
    // followed by the Piece from start to end
    virtual int valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const override;
};


class Queen : public Piece {

protected:
    friend PieceFactory<Queen>;
    Queen(Player owner, int piece_type) : Piece(owner, piece_type) {}

public:
    // Returns an integer representing move shape validity
    // where a value >= 0 means valid, < 0 means invalid.
    // also populates a vector of Positions with the trajectory
    // followed by the Piece from start to end
    virtual int valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const override;
};

class Ghost : public Piece {
protected:
  friend PieceFactory<Ghost>;
  Ghost(Player owner, int piece_type) : Piece(owner, piece_type) {}
public:
  virtual int valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const override;
};

class King : public Piece {

protected:
    friend PieceFactory<King>;
    King(Player owner, int piece_type) : Piece(owner, piece_type) {}

public:
    // Returns an integer representing move shape validity
    // where a value >= 0 means valid, < 0 means invalid.
    // also populates a vector of Positions with the trajectory
    // followed by the Piece from start to end
    virtual int valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const override;
};


#endif // CHESS_PIECE_H
