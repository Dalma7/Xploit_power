#include <iostream>
#include <unistd.h>
#include "Bony_Stony/bony_stony.h"
#include "dread/dread.h"
#include "stry/stry.h"
#include "hexer/hexer.h"
#include "aegis/aegis.h"
using namespace std;



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
        usleep(4000);
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
