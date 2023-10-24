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

int main()
{
	std::vector<int> number{ 1,2,3,4,5 };

	/*
	使用 std::bind 将 "Element:" 字符串绑定到函数对象的第一个参数，使用 std::placeholders::_1 表示将来传入的第二个参数。
	通过调用 std::for_each 算法遍历 numbers 容器，并对每个元素应用该函数对象。
	*/
	std::for_each(number.begin(), number.end(), std::bind(function1, "Element:", std::placeholders::_1));

	/*
	使用 std::bind 将 "Element:" 字符串绑定到函数对象的第二个参数，使用 std::placeholders::_1 表示将来传入的第一个参数。
	通过调用 std::for_each 算法遍历 numbers 容器，并对每个元素应用该函数对象。
	*/
	std::for_each(number.begin(), number.end(), std::bind(function2, std::placeholders::_1, "Element:"));

	auto function = std::bind(function1, std::placeholders::_1, std::placeholders::_2);
	function("Element:", 6);
	return 0;
}