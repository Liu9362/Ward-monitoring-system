#include "project.h"
ipc_msg_t msg;
int sockfd;
extern ipc_msg_t clientmsg;
extern char arg[2][128];
struct sockaddr_in server_addr;
extern pthread_t tid;
int main(int argc, char const *argv[])
{
    /*读取环境设置配置文件，用于保持环境线程初始化使用*/
    if (read_config(&clientmsg))
    {
        puts("读取文件失败");
        return -1;
    }
    /*根据用户阈值环境维护线程*/
    pthread_create(&tid, NULL, holdenvthread, NULL);

    /*网络初始化*/
    int ret = 0;
    if (0 == (sockfd = socket(AF_INET, SOCK_STREAM, 0)))
        ERR_LOG("socket error");
    server_addr.sin_addr.s_addr = inet_addr(arg[0]);
    server_addr.sin_port = htons(atoi(arg[1]));
    server_addr.sin_family = AF_INET;
    socklen_t server_addr_len = sizeof(server_addr);

    /*连接服务器*/
    printf("等待连接\n");
    if (-1 == connect(sockfd, (struct sockaddr *)&server_addr, server_addr_len))
        ERR_LOG("connect error");
    printf("连接并发送\n");
    /*登录*/
    if (-1 == (ret = send(sockfd, &clientmsg, sizeof(ipc_msg_t), 0)))
        ERR_LOG("send error");
    printf("111121/n");

    ipc_msg_t threadmsg;
    // 数据处理
    while (1)
    {
        memset(&clientmsg, 0, sizeof(ipc_msg_t));
        if (0 == recv(sockfd, &clientmsg, sizeof(ipc_msg_t), 0))
        {
            return -1;
        }
        if (clientmsg.log.flags)
        {
            threadmsg = clientmsg;
            switch (threadmsg.commd)
            {
            case 1:
                puts("获取环境数据指令到来");
                pthread_create(&tid, NULL, envgetthread, NULL);
                pthread_detach(tid);
                break;
            case 2:
                puts("设置阈值指令到来");
                pthread_create(&tid, NULL, envsetthread, &threadmsg);
                pthread_detach(tid);
                break;
            case 3:
                puts("控制设备指令到来");
                pthread_create(&tid, NULL, devctlthread, &threadmsg);
                pthread_detach(tid);
                break;
            case 255:
                puts("心跳包到来");
                send(sockfd, &clientmsg, sizeof(ipc_msg_t), 0);
                break;
            default:
                puts("未知指令到来");
                send(sockfd, &clientmsg, sizeof(ipc_msg_t), 0);
                break;
            }
        }
        else
        {
            close(sockfd);
            return -1;
        }
    }
    return 0;
}