#include <iostream>
#include <fstream>
#include "lexer.h"

using namespace std;

int main(int argc, char **argv) {
    if (argc < 2){
        cout << "No source file specified." << endl;
        return -1;
    }

    ifstream file(argv[1]);
    if (!file.is_open()){
        cout << "Error opening source file: " << argv[1] << endl;
        return -1;
    }
    for(;;){
        cout << getNextToken(file) << endl;
    }
    return 0;
}

