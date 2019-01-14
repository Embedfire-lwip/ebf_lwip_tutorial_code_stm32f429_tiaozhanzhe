///*
// * Copyright (c) 2017 Simon Goldschmidt
// * All rights reserved. 
// * 
// * Redistribution and use in source and binary forms, with or without modification, 
// * are permitted provided that the following conditions are met:
// *
// * 1. Redistributions of source code must retain the above copyright notice,
// *    this list of conditions and the following disclaimer.
// * 2. Redistributions in binary form must reproduce the above copyright notice,
// *    this list of conditions and the following disclaimer in the documentation
// *    and/or other materials provided with the distribution.
// * 3. The name of the author may not be used to endorse or promote products
// *    derived from this software without specific prior written permission. 
// *
// * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
// * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
// * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
// * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
// * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
// * OF SUCH DAMAGE.
// *
// * This file is part of the lwIP TCP/IP stack.
// * 
// * Author: Simon Goldschmidt
// *
// */


//#include <lwip/opt.h>
//#include <lwip/arch.h>
//#if !NO_SYS
//#include "sys_arch.h"
//#endif
//#include <lwip/stats.h>
//#include <lwip/debug.h>
//#include <lwip/sys.h>

//#include <string.h>

//int errno;


//u32_t lwip_sys_now;

//u32_t
//sys_jiffies(void)
//{
//  if(__get_IPSR() != 0)
//    lwip_sys_now = xTaskGetTickCountFromISR();
//  else
//    lwip_sys_now = xTaskGetTickCount();
//  
//  return lwip_sys_now;
//}

//u32_t
//sys_now(void)
//{
//  if(__get_IPSR() != 0)
//    lwip_sys_now = xTaskGetTickCountFromISR();
//  else
//    lwip_sys_now = xTaskGetTickCount();
//  
//  return lwip_sys_now;
//}

//void
//sys_init(void)
//{
//  
//  printf("system init ok!\n");
//}

//#if !NO_SYS

//test_sys_arch_waiting_fn the_waiting_fn;

//void
//test_sys_arch_wait_callback(test_sys_arch_waiting_fn waiting_fn)
//{
//  the_waiting_fn = waiting_fn;
//}

//err_t
//sys_sem_new(sys_sem_t *sem, u8_t count)
//{
//  /* 创建 sem */
//  if(count <= 1)    
//    *sem = xSemaphoreCreateBinary();
//  else
//    *sem = xSemaphoreCreateCounting(count,count);
//  
//  if(*sem != SYS_SEM_NULL)
//    return ERR_OK;
//  
//  else
//  {
//    printf("[sys_arch]:new sem fail!\n");
//    return ERR_MEM;
//  }
//}

//void
//sys_sem_free(sys_sem_t *sem)
//{
//  /* 删除 sem */
//  vSemaphoreDelete(*sem);
//  *sem = SYS_SEM_NULL;
//}


//int sys_sem_valid(sys_sem_t *sem)                                               
//{
//  if (*sem == SYS_SEM_NULL)
//    return 0;
//  else
//    return 1;                                       
//}


//void
//sys_sem_set_invalid(sys_sem_t *sem)
//{
//  *sem = SYS_SEM_NULL;
//}

///* 
// 如果timeout参数不为零，则返回值为
// 等待信号量所花费的毫秒数。如果
// 信号量未在指定时间内发出信号，返回值为
// SYS_ARCH_TIMEOUT。如果线程不必等待信号量
// 该函数返回零。 */
//u32_t
//sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
//{
//  u32_t wait_tick = 0;
//  u32_t start_tick = 0 ;
//  
//  //看看信号量是否有效
//  if(*sem == SYS_SEM_NULL)
//    return SYS_ARCH_TIMEOUT;
//  
//  //首先获取开始等待信号量的时钟节拍
//  start_tick = sys_now();
//  
//  //timeout != 0，需要将ms换成系统的时钟节拍
//  if(timeout != 0)
//  {
//    //将ms转换成时钟节拍
//    wait_tick = timeout / portTICK_PERIOD_MS;
//    if (wait_tick == 0)
//      wait_tick = 1;
//  }
//  //一直阻塞
//  else
//    wait_tick = portMAX_DELAY;
//  
//    //判断等待信号量的上下文环境是否在中断中
//  if(__get_IPSR() != 0 )
//  {
//    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
//    xSemaphoreTakeFromISR(*sem, &pxHigherPriorityTaskWoken);
//    //如果需要的话进行一次任务切换，系统会判断是否需要进行切换
//    portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
//  }
//  else
//  {
//    //等待成功，计算等待的时间，否则就表示等待超时
//    if(xSemaphoreTake(*sem, wait_tick) == pdTRUE)
//      return (sys_now() - start_tick);
//    else
//      return SYS_ARCH_TIMEOUT;
//  }
//  return 0;
//}

