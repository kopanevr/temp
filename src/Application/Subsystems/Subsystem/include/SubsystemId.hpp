#pragma once

//

#include <cstdint>

//

namespace subsystemManager {
/// @brief Идентификаторы подсистем.
enum class SubsystemId : int8_t {
  SubsystemManager = -1,

  Logger,
  EventDispatcher,

  Count
};
} // namespace subsystemManager
