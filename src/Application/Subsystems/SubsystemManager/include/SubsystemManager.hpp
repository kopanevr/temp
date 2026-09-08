/**
 * @file
 * @brief Описание менеджера подсистем.
 * @details Для добавления подсистемы в список менеджера стоит воспользоваться
 * @ref ADD_SUBSYSTEM.
 * @details Порядок вызова метода @ref process подсистемы зависит от порядка
 * добавления подсистемы в список менеджера.
 */

#pragma once

//

#include <cassert>
#include <cstddef>
#include <cstdlib>

//

#include <array>

//

#include "Subsystem.hpp"
#include "SubsystemId.hpp"

//

// Подсистемы.

#include "EventDispatcher.hpp"
#include "Logger.hpp"

//

/// @brief Добавляет подсистему в список менеджера подсистем.
/// @param name Имя класса подсистемы, производного от @ref Subsystem.
#define ADD_SUBSYSTEM(name)                                                    \
  do {                                                                         \
    assert(i < subsystemCount_);                                               \
    subsystems_[i++] = name::getInstance();                                    \
  } while (0)

//

/// @brief Менеджер подсистем.
class SubsystemManager final : public Subsystem {
public:
  /// @brief Получает доступ к объекту.
  /// @return
  static SubsystemManager &getInstance() { return *instance_; }

  /// @brief Настройка перед запуском подсистемы.
  void setBeforeStartUp() override {
    for (const auto &item : subsystems_) {
      item->startUp();
    }
  }

  /// @brief Настройка перед остановкой подсистемы.
  void setBeforeShutDown() override {
    for (const auto &item : subsystems_) {
      item->shutDown();
    }
  }

  /// @brief Тело основного цикла.
  void processBody() override {
    DEBUG("Подсистема ", subsystemHandle.name, " запущена");
    while (true) {
      for (const auto &item : subsystems_) {
        item->process();
      }
    }
  }

  /// @brief Возвращает количество подсистем.
  /// @return Количество подсистем.
  static constexpr size_t getSubsystemCount() { return subsystemCount_; }

  /// @brief Возвращает подсистему по идентификатору.
  /// @param id Идентификатор.
  /// @return Указатель на подсистему.
  constexpr Subsystem *getSubsystemById(const Subsys::SubsystemId id) const {
    for (const auto &item : subsystems_) {
      if (item->getId() == id)
        return item;
    }
    return nullptr;
  }

private:
  /// @brief Конструктор.
  SubsystemManager() {
    // Инициализация.
    init();

    // Добавление подсистем.
    size_t i = {};
    ADD_SUBSYSTEM(logger::Logger);
    ADD_SUBSYSTEM(eventDispatcher::EventDispatcher);
  }

  /// @brief Дружественный класс.
  friend class Application;

  /// @brief Инициализация подсистемы.
  void init() override {
    subsystemHandle.id = Subsys::SubsystemId::SubsystemManager;
    subsystemHandle.name = "Manager";
  }

private:
  /// @brief Количество подсистем.
  static constexpr size_t subsystemCount_ =
      static_cast<size_t>(Subsys::SubsystemId::Count) - 1u;
  /// @brief Подсистемы.
  std::array<Subsystem *, subsystemCount_> subsystems_;

  /// @brief Указатель на экземпляр.
  static SubsystemManager *instance_;
};

#undef ADD_SUBSYSTEM
