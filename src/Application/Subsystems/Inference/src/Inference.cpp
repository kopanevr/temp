#include "Inference.hpp"

//

#include <filesystem>
#include <vector>

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

  inferenceContext_->modelPath.modelFilePath = "";
  inferenceContext_->modelPath.modelFileName = "";
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
#if (USER_OPTION_PROFILE_INFERENCE)
    inferenceContext_->sessionOptions->EnableProfiling("");
#endif
    if (std::filesystem::exists(inferenceContext_->optimizedModelPath.getPathToModelFile())) {
      // Создание сессии.
      inferenceContext_->session.reset(new (std::nothrow) Ort::Session(*inferenceContext_->env, inferenceContext_->optimizedModelPath.getPathToModelFile(), *inferenceContext_->sessionOptions));
    } else {
      // Установка уровня оптимизации модели.
      inferenceContext_->sessionOptions->SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
      // Установка пути к файлу оптимизированной модели.
      inferenceContext_->sessionOptions->SetOptimizedModelFilePath(inferenceContext_->optimizedModelPath.modelDirectoryPath);

      // Создание сессии.
      inferenceContext_->session.reset(new (std::nothrow) Ort::Session(*inferenceContext_->env, inferenceContext_->modelPath.getPathToModelFile(), *inferenceContext_->sessionOptions));
    }
  } else {
    ERROR("Ошибка при подготовке провайдера вывода.");
    return;
  }

  // Создание входных и выходных тензоров.
  if (!createInputOutputTensors()) {
    ERROR("Ошибка при создании входных и выходных тензоров");
  }
}

/// @brief Подготовка провайдера вывода.
/// @param options Опции.
bool Inference::prepareProvider(const uint8_t options) {
  DEBUG("Подготовка провайдера вывода.");
  return true;
}

/// @brief Создание входных и выходных тензоров.
/// @param
bool Inference::createInputOutputTensors()
{
  // Получение информации о модели.
  inferenceContext_->modelInfo = getModelInfo(*inferenceContext_);
  return true;
}

#ifndef NDEBUG
/// @brief
#define PRINT_TENSOR_SHAPE(tensorInfo)                                         \
  do {                                                                         \
    LOG("Размерность:");                                                       \
    LOG("[");                                                                  \
    for (const auto &dim : *tensorInfo.shape) {                                \
      dim != tensorInfo.shape->back() ? LOG(" ", dim, ",") : LOG(" ", dim);    \
    }                                                                          \
    LOG("]");                                                                  \
  } while (false)
#endif

/// @brief Возвращает информацию о модели.
/// @param inferenceContext Контекст вывода.
/// @return Информация о модели.
std::unique_ptr<ModelInfo> Inference::getModelInfo(const InferenceContext &inferenceContext) {
  // Создание информации о модели
  auto modelInfo = std::unique_ptr<ModelInfo>(new (std::nothrow) ModelInfo());
  if (!modelInfo) {
    return nullptr;
  }

  //
  Ort::AllocatorWithDefaultOptions allocator{{}};

  // Получение количества входов.
  modelInfo->inputCount = inferenceContext_->session->GetInputCount();
  for (std::size_t i = 0; i < modelInfo->inputCount; i++) {
    // Получение информации о типе входа.
    const auto typeInfo = inferenceContext_->session->GetInputTypeInfo(i);
    const auto tensorTypeAndShapeInfo = typeInfo.GetTensorTypeAndShapeInfo();

    // Получение имени входа.
    inferenceContext_->modelInfo->inputTensorsInfo.at(i).name = inferenceContext_->session->GetOutputNameAllocated(i, allocator).get();
    inferenceContext_->inputTensorNames.push_back(inferenceContext.modelInfo->inputTensorsInfo.at(i).name.c_str());

    TensorInfo tensorInfo{};

    // Получение типа данных элементов входа.
    tensorInfo.tensorElementDataType = tensorTypeAndShapeInfo.GetElementType();
    // Получение размерности.
    tensorInfo.shape = std::make_shared<std::vector<int64_t>>(tensorTypeAndShapeInfo.GetShape());
    if (!tensorInfo.shape) {
      return nullptr;
    }

    modelInfo->inputTensorsInfo.push_back(std::move(tensorInfo));
  }

#ifndef NDEBUG
  size_t i = 0; // Индекс тензора.
#if (USER_OPTION_SHOW_MODEL_INFO == 1)
    // Вывод информации о входах.
    LOG("Входы: ");
    LOG("Количество: ", modelInfo->inputCount);

    for (const auto& tensorInfo : modelInfo->inputTensorsInfo) {
      LOG(i++, ":");
      LOG("Имя ", tensorInfo.name);
      PRINT_TENSOR_SHAPE(tensorInfo); // Смотреть выше.
      LOG("Тип элементов: ", tensorInfo.tensorElementDataType);
    }
#endif
#endif

  // Получение количества выходов.
  modelInfo->outputCount = inferenceContext_->session->GetOutputCount();
  for (std::size_t i = 0; i < modelInfo->outputCount; i++) {
    // Получение информации о типе выхода.
    const auto typeInfo = inferenceContext_->session->GetOutputTypeInfo(i);
    const auto tensorTypeAndShapeInfo = typeInfo.GetTensorTypeAndShapeInfo();

    // Получение имени выхода.
    inferenceContext_->modelInfo->outputTensorsInfo.at(i).name = inferenceContext_->session->GetOutputNameAllocated(i, allocator).get();
    inferenceContext_->outputTensorNames.push_back(inferenceContext.modelInfo->outputTensorsInfo.at(i).name.c_str());

    TensorInfo tensorInfo{};

    // Получение типа данных элементов выхода.
    tensorInfo.tensorElementDataType = tensorTypeAndShapeInfo.GetElementType();
    // Получение размерности.
    tensorInfo.shape = std::make_shared<std::vector<int64_t>>(tensorTypeAndShapeInfo.GetShape());
    if (!tensorInfo.shape) {
      return nullptr;
    }

    modelInfo->outputTensorsInfo.push_back(std::move(tensorInfo));
  }

#ifndef NDEBUG
#if (USER_OPTION_SHOW_MODEL_INFO == 1)
    // Вывод информации о входах.
    LOG("Входы: ");
    LOG("Количество: ", modelInfo->outputCount);

    for (const auto& tensorInfo : modelInfo->outputTensorsInfo) {
      LOG(i++, ":");
      LOG("Имя ", tensorInfo.name);
      PRINT_TENSOR_SHAPE(tensorInfo); // Смотреть выше.
      LOG("Тип элементов: ", tensorInfo.tensorElementDataType);
    }
#endif
#endif
  return modelInfo;
}

#undef PRINT_TENSOR_SHAPE

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
