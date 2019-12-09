#include "GameManager.h"

//Input: chiều rộng (width) (Kiểu int) và chiều cao (height) (Kiểu int) của màn hình console
//Output: None
//Chức năng: Khởi tạo một trò chơi mới: Tạo 1 GameManager với các giá trị:
//_quit = false (Để bắt đầu trò chơi)
//_up1 = 'W': Phím 'W' để di chuyển thanh trượt bên trái đi lên
//_up2 = 0x26: Phím mũi tên đi lên (Up Arrow) để di chuyển thanh trượt bên phải đi lên
//_down1 = 'S': Phím 'S' để di chuyển thanh trượt bên trái đi xuống
//_down2 = 0x28: Phím mũi tên đi xuống (Down Arrow) để di chuyển thanh trượt bên phải đi xuống
//Điểm số của 2 bên _score1 = _score2 = 0
//Chiều rộng của màn hình console _width = width, chiều cao của màn hình console _height = height
//Tạo 1 quả bóng (_ball), 1 thanh trượt bên trái (_player1) và 1 thanh trượt bên phải (_player2)
//Vẽ ScreenGame (Ở menu game) và Board (Khi bắt đầu chơi game)
GameManager::GameManager(int width, int height)
{
	this->_quit = false; //Bắt đầu trò chơi

	//Các phím dùng để di chuyển thanh trượt bên trái và bên phải đi lên và đi xuống
	this->_up1 = 'W'; this->_up2 = 0x26; //0x26: Phím mũi tên đi lên (Virtual-Key Codes)
	this->_down1 = 'S'; this->_down2 = 0x28; //0x28: Phím mũi tên đi xuống (Virtual-Key Codes)

	this->_score1 = this->_score2 = 0; //Điểm số của 2 bên
	this->_width = width; this->_height = height; //Chiều rộng và chiều cao của màn hình console

	//Tạo 1 quả bóng với vị trí ban đầu nằm ở trung tâm của trường chơi game
	POINT posBall = { width / 2, (height - 8) / 2 };
	this->_ball = new Ball(posBall);

	//Tạo 1 thanh trượt bên trái với vị trí ban đầu nằm ở chính giữa phía bên trái của trường chơi game
	POINT posPaddle1 = { 2, (height - 8) / 2 - 2 };
	this->_player1 = new Paddle(posPaddle1);

	//Tạo 1 thanh trượt bên phải với vị trí ban đầu nằm ở chính giữa phía bên phải của trường chơi game
	POINT posPaddle2 = { width - 2, (height - 8) / 2 - 2 };
	this->_player2 = new Paddle(posPaddle2);
	
	this->_item = new Item();
	this->_itemDownSpeed = new Item();
	this->_addScore = new Item();
	this->_subtracScore = new Item();


	DrawScreenGame(); //ScreenGame chỉ cần vẽ 1 lần
	DrawBoard(); //Board chỉ cần vẽ 1 lần
}

//Input: None
//Output: None
//Chức năng: Hủy 1 GameManager: Hủy các biến cấp phát trong vùng nhớ heap
//Hủy _ball (Quả bóng), _player1 (Thanh trượt bên trái) và _player2 (Thanh trượt bên phải)
GameManager::~GameManager()
{
	delete this->_ball;
	delete this->_player1;
	delete this->_player2;
	delete this->_item;
	delete this->_itemDownSpeed;
	delete this->_addScore;
	delete this->_subtracScore;
}

//Input: Thanh trượt của người chơi (Bên trái hoặc bên phải) cần tăng điểm (Kiểu Paddles)
//Output: None
//Chức năng: Cập nhật điểm số của người chơi: Tăng điểm số của người chơi (Bên trái hoặc bên phải) lên 1 đơn vị
void GameManager::ScoreUp(Paddle* player)
{
	DrawPaddles(" "); //Xóa 2 thanh trượt trên màn hình console (Để vẽ lại ở vị trí ban đầu khi qua ván mới)

	if (player == this->_player1) //Người chơi cần tăng điểm là người chơi bên trái
	{
		this->_score1++;
	}
	else if (player == this->_player2) //Người chơi cần tăng điểm là người chơi bên phải
	{
		this->_score2++;
	}

	this->_ball->RandomPos(this->_height - 8); //Random vị trí bắt đầu của quả bóng trên trục tung
	this->_ball->ResetData(); //Reset lại dữ liệu của quả bóng
	this->_player1->ResetData(); //Reset lại dữ liệu của thanh trượt bên trái
	this->_player2->ResetData(); //Reset lại dữ liệu của thanh trượt bên phải
	DrawPaddles("\xDB"); //Vẽ lại 2 thanh trượt ở vị trí ban đầu khi qua ván mới
	GoToXY(this->_item->X(), this->_item->Y());
	cout << " ";
	GoToXY(this->_itemDownSpeed->X(), this->_itemDownSpeed->Y());
	cout << " ";
	GoToXY(this->_addScore->X(), this->_addScore->Y());
	cout << " ";
	GoToXY(this->_subtracScore->X(), this->_subtracScore->Y());
	cout << " ";
	this->_item->ResetData();
	this->_itemDownSpeed->ResetData();
	this->_addScore->ResetData();
	this->_subtracScore->ResetData();
	DrawItem();
	DrawScores(); //Vẽ điểm số của 2 bên
}

//Input: None
//Output: None
//Chức năng: Xử lý Input từ bàn phím khi chơi + vẽ lại thanh trượt khi di chuyển lên hoặc xuống + xử lý Bot (Chế độ SinglePlayer)
//+ Cho quả bóng di chuyển + Đếm ngược 3...2...1 cho quả bóng khi bắt đầu 1 ván mới
void GameManager::Input()
{
	this->_ball->Move(); //Cho quả bóng di chuyển

	if (this->_ball->Direction() == STOP) //Quả bóng đang dừng (Khi bắt đầu 1 ván mới)
	{
		//Đếm ngược 3...2...1 với thời gian Sleep là 1 giây cho các khoảng. Tới 0 thì đó chính là quả bóng luôn
		DrawBall("3");
		Sleep(1000);
		DrawBall("2");
		Sleep(1000);
		DrawBall("1");
		Sleep(1000);
		this->_ball->RandomDirection(); //Quả bóng đang dừng (Khi bắt đầu 1 ván mới) thì cho hướng bay ngẫu nhiên
		DrawItem();
	}
	//Lấy tọa độ x của 2 thanh trượt
	int player1x = this->_player1->X();
	int player2x = this->_player2->X();

	//Lấy tọa độ y của 2 thanh trượt
	int player1y = this->_player1->Y();
	int player2y = this->_player2->Y();

	//Lấy chiều dài của 2 thanh trượt (2 thanh trượt có cùng 1 chiều dài)
	int length = this->_player1->Length();

	if (_kbhit()) //Nếu nhận được phím bấm từ người chơi
	{
		//Cần check GetAsyncKeyState và 0x8000 (Most significant bit) vì ta cần kiểm tra khi phím được bấm ngay tại thời điểm này
		//Bỏ qua trường hợp đã bấm khi gọi hàm này trước đó (Least significant bit)
		if (GetAsyncKeyState(this->_up1) & 0x8000) //Khi người chơi bên trái bấm phím 'W' (Di chuyển thanh trượt bên trái đi lên)
		{
			//Vị trí của thanh trượt khi di chuyển đi lên phải nằm dưới tường rào ở phía trên thì mới di chuyển đi lên được
			if (player1y > 1)
			{
				TextColor(14); //Màu của thanh trượt

				GoToXY(player1x, player1y + length - 1); cout << " "; //Xóa 1 kí tự ở dưới cùng của thanh trượt bên trái

				this->_player1->MoveUp(); //Di chuyển thanh trượt bên trái đi lên

				player1y--; //Giảm player1y xuống 1 đơn vị (Cùng với tọa độ y của _player1 đã giảm ở trên) để tránh bị lỗi

				GoToXY(player1x, player1y); cout << "\xDB"; //Vẽ thêm 1 kí tự ở trên cùng của thanh trượt bên trái
			}
		}
		//Cần check GetAsyncKeyState và 0x8000 (Most significant bit) vì ta cần kiểm tra khi phím được bấm ngay tại thời điểm này
		//Bỏ qua trường hợp đã bấm khi gọi hàm này trước đó (Least significant bit)
		if (GetAsyncKeyState(this->_down1) & 0x8000) //Khi người chơi bên trái bấm phím 'S' (Di chuyển thanh trượt bên trái đi xuống)
		{
			//Vị trí của thanh trượt khi di chuyển đi xuống phải nằm trên tường rào ở phía dưới thì mới di chuyển đi xuống được
			if (player1y + length < this->_height - 8)
			{
				TextColor(14); //Màu của thanh trượt

				GoToXY(player1x, player1y); cout << " "; //Xóa 1 kí tự ở trên cùng của thanh trượt bên trái

				this->_player1->MoveDown(); //Di chuyển thanh trượt bên trái đi xuống

				player1y++; //Tăng player1y lên 1 đơn vị (Cùng với tọa độ y của _player1 đã tăng ở trên) để tránh bị lỗi

				GoToXY(player1x, player1y + length - 1); cout << "\xDB"; //Vẽ thêm 1 kí tự ở dưới cùng của thanh trượt bên trái
			}
		}
		if (this->_isSinglePlayer == false) //Nếu không phải chế độ SinglePlayer
		{
			//Cần check GetAsyncKeyState và 0x8000 (Most significant bit) vì ta cần kiểm tra khi phím được bấm ngay tại thời điểm này
			//Bỏ qua trường hợp đã bấm khi gọi hàm này trước đó (Least significant bit)
			if (GetAsyncKeyState(this->_up2) & 0x8000) //Khi người chơi bên phải bấm phím Up Arrow (Di chuyển thanh trượt bên phải đi lên)
			{
				//Vị trí của thanh trượt khi di chuyển đi lên phải nằm dưới tường rào ở phía trên thì mới di chuyển đi lên được
				if (player2y > 1)
				{
					TextColor(14); //Màu của thanh trượt

					GoToXY(player2x, player2y + length - 1); cout << " "; //Xóa 1 kí tự ở dưới cùng của thanh trượt bên phải

					this->_player2->MoveUp(); //Di chuyển thanh trượt bên phải đi lên

					player2y--; //Giảm player2y xuống 1 đơn vị (Cùng với tọa độ y của _player2 đã giảm ở trên) để tránh bị lỗi

					GoToXY(player2x, player2y); cout << "\xDB"; //Vẽ thêm 1 kí tự ở trên cùng của thanh trượt bên phải
				}
			}
			//Cần check GetAsyncKeyState và 0x8000 (Most significant bit) vì ta cần kiểm tra khi phím được bấm ngay tại thời điểm này
			//Bỏ qua trường hợp đã bấm khi gọi hàm này trước đó (Least significant bit)
			if (GetAsyncKeyState(this->_down2) & 0x8000) //Khi người chơi bên phải bấm phím Down Arrow (Di chuyển thanh trượt bên phải đi xuống)
			{
				// Vị trí của thanh trượt khi di chuyển đi xuống phải nằm trên tường rào ở phía dưới thì mới di chuyển đi xuống được
				if (player2y + length < this->_height - 8)
				{
					TextColor(14); //Màu của thanh trượt

					GoToXY(player2x, player2y); cout << " "; //Xóa 1 kí tự ở trên cùng của thanh trượt bên phải

					this->_player2->MoveDown(); //Di chuyển thanh trượt bên phải đi xuống

					player2y++; //Tăng player2y lên 1 đơn vị (Cùng với tọa độ y của _player2 đã tăng ở trên) để tránh bị lỗi

					GoToXY(player2x, player2y + length - 1); cout << "\xDB"; //Vẽ thêm 1 kí tự ở dưới cùng của thanh trượt bên phải
				}
			}
		}
		//Cần check GetAsyncKeyState và 0x8000 (Most significant bit) vì ta cần kiểm tra khi phím được bấm ngay tại thời điểm này
		//Bỏ qua trường hợp đã bấm khi gọi hàm này trước đó (Least significant bit)
		if (GetAsyncKeyState('Q') & 0x8000) //Khi người chơi bấm phím 'Q'
		{
			this->_quit = true; //Kết thúc trò chơi
		}
	}

	if (this->_isSinglePlayer == true) //Nếu là chế độ SinglePlayer
	{
		Bot(); //Xử lý bot (Thanh trượt chạy tự động)
	}
}

