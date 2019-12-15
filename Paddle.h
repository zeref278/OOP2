#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>
#include <thread>
#include "GameConstant.h"
using namespace std;

class Paddle
{
private:
	POINT _pos; //Vị trí trên cùng của thanh trượt
	int _length; //Chiều dài của thanh trượt
	POINT _originalPos; //Vị trí trên cùng của thanh trượt lúc mới khởi tạo thanh trượt
	string _namePlayer; //Tên của người chơi điều khiển thanh trượt
	eDirPadd _direction; //Hướng của thanh trượt
public:
	int X();
	int Y();
	int Length();
	string NamePlayer();
	void SetNamePlayer(string value);
public:
	Paddle();
	Paddle(POINT);
public:
	void ResetData();
	void MoveUp();
	void MoveDown();
	string ToString();
public:
	eDirPadd Direction() { return this->_direction; };
	void ChangeDirection(eDirPadd newDic) { this->_direction = newDic; };
};
