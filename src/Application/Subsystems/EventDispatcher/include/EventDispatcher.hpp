/**
 * @file
 * @brief Диспетчер событий.
 */

#pragma once

//

#include <cassert>
#include <cstddef>
#include <cstdint>

//

#include <array>
#include <functional>
#include <mutex>
#include <variant>

//

#include "Subsystem.hpp"

//

#include "Logger.hpp"

//

using EventID = uint64_t;
using VariantPayload = std::variant<int, float>;

//

/// @brief Событие.
struct Event {
  EventID id;
  VariantPayload payload;
};

using SubscriptionID = uint64_t;
using EventHandler = std::function<void(const Event &)>;

/// @brief Подписка.
struct Subscription {
  SubscriptionID id;
  EventHandler handler;
};

/// @brief
class EventDispatcher final : public Subsystem {
public:
  ~EventDispatcher() = default;

  static EventDispatcher *getInstance() {
    static EventDispatcher instance{};
    return &instance;
  }

  /// @brief Возвращает ёмкость буфера событий.
  /// @return Ёмкость буфера событий.
  static constexpr size_t getEventBufferCapacity() { return BUFFER_SIZE; }

  /// @brief
  void subscribe(const SubscriptionID id, EventHandler handler);
  /// @brief
  void unsubscribe(const SubscriptionID id, EventHandler handler);

  /// @brief Публикует событие.
  /// @details Вызывается из любых потоков.
  /// @param id Идентификатор события.
  /// @param payload Нагрузка события.
  /// @return Результат публикации.
  bool postEvent(const EventID id, const VariantPayload payload) {
    std::lock_guard<std::mutex> lock(postMutex_);
    if (eventCount_ >= BUFFER_SIZE) {
      assert(false);
      return false;
    }
    (*wBuffer_)[eventCount_] = {id, payload};
    eventCount_++;
    return true;
  }

private:
  /// @brief Конструктор.
  EventDispatcher() {
    // Инициализация.
    init();
  }

  /// @brief Инициализация подсистемы.
  void init() override {
    subsystemHandle.id = Subsys::SubsystemId::EventDispatcher;
    subsystemHandle.name = "EventDispatcher";
  }

  /// @brief Предварительная настройка перед запуском подсистемы.
  void setBeforeStartUp() override {}
  /// @brief Предварительная настройка перед остановкой подсистемы.
  void setBeforeShutDown() override {}

  /// @brief Извлекает и обрабатывает события.
  /// @details Вызывается в главном потоке один раз за итерацию.
  void consumeEvents();
  /// @brief
  void processEvents(const Event &event);

  /// @brief Тело процесса.
  void processBody() override { consumeEvents(); }

private:
  static constexpr size_t BUFFER_SIZE = 1024UL;
  std::array<Event, BUFFER_SIZE> bufferA_;
  std::array<Event, BUFFER_SIZE> bufferB_;

  std::array<Event, BUFFER_SIZE> *wBuffer_ = &bufferA_;
  std::array<Event, BUFFER_SIZE> *rBuffer_ = &bufferB_;

  std::mutex postMutex_;

  /// @brief Количество событий.
  size_t eventCount_ = 0;
};
