#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>


/*
����ʾ����
void function(int i,const char* str)
{
	std::cout << str << i << std::endl;
}
void main()
{
	std::vector<int> number{ 1,2,3,4,5 };
	std::for_each(number.begin(), number.end(), std::bind(function, "Element:"));
}
����˵����ʹ�� std::bind �� function �����󶨵� "Element:" �ַ����ϣ��Դ���һ���µĺ�������
Ȼ��ʹ�� std::for_each �㷨���� number ����������ÿ��Ԫ��Ӧ�øú�������

std::bind Ĭ�Ͻ��󶨲������ݸ�Ŀ�꺯����β��������
�������������£�"Element:" �ַ��������󶨵� function �����ĵڶ��������ϡ�
���Ǹ������� function �������壬i �ǵ�һ���������� str �ǵڶ���������
��ˣ���Ҫ����������˳���Ա���ȷ�󶨲�����
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
	ʹ�� std::bind �� "Element:" �ַ����󶨵���������ĵ�һ��������ʹ�� std::placeholders::_1 ��ʾ��������ĵڶ���������
	ͨ������ std::for_each �㷨���� numbers ����������ÿ��Ԫ��Ӧ�øú�������
	*/
	std::for_each(number.begin(), number.end(), std::bind(function1, "Element:", std::placeholders::_1));

	/*
	ʹ�� std::bind �� "Element:" �ַ����󶨵���������ĵڶ���������ʹ�� std::placeholders::_1 ��ʾ��������ĵ�һ��������
	ͨ������ std::for_each �㷨���� numbers ����������ÿ��Ԫ��Ӧ�øú�������
	*/
	std::for_each(number.begin(), number.end(), std::bind(function2, std::placeholders::_1, "Element:"));

	auto function = std::bind(function1, std::placeholders::_1, std::placeholders::_2);
	function("Element:", 6);
	return 0;
}