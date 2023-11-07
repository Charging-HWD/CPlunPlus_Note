/*
默认情况下，std::map 使用 std::less 数对象进行键的比较，
这意味着它会按照键的升序进行排序

如果需要按照其他方式对其进行排序，可以提供自定义的比较函数对象，
比较函数对象应该接受两个参数，分别是要比较的键的类型，返回值为 bool 类型
如果第一个参数小于第二个参数，则返回 true,否则返回 false;
*/

#include <iostream>
#include <map>
#include <string>

// 自定义比较函数对象，按照键的长度进行排序
// 如果后面的键值长度等于map中键值的长度，则不会添加该键值
struct CompareByLength
{
    bool operator()(const std::string& str1, const std::string& str2) const
    {
        return str1.length() < str2.length();
    }
};

// 自定义比较函数对象，按照键的大小进行排序
struct CompareBySize {
    bool operator()(const std::string& a, const std::string& b) const {
        return a < b;
    }
};

int main()
{
    // 创建一个 std::map 对象，并使用自定义比较函数对象进行排序
    // 比较长度 CompareByLength,myMap 则不会添加 orange 与 grape
    // 比较大小 CompareBySize,myMap 则会添加全部键值
    std::map<std::string, int, CompareByLength> myMap;
    myMap.insert(std::make_pair("apple", 1));
    myMap.insert(std::make_pair("banana", 2));
    myMap.insert(std::make_pair("orange", 3));
    myMap.insert(std::make_pair("grape", 4));
    myMap.insert(std::make_pair("strawberry", 5));

    for (auto& iter : myMap)
    {
        std::cout << iter.first << ":" << iter.second << std::endl;
    }

    system("pause");
    return 0;
}