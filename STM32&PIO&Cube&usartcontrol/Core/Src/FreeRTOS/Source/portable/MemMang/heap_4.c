/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/*
 * A sample implementation of pvPortMalloc() and vPortFree() that combines
 * (coalescences) adjacent memory blocks as they are freed, and in so doing
 * limits memory fragmentation.
 *
 * See heap_1.c, heap_2.c and heap_3.c for alternative implementations, and the
 * memory management pages of http://www.FreeRTOS.org for more information.
 */
#include <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
all the API functions to use the MPU wrappers.  That should only be done when
task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#include "FreeRTOS.h"
#include "task.h"

#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#if (configSUPPORT_DYNAMIC_ALLOCATION == 0)
#error This file must not be used if configSUPPORT_DYNAMIC_ALLOCATION is 0
#endif

/* Block sizes must not get too small. */
#define heapMINIMUM_BLOCK_SIZE ((size_t)(xHeapStructSize << 1))

/* Assumes 8bit bytes! */
#define heapBITS_PER_BYTE ((size_t)8)

/* Allocate the memory for the heap. */
#if (configAPPLICATION_ALLOCATED_HEAP == 1)
/* The application writer has already defined the array used for the RTOS
heap - probably so it can be placed in a special segment or address. */
extern uint8_t ucHeap[configTOTAL_HEAP_SIZE];
#else
static uint8_t ucHeap[configTOTAL_HEAP_SIZE];
#endif /* configAPPLICATION_ALLOCATED_HEAP */

/* Define the linked list structure.  This is used to link free blocks in order
of their memory address. */
typedef struct A_BLOCK_LINK
{
	struct A_BLOCK_LINK *pxNextFreeBlock; /*<< The next free block in the list. */
	size_t xBlockSize;					  /*<< The size of the free block. */
} BlockLink_t;

/*-----------------------------------------------------------*/

/*
 * Inserts a block of memory that is being freed into the correct position in
 * the list of free memory blocks.  The block being freed will be merged with
 * the block in front it and/or the block behind it if the memory blocks are
 * adjacent to each other.
 */
static void prvInsertBlockIntoFreeList(BlockLink_t *pxBlockToInsert);

/*
 * Called automatically to setup the required heap structures the first time
 * pvPortMalloc() is called.
 */
static void prvHeapInit(void);

/*-----------------------------------------------------------*/

/* The size of the structure placed at the beginning of each allocated memory
block must by correctly byte aligned. */
static const size_t xHeapStructSize = (sizeof(BlockLink_t) + ((size_t)(portBYTE_ALIGNMENT - 1))) & ~((size_t)portBYTE_ALIGNMENT_MASK);

/* Create a couple of list links to mark the start and end of the list. */
static BlockLink_t xStart, *pxEnd = NULL;

/* Keeps track of the number of free bytes remaining, but says nothing about
fragmentation. */
static size_t xFreeBytesRemaining = 0U;
static size_t xMinimumEverFreeBytesRemaining = 0U;

/* Gets set to the top bit of an size_t type.  When this bit in the xBlockSize
member of an BlockLink_t structure is set then the block belongs to the
application.  When the bit is free the block is still part of the free heap
space. */
static size_t xBlockAllocatedBit = 0;

/*-----------------------------------------------------------*/

