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
bool Application::prepare(int argc, char *argv[]) {
  // Вывод информации о приложении.
  printInfo();

  DEBUG("Инициализация приложения.");

  // Создание контекста приложения.
  applicationContext_.reset(new (std::nothrow) ApplicationContext());
  if (!applicationContext_) {
    return false;
  }

  // Создание интерпретатора команд.
  commandInterpreter_.reset(new (std::nothrow)
                                cmd::CommandInterpreter(argc, argv));
  if (!commandInterpreter_) {
    return false;
  }

  // Проверка состояния обработки аргументов.
  if (!commandInterpreter_->isParsed()) {
    return false;
  }

  subsystemManager_.reset(new (std::nothrow)
                              subsystemManager::SubsystemManager());
  if (!subsystemManager_) {
    return false;
  }

  cmd::CommandInterpreter::instance_ = commandInterpreter_.get();
  subsystemManager::SubsystemManager::instance_ = subsystemManager_.get();

  // Запуск менеджера подсистем.
  subsystemManager_->startUp();

  return true;
}

/// @brief Инициализация.
/// @details
/// @param argc Количество аргументов.
/// @param argv Указатель на список аргументов.
/// @return Состояние выполнения.
int Application::init(int argc, char *argv[]) {
  if (!prepare(argc, argv)) {
    ERROR("Инициализация приложения не завершена.");
    return EXIT_FAILURE;
  }
  DEBUG("Инициализация приложения завершена.");
  applicationContext_->state = ApplicationContext::State::Ready;
  return EXIT_SUCCESS;
}

/// @brief Деструктор.
Application::~Application() {
  // Деинициализация.
  deinit();
}

/// @brief Выполнение.
/// @return Состояние выполнения.
int Application::exec() {
  if (applicationContext_->state != ApplicationContext::State::Ready) {
    return EXIT_FAILURE;
  }

  // Таймер для отсчета периода времени с момента запуска приложения.
  Timer timerToTimeSinceStartApplication(0u);

  applicationContext_->state = ApplicationContext::State::Running;

  // Запуск таймера и возврат времени запуска.
  START_TIMER_FIRST_TIME_AND_GET_START_TIME(timerManager_,
                                            timerToTimeSinceStartApplication,
                                            applicationContext_->startTime);

  subsystemManager_->process();

  // Остановка таймера и возврат истекшего периода времени.
  STOP_TIMER_AND_GET_ELAPSED_TIME(timerManager_,
                                  timerToTimeSinceStartApplication,
                                  applicationContext_->executedTime);

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

  cmd::CommandInterpreter::instance_ = nullptr;
  subsystemManager::SubsystemManager::instance_ = nullptr;

  applicationContext_->state = app::ApplicationContext::State::Deinitialized;
}
