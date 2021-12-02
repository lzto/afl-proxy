///
/// handle environment knobs
/// 2021 Tong Zhang <ztong0001@gmail.com>
///
#ifndef _ENV_KNOB_
#define _ENV_KNOB_
#include <assert.h>
#include <stdlib.h>
#include <string>

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
  int getIntValue() {
    assert(value.length() != 0);
    return atoi(value.c_str());
  };
  std::string getStringValue() { return value; }

private:
  const std::string name;
  std::string value;
};
#endif
