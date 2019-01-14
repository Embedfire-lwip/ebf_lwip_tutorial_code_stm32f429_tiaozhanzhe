
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

char zrxbuf[1024];


char msg[] = "hello, you are connected!\n";

static void tcp_client_thread(void * arg)
{
	struct sockaddr_in server_addr;
	int sock_fd;
	int ret;
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1) {
      printf("failed to create sock_fd!\n");
	}
	
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("192.168.1.112");
	server_addr.sin_port = htons(SOCK_TARGET_PORT);
	
	ret = connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
  printf("socket connect result [%d]\n",ret);
  
	while (1) {
		send(sock_fd, (char *)msg, sizeof(msg), 0);
		sys_msleep(1000);
	}
	
	close(sock_fd);
}


/** This is an example function that tests
    the recv function (timeout etc.). */
char rxbuf[1024];
char sndbuf[64];
static void socket_server(void *arg)
{
  int sock = -1,connected,bytes_received;
  int ret;
  int opt;
  size_t len;
  uint8_t *recv_data;
  fd_set readset;
  socklen_t sin_size;
  struct sockaddr_in server_addr,client_addr;
  struct timeval timeout;
  
  LWIP_UNUSED_ARG(arg);
  
  
  recv_data = (uint8_t *)pvPortMalloc(SOCK_BUFSZ);
  if (recv_data == NULL)
  {
      printf("No memory\n");
      goto __exit;
  }
  
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
  {
      printf("Socket error\n");
      goto __exit;
  }
  
  /* set up address to connect to */
//  memset(&server_addr, 0, sizeof(server_addr));
//  addr.sin_len = sizeof(addr);
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(5001);
  server_addr.sin_addr.s_addr = INADDR_ANY;
//  memset(&(server_addr.sin_zero), 0x0, sizeof(server_addr.sin_zero));

  if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
  {
    printf("Unable to bind\n");
    goto __exit;
  }
  
  if (listen(sock, 5) == -1)
  {
      printf("Listen error\n");
      goto __exit;
  }
  
  timeout.tv_sec = 1;
  timeout.tv_usec = 0;
  
  while(1)
  {
//    FD_ZERO(&readset);
//    FD_SET(sock, &readset);
//    printf("ABC\n");
//    if (select(sock + 1, &readset, NULL, NULL, &timeout) == 0)
//      continue;
    
    sin_size = sizeof(struct sockaddr_in);
    
    connected = accept(sock, (struct sockaddr *)&client_addr, (socklen_t *)&sin_size);
    
    printf("new client connected from (%s, %d)\n",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    {
      int opt = 100;

      setsockopt(connected,
                 SOL_SOCKET,     /* set option at TCP level */
                 SO_RCVTIMEO,     /* name of option */
                 (void *) &opt,  /* the cast is historical cruft */
                 sizeof(int));    /* length of option value */
    }
    
    while(1)
    {
      if(read(connected, recv_data, SOCK_BUFSZ) != -1)  
      {
        write(connected,recv_data,SOCK_BUFSZ);
        printf("%s\n",recv_data);
      }
      else
      {
        printf("err");
        
      }
      
    }
    
    
  }
  
__exit:
  if (sock >= 0) closesocket(sock);
  if (recv_data) free(recv_data);
  printf("close\n");
}





uint8_t recv_data[100];

static void tcp_server_thread(void *p_arg)
{
	struct sockaddr_in server_addr;
	struct sockaddr_in conn_addr;
	int sock_fd;				/* server socked */
	int sock_conn;			/* request socked */
	socklen_t addr_len;
	int err;
	int length;
	int count = 0;
//  uint8_t *recv_data;
  
//  recv_data = (uint8_t *)pvPortMalloc(SOCK_BUFSZ);
//  if (recv_data == NULL)
//  {
//      printf("No memory\n");
//  }
	
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1) {
		printf("failed to create sock_fd!\n");
	}
	
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr =htonl(INADDR_ANY);
	server_addr.sin_port = htons(5001);
	
	err = bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (err < 0) {
		 printf("Unable to bind\n");
	}
	
	err = listen(sock_fd, 1);
	if (err < 0) {
		 printf("Unable to listen\n");
	}

	addr_len = sizeof(struct sockaddr_in);
	
	printf("before accept!\n");
	sock_conn = accept(sock_fd, (struct sockaddr *)&conn_addr, &addr_len);
	printf("after accept!\n");
	
	while (1) {
		memset(recv_data, 0, sizeof(SOCK_BUFSZ));
		
		length = recv(sock_conn, (unsigned int *)recv_data, 20, 0);
		
		printf("length received %d\n", length);
		printf("received string: %s\n", recv_data);
		printf("received count: %d\n", count);

		send(sock_conn, "good", 5, 0);
	}
}

void tcp_server_init(void)
{
	sys_thread_new("tcp_server_thread",  tcp_server_thread, NULL, 2048, 1);
}




void socket_examples_init(void)
{
  tcp_server_init();
//  sys_thread_new("tcp_client", tcp_client_thread, NULL, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);
  //sys_thread_new("socket_nonblocking", socket_nonblocking, NULL, 0, TCPIP_THREAD_PRIO+2);
//  sys_thread_new("socket_timeoutrecv", socket_server, NULL, 1024, TCPIP_THREAD_PRIO+1);
}


