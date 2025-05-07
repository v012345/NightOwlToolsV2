#ifndef _AE_INCLUDE_H_
#define _AE_INCLUDE_H_
	
#include "xenet/XEPlatformConfig.h"
	
#if XE_TARGET_PLATFORM == XE_PLATFORM_WIN32
	#include <WinSock2.h>
	#include "mswsock.h"
	#include <ws2tcpip.h>
#else
	#include <unistd.h>
	#include <fcntl.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <pthread.h>
	#include <unistd.h> 
	#include <sys/time.h>
	#include <netdb.h>
#endif
	
#endif /*_AE_INCLUDE_H_*/