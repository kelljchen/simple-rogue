// Dungeon.cpp

#include <iostream>
#include <cstring>
#include <queue>
using namespace std;
#include "Actor.h"
#include "Dungeon.h"
#include "Scroll.h"

// Constructor
// Sets up first level
Dungeon::Dungeon(int GoblinSmellDistance)
	: m_level(0), win(false), goblinSmell(GoblinSmellDistance)
{
	// Set player
	m_Player = nullptr;
	m_idol = nullptr;

	// Set up initial maze
	makeMaze();

	// Add objects
	num_objects = randInt(2, 3);
	for (int i = 0; i < num_objects; i++) {
		int key = randInt(1, 7);
		Coord spot = openSpot();
		addObject(spot.r(), spot.c(), key);
	}

	// Add monsters
	num_monsters = randInt(2, 5 * (m_level + 1) + 1);
	for (int i = 0; i < num_monsters; i++) {

		int key;
		// levels 2-3, bogeymen, goblins, and snakewomen
		if ((m_level >= 2) & (m_level < 3)) { key = randInt(1, 3); }
		// level 3+, all
		else if (m_level >= 3) { key = randInt(1, 4); }
		// level 0, 1 goblin and snakewomen
		else { key = randInt(1, 2); }

		Coord spot = openSpot();
		addMonster(spot, key);
	}

	// Add the staircase OR the idol
	if (m_level == 4) {
		Coord pos = openSpot();
		setCellStatus(pos.r(), pos.c(), '&');
		m_idol = new Coord(pos.r(), pos.c());
	}
	else {
		Coord pos = openSpot();
		setCellStatus(pos.r(), pos.c(), '>');
		m_staircase = new Coord(pos.r(), pos.c());
	}
}

// Destructor
Dungeon::~Dungeon() {

	// Free Player
	delete m_Player;

	// Free idol
	delete m_idol;
	
	// Free staircase
	delete m_staircase;

	// Free GameObject Items
	vector<GameObject*>::iterator obj = m_FloorItems.begin();
	while (obj != m_FloorItems.end()) {
		delete* obj;
		obj++;
	}
	m_FloorItems.clear();

	// Free Monsters
	vector<Actor*>::iterator mon = m_Monsters.begin();
	while (mon != m_Monsters.end()) {
		delete* mon;
		mon++;
	}
	m_Monsters.clear();

	// Free Rooms
	vector<Room*>::iterator room = m_Rooms.begin();
	while (room != m_Rooms.end()) {
		delete* room;
		room++;
	}
	m_Rooms.clear();
}

bool Dungeon::makeRoom() {

	bool addedRoom = true;
	int r_length = randInt(3, 6);
	int c_length = randInt(10, 15);
	int r_start = randInt(1, 16);
	int c_start = randInt(1, 68);

	// Make sure that the generated room fall within grid
	while (((c_start + c_length) > 68) | ((r_start + r_length) > 16)) {
		r_start = randInt(1, 18);
		c_start = randInt(1, 70);
	}

	// Make sure all generated rooms aren't overlapping
	for (int i = r_start-1; i < r_start + r_length + 1; i++) {
		for (int j = c_start-1; j < c_start + c_length+1; j++) {
			if (getCellStatus(i, j) == ' ') { addedRoom = false; }
		}
	}

	// Make room
	if (addedRoom) {
		//cout << "added room" << endl;
		for (int i = r_start; i < r_start + r_length; i++) {
			for (int j = c_start; j < c_start + c_length; j++) {
				setCellStatus(i, j, ' ');
			}
		}
		m_Rooms.push_back(new Room(r_start, c_start, r_length, c_length));
	}

	 //just for testing
	//clearScreen();
	/*for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 70; j++) {
			cout << maze[i][j];
		}
		cout << endl;
	}*/

	return addedRoom;
}

