
/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#include <iostream>
#include "base/time.h"

// #include "task/sequenced_task_runner.h"

// class Test {
//  public:
//   Test(const std::string& name, TaskRunner* task_runner)
//     : name_(name), task_runner_(task_runner) {}

//   void Start() {
//     std::cout << "Start - " << name_ << std::endl;

//     Callback task = runner::bind(&Test::Print1, this); 
//     task_runner_.PostTask(task);
//   }

//   void Print1() {
//     std::cout << "Print1 - " << name_ << std::endl;

//     Callback task = runner::bind(&Test::Print2, this); 
//     task_runner_.PostTask(task);
//   }

//   void Print2() {
//     std::cout << "Print2 - " << name_ << std::endl;

//     Callback task = runner::bind(&Test::Print1, this); 
//     task_runner_.PostDelayTask(task, 1);
//   }

//  private:
//   std::string name_;
//   TaskRunner* task_runner_;
// };

// int main() {
//   std::cout << "Hello World!\n";

//   runner::Runner runner;
//   TaskRunner* task_runner = runner.CreateTaskRuuner();

//   runner.ExcuteAllRunner();

//   Test test("test1");
//   test.Start();

//   runner.WiatForFinish();
// }

#include "base/time.h"
#include "task_manager.h"
#include "task/task_runner.h"

runner::TaskManager manager;
runner::TaskRunner* test_runner = nullptr;

void TestFunc() {
  static uint32_t count = 0;
  std::cout << __func__ << " - count : " << count << " / " << runner::Time::InNanosecond() << std::endl;

  ++count;

  if (count < 100) {
    runner::TimeTick delay = runner::TimeTick::FromMilliseconds(100);
    test_runner->PostDelayTask(TestFunc, delay);
  } else {
    manager.StopAllRunner();
  }
}

int main() {
  std::cout << "Hello World!\n";
  test_runner = manager.CreateTaskRunner("test", runner::TaskRunner::Type::SEQUENCE);
  test_runner->PostTask(TestFunc);

  manager.WaitForTerminateTaskRunner();
}