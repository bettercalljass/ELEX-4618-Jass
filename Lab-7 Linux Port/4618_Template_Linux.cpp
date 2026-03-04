////////////////////////////////////////////////////////////////
// ELEX 4618 Template project for BCIT
// Created Oct 5, 2016 by Craig Hennessey
// NOTE: You must download the 'opencv.zip' file from D2L
// NOTE: Unzip and place in the parent folder before your lab folders
// i.e. you have \4618\opencv and then \4618\Labs
////////////////////////////////////////////////////////////////

// Add simple GUI elements
#define CVUI_DISABLE_COMPILATION_NOTICES
#define CVUI_IMPLEMENTATION
#include "cvui.h"
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <iomanip>

////////////////////////////////////////////////////
////////////////////////////////////////////////////
//
//         DONT TOUCH ANYTHING ABOVE THIS
//
////////////////////////////////////////////////////
////////////////////////////////////////////////////

// Lab 3
#include "CControlPi.h"
#define JOY_X 5
#define JOY_Y 6
#define DigitalPin 17
#define BlueLED 26
#define myServo 18
#define BAUD_RATE 115200
#define JOYSTICK_MAX 4095.0f
#define SERVO_MAX 180
#define SERVO_MIN 0

//Lab 5
#include "CPong.h"
// #define DigitalPin 17 using this (defined in lab 3)
#define CANVAS_NAME "Display Image"

//Lab 7
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

////////////////////////////////////////////////////////////////
// Generate ARUCO markers
////////////////////////////////////////////////////////////////
void generate_marks()
{
	std::string str;
	cv::Mat im;
	int mark_size = 250;

	cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

	for (int marker_id = 0; marker_id < 250; marker_id++)
	{
		cv::aruco::generateImageMarker(dictionary, marker_id, mark_size, im, 1);
		str = "mark" + std::to_string(marker_id) + ".png";
		cv::imwrite(str, im);
	}
}

////////////////////////////////////////////////////////////////
// Display Image on screen
////////////////////////////////////////////////////////////////
void do_image()
{
	cv::Mat im;
	// initialize GUI system
	cvui::init(CANVAS_NAME);
	cv::Point gui_position;
  
	// Load test image
	im = cv::imread("BCIT.jpg");

	// Seed random number generator with time
	srand(time(0));
  
	// Draw 500 circles and dots on the image
	for (int i = 0; i < 500; i++)
	{
		gui_position = cv::Point(10, 10);
		cvui::window(im, gui_position.x, gui_position.y, 200, 40, "Image Test");
		gui_position += cv::Point(5, 25);
		cvui::text(im, gui_position.x, gui_position.y, "Number of Circles: " + std::to_string(i));

    float radius = 50 * rand() / RAND_MAX;
		cv::Point center = cv::Point(im.size().width * rand() / RAND_MAX, im.size().height * rand() / RAND_MAX);

		cv::circle(im, center, radius, cv::Scalar(200, 200, 200), 1, cv::LINE_AA);
		im.at<char>(i, i) = 255;
    
		// Update the CVUI GUI system
		cvui::update();

		// Show image and delay (all display image to update)
		cv::imshow(CANVAS_NAME, im);
		cv::waitKey(1);
	}
}

