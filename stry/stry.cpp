#include "stry.h"
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <thread>
#include <mutex>
#include <chrono>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <algorithm>
#include <unistd.h>
using namespace std;
using Lock = lock_guard<mutex>;
mutex mtx; 

Stry::Stry() {
    ports = {20, 21, 22, 23, 25, 53, 67, 69, 80, 110, 119, 
        123, 135, 137, 138, 139, 143, 161, 194, 443, 445, 465, 993, 
        995, 3306, 3389, 5900, 6379, 8080
    };

    services = {
        {20, "FTP-DATA"}, {21, "FTP"}, {22, "SSH"}, {23, "TELNET"}, {25, "SMTP"},
        {53, "DNS"}, {67, "DHCP"}, {69, "TFTP"}, {80, "HTTP"}, {110, "POP3"},
        {119, "NNTP"}, {123, "NTP"}, {135, "RPC"}, {137, "NETBIOS-NS"},
        {138, "NETBIOS-DGM"}, {139, "NETBIOS-SSN"}, {143, "IMAP"}, {161, "SNMP"},
        {194, "IRC"}, {443, "HTTPS"}, {445, "SMB"}, {465, "SMTPS"}, {993, "IMAPS"},
        {995, "POP3S"}, {3306, "MYSQL"}, {3389, "RDP"}, {5900, "VNC"}, 
        {6379, "REDIS"}, {8080, "HTTP-PROXY"}
    };
};

void Stry::banner(){
    cout << "\033[36m";
    cout << R"(
####################################
#███████╗████████╗██████╗ ██╗   ██╗#
#██╔════╝╚══██╔══╝██╔══██╗╚██╗ ██╔╝#
#███████╗   ██║   ██████╔╝ ╚████╔╝ #
#╚════██║   ██║   ██╔══██╗  ╚██╔╝  #
#███████║   ██║   ██║  ██║   ██║   #
#╚══════╝   ╚═╝   ╚═╝  ╚═╝   ╚═╝   #
#                                  #
#            Stry v1.2             #
# By Xpl01t | www.XploitPower.com  #
####################################           
)";         
    cout << "\033[0m";
}

void Stry::takeIp(){
    cout << "[?]Enter target Ip: ";
    cin >> ip;   
}

void Stry::scanPort(int port){
            
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        cerr << "[-]Sock creating failed." << endl;
    }

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

        if(inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0){
            cerr << "\033[31m[-]Invalid IP address format.\033[0m" << endl;
            return;
        }

        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));

        int result = connect (sock, (sockaddr*)&addr, sizeof(addr));
        

        string status = (result == 0)? "open" : "closed";
        string version = "";

        if(status == "open"){
            char buffer[1024] = {0};
            recv(sock, buffer, sizeof(buffer), 0);
            version = string(buffer);
            
        }

        Lock lock(mtx);
        results.push_back({port, status, version});
        close(sock);
        
}

void Stry::scanPorts(){
    auto start = chrono::steady_clock::now();
    cout << "[*]Scanning " << ip << ".." << endl;

    vector<thread> threads;

    for(int port : ports){
        threads.emplace_back(&Stry::scanPort, this, port);
        this_thread::sleep_for(chrono::milliseconds(10));
    }

    for(auto& t : threads){
        t.join();
    }

    sort(results.begin(), results.end());

    for(const auto& [port, status, version] : results){
        string color = (status == "open") ? "\033[32m" : "\033[31m";
        string service = services.count(port)? services[port] : "UNKNOWN";

        cout << "[SCAN] ▶ " << color << "Port " 
                            << port << "/tcp" 
                            << "(" << service << ")" 
                            << ": " << status <<  endl;
        if(status == "open" && !version.empty()){
            cout << "       [VERSION] ▶ " << version ;
        }
        cout << "\033[0m";
    }


    auto end = chrono::steady_clock::now();
    auto duration_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    cout << "[+]Scan Completed in \033[1m" << duration_ms << " ms!\033[0m" << endl;
}
void Stry::run(){
    banner();
    takeIp();
    scanPorts();   
}
