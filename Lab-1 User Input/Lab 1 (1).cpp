#include <iostream>
#include <string> 

struct Grade {
	char studentNumber[9];
	double lab, quiz, midterm, finalExam;
};

Grade students[100];
int number_of_students = 0;

void print_menu();
void add_student(Grade& student, int& number_of_students);
void edit_student(Grade& student);
void print_grades();

int main() {
	char userInput;

	do {
		print_menu();
		std::cin >> userInput;

		if (userInput == 'A') {
			add_student(students[number_of_students], number_of_students);
		}
		else if (userInput == 'E') {
			int student_index;
			std::cout << "Student to edit: ";
			std::cin >> student_index;
			edit_student(students[student_index - 1]);
		}
		else if (userInput == 'P') {
			print_grades();
		}
	} while (userInput != 'Q');

	return 0;
}

void print_menu() {
	std::cout << "****************************************\n";
	std::cout << "  ELEX4618 Grade System, by Jass Saini\n";
	std::cout << "****************************************\n";
	std::cout << "(A)dd student\n";
	std::cout << "(E)dit student\n";
	std::cout << "(P)rint grades\n";
	std::cout << "(Q)uit\n";
	std::cout << "CMD> ";
}

void add_student(Grade& student, int& number_of_students) {

	std::cout << "Adding Student\n";
	std::cout << "Student Number: "; std::cin >> student.studentNumber;
	std::cout << "Lab Grade: "; std::cin >> student.lab;
	std::cout << "Quiz Grade: "; std::cin >> student.quiz;
	std::cout << "Midterm Grade: "; std::cin >> student.midterm;
	std::cout << "Final Exam Grade: "; std::cin >> student.finalExam;

	number_of_students++;
}

void edit_student(Grade& student) {

	std::cout << "Student Number: "; std::cin >> student.studentNumber;
	std::cout << "Lab Grade: "; std::cin >> student.lab;
	std::cout << "Quiz Grade: "; std::cin >> student.quiz;
	std::cout << "Midterm Grade: "; std::cin >> student.midterm;
	std::cout << "Final Exam Grade: ";	std::cin >> student.finalExam;
}

void print_grades() {
	std::cout << "#    Student    Lab  Quiz  Midterm  Final Exam  Final Grade\n";

	for (int student_print_index = 0; student_print_index < number_of_students; student_print_index++) {

		double finalGrade = 0.40 * students[student_print_index].lab + 
							0.10 * students[student_print_index].quiz +
							0.20 * students[student_print_index].midterm + 
							0.30 * students[student_print_index].finalExam;

		std::cout << (student_print_index + 1) << "   "
			<< students[student_print_index].studentNumber << "   "
			<< students[student_print_index].lab << "    "
			<< students[student_print_index].quiz << "      "
			<< students[student_print_index].midterm << "        "
			<< students[student_print_index].finalExam << "          "
			<< finalGrade << "\n";
	}
}