
/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#include <iostream>
#include <mutex>

#include "base/time.h"
#include "task_manager.h"
#include "task/task_runner.h"
#include "task/task_dispatcher.h"

#include "callback/callback.h"
#include "callback/bind.h"

runner::TaskManager manager;
std::mutex m;

void TestFunc() {
  m.lock();
  static uint32_t count = 0;
  std::cout << __func__ << " - count : " << count << " / " << runner::Time::InNanosecond() << std::endl;

  ++count;

  if (count < 50) {
    runner::TaskDispatcher* dispatcher = manager.GetTaskDispatcher();
    runner::TimeTick delay = runner::TimeTick::FromMilliseconds(100);
    dispatcher->PostDelayTask("test", TestFunc, delay);
  } else {
    manager.StopAllRunner();
  }
  m.unlock();
}

void CallbackTest(int t) {
  std::cout << __func__ << " - " << t << std::endl;
}

class TestClass {
 public:
  void Print(int a, int b) {
    std::cout << __func__ << " - " << a << " / " << b << std::endl;
  }

  TestClass* pointer = this;
};

int main() {
  std::cout << "Example Sequence Task Runner!\n";

  // runner::TaskRunner* test_runner = manager.CreateTaskRunner("test", runner::TaskRunner::Type::SEQUENCE);

  auto test1 = runner::Bind(&CallbackTest, 1);
  test1.Run();

  TestClass testclass;
  auto test2 = runner::Bind(&TestClass::Print, testclass.pointer, 1);
  test2.Run(2);

  auto test3 = runner::Bind([](int a) -> int {
    std::cout << __func__ << " - " << a << std::endl;
    return 1;
  }, 1);

  std::cout << "test3.Run() : " << test3.Run() << std::endl;

  // test_runner->PostTask(TestFunc);

  // manager.WaitForTerminateAllTaskRunner();
}
