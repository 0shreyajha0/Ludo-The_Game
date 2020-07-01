#include <vector>
#include <mutex>
#include <thread>
#include "keywords.hpp"
#include "ludo_state.h"

	//To solve - "game doesn't name a type, even thogh game.h is included"
struct _moveData;
class game;
class ludo_state;

typedef std::pair<unsigned short, unsigned short> combination;

class thinker{
	/*Each 'combination' holds these - 
		0. index to the goti
		1. dieNum (*0 signifies unlock)*/
private:
	std::mutex thinker_mutex;	//! A common mutex for all threads

	game* original;
	bool bestMove_available = false;	//signifies, whether getBestMove() has been called earlier
	std::vector<combination> bestMove;
	std::map<int, std::vector<combination>> completeMoves;	//Mapping profit to the move, instead of move(a whole vector) to an int, to lesser the hash function's 'probable' overhead, also it will automatically chose the last found move with same profit, which would indead have been the case, even when we would have used
	ludo_state* state;	//The colour and player used will be of these
	ludo_coords _ludo_coords;

public:
	bool unlock();	//! Only friend 'thinker' can use these to modify the class
	inline bool move();
	inline bool operator()();	//Both, move(), and the () operator will call 'getBestMove'

	static inline direction getDirOfMovement( const coordinate& );
	const _moveData isMovePossible(const coordinate&, int dist) const;	//! This will use 'currColour from state only'

	//NOTE - A consideration that has been used is, that when any move is made by any colour, then its goti CANT BE REMOVED, so the indexes of gotis that will be in combination will be valid for the whole move, since neither their order will change being in a set*/
	bool setBestMove();	//Uses 'lots' of midlessMovers to gather moveProfit from each, then pick out the best ones
	std::vector<combination> getBestMove();
	bool mindlessMovers ( unsigned short roll, std::vector<unsigned short> dieNumbers, unsigned short gotiIndex, std::vector<coordinate> movingColoursPos, std::vector<coordinate> opponentsPos, std::pair<std::vector<combination>, int> prevMoves );
	bool implementBestMove();

	bool updateState(game*);
	thinker(game*);
	~thinker();
};