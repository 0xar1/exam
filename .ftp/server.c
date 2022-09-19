#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include<fcntl.h>

int main()
{
FILE *fp;
char buf[100], fname[20];
int k, sock_desc, temp_sock_desc;

struct sockaddr_in server, client;
socklen_t len;

sock_desc = socket(AF_INET, SOCK_STREAM, 0);
bzero(&server, sizeof(server));

server.sin_family = AF_INET;
server.sin_port = 5050;
server.sin_addr.s_addr = inet_addr("127.0.0.1");

bind(sock_desc, (struct sockaddr*)&server, sizeof(server));
listen(sock_desc, 5);
len = sizeof(client);
temp_sock_desc = accept(sock_desc, (struct sockaddr*)&client, &len);
while(1)
{
recv(temp_sock_desc, buf, 100, 0);                    //recieves command : get, put or bye
if(strncmp(buf, "get", 3) == 0)
{
recv(temp_sock_desc, fname, 20, 0);
fp = fopen(fname,"r");
if(fp == NULL)
{
printf("Filename %s not found\n\n",fname);
send(temp_sock_desc, "notfound", 8, 0);
}
else
{
send(temp_sock_desc, "found", 5, 0);
while(fgets(buf, sizeof(buf), fp))
{
send(temp_sock_desc, buf, sizeof(buf), 0);
}
printf("File %s transferred to client\n\n", fname);
send(temp_sock_desc, "completed", 9, 0);
}
fclose(fp);
}

else if(strncmp(buf, "put", 3) == 0)
{
recv(temp_sock_desc, fname, sizeof(fname), 0);
recv(temp_sock_desc, buf, sizeof(buf), 0);                //recieves file not found or not
if(strncmp(buf, "notfound", 8) != 0)
{
fp = fopen(fname, "w");
while(1)
{
recv(temp_sock_desc, buf, sizeof(buf), 0);
if(strncmp(buf,"completed",9) == 0)
break;
fputs(buf, fp);
}
printf("File %s recieved from client\n\n",fname);
fclose(fp);
}}

else if(strncmp(buf,"bye" ,3)==0)
break;
}
close(temp_sock_desc);
return 0;
}