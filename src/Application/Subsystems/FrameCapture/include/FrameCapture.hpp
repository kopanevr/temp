/**
 * @brief
 *
 */

#pragma once

//

#include <memory>
#include <thread>

//

#include "Subsystem.hpp"

//

// Подсистемы.

#include "EventDispatcher.hpp"

//

#include "FrameBuffer.hpp"

//

#include <opencv4/opencv2/opencv.hpp>

//

namespace subsystemManager {
class SubsystemManager;
}

//

namespace frameCapture {
/// @brief
class FrameCapture : public Subsystem {
public:
  /// @brief Деструктор.
  ~FrameCapture() = default;

  /// @brief
  /// @return
  static FrameCapture *getInstance() { return instance_; }

private:
  /// @brief Конструктор.
  FrameCapture(std::shared_ptr<FrameBuffer> frameBuffer);

  FrameCapture &operator=(const FrameCapture &) = delete;
  FrameCapture(const FrameCapture &) = delete;

  /// @brief Дружественный класс.
  friend class subsystemManager::SubsystemManager;

  /// @brief Инициализация подсистемы.
  void init() override {
    SET_SUBSYSTEM_ID(subsystemManager::SubsystemId::FrameCapture);
    SET_SUBSYSTEM_NAME("FrameCapture");
  }

  /// @brief Предварительная настройка перед запуском подсистемы.
  void setBeforeStartUp() override;
  /// @brief Предварительная настройка перед остановкой подсистемы.
  void setBeforeShutDown() override {}

  /// @brief
  void prepareBeforeStartFrameCapture();

  /// @brief Тело процесса.
  void processBody() override;

  /// @brief
  void run();

private:
  /// @brief
  static inline FrameCapture *instance_;

  /// @brief
  std::unique_ptr<cv::VideoCapture> videoCapture_;

  /// @brief
  std::thread frameCaptureThread_;

  /// @brief
  std::shared_ptr<FrameBuffer> frameBuffer_;
};
} // namespace frameCapture
