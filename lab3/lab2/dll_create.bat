g++ -c -DBUILDING_EXAMPLE_DLL sort.c
g++ -shared -o sort.dll sort.o -Wl,--out-implib,libexample_dll.a
pause