//void
//sys_sem_signal(sys_sem_t *sem)
//{
//  BaseType_t xReturn = pdPASS;
//  //判断释放信号量的上下文环境是否在中断中
//  if(__get_IPSR() != 0 )
//  {
//    BaseType_t pxHigherPriorityTaskWoken;
//    //释放信号量
//    xReturn = xSemaphoreGiveFromISR(*sem , &pxHigherPriorityTaskWoken);	
//    
//    //如果需要的话进行一次任务切换，系统会判断是否需要进行切换
//    portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
//  }
//  else
//    xReturn = xSemaphoreGive( *sem );//给出二值信号量
//  
//  if(xReturn != pdTRUE)
//    printf("[sys_arch]:sem signal fail!\n");
//}

//err_t
//sys_mutex_new(sys_mutex_t *mutex)
//{
//  /* 创建 sem */   
//    *mutex = xSemaphoreCreateMutex();
//  if(*mutex != SYS_MRTEX_NULL)
//    return ERR_OK;
//  else
//  {
//    printf("[sys_arch]:new mutex fail!\n");
//    return ERR_MEM;
//  }
//}

//void
//sys_mutex_free(sys_mutex_t *mutex)
//{
//  vSemaphoreDelete(*mutex);
//}

//void
//sys_mutex_set_invalid(sys_mutex_t *mutex)
//{
//  *mutex = SYS_MRTEX_NULL;
//}

//void
//sys_mutex_lock(sys_mutex_t *mutex)
//{
//  xSemaphoreTake(*mutex,/* 互斥量句柄 */
//                 portMAX_DELAY); /* 等待时间 */
//}

//void
//sys_mutex_unlock(sys_mutex_t *mutex)
//{
//  xSemaphoreGive( *mutex );//给出互斥量
//}


//sys_thread_t
//sys_thread_new(const char *name, lwip_thread_fn function, void *arg, int stacksize, int prio)
//{
//  sys_thread_t handle;
//  BaseType_t xReturn = pdPASS;
//  /* 创建MidPriority_Task任务 */
//  xReturn = xTaskCreate((TaskFunction_t )function,  /* 任务入口函数 */
//                        (const char*    )name,/* 任务名字 */
//                        (uint16_t       )stacksize,  /* 任务栈大小 */
//                        (void*          )arg,/* 任务入口函数参数 */
//                        (UBaseType_t    )prio, /* 任务的优先级 */
//                        (TaskHandle_t*  )&handle);/* 任务控制块指针 */ 
//  if(xReturn != pdPASS)
//  {
//    printf("[sys_arch]:create task fail!err:%#lx\n",xReturn);
//    return NULL;
//  }
//  return handle;
//}

//err_t
//sys_mbox_new(sys_mbox_t *mbox, int size)
//{
//    /* 创建Test_Queue */
//  *mbox = xQueueCreate((UBaseType_t ) size,/* 消息队列的长度 */
//                       (UBaseType_t ) sizeof(void *));/* 消息的大小 */
//  
//	if(NULL == *mbox)
//    return ERR_MEM;
//  
//  return ERR_OK;
//}

//void
//sys_mbox_free(sys_mbox_t *mbox)
//{
//  vQueueDelete(*mbox);
//}

//int sys_mbox_valid(sys_mbox_t *mbox)          
//{      
//  if (*mbox == SYS_MBOX_NULL) 
//    return 0;
//  else
//    return 1;
//}   

//void
//sys_mbox_set_invalid(sys_mbox_t *mbox)
//{
//  *mbox = SYS_MBOX_NULL; 
//}

