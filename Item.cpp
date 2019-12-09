#include "Item.h"
#include <thread>
#include <random>

Item::Item()
{
	this->_pos.x = rand() % 103 + 5;
	this->_pos.y = rand() % 30;
}
Item::~Item()
{}
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

void Item::ResetData()
{
	this->_pos.x = rand() % 103 + 5;
	this->_pos.y = rand() % 21 + 2;
}

std::string Item::ToStringItem()
{
	std::stringstream out;
	out << "Item [" << this->_pos.x << "," << this->_pos.y << "]" << std::endl;
	return out.str();
}
int Item::X()
{
	return this->_pos.x;
}
int Item::Y()
{
	return this->_pos.y;
}
void Item::RandomPos()
{
	srand(time(NULL));
	int valuex = rand() % 103 + 5;
	int valuey = rand() % 30;
	this->_pos.x = valuex;
	this->_pos.y = valuey;
}

