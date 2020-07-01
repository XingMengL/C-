#include <iostream>
using namespace std;
#include <map>

#include <string>
#include<functional>
//插入
void test3()
{
	// 降序
	map<string,string,greater<string>> m;
	pair<string,string> array[3];
	array[0].first = "orgin";
	array[0].second = "橙子";
	array[1].first = "apple";
	array[1].second = "苹果";
	array[2].first = "banan";
	array[2].second = "香蕉";
	map<string,string> m(array,array+sizeof(array)/ sizeof(array[0]));

}
void test2()
{
	
	
	pair<string,string> array[3];
	array[0].first = "orgin";
	array[0].second = "橙子";
	array[1].first = "apple";
	array[1].second = "苹果";
	array[2].first = "banan";
	array[2].second = "香蕉";
	map<string,string> m(array,array+sizeof(array)/ sizeof(array[0]));

	//m.insert("name","名字");
	//调用pair构造函数创建一个匿名键值对象
	m.insert(pair<string,string>("name","名字"));
	m.insert(pair<string,string>("man","男"));
	m.insert(pair<string,string>("names","名字s"));

	//make_pair是一个全局函数，输入两个参数将这两个参数打包成一个键值对返回
	m.insert(make_pair("who","shui"));

	cout<<m.size()<<endl;
	for(auto& e: m)
		cout<<e.first<<"---"<<e.second<<endl;

	//插入失败：key不能重复，而apple已经存在
	m.insert(make_pair("apple","苹果"));

	//通过key来获取与key对应的value
	  // 注意：
	  // 1.operator[]:底层调用insert，其具有插入元素的功能
	  // 2.operator[key]:返回对应的value，其具有查找的功能
	// 原理：
	// 首先 将 <key , T> 构造成一个键值对，先map中进行insert
	// 如果key已经存在，插入失败，直接返回与key对应的value
	// 如果key不存在，插入成功，直接返回默认的value

	m.erase("apple");
	m.erase(m.begin(),m.end());
}
int main()
{
	map<string,string> m; // 构造一个空的map
	
	pair<string,string> array[3];
	array[0].first = "orgin";
	array[0].second = "橙子";
	array[1].first = "apple";
	array[1].second = "苹果";
	array[2].first = "banan";
	array[2].second = "香蕉";

	map<string,string> m2(array,array+sizeof(array)/ sizeof(array[0]));

	for(auto& e : m2)
		cout<<e.first<<"--------"<<e.second<<endl;
	cout<<endl;

	//采用迭代器遍历m2
	auto it = m2.begin();
	while(it!=m2.end())
	{
		cout<<it->first<<"----"<<it->second<<endl;
		it++;
	}

	test2();
	return 0;
}