//void
//sys_mbox_post(sys_mbox_t *q, void *msg)
//{
//  xQueueSend( *q, /* 消息队列的句柄 */
//              &msg,/* 发送的消息内容 */
//              portMAX_DELAY); /* 等待时间 */
//}

//err_t
//sys_mbox_trypost(sys_mbox_t *q, void *msg)
//{
//  if(xQueueSend(*q,&msg,0) == pdPASS)  
//    return ERR_OK;
//  else
//    return ERR_MEM;
//}

//err_t
//sys_mbox_trypost_fromisr(sys_mbox_t *q, void *msg)
//{
//  return sys_mbox_trypost(q, msg);
//}

//u32_t
//sys_arch_mbox_fetch(sys_mbox_t *q, void **msg, u32_t timeout)
//{
//  u32_t wait_tick = 0;
//  u32_t start_tick = 0 ;
//  
//  //看看信号量是否有效
//  if(*q == SYS_MBOX_NULL)
//  {
//    printf("[sys_arch]:SYS_MBOX_NULL!\n");
//    return SYS_ARCH_TIMEOUT;
//  }
//  //首先获取开始等待信号量的时钟节拍
//  start_tick = sys_now();
//  
//  //timeout != 0，需要将ms换成系统的时钟节拍
//  if(timeout != 0)
//  {
//    //将ms转换成时钟节拍
//    wait_tick = timeout / portTICK_PERIOD_MS;
//    if (wait_tick == 0)
//      wait_tick = 1;
//  }
//  //一直阻塞
//  else
//    wait_tick = portMAX_DELAY;
//  
//    //判断等待信号量的上下文环境是否在中断中
//  if(__get_IPSR() != 0 )
//  {
//    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
//    xQueueReceiveFromISR(*q,&(*msg),&pxHigherPriorityTaskWoken);
//    //如果需要的话进行一次任务切换，系统会判断是否需要进行切换
//    portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
//  }
//  else
//  {
//    //等待成功，计算等待的时间，否则就表示等待超时
//    if(xQueueReceive(*q,&(*msg), wait_tick) == pdTRUE)
//      return (sys_now() - start_tick);
//    else
//    {
//      *msg = NULL;
//      return SYS_ARCH_TIMEOUT;
//    }
//  }
//  return 0;
//}

//u32_t
//sys_arch_mbox_tryfetch(sys_mbox_t *q, void **msg)
//{
//  u32_t start_tick = 0 ;
//  
//  //看看信号量是否有效
//  if(*q == SYS_MBOX_NULL)
//    return SYS_MBOX_EMPTY;
//  
//  //首先获取开始等待信号量的时钟节拍
//  start_tick = sys_now();

//    //判断等待信号量的上下文环境是否在中断中
//  if(__get_IPSR() != 0 )
//  {
//    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
//    xQueueReceiveFromISR(*q,&(*msg),&pxHigherPriorityTaskWoken);
//    //如果需要的话进行一次任务切换，系统会判断是否需要进行切换
//    portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
//  }
//  else
//  {
//    //等待成功，计算等待的时间
//    if(xQueueReceive(*q,&(*msg), 0) == pdTRUE)
//      return (sys_now() - start_tick);
//    else
//      return SYS_MBOX_EMPTY;
//  }
//  return 0;
//}

//#if LWIP_NETCONN_SEM_PER_THREAD
//#error LWIP_NETCONN_SEM_PER_THREAD==1 not supported
//#endif /* LWIP_NETCONN_SEM_PER_THREAD */

//#endif /* !NO_SYS */
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 * Author: Stefano Oliveri <software@stf12.net>
 *
 * This file was modified by Stefano Oliveri to implement the system emulation layer for STR91x.
 *
 */

/* lwIP includes. */
#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"
#include "lwip/stats.h"
//#include "lwip/timers.h"
#include <lwip/opt.h>
#include <lwip/arch.h>
#if !NO_SYS
#include "sys_arch.h"
#endif
#include <lwip/stats.h>
#include <lwip/debug.h>
#include <lwip/sys.h>

#include <string.h>
struct sys_timeouts {
  struct sys_timeo *next;
};

int errno;

u32_t lwip_sys_now;

