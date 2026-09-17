#include "VideoCapture.hpp"

//

using namespace videoCapture;

//

/// @brief Конструктор.
VideoCapture::VideoCapture() {
  // Инициализация.
  init();
}

/// @brief Предварительная настройка перед запуском подсистемы.
void VideoCapture::setBeforeStartUp() {
  // Подготовка перед захватом.
  prepareBeforeStartVideCapture();
}

/// @brief
void VideoCapture::prepareBeforeStartVideCapture() {
  videoCapture_.reset(new (std::nothrow) cv::VideoCapture());
  if (!videoCapture_) {
    return;
  }

  if (!videoCapture_->isOpened()) {
    ERROR("Ошибка открытия устройства захвата.");
    return;
  }
}

/// @brief Тело процесса.
/// @details
void VideoCapture::processBody() {
  STATIC_BIT_FIELD(0, 1, FLAG(isStarted)); // Статическое битовое поле.

  if (!GET_FLAG_STATE(0, isStarted)) {
    // Выполнение при первом запуске.

    videoCaptureThread_ = std::thread(&VideoCapture::run, this);
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
void VideoCapture::run() {}
