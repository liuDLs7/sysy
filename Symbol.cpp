#include "Symbol.h"
#include <iostream>
using namespace std;

void NameManager::reset(){
    cnt = 0;
}

//返回当前块号，仅作测试用
std::string NameManager::getTmpName(){
    return "%" + std::to_string(cnt++);
}

std::string NameManager::getName(const std::string &s){
    auto i = no.find(s);
    // 第一次出现变量时赋名，存入map
    if(i == no.end()){
        no.insert(make_pair(s, 1));
        return "@" + s + "_0";
    }
    return "@" + s + "_"  + std::to_string(i->second++);
}
// 与上面的功能相同，为块命名
std::string NameManager::getLabelName(const std::string &s){
    auto i = no.find(s);
    if(i == no.end()){
        no.insert(make_pair(s, 1));
        return "%" + s + "_0";
    }
    return "%" + s + "_"  + std::to_string(i->second++);
}

SysYType::SysYType():ty(SYSY_INT), value(-1), next(nullptr){}

SysYType::SysYType(TYPE _t):ty(_t), value(-1), next(nullptr){}

SysYType::SysYType(TYPE _t, int _v): ty(_t), value(_v), next(nullptr){}

SysYType::SysYType(TYPE _t, const std::vector<int> &len):ty(_t), value(-1), next(nullptr){
    buildFromArrayType(len, _t == SYSY_ARRAY_CONST);
}

SysYType::~SysYType() { 
    auto p = next;
    if(p != nullptr){
        auto prev = p;
        p = p->next;
        delete prev;
    }
}

void SysYType::buildFromArrayType(const std::vector<int> &len, bool is_const){
    TYPE t = is_const ? SYSY_ARRAY_CONST : SYSY_ARRAY;
    SysYType *p = this;
    // 遍历容器给数组各个元素赋值
    for(int i : len){
        p->ty = t;
        p->value = i;
        p->next = new SysYType();
        p = p->next;
    }
    p->next = new SysYType();
    p = p->next;
    p->ty = is_const ? SYSY_INT_CONST : SYSY_INT;
}

void SysYType::getArrayType(std::vector<int> &len){
    len.clear();
    SysYType *p = this;
    while(p->next != nullptr && (p->ty == SYSY_ARRAY_CONST || p->ty == SYSY_ARRAY)){
        len.push_back(value);
        p = p->next;
    }
    return;
}

Symbol::Symbol(const std::string &_ident, const std::string &_name, SysYType *_t): ident(_ident), name(_name), ty(_t){
}

Symbol::~Symbol(){
    if(ty) delete ty;
}

SymbolTable::~SymbolTable(){
    for(auto &p : symbol_tb){
        delete p.second;
    }
}

void SymbolTable::insert(Symbol *symbol){
    symbol_tb.insert({symbol->ident, symbol});
}

void SymbolTable::insert(const std::string &ident, const std::string &name, SysYType::TYPE _type, int value){
    SysYType *ty = new SysYType(_type, value);
    Symbol *sym = new Symbol(ident, name, ty);
    insert(sym);
}

// 对int型变量分配地址，不保存值
void SymbolTable::insertINT(const std::string &ident, const std::string &name){
    insert(ident, name, SysYType::SYSY_INT, UNKNOWN);
}

// 对常类保存名与值
void SymbolTable::insertINTCONST(const std::string &ident, const std::string &name, int value){
    insert(ident, name, SysYType::SYSY_INT_CONST, value);
}

void SymbolTable::insertFUNC(const std::string &ident, const std::string &name, SysYType::TYPE _t){
    insert(ident, name, _t, UNKNOWN);
}

void SymbolTable::insertArray(const std::string &ident, const std::string &name, const std::vector<int> &len, SysYType::TYPE _t){
    SysYType *ty = new SysYType(_t, len);
    Symbol *sym = new Symbol(ident, name, ty);
    insert(sym);
}

bool SymbolTable::exists(const std::string &ident){
    return symbol_tb.find(ident) != symbol_tb.end();
}

