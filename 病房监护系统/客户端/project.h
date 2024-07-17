#ifndef __PROJECT_H__
#define __PROJECT_H__
#include<stdio.h>
#include<error.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sqlite3.h>
#include<unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

#define GETENV 1                                                                                 
#define SETENV 2                                                                                 
#define DEVCTL 3   

#define CONFIGPATH "./Iotconf"
#define MAXLINK 10

#define ERR_LOG(msg) do{\
    printf("%s :%s :%d\n",__FILE__,__func__,__LINE__);  \
    perror(msg);  \
    return -1;  \
}while(0);  

void task_func(int fd);

typedef struct{
    char ID[20];
    char PS[10];
    char flags;
}login_t;

typedef struct{
	float temp;
	unsigned char hume;
	unsigned short lux;
	unsigned char devstart;
} envdata_t;

typedef struct{
	float temp_up;
	float temp_down;
	unsigned char hume_up;
	unsigned char hume_down;
	unsigned short lux_up;
	unsigned short lux_down;

} limitset_t;

typedef struct{
	long long msgtype;
	long long recvmsgtype;
	/*登录信息*/
	login_t log;
	
	/*环境数据*/
	envdata_t env;

	/*阈值数据*/
	limitset_t setdata;
	/*控制数据*/
	unsigned char devctl;

	/*指令数据*/
	unsigned char commd;
} ipc_msg_t;

/*读取配置文件*/
int read_config(ipc_msg_t *buf);

/*获取环境数据线程*/
void *envgetthread(void *argv);

/*设置环境阈值线程*/
void *envsetthread(void *argv);

/*设备控制线程*/
void *devctlthread(void *argv);

/*根据用户阈值环境维护线程*/
void *holdenvthread(void *argv);
#endif