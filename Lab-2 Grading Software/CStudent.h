#pragma once
#include <string>

class CStudent
{
private:
    std::string _studentNumber;
    double _lab;
    double _quiz;
    double _midterm;
    double _finalExam;

public:
    CStudent();
    std::string get_student_number();
    double get_lab();
    double get_quiz();
    double get_midterm();
    double get_final_exam();

    bool set_student_number(std::string student_number);
    bool set_lab(double grade);
    bool set_quiz(double grade);
    bool set_midterm(double grade);
    bool set_final_exam(double grade);

    double calc_final_grade();

    bool valid_student_number(std::string student_number);
    bool valid_grade(double grade);
};