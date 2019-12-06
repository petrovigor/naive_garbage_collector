#include "gc.h"

void single_alloc( ) {
  {
    int *single = gc_new int;

    gc_delete_single( single ); //ok
  }

  {
    int *single = gc_new int;

    //automatic call of gc_delete_single( arr ) in main()
  }
}

void array_alloc( ) {
  {
    int *arr = gc_new int[4];

    gc_delete_array( arr ); //ok
  }

  {
    int *arr = gc_new int[4];

    //automatic call of gc_delete_array( arr ) in main()
  }
}

int main() {
  single_alloc();
  array_alloc();

  // memory leak message here at GC::~GC()
  return 0;
}

/*

  std::cerr on program exit:

potential leak prevented at: C:\Users\Igor\source\repos\ConsoleApplication4\ConsoleApplication4\example.cpp funcname: single_alloc line: 11 bytes: 4 is array: false
potential leak prevented at: C:\Users\Igor\source\repos\ConsoleApplication4\ConsoleApplication4\example.cpp funcname: array_alloc line: 25 bytes: 16 is array: true

*/
