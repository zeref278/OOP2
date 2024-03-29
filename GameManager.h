﻿#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <conio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <random>
#include <chrono>
#include "Ball.h"
#include "Item.h"
#include "Paddle.h"
#include "GameConstant.h"
using namespace std;




class GameManager
{
private:
	int _width, _height; //Chiều rộng và chiều cao của màn hình console
	int _score1, _score2; //Điểm của người chơi 1 (Bên trái màn hình) và người chơi 2 (Bên phải màn hình)
	char _up1, _down1, _up2, _down2; //Các kí tự trên bàn phím dùng để di chuyển 2 thanh trượt đi lên và đi xuống
	bool _quit; //Cho biết đã thoát khỏi trò chơi hay chưa
	bool _isSinglePlayer; //Cho biết có phải chế độ một người chơi (đấu với BOT) hay không
	int _difficulty; //Độ khó của BOT (Dành cho chế độ một người chơi)
	Ball* _ball; //Quả bóng
	Paddle* _player1; //Thanh trượt bên trái
	Paddle* _player2; //Thanh trượt bên phải
	Item* _item;
	Item* _itemDownSpeed;
	Item* _addScore;
	Item* _subtracScore;
	int lastTouch = 0;

	fstream fileHistory;

public: // Định nghĩa hàm tạo và hàm huỷ
	GameManager(int, int);
	~GameManager();
public: //Định nghĩa các hàm thành viên

	void DrawScreenGame();

	void NewGame();
	void LoadGame();
	void SaveGame();

	void Instruction();
	void AboutUs();

	void GameHistory();
	void WriteFileHistory();
	void PauseGame(HANDLE t);
	void QuitGame();
	void EndGame(HANDLE t);

	//Lấy vào tên hợp lệ khi người dùng nhập vào
	string GetNamePlayer(int x, int y);

	void X2Score(int lastTouch);
	void XHalfScore(int lastTouch);

	void ScoreUp(Paddle*);
	void Input();
	void Logic();
	void Run();
	void ThreadFunc();
	void DrawBoard();
	void DrawBall(const char*);
	void DrawPaddles(const char*);
	void DrawScores();
	void DrawItem();
	void Bot();
};

//Xoá con trỏ chuột trên màn hình console
void Nocursortype();
//Cố định kích thước và làm mờ nút phóng to
void FixConsoleWindow();
//Di chuyển tới vị trí trong màn hình console
void GoToXY(int x, int y);
//Chỉnh sửa màu sắc
void TextColor(int x);
//Chỉnh sửa lại kích thước console
void resizeConsole(int width, int height);
//Kiểm tra kí tự trong tên có hợp lệ không
int isCharacterValid(char value);