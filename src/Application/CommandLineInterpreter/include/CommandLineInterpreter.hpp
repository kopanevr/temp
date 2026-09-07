/**
 * @file
 * @brief Содержит описание интерпретатора команд.
 */

//

#include <cassert>

//

/// @brief Аргументы.
struct CommandLineArgs final {
  /// @brief Путь к директории модели.
  char* pathToModelDirectory;
};

//

/// @brief Интерпретатор команд.
class CommandLineInterpreter final {
public:
  /// @brief Деструктор.
  ~CommandLineInterpreter() = default;

  /// @brief
  /// @return
  static CommandLineInterpreter& getInstance() { return *instance_; }

  /// @brief Возвращает ссылку на аргументы.
  /// @return Ссылка на аргументы.
  const CommandLineArgs& getArgs() const { return args_; }

  /// @brief
  /// @return
  bool isParsed() const { return isSuccessfullyParsed_; }
private:
  /// @brief Конструктор.
  /// @param argc Количество аргументов.
  /// @param argv Указатель на список аргументов.
  CommandLineInterpreter(int argc, char* argv[]) {
    process(argc, argv);
  }

  /// @brief Дружественный класс.
  friend class Application;

  /// @brief Тело процесса.
  /// @param argc Количество аргументов.
  /// @param argv Указатель на список аргументов.
  void bodyProcess(int argc, char* argv[]);

  /// @brief Процесс.
  void process(int argc, char* argv[]) {
    bodyProcess(argc, argv);
  }
private:
  /// @brief Указатель на экземпляр.
  static CommandLineInterpreter* instance_;

  /// @brief Аргументы.
  CommandLineArgs args_;

  /// @brief Состояние обработки команд.
  bool isSuccessfullyParsed_ : 1;
};