void Dungeon::makeMaze() {

	// Clear last level, generate new blank map
	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 70; j++) {
			setCellStatus(i, j, '#');
		}
	}

	int num_room;
	if(m_level < 2) { num_room = randInt(4, 6); }
	else { num_room = randInt(5, 6); }

	int count = 0;

	// Add rooms
	while(count!=num_room) {
		if (makeRoom()) {
			count++;
		}
	}

	/*for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 70; j++) {
			cout << maze[i][j];
		}
		cout << endl;
	}*/

	

	// Add corridors
	vector<Room*>::iterator rooms = m_Rooms.begin();
	int cor = 0;
	for (int i = 0; i < num_room-1; i++) {
		int midpt1_r = m_Rooms[i]->getCenter()->r();
		int midpt1_c = m_Rooms[i]->getCenter()->c();
		int midpt2_r = m_Rooms[i+1]->getCenter()->r();
		int midpt2_c = m_Rooms[i+1]->getCenter()->c();

		// Room 1 is above room 2
		if (midpt1_r < midpt2_r) {
			// AND TO THE LEFT
			if (midpt1_c < midpt2_c) {
				// dig down and to right
				//cout << 8 << endl;
				for (int r = midpt1_r; r <= midpt2_r; r++) {
					setCellStatus(r, midpt1_c, ' ');
					/*for (int i = 0; i < 18; i++) {
						for (int j = 0; j < 70; j++) {
							cout << maze[i][j];
						}
						cout << endl;
					}*/

				}
				for (int c = midpt1_c; c <= midpt2_c; c++) {
					setCellStatus(midpt2_r, c, ' ');
					/*for (int i = 0; i < 18; i++) {
						for (int j = 0; j < 70; j++) {
							cout << maze[i][j];
						}
						cout << endl;
					}*/
				}
			}

			// AND TO THE RIGHT
			else if (midpt1_c > midpt2_c) {
				//cout << 7 << endl;
				// dig down and to left
				for (int r = midpt1_r; r <= midpt2_r; r++) {
					setCellStatus(r, midpt1_c, ' ');
				}
				for (int c = midpt1_c; c >= midpt2_c; c--) {
					setCellStatus(midpt2_r, c, ' ');
				}
			}
			// DIRECTLY BELOW
			else {
				//cout << 6 << endl;
				// just dig down
				for (int r = midpt1_r; r <= midpt2_r; r++) {
					setCellStatus(r, midpt1_c, ' ');
				}
			}
		}
		// ROOM 1 IS BELOW ROOM 2
		else if (midpt1_r > midpt2_r) {
			// AND TO RIGHT
			if (midpt1_c > midpt2_c) {
				//cout << 1 << endl;
				// dig left and up
				for (int c = midpt1_c; c >= midpt2_c; c--) {
					setCellStatus(midpt1_r, c, ' ');
				}
				for (int r = midpt1_r; r >= midpt2_r; r--) {
					setCellStatus(r, midpt2_c, ' ');
				}
			}
			// AND TO LEFT
			else if (midpt1_c < midpt2_c) {
				//cout << 2 << endl;
				// dig right and up
				for (int c = midpt1_c; c <= midpt2_c; c++) {
					setCellStatus(midpt1_r, c, ' ');
				}
				for (int r = midpt1_r; r >= midpt2_r; r--) {
					setCellStatus(r, midpt2_c, ' ');
				}
			}
			// directly above
			else {
				// dig up
				//cout << 5 << endl;
				for (int r = midpt1_r; r >= midpt2_r; r--) {
					setCellStatus(r, midpt2_c, ' ');
				}
			}
		}
		// ROOM CENTERS ARE LINED UP
		else {
			if (midpt1_c > midpt2_c) {
				//cout << 3 << endl;
				for (int c = midpt2_c; c <= midpt1_c; c++) {
					setCellStatus(midpt1_r, c, ' ');
				}
			}
			else {
				//cout << 4 << endl;
				for (int c = midpt1_c; c <= midpt2_c; c++) {
					setCellStatus(midpt1_r, c, ' ');
				}
			}
		}
		/*for (int i = 0; i < 18; i++) {
			for (int j = 0; j < 70; j++) {
				cout << maze[i][j];
			}
			cout << endl;
		}*/
	
		rooms++;
		cor++;
	}
	//cout << cor << " corridors" << endl;
	/*for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 70; j++) {
			cout << maze[i][j];
		}
		cout << endl;
	}*/

}

