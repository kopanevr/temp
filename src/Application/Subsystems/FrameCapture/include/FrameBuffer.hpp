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


  /// @brief
  /// @param frame
  void push(const cv::Mat &) {
    std::lock_guard<std::mutex> lock(mutex_);

    SET_FLAG(0, isFramePushed);
    cv_.notify_one();
  }

  /// @brief
  void pop() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this]{ return GET_FLAG_STATE(0, isFramePushed); });
  }

private:
  /// @brief Битовое поле.
  BIT_FIELD(0, 1, FLAG(isFramePushed));

  /// @brief
  std::mutex mutex_;

  /// @brief
  std::condition_variable cv_;
};
} // namespace frameCapture
