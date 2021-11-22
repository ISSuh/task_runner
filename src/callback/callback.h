/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef CALLBACK_CALLBACK_H_
#define CALLBACK_CALLBACK_H_

#include "callback/bind.h"
#include "callback/callback_traits.h"

namespace runner {

class BindStateBase;

class CallbackBase {
 public:
  bool is_null() const { return !bind_state_; }
  explicit operator bool() const { return !is_null(); }

 protected:
  using InvokeFuncStorage = BindStateBase::InvokeFuncStorage;

  // Returns true if this callback equals |other|. |other| may be null.
  bool EqualsInternal(const CallbackBase& other) const {
    return bind_state_ == other.bind_state_;
  }

  CallbackBase() = default;

  // Allow initializing of |bind_state_| via the constructor to avoid default
  // initialization of the scoped_refptr.
  explicit inline CallbackBase(BindStateBase* bind_state)
    : bind_state_(bind_state) {}

  InvokeFuncStorage polymorphic_invoke() const {
    return bind_state_->polymorphic_invoke_;
  }

  // Force the destructor to be instantiated inside this translation unit so
  // that our subclasses will not get inlined versions.  Avoids more template
  // bloat.
  ~CallbackBase() = default;

  BindStateBase* bind_state_;
};

template <typename R, typename... Args>
class Callback<R(Args...)> : public CallbackBase {
 public:
  using RunType = R(Args...);
  using PolymorphicInvoke = R(*)(BindStateBase*, PassingType<Args>...);

  constexpr Callback() = default;
  Callback(std::nullptr_t) = delete;

  explicit Callback(BindStateBase* bind_state)
      : CallbackBase(bind_state) {}

  Callback(const Callback&) = delete;
  Callback& operator=(const Callback&) = delete;

  Callback(Callback&&) noexcept = default;
  Callback& operator=(Callback&&) noexcept = default;

  R Run(Args... args) const & {
  }

  R Run(Args... args) && {
  }
};

}  // namespace runner

#endif  // CALLBACK_CALLBACK_H_