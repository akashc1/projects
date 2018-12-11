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
#include <strings.h>
#include <cassert>
using std::string;
using std::istringstream;
using std::cin;
using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;

// Set up the chess board with standard initial pieces
ChessGame::ChessGame(): Game() {
    initialize_factories();
    // Creates a standard vector of all of the variations of the non pawn pieces
    std::vector<int> pieces {
        ROOK_ENUM, KNIGHT_ENUM, BISHOP_ENUM, QUEEN_ENUM,
        KING_ENUM, BISHOP_ENUM, KNIGHT_ENUM, ROOK_ENUM
    };
    // Loops through and places the the pawns and the other pieces on the board
    for (size_t i = 0; i < pieces.size(); ++i) {
        init_piece(PAWN_ENUM, WHITE, Position(i, 6));
        init_piece(pieces[i], WHITE, Position(i, 7));
        init_piece(pieces[i], BLACK, Position(i, 0));
        init_piece(PAWN_ENUM, BLACK, Position(i, 1));
    }
}

// Helper function for save and load that looks up a letter and returns its corresponding array value
Position getPosition(string input){
  char lookup[] = {'a','b','c','d','e','f','g','h'};
  int i = 0;
  while (lookup[i] != input.front()){
    i++;
  }
  // Method of turning a character into a number
  int number = input.back() - '0';
  return Position(i,number - 1);
}


// Load function, able to load in given a game type and an input file, (decided to include game type because it was already decided at that point
ChessGame::ChessGame(string filename, int gameType): Game(){

  // Initial loading game setup
  string gamemode[] = {"filler", "chess", "king", "spooky"};

  // Opens inFile
  ifstream inFile;
  inFile.open(filename);
  if (!inFile){
    Prompts::load_failure();
    assert(inFile);
  } else {

    // Double checks that input gamemode is the same as the save file gamemode
    string temp;
    inFile >> temp;
    if (temp != gamemode[gameType]){
      Prompts::load_failure();
      assert(temp == gamemode[gameType]);
    } else {
      
      // Input individual variables into the game
      inFile >> _turn;
      if (gameType == 3){
	add_factory(new PieceFactory<Ghost>(GHOST_ENUM));
	
	inFile >> _numRandomed;
      }
      inFile.ignore();
      
      //Creates the pieces where they should be
      initialize_factories();
      string line;
      
      // Loop through and init the pieces from the save file
      while (std::getline(inFile,line)){
	istringstream iss(line);
	int player, piece;
	iss >> player;
	string position;
	iss >> position;
	iss >> piece;
	// Different initation for different players
	if (player == 1){
	  init_piece(piece,BLACK,getPosition(position));
	}
	else if (player == 0){
	  init_piece(piece,WHITE,getPosition(position));
	}
	else {
	  init_piece(piece,NO_ONE, getPosition(position));
	}
      }
    }
  }
}

// Helper function that is used in makemove, returns 0 if there is no check, 1 if they are moving into a check, and 2 if they are still in check after the move
int ChessGame::checkHelper(Position start, Position end){
  int result = 0;
  Piece* begin = get_piece(start);
  Piece* ending = get_piece(end);
  int type = -1;
  Player pl;
  // Save the piece that will get taken
  if (ending){
    type = ending->piece_type();
    pl = ending->owner();
  }
  // Make the move, determine if it is in check again
  if (in_check(player_turn())){
    make_move(start,end, 0);
    if (in_check(player_turn())){
      result = 2;
    }
  }
  // Still make the move, see if they are moving into check
  else {
    make_move(start,end,0);
    if (in_check(player_turn())){
      result = 1;
    }
  }
  // Reinstate the used piece
  _pieces[index(start)] = begin;
  _pieces[index(end)] = nullptr;
  if (type != -1){
    init_piece(type, pl, end);
  }
  return result;
}

