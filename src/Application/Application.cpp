#include "Application.hpp"

//

#include <cstdlib>

//

using namespace app;

//
/// @brief Настройка.
/// @details
/// @param argc Количество аргументов.
/// @param argv Указатель на список аргументов.
bool Application::prepare(int argc, char* argv[]) {
  // Вывод информации о приложении.
  printInfo();

  DEBUG("Инициализация приложения.");

  // Создание контекста приложения.
  applicationContext.reset(new(std::nothrow) ApplicationContext());
  if (!applicationContext) return false;

  // Создание интерпретатора команд.
  commandLineInterpreter.reset(new(std::nothrow) CommandLineInterpreter(argc, argv));
  if (!commandLineInterpreter) return false;

  // Проверка состояния обработки аргументов.
  if (!commandLineInterpreter->isParsed()) return false;

  SubsysManager.reset(new(std::nothrow) SubsystemManager());
  if (!SubsysManager) return false;

  CommandLineInterpreter::instance = commandLineInterpreter.get();
  SubsystemManager::instance = SubsysManager.get();

  // Запуск менеджера подсистем.
  SubsysManager->startUp();

  return true;
}

/// @brief Инициализация.
/// @details
/// @param argc Количество аргументов.
/// @param argv Указатель на список аргументов.
bool Application::init(int argc, char* argv[]) {
  if (!prepare(argc, argv)) {
    ERROR("Инициализация приложения не завершена.");
    return false;
  }
  DEBUG("Инициализация приложения завершена.");
  applicationContext->state = ApplicationContext::State::Ready;
  return true;
}

/// @brief Деструктор.
Application::~Application() {
  // Деинициализация.
  deinit();
}

/// @brief Выполнение.
/// @return Результат выполнения.
int Application::exec() {
  if (!(applicationContext->state != ApplicationContext::State::Ready))
    return EXIT_FAILURE;

  // Таймер для отсчета периода времени с момента запуска приложения.
  Timer timerToTimeSinceStartApplication(0u);

  applicationContext->state = ApplicationContext::State::Running;

  // Запуск таймера и возврат времени запуска.
  START_TIMER_FIRST_TIME_AND_GET_START_TIME(
    timerManager, timerToTimeSinceStartApplication, applicationContext->startTime);

  int ret = SubsysManager->process();

  // Остановка таймера и возврат истекшего периода времени.
  STOP_TIMER_AND_GET_ELAPSED_TIME(timerManager, timerToTimeSinceStartApplication, applicationContext->executedTime);

  if (ret)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

/// @brief Деинициализация.
/// @details Производит остановку менеджера подсистем.
void Application::deinit() {
  if (applicationContext->state == app::ApplicationContext::State::Ready ||
      applicationContext->state == app::ApplicationContext::State::Running) {
      // Остановка менеджера подсистем.
      SubsysManager->shutDown();
  }

  DEBUG("Период времени выполнения приложения составил: ",
      applicationContext->executedTime, " миллисекунд [мсек].");

  CommandLineInterpreter::instance = nullptr;
  SubsystemManager::instance = nullptr;

  applicationContext->state = app::ApplicationContext::State::Deinitialized;
}

/// @brief Вывод информации о приложении.
void Application::printInfo() const {
  LOG("Информация о приложении:");

  LOG("Мажорная версия:", 0);
  LOG("Минорная версия:", 0);
  LOG("Номер сборки:", 0);

  LOG("Дата сборки:", __DATE__);
  LOG("Время сборки:", __TIME__);

  SEPARATOR;
}
