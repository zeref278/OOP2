#include "Item.h"

POINT Item::Postion()
{
	return this->_pos;
}
void Item::SetPosition(int valuex, int valuey)
{
	this->_pos.x = valuex;
	this->_pos.y = valuey;
}
void Item::SetPosition(POINT value)
{
	SetPosition(value.x, value.y);
}
string Item::ToStringItem()
{
	stringstream out;
	out << "Item [" << this->_pos.x << "," << this->_pos.y << "]" << endl;
	return out.str();
}