//Input: None
//Output: None
//Chức năng: Xử lý logic của trò chơi (Khi bóng va chạm vào biên sân hay thanh trượt của người chơi)
void GameManager::Logic()
{
	//Lấy tọa độ x và y của quả bóng
	int ballx = this->_ball->X();
	int bally = this->_ball->Y();

	//Lấy tọa độ x của 2 thanh trượt
	int player1x = this->_player1->X();
	int player2x = this->_player2->X();

	//Lấy tọa độ y của 2 thanh trượt
	int player1y = this->_player1->Y();
	int player2y = this->_player2->Y();

	//Lấy chiều dài của 2 thanh trượt (2 thanh trượt có cùng 1 chiều dài)
	int length = this->_player1->Length();

	//Quả bóng va chạm vào thanh trượt bên trái
	for (int i = 0; i < length; i++)
	{
		if (ballx == player1x + 1) //Tọa độ x của quả bóng nằm ngay bên phải thanh trượt bên trái
		{
			if (bally == player1y + i) //Tọa độ y của quả bóng trùng với 1 vị trí nào đó trên thanh trượt bên trái
			{
				this->lastTouch = -1;
				if (i == 0 || i == 1) //Bóng va chạm ở nửa trên thanh trượt bên trái thì luôn phản xạ về phía phải trên
				{
					this->_ball->ChangeDirection(UPRIGHT);
				}
				else if (i == 2) //Bóng va chạm ở chính giữa thanh trượt bên trái
				{
					if (this->_ball->Direction() == DOWNLEFT) //Bóng va chạm theo hướng trái dưới
					{
						this->_ball->ChangeDirection(DOWNRIGHT); //Phản xạ theo hướng phải dưới
					}
					else if (this->_ball->Direction() == UPLEFT) //Bóng va chạm theo hướng trái trên
					{
						this->_ball->ChangeDirection(UPRIGHT); //Phản xạ theo hướng phải trên
					}
				}
				else if (i == 3 || i == 4) //Bóng va chạm ở nửa dưới thanh trượt bên trái thì luôn phản xạ về phía phải dưới
				{
					this->_ball->ChangeDirection(DOWNRIGHT);
				}

				float velocity = this->_ball->Velocity(); //Lấy vận tốc của quả bóng
				velocity += 0.1 * velocity; //Vận tốc của quả bóng tăng lên 10% sau khi va chạm vào thanh trượt
				this->_ball->SetVelocity(velocity); //Đặt lại vận tốc của quả bóng
			}
		}
	}

	//Quả bóng va chạm vào thanh trượt bên phải
	for (int i = 0; i < length; i++)
	{
		if (ballx == player2x - 1) //Tọa độ x của quả bóng nằm ngay bên trái thanh trượt bên phải
		{
			if (bally == player2y + i) //Tọa độ y của quả bóng trùng với 1 vị trí nào đó trên thanh trượt bên phải
			{
				this->lastTouch = 1;
				if (i == 0 || i == 1) //Bóng va chạm ở nửa trên thanh trượt bên phải thì luôn phản xạ về phía trái trên
				{
					this->_ball->ChangeDirection(UPLEFT);
				}
				else if (i == 2) //Bóng va chạm ở chính giữa thanh trượt bên phải
				{
					if (this->_ball->Direction() == DOWNRIGHT) //Bóng va chạm theo hướng phải dưới
					{
						this->_ball->ChangeDirection(DOWNLEFT); //Phản xạ theo hướng trái dưới
					}
					else if (this->_ball->Direction() == UPRIGHT) //Bóng va chạm theo hướng phải trên
					{
						this->_ball->ChangeDirection(UPLEFT); //Phản xạ theo hướng trái trên
					}
				}
				else if (i == 3 || i == 4) //Bóng va chạm ở nửa dưới thanh trượt bên phải thì luôn phản xạ về phía trái dưới
				{
					this->_ball->ChangeDirection(DOWNLEFT);
				}

				float velocity = this->_ball->Velocity(); //Lấy vận tốc của quả bóng
				velocity += 0.1 * velocity; //Vận tốc của quả bóng tăng lên 10% sau khi va chạm vào thanh trượt
				this->_ball->SetVelocity(velocity); //Đặt lại vận tốc của quả bóng
			}
		}
	}

	//Quả bóng va chạm vào tường rào bên dưới của trường chơi game (Tường rào bên trên của trường thông tin)
	if (bally == this->_height - 9) //Tọa độ y của quả bóng nằm ngay bên trên tường rào bên dưới
	{
		//Bóng va chạm tường rào bên dưới theo hướng phải dưới thì phản xạ theo hướng phải trên
		//Ngược lại (Bóng va chạm theo hướng trái dưới) thì phản xạ theo hướng trái trên
		this->_ball->ChangeDirection(this->_ball->Direction() == DOWNRIGHT ? UPRIGHT : UPLEFT);
	}

	//Quả bóng va chạm vào tường rào bên trên của trường chơi game
	if (bally == 1) //Tọa độ y của quả bóng nằm ngay bên dưới tường rào bên trên
	{
		//Bóng va chạm tường rào bên trên theo hướng phải trên thì phản xạ theo hướng phải dưới
		//Ngược lại (Bóng va chạm theo hướng trái trên) thì phản xạ theo hướng trái dưới
		this->_ball->ChangeDirection(this->_ball->Direction() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
	}

	//Quả bóng va chạm vào tường rào bên phải
	if (ballx == this->_width - 1) //Tọa độ x của quả bóng trùng với tường rào bên phải
	{
		ScoreUp(this->_player1); //Tăng điểm của người chơi bên trái lên 1 đơn vị
	}

	//Quả bóng va chạm vào tường rào bên trái
	if (ballx == 0) //Tọa độ x của quả bóng trùng với tường rào bên trái
	{
		ScoreUp(this->_player2); //Tăng điểm của người chơi bên phải lên 1 đơn vị
	}
	if (ballx == this->_itemDownSpeed->X() && bally == this->_itemDownSpeed->Y())
	{
		this->_ball->ChangeDirection(this->_ball->Direction() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
		this->_itemDownSpeed->SetPosition(0, 0);
	}
	if (ballx == this->_item->X() && bally == this->_item->Y())
	{
		this->_ball->SetVelocity(1.5 * this->_ball->Velocity());
		this->_item->SetPosition(0, 0);
	}
	if (ballx == this->_addScore->X() && bally == this->_addScore->Y() && this->lastTouch != 0)
	{
		X2Score(this->lastTouch);
	}
	if (ballx == this->_subtracScore->X() && bally == this->_subtracScore->Y() && this->lastTouch != 0)
	{
		XHalfScore(this->lastTouch);
	}
}
//Input: None
//Output: None
//Chức năng: Thủ tục cho thread (tiểu trình) cho quả bóng, xử lý input, xử lý logic (Để vận hành game)
void GameManager::ThreadFunc()
{
	while (true) //Vòng lặp vô tận (Cho đến khi thread bị hủy)
	{
		if (!this->_quit) //Khi trò chơi vẫn đang tiếp tục
		{
			DrawBall(" "); //Vẽ khoảng trắng để xóa vị trí trước đó của quả bóng

			Input(); //Xử lý input
			Logic(); //Xử lý logic của trò chơi

			DrawBall("O"); //Vẽ lại quả bóng ở vị trí mới
			Sleep(100 / this->_ball->Velocity()); //Hàm ngủ theo vận tốc Velocity
		}
	}
}

//Input: None
//Output: None
//Chức năng: Hàm chạy chính của chương trình:
//Tạo một thread (tiểu trình) để chạy song song với vòng lặp game, xử lý khi pause game, thắng, thua
//Và hủy thread sau khi vòng lặp game kết thúc (Thoát game)
void GameManager::Run()
{
	thread t1(&GameManager::ThreadFunc, this); //Tạo thread
	HANDLE handle_t1 = t1.native_handle(); //Lấy handle của thread

	while (!this->_quit) //Khi vẫn chưa thoát game
	{
		if (_kbhit()) //Nếu nhận được phím bấm từ người chơi
		{
			//Cần check GetAsyncKeyState và 0x8000 (Most significant bit) vì ta cần kiểm tra khi phím được bấm ngay tại thời điểm này
			//Bỏ qua trường hợp đã bấm khi gọi hàm này trước đó (Least significant bit)
			if (GetAsyncKeyState('P') & 0x8000) //Khi người chơi bấm phím 'P'
			{
				PauseGame(handle_t1); //Tạm dừng trò chơi (Suspend Thread)
			}
		}
		if (this->_score1 == 5 || this->_score2 == 5) //Nếu điểm số của người chơi bên trái hoặc bên phải == 5
		{
			EndGame(handle_t1); //Xử lý thắng, thua
		}
		Sleep(100 / this->_ball->Velocity()); //Để hàm Run chạy song song với thread
	}

	TextColor(0); //Màu trong suốt để tránh lỗi tiềm năng
	system("cls"); //Xóa trắng màn hình
	TerminateThread(handle_t1, 0); //Sau khi đã kết thúc trò chơi thì hủy thread

	if (t1.joinable())
	{
		t1.join(); //Join thread vào hàm Run
	}
}

//Input: None
//Output: None
//Chức năng: Vẽ tường rào (Khi bắt đầu chơi)
void GameManager::DrawBoard()
{
	system("cls"); //Xóa trắng màn hình
	if (this->_quit == false) //Kiểm tra trò chơi đã bắt đầu hay chưa
	{
		TextColor(78); //Màu của tường rào

		//Vẽ tường rào bên trên của trường chơi game
		GoToXY(0, 0); cout << "\xB2"; //Tới vị trí bên trái cùng của tường rào và vẽ lần lượt sang bên phải cùng

		for (int i = 1; i < this->_width; i++)
		{
			cout << "\xB2";
		}

		cout << '\xB2';

		//Vẽ tường rào bên dưới của trường thông tin về players và scores
		GoToXY(0, this->_height); cout << "\xB2"; //Tới vị trí bên trái cùng của tường rào và vẽ lần lượt sang bên phải cùng

		for (int i = 1; i < this->_width; i++)
		{
			cout << "\xB2";
		}

		cout << '\xB2';

		//Vẽ tường rào bên trên của trường thông tin về players và scores
		GoToXY(0, this->_height - 8); cout << "\xB2"; //Tới vị trí bên trái cùng của tường rào và vẽ lần lượt sang bên phải cùng

		for (int i = 1; i < this->_width; i++)
		{
			cout << "\xB2";
		}

		cout << '\xB2';

		//Vẽ tường rào ngăn cách giữa 2 phần trong trường thông tin
		for (int i = this->_height - 8; i < this->_height; i++)
		{
			GoToXY(this->_width / 2, i); cout << '\xB2'; //Tới từng vị trí của tường rào ngăn cách (Theo hướng dọc) và vẽ
		}

		//Vẽ tường rào ở 2 bên trái và bên phải
		for (int i = 1; i < this->_height; i++)
		{
			GoToXY(0, i); cout << '\xB2'; //Tới từng vị trí của tường rào bên trái (Theo hướng dọc) và vẽ
			GoToXY(this->_width, i); cout << "\xB2"; //Tới từng vị trí của tường rào bên phải (Theo hướng dọc) và vẽ
		}

		//"Player: " và tên của người chơi 1 (Bên trái) và người chơi 2 (Bên phải) chỉ cần vẽ 1 lần
		TextColor(14); //Màu của các dòng chữ

		GoToXY(2, this->_height - 7); //Tới vị trí để vẽ dòng chữ tên cho người chơi 1 và vẽ
		cout << "Player: ";
		cout << this->_player1->NamePlayer();

		GoToXY(this->_width / 2 + 2, this->_height - 7); //Tới vị trí để vẽ dòng chữ tên cho người chơi 2 và vẽ
		cout << "Player: ";
		cout << this->_player2->NamePlayer();

		//"Score: 0" của người chơi 1 (Bên trái) và người chơi 2 (Bên phải) chỉ cần vẽ 1 lần
		GoToXY(2, this->_height - 6); //Tới vị trí để vẽ dòng chữ điểm cho người chơi 1 và vẽ
		cout << "Score: "<<_score1;

		GoToXY(this->_width / 2 + 2, this->_height - 6); //Tới vị trí để vẽ dòng chữ điểm cho người chơi 2 và vẽ
		cout << "Score: "<<_score2;

		//Vẽ 2 thanh trượt lúc mới bắt đầu
		DrawPaddles("\xDB");
	}
}

//Input: None
//Output: None
//Chức năng: Vẽ quả bóng
void GameManager::DrawBall(const char* str)
{
	TextColor(14); //Màu của quả bóng

	//Lấy tọa độ x và y của quả bóng
	int ballx = this->_ball->X();
	int bally = this->_ball->Y();

	GoToXY(ballx, bally); //Tới vị trí của quả bóng
	cout << str; //Vẽ kí tự của quả bóng ra màn hình
}

//Input: None
//Output: None
//Chức năng: Vẽ 2 thanh trượt
void GameManager::DrawPaddles(const char* str)
{
	TextColor(14); //Màu của thanh trượt

	//Lấy tọa độ x của 2 thanh trượt
	int player1x = this->_player1->X();
	int player2x = this->_player2->X();

	//Lấy tọa độ y của 2 thanh trượt
	int player1y = this->_player1->Y();
	int player2y = this->_player2->Y();

	//Lấy chiều dài của 2 thanh trượt (2 thanh trượt có cùng 1 chiều dài)
	int length = this->_player1->Length();

	//Vẽ thanh trượt bên trái
	for (int i = 0; i < length; i++)
	{
		GoToXY(player1x, player1y + i); cout << str; //Tới từng vị trí của thanh trượt bên trái (Theo hướng dọc) và vẽ
	}

	//Vẽ thanh trượt bên phải
	for (int i = 0; i < length; i++)
	{
		GoToXY(player2x, player2y + i); cout << str; //Tới từng vị trí của thanh trượt bên phải (Theo hướng dọc) và vẽ
	}
}
//Input: None
//Output: None
//Chức năng: Vẽ điểm số của 2 bên

void GameManager::DrawScores()
{
	TextColor(14); //Màu của điểm số

	//Tới vị trí để vẽ điểm số cho người chơi 1 (Bên trái) và vẽ
	GoToXY(9, this->_height - 6);
	cout << this->_score1;

	//Tới vị trí để vẽ điểm số cho người chơi 2 (Bên phải) và vẽ
	GoToXY(this->_width / 2 + 9, this->_height - 6);
	cout << this->_score2;
}
void GameManager::DrawItem()
{
	TextColor(32);
	GoToXY(this->_item->X(), this->_item->Y());
	cout << "|" ;
	int f = rand() % 10;
	if (f == 5 || f == 9)
	{
		TextColor(78);
		GoToXY(this->_itemDownSpeed->X(), this->_itemDownSpeed->Y());
		cout << "|";
	}
	if (f == 2 || f == 3 || f == 4 || f == 9)
	{
		TextColor(49);
		GoToXY(this->_addScore->X(), this->_addScore->Y());
		cout << "|";
	}
	if (f == 6 || f == 7 || f == 4 || f == 9)
	{
		TextColor(98);
		GoToXY(this->_subtracScore->X(), this->_subtracScore->Y());
		cout << "|";
	}
}

//Input: None
//Output: None
//Chức năng: Xử lý Bot (Thanh trượt chạy tự động)
void GameManager::Bot()
{
	//Tạo số ngẫu nhiên theo kiểu của C++
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); //Tạo seed theo thời gian hiện tại
	std::default_random_engine generator(seed); //Tạo bộ sinh số ngẫu nhiên với seed vừa tạo
	//Distribution (Phân phối) để sinh ra 1 số ngẫu nhiên trong các số nguyên từ 1 đến 10
	std::uniform_int_distribution<int> distribution(1, 10);

	//Lấy tọa độ x, y và hướng bay của quả bóng
	int ballx = this->_ball->X();
	int bally = this->_ball->Y();
	int direction = this->_ball->Direction();

	//Lấy tọa độ x và y của thanh trượt bên phải
	int player2x = this->_player2->X();
	int player2y = this->_player2->Y();

	//Lấy chiều dài của thanh trượt bên phải
	int length = this->_player2->Length();

	if (ballx >= this->_width / 2 && direction >= 3) //Quả bóng đang nằm ở nửa bên phải của màn hình và đang bay về bên phải
	{
		//Vị trí của thanh trượt khi di chuyển đi lên phải nằm dưới tường rào ở phía trên thì mới di chuyển đi lên được
		//Và tọa độ y của quả bóng phải nằm từ nửa trên của thanh trượt trở lên thì mới xử lý cho di chuyển đi lên
		if (player2y > 1 && bally < player2y + 2)
		{
			//Nếu khoảng cách theo tọa độ y từ quả bóng tới phần chính giữa của thanh trượt > (Độ khó + 1)
			if ((player2y + 2) - bally > this->_difficulty + 1)
			{
				TextColor(14); //Màu của thanh trượt

				GoToXY(player2x, player2y + length - 1); cout << " "; //Xóa 1 kí tự ở dưới cùng của thanh trượt bên phải

				this->_player2->MoveUp(); //Di chuyển thanh trượt bên phải đi lên

				player2y--; //Giảm player2y xuống 1 đơn vị (Cùng với tọa độ y của _player2 đã giảm ở trên) để tránh bị lỗi

				GoToXY(player2x, player2y); cout << "\xDB"; //Vẽ thêm 1 kí tự ở trên cùng của thanh trượt bên phải
			}
			//Nếu khoảng cách theo tọa độ y từ quả bóng tới phần chính giữa của thanh trượt <= (Độ khó + 1)
			else
			{
				//Nếu chọn ngẫu nhiên 1 số trong các số nguyên từ 1 tới 10 mà >= Độ khó
				//Khả năng chọn đúng của các mức độ khó từ Easy đến Impossible lần lượt là 70%, 80%, 90%, 100%
				if (distribution(generator) >= this->_difficulty)
				{
					TextColor(14); //Màu của thanh trượt

					GoToXY(player2x, player2y + length - 1); cout << " "; //Xóa 1 kí tự ở dưới cùng của thanh trượt bên phải

					this->_player2->MoveUp(); //Di chuyển thanh trượt bên phải đi lên

					player2y--; //Giảm player2y xuống 1 đơn vị (Cùng với tọa độ y của _player2 đã giảm ở trên) để tránh bị lỗi

					GoToXY(player2x, player2y); cout << "\xDB"; //Vẽ thêm 1 kí tự ở trên cùng của thanh trượt bên phải
				}
			}
		}
		//Vị trí của thanh trượt khi di chuyển đi xuống phải nằm trên tường rào ở phía dưới thì mới di chuyển đi xuống được
		//Và tọa độ y của quả bóng phải nằm từ nửa dưới của thanh trượt trở xuống thì mới xử lý cho di chuyển đi xuống
		else if (player2y + length < this->_height - 8 && bally > player2y + 2)
		{
			//Nếu khoảng cách theo tọa độ y từ quả bóng tới phần chính giữa của thanh trượt > (Độ khó + 1)
			if (bally - (player2y + 2) > this->_difficulty + 1)
			{
				TextColor(14); //Màu của thanh trượt

				GoToXY(player2x, player2y); cout << " "; //Xóa 1 kí tự ở trên cùng của thanh trượt bên phải

				this->_player2->MoveDown(); //Di chuyển thanh trượt bên phải đi xuống

				player2y++; //Tăng player2y lên 1 đơn vị (Cùng với tọa độ y của _player2 đã tăng ở trên) để tránh bị lỗi

				GoToXY(player2x, player2y + length - 1); cout << "\xDB"; //Vẽ thêm 1 kí tự ở dưới cùng của thanh trượt bên phải
			}
			//Nếu khoảng cách theo tọa độ y từ quả bóng tới phần chính giữa của thanh trượt <= (Độ khó + 1)
			else
			{
				//Nếu chọn ngẫu nhiên 1 số trong các số nguyên từ 1 tới 10 mà >= Độ khó
				//Khả năng chọn đúng của các mức độ khó từ Easy đến Impossible lần lượt là 70%, 80%, 90%, 100%
				if (distribution(generator) >= this->_difficulty)
				{
					TextColor(14); //Màu của thanh trượt

					GoToXY(player2x, player2y); cout << " "; //Xóa 1 kí tự ở trên cùng của thanh trượt bên phải

					this->_player2->MoveDown(); //Di chuyển thanh trượt bên phải đi xuống

					player2y++; //Tăng player2y lên 1 đơn vị (Cùng với tọa độ y của _player2 đã tăng ở trên) để tránh bị lỗi

					GoToXY(player2x, player2y + length - 1); cout << "\xDB"; //Vẽ thêm 1 kí tự ở dưới cùng của thanh trượt bên phảiv
				}
			}
		}
	}
	else if (direction < 3) //Quả bóng đang bay về bên trái thì di chuyển thanh trượt về chính giữa
	{
		if (player2y > (this->_height - 8) / 2 - 2) //Nếu thanh trượt nằm ở nửa dưới của trường chơi game
		{
			TextColor(14); //Màu của thanh trượt

			GoToXY(player2x, player2y + length - 1); cout << " "; //Xóa 1 kí tự ở dưới cùng của thanh trượt bên phải

			this->_player2->MoveUp(); //Di chuyển thanh trượt bên phải đi lên

			player2y--; //Giảm player2y xuống 1 đơn vị (Cùng với tọa độ y của _player2 đã giảm ở trên) để tránh bị lỗi

			GoToXY(player2x, player2y); cout << "\xDB"; //Vẽ thêm 1 kí tự ở trên cùng của thanh trượt bên phải
		}
		else if (player2y < (this->_height - 8) / 2 - 2) //Nếu thanh trượt nằm ở nửa trên của trường chơi game
		{
			TextColor(14); //Màu của thanh trượt

			GoToXY(player2x, player2y); cout << " "; //Xóa 1 kí tự ở trên cùng của thanh trượt bên phải

			this->_player2->MoveDown(); //Di chuyển thanh trượt bên phải đi xuống

			player2y++; //Tăng player2y lên 1 đơn vị (Cùng với tọa độ y của _player2 đã tăng ở trên) để tránh bị lỗi

			GoToXY(player2x, player2y + length - 1); cout << "\xDB"; //Vẽ thêm 1 kí tự ở dưới cùng của thanh trượt bên phải
		}
	}
}

//Input: None
//Output: None
//Chức năng: Cố định kích thước và làm mờ nút phóng to
void FixConsoleWindow()
{
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}

//Input: None
//Output: None
//Chức năng: Di chuyển tới vị trí trong màn hình console
void GoToXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//Input: None
//Output: None
//Chức năng: Xoá con trỏ chuột trên màn hình console
void Nocursortype()
{
	CONSOLE_CURSOR_INFO	Info;
	Info.bVisible = FALSE;
	Info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}

//Input: Chỉ số của màu sắc (Kiểu int)
//Output: None
//Chức năng: Chỉnh sửa màu sắc
void TextColor(int x)
{
	HANDLE mau = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(mau, x);
}

//Input: Chiều rộng (width) và chiều cao (height) của console (Kiểu int)
//Output: None
//Chức năng: Chỉnh sửa lại kích thước console
void resizeConsole(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}

//Input: None
//Output: None
//Chức năng: Hiển thị menu game cho người chơi chọn.
void GameManager::DrawScreenGame()
{
	//Xoá màn hình console, chỉnh sửa lại kích thước màn hình console, cố định màn hình console, ẩn con trỏ màn hình.
	system("cls");
	resizeConsole(WIDTHSCREEN, HEIGHTSCREEN);
	FixConsoleWindow();
	Nocursortype();

	//Vẽ biên màn hình vào game
	TextColor(4);
	int count = 0;
	while (count < 35)
	{
		for (int i = 0; i < WIDTHCHAR - count * 2 - 20; i++)
		{
			GoToXY(i, HEIGHTCHAR - (count + 1)); printf("%c", 219);
		}
		count++;
	}
	TextColor(0);

	//Vẽ logo Ping Pong
	TextColor(78);
	GoToXY(8, 11); printf("%c%c%c%c%c%c%c %c%c%c%c%c%c%c   %c%c%c %c%c%c%c%c%c%c ", 219, 219, 219, 219, 219, 219, 187, 219, 219, 187, 219, 219, 219, 187, 219, 219, 187, 219, 219, 219, 219, 219, 219, 187);
	GoToXY(8, 12); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c ", 219, 219, 201, 205, 205, 219, 219, 187, 219, 219, 186, 219, 219, 219, 219, 187, 219, 219, 186, 219, 219, 201, 205, 205, 205, 205, 188);
	GoToXY(8, 13); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c %c%c%c%c%c%c  %c%c%c%c", 219, 219, 219, 219, 219, 219, 201, 188, 219, 219, 186, 219, 219, 201, 219, 219, 187, 219, 219, 186, 219, 219, 186, 219, 219, 219, 187);
	GoToXY(8, 14); printf("%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c   %c%c%c", 219, 219, 201, 205, 205, 205, 188, 219, 219, 186, 219, 219, 186, 200, 219, 219, 187, 219, 219, 186, 219, 219, 186, 219, 219, 186);
	GoToXY(8, 15); printf("%c%c%c     %c%c%c%c%c%c %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 219, 219, 186, 219, 219, 186, 219, 219, 186, 200, 219, 219, 219, 219, 186, 200, 219, 219, 219, 219, 219, 219, 201, 188);
	GoToXY(8, 16); printf("%c%c%c     %c%c%c%c%c%c  %c%c%c%c%c %c%c%c%c%c%c%c ", 200, 205, 188, 200, 205, 188, 200, 205, 188, 200, 205, 205, 205, 188, 200, 205, 205, 205, 205, 205, 188);

	GoToXY(18, 19); printf("%c%c%c%c%c%c%c  %c%c%c%c%c%c%c %c%c%c%c   %c%c%c %c%c%c%c%c%c%c ", 219, 219, 219, 219, 219, 219, 187, 219, 219, 219, 219, 219, 219, 187, 219, 219, 219, 187, 219, 219, 187, 219, 219, 219, 219, 219, 219, 187);
	GoToXY(18, 20); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c ", 219, 219, 201, 205, 205, 219, 219, 187, 219, 219, 201, 205, 205, 205, 219, 219, 187, 219, 219, 219, 219, 187, 219, 219, 186, 219, 219, 201, 205, 205, 205, 205, 188);
	GoToXY(18, 21); printf("%c%c%c%c%c%c%c%c%c%c%c   %c%c%c%c%c%c%c%c%c %c%c%c%c%c%c  %c%c%c%c", 219, 219, 219, 219, 219, 219, 201, 188, 219, 219, 186, 219, 219, 186, 219, 219, 201, 219, 219, 187, 219, 219, 186, 219, 219, 186, 219, 219, 219, 187);
	GoToXY(18, 22); printf("%c%c%c%c%c%c%c %c%c%c   %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c   %c%c%c", 219, 219, 201, 205, 205, 205, 188, 219, 219, 186, 219, 219, 186, 219, 219, 186, 200, 219, 219, 187, 219, 219, 186, 219, 219, 186, 219, 219, 186);
	GoToXY(18, 23); printf("%c%c%c     %c%c%c%c%c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 219, 219, 186, 200, 219, 219, 219, 219, 219, 219, 201, 188, 219, 219, 186, 200, 219, 219, 219, 219, 186, 200, 219, 219, 219, 219, 219, 219, 201, 188);
	GoToXY(18, 24); printf("%c%c%c      %c%c%c%c%c%c%c %c%c%c  %c%c%c%c%c %c%c%c%c%c%c%c ", 200, 205, 188, 200, 205, 205, 205, 205, 205, 188, 200, 205, 188, 200, 205, 205, 205, 188, 200, 205, 205, 205, 205, 205, 188);
	TextColor(0);

	//Tạo các thanh lựa chọn đang chở ở NEW GAME
	TextColor(236);
	GoToXY(70, 13); printf("%c   NEW GAME    ", 175);
	TextColor(78);
	GoToXY(70, 15); printf("     LOAD GAME  ");
	GoToXY(70, 17); printf("   INSTRUCTION  ");
	GoToXY(70, 19); printf("    ABOUT US    ");
	GoToXY(70, 21); printf("    QUIT        ");
	TextColor(0);

	//Xử lý chọn các thanh lựa chọn.
	CONTROLMENU press = NONEe;
	int toado = 15;
	do
	{
		//Kiểm tra xem người dùng bấm phím nào
		char a = _getch();
		if (a == 's' || a == 'S')
		{
			press = DOWNe;
		}
		else if (a == 'w' || a == 'W')
		{
			press = UPe;
		}
		else if (a == 13)
		{
			press = ENTERe;
		}
		else
		{
			press = NONEe;
		}


		if (press == DOWNe && toado < 19)
		{
			toado++;
		}
		else if (press == UPe && toado > 15)
		{
			toado--;
		}

		if (toado == 15)
		{

			TextColor(236);
			GoToXY(70, 13); printf("%c   NEW GAME    ", 175);
			TextColor(78);
			GoToXY(70, 15); printf("     LOAD GAME  ");
			GoToXY(70, 17); printf("   INSTRUCTION  ");
			GoToXY(70, 19); printf("    ABOUT US    ");
			GoToXY(70, 21); printf("    QUIT        ");

			TextColor(0);
		}
		if (toado == 16)
		{
			TextColor(78);
			GoToXY(70, 13); printf("    NEW GAME    ");
			TextColor(236);
			GoToXY(70, 15); printf("%c     LOAD GAME ",175);
			TextColor(78);
			GoToXY(70, 17); printf("   INSTRUCTION  ");
			GoToXY(70, 19); printf("    ABOUT US    ");
			GoToXY(70, 21); printf("    QUIT        ");

			TextColor(0);
		}
		if (toado == 17)
		{
			TextColor(78);
			GoToXY(70, 13); printf("    NEW GAME    ");
			GoToXY(70, 15); printf("     LOAD GAME  ");
			TextColor(236);
			GoToXY(70, 17); printf("%c   INSTRUCTION ",175);
			TextColor(78);
			GoToXY(70, 19); printf("    ABOUT US    ");
			GoToXY(70, 21); printf("    QUIT        ");
			TextColor(0);
		}
		if (toado == 18)
		{
			TextColor(78);
			GoToXY(70, 13); printf("    NEW GAME    ");
			GoToXY(70, 15); printf("     LOAD GAME  ");
			GoToXY(70, 17); printf("   INSTRUCTION  ");
			TextColor(236);
			GoToXY(70, 19); printf("%c    ABOUT US   ",175);
			TextColor(78);
			GoToXY(70, 21); printf("   QUIT         ");
			TextColor(0);
		}
		if (toado == 19)
		{
			TextColor(78);
			GoToXY(70, 13); printf("    NEW GAME    ");
			GoToXY(70, 15); printf("     LOAD GAME  ");
			GoToXY(70, 17); printf("   INSTRUCTION  ");
			GoToXY(70, 19); printf("    ABOUT US    ");
			TextColor(236);
			GoToXY(70, 21); printf("%c   QUIT        ", 175);
			TextColor(0);
		}
	} while (press != ENTERe);


	if (toado == 15)
	{
		//Vào NewGame()
		NewGame();
		return;
	}
	else if (toado == 16)
	{
		LoadGame();
		return;
	}
	else if (toado == 17)
	{

		//Vào Instruction()
		Instruction();
		return;
	}
	else if (toado == 18)
	{
		AboutUs();
		return;
	}
	else
	{
		//QuitGame()
		QuitGame();
		return;
	}
	return;
}

//Input: None
//Output: None
//Chức năng: Hiển thị thông tin cách chơi, các phím chức năng của trò chơi.
void GameManager::Instruction()
{
	GoToXY(70, 13); printf("                ");
	GoToXY(70, 15); printf("                ");
	GoToXY(70, 17); printf("                ");
	GoToXY(70, 19); printf("                ");
	GoToXY(70, 21); printf("                ");

	TextColor(199);
	GoToXY(73, 1);  printf("   INSTRUCTION  ");
	GoToXY(60, 3); TextColor(10); printf("/*Press anykey to return to menu game*/");

	GoToXY(55, 5);
	TextColor(236); printf("The first player to score 5 points wins the game.");


	GoToXY(53, 6);
	TextColor(6); printf("There are two modes to play:");
	GoToXY(53, 7);
	TextColor(27); printf("1. MultiPlayer (Player1 vs Player2):");
	GoToXY(58, 8);
	TextColor(11); printf("+ Player 1 uses 'W' and 'S' buttons to move the");
	GoToXY(53, 9);
	TextColor(11);
	printf("left paddle upwards and downwards.");

	GoToXY(58, 10);
	TextColor(11);
	printf("+ Player 2 uses Up and Down arrow buttons to move");

	GoToXY(53, 11);
	TextColor(11);
	printf("the right paddle upwards and downwards.");

	GoToXY(53, 12);
	TextColor(27); printf("2. SinglePlayer (Player vs BOT):");

	GoToXY(58, 13);
	TextColor(11); printf("+ Player uses 'W' and 'S' buttons to move the left");

	GoToXY(53, 14);
	TextColor(11); printf("paddle upwards and downwards.");

	GoToXY(53, 15);
	TextColor(6); printf("There are some special buttons that you can use while");
	GoToXY(53, 16);
	TextColor(6); printf("playing the game:");
	GoToXY(58, 17);
	TextColor(11); printf("+ Press the 'P' button to pause the game.");

	GoToXY(58, 18);
	TextColor(11); printf("+ Press the 'Q' button to quit the game.");

	GoToXY(60, 19);
	TextColor(236); printf("Attention: Turn off Unikey before playing.");

	TextColor(0);

	char a = NULL;
	do
	{
		a = _getch();
		if (a != NULL)
		{
			DrawScreenGame();
			break;
		}
	} while (1);
	TextColor(0);
}

//Input: None
//Output: None
//Chức năng: Hiển thị thông tin của các nhà phát triển.
void GameManager::AboutUs()
{

	GoToXY(70, 13); printf("                ");
	GoToXY(70, 15); printf("                ");
	GoToXY(70, 17); printf("                ");
	GoToXY(70, 19); printf("                ");
	GoToXY(70, 21); printf("                ");


	TextColor(199);
	GoToXY(71, 4);
	cout << "   ABOUT US   ";
	GoToXY(58, 6); TextColor(10); printf("/*Press anykey to return to menu game*/");
	TextColor(27);
	GoToXY(45 - 8, 8); cout << "Dev1:";
	TextColor(0);
	GoToXY(47 - 8, 9); TextColor(11); printf("Full Name:"); GoToXY(72, 9); TextColor(6); printf("Mai Trung Dong ");
	GoToXY(49 - 8, 10); TextColor(11); printf("Birthday:"); GoToXY(72, 10); TextColor(6); printf("02/09/2000");
	GoToXY(51 - 8, 11); TextColor(11); printf("Facebook:"); GoToXY(72, 11); TextColor(6); printf("www.fb.com/ChunDon.Mai");
	GoToXY(53 - 8, 12); TextColor(11); printf("Gmail:"); GoToXY(72, 12); TextColor(6); printf("maitrungdong.102.mtd@gmail.com");
	GoToXY(55 - 8, 13); TextColor(11); printf("School:"); GoToXY(72, 13); TextColor(6); printf("HCM University Of Science");
	GoToXY(57 - 8, 14); TextColor(11); printf("Job:"); GoToXY(72, 14); TextColor(6); printf("student");

	TextColor(27);
	GoToXY(61 - 8, 16); cout << "Dev2:";
	GoToXY(63 - 8, 17); TextColor(11); printf("Full Name:"); GoToXY(72, 17); TextColor(6); printf("Nguyen Hong Duc ");
	GoToXY(65 - 8, 18); TextColor(11); printf("Birthday:"); GoToXY(72, 18); TextColor(6); printf("24/11/2000");
	GoToXY(67 - 8, 19); TextColor(11); printf("Facebook:"); GoToXY(72, 19); TextColor(6); printf("www.fb.com/naturemother");
	GoToXY(69 - 8, 20); TextColor(11); printf("Gmail:"); GoToXY(72, 20); TextColor(6); printf("motherofsuper2@gmail.com");
	GoToXY(71 - 8, 21); TextColor(11); printf("School:"); GoToXY(72, 21); TextColor(6); printf("HCM University Of Science");
	GoToXY(73 - 8, 22); TextColor(11); printf("Job:"); GoToXY(72, 22); TextColor(6); printf("student");

	char a = NULL;
	do
	{
		a = _getch();
		if (a != NULL)
		{
			DrawScreenGame();
			break;
		}
	} while (1);
	TextColor(0);
}

//Input: Tọa độ x và y ở phần nhập tên trên màn hình console (Kiểu int)
//Output: Chuỗi tên của người dùng (Kiểu string)
//Chức năng: Lấy vào tên hợp lệ khi người dùng nhập vào
string GameManager::GetNamePlayer(int x, int y)
{
	string name_player;
	int nameplayer_size = 0;
	TextColor(11);
	GoToXY(x + nameplayer_size, y);
	do
	{
		char buffer = _getch();
		if (1 <= nameplayer_size && nameplayer_size <= 12 && buffer == '\r')
		{
			break;
		}
		else
		{
			if (isCharacterValid(buffer) == -1)
			{
				if (nameplayer_size > 0)
				{
					GoToXY(x + nameplayer_size, y); cout << " ";
					GoToXY(x + nameplayer_size, y);
					nameplayer_size--;
					name_player.resize(nameplayer_size);
				}
			}
			if (nameplayer_size < 12)
			{
				if (isCharacterValid(buffer) == 1)
				{
					nameplayer_size++;
					name_player.resize(nameplayer_size);
					name_player[nameplayer_size - 1] = buffer;
					GoToXY(x + nameplayer_size, y); cout << buffer;
				}

			}
		}
	} while (true);
	TextColor(0);
	return name_player;
}

void GameManager::X2Score(int lastTouch)
{
	if (lastTouch == -1)
	{
		this->_score1 *= 2;
	}
	if (lastTouch == 1)
	{
		this->_score2 *= 2;
	}
	this->_addScore->SetPosition(0, 0);
	TextColor(14);

	GoToXY(9, this->_height - 6);
	cout << " ";
	GoToXY(9, this->_height - 6);
	cout << this->_score1;

	GoToXY(this->_width / 2 + 9, this->_height - 6);
	cout << " ";
	GoToXY(this->_width / 2 + 9, this->_height - 6);
	cout << this->_score2;
}

void GameManager::XHalfScore(int lastTouch)
{
	if (this->lastTouch == -1)
	{
		this->_score1 *= 0.5;
	}
	if (this->lastTouch == 1)
	{
		this->_score2 *= 0.5;
	}
	this->_subtracScore->SetPosition(0, 0);
	TextColor(14);

	GoToXY(9, this->_height - 6);
	cout << " ";
	GoToXY(9, this->_height - 6);
	cout << this->_score1;

	GoToXY(this->_width / 2 + 9, this->_height - 6);
	cout << " ";
	GoToXY(this->_width / 2 + 9, this->_height - 6);
	cout << this->_score2;
}

//Input: None
//Output: None
//Chức năng: Hiển thị menu new game cho người chơi chọn những lựa chọn.
void GameManager::NewGame()
{
	//Xoá các thanh lựa chọn để vẽ các thanh lựa chọn trong chế độ new game.
	GoToXY(70, 13); printf("                ");
	GoToXY(70, 15); printf("                ");
	GoToXY(70, 17); printf("                ");
	GoToXY(70, 19); printf("                ");
	GoToXY(70, 21); printf("                ");

	TextColor(199);
	GoToXY(70, 7); printf("    NEW GAME    ");
	TextColor(236);
	GoToXY(70 - 9, 10); printf("%c SINGLE PLAYER ", 175);
	TextColor(78);
	GoToXY(70 + 9, 10); printf("  MULTIPLAYER   ");
	TextColor(0);

	//Xử lý lựa chọn của người dùng.
	CONTROLMENU press = NONEe;
	int toado = 15;
	do
	{
		char a = _getch();
		if (a == 'd' || a == 'D')
		{
			press = LEFTe;
		}
		else if (a == 'a' || a == 'A')
		{
			press = RIGHTe;
		}
		else if (a == 13)
		{
			press = ENTERe;
		}
		else
		{
			press = NONEe;
		}

		if (press == LEFTe && toado < 16)
		{
			toado++;
		}
		if (press == RIGHTe && toado > 15)
		{
			toado--;
		}

		if (toado == 15)
		{
			TextColor(236);
			GoToXY(70 - 9, 10); printf("%c SINGLE PLAYER ", 175);
			TextColor(78);
			GoToXY(70 + 9, 10); printf("  MULTIPLAYER   ");
			TextColor(0);
		}
		if (toado == 16)
		{
			TextColor(78);
			GoToXY(70 - 9, 10); printf("  SINGLE PLAYER ");
			TextColor(236);
			GoToXY(70 + 9, 10); printf("%c MULTIPLAYER   ", 175);
			TextColor(0);

		}
	} while (press != ENTERe);

	//Nếu người chơi chọn chơi một người.
	if (toado == 15)
	{
		GoToXY(70 - 9, 10); printf("                ");
		GoToXY(70 + 9, 10); printf("                ");

		TextColor(9);
		GoToXY(70 + 7, 8); printf("%c", 179);
		GoToXY(70 + 7, 9); printf("v");
		TextColor(236);
		GoToXY(70, 10); printf("  SINGLE PLAYER ");
		TextColor(0);

		TextColor(10);
		GoToXY(52, 14); printf("*Note: Character string cannot exceed 12 characters!!!");
		TextColor(0);

		//Lưu tên người chơi
		TextColor(27);
		GoToXY(57, 12); printf("Player's name:");
		TextColor(0);   printf(" ");

		//Xử lý lấy tên của người chơi sao cho nó chỉ có nhiều nhất 12 kí tự và ít nhất là 1 kí tự.
		//Tạo một mảng char

		string name_player = GetNamePlayer(57 + 14, 12);
		this->_player1->SetNamePlayer(name_player);
		this->_isSinglePlayer = true;

		//Chọn cấp độ cho con BOT
		GoToXY(57 + 15, 12); printf("            ");
		GoToXY(57, 12); printf("              ");
		GoToXY(52, 14); printf("                                                      ");

		TextColor(9);
		GoToXY(70 + 7, 11); printf("%c", 179);
		GoToXY(70 + 7, 12); printf("v");
		TextColor(172);
		GoToXY(70, 13); printf("   Difficulty   ");
		TextColor(0);

		TextColor(236);
		GoToXY(72, 15);  printf("%c   Easy    ", 175);
		TextColor(78);
		GoToXY(72, 16);  printf("   Normal   ");
		GoToXY(72, 17);  printf("    Hard    ");
		GoToXY(72, 18);  printf(" Impossible ");
		TextColor(0);

		int toadoLevel = 20;
		do
		{
			char a = _getch();
			if (a == 's' || a == 'S')
			{
				press = DOWNe;
			}
			else if (a == 'w' || a == 'W')
			{
				press = UPe;
			}
			else if (a == 13)
			{
				press = ENTERe;
			}
			else
			{
				press = NONEe;
			}

			if (press == DOWNe && toadoLevel < 23)
			{
				toadoLevel++;
			}
			if (press == UPe && toadoLevel > 20)
			{
				toadoLevel--;
			}

			if (toadoLevel == 20)
			{
				TextColor(236);
				GoToXY(72, 15);  printf("%c   Easy    ", 175);
				TextColor(78);
				GoToXY(72, 16);  printf("   Normal   ");
				GoToXY(72, 17);  printf("    Hard    ");
				GoToXY(72, 18);  printf(" Impossible ");
				TextColor(0);
			}
			if (toadoLevel == 21)
			{
				TextColor(78);
				GoToXY(72, 15);  printf("    Easy    ");
				TextColor(236);
				GoToXY(72, 16);  printf("%c  Normal   ", 175);
				TextColor(78);
				GoToXY(72, 17);  printf("    Hard    ");
				GoToXY(72, 18);  printf(" Impossible ");
				TextColor(0);
			}
			if (toadoLevel == 22)
			{
				TextColor(78);
				GoToXY(72, 15);  printf("    Easy    ");
				GoToXY(72, 16);  printf("   Normal   ");
				TextColor(236);
				GoToXY(72, 17);  printf("%c   Hard    ", 175);
				TextColor(78);
				GoToXY(72, 18);  printf(" Impossible ");
				TextColor(0);
			}
			if (toadoLevel == 23)
			{
				TextColor(78);
				GoToXY(72, 15);  printf("    Easy    ");
				GoToXY(72, 16);  printf("   Normal   ");
				GoToXY(72, 17);  printf("    Hard    ");
				TextColor(236);
				GoToXY(72, 18);  printf("%cImpossible ", 175);
				TextColor(0);
			}

		} while (press != ENTERe);

		//Xử lý chọn mức độ của con BOT
		//Mức độ khó của BOT (Có các mức sau: 4: Dễ, 3: Trung bình, 2: Khó) (1 là không thể đánh bại!!!!!)

		if (toadoLevel == 20)
		{
			this->_difficulty = 4;
			this->_player2->SetNamePlayer("BOT (Easy)");
			return;
		}
		else if (toadoLevel == 21)
		{
			this->_difficulty = 3;
			this->_player2->SetNamePlayer("BOT (Normal)");
			return;
		}
		else if (toadoLevel == 22)
		{
			this->_difficulty = 2;
			this->_player2->SetNamePlayer("BOT (Hard)");
			return;
		}
		else if (toadoLevel == 23)
		{
			this->_difficulty = 1;
			this->_player2->SetNamePlayer("BOT (Impossible)");
			return;
		}
	}

	//Nếu người chơi chọn chế độ chơi đối kháng hai người.
	if (toado == 16)
	{
		this->_isSinglePlayer = false;

		GoToXY(70 - 9, 10); printf("                ");
		GoToXY(70 + 9, 10); printf("                ");

		TextColor(9);
		GoToXY(70 + 7, 8); printf("%c", 179);
		GoToXY(70 + 7, 9); printf("v");
		TextColor(236);
		GoToXY(70, 10); printf("  MULTIPLAYER   ");
		TextColor(0);

		TextColor(10);
		GoToXY(54, 16); printf("*Note: Character string cannot exceed 12 characters!!!");
		TextColor(0);

		//Lưu tên người chơi 1
		TextColor(27);
		GoToXY(56, 12); printf("Player1's name:");
		TextColor(0); printf(" ");

		string nameplayer1 = GetNamePlayer(57 + 14, 12);
		this->_player1->SetNamePlayer(nameplayer1);

		//Lưu tên người chơi 2
		TextColor(27);
		GoToXY(56, 14); printf("Player2's name:");
		TextColor(0); printf(" ");

		string nameplayer2 = GetNamePlayer(57 + 14, 14);
		this->_player2->SetNamePlayer(nameplayer2);
		return;
	}
}

//Input: None
//Output: None
//Chức năng: Hiển thị menu new game cho người chơi chọn những lựa chọn.
void GameManager::LoadGame()
{
	string filename;
	int x, y;

	GoToXY(70, 13); printf("                ");
	GoToXY(70, 15); printf("                ");
	GoToXY(70, 17); printf("                ");
	GoToXY(70, 19); printf("                ");
	GoToXY(70, 21); printf("                ");

	TextColor(199);
	GoToXY(70, 7); printf("    LOAD GAME    ");
	TextColor(236);
	GoToXY(70 - 9, 13); printf("FILE NAME:");
	getline(cin, filename);
	filename = filename + ".txt";

	ifstream file;
	file.open(filename, ios::in);
	if (!file)
	{
		GoToXY(70, 15);
		cout << "Khong ton tai file nay !" << endl;
		Sleep(1000);
		TextColor(0);
		DrawScreenGame();
	}
	else
	{
		string line_info;
		getline(file, line_info);
		vector<string> tokens;

		string seperator = ",";
		int startPos = 0;
	
		size_t foundPos = line_info.find(seperator, startPos);

		while (foundPos != string::npos)
		{
			int count = foundPos - startPos;
			string token = line_info.substr(startPos, count);

			tokens.push_back(token);
			startPos = foundPos + seperator.length();
			foundPos = line_info.find(seperator, startPos);
		}

		int count = line_info.length() - startPos;
		string token = line_info.substr(startPos, count);
		tokens.push_back(token);

		if (tokens[0] == "1")
		{
			_isSinglePlayer = true;
			_difficulty = stoi(tokens[1]);

			_score1 = stoi(tokens[2]);
			_player1->SetNamePlayer(tokens[3]);

			_score2 = stoi(tokens[4]);
			_player2->SetNamePlayer(tokens[5]);

			_ball->SetVelocity(stof(tokens[6]));

		}
		else
		{
			_isSinglePlayer = false;

			_score1 = stoi(tokens[2]);
			_player1->SetNamePlayer(tokens[3]);

			_score2 = stoi(tokens[4]);
			_player2->SetNamePlayer(tokens[5]);

			_ball->SetVelocity(stof(tokens[6]));
		}

		GoToXY(70, 15);
		cout << "Doc thanh cong" << endl;
		Sleep(1000);
	}

	TextColor(0);
	
	file.close();
	
}

//Input: None
//Output: None
void GameManager::SaveGame()
{
	TextColor(12); GoToXY(this->_width / 2 - 3, this->_height / 2 - 7); printf("SAVE GAME");
	TextColor(140);
	GoToXY(this->_width / 2 - 9, this->_height / 2 - 5); printf("File name: ");

	string fileName;
	getline(cin, fileName);
	fileName = fileName + ".txt";

	ofstream file;
	file.open(fileName, ios::out | ios::trunc);
	GoToXY(this->_width / 2 - 4, this->_height / 2 - 7);
	if (!file)
	{
		GoToXY(this->_width / 2 - 4, this->_height / 2 - 3);
		cout << "Could not save";
	}
	else
	{
		GoToXY(this->_width / 2 - 4, this->_height / 2 - 3);
		if (_isSinglePlayer == true)
		{
			file << _isSinglePlayer << "," << _difficulty << ","
				<< _score1 << "," << _player1->NamePlayer() << ","
				<< _score2 << "," << _player2->NamePlayer() << ","
				<< _ball->Velocity()<<endl;
		}
		else
		{
			file << _isSinglePlayer << "," << "," 
				<< _score1 << "," << _player1->NamePlayer() << ","
				<< _score2 << "," << _player2->NamePlayer() << ","
				<< _ball->Velocity() << endl;
		}

		cout << "Sucessfully";
	}
	file.close();
	Sleep(1000);

}

//Input: Kí tự trong tên
//Output: Trả về 1 là đúng, trả về 0 là sai, trả về -1 là phím ENTER
//Chức năng: Kiểm tra kí tự trong tên có hợp lệ không
int isCharacterValid(char value)
{
	int check;
	if ('a' <= value && value <= 'z' || 'A' <= value && value <= 'Z' || '0' <= value && value <= '9')
	{
		check = 1;
	}
	else if (value == '\b')
	{
		check = -1;
	}
	else
	{
		check = 0;
	}
	return check;
}

//Input: Handle của thread (Kiểu HANDLE)
//Output: None
//Chức năng: Xử lý khi game kết thúc (thắng, thua)
void GameManager::EndGame(HANDLE t)
{
	//Dừng thread để xử lý cái đã.
	SuspendThread(t);

	//In thêm một lần để tránh trường hợp xấu xảy ra
	DrawBall("O");
	DrawPaddles("\xDB");

	TextColor(206);
	for (int j = 1; j <= 10; j++)
	{
		GoToXY(this->_width / 2 - 15 + 1, this->_height / 2 - 10 + j); printf("%c", 178);
		GoToXY(this->_width / 2 - 15 + 30, this->_height / 2 - 10 + j); printf("%c", 178);
	}
	for (int i = 1; i <= 30; i++)
	{
		GoToXY(this->_width / 2 - 15 + i, this->_height / 2 - 10 + 1); printf("%c", 178);
		GoToXY(this->_width / 2 - 15 + i, this->_height / 2 - 10 + 10); printf("%c", 178);
	}
	TextColor(0);

	TextColor(236);
	for (int i = 2; i < 30; i++)
	{
		for (int j = 2; j < 10; j++)
		{
			GoToXY(this->_width / 2 - 15 + i, this->_height / 2 - 10 + j);
			printf("%c", 219);
		}
	}
	TextColor(0);

	//Chơi với máy, thực tế máy cũng chỉ là một còn người giả mà thôi
	if (this->_isSinglePlayer == true)
	{
		if (this->_score2 == 5)
		{
			TextColor(12);
			GoToXY(this->_width / 2 - 7, this->_height / 2 - 7);
			printf("You have lost!!!");
			GoToXY(this->_width / 2 - 12, this->_height / 2 - 5);
			printf("Do you want to play again?");
			TextColor(0);
		}
		else
		{
			TextColor(12);
			GoToXY(this->_width / 2 - 7, this->_height / 2 - 7);
			printf("You have won!!!");
			GoToXY(this->_width / 2 - 12, this->_height / 2 - 5);
			printf("Do you want to play again?");
			TextColor(0);
		}
	}
	else if (this->_isSinglePlayer == false)
	{
		if (this->_score1 == 5)
		{
			TextColor(12);
			GoToXY(this->_width / 2 - 12, this->_height / 2 - 7);
			cout << this->_player1->NamePlayer();
			printf(" have won!!!");
			GoToXY(this->_width / 2 - 12, this->_height / 2 - 5);
			printf("Do you want to play again?");
			TextColor(0);
		}
		else
		{
			TextColor(12);
			GoToXY(this->_width / 2 - 12, this->_height / 2 - 7);
			cout << this->_player2->NamePlayer();
			printf(" have won!!!");
			GoToXY(this->_width / 2 - 12, this->_height / 2 - 5);
			printf("Do you want to play again?");
			TextColor(0);
		}

	}
	TextColor(124);
	GoToXY(this->_width / 2 - 9, this->_height / 2 - 3); printf("%c Play  ", 175);
	TextColor(140);
	GoToXY(this->_width / 2 + 3, this->_height / 2 - 3); printf("  Menu  ");
	TextColor(0);

	CONTROLMENU press = NONEe;
	int toado = 15;
	do
	{
		char a = _getch();
		if (a == 'd' || a == 'D')
		{
			press = RIGHTe;
		}
		else if (a == 'a' || a == 'A')
		{
			press = LEFTe;
		}
		else if (a == 13)
		{
			press = ENTERe;
		}
		else
		{
			press = NONEe;
		}

		if (press == RIGHTe && toado < 16)
		{
			toado++;
		}
		if (press == LEFTe && toado > 15)
		{
			toado--;
		}

		if (toado == 15)
		{
			TextColor(124);
			GoToXY(this->_width / 2 - 9, this->_height / 2 - 3); printf("%c Play  ", 175);
			TextColor(140);
			GoToXY(this->_width / 2 + 3, this->_height / 2 - 3); printf("  Menu  ");
			TextColor(0);
		}
		if (toado == 16)
		{
			TextColor(140);
			GoToXY(this->_width / 2 - 9, this->_height / 2 - 3); printf("  Play  ");
			TextColor(124);
			GoToXY(this->_width / 2 + 3, this->_height / 2 - 3); printf("%c Menu  ", 175);
			TextColor(0);
		}
	} while (press != ENTERe);

	if (toado == 15)
	{
		//Chơi lại
		this->_score1 = 0;
		this->_score2 = 0;
		this->_ball->RandomPos(this->_height - 8);
		this->_ball->ResetData();
		this->_player1->ResetData();
		this->_player2->ResetData();
		DrawBoard();
		ResumeThread(t);
		return;
	}
	else if (toado == 16)
	{
		//Về menu chính
		DrawScreenGame();
		this->_score1 = 0;
		this->_score2 = 0;
		this->_ball->RandomPos(this->_height - 8);
		this->_ball->ResetData();
		this->_player1->ResetData();
		this->_player2->ResetData();
		DrawBoard();
		ResumeThread(t);
		return;
	}
}

//Input: Handle của thread (Kiểu HANDLE)
//Output: None
//Chức năng: Xử lý tạm dừng (pause) game
void GameManager::PauseGame(HANDLE t)
{
	//Dừng thread
	SuspendThread(t);

	//In thêm một lần để tránh trường hợp xấu xảy ra
	DrawBall("O");
	DrawPaddles("\xDB");

	//In bên trong bảng pausegame
	TextColor(236);
	for (int i = 2; i <= 24; i++)
	{
		for (int j = 3; j <= 10; j++)
		{
			GoToXY(this->_width / 2 - 12 + i, this->_height / 2 - 11 + j);
			printf("%c", 219);
		}
	}
	TextColor(0);

	//In viền bảng pausegame
	TextColor(206);
	for (int j = 1; j <=8 ; j++)
	{
		GoToXY(this->_width / 2 - 12 + 1, this->_height / 2 - 9 + j); printf("%c", 178);
		GoToXY(this->_width / 2 - 12 + 25, this->_height / 2 - 9 + j); printf("%c", 178);
	}
	for (int i = 1; i <= 25; i++)
	{
		GoToXY(this->_width / 2 - 12 + i, this->_height / 2 - 11 + 2); printf("%c", 178);
		GoToXY(this->_width / 2 - 12 + i, this->_height / 2 - 11 + 11); printf("%c", 178);
	}
	TextColor(0);

	//In nội dung bên trong bảng pausegame
	TextColor(12); GoToXY(this->_width / 2 - 3, this->_height / 2 - 7); printf("PAUSED!!!");
	TextColor(124);
	GoToXY(this->_width / 2 - 9, this->_height / 2 - 5); printf("%c Resume ", 175);
	TextColor(140);
	GoToXY(this->_width / 2 + 3, this->_height / 2 - 5); printf("   Menu  ");
	TextColor(140);
	GoToXY(this->_width / 2 - 3, this->_height / 2 - 3); printf("   Save  ");
	TextColor(0);

	//Xử lý các nút trong bảng pausegame
	CONTROLMENU press = NONEe;
	int toado = 15;
	do
	{
		char a = _getch();
		if (a == 'd' || a == 'D')
		{
			press = LEFTe;
		}
		else if (a == 'a' || a == 'A')
		{
			press = RIGHTe;
		}
		else if (a == 13)
		{
			press = ENTERe;
		}
		else if (a == 's' || a == 'S')
		{
			press = UPe;
		}
		else if (a == 'w' || a == 'W')
		{
			press = DOWNe;
		}
		else
		{
			press = NONEe;
		}

		if (press == LEFTe && toado < 16)
		{
			toado++;
		}
		if (press == RIGHTe &&  toado > 15)
		{
			toado--;
		}
		if (press == UPe && toado < 17)
		{
			toado = 17;
		}
		if (press == DOWNe && toado == 17)
		{
			toado = 15;
		}

		if (toado == 15)
		{
			TextColor(124);
			GoToXY(this->_width / 2 - 9, this->_height / 2 - 5); printf("%c Resume ", 175);
			TextColor(140);
			GoToXY(this->_width / 2 + 3, this->_height / 2 - 5); printf("   Menu  ");
			GoToXY(this->_width / 2 - 3, this->_height / 2 - 3); printf("   Save  ");
			TextColor(0);
		}
		if (toado == 16)
		{
			TextColor(140);
			GoToXY(this->_width / 2 - 9, this->_height / 2 - 5); printf("  Resume ");
			TextColor(124);
			GoToXY(this->_width / 2 + 3, this->_height / 2 - 5); printf("%c  Menu  ", 175);
			TextColor(140);
			GoToXY(this->_width / 2 - 3, this->_height / 2 - 3); printf("   Save  ");
			TextColor(0);
			TextColor(0);

		}
		if (toado == 17)
		{
			TextColor(140);
			GoToXY(this->_width / 2 - 9, this->_height / 2 - 5); printf("  Resume ");
			GoToXY(this->_width / 2 + 3, this->_height / 2 - 5); printf("   Menu  ");
			TextColor(124);
			GoToXY(this->_width / 2 - 3, this->_height / 2 - 3); printf("%c   Save ",175);

		}
	} while (press != ENTERe);

	if (toado == 15)
	{
		//Xoá board pausegame
		for (int i = 1; i <= 25; i++)
		{
			for (int j = 2; j <= 11; j++)
			{
				GoToXY(this->_width / 2 - 12 + i, this->_height / 2 - 11 + j);
				printf(" ");
			}
		}

		ResumeThread(t);
		DrawBall(" ");
		
		return;
	}
	else if (toado == 16)
	{
		//Về menu chính
		this->_score1 = 0;
		this->_score2 = 0;
		this->_ball->RandomPos(this->_height - 8);
		this->_ball->ResetData();
		this->_player1->ResetData();
		this->_player2->ResetData();
		DrawScreenGame();
		DrawBoard();
		ResumeThread(t);
		return;
	}
	else if (toado == 17)
	{
		//In bên trong bảng pausegame
		TextColor(236);
		for (int i = 2; i <= 24; i++)
		{
			for (int j = 3; j <= 10; j++)
			{
				GoToXY(this->_width / 2 - 12 + i, this->_height / 2 - 11 + j);
				printf("%c", 219);
			}
		}
		SaveGame();
		TextColor(0);
		for (int i = 1; i <= 25; i++)
		{
			for (int j = 2; j <= 11; j++)
			{
				GoToXY(this->_width / 2 - 12 + i, this->_height / 2 - 11 + j);
				printf(" ");
			}
		}
		ResumeThread(t);
		DrawBall(" ");
		return;
	}
}

//Input: None
//Output: None
//Chức năng: Xử lý Thoát (Quit) game
void GameManager::QuitGame()
{
	TextColor(0);
	this->_quit = true;
}