Symbol *SymbolTable::Search(const std::string &ident){
    return symbol_tb.find(ident)->second;
}

int SymbolTable::getValue(const std::string &ident){
    return symbol_tb.find(ident)->second->ty->value;
}

SysYType *SymbolTable::getType(const std::string &ident){
    return symbol_tb.find(ident)->second->ty;
}

std::string SymbolTable::getName(const std::string &ident){
    return symbol_tb.find(ident)->second->name;
}


void SymbolTableStack::alloc(){
    // 移动语义，尾部位置插入
    sym_tb_st.emplace_back(new SymbolTable());
}

void SymbolTableStack::quit(){
    sym_tb_st.pop_back();
}

void SymbolTableStack::resetNameManager(){
    nm.reset();
}

void SymbolTableStack::insert(Symbol *symbol){
    sym_tb_st.back()->insert(symbol);
}

void SymbolTableStack::insert(const std::string &ident, SysYType::TYPE _type, int value){
    string name = nm.getName(ident);
    sym_tb_st.back()->insert(ident, name, _type, value);
}

void SymbolTableStack::insertINT(const std::string &ident){
    string name = nm.getName(ident);
    sym_tb_st.back()->insertINT(ident, name);
}

void SymbolTableStack::insertINTCONST(const std::string &ident, int value){
    string name = nm.getName(ident);
    sym_tb_st.back()->insertINTCONST(ident, name, value);
}

void SymbolTableStack::insertFUNC(const std::string &ident, SysYType::TYPE _t){
    string name = "@" + ident;
    // 转到队尾表，插入函数
    sym_tb_st.back()->insertFUNC(ident, name, _t);
}

// 插入SysY库函数
void SymbolTableStack::insertSysYFunc() {
    insertFUNC("getint", SysYType::SYSY_FUNC_INT);
    insertFUNC("getch", SysYType::SYSY_FUNC_INT);
    insertFUNC("getarray", SysYType::SYSY_FUNC_INT);
    insertFUNC("putint", SysYType::SYSY_FUNC_VOID);
    insertFUNC("putch", SysYType::SYSY_FUNC_VOID);
    insertFUNC("putarray", SysYType::SYSY_FUNC_VOID);
    insertFUNC("starttime", SysYType::SYSY_FUNC_VOID);
    insertFUNC("stoptime", SysYType::SYSY_FUNC_VOID);
}

void SymbolTableStack::insertArray(const std::string &ident, const std::vector<int> &len, SysYType::TYPE _t){
    string name = nm.getName(ident);
    sym_tb_st.back()->insertArray(ident, name, len, _t);
}

bool SymbolTableStack::exists(const std::string &ident){
    for(int i = (int)sym_tb_st.size() - 1; i >= 0; --i){
        if(sym_tb_st[i]->exists(ident))
            return true;
    }
    return false;
}

int SymbolTableStack::getValue(const std::string &ident){
    int i = (int)sym_tb_st.size() - 1;
    for(; i >= 0; --i){
        if(sym_tb_st[i]->exists(ident))
            break;
    }
    return sym_tb_st[i]->getValue(ident);
}

SysYType *SymbolTableStack::getType(const std::string &ident){
    int i = (int)sym_tb_st.size() - 1;
    for(; i >= 0; --i){
        if(sym_tb_st[i]->exists(ident))
            break;
    }
    return sym_tb_st[i]->getType(ident);
}

std::string SymbolTableStack::getName(const std::string &ident){
    int i = (int)sym_tb_st.size() - 1;
    for(; i >= 0; --i){
        if(sym_tb_st[i]->exists(ident))
            break;
    }
    return sym_tb_st[i]->getName(ident);
}

std::string SymbolTableStack::getTmpName(){
    return nm.getTmpName();
}

std::string SymbolTableStack::getLabelName(const std::string &label_ident){
    return nm.getLabelName(label_ident);
}
std::string SymbolTableStack::getVarName(const std::string& var){
    return nm.getName(var);
}