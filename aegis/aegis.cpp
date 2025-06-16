#include "aegis.h"
#include <iostream>
#include <vector>
#include <unistd.h>
#include <cstdio>
#include <fstream>
using namespace std;


Aegis::Aegis(){
    commands = {"whoami", 
                "sudo ss -tuln",    
                "sudo ufw status", 
                "cat /etc/os-release"
    };
}

void Aegis::CLI(){
    int choice;
    Aegis::banner();

    cout << "[1] General checkout for the security." << endl;
    cout << "[99] Exit." << endl;
    cout << ">>";
    cin >> choice;
    
    switch(choice){
        case 1:
            run();
            break;
        case 99:
            cout << "Exiting.." << endl;
            break;
        default:
            cerr << "[-]Invalid number." << endl;
    }
}

void Aegis::banner(){
    cout << "\033[96m";
    cout << R"( 
######################################
# █████╗ ███████╗ ██████╗ ██╗███████╗#
#██╔══██╗██╔════╝██╔════╝ ██║██╔════╝#
#███████║█████╗  ██║  ███╗██║███████╗#
#██╔══██║██╔══╝  ██║   ██║██║╚════██║#
#██║  ██║███████╗╚██████╔╝██║███████║#
#╚═╝  ╚═╝╚══════╝ ╚═════╝ ╚═╝╚══════╝#
#                                    #
#            Aegis v1.1              #
# By Xpl01t | www.XploitPower.com    #
######################################
)";
    cout << "\033[0m";
}

string Aegis::exec(string& command){
    FILE* fp = popen(command.c_str() , "r"); //open for reading
    //c_str() to convert string to const char*
    if( fp == nullptr){
        cerr << "[-]popen failed." << endl;
        return "Unknown";
    }

    char buffer[1024];
    string result ;

    if(fgets(buffer, sizeof(buffer), fp) != nullptr){
        result += buffer;
    }
    else{
        command = "Unknown";
    }
    pclose(fp);

    if( !result.empty() && result.back() == '\n'){
        result.pop_back();
    }

    return result;
}

void Aegis::runCommand(){
    results.clear();

    for (auto& cmd : commands){
        string output = exec(cmd);
        results.push_back(output);
    }

}
/*void Aegis::saveOutput() {

}*/

void Aegis::showOutput() {
    cout << "\033[1;96m";
    cout << "======================" << endl;
    cout << "   System checkup     " << endl;
    cout << "======================" << "\033[0m" << endl;
    for(int i = 0; i < results.size() ; i++) {
        cout << "[" << commands[i] <<"]" << endl;
        cout << results[i] << endl;
        cout << "------------------------------------" << endl;
    }
    
    //cout << "You are :" << whoami << endl;
}

void Aegis::run() {   
    runCommand();
    showOutput();
}

