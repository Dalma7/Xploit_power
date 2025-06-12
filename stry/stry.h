#ifndef STRY_H
#define STRY_H

#include <iostream>
#include <vector>
#include <map>

class Stry {
    private:
        std::string ip;
        std::vector<int> ports;
        std::map<int, std::string> services;
        std::vector<std::tuple <int, std::string, std::string>> results;
    public:
        Stry();
        void banner();
        void takeIp();
        void scanPort(int port);
        void scanPorts();
        void run();
};


#endif