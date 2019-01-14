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
 *
 */
#include "tcpecho.h"

#include "lwip/opt.h"
#include "lwip.h"
#if LWIP_NETCONN

#include "lwip/sys.h"
#include "lwip/api.h"

#define IPERF_PORT          5001
#define IPERF_BUFSZ         (4 * 1024)

#define IPERF_MODE_STOP     0
#define IPERF_MODE_SERVER   1
#define IPERF_MODE_CLIENT   2

int mode;

/*-----------------------------------------------------------------------------------*/

static void 
iperf_server(void *arg)
{
  struct netconn *conn, *newconn;
  struct netbuf *buf;
  err_t err;
  u16_t len;
  u32_t tick1,tick2;
  uint64_t recvlen;
  
  LWIP_UNUSED_ARG(arg);

  /* Create a new connection identifier. */
  /* Bind connection to well known port number 7. */
#if LWIP_IPV6
  conn = netconn_new(NETCONN_TCP_IPV6);
  netconn_bind(conn, IP6_ADDR_ANY, LOCAL_PORT);
#else /* LWIP_IPV6 */
  conn = netconn_new(NETCONN_TCP);
  netconn_bind(conn, IP_ADDR_ANY, LOCAL_PORT);
#endif /* LWIP_IPV6 */
  LWIP_ERROR("tcpecho: invalid conn", (conn != NULL), return;);

  /* Tell connection to go into listening mode. */
  netconn_listen(conn);
  /* Grab new connection. */
  err = netconn_accept(conn, &newconn);
  recvlen = 0;
  
  tick1 = sys_now();
  while (1) 
    {
      if(netconn_recv(newconn, &buf)== ERR_OK)
      {
  //      netbuf_data(buf, &data, &len);
  //      err = netconn_write(newconn, data, len, NETCONN_NOCOPY);
        len = netbuf_len(buf);
          
        if (len <= 0) break;
      
        recvlen += len; 
    
        tick2 = sys_now();
   
        if(tick2 - tick1 >= configTICK_RATE_HZ * 5)
        {     
          float f;

          f = (float)(recvlen*configTICK_RATE_HZ/(tick2 - tick1)/1000);
          f /= 1000.0f;
          printf("recv speed = %.4f MBytes!\n", f);
          
          tick1 = tick2;
          recvlen = 0;
        }
       netbuf_delete(buf);
      }
    }
    netconn_close(newconn);
    netconn_delete(newconn);
}

static void iperf_client(void *thread_param)
{
  struct netconn *conn, *newconn;
  struct netbuf *buf;
  int i;
  int sock;
  int ret;

  uint8_t *send_buf;
  
  uint64_t sentlen;
  
  u32_t tick1, tick2;
  ip4_addr_t ipaddr;

  char speed[32] = { 0 };

  send_buf = (uint8_t *) pvPortMalloc(IPERF_BUFSZ);
  if (!send_buf) return ;

  for (i = 0; i < IPERF_BUFSZ; i ++)
      send_buf[i] = i & 0xff;
  
  while(1)
  {
    conn = netconn_new(NETCONN_TCP);
    if (conn < 0)
    {
        printf("create conn failed!\n");
        vTaskDelay(10);
        continue;
    }
    IP4_ADDR(&ipaddr,192,168,1,112);
    ret = netconn_connect(conn,&ipaddr,5001);
    if (ret == -1)
    {
        printf("Connect failed!\n");
        netconn_close(conn);

        vTaskDelay(10);
        continue;
    }
    
    printf("Connect to iperf server successful!\n");
    tick1 = sys_now();
    mode = IPERF_MODE_CLIENT;
    while (mode != IPERF_MODE_STOP)
    {
      tick2 = sys_now();
      
      if(tick2 - tick1 >= configTICK_RATE_HZ * 5)
      {     
        float f;
        f = (float)(sentlen*configTICK_RATE_HZ/125/(tick2 - tick1));
        
        f /= 1000.0f;
        printf("send speed = %.4f Mbps!\n", f);
        
        tick1 = tick2;
        sentlen = 0;
      }
      ret = netconn_write(conn,send_buf,IPERF_BUFSZ,0);
      if (ret == ERR_OK)
      {
          sentlen += IPERF_BUFSZ;
      }

    }
    netconn_close(conn);
    netconn_delete(conn);
  }
}

/*-----------------------------------------------------------------------------------*/
void
iperf_server_init(void)
{
  mode = IPERF_MODE_CLIENT;
  
//  sys_thread_new(IPERF_SERVER_THREAD_NAME, iperf_server, NULL, IPERF_SERVER_THREAD_STACKSIZE, IPERF_SERVER_THREAD_PRIO);
  sys_thread_new(IPERF_SERVER_THREAD_NAME, iperf_client, NULL, IPERF_SERVER_THREAD_STACKSIZE, IPERF_SERVER_THREAD_PRIO);
}
/*-----------------------------------------------------------------------------------*/

#endif /* LWIP_NETCONN */
