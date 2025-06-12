#ifndef HEXER_H
#define HEXER_H

#include <iostream>
#include <vector>

class Hexer{
    public:
        void banner();
        void CLI();
    class Wordlists_generator{
        private:
            std::vector<std::string> words;
            std::vector<std::string> combos;
        public:
            void takeWords();
            void combinations(const std::vector<std::string>& words);
            void saveWordlist();
            void Wordlists_runner();
    };
    class Passwords_generator{
        private:
            int length;
            std::string password ;
            std::string use;
        public:
            void makePasswords();
            void savePassword();
            void Pass_run();
    };
    void mainRun();
};
#endif