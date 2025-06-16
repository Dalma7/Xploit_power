#ifndef AEGIS_H
#define AEGIS_H

#include <iostream>
#include <vector>
using namespace std;

//step 1 : make the CLI
//step 2 : run the command as the choice
// sudo ss -tuln
// sudo ufw status
// cat /etc/os-release
//step 3 : save and show the output
//step 4 : end

class Aegis {
    private: 
        
        vector <string> commands;
        vector <string> results;
    public:
        Aegis();
        void banner();
        void CLI();
        string exec(string& command);
        void runCommand();
        void saveOutput();
        void showOutput();
        void run();
};

#endif