// Perform a move from the start Position to the end Position
// The method returns an integer with the status           
// >= 0 is SUCCESS, < 0 is failure
int ChessGame::make_move(Position start, Position end, int speak) {
  Piece *to_move = get_piece(start);
  //If there is no piece there - check and make sure so that there isn't a segmentation fault
  if(to_move == NULL){
    if (speak) {Prompts::no_piece();}
    return -5;
  }
  if (start.x == end.x && start.y == end.y){
    if (speak) {Prompts::illegal_move();}
    return -5;
  }
  // Ghost chess functionality, just checks if the piece is the ghost piece
  if(to_move->owner() == NO_ONE){
    if(get_piece(end) != NULL){
      if(get_piece(end)->piece_type() == KING_ENUM){
	return 800;
      } else {
	delete get_piece(end);
	Prompts::ghost_capture();
      }
    }
    _pieces[index(end)] = to_move;
    _pieces[index(start)] = nullptr;
   return SUCCESS;
  }
  
  //The vector of trajectories
  std::vector<Position> trajectory;

  //If the move shape is invalid, return immediately, if not check other parameters
  if(!to_move->valid_move_shape(start, end, trajectory)){
    if (speak) {Prompts::illegal_move();}
    return -5;
  } else {
    
    //Check if the piece is owned by the player about to move it - if not return 0.
    if (to_move->owner() != player_turn()) {
      if (speak) {Prompts::no_piece();}
      return -5;
    }
    
    //Go through the trajectories and see if blocked on the way to the piece
    for(std::vector<Position>::iterator it = trajectory.begin(); it != trajectory.end(); it++){
      if(get_piece(*it) != NULL){
	if (speak) {Prompts::blocked();}
	return -5;
      }
    }

    // Checks if it is a pawn trying to move diagonally, and sorts through the inputs as such
    if(to_move->piece_type() == PAWN_ENUM){
      if(to_move->valid_move_shape(start, end, trajectory) == 6){
	if(get_piece(end) == nullptr){
	  if (speak) {Prompts::illegal_move();}
	  return -5;
	}
      } else {
	if(get_piece(end) != nullptr){
	  if(speak) { Prompts::blocked(); }
	  return -5;
	}
      }
    }
    
    //Check if the end piece is owned by the player - if yes then you are blocked, if not then capture it.
    bool occupied = false;
    if (get_piece(end) != nullptr) {
      occupied = true;
      Piece *end_piece = get_piece(end);
      if (to_move->owner() == end_piece->owner() || end_piece->owner() == NO_ONE) {
	if (speak) {Prompts::blocked();}
	return -5;
      } 
    }

    // Testing for check conditions
    if (speak){
      int temp = checkHelper(start,end);
      if (temp == 2){
	Prompts::must_handle_check();
	return -5;
      }
      else if (temp == 1){
	Prompts::cannot_expose_check();
	return -5;
      }
    }
    
    // Moving to a occupied space, but it is a viable move
    if (occupied){
      if(speak){Prompts::capture(to_move->owner());}
      delete get_piece(end);
    }

    // Handles if a pawn reaches the end and turns into a queen
    if(speak && to_move->piece_type() == PAWN_ENUM){

      if(to_move->owner() == WHITE && end.y == 0){
	delete get_piece(start);
	to_move = new_piece(QUEEN_ENUM, WHITE);
      }
      
      if(to_move->owner() == BLACK && end.y == 7){
	delete get_piece(start);
	to_move = new_piece(QUEEN_ENUM, BLACK);
      } 
    }
    
    _pieces[index(end)] = to_move;
    _pieces[index(start)] = nullptr;
    return SUCCESS;
  }
}


