#pragma once

//

#include <cstdint>

//

namespace Subsys
{
    /// @brief Идентификаторы подсистем.
    enum class SubsystemId : uint8_t
    {
        SubsystemManager,

        //
        Logger,
        //

        Count
    };
}