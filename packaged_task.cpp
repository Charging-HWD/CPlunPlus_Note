/*
packaged_task允许传入一个函数，并将函数计算的结果传递给std::future，包括函数运行时产生的异常。
std::packaged_task支持move，但不支持拷贝
std::packaged_task封装的函数的计算结果会通过与之联系的std::future::get获取(当然，可以在其它线程中异步获取)。
关联的std::future可以通过std::packaged_task::get_future获取到，get_future仅能调用一次，多次调用会触发std::future_error异常。
std::package_task除了可以通过可调用对象构造外，还支持缺省构造(无参构造)。但此时构造的对象不能直接使用，需通过右值赋值操作设置了可调用对象或函数后才可使用。
*/

/*
std::packaged_task::valid
该函数用于判断std::packaged_task对象是否是有效状态。
当通过缺省构造初始化时，由于其未设置任何可调用对象或函数，valid会返回false。
只有当std::packaged_task设置了有效的函数或可调用对象，valid才返回true。
*/

/*
std::packaged_task::operator()(ArgTypes...)
该函数会调用std::packaged_task对象所封装可调用对象R，但其函数原型与R稍有不同:void operator()(ArgTypes... );
operator()的返回值是void，即无返回值。
std::packaged_task的设计主要是用来进行异步调用，因此R(ArgTypes...)的计算结果是通过std::future::get来获取的。该函数会忠实地将R的计算结果反馈给std::future，即使R抛出异常(此时std::future::get也会抛出同样的异常)。
*/

/*
让std::packaged_task在线程退出时再将结果反馈给std::future
std::packaged_task::make_ready_at_thread_exit函数接收的参数与operator()(_ArgTypes...)一样，行为也一样。
只有一点差别，那就是不会将计算结果立刻反馈给std::future，而是在其执行时所在的线程结束后，std::future::get才会取得结果。
*/

/*
std::packaged_task::reset
与std::promise不一样， std::promise仅可以执行一次set_value或set_exception函数，但std::packagged_task可以执行多次.
其奥秘就是reset函数通过重新构造一个 promise 来达到多次调用的目的。
调用reset后，需要重新get_future，以便获取下次operator()执行的结果。
由于是重新构造了promise，因此reset操作并不会影响之前调用的make_ready_at_thread_exit结果，
即之前的定制的行为在线程退出时仍会发生。
*/

#include <iostream> 
#include <thread>  
#include <future>   // std::packaged_task, std::future

int sum(int a, int b) {
    int ret_a = a * a;
    int ret_b = b * 2;
    return ret_a + ret_b;
}

int main() {
    /*****************************************/
    auto a = std::bind(sum, 3, 4);
    int ret = a();
    std::cout << "return value is " << ret << std::endl;

    auto b = std::bind(sum, 3, 4);
    std::packaged_task<int()> t(b);
    t();
    std::cout << "return value is " << t.get_future().get() << std::endl;
    /*****************************************/
   
    /**************对任务进行包装******************/
    std::packaged_task<int(int, int)> task1(sum);
    task1(1, 2);
    std::cout << "return value is " << task1.get_future().get() << std::endl;
    /**************对任务进行包装******************/

    /**************对任务进行包装******************/
    std::packaged_task<int(int, int)> task2(sum);
    std::future<int> future = task2.get_future();
    std::thread t1(std::move(task2),1,2);// std::thread的第一个参数不止是函数，还可以是一个可调用对象，即支持operator()(Args...)操作
    t1.join();
    // 等待异步计算结果
    std::cout << "return value is " << future.get() << std::endl;
    /**************对任务进行包装******************/
 
    std::packaged_task<void()> task3;                            // 缺省构造、默认构造
    std::cout << std::boolalpha << task3.valid() << std::endl;   // false

    std::packaged_task<void()> task4(std::move(task3));          // 右值构造
    std::cout << std::boolalpha << task4.valid() << std::endl;   // false

    task3 = std::packaged_task<void()>([]() {});                 // 右值赋值, 可调用对象
    std::cout << std::boolalpha << task3.valid() << std::endl;   // true
    return 0;
}
