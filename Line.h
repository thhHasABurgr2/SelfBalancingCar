#ifndef LINE_H
#define LINE_H
#include<iostream>
#include<string>
class Line{
    private:
    std::string content;
    Line* nextLine;
    public:
    Line(): content(""), nextLine (nullptr){}

    void append(const char input){
        content+=input;

    }
    std::string getContent() const{
        return content;
    }
    Line* getNext() const{
        return nextLine;
    }
    void next(Line* next){
        nextLine=next;
    }

};

#endif