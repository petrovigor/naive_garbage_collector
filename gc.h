#pragma once

#include <list>

struct Alloc {
	const char*			fileName;
	const char*			fooName;
	int					    line;
	void*				    p;			   //heap pointer
	std::size_t	    size;		   //bytes allocated
  bool            arr;       //is array or not

	Alloc()
    : p(nullptr)
    , arr(false)
    , line(0)
    , size(0)
    , fileName(nullptr)
    , fooName(nullptr)
  { }

};

//singletone pattern class
class GarbageCollector final {
public:
	static GarbageCollector& Instance();

	GarbageCollector(const GarbageCollector&) = delete;
	GarbageCollector& operator=(const GarbageCollector&) = delete;
  
  GarbageCollector(GarbageCollector&&) = delete;
	GarbageCollector& operator=(GarbageCollector&&) = delete;

private:
	GarbageCollector();
	~GarbageCollector();

private:
	std::list<Alloc> allocs;

public:
	static void RegNew(void* ptr, std::size_t size, const char* fileName, const char* fooName, int line, bool arr) {
		Alloc a;

		a.p = ptr;
		a.fileName = fileName;
		a.fooName = fooName;
		a.line = line;
		a.size = size;
    a.arr = arr;

		GarbageCollector & gc = Instance();
		gc.allocs.push_back(a);
	}

	static void Unreg(void* ptr) {
		for(auto it = Instance().allocs.begin(); it != Instance().allocs.end(); ++it) {
			if(it->p == ptr) {
				GarbageCollector & gc = Instance();
				gc.allocs.erase(it);
				return;
			}
		}
	}

private:
	static void Dtor();
};

using GC = GarbageCollector;

//new & delete operators overloading
void* operator new(std::size_t size, const char* fileName, const char* fooName, int line, bool arr = false);
void* operator new[](std::size_t size, const char* fileName, const char* fooName, int line, bool arr = true);

void operator delete(void* p, bool __dummyVar);
void operator delete[](void* p, bool __dummyVar);

//defines:
//override global new/delete is bad practice, use only for debug

#define gc_new					      new(__FILE__, __func__, __LINE__)
#define gc_delete_single(p)		operator delete(p, false)
#define gc_delete_array(p)		operator delete[](p, false)
