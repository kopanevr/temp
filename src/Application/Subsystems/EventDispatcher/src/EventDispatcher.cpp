#include "EventDispatcher.hpp"

//

using namespace eventDispatcher;

//

/// @brief Подписывает на событие.
/// @details Вызывается из любых потоков.
/// @param id Идентификатор подписки.
/// @param handler Обработчик подписки.
/// @warning
/// @return Результат подписки.
bool EventDispatcher::subscribe(const SubscriptionID id,
                                const EventHandler handler) {
  std::unique_lock<std::shared_mutex> lock(subscribeMutex_);
  if (subscriptionCount_ >= MAX_SUBSCRIPTIONS) {
    assert(false);
    return false;
  }
  // Проверка на дубликат.
  for (size_t i = 0; i < subscriptionCount_; i++) {
    if (id == subscriptionBuffer_[i].id) {
      return false;
    }
  }
  subscriptionBuffer_[subscriptionCount_] = {id, handler};
  subscriptionCount_++;
  return true;
}

/// @brief Отписывает от события.
/// @details Вызывается из любых потоков.
/// @param id Идентификатор подписки.
/// @param handler Обработчик подписки.
void EventDispatcher::unsubscribe(const SubscriptionID id) {
  std::unique_lock<std::shared_mutex> lock(subscribeMutex_);
  for (size_t i = 0; i < subscriptionCount_; i++) {
    if (subscriptionBuffer_[i].id == id) {
      if (i < subscriptionCount_ - 1) {
        // Перемещение последнего элемента.
        subscriptionBuffer_[i] = subscriptionBuffer_[subscriptionCount_ - 1];
      }
      // Очистка последнего элемента.
      subscriptionBuffer_[subscriptionCount_ - 1] = {};
      subscriptionCount_--;
      return;
    }
  }
}

/// @brief Обрабатывает события.
/// @param event Событие.
void EventDispatcher::processEvents(const Event &event) {
  std::array<EventHandler, MAX_SUBSCRIPTIONS> localHandlerBuffer;
  size_t handlerCountToProcess = 0;

  {
    std::shared_lock<std::shared_mutex> lock(subscribeMutex_);
    handlerCountToProcess = subscriptionCount_;
    for (size_t i = 0; i < handlerCountToProcess; i++) {
      localHandlerBuffer[i] = subscriptionBuffer_[i].handler;
    }
  }

  for (size_t i = 0; i < handlerCountToProcess; i++) {
    if (localHandlerBuffer[i]) {
      localHandlerBuffer[i](event);
    }
  }
}

/// @brief Извлекает и обрабатывает события.
/// @details Вызывается в главном потоке один раз за итерацию.
void EventDispatcher::consumeEvents() {
  size_t eventCountToProcess = 0;

  {
    std::lock_guard<std::mutex> lock(postMutex_);
    if (eventCount_ == 0) {
      return;
    }
    eventCountToProcess = eventCount_;
    std::swap(wEventBuffer_, rEventBuffer_);
    // Сброс количества событий.
    eventCount_ = 0;
  }

  for (size_t i = 0; i < eventCountToProcess; i++) {
    processEvents((*rEventBuffer_)[i]);
  }
}
