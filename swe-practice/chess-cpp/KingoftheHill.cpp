/**
   # Akash Chaurasia - achaura1
   # akashc@jhu.edu
*/

#include "KingoftheHill.h"
#include "Prompts.h"

// The actual file...
int KingoftheHill::game_over() { 
  if (ChessGame::game_over()) {
    return 100;
  }
  Position points[] = {Position(3,4), Position(4,4), Position(4, 3), Position(3,3)}; 
  for (int i = 0; i < 4; i++) {
    if (get_piece(points[i])) {
      if (get_piece(points[i])->piece_type() == KING_ENUM) {
	Prompts::conquered(get_piece(points[i])->owner());
	Prompts::win(get_piece(points[i])->owner(), _turn);
	return 5;
      }
    }
    
  }
  return 0;
}
