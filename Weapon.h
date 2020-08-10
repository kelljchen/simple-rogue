
#ifndef WEAPON_INCLUDED
#define WEAPON_INCLUDED

#include "GameObject.h"
#include <iostream>
#include <string>
using namespace std;

class Weapon : public GameObject {
	public:
		Weapon(string name, int r, int c, int dexPts, int attackerPts) 
			: GameObject(name, r, c), damage(attackerPts), m_dexBonus(dexPts) {};

		// Accessor functions
		virtual char floor() { return ')'; };
		int getDexBonus() { return m_dexBonus; };
		int getDamagePts() { return damage; };

	private:
		int m_dexBonus;
		int damage;
};

class Mace : public Weapon {
	public:
		Mace(int r, int c) : Weapon("mace", r, c, 0, 2) {};
		virtual string action() { return "swings mace at"; };

};

class ShortSword : public Weapon {
	public:
		ShortSword(int r, int c) : Weapon("short sword", r, c, 0, 2) {};
		virtual string action() { return "slashes short sword at"; };
};

class LongSword : public Weapon {
	public:
		LongSword(int r, int c) : Weapon("long sword", r, c, 2, 4) {};
		virtual string action() { return "slashes long sword at"; };
};

class MagicAx : public Weapon {
	public:
		MagicAx(int r, int c) : Weapon("magic ax", r, c, 5, 5) {};
		virtual string action() { return "chops magic ax at"; };
};

class MagicFangs : public Weapon {
	public:
		MagicFangs(int r, int c) : Weapon("magic fangs of sleep", r, c, 3, 2) {};
		virtual string action() { return "strikes magic fangs at"; };
};

//In addition to the regular hit point damage magic fangs of sleep do when the attacker hits the defender, 
//there is a 1 in 5 chance that the magic fangs of sleep will put the defender to sleep.
//The number of moves the sleep will last(the sleep time) is a random integer from 2 to 6, call it X.
//If the defender is already asleep, with sleep time Y, then the defender's sleep time becomes the maximum of X and Y (and not, say, Y+X).

#endif
