/**
 * @file
 * @brief
 */

#pragma once

//

#include <cstdint>
#include <cstddef>
#include <cassert>

//

#include <variant>
#include <array>
#include <atomic>

//

using UIEventID = uint32_t;
using VariantPayload = std::variant<int,float>;
using UIEventPriority = uint8_t;

//

/// @brief Событие.
struct UIEvent {
    UIEventID id;
    VariantPayload payload;
    UIEventPriority priority;
};

/// @brief
class EventDispatcher final : public {
private:
  static constexpr size_t BUFFER_SIZE = 2048u;
  std::array<UIEvent, BUFFER_SIZE> eventBuffer;

  std::atomic<size_t> writeIndex{0};
  size_t readIndex = 0;
private:
  /// @brief Конструктор.
  EventDispatcher() = default;
public:
  ~EventDispatcher();

  static EventDispatcher* getInstance() {
      static EventDispatcher instance{};
      return &instance;
  }

  /// @brief Возвращает ёмкость буфера событий.
  /// @return Ёмкость буфера событий
  static constexpr size_t getEventBufferCapacity() { return BUFFER_SIZE; }

  /// @param id
  /// @param payload
  /// @brief
  void postEvent(const UIEventID id, const VariantPayload payload);
  /// @brief
  /// @details Вызывается в главном потоке один раз за кадр.
  void commitEvents();
  /// @brief
  /// @details Вызывается в главном потоке один раз сразу после @ref swapBuffers.
  void processEvents();
  /// @brief
  void process();
};

/// @brief Публикует событие.
/// @param id Идентификатор события.
/// @param payload Нагрузка события.
/// @details Вызывается из любых потоков.
inline void EventDispatcher::postEvent(const UIEventID id, const VariantPayload payload) {
  size_t idx = writeIndex.fetch_add(1);

  assert(idx < BUFFER_SIZE);
  if (idx < BUFFER_SIZE) {
      eventBuffer[idx] = {id, payload};
  } else {
      WARNING("Буфер событий переполнен!");
  }
}

/// @brief Фиксирует события.
/// @details Вызывается в главном потоке один раз за кадр.
inline void EventDispatcher::commitEvents() {
  readIndex = writeIndex.load();
  if (readIndex > BUFFER_SIZE) readIndex = BUFFER_SIZE;
}

/// @brief
/// @details Вызывается в главном потоке один раз за кадр сразу после @ref commitEvents.
inline void EventDispatcher::processEvents() {
  for (const auto& event : eventBuffer) {
  }
}

/// @brief
/// @details Вызывается в главном потоке один раз за кадр.
inline void EventDispatcher::process() {
  commitEvents();
  processEvents();
}
