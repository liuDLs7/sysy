#pragma once
#include <string>
#include <memory>
#include <vector>

/*
Number        ::= INT_CONST;
UnaryOp       ::= "+" | "-" | "!";
 */

// 所有类的声明
class BaseAST; 
class CompUnitAST;              //CompUnit      ::= [CompUnit] (Decl | FuncDef);
class DeclAST;                  // Decl          ::= ConstDecl | VarDecl;
class ConstDeclAST;             //ConstDecl     ::= "const" BType ConstDef {"," ConstDef} ";";
class BTypeAST;                 /*BType         ::= "int";
                                  FuncType      ::= "void" | "int";
                                  此处将BType与FuncType归为一类
                                */

class ConstDefAST;              //ConstDef      ::= IDENT {"[" ConstExp "]"} "=" ConstInitVal;
class ConstInitValAST;          //ConstInitVal  ::= ConstExp | "{" [ConstInitVal {"," ConstInitVal}] "}";
class VarDeclAST;               //VarDecl       ::= BType VarDef {"," VarDef} ";";
class VarDefAST;                /*VarDef        ::= IDENT {"[" ConstExp "]"}
                                                    | IDENT {"[" ConstExp "]"} "=" InitVal;
                                */

class InitValAST;               //InitVal       ::= Exp | "{" [InitVal {"," InitVal}] "}";
class FuncDefAST;               //FuncDef       ::= FuncType IDENT "(" [FuncFParams] ")" Block;
class FuncFParamsAST;           //FuncFParams   ::= FuncFParam {"," FuncFParam};
class FuncFParamAST;            //FuncFParam    ::= BType IDENT ["[" "]" {"[" ConstExp "]"}];
class BlockAST;                 //Block         ::= "{" {BlockItem} "}";
class BlockItemAST;             //BlockItem     ::= Decl | Stmt;


class StmtAST;                  /*Stmt          ::= LVal "=" Exp ";"
                                                  | [Exp] ";"
                                                  | Block
                                                  | "if" "(" Exp ")" Stmt ["else" Stmt]
                                                  | "while" "(" Exp ")" Stmt
                                                  | "break" ";"
                                                  | "continue" ";"
                                                  | "return" [Exp] ";";
                                */

class ExpAST;                   //Exp           ::= LOrExp;
class LValAST;                  //LVal          ::= IDENT {"[" Exp "]"};
class PrimaryExpAST;            //PrimaryExp    ::= "(" Exp ")" | LVal | Number;
class UnaryExpAST;              //UnaryExp      ::= PrimaryExp | IDENT "(" [FuncRParams] ")" | UnaryOp UnaryExp;
class FuncRParamsAST;           //FuncRParams   ::= Exp {"," Exp};
class MulExpAST;                //MulExp        ::= UnaryExp | MulExp ("*" | "/" | "%") UnaryExp;
class AddExpAST;                //AddExp        ::= MulExp | AddExp ("+" | "-") MulExp;
class RelExpAST;                //RelExp        ::= AddExp | RelExp ("<" | ">" | "<=" | ">=") AddExp;
class EqExpAST;                 //EqExp         ::= RelExp | EqExp ("==" | "!=") RelExp;
class LAndExpAST;               //LAndExp       ::= EqExp | LAndExp "&&" EqExp;
class LOrExpAST;                //LOrExp        ::= LAndExp | LOrExp "||" LAndExp;
class ConstExpAST;              //ConstExp      ::= Exp;


class ArrayIndexConstExpList;
class ArrayIndexExpList;



// 所有 AST 的基类
class BaseAST {
public:
    virtual ~BaseAST() = default;
    // 完成LR的打印，并返回计算结果（临时变量或立即数）所在的变量
    // virtual std::string Dump() const = 0;
};


class CompUnitAST : public BaseAST {
public:
    std::vector<std::unique_ptr<FuncDefAST>> func_defs;
    std::vector<std::unique_ptr<DeclAST>> decls;
    void Dump()const;
    void DumpGlobalVar() const;
};


class DeclAST : public BaseAST {
public:
    enum TAG {CONST_DECL, VAR_DECL};
    TAG tag;
    std::unique_ptr<ConstDeclAST> const_decl;
    std::unique_ptr<VarDeclAST> var_decl;
    void Dump() const;
};


