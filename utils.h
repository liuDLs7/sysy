#pragma once
#include <string>
#include <unordered_map>
#include <set>
#include <vector>
#include <stack>
#include "genKoopa.h"
#include "genRiscv.h"


// 判断块是否结束
class BlockController{
private:
    bool f = true;
public:
    bool alive(){
        return f;
    }

    void finish(){
        f = false;
    }

    void set(){
        f = true;
    }
};

class WhileName{
public:
    std::string entry_name, body_name, end_name;
    WhileName(const std::string &_entry, const std::string & _body, const std::string &_end): entry_name(_entry), body_name(_body), end_name(_end){}
};

class WhileStack{
private:
    std::stack<WhileName> whiles;
public:
    void append(const std::string &_entry, const std::string & _body, const std::string &_end){
        whiles.emplace(_entry, _body, _end);
    }
    
    void quit(){
        whiles.pop();
    }

    std::string getEntryName(){
        return whiles.top().entry_name;
    }

    std::string getBodyName(){
        return whiles.top().body_name;
    }

    std::string getEndName(){
        return whiles.top().end_name;
    }
};

// 后端riscv生成时，使用到的临时标号
class TempLabelManager{
private:
    int cnt;
public:
    TempLabelManager():cnt(0){ }
    std::string getTmpLabel(){
        return "Label" + std::to_string(cnt++);
    }
};