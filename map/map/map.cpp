#include <iostream>
using namespace std;
#include <map>

#include <string>
#include<functional>
//����
void test3()
{
	// ����
	map<string,string,greater<string>> m;
	pair<string,string> array[3];
	array[0].first = "orgin";
	array[0].second = "����";
	array[1].first = "apple";
	array[1].second = "ƻ��";
	array[2].first = "banan";
	array[2].second = "�㽶";
	map<string,string> m(array,array+sizeof(array)/ sizeof(array[0]));

}
void test2()
{
	
	
	pair<string,string> array[3];
	array[0].first = "orgin";
	array[0].second = "����";
	array[1].first = "apple";
	array[1].second = "ƻ��";
	array[2].first = "banan";
	array[2].second = "�㽶";
	map<string,string> m(array,array+sizeof(array)/ sizeof(array[0]));

	//m.insert("name","����");
	//����pair���캯������һ��������ֵ����
	m.insert(pair<string,string>("name","����"));
	m.insert(pair<string,string>("man","��"));
	m.insert(pair<string,string>("names","����s"));

	//make_pair��һ��ȫ�ֺ����������������������������������һ����ֵ�Է���
	m.insert(make_pair("who","shui"));

	cout<<m.size()<<endl;
	for(auto& e: m)
		cout<<e.first<<"---"<<e.second<<endl;

	//����ʧ�ܣ�key�����ظ�����apple�Ѿ�����
	m.insert(make_pair("apple","ƻ��"));

	//ͨ��key����ȡ��key��Ӧ��value
	  // ע�⣺
	  // 1.operator[]:�ײ����insert������в���Ԫ�صĹ���
	  // 2.operator[key]:���ض�Ӧ��value������в��ҵĹ���
	// ԭ��
	// ���� �� <key , T> �����һ����ֵ�ԣ���map�н���insert
	// ���key�Ѿ����ڣ�����ʧ�ܣ�ֱ�ӷ�����key��Ӧ��value
	// ���key�����ڣ�����ɹ���ֱ�ӷ���Ĭ�ϵ�value

	m.erase("apple");
	m.erase(m.begin(),m.end());
}
int main()
{
	map<string,string> m; // ����һ���յ�map
	
	pair<string,string> array[3];
	array[0].first = "orgin";
	array[0].second = "����";
	array[1].first = "apple";
	array[1].second = "ƻ��";
	array[2].first = "banan";
	array[2].second = "�㽶";

	map<string,string> m2(array,array+sizeof(array)/ sizeof(array[0]));

	for(auto& e : m2)
		cout<<e.first<<"--------"<<e.second<<endl;
	cout<<endl;

	//���õ���������m2
	auto it = m2.begin();
	while(it!=m2.end())
	{
		cout<<it->first<<"----"<<it->second<<endl;
		it++;
	}

	test2();
	return 0;
}