// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include "Dungeon.h"

// You may add data members and other member functions to this class.

class Game
{
    public:
        Game(int goblinSmellDistance);
        ~Game();
        void play();
        string takeTurn(char move);
    private:
        Dungeon* m_dungeon;
};

#endif // GAME_INCLUDED
