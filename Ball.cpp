#include "Ball.h"

//Input: None
//Output: Giá trị của tọa độ x trên màn hình console của vị trí của quả bóng (Kiểu int)
//Chức năng: Lấy tọa độ x của vị trí của quả bóng
int Ball::X()
{
	return this->_pos.x;
}

//Input: None
//Output: Giá trị của tọa độ y trên màn hình console của vị trí của quả bóng (Kiểu int)
//Chức năng: Lấy tọa độ y của vị trí của quả bóng
int Ball::Y()
{
	return this->_pos.y;
}

//Input: None
//Output: Giá trị thể hiện cho hướng bay của quả bóng (Kiểu eDir)
//Chức năng: Lấy hướng bay mà quả bóng đang bay
eDir Ball::Direction()
{
	return this->_direction;
}

//Input: None
//Output: Giá trị thể hiện cho vận tốc của quả bóng (Kiểu float)
//Chức năng: Lấy vận tốc của quả vóng
float Ball::Velocity()
{
	return this->_velocity;
}

//Input: Giá trị thể hiện cho vận tốc của quả bóng (Kiểu float)
//Output: None
//Chức năng: Đặt vận tốc của quả vóng
void Ball::SetVelocity(float velocity)
{
	this->_velocity = velocity;
}

//Input: None
//Output: None
//Chức năng: Tạo 1 Ball với các giá trị mặc định: 
//Vị trí (_pos) của quả bóng và vị trí ban đầu (_originalPos) của quả bóng = (0, 0)
//Hướng bay (_direction) của quả bóng = STOP
//Vận tốc (_velocity) của quả bóng = 1
Ball::Ball()
{
	this->_originalPos.x = 0;
	this->_originalPos.y = 0;
	this->_pos.x = 0;
	this->_pos.y = 0;
	this->_direction = STOP;
	this->_velocity = 1;
}

//Input: Vị trí của quả bóng cần được khởi tạo (pos có kiểu POINT với 2 thành phần là pos.x và pos.y)
//Output: None
//Chức năng: Tạo 1 Ball với các giá trị: 
//Vị trí (_pos) của quả bóng = pos
//Vị trí ban đầu (_originalPos) của quả bóng = pos 
//Hướng bay (_direction) của quả bóng = STOP
//Vận tốc (_velocity) của quả bóng = 1
Ball::Ball(POINT pos)
{
	this->_originalPos = pos;
	this->_pos = pos;
	this->_direction = STOP;
	this->_velocity = 1;
}

//Input: None
//Output: None
//Chức năng: Reset lại dữ liệu của quả bóng: Gán lại _direction = STOP và _velocity = 1
void Ball::ResetData()
{
	this->_direction = STOP;
	this->_velocity = 1;
}

//Input: Chặn trên của tọa độ y cần random của quả bóng
//Output: None
//Chức năng: Random vị trí bắt đầu của quả bóng trên trục tung
void Ball::RandomPos(int upperBoundary)
{
	//Tạo số ngẫu nhiên theo kiểu của C++
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); //Tạo seed theo thời gian hiện tại
	std::default_random_engine generator(seed); //Tạo bộ sinh số ngẫu nhiên với seed vừa tạo
	//Distribution (Phân phối) để sinh ra 1 số ngẫu nhiên trong các số nguyên từ 1 đến upperBoundary - 1
	std::uniform_int_distribution<int> distribution(1, upperBoundary - 1);

	int randomNumber = distribution(generator); //Sinh ra 1 số ngẫu nhiên trong các số nguyên từ 1 đến upperBoundary - 1

	this->_pos.x = this->_originalPos.x; //Gán tọa độ x (_pos.x) của quả bóng = tọa độ x ban đầu (_originalPos.x) của quả bóng
	this->_pos.y = randomNumber; //Gán tọa độ y (_pos.y) của quả bóng = số ngẫu nhiên
}

//Input: Giá trị thể hiện cho hướng bay của quả bóng (Kiểu eDir)
//Output: None
//Chức năng: Đặt lại hướng bay mà quả bóng đang bay
void Ball::ChangeDirection(eDir direction)
{
	this->_direction = direction;
}

