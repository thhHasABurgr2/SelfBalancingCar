#ifndef LINE_H
#define LINE_H
#include<iostream>
#include<memory>
#include<string>
class Line{
    private:
    std::string content;
    std::unique_ptr<Line> nextLine;
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
    void next(std::unique_ptr<Line> nextLINE){
        nextLine=std::move(nextLINE);
    }


};

#endif