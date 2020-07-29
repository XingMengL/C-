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
// ʹ��static���ε����Ա������Ϊ�����������ǲ��еģ���Ϊ��̬��Ա������������������
// ������Ӧ������Դ�ĸ�����ͬ
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
#endif


#if 0
// ʹ��ָ�������Ϊ����������

/*
	ȱ�ݣ�
	1.���е���Դ������delete��ʽ ---����ɾ����
	2.��������Դ�ǹ���ģ�����Ҫ��֤������Դ���̰߳�ȫ
*/
#include <mutex>
/*
	ͨ��mutex��֤shared_ptr���̰߳�ȫ��
	ע�⣺ mutex��֤��shared_prt����İ�ȫ�ԣ�ȷ������̹߳�����Դʱ�������ü����Ĳ����ǰ�ȫ��
	ͨ��mutex���ܱ�֤shared_ptr������Դ�İ�ȫ�� ---���û�����Դ�����İ�ȫ��
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
		
		// ���ǳ���������ü���
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
				// *thisʹ��sp�Ĺ�����Դ
				// *this���ܱ�����ӵ����Դ�������ȥ��sp����һ����Դ����ô����֮ǰ����Դ�Ͳ�����
				// *this������Դ�ļ��������� 1 ���� ���� 1
				// *thisӦ�ö�����Դ�ļ���-1
				_func();
				// ��*this��sp������Դ
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

		int SubRef()  // -- ���̰߳�ȫ
		{
			_pmutex->lock();
			--(*_pcount);
			_pmutex->unlock();
			
			return *_pcount;
		}

		void AddRef() // ++ ���̰߳�ȫ
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
shared_ptr�ŵ㣺����ʵ�ֶ���֮����Դ����
shared_ptrȱ�ݣ�����ѭ������
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