// Saves the game
void ChessGame::save(string filename, int gameType){
  ofstream newFile;
  string gamemode[] = {"filler", "chess\n", "king\n", "spooky\n"};
  char lookup[] = {'a','b','c','d','e','f','g','h'};
  newFile.open(filename);
  newFile << gamemode[gameType];
  newFile << turn() << endl;
  if(gameType == 3){
    newFile << _numRandomed <<endl;
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

// Function that returns true if in checkmate, false if there is a single move that isnt in check
bool ChessGame::inCheckMate(Player pl){
  for (unsigned int i = 0; i < _width; i++){
    for (unsigned int j = 0; j < _height; j++){
      // Gets the piece at a certain position
      Piece* temp = get_piece(Position(i,j));
      if (temp != nullptr && temp->owner() == player_turn()){
	for (unsigned int k = 0; k < _width; k++){
	  for (unsigned int l = 0; l <_height; l++){
	    std::vector<Position> tempTraj;
	    // Checks to see if it is a valid move
	    if (temp->valid_move_shape(Position(i,j),Position(k,l),tempTraj)){
	      if (!isBlocked(tempTraj)){
		Piece* tempEnd = get_piece(Position(k,l));
		int type = -1;
		bool works = false;
		Player player;
		if (tempEnd){
		  type = tempEnd->piece_type();
		  player = tempEnd->owner();
		}

		// Checks to make sure move is valid
		if (make_move(Position(i,j), Position(k,l), 0) > 0){
		  if (!in_check(pl)){
		    works = true;
		  }
		  
		  _pieces[index(Position(i,j))] = temp;
		  _pieces[index(Position(k,l))] = nullptr;
		  //		  delete tempEnd;
		  if (type != -1){
		    init_piece(type, player, Position(k,l));
		  }
		  if (works){
		    return false;
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
  return true;
}

//checks if the game is currently in stalemate
int ChessGame::stalemate(Player pl) {
  
  for (unsigned int i = 0; i < _width; i++) {
    for (unsigned int j = 0; j < _height; j++) {

      // Gets the piece at a certain position
      Piece* temp = get_piece(Position(i, j));

      // If real piece that belongs to current player, continue
      if (temp != nullptr && temp->owner() == player_turn()) {
	// Iterate through board to see eligible moves
	for (unsigned int k = 0; k < _width; k++) {
          for (unsigned int l = 0; l <_height; l++) {

	    std::vector<Position> tempTraj;

	    // Checks to see if it is a valid move
            if (temp->valid_move_shape(Position(i, j), Position(k, l), tempTraj)) {
	      // Make sure the move isn't blocked by other pieces
	      int isBlocked = 0;
	      for (std::vector<Position>::iterator it = tempTraj.begin(); it != tempTraj.end(); it++) {
		if (get_piece(*it) != nullptr) {
		  isBlocked = 1;
                }
              }

	      if (!isBlocked) {
		// Check to make sure this move doesn't put the player in check
		Piece* tempEnd = get_piece(Position(k, l));
		int type = -1;
                bool works = false;
                Player player;

		// Store info about end piece to replace after checking move
		if (tempEnd) {
                  type = tempEnd->piece_type();
                  player = tempEnd->owner();
                }

		// Make move and check if it puts player in check
		if (make_move(Position(i, j), Position(k, l), 0) > 0){

		  if (!in_check(pl)) {
		    works = true;
		  }
		  //		  delete tempEnd;
		// Undo move, reinitializing original end piece if necessary
		  _pieces[index(Position(i, j))] = temp;
		  _pieces[index(Position(k, l))] = nullptr;
		  
		  if (type != -1) {
		    init_piece(type, player, Position(k, l));
		  }
		  
		  //return no stalemate if move available
		  if (works) {
		    return 0;
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
  
  return 1;                                                                                                                                                                                                     
}             

void ChessGame::run(){
  int running = 1;
  //IGNORE THE NEWLINE FROM USING CIN>>
  cin.ignore();
  //A standin for a boolean turning the board on and off - 1 is on, -1 is off
  int board = -1;
    
  while(running){
    //NEED TO TAKE INPUT HERE - BOTH MOVE AND NON-MOVE INPUT
    //Check if board is turned on or off
    if(board == 1){
      setup_board();
    }
    int gameTemp = game_over();
    if(gameTemp){
      if (gameTemp == 1){
	_turn++;
	Prompts::checkmate(player_turn());
	Prompts::win(player_turn(), _turn -1);
	_turn--;
      }
      else if (gameTemp == 2){
	Prompts::stalemate();
      }
      Prompts::game_over();
      break;
    }
    //Prompt the player for a move
    Prompts::player_prompt(player_turn(), (_turn) + 1);
    
    // Collects the user input
    string in  = "";    
    getline(cin, in);
    
    if(in.length() >= 4 && isdigit(in[1]) && isdigit(in[4])){
      //Put the move parsing here
      if (move_parsing(in) >= 0){
	_turn--;
	if (in_check(player_turn())){
	  if (!inCheckMate(player_turn())){
	    _turn--;
	    Prompts::check(player_turn());
	    _turn++;
	  }
	}
	_turn++;
	_turn++;
      }
    } else {      
      //Put the non-move parsing here
      if(non_move_parsing(in) == 0){
	running = 0;
	return;
      }
      //Check if the board is turned on or off
      else if(non_move_parsing(in) == 1){
	board *= -1;
      }
      // Save game option
      else if(non_move_parsing(in) == 2){
	Prompts::save_game();
	string filename;
	cin >> filename;
	save(filename,gametype);
	return;
      } else {
	Prompts::parse_error();
      }
    }
  }
}

// Uses user input (that is 2 words long) and turns it into a move, collecting the information that the user gave and assigning those values to positions
int Game::move_parsing(string entry){
  istringstream iss;
  string first_pos_string, second_pos_string;
  iss.str(entry);
  iss >> first_pos_string >> second_pos_string;
  std::stringstream first_pos(first_pos_string.substr(1));
  std::stringstream second_pos(second_pos_string.substr(1));
  int first_y;
  first_pos >> first_y;
  int second_y;
  second_pos >> second_y;
  Position pos1(((int)(first_pos_string[0] - 'a')), 8-first_y);
  Position pos2(((int)(second_pos_string[0] - 'a')), 8-second_y);

  
  // Checks to make sure both areas are valid positions to place/have pieces
 if (valid_position(pos1) && valid_position(pos2)) {
   if(get_piece(pos1) != NULL && get_piece(pos1)->owner() == NO_ONE){
     Prompts::no_piece();
     return -5;
   }
   if(make_move(pos1, pos2,1) >= 0){
     // Only runs if it is spooky chess
     ghost_mover();
     return SUCCESS;
   } else {
     return -5;
   }
  } else {
    Prompts::out_of_bounds();
    return -5;
 }
}

int Game::non_move_parsing(string entry){
  if(strcasecmp("q", entry.c_str()) == 0){
    return 0;
  } else if(strcasecmp("board", entry.c_str()) == 0){
    return 1;
  } else if(strcasecmp("forfeit", entry.c_str()) == 0){
    _turn++;
    Prompts::win(player_turn(), _turn);
    Prompts::game_over();
    return 0;
  } else if(strcasecmp("save", entry.c_str()) == 0){
    return 2;
  } else {
    return -1;
  }
}


// Report whether the chess game is over                                                    
int ChessGame::game_over() {
  if(in_check(player_turn())){
      if(inCheckMate(player_turn())){
	return 1;
      } else {
	return 0;
      }
  } else if (stalemate(player_turn())){
    return 2;
  }
  return 0; 
}


// Prepare the game to create pieces to put on the board
void ChessGame::initialize_factories() {

    // Add all factories needed to create Piece subclasses
    add_factory(new PieceFactory<Pawn>(PAWN_ENUM));
    add_factory(new PieceFactory<Rook>(ROOK_ENUM));
    add_factory(new PieceFactory<Knight>(KNIGHT_ENUM));
    add_factory(new PieceFactory<Bishop>(BISHOP_ENUM));
    add_factory(new PieceFactory<Queen>(QUEEN_ENUM));
    add_factory(new PieceFactory<King>(KING_ENUM));
 }




