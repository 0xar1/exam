#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

int main()
{
char buf[100];
int k, sock_desc;
struct sockaddr_in client;
memset(&client, 0, sizeof(client));

sock_desc = socket(AF_INET, SOCK_STREAM, 0);
if(sock_desc == -1)
printf("Error in socket creation\n");

client.sin_family = AF_INET;
client.sin_port = 5500;
client.sin_addr.s_addr = inet_addr("127.0.0.1");

k = connect(sock_desc, (struct sockaddr*)&client, sizeof(client));
if(k == -1)
printf("Error in socket connection\n");

while(1)
{
printf("Enter data to be send to server: ");
fgets(buf,100,stdin);

send(sock_desc, buf, 100, 0);
if(strncmp(buf,"end",3)==0)
break;

recv(sock_desc, buf, 100, 0);
printf("Message got from Server is : %s",buf);
if(strncmp(buf,"end",3)==0)
break;
}

close(sock_desc);
return 0;
}