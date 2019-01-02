#include "stats.h"
#include "tcp.h"
#include <string.h>
#include <stdio.h>

void tcp_echoserver_init(void);
static err_t tcp_echoserver_accept(void *arg, struct tcp_pcb *newpcb,err_t err);
static err_t tcp_echoserver_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p,err_t err);
static struct tcp_pcb *tcp_echoserver_pcb;

void tcp_echoserver_init(void)
{
  err_t err;
  
  /* Create a new TCP control block  */
  tcp_echoserver_pcb = tcp_new();
  
  if(tcp_echoserver_pcb !=NULL)
  {
    /* Assign to the new pcb a local IP address and a port number */
    /* Using IP_ADDR_ANY allow the pcb to be used by any local interface */
    err = tcp_bind(tcp_echoserver_pcb,IP_ADDR_ANY,6060);
    if(err == ERR_OK)
    {
      /* Set the connection to the LISTEN state */	
      tcp_echoserver_pcb = tcp_listen(tcp_echoserver_pcb);
      
      /* Specify the function to be called when a connection is established */	
      tcp_accept(tcp_echoserver_pcb,tcp_echoserver_accept);
    }
    else
    {
      memp_free(MEMP_TCP_PCB, tcp_echoserver_pcb);
    }
  }
}
static err_t tcp_echoserver_accept(void *arg, struct tcp_pcb *newpcb,err_t err)
{
  /* initialize lwip tcp_recv callback function for newpcb */
  tcp_recv(newpcb, tcp_echoserver_recv);
  return ERR_OK;
}

static err_t tcp_echoserver_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p,err_t err)
{
  tcp_write(tpcb,p->payload,p->len,1);
  pbuf_free(p);return ERR_OK;
}
