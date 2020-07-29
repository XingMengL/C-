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

#if 0
// 使用static整形的类成员变量作为计数的类型是不行的，因为静态成员变量是所有类对象共享的
// 而计数应该与资源的个数相同
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
#endif


#if 0
// 使用指针变量作为计数的类型

/*
	缺陷：
	1.所有的资源都采用delete方式 ---定制删除器
	2.计数和资源是共享的：必须要保证共享资源的线程安全
*/
#include <mutex>
/*
	通过mutex保证shared_ptr的线程安全性
	注意： mutex保证的shared_prt自身的安全性，确保多个线程共享资源时，对引用计数的操作是安全的
	通过mutex不能保证shared_ptr管理资源的安全性 ---即用户对资源操作的安全性
*/
namespace bite
{
	template<class T>
	class DeleteDef
	{
	public:
		void operator()(T*& p)
		{
			if(p)
			{
				delete p;
				p = nullptr;
			}
		}
	};

	template<class T>
	class Free
	{
	public:
		void operator()(T*& p)
		{
			if(p)
			{
				free(p);
				p = nullptr;
			}
		}
	};

	template<class T>
	class FClose
	{
	public:
		void operator()(FILE*& p)
		{
			if(p)
			{
				fclose(p);
				p = nullptr;
			}
		}
	};
	template<class T, class DX = DeleteDef<T>>
	class shared_ptr
	{
	public:
		shared_ptr(T* ptr = nullptr)
			:_ptr(ptr)
			,_pcount(nullptr)
			,_pmutex(nullptr)
		{
			
			if(ptr)
			{
				_pcount = new int(1);
				_pmutex = new mutex;
			}
		}

		~shared_ptr()
		{
			_func();
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
			,_pcount(sp._pcount)
		{
			if(_ptr)
			{
				AddRef();
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
				_func();
				// 让*this和sp共享资源
				_ptr = sp._ptr;
				_pcount = sp._pcount;
				if(_ptr)
				{
					AddRef();
				}
			}
			return *this;
		}
		int use_count()const
		{
			if(_pcount == nullptr)
				throw bad_exception();

			return *_pcount;
		}
	private:
		void _func()
		{
			if(_ptr && 0 == SubRef())
			{
				DX() (_ptr);
				_ptr = nullptr;

				delete _pcount;
				_pcount = nullptr;
			}
		}

		int SubRef()  // -- 是线程安全
		{
			_pmutex->lock();
			--(*_pcount);
			_pmutex->unlock();
			
			return *_pcount;
		}

		void AddRef() // ++ 是线程安全
		{
			_pmutex->lock();
			++(*_pcount);
			_pmutex->unlock();
		}
	private:
		T* _ptr;
		T* _pcount;
		mutex* _pmutex;
	};
}

void Test1()
{
	bite::shared_ptr<int> sp1(new int);
	bite::shared_ptr<int> sp2(sp1);
	
	*sp1 = 10;
	*sp2 = 20;

	cout<<sp1.use_count()<<endl;
	cout<<sp2.use_count()<<endl;

	bite::shared_ptr<int> sp3(new int);
	cout<<sp3.use_count()<<endl;

}

int main()
{
	Test1();
	return 0;
}
#endif

/*
shared_ptr优点：可以实现对象之间资源共享
shared_ptr缺陷：存在循环引用
*/
#include <memory>
struct ListNode
{
	shared_ptr<ListNode> next;
	shared_ptr<ListNode> prev;
	int _data;

	ListNode(int data = 0)
		:next(nullptr)
		,prev(nullptr)
		,_data(data)
	{}

	~ListNode()
	{
		cout<<"~ListNode :"<<this<<endl;
	}
};
void Test()
{
	shared_ptr<ListNode> sp1(new ListNode(10));
	shared_ptr<ListNode> sp2(new ListNode(20));

	cout<<sp1.use_count<<endl;
	cout<<sp2.use_count<<endl;

	sp1->next = sp2;
	sp2->prev = sp1;
}
int main()
{
	Test();
	return 0;
}
