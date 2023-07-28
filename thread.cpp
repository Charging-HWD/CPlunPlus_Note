/*
并发的实现：
	1.多线程实现并发
		主要解决进程通信间问题
		NO.1 一个电脑上：管道、文件、消息队列、共享内存
		NO.2 不同电脑通过SOCKET网络通信实现
	2.单个进程，多个线程实现并发，即一个主线程多个子线程实现并发
		一个进程中的所有线程共享的内存空间:全局变量，指针引用
*/

/*
线程的多种创建方式：
	1.通过thread函数创建；
	2.通过类和对象；
	3.通过C++Lambada表达式；
	4.带参方式；
	5.带智能指针；
	6.类的成员函数。
*/

/*
线程停止：
一般情况下当线程函数执行完成后，线程“自然”停止。但在std::thread 析构时可能会造成线程异常终止。
如果想提前让线程结束，可以使用“共享变量”，线程定期地去检测该量，如果需要退出，则停止执行，退出线程函数。
使用“共享变量”需要注意，在多核、多CPU的情况下需要使用“原子”操作
*/

#include <iostream>
#include <thread>
#include <mutex>
#include <deque>

class Greet
{
	const char* owner = "Greet";
public:
	//STL仿函数
	void operator()()
	{
		std::cout << "类仿函数子线程启动" << std::endl;
	}
	void func(int& num)
	{
		std::cout << "有参类成员函数" << num << std::endl;
		std::cout << "ID:" << std::this_thread::get_id() << std::endl;
	}
	void SayHello(const char* name) {
		std::cout << "Hello " << name << " from " << this->owner << std::endl;
	}
};

//带参函数（是否传入引用参数）
void func1(int& a)
{
	std::cout << std::this_thread::get_id() << std::endl;
	for (int i = 0; i < a; i++)
	{
		std::cout << "Hello Word，" << "传入引用参数" << std::endl;
		std::this_thread::sleep_for(std::chrono::microseconds(50));
	}
}
void func2(const char* name)
{
	std::cout << std::this_thread::get_id() << std::endl;
	std::cout << "Hello " << name << std::endl;
}
void foo1() {
	std::cout << "Hello C++11" << std::endl;
}
void foo2() {
	for (int i = 0; i < 10; i++)
	{
		std::cout << "Hello C++11" << std::endl;
	}
	
}

void print(std::unique_ptr<int> ptr)
{
	std::cout << "子线程\t" << ptr.get() << std::endl;
}

/*
多线程需要对公共资源进行保护：
	1.通过 mutex.lock() 与 mutex.unlock() 进行上锁与解锁，效率低且不安全
	2.通过 std::lock_guard<std::mutex> 进行自动上、解锁（出作用域则自动析构）
	3.通过原子变量
*/
//int glob_variable = 0;			
std::atomic<int> glob_variable = 0;	//定义原子变量
std::mutex mtx1;
std::mutex mtx2;
void task()
{
	for (int i = 0; i < 99; i++)
	{
		//std::lock_guard<std::mutex> lock1(mtx1);
		//std::lock_guard<std::mutex> lock2(mtx2);
		glob_variable++;//原子变量，不需要锁管控
		glob_variable--;
	}
	std::cout << glob_variable << std::endl;
}

//线程暂停
void pausable() 
{
	// sleep 500毫秒
	std::this_thread::sleep_for(std::chrono:: milliseconds(500));
	// sleep 到指定时间点
	std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(500));
}



//1.Man();//MM 无名对象+() 等价于 operator()()
//2.Lambada表达式,返回函数指针 [](){};
//3.std::ref 用于包装 引用传递值
//4.std::move 
int thread_test()
{
	/************对象创建线程************/
	Greet greet;
	std::thread test1(greet);
	test1.join();
	std::thread test2((Greet()));//Man 无名对象+() 等价于 operator()()
	test2.join();
	/************对象创建线程************/

	/************Lambada表达式创建线程************/
	std::thread test3(
		[]()
		{
			std::cout << "Lambada子线程启动" << std::endl; 
		});
	test3.join();
	/************Lambada表达式创建线程************/

	/************函数创建线程************/
	int num = 10;
	std::thread test4(func1, std::ref(num));//func不使用引用参数则不需要ref
	test4.join();
	std::thread test5(func2, "C++11");
	test5.join();
	std::thread test6(foo1);
	test6.join();
	/************函数创建线程************/

	/************智能指针创建线程************/
	//int* p = new int(1000);
	std::unique_ptr<int> ptr(new int(1000));
	std::thread test7(print,move(ptr));
	test7.join();
	/************智能指针创建线程************/

	/************带参类成员函数创建线程************/
	int count = 1007;
	std::thread test8(&Greet::func, greet, std::ref(count));
	test8.join();
	std::thread test9(&Greet::SayHello, &greet, "C++11");
	test9.join();
	/************带参类成员函数创建线程************/

	/************访问公共资源************/
	std::thread task1(task);
	task1.detach();
	std::thread task2(task);
	task2.detach();
	/************访问公共资源************/

	/************线程暂停************/
	std::thread test10(pausable);
	test10.join();
	/************线程暂停************/

	/************线程传递************/
	std::thread test11(foo2);
	std::thread test12(std::move(test11));
	test12.join();
	/*
	使用std::move函数将线程对象test11的所有权转移到了test12，实现了线程对象的传递。
	一旦线程对象被移动，原始的线程对象就不能再使用，否则会导致未定义行为。
	*/
	/************线程传递************/

	std::cout << "主线程启动," << "ID:" << std::this_thread::get_id() << std::endl;

	return 0;
}
