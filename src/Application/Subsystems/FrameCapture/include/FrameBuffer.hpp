#pragma once

//

#include <mutex>
#include <condition_variable>

//

#include "BitField.hpp"

//

#include <opencv4/opencv2/opencv.hpp>

//

namespace frameCapture {
/// @brief
class FrameBuffer final {
public:
  /// @brief Конструктор.
  FrameBuffer() {
  }

  /// @brief Деструктор.
  ~FrameBuffer() = default;

  FrameBuffer(const FrameBuffer&) = delete;
  FrameBuffer& operator=(const FrameBuffer&) = delete;

  /// @brief
  /// @param frame
  void push(const cv::Mat &frame) {
    std::unique_lock<std::mutex> lock(mutex_);

    SET_FLAG(0, isFramePushed);
    cv_.notify_one();
  }

  /// @brief
  void pop(cv::Mat &frame) {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this]{ return GET_FLAG_STATE(0, isFramePushed); });

    RESET_FLAG(0, isFramePushed);
  }

private:
  /// @brief Битовое поле.
  BIT_FIELD(0, 1, FLAG(isFramePushed));

  /// @brief
  static inline FrameBuffer *instance_;

  /// @brief
  std::mutex mutex_;

  /// @brief
  std::condition_variable cv_;

  /// @brief
  cv::Mat frame_;
};
} // namespace frameCapture
