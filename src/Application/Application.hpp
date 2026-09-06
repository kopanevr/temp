/**
 * @file
 * @brief Содержит описание приложения.
 */

#pragma once

#include <memory>

//

#include "CommandLineInterpreter.hpp"

//

#include "TimerManager.hpp"

#include "BitField.hpp"

//

#include "ApplicationContext.hpp"

//

// Подсистемы.

#include "SubsystemManager.hpp"

//

namespace app {
/// @brief Приложение.
class Application final {
private:
  /// @brief Контекст приложения.
  std::unique_ptr<app::ApplicationContext> applicationContext;
  /// @brief Интерпретатор команд.
  std::unique_ptr<CommandLineInterpreter> commandLineInterpreter;
  /// @brief Менеджер подсистем.
  std::unique_ptr<SubsystemManager> SubsysManager;

  /// @brief Менеджер таймера.
  TimerManager timerManager;
private:
  /// @brief Конструктор.
  Application() = default;
  ~Application();

  /// @brief Подготовка при инициализации.
  /// @details
  /// @param argc Количество аргументов.
  /// @param argv Указатель на список аргументов.
  bool prepare(int argc, char* argv[]);

  /// @brief Вывод информации о приложении.
  void printInfo() const;
public:
  /// @brief
  static Application* getInstance() {
      static Application instance{};
      return &instance;
  }

  /// @brief Инициализация.
  /// @details
  /// @param argc Количество аргументов.
  /// @param argv Указатель на список аргументов.
  bool init(int argc, char* argv[]);
  /// @brief Деинициализация.
  /// @details
  void deinit();

  /// @brief Выполнение.
  /// @return Результат выполнения.
  int exec();
};
} // namespace app
