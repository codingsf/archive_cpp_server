
#include "ps_socket_api.h"

int socket_recv_by_size(int socketfd,char* buf,size_t nbytes)
{
	int received = 0;
	int receiving = nbytes;
	int offset = 0;

	while(receiving > 0)
	{
		received = 0;
		received = recv(socketfd,buf + offset,receiving,0);
		if(received <= 0)
		{
			return -1;
		}
		receiving = receiving - received;
		offset = offset + received;
	}

	return nbytes;
}

int socket_send_by_size(int socketfd,const char* buf,size_t nbytes)
{
	int sent = 0;
	int sending = nbytes;
	int offset = 0;

	while(sending > 0)
	{
		sent = 0;
		sent = send(socketfd,buf + offset,sending,0);
		if(sent <= 0)
		{
			return -1;
		}
		sending = sending - sent;
		offset = offset + sent;
	}

	return nbytes;
}



