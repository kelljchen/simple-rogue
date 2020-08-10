
#ifndef DUNGEON_INCLUDED
#define DUNGEON_INCLUDED

#include "Actor.h"
#include "GameObject.h"
#include "globals.h"
#include <vector>
#include <string>
#include <stack>
using namespace std;


class Dungeon
{
	public:
		Dungeon(int goblinSmellDistance);
		~Dungeon();

		// Game mechanics (adding, removing characters)
		void setCellStatus(int r, int c, char status);
		char getCellStatus(int r, int c);
		void addPlayer();
		void addObject(int r, int c, int key);
		void addMonster(Coord pos, int key);
		void removeDead();
		bool checkDead();
		bool makeRoom();
		void makeMaze();
		void setLevel(int level);
		void display(string msg);

		//void goblinTest();

		// Player mechanics
		string moveAttackPlayer(char direction);
		string pickUp();
		string readScroll();
		string wieldWeapon();
		void cheat();
		char viewInventory();

		// NPC moves
		string playMonsters();
		void moveMonster(Actor* monster);
		int goblinSearch(char map[18][70], int r, int c, queue<Coord> path, int shortest);

		// Accessors
		int getLevel() const { return m_level; };
		Player* getPlayer() const { return m_Player; };

		//goblin stuff
		queue<Coord> getSteps() { return steps; };
		void setSteps(queue<Coord> newPath) { steps = newPath; };
		void clearSteps() {
			queue<Coord> cleared;
			steps = cleared;
		};

	private:
		// Characters
		Player* m_Player;
		vector<GameObject*> m_FloorItems;
		vector<Actor*> m_Monsters;
		vector<Room*> m_Rooms;

		// Items
		bool win;
		int goblinSmell;
		int num_monsters;
		int num_objects;
		int m_level;
		char maze[18][70];
		Coord* m_idol;
		Coord* m_staircase;
		queue<Coord> steps;

		// Helper Functions
		Coord openSpot();
		bool inRange(int range, Coord* playerPos, Coord* monsterPos);
		bool noActor(int r, int c);
		//bool noMonster(int r, int c);
		//bool noObject(int r, int c);
};

#endif