////////////////////////////////////////////////////////////////
// Display Video on screen
////////////////////////////////////////////////////////////////
void do_video()
{
	cv::VideoCapture vid;

	vid.open(0, cv::CAP_DSHOW);

	bool do_canny = true;
	bool do_aruco = false;
	int canny_thresh = 30;
	bool do_exit = false;

	cv::aruco::DetectorParameters detectorParams = cv::aruco::DetectorParameters();
	cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
	cv::aruco::ArucoDetector detector(dictionary, detectorParams);

	// initialize GUI system
	cvui::init(CANVAS_NAME);
	cv::Point gui_position;

	std::vector<cv::Scalar> color_vec;
	color_vec.push_back(cv::Scalar(255, 255, 255));
	color_vec.push_back(cv::Scalar(255, 0, 0));
	color_vec.push_back(cv::Scalar(0, 255, 0));
	color_vec.push_back(cv::Scalar(0, 0, 255));
	int color_index = 0;

	if (vid.isOpened())
	{
		do
		{
			cv::Mat frame, edges;

			// Capture video frame
			vid >> frame;
      
			// Make sure video frame exists
			if (frame.empty() == false)
			{
				// ARUCO marker tracking
				if (do_aruco == true)
				{
					std::vector<int> ids;
					std::vector<std::vector<cv::Point2f>> corners;
					detector.detectMarkers(frame, corners, ids);

					if (ids.size() > 0)
					{
						cv::aruco::drawDetectedMarkers(frame, corners, ids);
					}
				}

				// Canny edge detection
				if (do_canny == true)
				{
					cv::cvtColor(frame, edges, cv::COLOR_BGR2GRAY);
					cv::GaussianBlur(edges, edges, cv::Size(7, 7), 1.5, 1.5);
					cv::Canny(edges, edges, 0, canny_thresh, 3);
					cv::add(frame, color_vec.at(color_index), frame, edges);
				}

				// GUI Menu
				gui_position = cv::Point(10, 10);
				cvui::window(frame, gui_position.x, gui_position.y, 200, 190, "Video Test");
				gui_position += cv::Point(5, 25);
				cvui::checkbox(frame, gui_position.x, gui_position.y, "Canny Filter", &do_canny);
				gui_position += cv::Point(0, 25);
				cvui::checkbox(frame, gui_position.x, gui_position.y, "ArUco", &do_aruco);
				gui_position += cv::Point(0, 25);
				cvui::text(frame, gui_position.x, gui_position.y, "Canny Threshold");
				gui_position += cv::Point(0, 15);
				cvui::trackbar(frame, gui_position.x, gui_position.y, 180, &canny_thresh, 5, 120);
				gui_position += cv::Point(0, 50);
				if (cvui::button(frame, gui_position.x, gui_position.y, 100, 30, "Colour Switch"))
				{
					color_index++;
					if (color_index >= color_vec.size()) { color_index = 0; }
				}
				gui_position += cv::Point(120, 0);
				if (cvui::button(frame, gui_position.x, gui_position.y, 50, 30, "Exit"))
				{
					do_exit = true;
				}

				// Update the CVUI GUI system
				cvui::update();
				cv::imshow(CANVAS_NAME, frame);
			}
		} while (cv::waitKey(1) != 'q' && do_exit == false);
	}
}

////////////////////////////////////////////////////////////////
// Lab 1
////////////////////////////////////////////////////////////////
void lab1(){}

///////////////////////////////////////////////////////////////
// Lab 2
////////////////////////////////////////////////////////////////
void lab2(){}

////////////////////////////////////////////////////////////////
// Lab 3
////////////////////////////////////////////////////////////////
char get_key()
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();
  
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    return ch;
}

