#include "genKoopa.h"

void KoopaString::append(const std::string &s)
{
    koopa_str += s;
}

// 单目运算
void KoopaString::binary(const std::string &op, const std::string &rd, const std::string &s1, const std::string &s2)
{
    koopa_str += "  " + rd + " = " + op + " " + s1 + ", " + s2 + "\n";
}

// 记录当前块号
void KoopaString::label(const std::string &s)
{
    koopa_str += s + ":\n";
}

// 返回
void KoopaString::ret(const std::string &name)
{
    koopa_str +="  ret " + name + "\n";
}

// 为int类型变量分配空间
void KoopaString::alloc(const std::string &name)
{
    koopa_str += "  " + name + " = alloc i32\n";
}

// 为其他类型分配空间
void KoopaString::alloc(const std::string &name, const std::string &type_str)
{
    koopa_str += "  " + name + " = alloc " + type_str + "\n";
}

// 为全局int变量分配空间
void KoopaString::globalAllocINT(const std::string &name, const std::string &init)
{
    koopa_str += "global " + name + " = alloc i32, " + init + "\n";
}

// 为全局数组分配空间
void KoopaString::globalAllocArray(const std::string &name, const std::string &array_type, const std::string &init)
{
    koopa_str += "global " + name + " = alloc " + array_type + ", " + init + "\n";
}

// 加载数据
void KoopaString::load(const std::string & to, const std::string &from)
{
    koopa_str += "  " + to + " = load " + from + '\n';
}

// 存储数据
void KoopaString::store(const std::string &from, const std::string &to)
{
    koopa_str += "  store " + from + ", " + to + '\n';
}

// 条件跳转
void KoopaString::br(const std::string &v, const std::string &then_s, const std::string &else_s)
{
    koopa_str += "  br " + v + ", " + then_s + ", " + else_s + '\n';
}

// 无条件跳转
void KoopaString::jump(const std::string &label)
{
    koopa_str += "  jump " + label + '\n';
}

// 函数调用
void KoopaString::call(const std::string &to, const std::string &func,const std::vector<std::string>& params)
{
    if(to.length()){
        koopa_str += "  " + to + " = ";
    }else{
        koopa_str += "  ";
    }
    koopa_str += "call " + func +"(";
    if(params.size()){
        int n = params.size();
        koopa_str += params[0];
        for(int i = 1; i < n; ++i){
            koopa_str += ", " + params[i];
        }
    }
    koopa_str += ")\n";
}

//
void KoopaString::getelemptr(const std::string& to, const std::string &from, const int i)
{
    koopa_str += "  " + to + " = getelemptr " + from + ", " + std::to_string(i) + "\n";
}

void KoopaString::getelemptr(const std::string& to, const std::string &from, const std::string& i)
{
    koopa_str += "  " + to + " = getelemptr " + from + ", " + i + "\n";
}

void KoopaString::getptr(const std::string& to, const std::string &from, const std::string& i)
{
    koopa_str += "  " + to + " = getptr " + from + ", " + i + "\n";
}

void KoopaString::declLibFunc()
{
    this->append("decl @getint(): i32\n");
    this->append("decl @getch(): i32\n");
    this->append("decl @getarray(*i32): i32\n");
    this->append("decl @putint(i32)\n");
    this->append("decl @putch(i32)\n");
    this->append("decl @putarray(i32, *i32)\n");
    this->append("decl @starttime()\n");
    this->append("decl @stoptime()\n");
    this->append("\n");
}

std::string KoopaString::getArrayType(const std::vector<int> &w)
{
    // int a[w0][w1]...[wn-1]
    std::string ans = "i32";
    for(int i = w.size() - 1; i >= 0; --i){
        ans = "[" + ans + ", " + std::to_string(w[i]) + "]";
    }
    return ans;
}

// 数组内容在ptr所指的内存区域，数组类型由len描述. ptr[i]为常量，或者是KoopaIR中的名字
std::string KoopaString::getInitList(std::string *ptr, const std::vector<int> &len)
{
    std::string ret = "{";
    if(len.size() == 1){
        int n = len[0];
        ret += ptr[0];
        for(int i = 1; i < n; ++i){
            ret += ", " + ptr[i];
        }
    } else {
        int n = len[0], width = 1;
        std::vector<int> sublen(len.begin() + 1, len.end());
        for(auto iter = len.end() - 1; iter != len.begin(); --iter)
            width *= *iter;
        ret += getInitList(ptr, sublen);
        for(int i = 1; i < n; ++i){
            ret += ", " + getInitList(ptr + width * i, sublen);
        }
    }
    ret += "}";
    return ret;
}


const char * KoopaString::c_str()
{
    return koopa_str.c_str();
}
