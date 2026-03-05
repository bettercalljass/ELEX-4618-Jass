#include "CStudent.h"
#include <cctype>

CStudent::CStudent()
{
    _studentNumber = "";
    _lab = 0;
    _quiz = 0;
    _midterm = 0;
    _finalExam = 0;
}

std::string CStudent::get_student_number() { return _studentNumber; }
double CStudent::get_lab() { return _lab; }
double CStudent::get_quiz() { return _quiz; }
double CStudent::get_midterm() { return _midterm; }
double CStudent::get_final_exam() { return _finalExam; }

bool CStudent::valid_grade(double grade)
{
    return (grade >= 0.0 && grade <= 100.0);
}

bool CStudent::valid_student_number(std::string student_number)
{
    if (student_number.length() != 9)
        return false;

    bool valid_prefix =
        ((student_number[0] == 'A' || student_number[0] == 'a') && student_number[1] == '0' && student_number[2] == '0') ||
        ((student_number[0] == 'A' || student_number[0] == 'a') && student_number[1] == '0' && student_number[2] == '1');

    if (!valid_prefix)
        return false;

    for (int student_number_index = 3; student_number_index < 9; student_number_index++) {
        if (student_number[student_number_index] < '0' || student_number[student_number_index] > '9') return false;
    }

    return true;
}

bool CStudent::set_student_number(std::string student_number)
{
    if (!valid_student_number(student_number)) return false;
    _studentNumber = student_number;
    return true;
}

bool CStudent::set_lab(double grade)
{
    if (!valid_grade(grade)) return false;
    _lab = grade;
    return true;
}

bool CStudent::set_quiz(double grade)
{
    if (!valid_grade(grade)) return false;
    _quiz = grade;
    return true;
}

bool CStudent::set_midterm(double grade)
{
    if (!valid_grade(grade)) return false;
    _midterm = grade;
    return true;
}

bool CStudent::set_final_exam(double grade)
{
    if (!valid_grade(grade)) return false;
    _finalExam = grade;
    return true;
}

double CStudent::calc_final_grade()
{
    return 0.40 * _lab + 0.10 * _quiz + 0.20 * _midterm + 0.30 * _finalExam;
}