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
		
		// 解决浅拷贝：引用计数
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
				// *this使用sp的共享资源
				// *this可能本来就拥有资源，如果再去和sp共享一份资源，那么它的之前的资源就不用了
				// *this管理资源的计数可能是 1 或者 大于 1
				// *this应该对其资源的计数-1
				if(_ptr&& 0 ==_count)
				{
					delete _ptr;
				}
				// 让*this和sp共享资源
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