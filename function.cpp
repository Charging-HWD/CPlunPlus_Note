/*
满足以下条件之一就可称为可调用对象
1.是一个函数指针
2.是一个具有 operator() 成员函数的类对象（仿函数），lambada 表达式
3.是一个可被转换为函数指针的类对象
4.是一个类成员（函数）指针
5.bind 表达式或其他函数对象

std::function 是这种可调用对象的封装器，可以把 std::function 看做一个函数对象，用于表示函数这个抽象概念。
std::function 的实例可以存储、复制和调用任何可调用对象，存储的可调用对象称为 std::function 的目标。
若 std::function 不含目标，则称它为空，调用空的 std::function 的目标会抛出异常 std::bad_function_call
*/

/*
当给 std::function 填入合适的参数表和返回值后，它就变成了可以容纳所有这一类调用方式的函数封装器。
std::function 还可以用作回调函数，如果在 C++里如果需要使用回调那就一定要使用 std::function
*/

#include <iostream>
#include <functional>

//function 的对象 f 的参数是 int ，返回是 void
std::function<void(int)> f;

struct Foo 
{
	Foo(int num) :num_(num) {}
	void printf_add(int i)const { std::cout << num_ + i << std::endl; }
	int num_;
};

void printf_num(int i)
{
	std::cout << i << std::endl;
}

struct PrintfNum
{
	void operator()(int i)const { std::cout << i << std::endl; }
};

int main()
{
	//存储自由函数(参数是 int ，返回是 void)
	std::function<void(int)> f_display_1 = printf_num;
	f_display_1(10);

	//存储 lambada 函数()
	std::function<void()>f_display_2 = []() { printf_num(20); };
	f_display_2();

	//存储到 std::bind 调用的结果(将函数与入参绑定成一个函数)
	std::function<void()> f_display_3 = std::bind(printf_num, 30);
	f_display_3();

	{
		//存储到成员函数的调用
		//f_add_display_1 是一个 function 对象，它保存了一个类成员函数指针 &Foo::printf_add
		std::function<void(const Foo&, int)>f_add_display = &Foo::printf_add;

		//创建一个 Foo 类的对象 foo
		const Foo foo(10);

		// 调用成员函数时，需要传入对象作为第一个参数
		f_add_display(foo, 10);

		//存储到数据成员访问器的调用
		std::function<int(Foo const&)> f_num = &Foo::num_;
		std::cout << "num_:" << f_num(foo) << std::endl;
	}

	//存储到成员函数及对象的调用
	using std::placeholders::_1;
	std::function<void(int)>f_add_display_1 = std::bind(&Foo::printf_add, foo, _1);
	f_add_display_1(10);

	//存储到成员函数和对象指针的调用
	std::function<void(int)>f_add_display_2 = std::bind(&Foo::printf_add, &foo, _1);
	f_add_display_2(20);

	//存储到函数对象的调用
	std::function<void(int)>f_display_obj = PrintfNum();
	f_display_obj(30);
}