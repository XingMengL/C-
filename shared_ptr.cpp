#include <iostream>
using namespace std;
#if 0
#include <memory>
void TestSharedPtr()
{
	shared_ptr<int> sp1(new int);
	shared_ptr<int> sp2(sp1);

	*sp1 = 10;
	*sp2 = 20;

	cout<<sp1.use_count()<<endl;
	cout<<sp2.use_count()<<endl;

}
int main()
{
	TestSharedPtr();
	return 0;
}
#endif

namespace bite
{
	template<class T>
	class shared_ptr
	{
	public:
		shared_ptr(T* ptr = nullptr)
			:_ptr(ptr)
		{
			_count = 0;
			if(ptr)
			{
				_count = 1;
			}
		}

		~shared_ptr()
		{
			if(_ptr && 0 == --_count)
			{
				delete _ptr;
				_ptr = nullptr;
			}
		}

		T& operator*()
		{
			return *_ptr;
		}

		T* operator->()
		{
			return _ptr;
		}
		T* get()
		{
			return _ptr;
		}
		
		// ���ǳ���������ü���
		shared_ptr(const shared_ptr<T>& sp)
			:_ptr(sp._ptr)
		{
			if(_ptr)
			{
				++_count;
			}
		}
		shared_ptr<T>& operator=(const shared_ptr<T>& sp)
		{
			if(this!= &sp)
			{
				// *thsi = sp;
				// *thisʹ��sp�Ĺ�����Դ
				// *this���ܱ�����ӵ����Դ�������ȥ��sp����һ����Դ����ô����֮ǰ����Դ�Ͳ�����
				// *this������Դ�ļ��������� 1 ���� ���� 1
				// *thisӦ�ö�����Դ�ļ���-1
				if(_ptr&& 0 ==_count)
				{
					delete _ptr;
				}
				// ��*this��sp������Դ
				_ptr = sp._ptr;
				_count++;
			}
			return *this;
		}
		int use_count()const
		{
			return _count;
		}
	private:
		T* _ptr;
		static size_t _count;
	};
	template<class T>
	size_t shared_ptr<T>::_count = 0;
}

void Test()
{
	bite::shared_ptr<int> sp1(new int);
	bite::shared_ptr<int> sp2(sp1);
	
	*sp1 = 10;
	*sp2 = 20;

	cout<<sp1.use_count()<<endl;
	cout<<sp2.use_count()<<endl;
}
int main()
{
	Test();
	return 0;
}