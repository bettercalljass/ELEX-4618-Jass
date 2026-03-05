#pragma once
#include <vector>
#include <string>
#include "CStudent.h"

class CCourse
{
private:
    std::vector<CStudent> _students;
    std::string input_student_number(std::string current = "");
    double input_grade(std::string prompt);
    bool student_number_exists(const std::string& sn);

public:
    CCourse();
    void AddStudent();
    void EditStudent();
    void DeleteStudent();
    void PrintGrades();
    void SaveClass();
    void LoadClass();
};