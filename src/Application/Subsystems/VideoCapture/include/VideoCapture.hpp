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

//

// Подсистемы.

#include "EventDispatcher.hpp"

//

#include <opencv4/opencv2/opencv.hpp>

//

namespace subsystemManager {
  class SubsystemManager;
}

//

namespace videoCapture {
/// @brief
class VideoCapture : public Subsystem {
public:
  /// @brief Деструктор.
  ~VideoCapture() = default;

  /// @brief
  /// @return
  static VideoCapture *getInstance() {
    return instance_;
  }

private:
  /// @brief Конструктор.
  VideoCapture();

  VideoCapture &operator=(const VideoCapture &) = delete;
  VideoCapture(const VideoCapture &) = delete;

  /// @brief Дружественный класс.
  friend class subsystemManager::SubsystemManager;

  /// @brief Инициализация подсистемы.
  void init() override {
    SET_SUBSYSTEM_ID(subsystemManager::SubsystemId::VideoCapture);
    SET_SUBSYSTEM_NAME("VideoCapture");
  }

  /// @brief Предварительная настройка перед запуском подсистемы.
  void setBeforeStartUp() override;
  /// @brief Предварительная настройка перед остановкой подсистемы.
  void setBeforeShutDown() override {}

  /// @brief
  void prepareBeforeStartVideCapture();

  /// @brief Тело процесса.
  void processBody() override;

  /// @brief
  void run();

private:
  /// @brief
  static inline VideoCapture *instance_;

  /// @brief
  std::unique_ptr<cv::VideoCapture> videoCapture_;

  /// @brief
  std::thread videoCaptureThread_;
};
} // namespace videoCapture
