
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"

#include "dns-netconn.h"

#if LWIP_NETCONN
#define MAX_BUFFER_LEN 256
char recvbuf[MAX_BUFFER_LEN];
char sendbuf[MAX_BUFFER_LEN];

static void dns_netconn_thread(void *arg)
{
    struct netconn *conn,*newconn = NULL;
	err_t ret = ERR_OK;
	conn = netconn_new(NETCONN_TCP);
	netconn_bind(conn,NULL,8080);
	netconn_listen(conn);
	
	while(1)
	{
		 ret = netconn_accept(conn, &newconn);
		 while(newconn != NULL)
		 {
		   struct netbuf *inbuf = NULL;
		   char *dataptr;
		   u16_t size;
		   ret = netconn_recv(newconn, &inbuf);
		   if(ret == ERR_OK)
		   {
			 ip_addr_t dnsaddr;
			 
			 netbuf_data(inbuf, (void*)&dataptr, &size);
			 if(size >= MAX_BUFFER_LEN)
			 {
                netbuf_delete(inbuf);
				continue;
			 }
			 
			 MEMCPY( recvbuf, dataptr, size);
			 recvbuf[size] = '\0';
			 netbuf_delete(inbuf);
			 
			 if ((ret = netconn_gethostbyname((char*)(recvbuf), &(dnsaddr))) == ERR_OK){
				u16_t strlen = sprintf(sendbuf,"%s = %s\n",recvbuf,ip_ntoa(&dnsaddr));
				if(strlen > 0)
				  netconn_write(newconn,sendbuf, strlen, NETCONN_COPY);
	
			 }
			 
		   }else{
		   
			netconn_close(newconn);
			netconn_delete(newconn);
			newconn = NULL;
		   }
		 }//while(newconn!=NULL)
	 }

}

void
dns_netconn_init()
{
  sys_thread_new("http_server_netconn", dns_netconn_thread, NULL, DEFAULT_THREAD_STACKSIZE, TCPIP_THREAD_PRIO+1);
}

#endif /* LWIP_NETCONN*/
