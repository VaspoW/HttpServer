
#include <iostream>
#include <thread>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <getopt.h>
#define BUF_LEN 1024

#include "Configuration.h"

void HandleConnection(int fd);
void PrintVersion(const char* app);
void Usage(const char* app);
int main(int argc,char** argv)
{
	//initialize app
		//inilialize default logging
		//read configrutaion
	int opt;
	std::string confPath="httpserver.conf";
	while ((opt = getopt(argc, argv, "hvc:")) != -1)
	{
		switch (opt)
		{
			case 'c':
				confPath = std::string(optarg);
			break;
			case 'v':
				PrintVersion(argv[0]);
				exit(EXIT_SUCCESS);
			break;
			case 'h':
				Usage(argv[0]);
				exit(EXIT_SUCCESS);
			break;
			default:
				Usage(argv[0]);
				exit(EXIT_FAILURE);
		}
	}
	try
	{
		Configuration conf(confPath);
			//change logging if needed
			//setup signal handler
	
		//create socket
		int sockfd, newsockfd, portno;
		socklen_t clilen;
		struct sockaddr_in serv_addr, cli_addr;
		int n;
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		//set address
		bzero((char *) &serv_addr, sizeof(serv_addr));
		portno = 7000;
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(portno);
	
		//bind socket
		if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
		{
			perror("Error while bind");
			exit(-1);
		}
	
		//listen socket
		listen(sockfd,5);
	
		//accept on socket
		while(1)
		{
			newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
			//on request
			if (newsockfd < 0) 
			{
				perror("Error while accept");
				//exit(-2);
			}
			std::cout<<"Hello\n";
			//create new thread to handle connection
			std::thread t(HandleConnection,newsockfd);
			t.detach();
			//sleep(1);
		}
		close(newsockfd);
		close(sockfd);
	}
	catch(std::exception& ex)
	{
		std::cout<<ex.what()<<" at:"<<__LINE__<<" in "<<__FILE__<<std::endl;
	}
	catch(...)
	{
		std::cout<<"Unknown exception at:"<<__LINE__<<"in"<<__FILE__<<std::endl;
	}
	return 0;
}
void PrintVersion(const char* app)
{
	std::cout<<app<<":"<<"0.0.0 dev"<<std::endl;
}
void Usage(const char* app)
{
	std::cout<<app<<" "<<"[-v] [-h] [-c file]"<<std::endl;
	std::cout<<"\t-h\t Print help\n";
	std::cout<<"\t-v\t Print version\n";
	std::cout<<"\t-c file\t path to config file [default:httpserver.conf]\n";
}
void HandleConnection(int fd)
{
	char buf[BUF_LEN];
	int n=0;
	bzero(buf,BUF_LEN);
	struct timeval timeout;      
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    if (setsockopt (fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,sizeof(timeout)) < 0)
    {
        perror("setsockopt failed");
    }
		//read data from connection
		
		//get version
			//validate version
		//get request type
			//validate request type
		//respond request
	//fcntl(fd, F_SETFL, O_NONBLOCK | O_RDWR);
	bool doRead=true;
	while(doRead)
	{
		if((n=read(fd,buf,BUF_LEN-1))>0)
		{
			std::cout<<">>"<<n<<std::endl;
			std::cout<<">>"<<buf<<std::endl;
			char* p;
			if(p = strstr(buf,"\r\n\r\n"))
			{
				std::cout<<"header recieved\n";
			}
			else
			{
				//still some data on wire
				std::cout<<"some data pending\n";
			}
			//write(fd,"501 Not Implemented\nManthan",strlen("501 Not Implemented\nManthan"));
			bzero(buf,BUF_LEN);
		}
		else
		{
			std::cout<<"errno"<<errno<<std::endl;
			switch (errno) 
			{
				case EAGAIN:
					std::cout<<"EAGAIN"<<std::endl;
					doRead = false;
				break;
				case EINTR:
					std::cout<<"EINTR"<<std::endl;
				break;
				case ECONNRESET:
					std::cout<<"ECONNRESET"<<std::endl;
					doRead = false;
				break;
				default:
					std::cout<<"default"<<std::endl;
					doRead = false;
				break;
			}
			break;
		}
	}
	std::cout<<"clossing connection\n";
	close(fd);
}
