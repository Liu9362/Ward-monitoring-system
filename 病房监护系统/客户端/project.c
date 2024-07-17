#include "project.h"
int fd;
char arg[2][128];
ipc_msg_t  clientmsg;
pthread_t tid;
struct sockaddr_in server_addr;

void task_func(int fd){
    int nbytes=0;
    char buff[16]={0};
    while(1){
    if(-1 == (nbytes = recv(fd, buff, sizeof(buff), 0)))
        printf("recv error");
    memset(buff,0,sizeof(buff));
    sprintf(buff,"ACK");
    if(-1 == (nbytes = send(fd, buff, sizeof(buff), 0)))
        printf("recv error");
    }
}

/*读取配置文件*/
int read_config(ipc_msg_t *buf)
{
	char data[128] = {0};
	char *s = NULL;
	char id[20] = {0};
	int i = 0;
	FILE *fp = fopen(CONFIGPATH,"r");
	if(NULL==fp){
		puts("打开文件失败");
		return -1;
	}

	while(fgets(data,128,fp)){
		/*获取 IP 和 端口号*/
		if(strstr(data,"ip")){
			s = data;
			memset(id,0,20);
			while(*(s++) != '{');
			i = 0;
			while(*s != ','){
				arg[0][i] = *s;
				i++;s++;
			}
			arg[0][i] = '\0';
			printf("ip:%s\n",arg[0]);
			i=0;s++;
			memset(id,0,20);
			while(*s != '}'){
				arg[1][i] = *s;
				i++;s++;
			}
			arg[1][i] = '\0';
			printf("prot:%s\n",arg[1]);
			memset(data,0,128);
			continue;
		}
		/*获取登录ID*/
		if(strstr(data,"login")){
			s = data;
			while(*(s++)!='{');
			i=0;
			while(*s!=','){
				buf->log.ID[i] = *s;
				i++;s++;
			}
			buf->log.ID[i] = '\0';
			printf("id:%s\n",buf->log.ID);
			i=0;
			s++;
			while(*s != '}'){
				buf->log.PS[i] = *s;
				i++;s++;
			}
			buf->log.PS[i] = '\0';
			printf("ps:%s\n",buf->log.ID);
			continue;
		}
		memset(data,0,sizeof(data));

	}
	fclose(fp);
	puts("读取文件完成");
	return 0;
}

/*获取环境数据线程*/
void *envgetthread(void *argv)
{
	ipc_msg_t buf;
	memset(&buf,0,sizeof(ipc_msg_t));
	buf.env.temp = 30.2;
	buf.env.hume = 55;
	buf.env.lux = 350;
	buf.env.devstart = 0;
	send(fd,&buf,sizeof(ipc_msg_t),0);
	puts("环境数据返回");
	pthread_exit(NULL);
}

/*设置环境阈值线程*/
void *envsetthread(void *argv)
{

}

/*设备控制线程*/
void *devctlthread(void *argv)
{

}

/*根据用户阈值环境维护线程*/
void *holdenvthread(void *argv)
{
	while(1){
		sleep(1);
	}
}