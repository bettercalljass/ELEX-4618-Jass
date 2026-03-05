#include <iostream>
#include "CCourse.h"

void print_menu();

int main()
{
    CCourse course;
    char cmd;

    do {
        print_menu();

        if (!(std::cin >> cmd)) {
            std::cin.clear();
            while (std::cin.get() != '\n');
            std::cout << "Invalid action\n";
            continue;
        }

        if (std::cin.peek() != '\n') {
            while (std::cin.get() != '\n');
            std::cout << "Invalid action\n";
            continue;
        }

        switch (cmd) {
        case 'A': case 'a': course.AddStudent();    break;
        case 'E': case 'e': course.EditStudent();   break;
        case 'D': case 'd': course.DeleteStudent(); break;
        case 'P': case 'p': course.PrintGrades();   break;
        case 'S': case 's': course.SaveClass();     break;
        case 'L': case 'l': course.LoadClass();     break;
        case 'Q': case 'q':                         break;
        default: std::cout << "Invalid action\n";
            break;
        }
    } while (!(cmd == 'Q' || cmd == 'q'));

    return 0;
}

void print_menu()
{
    std::cout << "****************************************\n";
    std::cout << "  ELEX4618 Grade System, by Jass Saini\n";
    std::cout << "****************************************\n";
    std::cout << "(A)dd student\n";
    std::cout << "(E)dit student\n";
    std::cout << "(D)elete student\n";
    std::cout << "(P)rint grades\n";
    std::cout << "(S)ave class\n";
    std::cout << "(L)oad class\n";
    std::cout << "(Q)uit\n";
    std::cout << "CMD> ";
}