void *pvPortMalloc(size_t xWantedSize)
{
	BlockLink_t *pxBlock, *pxPreviousBlock, *pxNewBlockLink;
	void *pvReturn = NULL;

	vTaskSuspendAll();
	{
		/* If this is the first call to malloc then the heap will require
		initialisation to setup the list of free blocks. */
		if (pxEnd == NULL)
		{
			prvHeapInit();
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}

		/* Check the requested block size is not so large that the top bit is
		set.  The top bit of the block size member of the BlockLink_t structure
		is used to determine who owns the block - the application or the
		kernel, so it must be free. */
		//判断是否小于最小块大小，如果小于允许的最大块大小(将xBlockAllocatedBit某个非最高位置1即可实现控制允许分配的最大空间)，
		//则返回NULL，分配失败
		//同时此变量最高位还用于判断某一内存块是否是空闲块
		if ((xWantedSize & xBlockAllocatedBit) == 0)
		{
			/* The wanted size is increased so it can contain a BlockLink_t
			structure in addition to the requested amount of bytes. */
			if (xWantedSize > 0)
			{
				//腾出一个结构体的空间存放BlockLink_t，用于记录是否已经被使用了，还是当前还是空闲的
				xWantedSize += xHeapStructSize;

				/* Ensure that blocks are always aligned to the required number
				of bytes. */
				if ((xWantedSize & portBYTE_ALIGNMENT_MASK) != 0x00)
				{
					/* Byte alignment required. */
					//保证每次分配都是对齐，在进行向下取整操作，把最低三位不是0的变成0，用户申请的空间会少了没有对齐的部分，至此就知道需要多少空间
					xWantedSize += (portBYTE_ALIGNMENT - (xWantedSize & portBYTE_ALIGNMENT_MASK));
					configASSERT((xWantedSize & portBYTE_ALIGNMENT_MASK) == 0);
				}
				else
				{
					mtCOVERAGE_TEST_MARKER();
				}
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}

			if ((xWantedSize > 0) && (xWantedSize <= xFreeBytesRemaining))
			{
				/* Traverse the list from the start	(lowest address) block until
				one	of adequate size is found. */
				pxPreviousBlock = &xStart;
				pxBlock = xStart.pxNextFreeBlock;
				while ((pxBlock->xBlockSize < xWantedSize) && (pxBlock->pxNextFreeBlock != NULL))
				{
					//旧的节点
					pxPreviousBlock = pxBlock;
					//更新当前指针
					pxBlock = pxBlock->pxNextFreeBlock;
				}

				/* If the end marker was reached then a block of adequate size
				was	not found. */
				if (pxBlock != pxEnd)
				{
					/* Return the memory space pointed to - jumping over the
					BlockLink_t structure at its start. */
					pvReturn = (void *)(((uint8_t *)pxPreviousBlock->pxNextFreeBlock) + xHeapStructSize);

					/* This block is being returned for use so must be taken out
					of the list of free blocks. */
					//把当前的空闲节点跳过，把下一个节点的前驱节点指向当前节点的下一个节点
					pxPreviousBlock->pxNextFreeBlock = pxBlock->pxNextFreeBlock;

					/* If the block is larger than required it can be split into
					two. */
					if ((pxBlock->xBlockSize - xWantedSize) > heapMINIMUM_BLOCK_SIZE)
					{
						/* This block is to be split into two.  Create a new
						block following the number of bytes requested. The void
						cast is used to prevent byte alignment warnings from the
						compiler. */
						// pxNewBlockLink存着可以被再分出加入到空闲链表的节点
						//此处的xWantedSize已经包含了头，所以会正确的跳到空闲的开始位置,赋值给了pxNewBlockLink，后面用它去插入到空闲链表中
						pxNewBlockLink = (void *)(((uint8_t *)pxBlock) + xWantedSize);
						configASSERT((((size_t)pxNewBlockLink) & portBYTE_ALIGNMENT_MASK) == 0);

						/* Calculate the sizes of two blocks split from the
						single block. */
						pxNewBlockLink->xBlockSize = pxBlock->xBlockSize - xWantedSize;
						pxBlock->xBlockSize = xWantedSize;

						/* Insert the new block into the list of free blocks. */
						prvInsertBlockIntoFreeList(pxNewBlockLink);
					}
					else
					{
						mtCOVERAGE_TEST_MARKER();
					}

					xFreeBytesRemaining -= pxBlock->xBlockSize;

					if (xFreeBytesRemaining < xMinimumEverFreeBytesRemaining)
					{
						xMinimumEverFreeBytesRemaining = xFreeBytesRemaining;
					}
					else
					{
						mtCOVERAGE_TEST_MARKER();
					}

					/* The block is being returned - it is allocated and owned
					by the application and has no "next" block. */
					//表示分配出去了，xBlockSize最高位为1表示已经分配出去了，并且下个块的指向为空
					pxBlock->xBlockSize |= xBlockAllocatedBit;
					pxBlock->pxNextFreeBlock = NULL;
				}
				else
				{
					mtCOVERAGE_TEST_MARKER();
				}
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}

		traceMALLOC(pvReturn, xWantedSize);
	}
	(void)xTaskResumeAll();

#if (configUSE_MALLOC_FAILED_HOOK == 1)
	{
		if (pvReturn == NULL)
		{
			extern void vApplicationMallocFailedHook(void);
			vApplicationMallocFailedHook();
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}
	}
#endif

	configASSERT((((size_t)pvReturn) & (size_t)portBYTE_ALIGNMENT_MASK) == 0);
	return pvReturn;
}
/*-----------------------------------------------------------*/

void vPortFree(void *pv)
{
	uint8_t *puc = (uint8_t *)pv;
	BlockLink_t *pxLink;

	if (pv != NULL)
	{
		/* The memory being freed will have an BlockLink_t structure immediately
		before it. */
		//用户获得分配的内存首地址是除去了结构体的，下面补上
		puc -= xHeapStructSize;

		/* This casting is to keep the compiler from issuing warnings. */
		pxLink = (void *)puc;

		/* Check the block is actually allocated. */
		configASSERT((pxLink->xBlockSize & xBlockAllocatedBit) != 0);
		configASSERT(pxLink->pxNextFreeBlock == NULL);

		if ((pxLink->xBlockSize & xBlockAllocatedBit) != 0)
		{
			if (pxLink->pxNextFreeBlock == NULL)
			{
				/* The block is being returned to the heap - it is no longer
				allocated. */
				//表示这块空间回收回来了
				pxLink->xBlockSize &= ~xBlockAllocatedBit;

				vTaskSuspendAll();
				{
					/* Add this block to the list of free blocks. */
					xFreeBytesRemaining += pxLink->xBlockSize;
					traceFREE(pv, pxLink->xBlockSize);
					prvInsertBlockIntoFreeList(((BlockLink_t *)pxLink));
				}
				(void)xTaskResumeAll();
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}
	}
}
/*-----------------------------------------------------------*/

size_t xPortGetFreeHeapSize(void)
{
	return xFreeBytesRemaining;
}
/*-----------------------------------------------------------*/

size_t xPortGetMinimumEverFreeHeapSize(void)
{
	return xMinimumEverFreeBytesRemaining;
}
/*-----------------------------------------------------------*/

void vPortInitialiseBlocks(void)
{
	/* This just exists to keep the linker quiet. */
}
/*-----------------------------------------------------------*/

static void prvHeapInit(void)
{
	// pxFirstFreeBlock是一个局部变量，只是用它来表示start的下一个指向节点
	BlockLink_t *pxFirstFreeBlock;
	uint8_t *pucAlignedHeap;
	size_t uxAddress;
	size_t xTotalHeapSize = configTOTAL_HEAP_SIZE;

	/* Ensure the heap starts on a correctly aligned boundary. */
	uxAddress = (size_t)ucHeap;

	if ((uxAddress & portBYTE_ALIGNMENT_MASK) != 0)
	{
		//对齐操作，+7是进位，把后三位与成0
		uxAddress += (portBYTE_ALIGNMENT - 1);
		uxAddress &= ~((size_t)portBYTE_ALIGNMENT_MASK);
		//原本大小减去地址对齐损失的大小，得到了当前可以使用的堆区的大小
		xTotalHeapSize -= uxAddress - (size_t)ucHeap;
	}
	//完成对齐的堆区
	pucAlignedHeap = (uint8_t *)uxAddress;

	/* xStart is used to hold a pointer to the first item in the list of free
	blocks.  The void cast is used to prevent compiler warnings. */
	//双向链表
	xStart.pxNextFreeBlock = (void *)pucAlignedHeap;
	xStart.xBlockSize = (size_t)0;

	/* pxEnd is used to mark the end of the list of free blocks and is inserted
	at the end of the heap space. */
	//腾出了一个BlockLink_t结构体的空间用于记录该空闲块的是被使用的还是空闲的
	uxAddress = ((size_t)pucAlignedHeap) + xTotalHeapSize;

	uxAddress -= xHeapStructSize;
	uxAddress &= ~((size_t)portBYTE_ALIGNMENT_MASK);
	pxEnd = (void *)uxAddress;
	pxEnd->xBlockSize = 0;
	pxEnd->pxNextFreeBlock = NULL;

	/* To start with there is a single free block that is sized to take up the
	entire heap space, minus the space taken by pxEnd. */
	pxFirstFreeBlock = (void *)pucAlignedHeap;
	pxFirstFreeBlock->xBlockSize = uxAddress - (size_t)pxFirstFreeBlock;
	pxFirstFreeBlock->pxNextFreeBlock = pxEnd;

	/* Only one block exists - and it covers the entire usable heap space. */
	xMinimumEverFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;
	xFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;

	/* Work out the position of the top bit in a size_t variable. */
	//后续使用此变量去判断该内存块是否是空闲的，32位的变量，最高位为1
	xBlockAllocatedBit = ((size_t)1) << ((sizeof(size_t) * heapBITS_PER_BYTE) - 1);
}
/*-----------------------------------------------------------*/

static void prvInsertBlockIntoFreeList(BlockLink_t *pxBlockToInsert)
{
	BlockLink_t *pxIterator;
	uint8_t *puc;

	/* Iterate through the list until a block is found that has a higher address
	than the block being inserted. */
	//找高地址的位置进行插入
	for (pxIterator = &xStart; pxIterator->pxNextFreeBlock < pxBlockToInsert; pxIterator = pxIterator->pxNextFreeBlock)
	{
		/* Nothing to do here, just iterate to the right position. */
	}

	/* Do the block being inserted, and the block it is being inserted after
	make a contiguous block of memory? */
	puc = (uint8_t *)pxIterator;
	if ((puc + pxIterator->xBlockSize) == (uint8_t *)pxBlockToInsert)
	{
		pxIterator->xBlockSize += pxBlockToInsert->xBlockSize;
		// pxBlockToInsert指向的内存块被合并到了pxIterator指向的内存块中
		//并以pxBlockToInsert存着，作为后面判断是否是中间的碎片的依据
		pxBlockToInsert = pxIterator;
	}
	else
	{
		mtCOVERAGE_TEST_MARKER();
	}

	/* Do the block being inserted, and the block it is being inserted before
	make a contiguous block of memory? */
	puc = (uint8_t *)pxBlockToInsert;
	// if成立表示后面是连续的
	if ((puc + pxBlockToInsert->xBlockSize) == (uint8_t *)pxIterator->pxNextFreeBlock)
	{
		//插在后面元素的前面就得考虑是否是结束节点的问题
		if (pxIterator->pxNextFreeBlock != pxEnd)
		{
			/* Form one big block from the two blocks. */
			pxBlockToInsert->xBlockSize += pxIterator->pxNextFreeBlock->xBlockSize;
			pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock->pxNextFreeBlock;
		}
		//说明这个空闲块的结束节点的相邻块
		else
		{
			pxBlockToInsert->pxNextFreeBlock = pxEnd;
		}
	}
	else //表示后面不连续，跳着来
	{
		pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock;
	}

	/* If the block being inserted plugged a gab, so was merged with the block
	before and the block after, then it's pxNextFreeBlock pointer will have
	already been set, and should not be set here as that would make it point
	to itself. */
	// pxIterator != pxBlockToInsert说明不是以上两种情况，属于这个空闲块在前后两个空闲块中间的且不相邻
	//先让它指向自己，后面有处理方法，作为一个标记
	if (pxIterator != pxBlockToInsert)
	{
		pxIterator->pxNextFreeBlock = pxBlockToInsert;
	}
	else
	{
		mtCOVERAGE_TEST_MARKER();
	}
}
