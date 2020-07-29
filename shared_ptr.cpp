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

#if 0
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
#endif

#if 0
void TestFunc1()
{
	FILE* pf = fopen("11111111.txt","rb");
	if(pf == nullptr)
	{
		throw 1;
	}

	fclose(pf);
}

void TestFunc()
{
	int* p = new int[10];

		delete p;
}

int main()
{

return 0;
}
#endif

/*
	RAII: 资源获取即初始化
	在构造函数中接收资源，在析构函数中释放资源。
	即：将资源委托给一个对象来控制
*/

#if 0
template<class T>
class smart_ptr
{
public:
	// RAII的思想来管理资源，好处：该资源不用手动释放
	/*
		缺陷：当使用一个smart_ptr对象构造一个新的对象时或者 两个对象在赋值时
		而该类的拷贝构造函数和赋值运算符重载没有实现，那么编译器按照浅拷贝的方式自己生成
	*/
	smart_ptr(T* ptr = nullptr)
		:_ptr(ptr)
	{}
	~smart_ptr()
	{
		if(_ptr)
		{
			delete _ptr;
		}
	}

	T* operator->()
	{
		return _ptr;
	}

	T& operator*()
	{
		return *_ptr;
	}


private:
	T* _ptr;
};

bool TestFunc0()
{
	//xxxx

	// 遇到非法错误
	return false;

	//xxxx
	return true;
}

void TestFunc1()
{
	FILE* pf = fopen("1111111.txt", "rb");
	if (pf == nullptr)
	{
		throw 1;
	}

	// 对文件进行响应的操作

	fclose(pf);
}


// ...

void TestFunc()
{
	smart_ptr<int> sp(new int);
	*sp = 10;

	// xxx

	if (!TestFunc0())
	{
		return;
	}

	try
	{
		TestFunc1();
	}
	catch (...)
	{
		throw;
	}

	// 像该种中途退出的情况比较多，每个位置都需要对空间来进行释放，否则就会造成内存泄漏
	// 因此在写代码时，要非常非常的谨慎

	// 该出调用了一个第三方的函数
}

// 复杂的问题
void TestSmartPtr()
{
	smart_ptr<int> sp1(new int);
	*sp1 = 100;
	smart_ptr<int> sp2(sp1);

	smart_ptr<int>sp3;
	sp3 = sp1;
}
int main()
{
	TestFunc();
	return 0;
}
#endif


#if 0
/*
	不能采用深拷贝的方式来解决浅拷贝的问题：因为智能指针不负责资源的申请。
*/
namespace bite
{
	/*
		缺陷：不能同时访问一份资源
		会造成野指针，很严重
	*/
	template<class T>
	class auto_ptr
	{
	public:
		auto_ptr(T* ptr)
			:_ptr(ptr)
			,_owner(false)
		{
			if(_ptr)
				_owner = true;
		}
		~auto_ptr()
		{
			if(_ptr && _owner)
			{
				delete _ptr;
				_owner = false;
			}
		}

		T* operator->()
		{
			return _ptr;
		}
		T& operator*()
		{
			return *_ptr;
		}


		// 解决浅拷贝问题：采用资源的转移方式来解决问题
		// 让一个对象拥有资源
		auto_ptr(auto_ptr<T>& ap)
			:_ptr(ap._ptr)
			,_owner(ap._owner)
		{
			ap._owner = false;
		}

		auto_ptr<T>& operator=(auto_ptr<T>& ap)
		{
			if(this!=&ap)
			{
				// 当前对象本身管理资源了，那么将资源释放
				if(_ptr && _owner)
					delete _ptr;

				_ptr = ap._ptr;
				_owner = ap._owner;
				ap._owner = false;
			}
			return *this;
		}
	private:
		T* _ptr;
		bool _owner; // false 当前对象没有权力释放资源
	};
}
int main()
{
	
	return 0;
}
#endif

// unique——ptr 对象资源独占，不在共享
template<class T>
class unique_ptr
{
public:
	unique_ptr(T* ptr = nullptr)
		:_ptr(ptr)
	{}
	~unique_ptr()
	{
		if(_ptr)
		{
			delete _ptr;
		}
	}

	T* operator->()
	{
		return _ptr;
	}
	T& operator*()
	{
		return *_ptr;
	}
	// 解决浅拷贝问题
	unique_ptr(const unique_ptr<T>&);
	unique_ptr<T>& operator=(const unique_ptr<T>&);

private:
	T* _ptr;
};

int main()
{
	return 0;
}