void Dungeon::setLevel(int level) {

	// Adjust new level
	m_level = level;

	// Delete objects and monsters from the last level
	vector<GameObject*>::iterator obj = m_FloorItems.begin();
	while (obj != m_FloorItems.end()) {
		delete* obj;
		obj++;
	}
	m_FloorItems.clear();

	vector<Actor*>::iterator mon = m_Monsters.begin();
	while (mon != m_Monsters.end()) {
		delete* mon;
		mon++;
	}
	m_Monsters.clear();

	vector<Room*>::iterator room = m_Rooms.begin();
	while (room != m_Rooms.end()) {
		delete* room;
		room++;
	}
	m_Rooms.clear();

	// Make new maze for new level
	makeMaze();

	// Add player
	Coord levelPos = openSpot();
	m_Player->move(levelPos.r(), levelPos.c());

	// Add objects
	num_objects = randInt(2, 3);
	for (int i = 0; i < num_objects; i++) {
		int key = randInt(1, 7);
		Coord spot = openSpot();
		addObject(spot.r(), spot.c(), key);
	}

	// Add monsters
	num_monsters = randInt(2, 5 * (m_level + 1) + 1);
	for (int i = 0; i < num_monsters; i++) {
		int key;
		// levels 2-3, bogeymen, goblins, and snakewomen
		if ((m_level >= 2) & (m_level < 3)) { key = randInt(1, 3); }
		// level 3+, all
		else if (m_level >= 3) { key = randInt(1, 4); }
		// level 0, 1 goblin and snakewomen
		else { key = randInt(1, 2); }
		Coord spot = openSpot();
		addMonster(spot, key);
	}

	// Add the staircase OR the idol
	if (m_level == 4) {
		Coord pos = openSpot();
		setCellStatus(pos.r(), pos.c(), '&');
		m_idol = new Coord(pos.r(), pos.c());
	}
	else {
		delete m_staircase;
		Coord pos = openSpot();
		setCellStatus(pos.r(), pos.c(), '>');
		m_staircase = new Coord(pos.r(), pos.c());
	}
}


// Adding Players and Items to Dungeon
void Dungeon::addPlayer() {

	if (m_Player != nullptr) {
		return;
	}

	m_Player = new Player();
	Coord pos = openSpot();
	while (!noActor(pos.r(), pos.c()))
		pos = openSpot();
	m_Player->move(pos.r(), pos.c());

}

void Dungeon::addObject(int r, int c, int key) {
	// Set Game Objects
	if ((key <= 3) | (key == 9) | (key == 10)) {
		//setCellStatus(r, c, ')');
		switch (key) {
			case 1:
				m_FloorItems.push_back(new ShortSword(r, c));
				break;
			case 2:
				m_FloorItems.push_back(new LongSword(r, c));
				break;
			case 3:
				m_FloorItems.push_back(new Mace(r, c));
				break;
			case 9:
				m_FloorItems.push_back(new MagicFangs(r, c));
				break;
			case 10:
				m_FloorItems.push_back(new MagicAx(r, c));
				break;
		}
	}
	else {
		//setCellStatus(r, c, '?');
		switch (key) {
			case 4:
				m_FloorItems.push_back(new strengthScroll(r, c));
				break;
			case 5:
				m_FloorItems.push_back(new dexScroll(r, c));
				break;
			case 6:
				m_FloorItems.push_back(new armorScroll(r, c));
				break;
			case 7:
				m_FloorItems.push_back(new healthScroll(r, c));
				break;
			case 8:
				m_FloorItems.push_back(new teleportScroll(r, c));
		}
	}
}

void Dungeon::addMonster(Coord pos, int key) {
	// Set Monsters
	switch (key) {
		case 1:
			m_Monsters.push_back(new Goblin(pos.r(), pos.c()));
			break;
		case 2:
			m_Monsters.push_back(new SnakeWoman(pos.r(), pos.c()));
			break;
		case 3:
			m_Monsters.push_back(new Bogeyman(pos.r(), pos.c()));
			break;
		case 4:
			m_Monsters.push_back(new Dragon(pos.r(), pos.c()));
			break;
	}
}

