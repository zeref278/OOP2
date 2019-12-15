#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>
#include <chrono>
#include <random>
#include <thread>
#include "GameConstant.h"
using namespace std;



class Ball
{
private:
	POINT _pos; //Vị trí của quả bóng trên màn hình
	POINT _originalPos; //Vị trí lúc mới khởi tạo quả bóng
	eDir _direction; //Hướng bay của quả bóng
	float _velocity; //Vận tốc của quả bóng
public:
	int X();
	int Y();
	eDir Direction();
	float Velocity();
	void SetVelocity(float);
public:
	Ball();
	Ball(POINT pos);
public:
	void ResetData();
	void RandomPos(int upperBoundary);
	void ChangeDirection(eDir);
	void RandomDirection();
	void Move();
	string ToString();
};
