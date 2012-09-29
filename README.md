DLL Injector
============

As I wasn't able to find an equivalent project, I decided to create my own one. This project injects a DLL into a running process or starts a process with DLL being the very first one imported.


Command Line Options
--------------
  -h [ --help ]         show help message
  -r [ --run ] arg      starts supplied executable and injects the DLL before any other DLL is loaded or the entry point is reached
  -p [ --pid ] arg      injects DLL into a running process denoted by supplied PID
  -s [ --suspend ]      pause process after DLL injection. Useful for debugging
  -d [ --dll ] arg      DLL to be injected


Examples
--------------
	DllInjector -r testapp.exe -d dll.dll
	
	DllInjector -p [PID of explorer.exe] -d dll.dll


Build Prerequisites
--------------

* Install the [Boost](http://www.boost.org/) library. Make sure to install it with runtime-link=static!
* Create an environment variable called BOOSTDIR and make it point to the installed Boost library.
* If necessary, copy the target installations files, so you will have a directory structure like this:
	* %BOOSTDIR%\include\boost contains all Boost header files
	* %BOOSTDIR%\lib contains all Boost library files
* In order to run the tests successfully, install the [Google C++ Testing Framework](http://code.google.com/p/googletest/).
* Compile googletest,
* Create an environment variable called GTESTDIR pointing to the googletest directory.

