/*
 * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: fw_p.h 2.12 1996/03/07 20:25:43 sam Exp $
 * $Locker:  $
 *
 * Public function prototypes for BRender framework
 */
#ifndef _FW_P_H_
#define _FW_P_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _NO_PROTOTYPES

/*
 * Global setup
 */
br_error BR_PUBLIC_ENTRY BrBegin(void);
br_error BR_PUBLIC_ENTRY BrEnd(void);

/*
 * Framework Setup
 */
br_error BR_PUBLIC_ENTRY BrFwBegin(void);
br_error BR_PUBLIC_ENTRY BrFwEnd(void);

/*
 * Resource class handling
 */
br_resource_class * BR_PUBLIC_ENTRY BrResClassAdd(br_resource_class *pixelmap);
br_resource_class * BR_PUBLIC_ENTRY BrResClassRemove(br_resource_class *pixelmap);
br_resource_class * BR_PUBLIC_ENTRY BrResClassFind(char *pattern);
typedef br_resource_class * BR_CALLBACK br_resclass_find_cbfn(char *name);
br_resclass_find_cbfn *BR_PUBLIC_ENTRY BrResClassFindHook(br_resclass_find_cbfn *hook);
br_uint_32 BR_PUBLIC_ENTRY BrResClassAddMany(br_resource_class **items, int n);
br_uint_32 BR_PUBLIC_ENTRY BrResClassRemoveMany(br_resource_class **items, int n);
br_uint_32 BR_PUBLIC_ENTRY BrResClassFindMany(char *pattern, br_resource_class **items, int max);
br_uint_32 BR_PUBLIC_ENTRY BrResClassCount(char *pattern);

typedef br_uint_32 BR_CALLBACK br_resclass_enum_cbfn(br_resource_class *item, void *arg);

br_uint_32 BR_PUBLIC_ENTRY BrResClassEnum(char *pattern,
		br_resclass_enum_cbfn *callback, void *arg);

/*
 * Block pool allocator
 */
br_pool * BR_PUBLIC_ENTRY BrPoolAllocate(int block_size, int chunk_size, br_uint_8 mem_type);
void BR_PUBLIC_ENTRY BrPoolFree(br_pool *pool);

void * BR_PUBLIC_ENTRY BrPoolBlockAllocate(struct br_pool *pool);
void BR_PUBLIC_ENTRY BrPoolBlockFree(struct br_pool *pool,void *block);

void BR_PUBLIC_ENTRY BrPoolEmpty(struct br_pool *pool);

/*
 * Byte swapping
 */
br_uint_32 BR_RESIDENT_ENTRY BrSwap32(br_uint_32 l);
br_uint_16 BR_RESIDENT_ENTRY BrSwap16(br_uint_16 s);
br_float   BR_RESIDENT_ENTRY BrSwapFloat(br_float f);
void *BR_RESIDENT_ENTRY BrSwapBlock(void *block, int count, int size);

/*
 * Misc. support
 */
typedef int BR_CALLBACK br_qsort_cbfn(const void *, const void *);
void BR_RESIDENT_ENTRY BrQsort(void *basep, unsigned int nelems, unsigned int size,
                            br_qsort_cbfn *comp);
/*
 * Diagnostic generation
 */
void BR_RESIDENT_ENTRY BrFailure(char *s,...);
void BR_RESIDENT_ENTRY BrWarning(char *s,...);
void BR_RESIDENT_ENTRY BrFatal(char *name, int line, char *s,...);
/*
 * Set new handlers
 */
br_diaghandler * BR_PUBLIC_ENTRY BrDiagHandlerSet(br_diaghandler *newdh);
br_filesystem * BR_PUBLIC_ENTRY BrFilesystemSet(br_filesystem *newfs);
br_allocator * BR_PUBLIC_ENTRY BrAllocatorSet(br_allocator *newal);

/*
 * Backwards compatibility
 */
#define BrErrorHandlerSet BrDiagHandlerSet

/*
 * Generic file IO
 */
br_uint_32 BR_PUBLIC_ENTRY BrFileAttributes(void);


void * BR_PUBLIC_ENTRY BrFileOpenRead(char *name, br_size_t n_magics,
		br_mode_test_cbfn *mode_test,
		int *mode_result);
void * BR_PUBLIC_ENTRY BrFileOpenWrite(char *name, int text);
void BR_PUBLIC_ENTRY BrFileClose(void *f);
int BR_PUBLIC_ENTRY BrFileEof(void *f);
int BR_PUBLIC_ENTRY BrFileGetChar(void *f);
void BR_PUBLIC_ENTRY BrFilePutChar(int c, void *f);
int BR_PUBLIC_ENTRY BrFileRead(void *buf, int size, int n,void *f);
int BR_PUBLIC_ENTRY BrFileWrite(void *buf, int size, int n, void *f);
int BR_PUBLIC_ENTRY BrFileGetLine(char *buf, br_size_t buf_len, void * f);
void BR_PUBLIC_ENTRY BrFilePutLine(char *buf, void * f);
void BR_PUBLIC_ENTRY BrFileAdvance(long int count, void *f);

int BR_PUBLIC_ENTRY BrFilePrintf(void *f, char *fmt, ...);

br_int_32 BR_RESIDENT_ENTRY BrSprintf(char *buf, char *fmt, ...);
br_int_32 BR_RESIDENT_ENTRY BrSprintfN(char *buf, br_size_t buf_size, char *fmt, ...);

br_int_32 BR_RESIDENT_ENTRY BrSScanf(char *str, char *fmt, ...);

