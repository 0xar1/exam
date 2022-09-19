#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
char buf[10],frameNum[10];
int sock_desc;
struct sockaddr_in client;
socklen_t len;

sock_desc = socket(AF_INET, SOCK_DGRAM, 0);
bzero(&client, sizeof(client));

client.sin_family = AF_INET;
client.sin_port = 5656;
client.sin_addr.s_addr = inet_addr("127.0.0.1");
len = sizeof(client);

int n, i=1 ; //i --> frame number
printf("Enter the number of frames\n");
scanf("%d",&n);
char delim[] = "ACK";


while(1)
{
strcpy(buf,"Frame");
sprintf(frameNum,"%d",i); //frame number to string
strcat(buf,frameNum);
sendto(sock_desc, buf, 10, 0, (struct sockaddr*)&client, len);
printf("%s send!!\n\n\n",buf);
if(i==n)
break;

recvfrom(sock_desc, buf, 10, 0, (struct sockaddr*)&client, &len);
char *ptr = strtok(buf, delim); //For eg, ptr points to 2 in ACK2
int check = atoi(ptr);
if(check == i+1)
{
printf("Acknowledgement got from Server : %s\n",buf);
printf("Acknowledgement success, next packet ready to sent\n");
++i;
}
else if(check == 0)
{
printf("Frame is Missing, resending frame%d\n",i);
continue;
}
}

close(sock_desc);
return 0;
}