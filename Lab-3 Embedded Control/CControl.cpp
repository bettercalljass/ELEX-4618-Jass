#include "stdafx.h"
#include "CControl.h"
#include "Serial.h"
#include <iostream>
#include <thread>
#include <conio.h>

#define BAUD_RATE 115200
#define JOYSTICK_MAX 4095.0f
#define JOY_X 11
#define JOY_Y 4
#define SERVO_MAX 180
#define SERVO_MIN 0

using namespace std;

CControl::CControl() {
	_connected = false;
	_lastDebounceTime = std::chrono::steady_clock::now();
}

CControl::~CControl() {}

void CControl::init_com(int comport){
	std::string port = "COM" + std::to_string(comport);

	_connected = _com.open(port, BAUD_RATE);

	if (_connected) {
		std::cout << "Connected to " << port << '\n';

		char dump[256];
		for (int i = 0; i < 50; i++) {
			_com.read(dump, 256);
			Sleep(20);
		}
	}
};

bool CControl::get_data(int type, int channel, int &result) {
	if (!_com.is_open()) return false;

	std::string cmd = "G " + std::to_string(type) + " " +
		std::to_string(channel) + "\n";

	_com.write(cmd.c_str(), (int)cmd.length());

	std::string recieve = "";
	char ch;

	auto start = std::chrono::steady_clock::now();

	while (true)
	{
		if (_com.read(&ch, 1) > 0)
		{
			if (ch == '\n')
				break;      
			recieve += ch;
		}

		if (std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::steady_clock::now() - start).count() > 1000)
		{
			return false;
		}
	}

	if (recieve.size() > 0 && recieve[0] == 'A')
	{
		int rType, rChannel, rValue;

		if (sscanf_s(recieve.c_str(), "A %d %d %d",
			&rType, &rChannel, &rValue) == 3)
		{
			result = rValue;
			return true;
		}
	}

	return false;
};

bool CControl::set_data(int type, int channel, int value) {
	if (!_com.is_open()) return false;

	std::string cmd = "S " + std::to_string(type) + " " +
		std::to_string(channel) + " " +
		std::to_string(value) + "\n";

	_com.write(cmd.c_str(), (int)cmd.length());

	std::string recieve = "";
	char ch;
	auto start = std::chrono::steady_clock::now();

	while (true)
	{
		if (_com.read(&ch, 1) > 0)
		{
			if (ch == '\n') break;
			recieve += ch;
		}

		if (std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::steady_clock::now() - start).count() > 1000)
		{
			return false;
		}
	}
	int rType, rChannel, rValue;
	if (sscanf_s(recieve.c_str(), "A %d %d %d", &rType, &rChannel, &rValue) == 3)
	{
		return true;
	}

	return false;
};

bool CControl::get_analog(int channel, float& percentFullScale) {
	int rawVal;
	if (!get_data(Analog, channel, rawVal)) return false;

	percentFullScale = rawVal / 4095.0f; 
	return true;
}


bool CControl::get_button(int channel, bool& pressed) {
	static bool lastState = false;
	pressed = false;

	int rawVal;
	if (!get_data(Digital, channel, rawVal)) return false;

	bool current = (rawVal == 0);

	auto now = chrono::steady_clock::now();
	long elapsed = chrono::duration_cast<chrono::milliseconds>(now - _lastDebounceTime).count();

	if (current && !lastState && elapsed > 50)
	{
		pressed = true;
		_lastDebounceTime = now;
	}

	lastState = current;
	return true;
}

bool CControl::auto_connect()
{
	_com = Serial();

	for (int port = 1; port <= 20; port++)
	{
		std::string name = "COM" + std::to_string(port);
		if (_com.open(name, BAUD_RATE))
		{
			_connected = true;
			Sleep(150);
			char dump[256];
			for (int index_of_flush = 0; index_of_flush < 40; index_of_flush++)
			{
				_com.read(dump, 256);
				Sleep(20);
			}
			std::cout << "Auto-connected to " << name << '\n';
			return true;
		}
	}
	return false;
}

bool CControl::ensure_connection()
{
	if (_com.is_open())
		return true;

	std::cout << "Connection lost. Scanning ports.\n";
	return auto_connect();
}

bool CControl::analog_test()
{
	std::cout << "\nANALOG TEST (press S to stop)\n";

	while (true)
	{
		if (_kbhit() && toupper(_getch()) == 'S')
			return true;

		int chX = 0, chY = 0;

		bool okX = get_data(Analog, JOY_X, chX);
		bool okY = get_data(Analog, JOY_Y, chY);

		if (!okX || !okY)
		{
			std::cout << "Connection lost, reconnecting\n";
			auto_connect();
			Sleep(500);
			continue;
		}

		float xPct = chX * 100.0f / JOYSTICK_MAX;
		float yPct = chY * 100.0f / JOYSTICK_MAX;

		std::cout << std::fixed << std::setprecision(1);
		std::cout << "ANALOG TEST: X = " << chX << " (" << xPct << " %)  ";
		std::cout << "Y = " << chY << " (" << yPct << " %)\n";

		Sleep(200);
	}
}

bool CControl::digital_test(int buttonPin, int ledPin)
{
	std::cout << "\nDIGITAL TEST (press S to stop)\n";

	while (true)
	{
		if (_kbhit() && toupper(_getch()) == 'S')
			return true;

		int val = 0;

		if (!get_data(Digital, buttonPin, val))
		{
			std::cout << "Connection lost, reconnecting\n";
			auto_connect();
			Sleep(500);
			continue;
		}

		std::cout << "DIGITAL TEST: CH32 = " << !val << '\n';
		set_data(Digital, ledPin, !val);

		Sleep(100);
	}
}

bool CControl::button_test(int buttonPin)
{
	std::cout << "\nBUTTON TEST (press S to stop)\n";

	int count = 0;

	while (true)
	{
		if (_kbhit() && toupper(_getch()) == 'S')
			return true;

		bool pressed;
		get_button(buttonPin, pressed);

		if (pressed)
		{
			count++;
			std::cout << "BUTTON TEST COUNT = " << count << '\n';
		}

		Sleep(100);
	}
}

bool CControl::servo_test(int servoChannel)
{
	std::cout << "\nSERVO TEST (press X to stop)\n";

	int pos = 20;
	int step = 5;

	while (true)
	{
		if (_kbhit() && toupper(_getch()) == 'X')
			return true;

		set_data(Servo, servoChannel, pos);
		std::cout << "SERVO POS = " << pos << '\n';

		pos += step;
		if (pos >= SERVO_MAX || pos <= SERVO_MIN)
			step = -step;

		Sleep(2);
	}
}