/*
 * Data file output type (one of BR_FS_MODE_xxx)
 */
int BR_PUBLIC_ENTRY BrWriteModeSet(int text);


/*
 * Generic memory allocation
 */
void * BR_RESIDENT_ENTRY BrMemAllocate(br_size_t size, br_uint_8 type);
void BR_RESIDENT_ENTRY BrMemFree(void *block);
br_size_t BR_RESIDENT_ENTRY BrMemInquire(br_uint_8 type);
void BR_RESIDENT_ENTRY BrMemShrink(void *block, br_size_t size);

char * BR_RESIDENT_ENTRY BrMemStrDup(char *str);
void * BR_RESIDENT_ENTRY BrMemCalloc(int nelems, br_size_t size, br_uint_8 type);

/*
 * Resource allocation
 */
void * BR_RESIDENT_ENTRY BrResAllocate(void *vparent, br_size_t size, br_uint_8 res_class);
void BR_RESIDENT_ENTRY BrResFree(void *vres);
void BR_RESIDENT_ENTRY BrResFreeNoCallback(void *vres);
char * BR_RESIDENT_ENTRY BrResStrDup(void *vparent, char *str);

void * BR_RESIDENT_ENTRY BrResAdd(void *vparent, void *vres);
void * BR_RESIDENT_ENTRY BrResRemove(void *vres);
br_uint_8 BR_RESIDENT_ENTRY BrResClass(void * vres);
br_uint_32 BR_RESIDENT_ENTRY BrResSize(void *vres);
br_uint_32 BR_RESIDENT_ENTRY BrResSizeTotal(void *vres);
typedef br_uint_32 BR_CALLBACK br_resenum_cbfn(void *vres, void *arg);
br_uint_32 BR_RESIDENT_ENTRY BrResChildEnum(void *vres, br_resenum_cbfn *callback, void *arg);
br_boolean BR_RESIDENT_ENTRY BrResIsChild(void *vparent, void *vchild);

/*
 * Block operations (XXX should be made private)
 */
void BR_ASM_CALL BrBlockFill(void *dest_ptr, int value, int dwords);
void BR_ASM_CALL BrBlockCopy(void *dest_ptr, void *src_ptr, int dwords);

#if BR_HAS_FAR
void BR_ASM_CALL BrFarBlockCopy(void __far *dest_ptr, void *src_ptr, int dwords);
#endif

/*
 * Scratchpad buffer allocation - Currenty, only one allocation
 * may be outstanding at any time
 */
void *BR_RESIDENT_ENTRY BrScratchAllocate(br_size_t size);
void BR_RESIDENT_ENTRY BrScratchFree(void *scratch);
br_size_t BR_RESIDENT_ENTRY BrScratchInquire(void);
void BR_RESIDENT_ENTRY BrScratchFlush(void);

/*
 * Error retrieval
 */
br_error BR_RESIDENT_ENTRY BrGetLastError(void **valuep);

/*
 * Device mangement
 */
struct br_device;

typedef struct br_device * BR_EXPORT br_device_begin_fn(char *arguments);

br_error BR_RESIDENT_ENTRY BrDevAdd(struct br_device **pdev, char * image, char *args);
br_error BR_RESIDENT_ENTRY BrDevAddStatic(struct br_device **pdev, br_device_begin_fn *begin, char *args);
br_error BR_RESIDENT_ENTRY BrDevAddConfig(char *config);
br_error BR_RESIDENT_ENTRY BrDevRemove(struct br_device *dev);

br_error BR_RESIDENT_ENTRY BrDevicesQuery(struct br_device **devices, br_int_32 *ndevices, br_int_32 max_devices);

br_error BR_RESIDENT_ENTRY BrDevFindMany(struct br_device **devices, br_int_32 *ndevices, br_int_32 max_devices, char *pattern);
br_error BR_RESIDENT_ENTRY BrDevCount(br_int_32 *ndevices, char *pattern);
br_error BR_RESIDENT_ENTRY BrDevFind(struct br_device **pdev, char *pattern);

br_error BR_RESIDENT_ENTRY BrDevContainedFind(struct br_object **ph, br_token type, char *pattern, br_token_value *tv);
br_error BR_RESIDENT_ENTRY BrDevContainedFindMany(struct br_object **objects, br_int_32 max_objects,
			br_int_32 *pnum_objects, br_token type, char *pattern, br_token_value *tv);
br_error BR_RESIDENT_ENTRY BrDevContainedCount(br_int_32 *pcount, br_token type, char *pattern, br_token_value *tv);

br_error BR_PUBLIC_ENTRY BrDevBegin(br_pixelmap **ppm, char *setup_string);
br_pixelmap * BR_PUBLIC_ENTRY BrDevBeginOld(char *setup_string);
void BR_PUBLIC_ENTRY BrDevEndOld(void);
void BR_PUBLIC_ENTRY BrDevPaletteSetOld(br_pixelmap *pm);
void BR_PUBLIC_ENTRY BrDevPaletteSetEntryOld(int i,br_colour colour);

br_error BR_RESIDENT_ENTRY BrRendererFacilityFind(struct br_renderer_facility **prf,
				struct br_device_pixelmap *destination, br_token scalar_type);

br_error BR_RESIDENT_ENTRY BrPrimitiveLibraryFind(struct br_primitive_library **ppl,
				struct br_device_pixelmap *destination, br_token scalar_type);

#endif /* _NO_PROTOTYPES */

#ifdef __cplusplus
};
#endif
#endif

