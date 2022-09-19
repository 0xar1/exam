#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<fcntl.h>

int main()
{
char buf[100], temp[100], fname[20];
FILE *fp;
int k, sock_desc;
struct sockaddr_in client;

sock_desc = socket(AF_INET, SOCK_STREAM, 0);
if(sock_desc == -1)
printf("Error in socket creation\n");

bzero(&client, sizeof(client));
client.sin_family = AF_INET;
client.sin_port = 5050;
client.sin_addr.s_addr = inet_addr("127.0.0.1");

k = connect(sock_desc, (struct sockaddr*)&client, sizeof(client));
if(k == -1)
printf("Error in socket connection\n");

while(1)
{
printf("Enter the command\n");
fgets(buf,100,stdin);

if(strncmp(buf,"bye",3) == 0)
{
send(sock_desc, buf, sizeof(buf),0);
break;
}
char *token = strtok(buf, " ");
token = strtok(NULL, "\n");
strcpy(fname, token);
send(sock_desc, buf, sizeof(buf),0);            //if input is get hello.txt, now buf = get

if(strncmp(buf,"get",3)==0)
{
send(sock_desc, fname, sizeof(fname), 0);
recv(sock_desc, buf, sizeof(buf), 0);                  //checks if file is present or not

if(strncmp(buf, "notfound", 8) != 0)
{
fp = fopen(fname, "w");
while(1)
{
recv(sock_desc, buf, sizeof(buf), 0);
if(strncmp(buf,"completed",9) == 0)
break;
fputs(buf, fp);
}
printf("file %s downloaded from server\n\n",fname);
fclose(fp);
}
}
else if(strncmp(buf,"put",3)==0)
{
send(sock_desc, fname, sizeof(fname), 0);
fp = fopen(fname, "r");
if(fp == NULL)
{
printf("Filename %s not found\n\n",fname);
send(sock_desc, "notfound", 8, 0);
}
else
{
send(sock_desc, "found", 5, 0);
while(fgets(buf, sizeof(buf), fp))
{
send(sock_desc, buf, sizeof(buf), 0);
}
send(sock_desc, "completed", 9, 0);
printf("File %s uploaded successfully\n\n",fname);
}
fclose(fp);
}
else
printf("Invalid input\n\n");
}
close(sock_desc);
close(k);
return 0;
}