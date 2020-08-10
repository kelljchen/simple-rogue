#include "Actor.h"


// Constructor
Player::Player() : Actor("Player", 20, 2, 2, 2, 20, 0, 0)
{
	setWeapon(new ShortSword(0, 0));
	inventory.push_back(getWeapon());
}

Player::~Player() {
	vector<GameObject*>::iterator obj = inventory.begin();

	for (int i = 1; i < inventory.size(); i++) {
		delete inventory[i];
	}
	inventory.clear();

}