void Dungeon::removeDead() {

	// Remove dead monsters
	vector<Actor*>::iterator monster = m_Monsters.begin();
	while (monster != m_Monsters.end()) {
		if ((*monster)->getHP() <= 0) {
			if ((*monster)->name() == "Snakewoman") {
				if ((randInt(1,3)==1) & (getCellStatus((*monster)->getPos()->r(), (*monster)->getPos()->c())==' ')) {
					addObject((*monster)->getPos()->r(), (*monster)->getPos()->c(), 9);
				}
			}
			else if ((*monster)->name() == "Goblin") {
				if ((randInt(1, 3) == 1) & (getCellStatus((*monster)->getPos()->r(), (*monster)->getPos()->c())==' ')) {
					addObject((*monster)->getPos()->r(), (*monster)->getPos()->c(), randInt(9, 10));
				}
			}
			else if ((*monster)->name() == "Bogeyman") {
				if ((randInt(1, 10) == 1) & (getCellStatus((*monster)->getPos()->r(), (*monster)->getPos()->c()) == ' ')) {
					addObject((*monster)->getPos()->r(), (*monster)->getPos()->c(), 10);
				}
			}
			else if ((*monster)->name() == "Dragon") {
				if((getCellStatus((*monster)->getPos()->r(), (*monster)->getPos()->c()) == ' '))
					addObject((*monster)->getPos()->r(), (*monster)->getPos()->c(), randInt(4,8));
			}

			delete* monster;
			monster = m_Monsters.erase(monster);
			continue;
		}
		monster++;
	}
}

bool Dungeon::checkDead() {
	// Player is dead
	if (m_Player->getHP() <= 0) { return true; }
	return false;
}

// NPC Actions
// Monster Actions
string Dungeon::playMonsters() {
	string msg = "";
	vector<Actor*>::iterator monster = m_Monsters.begin();
	while (monster != m_Monsters.end()) {

		if ((*monster)->name() == "Dragon") {
			if (randInt(1,10)==1) { (*monster)->setHP((*monster)->getHP() + 1); }
		}

		if (inRange(1, (*monster)->getPos(), m_Player->getPos())) {
			msg+=(*monster)->attack(m_Player);
		}
		else {
			if ((*monster)->name() == "Snakewoman") {
				if (inRange(3, (*monster)->getPos(), m_Player->getPos())) { 
					moveMonster(*monster);
				}
			}
			else if ((*monster)->name() == "Bogeyman") {
				if (inRange(5, (*monster)->getPos(), m_Player->getPos())) {
					moveMonster(*monster);
				}
			}
			else if ((*monster)->name() == "Goblin") {
				// Only try moving/checking for an open path if the Goblin is within 15 spaces of the player
				if (inRange(goblinSmell, (*monster)->getPos(), m_Player->getPos())) {
					// Clear the path
					clearSteps();
					// Search for the open spot. Returns number of steps
					int value = goblinSearch(maze, (*monster)->getPos()->r(), (*monster)->getPos()->c(), getSteps(), goblinSmell+1);
					//cout << value << endl;
					Coord gobMove = Coord(0,0);
					// If a pathway was found, take a step
					if (!steps.empty()) {
						steps.pop();
						//cout << "first coord: " << steps.front().r() << ", " << steps.front().c() << endl;
						gobMove = getSteps().front();
						if (noActor(gobMove.r(), gobMove.c())) {
							(*monster)->move(gobMove.r(), gobMove.c());
						}
					}
					//getCharacter();

				}
			}
		}
		monster++;
	}
	return msg;
}

// Generic movement for dumb monsters towards player
void Dungeon::moveMonster(Actor* monster){
	int player_r = m_Player->getPos()->r();
	int player_c = m_Player->getPos()->c();
	int monster_r = monster->getPos()->r();
	int monster_c = monster->getPos()->c();
	
	// if below player
	if (player_r > monster_r){
		if ((noActor(monster_r+1, monster_c)) & (getCellStatus(monster_r+1, monster_c) != '#')) {
			monster->move(monster_r + 1, monster_c);
			return;
		}
	}
	if (player_c > monster_c) {
		if ((noActor(monster_r, monster_c + 1)) & (getCellStatus(monster_r, monster_c + 1) != '#')) {
			monster->move(monster_r, monster_c + 1);
			return;
		}
	}
	if (player_r < monster_r) {
		if ((noActor(monster_r - 1, monster_c)) & (getCellStatus(monster_r - 1, monster_c) != '#')) {
			monster->move(monster_r - 1, monster_c);
			return;
		}
	}
	if (player_c < monster_c) {
		if ((noActor(monster_r, monster_c-1)) & (getCellStatus(monster_r, monster_c-1) != '#')) {
			monster->move(monster_r, monster_c - 1);
			return;
		}
	}
}


