/*
c++11 关于异步操作提供了 future 相关的类，
主要有 std::future、std::promise 和 std::packaged_task
std::future 比 std::thread 高级些，std::future 作为异步结果的传输通道，通过 get()可以很方便的获取线程函数的返回值
std::promise 用来包装一个值，将数据和 future 绑定起来，
而 std::packaged_task 则用来包装一个调用对象，将函数和 future 绑定起来，方便异步调用。
而 std::future 是不可以复制的，如果需要复制放到容器中可以使用 std::shared_future

std::future 用于访问异步操作的结果，
std::promise 和 std::packaged_task 在 future 高一层，它们内部都有一个 future
promise包装的是一个值，packaged_task 包装的是一个函数
当需要获取线程中的某个值，可以使用 std::promise
当需要获取线程函数返回值，可以使用 std::packaged_task
*/

#include <iostream>
#include <future>
#include <functional>
#include <thread>

void func_1(std::future<int>& fut) {
	int x = fut.get();
	std::cout << "value: " << x << std::endl;
}

void func_2(std::promise<int>& pro) {
	std::this_thread::sleep_for(std::chrono::seconds(3));
	pro.set_value(100);
}

int Func(int in) {
	return in + 1;
}

int main() {
	std::promise<int> prom1;
	std::future<int> fut1 = prom1.get_future();
	std::thread t1(func_1, std::ref(fut1));
	prom1.set_value(144);
	t1.join();

	std::promise<int> prom2;
	std::future<int> fut2 = prom2.get_future();
	std::thread t2(func_2, std::ref(prom2));
	t2.detach();
	//需要 prom2 进行 set_value 后才会进行 get
	std::cout << "result: " << fut2.get() << std::endl;
	

	std::packaged_task<int(int)> task(Func);
	std::future<int> Fut = task.get_future();
	std::thread(std::move(task), 5).detach();
	

	std::cout << "result " << Fut.get() << std::endl;
	return 0;
}