#include "gc.h"
#include <iostream> //possible log implementation

GarbageCollector::GarbageCollector() { }

GarbageCollector::~GarbageCollector() {
	Dtor();
}

GarbageCollector& GarbageCollector::Instance() {
	static GarbageCollector gc;
	return gc;
}

void GarbageCollector::Dtor() {
	GarbageCollector& gc = Instance();

	if(gc.allocs.size() > 0) {

    //possible log implementation
		for(auto it = gc.allocs.begin(); it != gc.allocs.end(); ++it) {
      std::cerr <<  "potential leak prevented at: "  << it->fileName <<
                    " funcname: " << it->fooName  <<
                    " line: "     << it->line     <<
                    " bytes: "    << it->size     <<
                    " is array: " << std::boolalpha << it->arr << std::dec << std::endl;
		}

		for(auto& it : gc.allocs) {
      it.arr? delete[] it.p : delete it.p;
		}

	}
}

void* operator new(std::size_t size, const char* fileName, const char* fooName, int line, bool arr) {
	void* ret = operator new(size);
	GC::RegNew(ret, size, fileName, fooName, line, arr);
	return ret;
}

void* operator new[](std::size_t size, const char* fileName, const char* fooName, int line, bool arr) {
	void* ret = operator new[](size);
	GC::RegNew(ret, size, fileName, fooName, line, arr);
	return ret;
}

void operator delete(void* p, bool __dummyVar) {
	operator delete(p);
	GC::Unreg(p);
}

void operator delete[](void* p, bool __dummyVar) {
	operator delete[](p);
	GC::Unreg(p);
}
