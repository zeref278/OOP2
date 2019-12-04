#include "Paddle.h"

//Input: None
//Output: Giá trị của tọa độ x trên màn hình console của vị trí trên cùng của thanh trượt (Kiểu int)
//Chức năng: Lấy tọa độ x của vị trí trên cùng của thanh trượt
int Paddle::X()
{
	return this->_pos.x;
}

//Input: None
//Output: Giá trị của tọa độ y trên màn hình console của vị trí trên cùng của thanh trượt (Kiểu int)
//Chức năng: Lấy tọa độ y của vị trí trên cùng của thanh trượt
int Paddle::Y()
{
	return this->_pos.y;
}

//Input: None
//Output: Giá trị của chiều dài của thanh trượt (Kiểu int)
//Chức năng: Lấy chiều dài của thanh trượt
int Paddle::Length()
{
	return this->_length;
}

//Input: None
//Output: Chuỗi tên của người chơi điều khiển thanh trượt (Kiểu string)
//Chức năng: Lấy chuỗi tên của người chơi
string Paddle::NamePlayer()
{
	return this->_namePlayer;
}

//Input: Chuỗi tên của người chơi điều khiển thanh trượt (Kiểu string)
//Output: None
//Chức năng: Đặt chuỗi tên của người chơi
void Paddle::SetNamePlayer(string value)
{
	this->_namePlayer = value;
}

//Input: None
//Output: None
//Chức năng: Tạo 1 Paddle với các giá trị mặc định: Tọa độ (x, y) = (0, 0) và chiều dài = 5
Paddle::Paddle()
{
	this->_pos.x = 0;
	this->_pos.y = 0;
	this->_length = 5;
}

//Input: Vị trí trên cùng của thanh trượt cần được khởi tạo (pos có kiểu POINT với 2 thành phần là pos.x và pos.y)
//Output: None
//Chức năng: Tạo 1 Paddle với các giá trị: 
//Vị trí (_pos) của thanh trượt = pos
//Vị trí ban đầu (_originalPos) của thanh trượt = pos (Dùng để gán giá trị cho _pos khi ResetData)
//Chiều dài (_length) của thanh trượt = 5
Paddle::Paddle(POINT pos)
{
	this->_originalPos = pos;
	this->_pos = pos;
	this->_length = 5;
}

//Input: None
//Output: None
//Chức năng: Reset lại dữ liệu của thanh trượt: Gán lại _pos = _originalPos
void Paddle::ResetData()
{
	this->_pos = this->_originalPos;
}

//Input: None
//Output: None
//Chức năng: Giảm tọa độ y xuống 1 đơn vị để di chuyển thanh trượt đi lên (trên màn hình console)
void Paddle::MoveUp()
{
	this->_pos.y--;
}

//Input: None
//Output: None
//Chức năng: Tăng tọa độ y lên 1 đơn vị để di chuyển thanh trượt đi xuống (trên màn hình console)
void Paddle::MoveDown()
{
	this->_pos.y++;
}

//Input: None
//Output: Chuỗi chứa thông tin về vị trí trên cùng của thanh trượt trên màn hình console (Kiểu string)
//Chức năng: In ra vị trí trên cùng của thanh trượt lên màn hình (Dùng để debug)
string Paddle::ToString()
{
	stringstream out;
	out << "Paddle [" << this->_pos.x << "," << this->_pos.y << "]" << endl;
	return out.str();
}