/*
异步运行(std::async):async意思是将一个函数A移至另一线程中去运行。A可以是静态函数，全局函数，甚至类成员函数。
*/

/*
std::promise是一个模板类: 
template<typename R> class promise
其泛型参数R为std::promise对象保存的值的类型，R可以是void类型。
std::promise保存的值可被与之关联的std::future读取，读取操作可以发生在其它线程。
std::promise允许move语义(右值构造，右值赋值)，但不允许拷贝(拷贝构造、赋值)，std::future亦然。
std::promise和std::future合作共同实现了多线程间通信。
*/

/*
通过成员函数 set_value 可以设置 std::promise 中保存的值，该值最终会被与之关联的 std::future::get 读取到
set_value只能被调用一次，多次调用会抛出std::future_error异常。
*/

/*
一个std::promise实例只能与一个std::future关联共享状态，当在同一个std::promise上反复调用get_future会抛出future_error异常。
在std::promise构造时，std::promise对象会与共享状态关联起来，这个共享状态可以存储一个R类型的值或者一个由std::exception派生出来的异常值。
通过std::promise::get_future调用获得的std::future与std::promise共享相同的共享状态。
*/

/*
std::promise<void>是合法的。此时std::promise.set_value不接受任何参数，仅用于通知关联的std::future.get()解除阻塞。
*/

/*
std::promise支持定制线程退出时的行为：
    std::promise::set_value_at_thread_exit 线程退出时，std::future收到通过该函数设置的值。
    std::promise::set_exception_at_thread_exit 线程退出时，std::future则抛出该函数指定的异常。
*/

/*

*/

#include <iostream> 
#include <thread>   
#include <string>  
#include <future>//std::promise, std::future
#include <chrono> 
using namespace std::chrono;

std::mutex _mtx;
std::condition_variable _cv;
void task1(int a, int b, int& ret)
{
	int ret_a = a * a;
	int ret_b = b * 2;
	std::this_thread::sleep_for(seconds(3));
	std::unique_lock<std::mutex> lock(_mtx);
	ret = ret_a + ret_b;
	_cv.notify_one();
}

//子线程进行赋值，主线程进行获取
void task2(int a, int b, std::promise<int>& ret)
{
	int ret_a = a * a;
	int ret_b = b * 2;	
	ret.set_value(ret_a + ret_b);
}

//主线程中赋值，子线程中取值并赋值，主线程中再取值
void task3(int a,std::future<int>& b, std::promise<int>& ret)
{
	int ret_a = a * a;
	int ret_b = b.get() * 2;
	ret.set_value(ret_a + ret_b);
}

//主线程进行赋值，子线程进行获取,多个子线程
//shared_future可多次get
void task4(int a, std::shared_future<int> b, std::promise<int> &ret)
{
	int ret_a = a * a;
	int ret_b = b.get() * 2;
	ret.set_value(ret_a + ret_b);
}

void read(std::future<std::string>* future) {
    std::cout << future->get() << std::endl;// future会一直阻塞，直到有值到来
}

int main() {
	/*************************通过锁与条件变量进行赋值与获取*************************/
	int ret = 0;
	std::thread t1(task1, 1, 2, std::ref(ret));
	//do something else
	//
	//get the return value
	std::unique_lock<std::mutex>lock(_mtx);
	_cv.wait(lock);
	std::cout << "return value is " << ret << std::endl;
	t1.join();
	/*************************通过锁与条件变量进行赋值与获取*************************/

	/*************************子线程进行赋值，主线程进行获取*************************/
	std::promise<int> p1;
	std::future<int> f1 = p1.get_future();//将 future 与 promise 绑定
	std::thread t2(task2, 1, 2, std::ref(p1));
	//do something else
	//
	//get the return value
	std::cout << "return value is " << f1.get() << std::endl;//get()只能进行一次
	t2.join();
	/*************************子线程进行赋值，主线程进行获取*************************/

	/*************************主线程中赋值，子线程中取值并赋值，主线程中再取值*************************/
	{
		std::promise<int> p_in;  std::future<int> f_in = p_in.get_future();
		std::promise<int> p_out; std::future<int> f_out = p_out.get_future();
		std::thread t3(task3, 1, std::ref(f_in), std::ref(p_out));
		p_in.set_value(2);
		std::cout << "return value is " << f_out.get() << std::endl;//get()只能进行一次
		t3.join();
	}
	/*************************主线程中赋值，子线程中取值并赋值，主线程中再取值*************************/

	/*************************主线程进行赋值，多个子线程进行获取*************************/
	{
		std::promise<int> p_in; std::future<int> f_in = p_in.get_future();
		std::promise<int> p_out1; std::future<int> f_ret1 = p_out1.get_future();
		std::promise<int> p_out2; std::future<int> f_ret2 = p_out2.get_future();
		std::promise<int> p_out3; std::future<int> f_ret3 = p_out3.get_future();
		std::shared_future<int> s_f = f_in.share();

		//多个子线程获取值
		std::thread t4(task4, 2, s_f, std::ref(p_out1));
		std::thread t5(task4, 3, s_f, std::ref(p_out2));
		std::thread t6(task4, 4, s_f, std::ref(p_out3));
		//do something else
		//
		p_in.set_value(2);
		std::cout << "return value is " << f_ret1.get() << std::endl;
		std::cout << "return value is " << f_ret2.get() << std::endl;
		std::cout << "return value is " << f_ret3.get() << std::endl;
		t4.join();
		t5.join();
		t6.join();
	}
	/*************************主线程进行赋值，多个子线程进行获取*************************/

	/*************************主线程中赋值，子线程中取值*************************/
	{
		std::promise<std::string> promise;                      // promise 相当于生产者，许下承诺(生产)
		std::future<std::string> future = promise.get_future(); // future 相当于消费者, 右值构造，与生产者关联，未来得到生产的值(承诺)
		std::thread thread(read, &future);                      // 在另一线程中通过 future 来读取生产的值
		std::this_thread::sleep_for(seconds(1));                // 让read等一会儿
		promise.set_value("hello future");                      // 生产者完成生产
		thread.join();                                          // 控制台输: hello future
	}
	/*************************主线程中赋值，子线程中取值*************************/
	return 0;
}

