#ifndef BONY_STONY_H
#define BONY_STONY_H

class Bony_Stony {
    public:
        class Bony {
            public:
            void bony_banner();
            void listener();
            void connect_os();
        };
        class Stony {
            public:
            void Stony_banner();
            void listener();

        };
        void run();
};


#endif