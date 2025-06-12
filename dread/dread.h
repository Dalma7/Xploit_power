#ifndef DREAD_H
#define DREAD_H

#include <iostream>
#include <atomic>
#include <vector>

class Dread {
    private:
        std::vector<std::string> usernames;
        std::vector<std::string> passwords;
        std::string targetURL;
        std::atomic<bool> found ;
    public:
        void banner();
        void loadWordlists();
        bool trylogin(const std::string& user, const std::string& pass) const;
        void bruteforce();
        void run();
};
#endif