#ifndef LINE_H
#define LINE_H
#include<iostream>
<<<<<<< HEAD
#include<memory>
=======
>>>>>>> e765849e95c897eb133c26dfce7e26a3bf08c7af
#include<string>
class Line{
    private:
    std::string content;
<<<<<<< HEAD
    std::unique_ptr<Line> nextLine;
=======
    Line* nextLine;
>>>>>>> e765849e95c897eb133c26dfce7e26a3bf08c7af
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
<<<<<<< HEAD
    void next(std::unique_ptr<Line> nextLINE){
        nextLine=std::move(nextLINE);
    }


=======
    void next(Line* next){
        nextLine=next;
    }

>>>>>>> e765849e95c897eb133c26dfce7e26a3bf08c7af
};

#endif