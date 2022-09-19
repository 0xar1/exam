#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

int main()
{
char buf[100];
int sock_desc;
struct sockaddr_in server, client;
socklen_t len;

sock_desc = socket(AF_INET, SOCK_DGRAM, 0);
bzero(&server, sizeof(server));               //instead of memset, it fills with 0 default

server.sin_family = AF_INET;
server.sin_port = 5656;
server.sin_addr.s_addr = inet_addr("127.0.0.1");

bind(sock_desc,(struct sockaddr*)&server, sizeof(server));
len = sizeof(client);

while(1)
{
recvfrom(sock_desc, buf, sizeof(buf), 0, (struct sockaddr*)&client,&len);
printf("Message got from Client : %s",buf);
if(strncmp(buf,"end",3)==0)
break;
printf("Enter data to be send to Client : ");
fgets(buf, 100, stdin);

sendto(sock_desc, buf, 100, 0, (struct sockaddr*)&client, len);
if(strncmp(buf,"end",3)==0)
break;
}

close(sock_desc);
}
