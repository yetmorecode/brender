/*
 * Add comments for appropriate libraries - Microsoft
 */
#include <brender.h>

#if defined(_DEBUG)
#define _REL "r"
#else
#define _REL "r"
#endif

#if BASED_FIXED
#define _BASED "x"
#endif

#if BASED_FLOAT
#define _BASED "f"
#endif

#pragma comment(lib,"brdbm" _BASED _REL)
#pragma comment(lib,"brmtm" _BASED _REL)
#pragma comment(lib,"brfmm" _BASED _REL)

#pragma comment(lib,"brfwm" _REL)
#pragma comment(lib,"brpmm" _REL)
#pragma comment(lib,"brstm" _REL)
#pragma comment(lib,"hstwnm" _REL)


