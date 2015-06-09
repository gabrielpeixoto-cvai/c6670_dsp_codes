/*
 * osal.c
 *
 *  Created on: Apr 24, 2014
 *      Author: ima
 */

#include <xdc/std.h>
#include <ti/sysbios/BIOS.h> 				//mandatory - if you call APIs like BIOS_start()
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/knl/Semaphore.h>

void Osal_qmssBeginMemAccess(void* a, unsigned int b)
{
}

void* Osal_qmssCsEnter()
{
	return NULL;
}
void Osal_qmssCsExit(void* a)
{
}

void Osal_qmssEndMemAccess(void* a, unsigned int b)
{
}

void Osal_cppiBeginMemAccess(void* a, unsigned int b)
{
}

void* Osal_cppiCsEnter()
{
	return NULL;
}
void Osal_cppiCsExit(void* a)
{
}

void Osal_cppiEndMemAccess(void* a, unsigned int b)
{
}

Ptr Osal_cppiMalloc (UInt32 num_bytes)
{
	return Memory_alloc(NULL, num_bytes, 0, NULL);
}

Void Osal_cppiFree (Ptr ptr, UInt32 size)
{
	Memory_free (NULL, ptr, size);
}

/* FFTC specific */
#if 1
void* Osal_fftcMalloc(UInt32 num_bytes, uint8_t bGlobalAddress)
{
	return Osal_cppiMalloc(num_bytes);
}
void Osal_fftcFree(Ptr dataPtr, UInt32 num_bytes, Bool bGlobalAddress)
{
	Osal_cppiFree(dataPtr, num_bytes);
}
void Osal_fftcBeginDescMemAccess() {}
void Osal_fftcBeginMemAccess() {}
void* Osal_fftcCreateSem()
{
	return ((Void *) Semaphore_create (0, NULL, NULL));
}
void Osal_fftcDeleteSem(Void* hSem)
{
	Semaphore_delete(hSem);
}
void Osal_fftcPostSem(Void* hSem)
{
	Semaphore_post(hSem);
}
void Osal_fftcEndDescMemAccess() {}
void Osal_fftcEndMemAccess() {}
void Osal_fftcLog() {}
void Osal_fftcMultiCoreCsEnter() {}
void Osal_fftcMultiCoreCsExit() {}

void Osal_fftcBeginDataBufMemAccess (Void *dataBufPtr, UInt32 size) {}
void Osal_fftcEndDataBufMemAccess (Void *dataBufPtr, UInt32 size) {}
void Osal_fftcInterruptCsEnter () {}
void Osal_fftcInterruptCsExit() {}
void Osal_fftcPendSem(void*  hSem) {
	Semaphore_pend (hSem, BIOS_WAIT_FOREVER);
}
#endif

