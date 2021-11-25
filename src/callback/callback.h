/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef CALLBACK_CALLBACK_H_
#define CALLBACK_CALLBACK_H_

#include "callback/bind.h"

namespace runner {

class BindStateBase;

class CallbackBase {
 public:
  bool is_null() const { return !bind_state_; }
  explicit operator bool() const { return !is_null(); }

 protected:
  using InvokeFuncStorage = BindStateBase::InvokeFuncStorage;

  bool EqualsInternal(const CallbackBase& other) const {
    return bind_state_ == other.bind_state_;
  }

  CallbackBase() : bind_state_(nullptr) {}

  explicit inline CallbackBase(BindStateBase* bind_state)
    : bind_state_(bind_state) {}

  InvokeFuncStorage polymorphic_invoke() const {
    return bind_state_->polymorphic_invoke_;
  }

  ~CallbackBase() = default;

  BindStateBase* bind_state_;
};

template <typename R, typename... Args>
class TaskCallback<R(Args...)> : public CallbackBase {
 public:
  using RunType = R(Args...);
  using PolymorphicInvoke = R(*)(BindStateBase*, PassingType<Args>...);

  constexpr TaskCallback() = default;
  TaskCallback(std::nullptr_t) = delete;

  explicit TaskCallback(BindStateBase* bind_state)
      : CallbackBase(bind_state) {}

  TaskCallback(const TaskCallback&) = delete;
  TaskCallback& operator=(const TaskCallback&) = delete;

  TaskCallback(TaskCallback&&) noexcept = default;
  TaskCallback& operator=(TaskCallback&&) noexcept = default;

  R Run(Args... args) const & {
    PolymorphicInvoke f =
        reinterpret_cast<PolymorphicInvoke>(this->polymorphic_invoke());
    return f(this->bind_state_, std::forward<Args>(args)...);
  }
};

}  // namespace runner

#endif  // CALLBACK_CALLBACK_H_