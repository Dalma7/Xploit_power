#ifndef AEGIS_H
#define AEGIS_H

#include <iostream>
#include <vector>


class Aegis {
    private:
        std::vector<std::string> blackListsIp;
        std::string ip;
    public:
        Aegis();
        void banner();
        void takeIp();
        bool isBlackListed(const std::string& ip);
        void saveBlockedIp(const std::string& ip);
        void checkConnection(const std::string& ip);
        void run();
};

#endif