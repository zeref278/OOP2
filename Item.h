#pragma once
#include <Windows.h>
#include <sstream>
#include <time.h>
#include <iostream>
#include <string>

class Item
{
private:
	POINT _pos;
public:
	Item();
	~Item();
	POINT Postion();
	int X();
	int Y();
	void SetPosition(int valuex, int valuey);
	void RandomPos();
	void SetPosition(POINT value);
	void ResetData();
	std::string ToStringItem();
};

