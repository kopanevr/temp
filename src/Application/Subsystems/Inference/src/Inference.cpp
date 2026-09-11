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
  STATIC_BIT_FIELD(0, 1, FLAG(isStarted)); // Статическое битовое поле.

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

/// @brief Предварительная настройка перед запуском подсистемы.
void Inference::setBeforeStartUp() {
  // Подготовка перед выводом.
  prepareBeforeStartInference();
}

namespace inference {
namespace prepareSettings {
/*
inline constexpr uint8_t option = 1U;
*/
} // namespace prepareSettings
} // namespace inference

/// @brief Подготовка перед запуском вывода.
/// @param options Опции. Дополнительно смотреть @ref prepareSettings.
void Inference::prepareBeforeStartInference(const uint8_t options) {
  /*
  if (options & prepareSettings::option) {
  }
  */

  // Создание контекста вывода.
  inferenceContext_.reset(new (std::nothrow) InferenceContext());
  if (!inferenceContext_) {
    return;
  }

  // Создание опций пулов потоков.
  inferenceContext_->threadingOptions.reset(new (std::nothrow) Ort::ThreadingOptions());
  if (!inferenceContext_->threadingOptions) {
    return;
  }

  // Создание окружения.
  inferenceContext_->env.reset(
      new (std::nothrow) Ort::Env(*inferenceContext_->threadingOptions, ORT_LOGGING_LEVEL_WARNING, "onnxInference"));
  if (!inferenceContext_->threadingOptions) {
    return;
  }

  // Создание опций сессии.
  inferenceContext_->sessionOptions.reset(new (std::nothrow) Ort::SessionOptions());
  if (!inferenceContext_->sessionOptions) {
    return;
  }

  if (prepareProvider()) {
    DEBUG("Подготовка провайдера вывода.");
#if (USER_OPTION_ENABLE_PROFILING)
    inferenceContext_->sessionOptions->EnableProfiling("");
#endif

  } else {
    ERROR("Ошибка при подготовке провайдера вывода.");
    return;
  }
}

/// @brief Подготовка провайдера вывода.
/// @param options Опции.
bool Inference::prepareProvider(uint8_t options) { return true; }

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

#define PROCESS(process)                                                       \
  if (!process()) {                                                            \
    SET_FLAG(0, isErrorAppeared);                                              \
    break;                                                                     \
  }                                                                            \
  step++;

/// @brief Конвейер.
void Inference::pipeline() {
  STATIC_BIT_FIELD(0, 1, FLAG(isErrorAppeared)); // Статическое битовое поле.

  int step = 0;

  switch (step) {
  case 0:
    PROCESS(prepareInputTensors);
  case 1:
    PROCESS(inference);
  case 2:
    PROCESS(prepareOutputTensors);

  default:
    break;
  }
}

#undef PROCESS

/// @brief Подготовка входных тензоров.
bool Inference::prepareInputTensors() {
  if (!inferenceContext_->inputTensors.empty()) {
    return false;
  }
  return true;
}

/// @brief
bool Inference::inference() { return true; }

/// @brief Подготовка выходных тензоров.
bool Inference::prepareOutputTensors() {
  if (!inferenceContext_->outputTensors.empty()) {
    return false;
  }
  return true;
}
