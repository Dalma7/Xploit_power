#include <iostream>
#include "hexer.h"
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <limits>


using namespace std;

void Hexer::banner(){
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
void Hexer::CLI(){
    Wordlists_generator gen;
    Passwords_generator pas;
    Numbers_generator num;
    int choice;
    sleep(1);

    cout << "[1]. Wordlists generator." << endl;
    cout << "[2]. Passwords generator." << endl;
    cout << "[3]. Numbers generator." << endl;
    cout << "[99]. Exit." << endl;
    cout << "[?]. Enter your choice >> ";
    cin >> choice;

    switch (choice){
        case 1 :
            gen.Wordlists_runner();
            break;
        case 2 : 
            pas.Pass_run();
            break;
        case 3:
            num.Nums_run();
            break;
        case 99:
            cout << "Exiting..";
            break;
        default:
            cerr << "\033[31mInvalid number\033[0m" << endl;
            break; 
    }
            
}  
void Hexer::Wordlists_generator::takeWords(){
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
void Hexer::Wordlists_generator::combinations(const vector<string>& words){
    combos.clear();
    for(size_t i=0; i < words.size(); i++){
        for(size_t j=0; j < words.size(); j++){
            if (i != j ){
                combos.push_back(words[i] + words[j]);
            }
        }
    }
}
void Hexer::Wordlists_generator::saveWordlist(){
    string fileName;

    cout << "[?]Enter the name of the file(no extension): ";
    getline(cin, fileName);

    string fullPath = "/home/malloc/Documents/infos/wordlists" + fileName + ".txt";

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
    cout << "\033[35m[+]Wordlist has been saved successfully in secrete file\033[0m" << endl;
}
void Hexer::Wordlists_generator::Wordlists_runner(){
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    takeWords();
    combinations(words);
    saveWordlist();
}

void Hexer::Passwords_generator::makePasswords(){
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
    
}
void Hexer::Passwords_generator::savePassword(){
    ofstream file("/home/malloc/Documents/infos/passwords.txt", ios::app);
    file << use << ": " << password << endl;
    file.close();
    cout << "\033[92m[+] Your password has been saved successfully in passwords folder.\033[0m" << endl;
}
void Hexer::Passwords_generator::Pass_run(){
    makePasswords();
    savePassword();
}

bool Hexer::Numbers_generator::takeInput(){
    cout << "[?] Enter the length of the numbers (1-10): ";
    cin >> length;

    if(length < 0 || length > 10 || cin.fail()){
        cerr << "[-]Invalid number." << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        return false;
    }
    return true;
}
void Hexer::Numbers_generator::create_numbers(){
    numbers.clear();
    int max = 1;
    for(int i = 0; i < length ; i++) max *= 10;

    for(int i = 0; i < max; i++){
        string num = to_string(i);
        int padding = length - num.length();
        if (padding < 0) padding = 0;
        num = string(padding , '0') + num;
        numbers.push_back(num);
    }
    cout << "[+] Numbers generated successefully!";
}
void Hexer::Numbers_generator::Save_Numbers(){
    string filename;
    cout << "[?] Enter the name of the file (no extention): ";
    cin >> filename;

    string path = "/home/malloc/Documents/infos/wordlists/" + filename + ".txt";

    ofstream file(path);
    if(!file.is_open()){
        cerr << "[-] Cant open file.";
        return;
    }
    for( auto& num : numbers){
        file << num << endl;
    }
    file.close();
    cout << "[+] The wordlist has been saved successfully!";
}
void Hexer::Numbers_generator::Nums_run(){
    takeInput();
    create_numbers();
    Save_Numbers();
}

void Hexer::mainRun(){
    banner();
    CLI();
}