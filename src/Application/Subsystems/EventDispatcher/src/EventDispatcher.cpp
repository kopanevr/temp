#include "EventDispatcher.hpp"

//

/// @brief
void EventDispatcher::subscribe()
{}

/// @brief
void EventDispatcher::unsubscribe()
{}

/// @brief
/// @param event Событие.
void EventDispatcher::processEvents(const Event& event) {
  switch (event.id) {
  case 0:
    break;

  default:
    break;
  }
}

/// @brief Публикует событие.
/// @details Вызывается из любых потоков.
/// @param id Идентификатор события.
/// @param payload Нагрузка события.
bool EventDispatcher::postEvent(const EventID id, const VariantPayload payload) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (eventCount_ >= BUFFER_SIZE) {
    assert(false);
    return false;
  }
  (*writeBuffer_)[eventCount_] = {id, payload};
  eventCount_++;
  return true;
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