// input global queue with null, shortest path length
// take path if available, add to queue and add to path length
// compare if path length is longer than shortest path
// continue until you reach the end -- compare with shortest, if shortest, update and return 

// Goblin Move Function
int Dungeon::goblinSearch(char map[18][70], int r, int c, queue<Coord> path, int shortest) {

	// Start position
	map[r][c] = '1';
	int pr = m_Player->getPos()->r();
	int pc = m_Player->getPos()->c();
	//map[pr][pc] = '@';

	path.push(Coord(r, c));

	// First base case: Found a path!
	//cout << r << ", " << c << endl;
	if ((pr == r) & (pc == c)) {
		//cout << "made to player" << endl;
		//cout << path.size() << endl;
		if (path.size() <= shortest) {
			setSteps(path);
			//cout << "new shortest: " << shortest;
			//getCharacter();
			return path.size();
		}
		else {
			map[r][c] = ' ';
			return shortest;
		}
	}

	// Second case, path is longer than shortest, no need to continue looking
	if (path.size() > shortest) {
		//cout << "reached" << endl;
		map[r][c] = ' ';
		return shortest;
	}

	// down
	if (( getCellStatus(r + 1, c) != '#' ) & (map[r+1][c] != '1')) {
		shortest = goblinSearch(map, r + 1, c, path, shortest);
	}
	// left
	if (((getCellStatus(r, c - 1) != '#') & (map[r][c-1] != '1'))) {
		shortest = goblinSearch(map, r, c-1, path, shortest);
	}
	// up
	if (((getCellStatus(r - 1, c) != '#') & (map[r-1][c] != '1'))) {
		shortest = goblinSearch(map, r-1, c, path, shortest);
	}
	// right
	if (((getCellStatus(r, c + 1) != '#') & (map[r][c + 1] != '1'))) {
		shortest = goblinSearch(map, r, c + 1, path, shortest);
	}

	map[pr][pc] = ' ';
	maze[r][c] = ' ';

	//cout << "found shortest path:" << shortest << endl;
	//getCharacter();

	return shortest;
}

// Player Actions

string Dungeon::moveAttackPlayer(char direction) {
	
	int r = m_Player->getPos()->r();
	int c = m_Player->getPos()->c();
	bool attack = false;
	int monster_r = 0, monster_c = 0;
	string msg = "";
	if (randInt(1,10) == 1) {
		m_Player->setHP(m_Player->getHP() + 1);
	}
	if (direction == ARROW_DOWN) {
		if (!noActor(r + 1, c)) {
			attack = true;
			monster_r = r + 1; monster_c = c;
		}
		else if (getCellStatus(r + 1, c) != '#') {
			m_Player->move(r + 1, c);
		}

	}
	else if (direction == ARROW_UP) {
		if (!noActor(r - 1, c)) {
			attack = true;
			monster_r = r - 1; monster_c = c;
		}
		else if (getCellStatus(r - 1, c) != '#') {
			m_Player->move(r - 1, c);
		}
	}
	else if (direction == ARROW_LEFT) {
		if (!noActor(r, c - 1)) {
			attack = true;
			monster_r = r; monster_c = c-1;
		}
		else if (getCellStatus(r, c - 1) != '#') {
			m_Player->move(r, c - 1);
		}
	}
	else if (direction == ARROW_RIGHT) {
		if (!noActor(r, c + 1)) {
			attack = true;
			monster_r = r; monster_c = c + 1;
		}
		else if (getCellStatus(r, c + 1) != '#') {
			m_Player->move(r, c + 1);
		}
	}

	if (attack) {
		vector<Actor*>::iterator monster = m_Monsters.begin();
		while (monster != m_Monsters.end()) {
			if (((*monster)->getPos()->r() == monster_r) & ((*monster)->getPos()->c() == monster_c)) {
				msg = m_Player->attack(*monster);
			}
			monster++;
		}
	}

	return msg;
}


