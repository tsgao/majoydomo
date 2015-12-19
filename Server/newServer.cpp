#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<time.h>
#include<pthread.h> //for threading , link with lpthread
#include "input_output.hpp"

#define MAX_CLIENTS 10000

void *connection_handler(void *);

class MyThread{
private:
	
	int num_of_thread;
	
public:
	pthread_t * threads;
	char ** name;
	int * socket;
	char * connected;
	unsigned short* port;
	struct in_addr * ip_address;
	MyThread(int num_of_thread);
	~MyThread();
	
};



MyThread::MyThread(int number_of_thread){
	this->num_of_thread = number_of_thread;
	this->threads = (pthread_t *)malloc(number_of_thread*sizeof(pthread_t));
	this->socket = (int *) malloc(number_of_thread*sizeof(int));
	this->connected = (char *) malloc(number_of_thread*sizeof(char));
	this->name = (char **) malloc(number_of_thread*sizeof(char*));
	this->port = (unsigned short*)malloc(number_of_thread*sizeof(unsigned short));
	this->ip_address = (struct in_addr*)malloc(number_of_thread*sizeof(struct in_addr));
	
	for(int i=0;i<number_of_thread;i++){
		this->connected[i] = 0;
		this->name[i] = (char*) malloc(41*sizeof(char));
		memset (this->name[i],'\0',41);
	}
}
MyThread::~MyThread(){
	free(this->threads);
	free(this->socket);
}


int main(int argc , char *argv[])
{
   	int 							socket_desc;
    int 							new_socket;
    int 							c;
    int 							*new_sock;
    
    struct sockaddr_in 				server;
    struct sockaddr_in 				client;
    char *message;
    
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
    
    //Bind
    bind(socket_desc,(struct sockaddr *)&server , sizeof(server));
    
    //Listen
    listen(socket_desc , 3);
    
    //Accept and incoming connection
    time_t rawtime;
	time (&rawtime);
  	printf ("The current local time is: %s", ctime (&rawtime));
    printf("Waiting for incoming connections...\n");
    
    c = sizeof(struct sockaddr_in);
    MyThread * myThread = new MyThread(MAX_CLIENTS);
    int index_of_thread=0;
    int temp=0;
    while( true )
    {
    	temp = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    	
    	for(index_of_thread=0;index_of_thread<MAX_CLIENTS;index_of_thread++){
    		if(myThread->connected[index_of_thread]==0){
    			myThread->socket[index_of_thread] = temp;
    			if( pthread_create( &myThread->threads[index_of_thread] , NULL ,  connection_handler , (void*) myThread) < 0)
        		{
            		perror("could not create thread");
            		return 1;
       			}
        		myThread->port[index_of_thread] = ntohs(client.sin_port);
        		myThread->ip_address[index_of_thread]= client.sin_addr;
        		break;
    		}
    	}
        
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( myThread->threads[index_of_thread] , NULL);
        
    }
    return 0;
}
/*
 * This will handle connection for each client
 * */
void *connection_handler(void * allThread)
{
    //Get the socket descriptor
    MyThread* myThread = (MyThread*)allThread;
    
    // Variables
    char *message;
    char *client_message;
    char *password;
    int read_size;
    time_t rawtime;
    int index =0;  // index of users
    FILE * file;
    bool match = false;
    
    // Malloc memory for variables
    message = (char*)malloc(3001*sizeof(char));
    client_message = (char*)malloc(2001*sizeof(char));
    password = (char*)malloc(25*sizeof(char));
    
	// Find out who I am, save index of threads in "index"
    for(int i=0;i<MAX_CLIENTS;i++){
    		if(myThread->threads[i] == pthread_self())
    		{
    			index = i;
    			break;
    		}
    }
    
    // Ask for Username and Password
    // TODO: Client need to restrict username length
    file = fopen("database.txt","r");
    write(myThread->socket[index] , "Username:", strlen("Username:"));
    if(read(myThread->socket[index] , myThread->name[index] , 40)>0)
    {
    	for(int wrong_time=0;wrong_time<10&& !match;wrong_time++){
    		write(myThread->socket[index] , "Password:", strlen("Password:"));
    		read(myThread->socket[index] , password , 24);
    		while(getLine(file,message)){
    			if(strncmp(message,myThread->name[index],strlen(message))==0){
    				getLine(file,message);
    				//printf("Password is %s : You Input is %s\n", message, password);
    				fflush(stdout);
    			
    				if(strcmp(message,password)==0){
    					match = true;
                        write(myThread->socket[index] , "Welcome!", strlen("Welcome!"));
    					break;
    				}
    				memset (password,'\0',25);
    			}
    		getLine(file,message);
    		}
    		rewind(file);
    		if(!match)
    			write(myThread->socket[index] , "Sorry, password not correct.", strlen("Sorry, password not correct."));
        }
    }
    if(match){
    	myThread->connected[index]=1;
		time (&rawtime);
    	printf("<--%-10s Thread NO.%d Connected! From %s:%hu Time: %s",myThread->name[index],index, inet_ntoa(myThread->ip_address[index]), myThread->port[index],ctime(&rawtime));
    	//write(myThread->socket[index] , "Welcome! You can now chat with each other!\n", strlen("Welcome! You can now chat with each other!\n"));
    	while( (read_size = read(myThread->socket[index] , client_message , 2000 )) > 0 && strncmp(client_message, "!quit",5)!=0)
    	{
            printf("%s\n",client_message);
    		memset (message,'\0',3001);
    		time (&rawtime);
    		//strcpy(message, ctime(&rawtime));
    		//strncat(message,myThread->name[index],name_length);
            if(strncmp(client_message, ">",1)==0){
                strcat(message,myThread->name[index]);
                strcat(message," said: ");
                strcat(message,client_message+1);
                for(int i=0;i<MAX_CLIENTS;i++){
                    if(myThread->connected[i]==1 && strlen(client_message)>0) {
                        write(myThread->socket[i] , message , strlen(message));
                    }
                }
            }else if(strncmp(client_message, "*",1)==0){
                strcat(message,client_message);
                for(int i=0;i<MAX_CLIENTS;i++){
                    if(myThread->connected[i]==1 && strlen(client_message)>0 && i!=index && strcmp(myThread->name[i],myThread->name[index])==0) {
                        write(myThread->socket[i] , message , strlen(message));
                    }
                }
            }else{
                strcat(message,">> ");
                strcat(message,client_message);
                for(int i=0;i<MAX_CLIENTS;i++){
                    if(myThread->connected[i]==1 && i!=index && strlen(client_message)>0 && strcmp(myThread->name[i],myThread->name[index])==0 ) {
                        write(myThread->socket[i] , message , strlen(message));
                    }
                }
            }
        	memset (client_message,'\0',2001);
    	}
    
    	if(read_size == 0 || strncmp(client_message, "!quit",5)==0)
    	{
    		time (&rawtime);
       		printf("-->%-10s Thread N0.%d Disconnected! From %s:%hu Time: %s",myThread->name[index],index, inet_ntoa(myThread->ip_address[index]), myThread->port[index], ctime (&rawtime));
        	fflush(stdout);
    	}
    	else if(read_size == -1)
    	{
        	perror("recv failed");
    	}
    	//write(myThread->socket[index] , "Good Bye!\n", strlen("Good Bye!\n"));
    }else{
    	write(myThread->socket[index] , "Sorry, your password or username is incorrect.", strlen("Sorry, your password or username is incorrect."));
    }
    myThread->connected[index]=0;
    free(message);
    free(client_message);
    free(password);
    memset (myThread->name[index],'\0',41);
    fclose(file);
    return 0;
}
