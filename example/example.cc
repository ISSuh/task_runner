
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
};

int main() {
  std::cout << "Hello World!\n";

  auto test1 = runner::Bind(&CallbackTest, 1);
  test1.Run();

  TestClass testclass;
  auto test2 = runner::Bind(&TestClass::Print, &testclass, 1, 2);
  test2.Run();

  // auto test3 = runner::Bind([](int a){
  //   std::cout << __func__ << " - " << a << std::endl;
  // }, 1);
  // test3.Run();

  // runner::TaskRunner* test_runner = manager.CreateTaskRunner("test", runner::TaskRunner::Type::CONCURRENT, 5);
  // test_runner->PostTask(TestFunc);

  // manager.WaitForTerminateAllTaskRunner();
}