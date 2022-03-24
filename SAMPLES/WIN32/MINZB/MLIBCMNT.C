/*
 * Add comments for appropriate libraries - Microsoft
 */
#include <brender.h>

#if defined(_DEBUG)
	/*
	 * Debug libraries
	 */
#if BASED_FIXED
#pragma comment(lib,"brdbmxr")
#pragma comment(lib,"brfwmxr")
#pragma comment(lib,"brmtmxr")
#pragma comment(lib,"brfmmxr")
#endif
#if BASED_FLOAT
#pragma comment(lib,"brdbmfr")
#pragma comment(lib,"brfwmfr")
#pragma comment(lib,"brmtmfr")
#pragma comment(lib,"brfmmfr")
#endif

#pragma comment(lib,"brpmmr")
#pragma comment(lib,"brstmr")
#pragma comment(lib,"hstwnmr")


#else
	/*
	 * Release libraries
	 */
#if BASED_FIXED
#pragma comment(lib,"brdbmxr")
#pragma comment(lib,"brfwmxr")
#pragma comment(lib,"brmtmxr")
#pragma comment(lib,"brfmmxr")
#endif
#if BASED_FLOAT
#pragma comment(lib,"brdbmfr")
#pragma comment(lib,"brfwmfr")
#pragma comment(lib,"brmtmfr")
#pragma comment(lib,"brfmmfr")
#endif

#pragma comment(lib,"brpmmr")
#pragma comment(lib,"brstmr")
#pragma comment(lib,"hstwnmr")

#endif