u32_t
sys_jiffies(void)
{
  if(__get_IPSR() != 0)
    lwip_sys_now = xTaskGetTickCountFromISR();
  else
    lwip_sys_now = xTaskGetTickCount();
  
  return lwip_sys_now;
}

u32_t
sys_now(void)
{
  if(__get_IPSR() != 0)
    lwip_sys_now = xTaskGetTickCountFromISR();
  else
    lwip_sys_now = xTaskGetTickCount();
  
  return lwip_sys_now;
}

struct timeoutlist
{
	struct sys_timeouts timeouts;
	xTaskHandle pid;
};

/* This is the number of threads that can be started with sys_thread_new() */
#define SYS_THREAD_MAX 4

static struct timeoutlist s_timeoutlist[SYS_THREAD_MAX];
static u16_t s_nextthread = 0;
int sys_mbox_valid(sys_mbox_t *mbox)
{
	return (*mbox != SYS_MBOX_NULL);
}

void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
	*mbox = SYS_MBOX_NULL;
}

void sys_sem_set_invalid(sys_sem_t *sem)
{
	*sem = SYS_SEM_NULL;
}

int sys_sem_valid(sys_sem_t* sem)
{
	return (*sem != SYS_SEM_NULL);
}
//  Creates an empty mailbox.
err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
	*mbox = xQueueCreate( size, sizeof( void * ) );
	#if SYS_STATS
      ++lwip_stats.sys.mbox.used;
      if (lwip_stats.sys.mbox.max < lwip_stats.sys.mbox.used) {
         lwip_stats.sys.mbox.max = lwip_stats.sys.mbox.used;
	  }
#endif /* SYS_STATS */
    if(0==*mbox)
    	return ERR_MEM;
	return ERR_OK;
}

/*-----------------------------------------------------------------------------------*/
/*
  Deallocates a mailbox. If there are messages still present in the
  mailbox when the mailbox is deallocated, it is an indication of a
  programming error in lwIP and the developer should be notified.
*/
void sys_mbox_free(sys_mbox_t* mbox)
{
	if( uxQueueMessagesWaiting( *mbox ) )
	{
		/* Line for breakpoint.  Should never break here! */
		portNOP();
#if SYS_STATS
	    lwip_stats.sys.mbox.err++;
#endif /* SYS_STATS */

		// TODO notify the user of failure.
	}

	vQueueDelete( *mbox );

#if SYS_STATS
     --lwip_stats.sys.mbox.used;
#endif /* SYS_STATS */
}

/*-----------------------------------------------------------------------------------*/
//   Posts the "msg" to the mailbox.
void sys_mbox_post(sys_mbox_t* mbox, void *data)
{
	xQueueSendToBack(*mbox, &data, portMAX_DELAY );
}


/*-----------------------------------------------------------------------------------*/
//   Try to post the "msg" to the mailbox.
err_t sys_mbox_trypost(sys_mbox_t* mbox, void *msg)
{
	err_t result;
	if ( xQueueSend( *mbox, &msg, 0 ) == pdPASS )
	{
      result = ERR_OK;
	}
	else {
      // could not post, queue must be full
      result = ERR_MEM;
#if SYS_STATS
      lwip_stats.sys.mbox.err++;
#endif /* SYS_STATS */
	}
	return result;
}

err_t
sys_mbox_trypost_fromisr(sys_mbox_t *q, void *msg)
{
  return sys_mbox_trypost(q, msg);
}

