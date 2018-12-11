# Finn Banks - fbanks2
# Akash Chaurasia - achaura1
# Nikita Shtarkman - nshtark1
# 601.220
# December 8th 2017
# Final Project
# fbanks2@jhu.edu
# achaura1@jhu.edu
# nshtark1@jhu.edu

CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -g

play: Play.o Game.o ChessGame.o ChessPiece.o SpookyChess.o KingoftheHill.o
	$(CXX) Play.o Game.o ChessGame.o ChessPiece.o SpookyChess.o KingoftheHill.o -o play

Play.o: Play.cpp Game.h ChessGame.h Prompts.h SpookyChess.h KingoftheHill.h
	$(CXX) $(CXXFLAGS) -c Play.cpp

Game.o: Game.cpp Game.h Piece.h Prompts.h Enumerations.h Terminal.h
	$(CXX) $(CXXFLAGS) -c Game.cpp

ChessGame.o: ChessGame.cpp Game.h ChessGame.h Piece.h ChessPiece.h Prompts.h Enumerations.h
	$(CXX) $(CXXFLAGS) -c ChessGame.cpp

ChessPiece.o: ChessPiece.cpp ChessPiece.h Enumerations.h
	$(CXX) $(CXXFLAGS) -c ChessPiece.cpp

SpookyChess.o: SpookyChess.cpp SpookyChess.h Game.h ChessGame.h Piece.h ChessPiece.h Prompts.h Enumerations.h
	$(CXX) $(CXXFLAGS) -c SpookyChess.cpp

KingoftheHill.o: KingoftheHill.cpp KingoftheHill.h Game.h ChessGame.h Piece.h ChessPiece.h Prompts.h Enumerations.h
	$(CXX) $(CXXFLAGS) -c KingoftheHill.cpp


clean:
	rm *.o *~ play
