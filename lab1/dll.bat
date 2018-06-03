g++ -c -DBUILDING_EXAMPLE_DLL lab1.cpp
g++ -shared -o example_dll.dll example_dll.o -Wl,--out-implib,libexample_dll.a
pause