class ConstDeclAST : public BaseAST {
public:
    std::vector<std::unique_ptr<ConstDefAST>> const_defs;
    std::unique_ptr<BTypeAST> btype;
    void Dump() const;
};


class BTypeAST : public BaseAST {
public:
    enum TAG {VOID, INT};
    TAG tag;
    void Dump() const;
};


class ConstDefAST : public BaseAST {
public:
    enum TAG { VARIABLE, ARRAY };
    TAG tag;
    std::string ident;
    std::vector<std::unique_ptr<ConstExpAST>> const_exps;   // size !=0, Array
    std::unique_ptr<ConstInitValAST> const_init_val;
    void Dump(bool is_global = false) const;
    void DumpArray(bool is_global = false) const;
};


class ConstInitValAST : public BaseAST {
public:
    enum TAG { CONST_EXP, CONST_INIT_LIST };
    TAG tag;
    std::unique_ptr<ConstExpAST> const_exp;
    std::vector<std::unique_ptr<ConstInitValAST>> inits;
    // 表达式求值，计算结果放在pi所指的int内存地址
    int getValue();
    void getInitVal(std::string *ptr, const std::vector<int> &len) const;
};


class VarDeclAST : public BaseAST {
public:
    std::vector<std::unique_ptr<VarDefAST>> var_defs;
    std::unique_ptr<BTypeAST> btype;
    void Dump() const;
};


class VarDefAST: public BaseAST {
public:
    enum TAG { VARIABLE, ARRAY };
    TAG tag;
    std::string ident;
    std::vector<std::unique_ptr<ConstExpAST>> const_exps;   // size != 0, Array
    std::unique_ptr<InitValAST> init_val;   // nullptr implies no init_val
    void Dump(bool is_global = false) const;
    void DumpArray(bool is_global = false) const;
};


class InitValAST : public BaseAST{
public:
    enum TAG { EXP, INIT_LIST};
    TAG tag;
    std::unique_ptr<ExpAST> exp;
    std::vector<std::unique_ptr<InitValAST>> inits; // can be 0, 1, 2,....
    std::string Dump() const;
    void getInitVal(std::string *ptr, const std::vector<int> &len, bool is_global = false) const;
};


class FuncDefAST : public BaseAST {
public:
    std::unique_ptr<BTypeAST> btype;
    std::string ident;
    std::unique_ptr<FuncFParamsAST> func_params;
    std::unique_ptr<BlockAST> block;
    void Dump() const;
};


class FuncFParamsAST : public BaseAST {
public:
    std::vector<std::unique_ptr<FuncFParamAST>> func_f_params;
    void Dump() const;
};


class FuncFParamAST : public BaseAST {
public:
    enum TAG { VARIABLE, ARRAY };
    TAG tag;
    std::unique_ptr<BTypeAST> btype;
    std::string ident;
    std::vector<std::unique_ptr<ConstExpAST>> const_exps;
    std::string Dump() const;                               // 返回参数类型，如i32, *[i32, 4]
    void getIndex(std::vector<int> &len);
};


class BlockAST : public BaseAST {
public:
    std::vector<std::unique_ptr<BlockItemAST>> block_items;
    void Dump(bool new_symbol_tb = true) const;
};


class BlockItemAST : public BaseAST {
public:
    enum TAG {DECL, STMT};
    TAG tag;
    std::unique_ptr<DeclAST> decl;
    std::unique_ptr<StmtAST> stmt;
    void Dump() const;
};


class StmtAST : public BaseAST {
public:
    enum TAG {EXP, BLOCK, IF, WHILE, BREAK, CONTINUE, RETURN, ASSIGN};
    TAG tag;
    std::unique_ptr<ExpAST> exp;
    std::unique_ptr<LValAST> lval;
    std::unique_ptr<BlockAST> block;
    std::unique_ptr<StmtAST> stmt;
    std::unique_ptr<StmtAST> if_stmt;
    std::unique_ptr<StmtAST> else_stmt;
    void Dump() const;
};


class ExpAST : public BaseAST {
public:
    std::unique_ptr<LOrExpAST> l_or_exp;
    std::string Dump() const;
    int getValue();
};


