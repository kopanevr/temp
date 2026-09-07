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
  applicationContext_.reset(new(std::nothrow) ApplicationContext());
  if (!applicationContext_) return false;

  // Создание интерпретатора команд.
  commandLineInterpreter_.reset(new(std::nothrow) CommandLineInterpreter(argc, argv));
  if (!commandLineInterpreter_) return false;

  // Проверка состояния обработки аргументов.
  if (!commandLineInterpreter_->isParsed()) return false;

  subsystemManager_.reset(new(std::nothrow) SubsystemManager());
  if (!subsystemManager_) return false;

  CommandLineInterpreter::instance_ = commandLineInterpreter_.get();
  SubsystemManager::instance_ = subsystemManager_.get();

  // Запуск менеджера подсистем.
  subsystemManager_->startUp();

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
  applicationContext_->state = ApplicationContext::State::Ready;
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
  if (!(applicationContext_->state != ApplicationContext::State::Ready))
    return EXIT_FAILURE;

  // Таймер для отсчета периода времени с момента запуска приложения.
  Timer timerToTimeSinceStartApplication(0u);

  applicationContext_->state = ApplicationContext::State::Running;

  // Запуск таймера и возврат времени запуска.
  START_TIMER_FIRST_TIME_AND_GET_START_TIME(
    timerManager_, timerToTimeSinceStartApplication, applicationContext_->startTime);

  subsystemManager_->process();

  // Остановка таймера и возврат истекшего периода времени.
  STOP_TIMER_AND_GET_ELAPSED_TIME(timerManager_, timerToTimeSinceStartApplication, applicationContext_->executedTime);

  return EXIT_SUCCESS;
}

/// @brief Деинициализация.
/// @details Производит остановку менеджера подсистем.
void Application::deinit() {
  if (applicationContext_->state == ApplicationContext::State::Ready ||
      applicationContext_->state == ApplicationContext::State::Running) {
    // Остановка менеджера подсистем.
    subsystemManager_->shutDown();
  }

  DEBUG("Период времени выполнения приложения составил: ",
      applicationContext_->executedTime, " миллисекунд [мсек].");

  CommandLineInterpreter::instance_ = nullptr;
  SubsystemManager::instance_ = nullptr;

  applicationContext_->state = app::ApplicationContext::State::Deinitialized;
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
