
#ifndef ACTOR_INCLUDED
#define ACTOR_INCLUDED

#include <algorithm>
#include <string>
using namespace std;
#include "GameObject.h"
#include "utilities.h"
#include <queue>
#include "globals.h"
#include "Weapon.h"
#include <vector>

class Actor {

	public:
		
		// Constructor
		Actor(string name, int hp, int armor, int strength, int dex, int maxhp, int r=0, int c=0) 
			: m_name(name), m_hp(hp), m_armorPts(armor), m_strengthPts(strength), m_dexterity(dex), maxHP(maxhp), 
			m_sleepTime(0), position(new Coord(r, c)) , m_weapon(nullptr) {};
		virtual ~Actor() {
			delete position;
			if(m_weapon)
				delete m_weapon;
		};

		// Member functions
		virtual string attack(Actor* foe) {
			int attackPts = foe->getDexterity() + foe->getWeapon()->getDexBonus();
			int foePts = foe->getDexterity() + foe->getArmor();
			int damagePts = randInt(0, attackPts + m_weapon->getDamagePts() - 1);
			
			string msg = m_name + " " + m_weapon->action() + " " + foe->name();
			if (randInt(1, attackPts) >= randInt(1, foePts)) {
				foe->setHP(foe->getHP() - damagePts);
				if (foe->getHP() <=0) { msg += " dealing a final blow.\n"; }
				else if (m_weapon->name() == "magic fangs of sleep") {
					if (trueWithProbability(0.2)) {
						int time = randInt(2, 6);
						if (foe->getSleepTime() > 0) {
							foe->setSleepTime(max(time, foe->getSleepTime()));
						}
						else { foe->setSleepTime(time); }
					}
					msg += " and hits putting " + foe->name() + " to sleep.\n";
				}
				else { msg += " and hits.\n"; }
			}
			else {
				msg += " and misses.\n";
			}
			return msg;
		};

		virtual void move(int r, int c) {
			(*position).setPos(r, c);
		}

		// Accessor functions 
		string name() { return m_name; };
		Coord* getPos() { return position; };
		Weapon* getWeapon() { return m_weapon; };
		int getSleepTime() { return m_sleepTime; };
		int getHP() { return m_hp; };
		int getArmor() { return m_armorPts; };
		int getStrength() { return m_strengthPts; };
		int getDexterity() { return m_dexterity; };
		int getMaxHP() { return maxHP; };
		
		// Setter functions
		void setSleepTime(int time) { m_sleepTime = time; };
		void setWeapon(Weapon* newWeapon) { m_weapon = newWeapon; }; 
		void setHP(int hp) {
			if (hp <= maxHP) { m_hp = hp; }
		};
		void setMaxHP(int hp) { maxHP = hp; };
		void setArmor(int armor) { m_armorPts=armor; };
		void setStrength(int strength) { m_strengthPts=strength; };
		void setDexterity(int dex) { m_dexterity=dex; };

	private:
		string m_name;
		Coord* position;
		int m_hp;
		int maxHP;
		Weapon* m_weapon;
		int m_armorPts;
		int m_strengthPts;
		int m_dexterity;
		int m_sleepTime;
};

// Class Definition for Player
class Player : public Actor {
public:
	Player();
	virtual ~Player();

	vector<GameObject*> getInventory() { return inventory; };
	void addToInventory(GameObject* obj) { inventory.push_back(obj); };
	void removeFromInventory(int index) { 
		vector<GameObject*>::iterator item = inventory.begin();
		for (int i = 0; i != index; i++) {
			item++;
		};
		inventory.erase(item); 
	};

private:
	vector<GameObject*> inventory;
};


// Class Definitions and in-line implementation for Monsters
class Bogeyman : public Actor {
public:
	Bogeyman(int r, int c) : Actor("Bogeyman", 0, 2, randInt(2, 3), randInt(2, 3), randInt(5, 10), r, c) {
		setWeapon(new ShortSword(0, 0));
		setHP(getMaxHP());
	};
	~Bogeyman() {};
};

class SnakeWoman : public Actor {
public:
	SnakeWoman(int r, int c) : Actor("Snakewoman", 0, 3, 2, 3, randInt(3, 6), r, c) {
		setHP(getMaxHP());
		setWeapon(new MagicFangs(0, 0));
	};
	~SnakeWoman() {};

};

class Dragon : public Actor {
public:
	Dragon(int r, int c) : Actor("Dragon", 0, 4, 4, 4, randInt(20, 25), r, c) {
		setHP(getMaxHP());
		setWeapon(new LongSword(0, 0));
	}

};

class Goblin : public Actor {
	public:
		Goblin(int r, int c) : Actor("Goblin", 0, 1, 3, 1, randInt(15, 20), r, c) {
			setHP(getMaxHP());
			setWeapon(new ShortSword(0, 0));
		}
		/*queue<Coord> getSteps() { return steps; };
		void setSteps(queue<Coord> newPath) { steps = newPath; };*/
	private:
		//queue<Coord> steps;

};

#endif