class PrimaryExpAST : public BaseAST {
public:
    enum TAG { PARENTHESES, NUMBER, LVAL};
    TAG tag;
    std::unique_ptr<ExpAST> exp;
    std::unique_ptr<LValAST> lval;
    int number;
    std::string Dump() const ;
    int getValue();
};


class UnaryExpAST : public BaseAST {
public:
    enum TAG { PRIMARY_EXP, OP_UNITARY_EXP, FUNC_CALL};
    TAG tag;
    std::unique_ptr<PrimaryExpAST> primary_exp;
    char unary_op;
    std::unique_ptr<UnaryExpAST> unary_exp;
    std::string ident;
    std::unique_ptr<FuncRParamsAST> func_params;
    std::string Dump() const;
    int getValue();
};

class LValAST : public BaseAST {
public:
    enum TAG { VARIABLE, ARRAY };
    TAG tag;
    std::string ident;
    std::vector<std::unique_ptr<ExpAST>> exps;      // exps.size() != 0 implies ARRAY
    std::string Dump(bool dump_ptr = false) const;   // 默认返回的是i32而非指针。
    int getValue();
};

class ConstExpAST : public BaseAST {
public:
    std::unique_ptr<ExpAST> exp;
    int getValue();
};

class ArrayIndexConstExpList : public BaseAST {
public:
    std::vector<std::unique_ptr<ConstExpAST>> const_exps;
};

class ArrayIndexExpList : public BaseAST {
public:
    std::vector<std::unique_ptr<ExpAST>> exps;
};


class ConstExpListAST: public BaseAST {
public:
    std::vector<std::unique_ptr<ConstExpAST>> const_exps;
};

class ExpListAST: public BaseAST {
public:
    std::vector<std::unique_ptr<ExpAST>> exps;
    int getValue();
};



class MulExpAST : public BaseAST {
public:
    enum TAG {UNARY_EXP, OP_MUL_EXP};
    TAG tag;
    std::unique_ptr<UnaryExpAST> unary_exp;
    std::unique_ptr<MulExpAST> mul_exp_1;
    std::unique_ptr<UnaryExpAST> unary_exp_2;
    char mul_op;
    std::string Dump() const;
    int getValue();
};


class AddExpAST : public BaseAST {
public:
    enum TAG {MUL_EXP, OP_ADD_EXP};
    TAG tag;
    std::unique_ptr<MulExpAST> mul_exp;
    std::unique_ptr<AddExpAST> add_exp_1;
    std::unique_ptr<MulExpAST> mul_exp_2;
    char add_op;
    std::string Dump() const;
    int getValue();
};


class RelExpAST : public BaseAST {
public:
    enum TAG {ADD_EXP, OP_REL_EXP};
    TAG tag;
    std::unique_ptr<AddExpAST> add_exp;
    std::unique_ptr<RelExpAST> rel_exp_1;
    std::unique_ptr<AddExpAST> add_exp_2;
    char rel_op[2];     // <,>,<=,>=
    std::string Dump() const;
    int getValue();
};

class EqExpAST : public BaseAST {
public:
    enum TAG {REL_EXP, OP_EQ_EXP};
    TAG tag;
    std::unique_ptr<RelExpAST> rel_exp;
    std::unique_ptr<EqExpAST> eq_exp_1;
    std::unique_ptr<RelExpAST> rel_exp_2;
    char eq_op;     // =,!
    std::string Dump() const;
    int getValue();
};

class LAndExpAST : public BaseAST {
public:
    enum TAG {EQ_EXP, OP_L_AND_EXP};
    TAG tag;
    std::unique_ptr<EqExpAST> eq_exp;
    std::unique_ptr<LAndExpAST> l_and_exp_1;
    std::unique_ptr<EqExpAST> eq_exp_2;
    std::string Dump() const;
    int getValue();
};

class LOrExpAST : public BaseAST {
public:
    enum TAG {L_AND_EXP, OP_L_OR_EXP};
    TAG tag;
    std::unique_ptr<LAndExpAST> l_and_exp;
    std::unique_ptr<LOrExpAST> l_or_exp_1;
    std::unique_ptr<LAndExpAST> l_and_exp_2;
    std::string Dump() const;
    int getValue();
};

class FuncRParamsAST : public BaseAST {
public:
    std::vector<std::unique_ptr<ExpAST>> exps;
    std::string Dump() const;
};
