/**
 * @file
 * @brief Содержит описание интерпретатора команд.
 */

//

#include <cassert>

//

namespace cmd {
/// @brief Аргументы.
struct CommandLineArgs final {};
} // namespace cmd

//

/// @brief Интерпретатор команд.
class CommandInterpreter final {
public:
  /// @brief Деструктор.
  ~CommandInterpreter() = default;

  /// @brief
  /// @return
  static CommandInterpreter &getInstance() { return *instance_; }

  /// @brief Возвращает ссылку на аргументы.
  /// @return Ссылка на аргументы.
  const cmd::CommandLineArgs &getArgs() const { return args_; }

  /// @brief
  /// @return
  bool isParsed() const { return isSuccessfullyParsed_; }

private:
  /// @brief Конструктор.
  /// @param argc Количество аргументов.
  /// @param argv Указатель на список аргументов.
  CommandInterpreter(int argc, char *argv[]) { process(argc, argv); }

  /// @brief Дружественный класс.
  friend class Application;

  /// @brief Тело процесса.
  /// @param argc Количество аргументов.
  /// @param argv Указатель на список аргументов.
  void bodyProcess(int argc, char *argv[]);

  /// @brief Процесс.
  void process(int argc, char *argv[]) { bodyProcess(argc, argv); }

private:
  /// @brief Указатель на экземпляр.
  static CommandInterpreter *instance_;

  /// @brief Аргументы.
  cmd::CommandLineArgs args_;

  /// @brief Состояние обработки команд.
  bool isSuccessfullyParsed_ : 1;
};
