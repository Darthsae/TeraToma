#pragma once
#ifndef STATUS_H
#define STATUS_H
#include <string>
namespace TeraToma {
    class StatusType {
    public:
        // Information of the status.
        std::string name;
        std::string description;
    };
}
#endif