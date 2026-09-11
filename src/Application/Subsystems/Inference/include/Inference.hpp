/**
 * @brief
 *
 */

#pragma once

//

#include <thread>
#include <memory>

//

#include "Subsystem.hpp"

#include "InferenceContext.hpp"

//

// Подсистемы.

#include "EventDispatcher.hpp"

//

#include "onnxruntime_cxx_api.h"

//

namespace inference {
/// @brief
class Inference : public Subsystem {
public:
  /// @brief Деструктор.
  ~Inference();

  /// @brief
  /// @return
  static Inference *getInstance() {
    static Inference instance{};
    return &instance;
  }

private:
  /// @brief Конструктор.
  Inference();

  Inference &operator=(const Inference &) = delete;
  Inference(const Inference &) = delete;

  /// @brief Инициализация подсистемы.
  void init() override {
    SET_SUBSYSTEM_ID(subsystemManager::SubsystemId::Inference);
    SET_SUBSYSTEM_NAME("Inference");
  }

  /// @brief Предварительная настройка перед запуском подсистемы.
  void setBeforeStartUp() override;
  /// @brief Предварительная настройка перед остановкой подсистемы.
  void setBeforeShutDown() override {}

  /// @brief Тело процесса.
  void processBody() override;

  /// @brief Конвейер.
  void pipeline();

  /// @brief
  void run();

  /// @brief
  bool body();

  /// @brief Подготовка входных тензоров.
  bool prepareInputTensors();
  /// @brief
  bool inference();
  /// @brief Подготовка выходных тензоров.
  bool prepareOutputTensors();

  /// @brief Подготовка перед запуском вывода.
  /// @param options Опции. Дополнительно смотреть @ref inference::prepareSettings.
  void prepareBeforeStartInference([[maybe_unused]] const uint8_t options = 0);
  /// @brief Подготовка провайдера вывода.
  /// @param options Опции. Дополнительно смотреть @ref inference::prepareSettings.
  bool prepareProvider([[maybe_unused]] const uint8_t options = 0);
  /// @brief Создание входных и выходных тензоров.
  /// @param
  bool createInputOutputTensors();
  /// @brief Возвращает информацию о модели.
  /// @param inferenceContext Контекст вывода.
  /// @return Информация о модели.
  [[nodiscard]] std::unique_ptr<ModelInfo> getModelInfo(const InferenceContext &inferenceContext);
private:
  /// @brief
  std::thread inferenceThread_;

  /// @brief Диспетчер событий
  eventDispatcher::EventDispatcher *eventDispatcher_;

  /// @brief Контекст вывода.
  std::unique_ptr<InferenceContext> inferenceContext_;
};
} // namespace inference
