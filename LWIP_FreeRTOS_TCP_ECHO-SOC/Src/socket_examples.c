
#include "socket_examples.h"

#include "lwip/opt.h"


#include "lwip/sockets.h"
#include "lwip/sys.h"

#include <string.h>
#include <stdio.h>

#ifndef SOCK_TARGET_HOST
#define SOCK_TARGET_HOST  "192.168.1.112"
#endif

#ifndef SOCK_TARGET_PORT
#define SOCK_TARGET_PORT  5001
#endif

#define SOCK_BUFSZ         (4 * 1024)



static void tcp_client_thread(void * arg)
{
	struct sockaddr_in server_addr;
	int sock_fd;
	int ret;
  char msg[] = "hello, you are connected!\n";
  
  while(1)
  {
    sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_fd == -1) {
        printf("failed to create sock_fd!\n");
        continue;
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SOCK_TARGET_HOST);
    server_addr.sin_port = htons(SOCK_TARGET_PORT);
    
    ret = connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
    printf("socket connect result [%d]\n",ret);
    
    while (1) {
      send(sock_fd, (char *)msg, sizeof(msg), 0);
      sys_msleep(1000);
    }
	}

}


static void tcp_server_thread(void *p_arg)
{
	struct sockaddr_in server_addr;
	struct sockaddr_in conn_addr;
	int sock_fd;				/* server socked */
	int sock_conn;			/* request socked */
	socklen_t addr_len;
	int err;
	int length;
  uint8_t *recv_data;
  
  recv_data = (uint8_t *)pvPortMalloc(SOCK_BUFSZ);
  if (recv_data == NULL)
  {
      printf("No memory\n");
      goto __exit;
  }
	
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1) {
		printf("failed to create sock_fd!\n");
    goto __exit;
	}
	
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr =htonl(INADDR_ANY);
	server_addr.sin_port = htons(5001);
	
	err = bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (err < 0) {
		printf("Unable to bind\n");
    goto __exit;
	}
	
	err = listen(sock_fd, 1);
	if (err < 0) {
		printf("Unable to listen\n");
    goto __exit;
	}

	addr_len = sizeof(struct sockaddr_in);
	
	printf("before accept!\n");
	sock_conn = accept(sock_fd, (struct sockaddr *)&conn_addr, &addr_len);
	printf("after accept!\n");
	
	while (1) {
    
		memset(recv_data, 0, sizeof(SOCK_BUFSZ));
		
		if((length = recv(sock_conn, recv_data, SOCK_BUFSZ, 0)) != -1)
		{
      printf("length received %d\n", length);
      printf("received string: %s\n", recv_data);
      printf("sizeof(recv_data): %d\n", strlen((char*)recv_data));
      send(sock_conn, recv_data, strlen((char*)recv_data), 0);
    }
	}
  
__exit:
  if (sock_fd >= 0) closesocket(sock_fd);
  if (recv_data) free(recv_data);
  printf("close\n");
  
}

static void tcp_server_init(void)
{
	sys_thread_new("tcp_server_thread",  tcp_server_thread, NULL, TCPECHO_THREAD_STACKSIZE, TCPECHO_THREAD_PRIO);
}

static void tcp_client_init(void)
{
	sys_thread_new("tcp_client_thread",  tcp_client_thread, NULL, TCPECHO_THREAD_STACKSIZE, TCPECHO_THREAD_PRIO);
}


void socket_examples_init(int mode)
{
  if(mode == TCP_SERVER_MODE)
    tcp_server_init();
  else if(mode == TCP_CLIENT_MODE)
    tcp_client_init();
  else
    printf("socket examples init failed\n");
}


