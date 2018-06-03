#ifndef LAB2_DLL_H
#define LAB2_DLL_H
#ifdef __cplusplus
extern "C"
{
#endif


#ifdef BUILDING_EXAMPLE_DLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif
DLL void sort_binary_inserts(int* arrayPtr, int size);
DLL void sort_heaps(char* filename) ;
DLL void sort_mapping(char* filename) ;
DLL void sort_basepoints(char* filename) ;




#ifdef __cplusplus
}
#endif

#endif