//Input: None
//Output: None
//Chức năng: Random hướng bay của quả bóng (Khi mới bắt đầu ván mới)
void Ball::RandomDirection()
{
	//Tạo số ngẫu nhiên theo kiểu của C++
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); //Tạo seed theo thời gian hiện tại
	std::default_random_engine generator(seed); //Tạo bộ sinh số ngẫu nhiên với seed vừa tạo
	//Distribution (Phân phối) để sinh ra 1 số ngẫu nhiên trong 2 số nguyên là 1 hoặc 2 (50/50)
	std::uniform_int_distribution<int> distribution1(1, 2);

	int randomNumber = distribution1(generator); //Sinh ra 1 số ngẫu nhiên trong 2 số nguyên là 1 hoặc 2

	if (randomNumber == 1) //Nếu là 1 thì quả bóng bay về bên phải (Gồm có 2 hướng là UPRIGHT và DOWNRIGHT)
	{
		//Distribution (Phân phối) để sinh ra 1 số ngẫu nhiên trong 2 số nguyên là 3 hoặc 4 (3 là UPRIGHT, 4 là DOWNRIGHT)
		std::uniform_int_distribution<int> distribution2(3, 4);

		randomNumber = distribution2(generator); //Sinh ra 1 số ngẫu nhiên trong 2 số nguyên là 3 hoặc 4

		this->_direction = (eDir)(randomNumber); //Gán hướng bay (_direction) của quả bóng = số ngẫu nhiên (Ép kiểu về eDir)
	}
	else //Nếu là 2 thì quả bóng bay về bên trái
	{
		//Distribution (Phân phối) để sinh ra 1 số ngẫu nhiên trong 2 số nguyên là 1 hoặc 2 (1 là UPLEFT, 2 là DOWNLEFT)
		std::uniform_int_distribution<int> distribution2(1, 2);

		randomNumber = distribution2(generator); //Sinh ra 1 số ngẫu nhiên trong 2 số nguyên là 1 hoặc 2

		this->_direction = (eDir)(randomNumber); //Gán hướng bay (_direction) của quả bóng = số ngẫu nhiên (Ép kiểu về eDir)
	}
}

//Input: None
//Output: None
//Chức năng: Di chuyển quả bóng trên màn hình dựa vào hướng bay
void Ball::Move()
{
	switch (this->_direction)
	{
	case STOP: //Hướng bay là STOP (Đang dừng) thì không cần di chuyển
		break;
	case UPLEFT: //Hướng bay là UPLEFT (Đang bay về phía trên trái) thì
		this->_pos.x--; //Giảm tọa độ x xuống 1 đơn vị để di chuyển quả bóng đi qua trái (trên màn hình console)
		this->_pos.y--; //Giảm tọa độ y xuống 1 đơn vị để di chuyển quả bóng đi lên (trên màn hình console)
		break;
	case DOWNLEFT: //Hướng bay là DOWNLEFT (Đang bay về phía dưới trái) thì
		this->_pos.x--; //Giảm tọa độ x xuống 1 đơn vị để di chuyển quả bóng đi qua trái (trên màn hình console)
		this->_pos.y++; //Tăng tọa độ y lên 1 đơn vị để di chuyển quả bóng đi xuống (trên màn hình console)
		break;
	case UPRIGHT: //Hướng bay là UPRIGHT (Đang bay về phía trên phải) thì
		this->_pos.x++; //Tăng tọa độ x lên 1 đơn vị để di chuyển quả bóng đi qua phải (trên màn hình console)
		this->_pos.y--; //Giảm tọa độ y xuống 1 đơn vị để di chuyển quả bóng đi lên (trên màn hình console)
		break;
	case DOWNRIGHT: //Hướng bay là DOWNRIGHT (Đang bay về phía dưới phải) thì
		this->_pos.x++; //Tăng tọa độ x lên 1 đơn vị để di chuyển quả bóng đi qua phải (trên màn hình console)
		this->_pos.y++; //Tăng tọa độ y lên 1 đơn vị để di chuyển quả bóng đi xuống (trên màn hình console)
		break;
	default: //Không phải các hướng bay trên thì không cần di chuyển
		break;
	}
}

//Input: None
//Output: Chuỗi chứa thông tin về vị trí của quả bóng trên màn hình console (Kiểu string)
//Chức năng: In ra vị trí của quả bóng lên màn hình (Dùng để debug)
string Ball::ToString()
{
	stringstream out;
	out << "Ball [" << this->_pos.x << "," << this->_pos.y << "][" << this->_direction << "]" << endl;
	return out.str();
}