// Game.cpp

#include "Game.h"
#include "utilities.h"
#include <iostream>
using namespace std;

// Implement these and other Game member functions you may have added.

Game::Game(int goblinSmellDistance) 
    : m_dungeon(new Dungeon(goblinSmellDistance))
{
}

Game::~Game() {
    delete m_dungeon;
}

void Game::play()
{
    m_dungeon->addPlayer();
    // Display initial screen
   // m_dungeon->display("");
    m_dungeon->display("");
    char move = 'a';
    while (move != 'q') {        
        // move player
        move = getCharacter();
        string msg = takeTurn(move);
        if (m_dungeon->checkDead()) { move = 'q'; }
        m_dungeon->display(msg);
    }

}

string Game::takeTurn(char move) {

    // Take character input
    char play = move;
    Player* player = m_dungeon->getPlayer();
    string msg = "";

    if (player->getSleepTime() == 0) {
        switch (play) {

        // Special commands
        case 'w':
            msg += m_dungeon->wieldWeapon();
            break;
        case 'r':
            msg += m_dungeon->readScroll();
            break;
        case 'i':
            m_dungeon->viewInventory();
            break;
        case 'g':
            msg += m_dungeon->pickUp();
            break;
        case '>':
            m_dungeon->setLevel(m_dungeon->getLevel()+1);
            break;
        case 'c':
            m_dungeon->cheat();
            break;
        }

        // Movement 
        if ((play == ARROW_DOWN) | (play == ARROW_UP) | (play == ARROW_LEFT) | (play == ARROW_RIGHT)) {
            msg+=m_dungeon->moveAttackPlayer(play);
        }
    }
    else {
        player->setSleepTime(player->getSleepTime() - 1);
    }

    m_dungeon->removeDead();

    msg+=m_dungeon->playMonsters(); 

    return msg;

}
