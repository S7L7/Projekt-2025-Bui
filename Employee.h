//
// Created by buijo on 02.01.2026.
//

#ifndef DOCHAZKOVY_SYSTEM_EMPLOYEE_H
#define DOCHAZKOVY_SYSTEM_EMPLOYEE_H

#include <string>

struct Employee {
    int id;
    std::string name;
    std::string rfid;
    int status; // 0 = out , 1 = in bude mi určovat stav zaměstnance
};
#endif //DOCHAZKOVY_SYSTEM_EMPLOYEE_H