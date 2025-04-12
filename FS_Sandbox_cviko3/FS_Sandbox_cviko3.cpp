// FS_Sandbox_cviko3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <libds/mm/compact_memory_manager.h>
#include <iostream>
#include <string>
#include <vector>

struct Person
{
    int os_cislo;
    std::string meno;

    Person() = default;
    Person(const Person&) = default;
    Person& operator=(const Person&) = default;
};

struct Student : Person {
    double average;
    std::vector<char> grades;


    Student() = default;
    Student(const Student&) = default;
    Student& operator=(const Student&) = default;
};

int main()
{

    /*ds::mm::CompactMemoryManager<int> cmm_short;
    *cmm_short.allocateMemory() = 10;
    *cmm_short.allocateMemory() = 20;
    *cmm_short.allocateMemory() = 30;
    *cmm_short.allocateMemory() = 1000;
    cmm_short.releaseMemory();
    cmm_short.releaseMemory();
    *cmm_short.allocateMemory() = 40;
    cmm_short.print(std::cout);*/
    /*ds::mm::CompactMemoryManager<Person> cmm_obj;
    *cmm_obj.allocateMemory() = { 10, "Filip" };
    cmm_obj.print(std::cout);*/
    ds::mm::CompactMemoryManager<Student> cmm_gen;
    *cmm_gen.allocateMemory() = {10, "Filip", 2.3, {'A','B','C'}};
    cmm_gen.print(std::cout);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
