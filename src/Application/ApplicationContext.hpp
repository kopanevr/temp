#pragma once

//

#include <cstdint>

//

#include <chrono>

//

namespace App
{
    /// @brief
    struct ApplicationContext final
    {
        /// @brief Время запуска.
        std::chrono::steady_clock::time_point startTime;
        /// @brief Период времени выполнения.
        std::chrono::steady_clock::duration executedTime;

        /// @brief Состояние.
        enum class State : uint8_t
        {
            Uninitialized,
            Ready,
            Running,
            Deinitialized
        }
        state{State::Uninitialized};
    };
}
