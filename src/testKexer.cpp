#include "Token.h"
#include "Line.h"
#include "imp.h"
#include<iostream>
#include<memory>
#include<string>
#include<fstream>
#include<vector>
using namespace imp;

int main(int argc, char* argv[]){
    std::ifstream inpFile;
    inpFile.open(argv[1]);

    std::unique_ptr<Line> lineHead= std::make_unique<Line>();
    Line* popLine=lineHead.get();

    // time to get the input into a linked list of lines
    if(inpFile.is_open()){
        char c;
        while(inpFile.get(c)){
            if(c== ' '){
                continue;
            }else if(c=='\n'){
                popLine->next(std::make_unique<Line>());
                popLine=popLine->getNext();

            }else{            
                popLine->append(c);
            }
        }
    }
    popLine=lineHead.get();

    std::vector<std::unique_ptr<Token>> headList;
    
    while(popLine!= nullptr){// im assuming shared mem by copy here
        std::unique_ptr<Token> head=TokensPerLine(popLine);
        if(head){
            head.get()->printTokens();
            headList.push_back(std::move(head));
        }

        popLine=popLine->getNext();
    }

    return 0;
}