char Dungeon::viewInventory() {
	clearScreen();
	vector<GameObject*>::iterator listItem = m_Player->getInventory().begin();
	char ch = 97;
	for (int i = 0; i < m_Player->getInventory().size(); i++) {
		cout << char(ch) << ". " << m_Player->getInventory()[i]->name() << endl;
		ch++;
	}
	char input = getCharacter();
	return input;
}

string Dungeon::readScroll() {
	// Calls view inventory to show scrolls
	string msg = "";
	char read = viewInventory();
	if (m_Player->getInventory().size() < (int(read) - 97 + 1)) {
		return msg;
	}
	GameObject* item = m_Player->getInventory()[int(read) - 97];
	if ((item->name() == "scroll of teleportation") | (item->name() == "scroll of strength") | 
		(item->name() == "scroll of enhance armor") | (item->name() == "scroll of enhance health") |
		(item->name() == "scroll of enhance dexterity")) {
		msg += "You read the scroll called " + item->name() + ".\n" + item->action();
		// Adjust player stats accordingly
		if (item->name() == "scroll of teleportation") {
			Coord teleport = openSpot();
			m_Player->move(teleport.r(), teleport.c());
		}
		else if (item->name() == "scroll of enhance armor") {
			m_Player->setArmor(m_Player->getArmor() + randInt(1, 3));
		}
		else if (item->name() == "scroll of strength") {
			m_Player->setStrength(m_Player->getStrength() + randInt(1, 3));
		}
		else if (item->name() == "scroll of enhance health") {
			m_Player->setMaxHP(m_Player->getMaxHP() + randInt(3, 8));
		}
		else if (item->name() == "scroll of enhance dexterity") {
			m_Player->setDexterity(m_Player->getDexterity() + 1);
		}
		delete item;
		m_Player->removeFromInventory(int(read) - 97);
	}
	else {
		msg += "You can't read a " + item->name() + ".\n";
	}
	return msg;
}

string Dungeon::wieldWeapon() {
	string msg = "";
	char wield = viewInventory();
	if (m_Player->getInventory().size() < (int(wield) - 97 + 1)) {
		return msg;
	}
	GameObject* item = m_Player->getInventory()[int(wield) - 97];
	if ((item->name() == "scroll of teleportation") | (item->name() == "scroll of strength") | 
		(item->name() == "scroll of enhance armor") | (item->name() == "scroll of enhance health") |
		(item->name() == "scroll of enhance dexterity")) {
		msg+="You can't wield " + item->name() + ".\n";
	}
	else {	
		// Wield new weapon
		m_Player->setWeapon(dynamic_cast<Weapon*>(item));
		msg += "You are wielding " + item->name() + "\n";
	}
	return msg;
}


string Dungeon::pickUp() {

	string msg = "";

	// If right on idol
	if (m_level == 4) {
		if ((m_Player->getPos()->r() == m_idol->r()) & (m_Player->getPos()->c() == m_idol->c())) {
			msg += "You pick up the golden idol\n";
			msg += "Congratulations, you won!\n";
			win = true;
			return msg;
		}
	}

	// If on an item
	if (m_Player->getInventory().size() > 25) {
		msg += "Your knapsack is full; you can't pick that up.\n";
	}
	else {
		vector<GameObject*>::iterator obj = m_FloorItems.begin();
		while (obj != m_FloorItems.end()) {
			if ((m_Player->getPos()->r() == (*obj)->floorPos()->r()) & (m_Player->getPos()->c() == (*obj)->floorPos()->c())) {
				if (((*obj)->name() == "scroll of teleportation") | ((*obj)->name() == "scroll of strength") | ((*obj)->name() == "scroll of enhance armor") | ((*obj)->name() == "scroll of enhance health") | ((*obj)->name() == "scroll of enhance dexterity")) {
					msg += "You pick up a scroll called " + (*obj)->name() + ".\n";
					setCellStatus(m_Player->getPos()->r(), m_Player->getPos()->c(), ' ');
					m_Player->addToInventory(*obj);
					obj = m_FloorItems.erase(obj);
					return msg;
				}
				else {
					msg += "You pick up " + (*obj)->name() + "\n";
					setCellStatus(m_Player->getPos()->r(), m_Player->getPos()->c(), ' ');
					m_Player->addToInventory(*obj);
					m_FloorItems.erase(obj);
					return msg;
				}
			}
			obj++;
		}
	}
	return msg;
};


