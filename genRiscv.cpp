#include "genRiscv.h"

bool RiscvString::immediate(int i)
{
    return -2048 <= i && i < 2048;
}

void RiscvString::binary(const std::string &op, const std::string &rd, const std::string &rs1, const std::string &rs2)
{
    riscv_str += "  " + op + std::string(6-op.length(),' ') + rd + ", " + rs1 + ", " + rs2 + "\n";
}

void RiscvString::two(const std::string &op, const std::string &a, const std::string &b)
{
    riscv_str += "  " + op + std::string(6 - op.length(), ' ') + a + ", " + b + "\n";
}

void RiscvString::append(const std::string &s)
{
    riscv_str += s;
}

void RiscvString::mov(const std::string &from, const std::string &to)
{
    riscv_str += "  mv    " + to + ", "  + from + '\n';
}

void RiscvString::ret(){
    riscv_str += "  ret\n";
}

void RiscvString::li(const std::string &to, int im)
{
    riscv_str += "  li    " + to + ", " + std::to_string(im) + "\n";
}

void RiscvString::load(const std::string &to, const std::string &base ,int offset)
{
    if(offset >= -2048 && offset < 2048)
        riscv_str += "  lw    " + to + ", " + std::to_string(offset) + "(" + base + ")\n";
    else
    {
        this->li("t3", offset);
        this->binary("add", "t3", "t3", base);
        riscv_str += "  lw    " + to + ", " + "0" + "(" + "t3" + ")\n";
    }
}


void RiscvString::store(const std::string &from, const std::string &base ,int offset)
{
    if(offset >= -2048 && offset < 2048)
        riscv_str += "  sw    " + from + ", " + std::to_string(offset) + "(" + base + ")\n";
    else
    {
        this->li("t3", offset);
        this->binary("add", "t3", "t3", base);
        riscv_str += "  sw    " + from + ", " + "0" + "(" + "t3" + ")\n";
    }
}

void RiscvString::sp(int delta){
    if(delta >= -2048 && delta < 2048)
    {
        this->binary("addi", "sp", "sp", std::to_string(delta));
    }
    else
    {
        this->li("t0", delta);
        this->binary("add", "sp", "sp", "t0");
    }
}

void RiscvString::label(const std::string &name)
{
    this->append(name + ":\n");
}

void RiscvString::bnez(const std::string &rs, const std::string &target)
{
    this->two("bnez", rs, target);
}

void RiscvString::jump(const std::string &target)
{
    this->append("  j     " + target + "\n");
}

void RiscvString::call(const std::string &func)
{
    this->append("  call " + func + "\n");
}

void RiscvString::zeroInitInt()
{
    this->append("  .zero 4\n");
}

void RiscvString::word(int i)
{
    this->append("  .word " + std::to_string(i) + "\n");
}

void RiscvString::la(const std::string &to, const std::string &name)
{
    this->append("  la    " + to + ", " + name + "\n");
}

const char* RiscvString::c_str()
{
    return riscv_str.c_str();
}