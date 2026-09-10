#pragma once

//

#include "Subsystem.hpp"

//

namespace temp {
class Temp : public Subsystem {
public:
  /// @brief Деструктор.
  ~Temp() = default;

  /// @brief
  /// @return
  static Temp *getInstance() {
    static Temp instance{};
    return &instance;
  }

private:
  /// @brief Конструктор.
  Temp() {
    // Инициализация.
    init();
  }

  Temp &operator=(const Temp &) = delete;
  Temp(const Temp &) = delete;

  /// @brief Инициализация подсистемы.
  void init() override {
    SET_SUBSYSTEM_ID(subsystemManager::SubsystemId::Temp);
    SET_SUBSYSTEM_NAME("Temp");
  }

  /// @brief Предварительная настройка перед запуском подсистемы.
  void setBeforeStartUp() override {}
  /// @brief Предварительная настройка перед остановкой подсистемы.
  void setBeforeShutDown() override {}

  /// @brief Тело процесса.
  void processBody() override {}

private:
};
} // namespace temp