void analog_test(CControl& board)
{
    std::cout << "\nANALOG TEST (press X to stop)\n";
    while (true)
    {
        char key = get_key();
        if (key == 'x' || key == 'X')
            return;

        int chX = 0;
        int chY = 0;
        
        bool okX = board.get_data(Analog, 0, chX);
        bool okY = board.get_data(Analog, 1, chY);

        if (!okX || !okY)
        {
            std::cout << "SPI read failed\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            continue;
        }

        float xPct = (chX * 100.0f) / 1023.0f;
        float yPct = (chY * 100.0f) / 1023.0f;

        std::cout << "X = " << chX << " (" << xPct << "%)   ";
        std::cout << "Y = " << chY << " (" << yPct << "%)\n";
      
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void digital_test(CControl& board, int buttonPin, int ledPin)
{
    std::cout << "\nDIGITAL TEST (press X to stop)\n";

    while (true)
    {
        char key = get_key();
        if (key == 'x' || key == 'X')
            return;

        int val = 0;

        if (!board.get_data(Digital, buttonPin, val))
        {
            std::cout << "Digital read failed\n";
            continue;
        }

        int pressed = !val;
        std::cout << "BUTTON = " << pressed << '\n';

        board.set_data(Digital, ledPin, !pressed);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void button_test(CControl& board, int buttonPin)
{
	std::cout << "\nBUTTON TEST (press X to stop)\n";
  
	int count = 0;
	bool prevPressed = false;

	while (true)
	{
		char key = get_key();
    if (key == 'x' || key == 'X')
			return;

		bool pressed = false;
		board.get_button(buttonPin, pressed);

		if (pressed && !prevPressed)
		{
			count++;
			std::cout << "COUNT = " << count << '\n';
		}

		prevPressed = pressed;
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}

void servo_test(CControl& board, int servoChannel)
{
    std::cout << "\nSERVO TEST (press X to stop)\n";
  
    int angle = 0;
    int step = 3;

    while (true)
    {
       char key = get_key();
        if (key == 'x' || key == 'X')
        {
            board.set_data(Servo, servoChannel, 90); 
            return;
        }

        board.set_data(Servo, servoChannel, angle);
      
        angle += step;
        std::cout << "Servo Position: " << angle << std::endl;
        if (angle >= 180)
        {
            angle = 180;
            step = -step;
        }
        else if (angle <= 0)
        {
            angle = 0;
            step = -step;
        }
      
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}

void lab3()
{
	CControl board;
	char choice;

  while (true)
  {
    std::cout << "\n***********************************************";
    std::cout << "\n   ELEX4618 - Embedded Control by Jass Saini";
    std::cout << "\n***********************************************";
    std::cout << "\n(A) Analog Test";
    std::cout << "\n(D) Digital Test";
    std::cout << "\n(B) Debounce Test";
    std::cout << "\n(S) Servo Test";
    std::cout << "\n(Q) Quit Lab-03";
    std::cout << "\n******************************";
    std::cout << "\nCMD: ";

    std::cin >> choice;
    choice = toupper(choice);

    if (choice == 'Q') break;
    if (choice == 'A') analog_test(board);
    else if (choice == 'D') digital_test(board, DigitalPin, BlueLED);
    else if (choice == 'B') button_test(board, DigitalPin);
    else if (choice == 'S') servo_test(board, myServo);
}
  
cv::destroyWindow("Menu");
}

////////////////////////////////////////////////////////////////
// Lab 4
////////////////////////////////////////////////////////////////
void lab4(){}

////////////////////////////////////////////////////////////////
// Lab 5
////////////////////////////////////////////////////////////////
void lab5()
{
	CPong pong;
	pong.run();
}

////////////////////////////////////////////////////////////////
// Lab 6
////////////////////////////////////////////////////////////////
void lab6(){}

void print_menu()
{
	std::cout << "\n***********************************";
	std::cout << "\n* ELEX4618 Template Project";
	std::cout << "\n***********************************";
	std::cout << "\n(1) Lab 1 - User Input";
	std::cout << "\n(2) Lab 2 - Grading";
	std::cout << "\n(3) Lab 3 - Embedded Control";
	std::cout << "\n(4) Lab 4 - Classic Arcade Game 1";
	std::cout << "\n(5) Lab 5 - Classic Arcade Game 2";
	std::cout << "\n(6) Lab 6 - Classic Arcade Game 3";
	std::cout << "\n(7) Lab 7 - Linux Port";
	std::cout << "\n(8) Lab 8 - Sorting";
	std::cout << "\n(9) Lab 9 - Sockets";
	std::cout << "\n(10) Test serial COM communication";
	std::cout << "\n(11) Show image manipulation";
	std::cout << "\n(12) Show video manipulation";
	std::cout << "\n(13) Test client/server communication";
	std::cout << "\n(0) Exit";
	std::cout << "\nCMD> ";
}

int main(int argc, char* argv[])
{
	// generate_marks();
	int cmd = -1;
	do
	{
		print_menu();
		std::cin >> cmd;
		switch (cmd)
		{
		case 1: lab1(); break;
		case 2: lab2(); break;
		case 3: lab3(); break;
		case 4: lab4(); break;
		case 5: lab5(); break;
		case 6: lab6(); break;
		case 11: do_image(); break;
		case 12: do_video(); break;
		}
	} while (cmd != 0);
}
