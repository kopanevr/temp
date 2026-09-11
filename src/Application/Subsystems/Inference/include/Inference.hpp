/**
 * @brief
 *
 */

#pragma once

//

#include <thread>

//

#include "Subsystem.hpp"

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
  void setBeforeStartUp() override {}
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

private:
  /// @brief
  std::thread inferenceThread_;

  /// @brief Диспетчер событий
  eventDispatcher::EventDispatcher *eventDispatcher_;
};
} // namespace inference
