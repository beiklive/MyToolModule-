// Copyright (c) RealCoolEngineer. 2024. All rights reserved.
// Author: beiklive
// Date: 2024-03-20
#ifndef INC_COMMON_H_
#define INC_COMMON_H_
#include <iostream>

#define EXAMPLE_MAIN_START(name) \
    int main(int argc, char* argv[]){ \
    std::cout << "==== Start Example " << #name << " ====" << std::endl;



#define EXAMPLE_MAIN_FINISH(name) \
    std::cout << "==== Finish Example " << #name << " ====" << std::endl; \
    return 0;}



#endif  // INC_COMMON_H_
