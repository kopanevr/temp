#pragma once

//

#include "Subsystem.hpp"

//

namespace temp {
class temp : public Subsystem {
private:
  /* data */
public:
  temp(/* args */);
  ~temp();
};

temp::temp(/* args */) {}

temp::~temp() {}

} // namespace temp
