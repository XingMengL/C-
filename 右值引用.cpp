#include <iostream>
using namespace std;

#if 0
// �������ֵ�ķ�ʽ���أ��ڷ��صĽ��֮ǰ���Ȼᴴ��һ����ʱ�ı�����Ȼ�����ٺ�����ջ֡��󷵻ظ���ʱ����
// ��ʱ�������Ǳ������Լ������ģ��û����޷�֪�������ĵ�ַ���֣�
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
	//������һ��������ֵ����Ϊ����ȡ��ַ


	return 0;
}
#endif
#if 0
// ���ú���ֵ���õ�����
void func1()
{
	int a = 10;
	
	// ��ͨ���Ϳ���������ֵ
	int& ra1 = a;
	// a+1����ֵ����ͨ���ò���������ֵ
	//int& ra2 = a+1;

	// const����  
	const int& ra3 = a;
	const int& ra4 = a+1;
	/* const���͵����ò�������������ֵ������������ֵ */
	
}

void func1()
{
	int a = 10;

	// ��ֵ����ֻ��������ֵ
	int&& ra1 = a + 1;

	// ��Щ�������Ҫ������ֵ��������ֵ���ò���ֱ��������ֵ������ͨ��move������һ����ֵת��Ϊ��ֵ
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
		// ����ǰ�����s�е��ַ���ƴ��
		int len1 = strlen(_str);
		int len2 = strlen(s._str);
		char* temp = new char[len1+len2+1];
		strcpy(temp,_str);
		strcat(temp,s._str);
		swap(strtemp._str,temp);
		delete [] temp;

		return strtemp;
	}

	// �ƶ�����
	/* ��һ�������е���Դת�Ƶ���һ�������� */
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
	// �ƶ���ֵ
	// s1 = s2;
	// ���s2����ֵ ������ͨ�ĸ�ֵ
	// ���s2����ֵ �����ƶ���ֵ
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
	//  ʵ���Ͻ�s2�е���Դת�Ƶ�s1�У�s1�е�_strָ����һ����


}
int main()
{
	return 0;
}