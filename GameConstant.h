#pragma once
#include "string"

#define HEIGHTSCREEN 600 //Chiều cao của màn hình console theo pixel
#define WIDTHSCREEN 900 //Chiều rộng của màn hình console theo pixel
#define HEIGHTCHAR 35 //Chiều cao của màn hình console theo kí tự
#define WIDTHCHAR 108 //Chiều rộng của màn hình console theo kí tự
enum CONTROLMENU { NONEe, UPe, DOWNe, LEFTe, RIGHTe, ENTERe }; //Các phím bấm để di chuyển, chọn option trong menu

enum eDir { STOP = 0, UPLEFT = 1, DOWNLEFT = 2, UPRIGHT = 3, DOWNRIGHT = 4, LEFT = 5, RIGHT = 6 };//Các hướng bay của quả bóng
enum eDirPadd { PaddleSTOP=0, PaddleUP = 1, PaddleDOWN = 2 }; //Các hướng di chuyển của thanh trượt
const std::string HISTORY = "History.txt";

const int yNewGame = 11;
const int yLoadGame = 13;
const int yHistory = 15;
const int yInstruction = 17;
const int yAbout = 19;
const int yQuit = 21;

