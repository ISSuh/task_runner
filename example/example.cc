
/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#include <iostream>

#include "base/time.h"
#include "task_manager.h"
#include "task/task_runner.h"
#include "task/task_dispatcher.h"

#include "callback/callback.h"
#include "callback/bind.h"

class TestClass;
void TestFunc(int* count, TestClass* test_class);

runner::TaskManager manager;

class TestClass {
 public:
  TestClass(runner::TaskRunner* task_runner) 
    : task_runner_(task_runner) {}

  void TestMemeberFunc(int* count) {
    std::cout <<"TestClass::TestMemeberFunc - " << *count << std::endl;
    *count += 1;

    task_runner_->PostTask(runner::Bind([](int* count, TestClass* test_class) {
        std::cout << "TestLambda - " << *count << std::endl;
        *count += 1;

        manager.GetTaskDispatcher()->PostTask("example",
          runner::Bind(&TestFunc, count, test_class));
      }, count, this));
  }

 private:
  runner::TaskRunner* task_runner_;
};

void TestFunc(int* count, TestClass* test_class) {
  std::cout << "TestFunc - " << *count << std::endl;
  *count += 1;

  if (*count < 50) {
    runner::TaskDispatcher* dispatcher = manager.GetTaskDispatcher();
    dispatcher->PostTask("example",
      runner::Bind(&TestClass::TestMemeberFunc, test_class, count));
  } else {
    manager.StopAllRunner();
  }
}

int main() {
  std::cout << "Example Sequence Task Runner!\n";

  runner::TaskRunner* test_runner = manager.CreateTaskRunner("example", runner::TaskRunner::Type::SEQUENCE);

  TestClass test_class(test_runner);
  int count = 0;

  test_runner->PostTask(runner::Bind(&TestFunc, &count, &test_class));

  manager.WaitForTerminateAllTaskRunner();
}
