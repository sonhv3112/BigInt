#include "Command.h" 

int main(int argv, char **argc) { 
    // Open file
    ifstream inp(argc[1]); 
    if (inp.fail()) { 
        cout << "Khong the mo file!\n"; 
        return 0; 
    }

    ofstream out(argc[2]); 
    if (out.fail()) { 
        cout << "Khong the mo file!\n"; 
        return 0; 
    }

    RunCommand(inp, out);

    inp.close(); 
    out.close(); 

    return 0; 
}