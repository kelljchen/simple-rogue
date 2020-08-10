
#ifndef GameObject_INCLUDED
#define SCROLL_INCLUDED
#include <iostream>
using namespace std;
#include "GameObject.h"

class Scroll : public GameObject {
	public:
		Scroll(string type, int r, int c) : GameObject(type, r, c) {};
		virtual char floor() { return '?'; };
};

class teleportScroll : public Scroll {
	public:
		teleportScroll(int r, int c) : Scroll("scroll of teleportation", r, c) {};
		virtual string action() { return "You feel your body wrenched in space and time."; };
};

class strengthScroll : public Scroll {
	public:
		strengthScroll(int r, int c) : Scroll("scroll of strength", r, c) {};
		virtual string action() { return "Your muscles bulge."; };
};

class armorScroll : public Scroll {
	public:
		armorScroll(int r, int c) : Scroll("scroll of enhance armor", r, c) {};
		virtual string action() { return "Your armor glows blue."; };
};

class dexScroll : public Scroll {
	public:
		dexScroll(int r, int c) : Scroll("scroll of enhance dexterity", r, c) {};
		virtual string action() { return "You feel like less of a klutz."; };
};

class healthScroll : public Scroll {
	public:
		healthScroll(int r, int c) : Scroll("scroll of enhance health", r, c) {};
		virtual string action() { return "You feel your heart beating stronger."; };
};

#endif
