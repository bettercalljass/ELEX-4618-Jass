// JSON library from: https://github.com/nlohmann/json

#include "CCourse.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include "json.hpp"

using json = nlohmann::json;

CCourse::CCourse()
{
}

std::string CCourse::input_student_number(std::string current)
{
    std::string studentNum;

    while (true) {
        std::cout << "Student Number: ";
        std::cin >> studentNum;

        CStudent temp;
        if (!temp.valid_student_number(studentNum)) {
            std::cout << "Invalid student number.\n";
            continue;
        }
        if (studentNum != current && student_number_exists(studentNum)) {
            std::cout << "This student number exists already.\n";
            continue;
        }

        return studentNum;
    }
}

bool CCourse::student_number_exists(const std::string& student_number)
{
    for (int student_number_index = 0; student_number_index < (int)_students.size(); student_number_index++) {
        if (_students[student_number_index].get_student_number() == student_number)
            return true;
    }
    return false;
}

double CCourse::input_grade(std::string prompt)
{
    std::string grade_check;

    while (true) {
        std::cout << prompt << ": ";
        std::cin >> grade_check;

        int grade_index = 0;
        bool seen_dot = false;
        bool seen_digit = false;

        if (grade_check == ".") {
            grade_check = "0";
        }

        if (grade_check.length() > 0 && grade_check[0] == '-') grade_index++;

        if (grade_index >= (int)grade_check.length()) {
            std::cout << "Invalid grade input.\n";
            continue;
        }
        

        for (; grade_index < (int)grade_check.length(); grade_index++) {
            if (grade_check[grade_index] >= '0' && grade_check[grade_index] <= '9') {
                seen_digit = true;
            }
            else if (grade_check[grade_index] == '.') {
                if (seen_dot) { 
                    seen_digit = false;
                    break;
                }
                seen_dot = true;
            }
            else {
                seen_digit = false;
                break;
            }
        }

        if (!seen_digit) {
            std::cout << "Invalid grade input.\n";
            continue;
        }

        double valid_grade = std::atof(grade_check.c_str());

        CStudent temp;
        if (!temp.valid_grade(valid_grade)) {
            std::cout << "Invalid grade.\n";
            continue;
        }

        return valid_grade;
    }
}

void CCourse::AddStudent()
{
    std::cout << "Adding Student\n";
    CStudent student;

    std::string student_number = input_student_number();
    double lab = input_grade("Lab Grade");
    double quiz = input_grade("Quiz Grade");
    double mid = input_grade("Midterm Grade");
    double final = input_grade("Final Exam Grade");

    student.set_student_number(student_number);
    student.set_lab(lab);
    student.set_quiz(quiz);
    student.set_midterm(mid);
    student.set_final_exam(final);

    _students.push_back(student);
}

void CCourse::EditStudent()
{
    if (_students.size() == 0) {
        std::cout << "No students to edit.\n";
        return;
    }

    int student_index;
    std::cout << "Student to edit (1 - " << _students.size() << "): ";
    std::cin >> student_index;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid input.\n";
        return;
    }

    if (student_index < 1 || student_index >(int)_students.size()) {
        std::cout << "That student index doesn't exist.\n";
        return;
    }

    CStudent& student = _students[student_index - 1];

    std::cout << "Editing student #" << student_index << '\n';
    std::string student_number = input_student_number(student.get_student_number());
    double lab = input_grade("Lab Grade");
    double quiz = input_grade("Quiz Grade");
    double mid = input_grade("Midterm Grade");
    double final = input_grade("Final Exam Grade");

    student.set_student_number(student_number);
    student.set_lab(lab);
    student.set_quiz(quiz);
    student.set_midterm(mid);
    student.set_final_exam(final);
}

void CCourse::DeleteStudent()
{
    if (_students.size() == 0) {
        std::cout << "No students to delete.\n";
        return;
    }

    int student_index;
    std::cout << "Student to delete (1 - " << _students.size() << "): ";
    std::cin >> student_index;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid input.\n";
        return;
    }

    if (student_index < 1 || student_index >(int)_students.size()) {
        std::cout << "That student index doesn't exist.\n";
        return;
    }
    _students.erase(_students.begin() + (student_index - 1));
    std::cout << "Student " << student_index << " deleted.\n";
}

void CCourse::PrintGrades()
{
    if (_students.size() == 0) {
        std::cout << "No students to print.\n";
        return;
    }
    std::cout << "#    Student     Lab     Quiz    Midterm   Final   Final Grade\n";

    for (int student_index = 0; student_index < (int)_students.size(); student_index++) {
        CStudent& student = _students[student_index];

        std::cout << (student_index + 1) << "    "
            << std::fixed << std::setprecision(1)
            << student.get_student_number() << "   "
            << student.get_lab() << "     "
            << student.get_quiz() << "     "
            << student.get_midterm() << "       "
            << student.get_final_exam() << "       "
            << student.calc_final_grade() << "\n";
    }
}

void CCourse::SaveClass()
{
    std::string filename;
    std::cout << "Name of save file: ";
    std::cin >> filename;
    size_t dotPos = filename.find_last_of('.');

    std::string ext;
    if (dotPos == std::string::npos) {
        filename += ".txt";
        ext = ".txt";
    }
    else {
        ext = filename.substr(dotPos);
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        if (ext != ".json" && ext != ".txt" && ext != ".doc" && ext != ".docx"
            && ext != ".csv" && ext != ".xml") {
            std::cout << "Invalid file format.\n";
            return;
        }
    }

    json json_file;
    json_file["students"] = json::array();

    for (int student_save_index = 0; student_save_index < (int)_students.size(); student_save_index++) {
        CStudent& student = _students[student_save_index];

        json json_save;
        json_save["student_number"] = student.get_student_number();
        json_save["lab"] = student.get_lab();
        json_save["quiz"] = student.get_quiz();
        json_save["midterm"] = student.get_midterm();
        json_save["final_exam"] = student.get_final_exam();
        json_file["students"].push_back(json_save);
    }

    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cout << "Could not open file for writing.\n";
        return;
    }

    out << std::setw(4) << json_file;
    out.close();
    std::cout << "File saved as: " << filename << "\n";
}

void CCourse::LoadClass()
{
    std::string filename;
    std::cout << "Name of load file: ";
    std::cin >> filename;
    std::ifstream in(filename.c_str());

    if (!in.is_open()) {
        std::cout << "File does not exist for reading.\n";
        return;
    }

    json json_load;
    try {
        in >> json_load;
    }
    catch (...) {
        std::cout << "Invalid file.\n";
        return;
    }

    if (!json_load.contains("students") || !json_load["students"].is_array()) {
        std::cout << "Invalid format.\n";
        return;
    }

    std::vector<CStudent> loaded;

    for (auto& json_save : json_load["students"]) {
        CStudent student;

        if (!json_save.contains("student_number") ||
            !json_save.contains("lab") ||
            !json_save.contains("quiz") ||
            !json_save.contains("midterm") ||
            !json_save.contains("final_exam")) {
            std::cout << "Invalid student entry in file.\n";
            return;
        }

        if (!student.set_student_number(json_save["student_number"]) ||
            !student.set_lab(json_save["lab"]) ||
            !student.set_quiz(json_save["quiz"]) ||
            !student.set_midterm(json_save["midterm"]) ||
            !student.set_final_exam(json_save["final_exam"])) {
            std::cout << "Invalid student data. Load aborted.\n";
            return;
        }
        loaded.push_back(student);
    }

    _students = loaded;
    std::cout << "File loaded!\n";
}