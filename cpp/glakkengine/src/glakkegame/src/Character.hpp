/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef CASDJASJD
#define CASDJASJD

class Character 
{
public:   
	Character() { _x = 0; _y = 0; }
	void setPosition(int x, int y) { _x = x; _y = y; }  
	int  getXPosition() const { return _x; }
	int  getYPosition() const { return _y; }
private:   
	int _x, _y;
};

#endif
