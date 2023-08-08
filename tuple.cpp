#include<iostream>
#include<tuple>
using namespace std;

void main()
{
	tuple<int, double, string> tp1 = make_tuple(1, 2.34, "5");
	cout << tuple_size<decltype(tp1)>::value << endl;
	cout << get<1>(tp1) << endl;//索引号从0开始,输出2.34

	int a = 1;
	double b = 2.34;
	string c = "5";
	auto tp2 = std::tie(a, b, c);
	cout << get<1>(tp2) << endl;
	b = 2.34444;
	cout << get<1>(tp2) << endl;
	double d = 0.0;
	std::tie(std::ignore, d, std::ignore) = tp2;//只为d赋值
	cout << "d:" << d << endl;
}