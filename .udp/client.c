#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

int main()
{
char buf[100];
int sock_desc;
struct sockaddr_in client;
socklen_t len;

sock_desc = socket(AF_INET, SOCK_DGRAM, 0);
bzero(&client, sizeof(client));

client.sin_family = AF_INET;
client.sin_port = 5656;
client.sin_addr.s_addr = inet_addr("127.0.0.1");

len = sizeof(client);

while(1)
{
printf("Enter data to be send to Server : ");
fgets(buf,100,stdin);

sendto(sock_desc, buf, 100, 0, (struct sockaddr*)&client, len);
if(strncmp(buf,"end",3) == 0)
break;
recvfrom(sock_desc, buf, 100, 0, (struct sockaddr*)&client, &len);
printf("Message got from Server : %s",buf);
if(strncmp(buf,"end",3) == 0)
break;
}

close(sock_desc);
return 0;
}