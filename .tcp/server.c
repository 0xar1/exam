#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

int main()
{
char buf[100];
int k, sock_desc, temp_sock_desc;
socklen_t len;
struct sockaddr_in server, client;
memset(&server, 0, sizeof(server));
memset(&client, 0, sizeof(client));

sock_desc = socket(AF_INET, SOCK_STREAM, 0);

server.sin_family = AF_INET;
server.sin_port = 5500;
server.sin_addr.s_addr = inet_addr("127.0.0.1");

bind(sock_desc,(struct sockaddr*)&server, sizeof(server));
listen(sock_desc, 20);
len = sizeof(client);

temp_sock_desc = accept(sock_desc, (struct sockaddr*)&client, &len);

while(1)
{
recv(temp_sock_desc, buf, 100, 0);
printf("Message got from client is : %s",buf);
if(strncmp(buf,"end",3)==0)
break;

printf("Enter data to be send to client: ");
fgets(buf, 100, stdin);
send(temp_sock_desc, buf, 100, 0);
if(strncmp(buf,"end",3)==0)
break;
}
close(temp_sock_desc);
return 0;
}