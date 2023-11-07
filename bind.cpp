/*
使用 std::bind 可以将可调用对象和参数一起绑定，绑定后的结果使用 std::function 进行保存，并延迟调用到任何我们需要的时候。
std::bind 通常有两大作用：
将可调用对象与参数一起绑定为另一个 std::function 供调用
将 n 元可调用对象转成 m(m < n)元可调用对象，绑定一部分参数，这里需要使用 std::placeholders
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

/*
错误示例：
void function(int i,const char* str)
{
	std::cout << str << i << std::endl;
}
void main()
{
	std::vector<int> number{ 1,2,3,4,5 };
	std::for_each(number.begin(), number.end(), std::bind(function, "Element:"));
}
函数说明：使用 std::bind 将 function 函数绑定到 "Element:" 字符串上，以创建一个新的函数对象。
然后使用 std::for_each 算法遍历 number 容器，并对每个元素应用该函数对象。

std::bind 默认将绑定参数传递给目标函数的尾部参数，
因此在这种情况下，"Element:" 字符串将被绑定到 function 函数的第二个参数上。
但是根据您的 function 函数定义，i 是第一个参数，而 str 是第二个参数。
因此，需要交换参数的顺序，以便正确绑定参数。
*/

void function1(const char* str, int i)
{
	std::cout << str << i << std::endl;
}

void function2(int i, const char* str)
{
	std::cout << str << i << std::endl;
}

void f(int n1, int n2, int n3, const int& n4, int n5) {
	std::cout << n1 << ' ' << n2 << ' ' << n3 << ' ' << n4 << ' ' << n5 << std::endl;
}

int g(int n1) { return n1; }

struct Foo {
	void print_sum(int n1, int n2) { std::cout << n1 + n2 << std::endl; }
	int data = 10;
};

int main()
{
	std::vector<int> number{ 1,2,3,4,5 };

	/*
	使用 std::bind 将 "Element:" 字符串绑定到函数对象的第一个参数，使用 std::placeholders::_1 表示将来传入的第二个参数。
	通过调用 std::for_each 算法遍历 numbers 容器，并对每个元素应用该函数对象。
	*/
	std::for_each(number.begin(), number.end(), std::bind(function1, "Element:", std::placeholders::_1));
	std::cout << "***********************************\n" << std::endl;
	/*
	使用 std::bind 将 "Element:" 字符串绑定到函数对象的第二个参数，使用 std::placeholders::_1 表示将来传入的第一个参数。
	通过调用 std::for_each 算法遍历 numbers 容器，并对每个元素应用该函数对象。
	*/
	std::for_each(number.begin(), number.end(), std::bind(function2, std::placeholders::_1, "Element:"));
	std::cout << "***********************************\n" << std::endl;

	auto function = std::bind(function1, std::placeholders::_1, std::placeholders::_2);
	function("Element:", 6);
	std::cout << "***********************************\n" << std::endl;
	
	using namespace std::placeholders;
	int n = 7;
	// std::cref(n) 为 const int&
	auto f1 = std::bind(f, _2, 42, _1, std::cref(n), n);
	n = 10;

	// 1 为 _1 所绑定, 2 为 _2所绑定,不使用 1001
	f1(1, 2, 1001);// 进入到 f(2, 42, 1, n, 7) 的调用
	std::cout << "***********************************\n" << std::endl;

	// 嵌套 bind 子表达式共享占位符
	auto f2 = std::bind(f, _3, std::bind(g, _3), _3, 4, 5);
	f2(10, 11, 12); // 进入到 f(12, g(12), 12, 4, 5);
	std::cout << "***********************************\n" << std::endl;

	// 绑定指向成员函数指针
	Foo foo;
	auto f3 = std::bind(&Foo::print_sum, &foo, 95, _1);
	f3(5);
	std::cout << "***********************************\n" << std::endl;

	// 绑定指向数据成员指针
	auto f4 = std::bind(&Foo::data, _1);
	std::cout << f4(foo) << std::endl;
	std::cout << "***********************************\n" << std::endl;

	// 智能指针亦能用于调用被引用对象的成员
	std::cout << f4(std::make_shared<Foo>(foo)) << std::endl;
	std::cout << "***********************************\n" << std::endl;
	return 0;
}