void Dungeon::cheat() {
	m_Player->setStrength(9);
	m_Player->setHP(50);
	m_Player->setMaxHP(50);
}

void Dungeon::display(string msg) {

	// Clear screen and display new screen
	clearScreen();

	char displayGrid[18][70];
	int r, c;

	// Fill displayGrid
	for (r = 1; r <= 18; r++) {
		for (c = 1; c <= 70; c++) {
			displayGrid[r - 1][c - 1] = getCellStatus(r - 1, c - 1);
		}
	}

	// Set objects
	for (int i = 0; i < m_FloorItems.size(); i++) {
		displayGrid[m_FloorItems.at(i)->floorPos()->r()][m_FloorItems.at(i)->floorPos()->c()] = m_FloorItems.at(i)->floor();
	}

	// Set the staircase/idol
	if (m_level != 4) {
		displayGrid[m_staircase->r()][m_staircase->c()] = '>';
	}
	else if ((m_level == 4) & (m_idol!= nullptr)) {
		displayGrid[m_staircase->r()][m_staircase->c()] = '#';
		displayGrid[m_idol->r()][m_idol->c()] = '&';
	}

	// Set players
	displayGrid[m_Player->getPos()->r()][m_Player->getPos()->c()] = '@';

	// Set monsters
	vector<Actor*>::iterator monster = m_Monsters.begin();
	while (monster != m_Monsters.end()) {
		displayGrid[(*monster)->getPos()->r()][(*monster)->getPos()->c()] = (*monster)->name()[0];
		monster++;
	}

	// Display current dungeon
	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 70; j++) {
			cout << displayGrid[i][j];
		}
		cout << endl;
	}
	
	// Display player statistics
	cout << "Dungeon Level: " << getLevel() << ", ";
	cout << "Hit points: " << getPlayer()->getHP() << ", ";
	cout << "Armor: " << getPlayer()->getArmor() << ", ";
	cout << "Strength: " << getPlayer()->getStrength() << ", ";
	cout << "Dexterity: " << getPlayer()->getDexterity() << endl << endl;

	cout << msg << endl;

	// Various moves
	if (m_level == 4) {
		if (win) {
			cout << "Press q to exit game.\n";
			char ch = 'a';
			while (ch != 'q') {
				ch = getCharacter();
				if (ch == 'q')
					exit(1);
			}
			return;
		}
	}
	// if dead
	if (m_Player->getHP() <= 0) {
		cout << "Press q to exit game.\n";
		char ch = 'a';
		while (ch != 'q') {
			ch = getCharacter();
			if (ch == 'q')
				exit(1);
		}
		return;
	}
}

char Dungeon::getCellStatus(int r, int c) { return maze[r][c]; }

void Dungeon::setCellStatus(int r, int c, char status) { maze[r][c] = status; };

Coord Dungeon::openSpot() {

	int r = randInt(1, 18)-1;
	int c = randInt(1, 70)-1;
	char spot = getCellStatus(r, c);
	while (spot != ' ') {
		r = randInt(1, 18)-1;
		c = randInt(1, 70)-1;
		spot = getCellStatus(r, c);
	}

	return Coord(r, c);
}

bool Dungeon::noActor(int r, int c) {
	if ((r == m_Player->getPos()->r()) & (c == m_Player->getPos()->c())) {
		return false;
	}
	vector<Actor*>::iterator monster = m_Monsters.begin();
	while (monster != m_Monsters.end()) {
		if (((*monster)->getPos()->r() == r) & ((*monster)->getPos()->c() == c)) {
			return false;
		}
		monster++;
	}
	return true;
}


//bool Dungeon::noMonster(int r, int c) {
//	vector<Actor*>::iterator monster = m_Monsters.begin();
//	while (monster != m_Monsters.end()) {
//		if (((*monster)->getPos()->r() == r) & ((*monster)->getPos()->c() == c)) {
//			return false;
//		}
//		monster++;
//	}
//	return true;
//}

bool Dungeon::inRange(int range, Coord* playerPos, Coord* monsterPos) {
	int playerDif = abs(playerPos->r() - monsterPos->r()) + abs(playerPos->c() - monsterPos->c());
	if (playerDif <= range) {
		return true;
	}
	return false;
}