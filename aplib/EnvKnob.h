///
/// handle environment knobs
/// 2021 Tong Zhang <ztong0001@gmail.com>
///

#include <stdlib.h>

class EnvKnob {
public:
  EnvKnob(const std::string name) : name(name), value("") {
    auto *val = getenv(name.c_str());
    if (val == nullptr)
      return;
    value = std::string(val);
  }
  ~EnvKnob(){};
  bool isPresented() { return value.length() != 0; }
  bool isSet() {
    if (value.length() == 0)
      return false;
    if (value == "1")
      return true;
    if (value == "true")
      return true;
    return false;
  };
  int getValue() {
    assert(value.length() != 0);
    return atoi(value.c_str());
  };

private:
  const std::string name;
  std::string value;
};
