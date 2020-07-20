#include <iostream>
using namespace std;

#if 0
// 如果按照值的方式返回，在返回的结果之前，先会创建一个临时的变量，然后销毁函数的栈帧最后返回该临时变量
// 临时变量：是编译器自己创建的，用户是无法知道变量的地址名字，
int add(int a, int b)
{
	return a+b;
}
int main()
{
	int b = 10;
	int& rb = b;
	int&& rrb = b+1;
	
	const int a = 10;
	//int&& ra = a;

	int&& radd = add(1,2);
	//常量不一定就是右值，因为可以取地址


	return 0;
}
#endif
#if 0
// 引用和右值引用的区别
void func1()
{
	int a = 10;
	
	// 普通类型可以引用左值
	int& ra1 = a;
	// a+1是右值，普通引用不能引用右值
	//int& ra2 = a+1;

	// const引用  
	const int& ra3 = a;
	const int& ra4 = a+1;
	/* const类型的引用不仅可以引用左值还可以引用右值 */
	
}

void func1()
{
	int a = 10;

	// 右值引用只能引用右值
	int&& ra1 = a + 1;

	// 有些情况下需要引用左值，但是右值引用不能直接引用左值，可以通过move函数将一个左值转化为右值
	int&& ra2 = move(a);

}

int main()
{

	return 0;
}
#endif

class String
{
public:
	String(const char* str = "")
	{
		if(nullptr == str)
			_str = "";
		_str = new char [strlen(str)+1];
		strcpy(_str,str);

	}

	String(const String& s)
		:_str(new char[strlen(s._str)+1])
	{
		strcpy(_str,s._str);
	}

	String& operator=(const String& s)
	{
		if(this!=&s)
		{
			delete [] _str;
			char* temp = new char[strlen(s._str)+1];
			strcpy(_str,s._str);
			_str = temp;
		}
	}
	String operator+(const String& s)
	{
		String strtemp;
		// 将当前对象和s中的字符串拼接
		int len1 = strlen(_str);
		int len2 = strlen(s._str);
		char* temp = new char[len1+len2+1];
		strcpy(temp,_str);
		strcat(temp,s._str);
		swap(strtemp._str,temp);
		delete [] temp;

		return strtemp;
	}

	// 移动构造
	/* 将一个对象中的资源转移到另一个对象中 */
	String(String&& s)
		:_str(s._str)
	{
		s._str = nullptr;
	}
	~String()
	{
		delete [] _str;
		_str = nullptr;
	}
	// 移动赋值
	// s1 = s2;
	// 如果s2是左值 调用普通的赋值
	// 如果s2是右值 调用移动赋值
	String& operator=(String&& s)
	{
		if(this!=&s)
		{
			swap(_str,s._str);
		}
		return *this;
	}
private:
	char* _str;
};
void func()
{
	String s1("hello");
	String s2(s1);
	
	String s3("world");
	s1 = s3;

	String s4;
	s4 = s2 + s3;

}

void func3()
{
	String s1("he");
	String&& s2 = move(s1);
	//  实际上将s2中的资源转移到s1中，s1中的_str指向了一个空


}
int main()
{
	return 0;
}