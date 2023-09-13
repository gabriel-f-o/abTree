#include "CppUTest/CommandLineTestRunner.h"
#include <iostream>

int main(int ac, char** av)
{
    std::cout<<"Running Test Suite\n";
    return CommandLineTestRunner::RunAllTests(ac, av); /* cover alternate method */;
}