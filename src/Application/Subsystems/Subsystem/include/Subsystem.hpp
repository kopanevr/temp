/**
 * @file
 * @brief Описание подсистемы.
 */

#pragma once

//

#include <cstdint>

//

#include <string>

//

#include "SubsystemId.hpp"

//

struct SubsystemHandle {
  /// @brief Идентификатор подсистемы.
  Subsys::SubsystemId id;
  /// @brief Имя подсистемы.
  /// @warning
  std::string name;

  /// @brief Состояние запуска подсистемы.
  bool isStarted : 1;
};

/// @brief Подсистема.
class Subsystem {
private:
protected:
  /// @brief Дескриптор подсистемы.
  SubsystemHandle subsystemHandle;
protected:
  /// @brief Инициализация подсистемы.
  virtual void init() = 0;
  /// @brief
  virtual void setBeforeStartUp() = 0;
  /// @brief
  virtual void setBeforeShutDown() = 0;
  /// @brief Тело основного цикла.
  virtual void processBody() = 0;
public:
  /// @brief Конструктор.
  Subsystem() = default;

  /// @brief Деструктор.
  virtual ~Subsystem() = default;

  /// @brief Запуск подсистемы.
  void startUp() {
      if (subsystemHandle.isStarted) return;
      setBeforeStartUp();
      subsystemHandle.isStarted = true;
  }

  /// @brief Остановка подсистемы.
  void shutDown() {
      if (!subsystemHandle.isStarted) return;
      setBeforeShutDown();
      subsystemHandle.isStarted = false;
  }

  /// @brief Возвращает идентификатор подсистемы.
  [[nodiscard]] Subsys::SubsystemId getId() const { return subsystemHandle.id; }

  /// @brief Проверка запуска подсистемы.
  [[nodiscard]] bool isRunning() const { return subsystemHandle.isStarted; }

  /// @brief Основной процесс.
  void process() {
    processBody();
  }
};
