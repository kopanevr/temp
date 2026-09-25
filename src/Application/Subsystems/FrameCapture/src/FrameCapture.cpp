#include "FrameCapture.hpp"

//

using namespace frameCapture;

//

/// @brief Конструктор.
FrameCapture::FrameCapture(std::shared_ptr<FrameBuffer> frameBuffer)
    : frameBuffer_(frameBuffer) {
  // Инициализация.
  init();
}

/// @brief Предварительная настройка перед запуском подсистемы.
void FrameCapture::setBeforeStartUp() {
  // Подготовка перед захватом.
  prepareBeforeStartFrameCapture();
}

/// @brief
void FrameCapture::prepareBeforeStartFrameCapture() {
  videoCapture_.reset(new (std::nothrow) cv::VideoCapture());
  if (!videoCapture_) {
    return;
  }

  if (!videoCapture_->open(0)) {
    ERROR("Ошибка открытия устройства захвата.");
    return;
  }
}

/// @brief Тело процесса.
/// @details
void FrameCapture::processBody() {
  STATIC_BIT_FIELD(0, 1, FLAG(isStarted)); // Статическое битовое поле.

  if (!GET_FLAG_STATE(0, isStarted)) {
    // Выполнение при первом запуске.

    frameCaptureThread_ = std::thread(&FrameCapture::run, this);
    SET_FLAG(0, isStarted);
  } else {
    // Выполнение при последующих запусках.

    if (false) {
      // Стирание битового поля.
      ERASE_BIT_FIELD(0);
    }
  }
}

/// @brief
void FrameCapture::run() {
  while (true) {
    if (true) {
    }
  }
}
