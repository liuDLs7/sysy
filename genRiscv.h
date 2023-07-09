#pragma once
#include <string>
#include <unordered_map>
#include <set>
#include <vector>
#include <stack>

class RiscvString{

public:
    std::string riscv_str;
    /**
     * 默认只用t0 t1 t2
     * t3 t4 t5作为备用，临时的，随时可能被修改，不安全
    */

    bool immediate(int i);

    void binary(const std::string &op, const std::string &rd, const std::string &rs1, const std::string &rs2);

    void two(const std::string &op, const std::string &a, const std::string &b);

    void append(const std::string &s);

    void mov(const std::string &from, const std::string &to);

    void ret();

    void li(const std::string &to, int im);

    void load(const std::string &to, const std::string &base ,int offset);


    void store(const std::string &from, const std::string &base ,int offset);

    void sp(int delta);

    void label(const std::string &name);

    void bnez(const std::string &rs, const std::string &target);

    void jump(const std::string &target);

    void call(const std::string &func);

    void zeroInitInt();

    void word(int i);

    void la(const std::string &to, const std::string &name);

    const char* c_str();

};
