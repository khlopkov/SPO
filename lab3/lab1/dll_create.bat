g++ -c -DBUILDING_EXAMPLE_DLL lab1.cpp
g++ -shared -o labspo.dll lab1.o -Wl,--out-implib,libexample_dll.a
pause