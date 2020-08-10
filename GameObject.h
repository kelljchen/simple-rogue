#ifndef GAMEOBJECT_INCLUDED
#define GAMEOBJECT_INCLUDED

#include <string>
#include "globals.h"
using namespace std;

class GameObject {
	public:
		GameObject(string name, int r, int c) : m_name(name), obj_Pos(new Coord(r, c)) {};
		virtual ~GameObject() { delete obj_Pos; };
		
		// Contains action string for each object
		virtual string action() = 0;
		virtual char floor() = 0;

		// Accessors
		string name() const { return m_name; };
		Coord* floorPos() { return obj_Pos; };

		// Mutators
		void setFloorPos(Coord newPos) { obj_Pos->setPos(newPos.r(), newPos.c()); };

	private:
		string m_name;
		Coord* obj_Pos;
};

#endif
