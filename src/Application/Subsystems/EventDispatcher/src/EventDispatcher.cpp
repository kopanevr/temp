#include "EventDispatcher.hpp"

//

/// @brief
/// @param id
/// @param handler
void EventDispatcher::subscribe(const SubscriptionID id, EventHandler handler) {

}

/// @brief
/// @param id
/// @param handler
void EventDispatcher::unsubscribe(const SubscriptionID id, EventHandler handler) {

}

/// @brief
/// @param event Событие.
void EventDispatcher::processEvents(const Event &event) {
  for (size_t i = 0; i < 0; i++) {
  }
}

/// @brief Извлекает и обрабатывает события.
/// @details Вызывается в главном потоке один раз за итерацию.
void EventDispatcher::consumeEvents() {
  size_t eventToProcess = 0;

  {
    std::lock_guard<std::mutex> lock(postMutex_);
    if (eventCount_ == 0) {
      return;
    }
    eventToProcess = eventCount_;
    std::swap(wBuffer_, rBuffer_);
    // Сброс количества событий.
    eventCount_ = 0;
  }

  for (size_t i = 0; i < eventToProcess; i++) {
    processEvents((*rBuffer_)[i]);
  }
}
