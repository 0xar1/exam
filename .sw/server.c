#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
char buf[10], ackNum[10], delim[] = "Frame";
int sock_desc;
struct sockaddr_in server, client;
socklen_t len;

sock_desc = socket(AF_INET, SOCK_DGRAM, 0);
bzero(&server, sizeof(server)); //instead of memset, it fills with 0

server.sin_family = AF_INET;
server.sin_port = 5656;
server.sin_addr.s_addr = inet_addr("127.0.0.1");

bind(sock_desc,(struct sockaddr*)&server, sizeof(server));
len = sizeof(client);

int n,lostFrames;
printf("Enter the number of frames\n");
scanf("%d",&n);
printf("Enter number of lost frames\n");
scanf("%d",&lostFrames);

int lost[lostFrames];
printf("Enter lost frames\n");
for(int i=0; i<lostFrames; ++i)
scanf("%d",&lost[i]);

int i = 2;
while(i <= n)
{
int flag = 0;
recvfrom(sock_desc, buf, sizeof(buf), 0, (struct sockaddr*)&client,&len);
char *ptr = strtok(buf,delim);
for(int j=0; j<lostFrames; ++j)
{
int check = atoi(ptr);
if(lost[j] == check) //checking frame is in lostframe or not
{
strcpy(buf,"ACK0");
lost[j] = 0;
++flag;
break;
}
}

if(flag == 0) //frame not in lost frame, so send ack
{
strcpy(buf,"ACK");
sprintf(ackNum,"%d",i++); //ack number to string
strcat(buf,ackNum);
}
sendto(sock_desc, buf, 10, 0, (struct sockaddr*)&client, len);
printf("Acknowledgment sent : %s\n\n",buf);
}

close(sock_desc);
}