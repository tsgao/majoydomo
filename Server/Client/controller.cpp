#include <string.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>
#include <pthread.h>
using namespace std;
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
struct data {
    int* socket;
    int* xbee;
};
int16_t * getdata(int &xbee_module){
    int16_t temp;
    int16_t * xbee = new int16_t[30];
    
    
    read(xbee_module, &temp, 1); // Length first bit ignored
    read(xbee_module, &temp, 1); // length max 0xff
    xbee[0]=temp;
    int length = temp;
    
    for(int i=1;i<length+2;i++){
        if(read(xbee_module, &temp, 1)!=0)
            xbee[i]= temp;
    }
    /*
    for(int i=0;i<30;i++){
        if(read(xbee_module, &temp, 1)!=0){
            printf("%x ", temp);
        }
    }*/
    return xbee;
}
char calculate_sum(char* buffer){
        long sum =0;
        for(int i=3;i<buffer[2]+3;i++)
                sum+= buffer[i];
        return 0xff- (sum & 0xff);
}
void turn_OFF(int &port){
        char buf[20] = {0x7E, 0x00,0x10,0x17,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFE,0x02,0x44,0x33,0x04,0x00};
        buf[19] = calculate_sum(buf);
        write(port, buf, 20);
}

void turn_ON(int &port){
        char buf[20] = {0x7E, 0x00,0x10,0x17,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFE,0x02,0x44,0x33,0x05,0x00};
        buf[19] = calculate_sum(buf);
        write(port, buf, 20);
}

void *send(void *input)
{
    struct data * args = (struct data *)input;
    
	int listenFd = *(int *)args->socket;
    int fd = *(int *)args->xbee;
    
    
    int16_t * xbee = new int16_t[30];
    memset (xbee,'\0',30);
    int16_t temp;
    int counter = 0;
    
    
    // Waiting to login.
	for(int i=0;i<2;i++)
    {
        char s[3000];
        memset (s,'\0',3000);
        cin.getline(s, 500);
        int length = strlen(s);
        write(listenFd, s, strlen(s)+1);
    }
    // Read Xbee and send to socket
    for(;;){
        if(read(fd, &temp, 1)!=0){
            if(temp == 0x7e)
                xbee = getdata(fd);
            /*
            for(int i = 0;i< xbee[0]+2;i++){
                printf("%x ", xbee[i]);
                if(i==xbee[0]+1)
                    printf("\n");
            }*/
            if(xbee[0]==0x12){
                if(xbee[xbee[0]]!=0x0)
                    write(listenFd, "* On", sizeof("* On"));
                else
                    write(listenFd, "* Off", sizeof("* Off"));
            }
            /*
            if(temp == 0x7e){
                for(int i=0;i<30;i++){
                    printf("%x ", xbee[i]);
                    fflush(stdout);
                }
                counter = 0;
                memset(xbee,0,30);
                xbee[counter]=temp;
                //printf("\n");
            }else if(counter<28){
                counter++;
                xbee[counter]=temp;
            }if(counter>29){
                counter=0;
            }
            //printf("%x ", temp);
            //fflush(stdout);
            */
        }
    }
	return 0;
}

int main (int argc, char* argv[])
{
    int listenFd, portNo;
    bool loop = false;
    struct sockaddr_in svrAdd;
    struct hostent *server;
    
    if(argc < 3)
    {
        cerr<<"Syntax : ./client <host name> <port>"<<endl;
        return 0;
    }
    
    portNo = atoi(argv[2]);
    
    if((portNo > 65535) || (portNo < 2000))
    {
        cerr<<"Please enter port number between 2000 - 65535"<<endl;
        return 0;
    }       
    
    //create client skt
    listenFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if(listenFd < 0)
    {
        cerr << "Cannot open socket" << endl;
        return 0;
    }
    
    server = gethostbyname(argv[1]);
    
    if(server == NULL)
    {
        cerr << "Host does not exist" << endl;
        return 0;
    }
    
    bzero((char *) &svrAdd, sizeof(svrAdd));
    svrAdd.sin_family = AF_INET;
    
    bcopy((char *) server -> h_addr, (char *) &svrAdd.sin_addr.s_addr, server -> h_length);
    
    svrAdd.sin_port = htons(portNo);
    
    int checker = connect(listenFd,(struct sockaddr *) &svrAdd, sizeof(svrAdd));
    
    if (checker < 0)
    {
        cerr << "Cannot connect!" << endl;
        return 0;
    }
    //int fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
	int fd = open("/dev/cu.usbserial-DA00SUO8", O_RDWR | O_NOCTTY);
    if (fd == -1)
        {
                perror("open_port: Unable to open /dev/ttyS0 - ");
        }
	
    struct data txrx;
    txrx.socket = &listenFd;
    txrx.xbee = &fd;
    
	pthread_t threads;
	pthread_create (&threads, NULL, send, (void*)&txrx);

	
	char line[200];
    memset (line,'\0',200);
    
    //send stuff to server
    for(;;)
    {
       	if(read(listenFd, line, 200)!=0)
       		printf("%s\n", line);
        
       	if(strncmp(line,">> o",4)==0){
        	printf("It's ON!\n");
        	 turn_ON(fd);	
        }
        if(strncmp(line,">> c",4)==0){
        	printf("It's OFF!\n");
        	 turn_OFF(fd);	
        }
        if(strncmp(line,">> q",4)==0){
        	printf("BYE!\n");
        	 turn_OFF(fd);	
        }
       	fflush(stdout);
       	memset (line,0,200);
        //for(int i=0;i<200;i++)
        //	line[i]=0;
        /*
        char character =0;
        int counter =0;
        for(int i=0;i<20;i++){
        
        	while(read(listenFd, &character, sizeof(char))==0  )
        		;
        	line[counter]=character;
        	if(line[counter]=='\n'){
        		//line[counter]='\0';
        		break;
        	}
        	counter++;
        }
        if(strncmp(line,">> o",4)==0){
        	printf("Yeah!");
        	 //turn_ON(fd);	
        }
        printf("%s", line);
        fflush(stdout);
        memset (line,'\0',200);
        */
    }
}
