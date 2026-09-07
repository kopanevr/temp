#include "EventDispatcher.hpp"

//

/// @brief
void EventDispatcher::subscribe() {}

/// @brief
void EventDispatcher::unsubscribe() {}

/// @brief
/// @param event Событие.
void EventDispatcher::processEvents(const Event &event) {
  switch (event.id) {
  case 0:
    break;

  default:
    break;
  }
}

/// @brief Извлекает и обрабатывает события.
/// @details Вызывается в главном потоке один раз за итерацию.
void EventDispatcher::consumeEvents() {
  size_t eventToProcess = 0;

  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (eventCount_ == 0) {
      return;
    }
    eventToProcess = eventCount_;
    std::swap(writeBuffer_, readBuffer_);
    // Сброс количества событий.
    eventCount_ = 0;
  }

  for (size_t i = 0; i < eventToProcess; i++) {
    processEvents((*readBuffer_)[i]);
  }
}
