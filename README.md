# kamelib
kamelib is my own version of the standard C library to replace it.

My goal with it is to be able to fully use it without the glibc library. It is still a work in progress so you still cant fully achieve this.

There are currently 6 mini libraries that make up kamelib (with more in the future) kmem, kproc, kio, kstr, ktypes, and kvec.

kmem is my own memory allocator with the functions _malloc, _realloc, _free, and other helper functions.

kproc is my memory manipulator. It has functions like proc_getpid, proc_readmem, and proc_writemem for changing or reading memory in a process.
It has other functions like proc_scanmem and proc_rescanmem that take a process id, a value, and alist (which is a simple address list vector) and then scan the memory of the process for that value
or rescan again to find the exact value. It is inspired from cheat engine and PINCE.

kio is my input and output library. But i still don't have any input functions (as i said kamelib is still a work in progress). The functions that i currently have are _print which acts like printf but only has a limited amount of identifiers (%d, %c, %cs for char pointer or array, %s for my own string, and %us for my own type usize). _printin acts like snprintf, it also has the same limited identifiers for now.

kstr is my own string library. I made it to have similar strings to c++. It has many functions that i wont cover here now (i will add them here in the future because i want to get this git stuff working first)

kvec is the string vector library. I also made it to have similar vectors to c++. Again, it has many functions that i will add them here later.

kstr and kvec sorta only work with each other (because they were the first ones i made and i didn't plan to add any more types to them) so in the future i will make different vectors for other data types. 

To include the libraries you can just download the header files and either include kamelib.h only or include the ones you need.