/*-----------------------------------------------------------------------------------*/
/*
  Blocks the thread until a message arrives in the mailbox, but does
  not block the thread longer than "timeout" milliseconds (similar to
  the sys_arch_sem_wait() function). The "msg" argument is a result
  parameter that is set by the function (i.e., by doing "*msg =
  ptr"). The "msg" parameter maybe NULL to indicate that the message
  should be dropped.

  The return values are the same as for the sys_arch_sem_wait() function:
  Number of milliseconds spent waiting or SYS_ARCH_TIMEOUT if there was a
  timeout.

  Note that a function with a similar name, sys_mbox_fetch(), is
  implemented by lwIP.
*/
u32_t sys_arch_mbox_fetch(sys_mbox_t* mbox, void **msg, u32_t timeout)
{
	void *dummyptr;
	portTickType StartTime, EndTime, Elapsed;
	StartTime = xTaskGetTickCount();
	if ( msg == NULL )
		msg = &dummyptr;
	if ( timeout != 0 )
	{
		if ( pdTRUE == xQueueReceive( *mbox, &(*msg), timeout / portTICK_RATE_MS ) )
		{
			EndTime = xTaskGetTickCount();
			Elapsed = (EndTime - StartTime) * portTICK_RATE_MS;
			if( Elapsed == 0 )
				Elapsed = 1;
			return ( Elapsed );
		}
		else // timed out blocking for message
		{
			*msg = NULL;
			return SYS_ARCH_TIMEOUT;
		}
	}
	else // block forever for a message.
	{
		while( pdTRUE != xQueueReceive( *mbox, &(*msg), portMAX_DELAY ) ); // time is arbitrary
		EndTime = xTaskGetTickCount();
		Elapsed = (EndTime - StartTime) * portTICK_RATE_MS;
		if(Elapsed == 0)
			Elapsed = 1;
		return ( Elapsed ); // return time blocked TODO test
	}
}

/*-----------------------------------------------------------------------------------*/
/*
  Similar to sys_arch_mbox_fetch, but if message is not ready immediately, we'll
  return with SYS_MBOX_EMPTY.  On success, 0 is returned.
*/
u32_t sys_arch_mbox_tryfetch(sys_mbox_t* mbox, void **msg)
{
	void *dummyptr;
	if ( msg == NULL )
		msg = &dummyptr;
   if ( pdTRUE == xQueueReceive( *mbox, &(*msg), 0 ) )
      return ERR_OK;
   else
      return SYS_MBOX_EMPTY;
}

err_t sys_mutex_new(sys_mutex_t* mutex)
{
	*mutex = xSemaphoreCreateMutex();
	if(0!=*mutex)
		return ERR_OK;
	else
		return ERR_MEM;
}

void sys_mutex_lock(sys_mutex_t* mutex)
{
	xSemaphoreTake(*mutex,portMAX_DELAY);
}

void sys_mutex_unlock(sys_mutex_t* mutex)
{
	xSemaphoreGive(*mutex);
}

/*-----------------------------------------------------------------------------------*/
//  Creates and returns a new semaphore. The "count" argument specifies
//  the initial state of the semaphore.
err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
	vSemaphoreCreateBinary((*sem));
	if( NULL == *sem)
	{
#if SYS_STATS
      ++lwip_stats.sys.sem.err;
#endif /* SYS_STATS */
		return ERR_MEM;
	}
	if(count == 0)
	{
		xSemaphoreTake(*sem,1);
	}
#if SYS_STATS
	++lwip_stats.sys.sem.used;
 	if (lwip_stats.sys.sem.max < lwip_stats.sys.sem.used) {
		lwip_stats.sys.sem.max = lwip_stats.sys.sem.used;
	}
#endif /* SYS_STATS */
	return ERR_OK;
}

/*-----------------------------------------------------------------------------------*/
/*
  Blocks the thread while waiting for the semaphore to be
  signaled. If the "timeout" argument is non-zero, the thread should
  only be blocked for the specified time (measured in
  milliseconds).

  If the timeout argument is non-zero, the return value is the number of
  milliseconds spent waiting for the semaphore to be signaled. If the
  semaphore wasn't signaled within the specified time, the return value is
  SYS_ARCH_TIMEOUT. If the thread didn't have to wait for the semaphore
  (i.e., it was already signaled), the function may return zero.

  Notice that lwIP implements a function with a similar name,
  sys_sem_wait(), that uses the sys_arch_sem_wait() function.
*/
u32_t sys_arch_sem_wait(sys_sem_t* sem, u32_t timeout)
{
	portTickType StartTime, EndTime, Elapsed;
	StartTime = xTaskGetTickCount();
	if(	timeout != 0)
	{
		if( xSemaphoreTake( *sem, timeout / portTICK_RATE_MS ) == pdTRUE )
		{
			EndTime = xTaskGetTickCount();
			Elapsed = (EndTime - StartTime) * portTICK_RATE_MS;
			if(Elapsed==0)
				Elapsed = 1;
			return (Elapsed); // return time blocked TODO test
		}
		else
		{
			return SYS_ARCH_TIMEOUT;
		}
	}
	else // must block without a timeout
	{
		while( xSemaphoreTake( *sem, portMAX_DELAY ) != pdTRUE );
		EndTime = xTaskGetTickCount();
		Elapsed = (EndTime - StartTime) * portTICK_RATE_MS;
		if(Elapsed == 0)
			Elapsed = 1;
		return ( Elapsed ); // return time blocked
	}
}

