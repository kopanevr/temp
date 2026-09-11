#include "Inference.hpp"

//

#include "BitField.hpp"

//

// Подсистемы.

#include "Logger.hpp"

//

using namespace inference;

//

/// @brief Конструктор.
Inference::Inference()
    : eventDispatcher_(eventDispatcher::EventDispatcher::getInstance()) {
  // Инициализация.
  init();
}

/// @brief Деструктор.
Inference::~Inference() {
  if (inferenceThread_.joinable()) {
    inferenceThread_.join();
  }
}

/// @brief Тело процесса.
/// @details
void Inference::processBody() {
  STATIC_BIT_FIELD(0, 1, FLAG(isStarted));

  if (!GET_FLAG_STATE(0, isStarted)) {
    // Выполнение при первом запуске.

    inferenceThread_ = std::thread(&Inference::run, this);
    SET_FLAG(0, isStarted);
  } else {
    // Выполнение при последующих запусках.

    if (false) {
      // Стирание битового поля.
      ERASE_BIT_FIELD(0);
    }
  }
}

/// @brief
void Inference::run() {
  DEBUG("Подсистема ", subsystemHandle.name, " запущена.");
  while (true) {
    if (!body()) {
      break;
    }
  }
  DEBUG("Подсистема ", subsystemHandle.name, " остановлена.");
}

/// @brief
/// @return
bool Inference::body() {
  STATIC_BIT_FIELD(0, 1, FLAG(isStarted)); // Статическое битовое поле.

  // Запуск конвейера.
  pipeline();

  if (false) {
    return false;
  }
  return true;
}

/// @brief Конвейер.
void Inference::pipeline() {
  if (eventDispatcher_) {
  }
}
