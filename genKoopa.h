#pragma once
#include <string>
#include <unordered_map>
#include <set>
#include <vector>
#include <stack>

class KoopaString{

public:
    // 用来记录生成的KoopaIR
    std::string koopa_str;

    void append(const std::string &s);

    // 单目运算
    void binary(const std::string &op, const std::string &rd, const std::string &s1, const std::string &s2);

    // 记录当前块号
    void label(const std::string &s);

    // 返回
    void ret(const std::string &name);

    // 为int类型变量分配空间
    void alloc(const std::string &name);

    // 为其他类型分配空间
    void alloc(const std::string &name, const std::string &type_str);

    // 为全局int变量分配空间
    void globalAllocINT(const std::string &name, const std::string &init = "zeroinit");

    // 为全局数组分配空间
    void globalAllocArray(const std::string &name, const std::string &array_type, const std::string &init);

    // 加载数据
    void load(const std::string & to, const std::string &from);

    // 存储数据
    void store(const std::string &from, const std::string &to);

    // 条件跳转
    void br(const std::string &v, const std::string &then_s, const std::string &else_s);

    // 无条件跳转
    void jump(const std::string &label);

    // 函数调用
    void call(const std::string &to, const std::string &func,const std::vector<std::string>& params);
    //
    void getelemptr(const std::string& to, const std::string &from, const int i);

    void getelemptr(const std::string& to, const std::string &from, const std::string& i);

    void getptr(const std::string& to, const std::string &from, const std::string& i);

    void declLibFunc();

    std::string getArrayType(const std::vector<int> &w);

    // 数组内容在ptr所指的内存区域，数组类型由len描述. ptr[i]为常量，或者是KoopaIR中的名字
    std::string getInitList(std::string *ptr, const std::vector<int> &len);

    const char * c_str();
};

