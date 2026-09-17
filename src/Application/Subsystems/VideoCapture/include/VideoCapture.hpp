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

namespace videoCapture {
/// @brief
class VideoCapture : public Subsystem {
public:
  /// @brief Деструктор.
  ~VideoCapture() = default;

  /// @brief
  /// @return
  static VideoCapture *getInstance() {
    static VideoCapture instance{};
    return &instance;
  }

private:
  /// @brief Конструктор.
  VideoCapture();

  VideoCapture &operator=(const VideoCapture &) = delete;
  VideoCapture(const VideoCapture &) = delete;

  /// @brief Инициализация подсистемы.
  void init() override {
    SET_SUBSYSTEM_ID(subsystemManager::SubsystemId::VideoCapture);
    SET_SUBSYSTEM_NAME("VideoCapture");
  }

  /// @brief Предварительная настройка перед запуском подсистемы.
  void setBeforeStartUp() override {}
  /// @brief Предварительная настройка перед остановкой подсистемы.
  void setBeforeShutDown() override {}

  /// @brief Тело процесса.
  void processBody() override;

  /// @brief
  void run();

private:
  /// @brief
  std::unique_ptr<cv::VideoCapture> videoCapture_;

  /// @brief
  std::thread videoCaptureThread_;
};
} // namespace videoCapture
