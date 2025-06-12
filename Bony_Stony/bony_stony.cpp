#include "bony_stony.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>

using namespace std;

void Bony_Stony::Bony::bony_banner(){
    cout << "\033[38;5;46m";
    cout << R"(
#######################################                 
#██████╗  ██████╗ ███╗   ██╗██╗   ██╗ # 
#██╔══██╗██╔═══██╗████╗  ██║╚██╗ ██╔╝ # 
#██████╔╝██║   ██║██╔██╗ ██║ ╚████╔╝  # 
#██╔══██╗██║   ██║██║╚██╗██║  ╚██╔╝   # 
#██████╔╝╚██████╔╝██║ ╚████║   ██║    # 
#╚═════╝  ╚═════╝ ╚═╝  ╚═══╝   ╚═╝    #                     
#                                     #
#            Bony v1.0                #
#   By Xpl01t | www.XploitPower.com   #
#######################################                     
)" << "\033[0m";
}

void Bony_Stony::Bony::listener(){
    cout << "[*Bony]Start listening..." << endl;
}

void connect_os(){
    int sock = socket( AF_INET, SOCK_STREAM, 0);
    if (sock == -1){
        cerr << "[-]Failed to create a socket." << endl;
        return;
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(4444);
    inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1){
        cerr << "\033[31m[-] Connection failed.\033[0m" << endl;
        close(sock);
        return;
    }

    cout << "\033[33m[+]Connected to Stony.\033[0m" << endl;

    const char* message = "hello from bony!, the password is 13534";
    send(sock, message, strlen(message), 0);

    close(sock);   
}

void Bony_Stony::Stony::Stony_banner(){
    cout << "\033[38;5;46m";
    cout << R"(
###############################################                    
#███████╗████████╗ ██████╗ ███╗   ██╗██╗   ██╗#
#██╔════╝╚══██╔══╝██╔═══██╗████╗  ██║╚██╗ ██╔╝#
#███████╗   ██║   ██║   ██║██╔██╗ ██║ ╚████╔╝ #
#╚════██║   ██║   ██║   ██║██║╚██╗██║  ╚██╔╝  #
#███████║   ██║   ╚██████╔╝██║ ╚████║   ██║   #
#╚══════╝   ╚═╝    ╚═════╝ ╚═╝  ╚═══╝   ╚═╝   #
#                                             #
#                 Stony v1.0                  #
#       By Xpl01t | www.XploitPower.com       #
###############################################                      
)";
    cout << "\033[0m";
}

void Bony_Stony::Stony::listener(){
    int server_fd, new_sock;
    struct sockaddr_in adress;
    int opt = 1;
    int addrlen = sizeof(adress);
    char buffer[1024] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == 0){
        cerr << "\033[31m[-]Socket creation failed.\033[0m" << endl;
        return;
    }
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt));
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT , &opt, sizeof(opt));

    adress.sin_family = AF_INET;
    adress.sin_port = htons(4444);
    inet_pton(AF_INET, "127.0.0.1", &adress.sin_addr);

    if(bind(server_fd, (struct sockaddr* )&adress, sizeof(adress)) < 0){
        cerr << "\033[31m[-]bind failed.\033[0m" << endl;
        return;
    }

    if(listen(server_fd, 3) < 0){
        cerr << "\033[31m[-]Listen failed.\033[0m" << endl;
        return;
    }

    cout << "[Stony] Listening on port 4444.." << endl;

    new_sock = accept(server_fd, (struct sockaddr *)&adress, (socklen_t*)&addrlen);
    if(new_sock < 0){
        cerr << "\033[31m[-]Accepting failed" << endl;
    }

    cout << "[Stony] Connection received from Bony!" << endl;

    int valread = read(new_sock, buffer, 1024);
    if(valread > 0){
        cout << "[Bony] Data received :\n" << buffer << endl;
    }
    else{
        cout << "[Bony] No data received." << endl;
    }
    

    close(new_sock);
    close(server_fd);
}

void Bony_Stony::run(){
    Bony b;
    Stony s;
    
    thread strony_thread([&](){
        s.Stony_banner();
        sleep(1);
        s.listener();
    });
    sleep(2);

    cout << "[+] Press [Enter] key to continue to Bony...";
    cin.ignore();
    cin.get();

    b.bony_banner();
    sleep(1);
    b.listener();
    b.connect_os();
    sleep(1);
    
    strony_thread.join();
}