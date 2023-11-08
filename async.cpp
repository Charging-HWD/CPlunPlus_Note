/*
async 是比 future，packaged_task，promise 更高级的东西
它是基于任务的异步操作，通过 async 可以直接创建异步的任务
返回的结果会保存在 future 中，不需要像 packaged_task 和 promise 那么麻烦

async 具体语法如下：
async(std::launch::async | std::launch::deferred, func, args...);
第一个参数是创建策略：
std::launch::async 表示任务执行在另一线程
std::launch::deferred 表示延迟执行任务，调用 get 或者 wait 时才会执行，不会创建线程，惰性执行在当前线程。
如果不明确指定创建策略，以上两个都不是 async 的默认策略，而是未定义，它是一个基于任务的程序设计，内部有一个调度
器(线程池)，会根据实际情况决定采用哪种策略。

若从 std::async 获得的 std::future 未被移动或绑定到引用，则在完整表达式结尾
std::future 的析构函数将阻塞直至异步计算完成，实际上相当于同步操作：
std::async(std::launch::async, []{ f(); }); // 临时量的析构函数等待 f()
std::async(std::launch::async, []{ g(); });
*/

#include <functional>
#include <future>
#include <iostream>
#include <thread>

int func(int in) 
{ 
	std::this_thread::sleep_for(std::chrono::seconds(5));
	std::cout <<"func return " << std::endl; 
	return in + 1;
}

int main() {
	auto res = std::async(func, 5);
	std::cout << "do thing" << std::endl;
	std::cout << res.get() << std::endl; // 阻塞直到函数返回
	return 0;
}