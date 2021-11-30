# Task Runner

## 

## Callback & Binding

```c++
  int AddWithOffet(int offset, int a) {
    return offset + a;
  }

  // runner::Callback<ReturnType(UnBoundedArgument)> add = runner::Bind(Functor, BoundedArgument) 
  runner::Callback<int(int)> add = runner::Bind(&AddWithOffet, 1);

  // Callback.run(UnBoundedArgument);
  int result = add.Run(5);

```

```c++
  // Binding function
  void CallbackTest(int t) {
    std::cout << t << std::endl;
  }

  runner::Callback<void()> test1 = runner::Bind(&CallbackTest, 1);
  test1.Run();

  ...
  
  // Binding class memeber function
  class TestClass {
  public:
    void Print(int a, int b) {
      std::cout << a << " / " << b << std::endl;
    }
  };

  TestClass test_class;
  runner::Callback<void(int)> test2 = runner::Bind(&TestClass::Print, &test_class, 1);
  test2.Run(2);

  ...

  // Binding lambda function
  runner::Callback<void(int)> test3 = runner::Bind([](int a) -> int {
    std::cout << __func__ << " - " << a << std::endl;
    return 1;
  }, 1);
```

## Running Task

```c++
```

## Example

```c++

runner::TaskManager manager;

class TestClass {
 public:
  TestClass(runner::TaskRunner* task_runner) 
    : task_runner_(task_runner) {}

  void TestMemeberFunc(int* count) {
    std::cout <<"TestClass::TestMemeberFunc - " << *count << std::endl;
    *count += 1;

    // add task which is a lambda function
    task_runner_->PostTask(runner::Bind([](int* count, TestClass* test_class) {
        std::cout << "TestLambda - " << *count << std::endl;
        *count += 1;

        // get task dispatcher from task manager and add task which is a function
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

    // add task which is a class member function using dispatcher
    dispatcher->PostTask("example",
      runner::Bind(&TestClass::TestMemeberFunc, test_class, count));
  } else {
    // stop all task runner
    manager.StopAllRunner();
  }
}

int main() {
  std::cout << "Example Sequence Task Runner!\n";

  // create new sequenc task runner with label
  runner::TaskRunner* test_runner = manager.CreateTaskRunner("example", runner::TaskRunner::Type::SEQUENCE);

  TestClass test_class(test_runner);
  int count = 0;

  // add task which is a class member function
  test_runner->PostTask(runner::Bind(&TestFunc, &count, &test_class));

  // wait for task runner until finished
  manager.WaitForTerminateAllTaskRunner();
}
```