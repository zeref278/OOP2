#pragma warning
#include <iostream>
#include <string>
#include <sstream>
#include "GameManager.h"
#include <Windows.h>
#include <chrono>
#include <random>
#include <thread>
#include <string>
#include <sstream>
using namespace std;

class Item
{
private:
	POINT _pos;
public:
	POINT Postion();
	void SetPosition(int valuex, int valuey);
	void SetPosition(POINT value);
	string ToStringItem();
};

