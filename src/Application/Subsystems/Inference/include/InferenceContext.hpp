#pragma once

//

#include <memory>

//

#include "onnxruntime_cxx_api.h"

//

namespace inference {
//// @brief
struct ModelInfo {
  /// @brief Количество входов.
  std::size_t inputCount;
  /// @brief Количество выходов.
  std::size_t outputCount;
};

/// @brief Тензор.
/// @details
struct Tensor final {
  struct MetaData final {
  } metaData;

  /// @brief Сырые данные тензора.
  std::vector<std::byte> rawData;
};

/// @brief Контекст вывода.
struct InferenceContext final {
  /// @brief Параметры пулов потоков.
  std::unique_ptr<Ort::ThreadingOptions> threadingOptions;
  /// @brief Окружение.
  std::unique_ptr<Ort::Env> env;
  /// @brief Параметры сессии.
  std::unique_ptr<Ort::SessionOptions> sessionOptions;
  /// @brief Сессию.
  std::unique_ptr<Ort::Session> session;
  /// @brief Указатель на информацию о модели.
  std::unique_ptr<ModelInfo> modelInfo;

  /// @brief Входные тензоры.
  std::vector<Tensor> inputTensors;
  /// @brief Выходные тензоры.
  std::vector<Tensor> outputTensors;

  /// @brief
  std::vector<Ort::Value> inputTensorValues;
  /// @brief
  std::vector<Ort::Value> outputTensorValues;

  /// @brief Имена входных тензоров.
  std::vector<const char *> inputTensorNames;
  /// @brief Имена выходных тензоров.
  std::vector<const char *> outputTensorNames;
};
} // namespace inference
