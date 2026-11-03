# kamelib
kamelib is my own version of the standard C library to replace it.

My goal with it is to be able to fully use it without the glibc library. It is still a work in progress so you still can't fully achieve this.

There are currently 6 mini libraries that make up kamelib (with more in the future) kmem, kproc, kio, kstr, ktypes, and kvec.

kmem is my own memory allocator with the functions _malloc, _realloc, _free, and other helper functions.

kproc is my memory manipulator. It has functions like proc_getpid, proc_readmem, and proc_writemem for changing or reading memory in a process.
It has other functions like proc_scanmem and proc_rescanmem that take a process id, a value, and alist (which is a simple address list vector found in kvec) and then scan the memory of the process for that value
or rescan again to find the exact value. It is inspired from cheat engine and PINCE.

kio is my input and output library. But I still don't have any input functions (as I said kamelib is still a work in progress). The functions that I currently have are _print which acts like printf but only has a limited amount of identifiers (%d, %c, %cs for char pointer or array, %s for my own string, %us for my own type usize, and %v for my own vector). _printin acts like snprintf, it also has the same limited identifiers for now.

kstr is my own string library. I made it to have similar strings to c++. It has many functions that I won't cover here now (i will add them here in the future because I want to get this git stuff working first)

kvec is the string vector library. I also made it to have similar vectors to c++. Again, it has many functions that I will add them here later.

The string and the vector structs only work with each other (because they were the first ones I made and I didn't plan to add any more types to them) so in the future I will make different vectors for other data types (like alist). 

The code currently doesn't have comments. I am planning for a future patch to add comments in the code to further explain the flow of it.

Currently some stuff only work on Linux like the kproc library, other stuff might work more on macOS than windows due to both being unix systems, but I haven't tested it there so they aren't really supported for now.

kamelib is header-only, to include the libraries you can just download the header files and either include kamelib.h only or include the ones you need.
