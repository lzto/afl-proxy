#include <stdint.h>
#include <set>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <sstream>
class HWInputValue {
public:
  enum class ValueType {
    RANGE,
    ValueSet,
    BitSet,
  };

  HWInputValue(uint64_t start, uint64_t end)
    : range_start(start), range_end(end) {
    vtype = ValueType::RANGE;
  }
  
  HWInputValue(ValueType t) {
    vtype = t;
  }

  void insertValue(uint64_t v) {
    if (vtype != ValueType::ValueSet || vtype != ValueType::BitSet) {
      return;
    }
    vbset.insert(v);
  }

  inline bool isEmpty() {
    if (vtype == ValueType::RANGE) {
      return range_start == range_end;
    }
    return vbset.empty();
  }

  inline ValueType getType() { return vtype; }

private:
  union {
    struct {
      uint64_t range_start;
      uint64_t range_end;
    };
    std::set<uint64_t> vbset;

  };
  ValueType vtype;
};

class HWInput {
public:
  HWInput() {}
  HWInput(int off, int nbytes)
    :offset(off), bytes(nbytes) {
      value_set = nullptr;
      bit_set = nullptr;
  }
  inline bool hasRange() { return ranges.size() != 0; }
  inline std::vector<HWInputValue*> & getRanges() { return ranges; }
  inline HWInputValue * getValueSet() { return value_set; }
  inline HWInputValue * getBitsSet() { return bit_set; }

  void addRange(HWInputValue* v) {
    ranges.push_back(v);
  }

  void addValue(uint64_t v) {
    if (!value_set) {
      value_set = new HWInputValue(HWInputValue::ValueType::ValueSet);
    }
    value_set->insertValue(v);
  }

  void addBits(uint64_t v) {
    if (!bit_set) {
      bit_set = new HWInputValue(HWInputValue::ValueType::BitSet);
    }
    bit_set->insertValue(v);
  }

private:
  std::vector<HWInputValue*> ranges;
  HWInputValue *value_set;
  HWInputValue *bit_set;
  int offset;
  int bytes;
};

class HWInputModel {
public:
  HWInputModel(const std::string & name) {
    base_struct_name = name;
  }
  HWInput & getInputAt(int offset) {
    return inputs[offset];
  }
  std::unordered_map<int, HWInput> & getInputs() {
    return inputs;
  }
private:
  std::string base_struct_name;
  std::unordered_map<int, HWInput> inputs;
};

static std::string op_symbols[] = {
  "&&", "||", "!",
  "<", "<=", ">", ">=", "==",
  "!=", "&", "|", "^",
  "+", "*", "/", "-", "<<", ">>",
};

class Expression {
public:
  enum class ExpType {
    AND, OR, NOT,
    LT, LE, GT, GE, EQ,
    NE, BITAND, BITOR, BITXOR,
    ADD, MUL, DIV, SUB, LSHIFT, RSHIFT,
    VALUE,
  };
  
  Expression(ExpType t) :exp_type(t) {}

  virtual std::string str()=0;
  inline ExpType getType() { return exp_type; };
  inline bool isValue() { return exp_type == ExpType::VALUE; }
  
protected:
  ExpType exp_type;
};

class ArithExpr: public Expression {
public:
  ArithExpr(ExpType t, Expression * l, Expression * r)
    : Expression(t), lhs(l), rhs(r) {}
  std::string str() { 
    std::string res;
    if (rhs) {
      res = lhs->str() + " " + op_symbols[(int)exp_type] + " " + rhs->str();
    } else {
      res = op_symbols[(int)exp_type] + lhs->str();
    }
    res = "(" + res + ")";
    return res;
  }
  void setRHS(Expression * expr) {
    rhs = expr;
  }
  void setLHS(Expression * expr) {
    lhs = expr;
  }
private:
  Expression * lhs;
  Expression * rhs;
};

class ValueExpr: public Expression {
public:
  enum class ValueType {
    HW_INPUT,
    CONSTANT,
  };
  
  ValueExpr(const std::string & name, int off, int sz) 
    :Expression(ExpType::VALUE)  
  {
    base_type_name = name;
    off = offset;
    size = sz;
    vtype = ValueType::HW_INPUT;
  }
  ValueExpr(uint64_t v) 
    :Expression(ExpType::VALUE) 
  {
    value = v;
    vtype = ValueType::CONSTANT;
  }
  inline bool isConstant() { return vtype == ValueType::CONSTANT; }
  inline bool isHWInput() { return vtype == ValueType::HW_INPUT; }
  std::string str() {
    if (isConstant()) {
      std::ostringstream ss;
      ss << "0x" << std::hex << value;
      return ss.str();
    } else {
      return "x";
    }
  }
private:
  std::string base_type_name;
  int offset;
  int size;
  uint64_t value;
  ValueType vtype;
};

