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
#include <mutex>
#include <shared_mutex>
#include <variant>

//

#include "Subsystem.hpp"

//

// Подсистемы.

#include "Logger.hpp"

//

#include "BitField.hpp"

//

using EventID = uint64_t;
using VariantPayload = std::variant<int, float>;

//

namespace eventDispatcher {
/// @brief Событие.
struct Event {
  EventID id;
  VariantPayload payload;
};

using SubscriptionID = uint64_t;
using EventHandler = void (*)(const Event &);

/// @brief Подписка.
struct Subscription {
  SubscriptionID id;
  EventHandler handler;
};

/// @brief
class EventDispatcher final : public Subsystem {
public:
  ~EventDispatcher() = default;
  EventDispatcher(const EventDispatcher &) = delete;
  EventDispatcher &operator=(const EventDispatcher &) = delete;

  static EventDispatcher *getInstance() {
    static EventDispatcher instance{};
    return &instance;
  }

  /// @brief
  /// @return Идентификатор подписки.
  [[nodiscard]] static constexpr SubscriptionID createEventUniqueID() {
    return {};
  }

  /// @brief Возвращает ёмкость буфера событий.
  /// @return Ёмкость буфера событий.
  static constexpr size_t getEventCap() { return MAX_EVENTS; }

  /// @brief Возвращает ёмкость буфера подписок.
  /// @return Ёмкость буфера подписок.
  static constexpr size_t getSubscriptionCap() { return MAX_SUBSCRIPTIONS; }

  /// @brief Подписывает на событие.
  /// @details Вызывается из любых потоков.
  /// @param id Идентификатор подписки.
  /// @warning
  /// @param handler Обработчик подписки.
  /// @return Результат подписки.
  bool subscribe(const SubscriptionID id, const EventHandler handler);
  /// @brief Отписывает от события.
  /// @details Вызывается из любых потоков.
  /// @param id Идентификатор подписки.
  void unsubscribe(const SubscriptionID id);

  /// @brief Публикует событие.
  /// @details Вызывается из любых потоков.
  /// @param id Идентификатор события.
  /// @param payload Нагрузка события.
  /// @return Результат публикации.
  bool postEvent(const EventID id, const VariantPayload payload) {
    std::lock_guard<std::mutex> lock(postMutex_);
    if (eventCount_ >= MAX_EVENTS) {
      assert(false);
      return false;
    }
    (*wEventBuffer_)[eventCount_] = {id, payload};
    eventCount_++;
    return true;
  }

#ifndef NDEBUG
  /// @brief Возвращает максимальное количество обработанных событий за
  /// итерацию.
  /// @return Максимальное количество обработанных событий за итерацию
  size_t getMaxEverEventCount() const { return maxEverEventCountToProcess_; }
#endif

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
  /// @brief Обрабатывает события.
  /// @param event Событие.
  [[deprecated]] void processEvents(const Event &event);

  /// @brief Тело процесса.
  void processBody() override {
#ifndef NDEBUG
    STATIC_BIT_FIELD(0, 1, FLAG(isStarted));
    if (!GET_FLAG_STATE(0, isStarted)) {
      DEBUG("Подсистема ", subsystemHandle.name, " запущена.");
      SET_FLAG(0, isStarted);
    }
#endif
    consumeEvents();
  }

private:
  static constexpr size_t MAX_EVENTS = 128UL;
  using Events = std::array<Event, MAX_EVENTS>;
  Events eventBufferA_;
  Events eventBufferB_;

  Events *wEventBuffer_ = &eventBufferA_;
  Events *rEventBuffer_ = &eventBufferB_;

  static constexpr size_t MAX_SUBSCRIPTIONS = 128UL;
  using Subscriptions = std::array<Subscription, MAX_SUBSCRIPTIONS>;
  Subscriptions subscriptionBuffer_;

  /// @brief
  std::shared_mutex subscribeMutex_;
  /// @brief
  std::mutex postMutex_;

  /// @brief Количество событий.
  size_t eventCount_ = 0;
  /// @brief Количество подписок.
  size_t subscriptionCount_ = 0;

#ifndef NDEBUG
  /// @brief Максимальное количество обработанных событий за итерацию.
  size_t maxEverEventCountToProcess_ = 0;
#endif
};
} // namespace eventDispatcher
