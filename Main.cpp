#include <iostream>
#include <Windows.h>
#include <thread>
#include "Ball.h"
#include "Paddle.h"
#include "GameManager.h"
#include "Item.h"
using namespace std;

void main()
{
	GameManager Console(WIDTHCHAR - 1, HEIGHTCHAR - 1);
	Console.Run();
}