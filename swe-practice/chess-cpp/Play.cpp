/**
   Akash Chaurasia - achaura1
   akashc@jhu.edu
*/  


#include <iostream>
#include "Prompts.h"
#include "Game.h"
#include "ChessGame.h"
#include "SpookyChess.h"
#include "KingoftheHill.h"

using std::cin;
using std::string;

// Game variant enumeration
enum GameName {STANDARD_CHESS = 1, KING_OF_THE_HILL, SPOOKY_CHESS};


// Ask user which game they want to play
int collect_game_choice() {
    Prompts::game_choice();
    int game_choice;
    cin >> game_choice;
    if (game_choice != 1 && game_choice != 2 && game_choice != 3){
      std::cout << "Invalid option(s) selected. Exiting the program. \n" << std::endl;
      return 0;
    }
    return game_choice;
}

// Ask user whether to launch new game or load existing one
int determine_new_or_load() {
    Prompts::new_or_load_choice();
    int new_or_load;
    cin >> new_or_load;
    if (new_or_load != 1 && new_or_load != 2) {
      std::cout << "Invalid option(s) selected. Exiting the program. \n" << std::endl;
      return 0;
    }
    return new_or_load;
}

// Ask user for name of file where game state is stored
string collect_filename() {
    Prompts::load_game();
    string f;
    cin >> f;
    return f;
}

int main() {

    // Determine which game to play, and how to begin it
    int game_choice = collect_game_choice();
    if (!game_choice){
      return 0;
    }
    int new_or_load_choice = determine_new_or_load();
    if (!new_or_load_choice) {
      return 0;
    }
    
    // Set up the desired game
    Game *g = nullptr;
    if (game_choice == STANDARD_CHESS && new_or_load_choice == 1) {
        g = new ChessGame();
    } else if (game_choice == STANDARD_CHESS && new_or_load_choice == 2) {
      string filename = collect_filename();
      g = new ChessGame(filename,1);
    } else if (game_choice == KING_OF_THE_HILL && new_or_load_choice == 1) {
      g = new KingoftheHill();
    } else if (game_choice == KING_OF_THE_HILL && new_or_load_choice == 2) {
      string filename = collect_filename();
      g = new KingoftheHill(filename, 2);
    } else if (game_choice == SPOOKY_CHESS && new_or_load_choice == 1) {
      g = new SpookyChess();
    } else if (game_choice == SPOOKY_CHESS && new_or_load_choice == 2) {
      string filename = collect_filename();
      g = new SpookyChess(filename, 3);
    }

    else {
      std::cout << "Invalid option(s) selected. Exiting the program. \n" << std::endl;
      return 1;
    }
    // Begin play of the selected game!
    g->run();
    
    // Nothing else to do here in main, so clean up
    delete g;

    return 0;
}

