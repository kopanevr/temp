#pragma once

//

#include <cstdint>

//

namespace subsystemManager {
/// @brief Идентификаторы подсистем.
enum class SubsystemId : uint8_t {
  SubsystemManager,

  Logger,
  EventDispatcher,

  Count
};
} // namespace subsystemManager