/*-----------------------------------------------------------------------------------*/
// Signals a semaphore
void sys_sem_signal(sys_sem_t* sem)
{
	xSemaphoreGive( *sem );
}

/*-----------------------------------------------------------------------------------*/
// Deallocates a semaphore
void sys_sem_free(sys_sem_t* sem)
{
#if SYS_STATS
      --lwip_stats.sys.sem.used;
#endif /* SYS_STATS */
      vSemaphoreDelete(*sem);
}

/*-----------------------------------------------------------------------------------*/
// Initialize sys arch
void sys_init(void)
{
	int i;
	// Initialize the the per-thread sys_timeouts structures
	// make sure there are no valid pids in the list
	for(i = 0; i < SYS_THREAD_MAX; i++)
	{
		s_timeoutlist[i].pid = 0;
		s_timeoutlist[i].timeouts.next = NULL;
	}
	// keep track of how many threads have been created
	s_nextthread = 0;
}

/*-----------------------------------------------------------------------------------*/
/*
  Returns a pointer to the per-thread sys_timeouts structure. In lwIP,
  each thread has a list of timeouts which is represented as a linked
  list of sys_timeout structures. The sys_timeouts structure holds a
  pointer to a linked list of timeouts. This function is called by
  the lwIP timeout scheduler and must not return a NULL value.

  In a single threaded sys_arch implementation, this function will
  simply return a pointer to a global sys_timeouts variable stored in
  the sys_arch module.
*/
struct sys_timeouts *sys_arch_timeouts(void)
{
	int i;
	xTaskHandle pid;
	struct timeoutlist *tl;
	pid = xTaskGetCurrentTaskHandle( );
	for(i = 0; i < s_nextthread; i++)
	{
		tl = &(s_timeoutlist[i]);
		if(tl->pid == pid)
		{
			return &(tl->timeouts);
		}
	}
	return NULL;
}

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
// TODO
/*-----------------------------------------------------------------------------------*/
/*
  Starts a new thread with priority "prio" that will begin its execution in the
  function "thread()". The "arg" argument will be passed as an argument to the
  thread() function. The id of the new thread is returned. Both the id and
  the priority are system dependent.
*/
sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
	xTaskHandle CreatedTask = NULL;
	int result;
    if ( s_nextthread < SYS_THREAD_MAX )
    {
      result = xTaskCreate( thread, ( signed portCHAR * ) name, stacksize, arg, prio, &CreatedTask );
	   if(result != pdPASS)
		    CreatedTask = NULL;
	   else
		   // For each task created, store the task handle (pid) in the timers array.
		   // This scheme doesn't allow for threads to be deleted
		   s_timeoutlist[s_nextthread++].pid = CreatedTask;

   }
   return CreatedTask;
}

/*
  This optional function does a "fast" critical region protection and returns
  the previous protection level. This function is only called during very short
  critical regions. An embedded system which supports ISR-based drivers might
  want to implement this function by disabling interrupts. Task-based systems
  might want to implement this by using a mutex or disabling tasking. This
  function should support recursive calls from the same task or interrupt. In
  other words, sys_arch_protect() could be called while already protected. In
  that case the return value indicates that it is already protected.

  sys_arch_protect() is only required if your port is supporting an operating
  system.
*/
sys_prot_t sys_arch_protect(void)
{
	vPortEnterCritical();
	return 1;
}

/*
  This optional function does a "fast" set of critical region protection to the
  value specified by pval. See the documentation for sys_arch_protect() for
  more information. This function is only required if your port is supporting
  an operating system.
*/
void sys_arch_unprotect(sys_prot_t pval)
{
	( void ) pval;
	vPortExitCritical();
}

