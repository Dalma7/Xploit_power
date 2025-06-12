#include "aegis.h"
#include <iostream>
#include <vector>
#include <unistd.h>
#include <fstream>
using namespace std;

Aegis::Aegis(){
        blackListsIp = {
            "192.168.1.100",
            "10.0.0.5",
            "123.456.78.9"
        };
        
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
#            Aegis v1.0              #
# By Xpl01t | www.XploitPower.com    #
######################################
)";
            cout << "\033[0m";
        }

        void Aegis::takeIp(){
            cout << "[?]Enter the Ip address: ";
            cin >> ip;
            sleep(1);
        }

        bool Aegis::isBlackListed(const string& ip){
            for(const auto& blockedIp : blackListsIp){
                if (blockedIp == ip){
                    return true;
                }
            }
            return false;
        }
        void Aegis::saveBlockedIp(const string& ip){
            ofstream log("/home/malloc/Documents/infos/aegis_logs.txt", ios::app);
            if (!log) {
                std::cerr << "[ERROR] Could not open the secret file!" << std::endl;
            }
            log << "[AEGIS] ▶ [BLOCKED] " << ip << endl;
            log.flush();
            cout << "[*]The ip has been saved in a secrete file" << endl;
            
        }

        void Aegis::checkConnection(const string& ip){
            if(isBlackListed(ip)){
                cout << "\033[31m[-]Connection from " << ip << " is blocked!\033[0m" << endl;
                saveBlockedIp(ip);
            }
            else{
                cout << "\033[32m[+]Connection from " << ip << " is allowed!\033[0m" << endl;
            }
        }

        void Aegis::run(){
            banner();
            takeIp();
            checkConnection(ip);
        }