#ifndef LAB1_DLL_H
#define LAB1_DLL_H
#ifdef __cplusplus
extern "C"
{
#endif


#ifdef BUILDING_EXAMPLE_DLL
#define EXAMPLE_DLL __declspec(dllexport)
#else
#define EXAMPLE_DLL __declspec(dllimport)
#endif
EXAMPLE_DLL int lab1();
EXAMPLE_DLL int lab2();

#ifdef __cplusplus
}
#endif

#endif