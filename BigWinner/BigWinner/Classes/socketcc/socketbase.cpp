/******************************************************************************/
/* File: socketbase.cpp                                                       */
/******************************************************************************/
/* This source code file is the source file for the SocketBase class and      */
/* SocketAddress classes - SocketAddress is a private class to this file      */
/* code implementation while SocketBase is used to provide basic socket       */
/* functionality within a C++ class.                                          */
/******************************************************************************/
/* Written By: Jason But                                                      */
/* Copyright:  CTIE - Monash University 2001-2003                             */
/*                                                                            */
/* Contributions:                                                             */
/* Andreas Almroth  - Support for Solaris OS.                                 */
/* Desmond Schmidt  - Support for MacOS X 1.                                  */
/* Daniel Grimm     - Testing for MacOS X Jaguar.                             */
/* Clayborne Taylor - Support for FreeBSD.                                    */
/* Andrea Rui       - Inspiration for cleaner implementation to accept a      */
/*                    connection on a listening socket.                       */
/*                                                                            */
/* Notes:                                                                     */
/*    Version 1.00 - Original Version of module.                              */
/*                                                                            */
/*    Version 1.11 - Support for Solaris, many thanks to Andreas Almroth for  */
/*                   his effort.                                              */
/*                 - Included all-platform fixes of bugs which didn't show up */
/*                   on a Linux Compile.                                      */
/*                 - Default paramter values in implementation cause some     */
/*                   compilers to complain.                                   */
/*                                                                            */
/*    Version 1.20 - All errUnknown exceptions now include value of errno.    */
/*                 - Now catching and reporting "Host Unreachable" Exceptions.*/
/*                                                                            */
/*    Version 1.30 - Support for MacOS, many thanks to Desmond Schmidt and    */
/*                   Daniel Grimm.                                            */
/*                                                                            */
/*    Version 1.38 - Support for FreeBSD, many thanks to Clayborne Taylor.    */
/*                 - Send() and SendTo() methods now accept const void        */
/*                   pointers as a parameter, this is more correct when       */
/*                   defining what the method does.                           */
/*                 - Created a new constructor for the SocketBase Class, this */
/*                   constructor allows creation of a SocketBase instance by  */
/*                   providing a socket descriptor.  This method is primarily */
/*                   for convenience as well as a better implementation of    */
/*                   accepting connections on TCP Sockets.  Please note that  */
/*                   the provided socket descriptor is not checked to ensure  */
/*                   it is a valid descriptor.                                */
/*                 - Removed method NewSocketDetails() as it is no longer     */
/*                   required with the new streamlined implementation of      */
/*                   accepting a connection on a listening socket.  Also,     */
/*                   the new templated Accept() method leads to the           */
/*                   implementation of the new protected method Accept(),     */
/*                   containing must the same code as the original Accept()   */
/*                   but now returning a socket descriptor.  Many thanks to   */
/*                   Andrea Rui for pointing out the wastefulness of socket   */
/*                   resource allocation during TCPServerSocket AcceptClient()*/
/*                   calls and making me think of a better solution.          */
/******************************************************************************/

/******************************************************************************/
/* Standard C Includes.                                                       */
/******************************************************************************/
#include <stdio.h>
#include <errno.h>

/******************************************************************************/
/* FreeBSD                                                                    */
/* - Must include sys/types for socket support.                               */
/******************************************************************************/
#if defined(PLATFORM_freebsd)
#include <sys/types.h>
#endif

#ifndef WIN32
#include <sys/socket.h>
#include <unistd.h>
#include <strings.h>
#include <signal.h>
#include <sys/ioctl.h>
#else
#define PLATFORM_win32
#undef errno
#define errno WSAGetLastError()
#endif
#include <string.h>


/******************************************************************************/
/* Include class header file.                                                 */
/******************************************************************************/
#include "socketcc.h"

#include "cocos2d.h"

/******************************************************************************/
/* Platform specific code.                                                    */
/*                                                                            */
/* This section allows for platform specific code.  Library function calls    */
/* which are not supported on certain platforms are replaced with MACROS that */
/* translate to the APIs for the given platform.                              */
/******************************************************************************/
/* MacOS X Jaguar                                                             */
/* - Type socklen_t is undefined, socket functions on the Mac use int instead.*/
/*   socklen_t is 'typedefed' to int for MacOS X.                             */
/******************************************************************************/
#if defined(PLATFORM_macosx_jaguar)

typedef int     socklen_t;

/******************************************************************************/
/* MacOS X.1.?                                                                */
/* - Type socklen_t is undefined, socket functions on the Mac use int instead.*/
/*   socklen_t is 'typedefed' to int for MacOS X.                             */
/* - in6addr_any defined differently for MacOS X.1, this is a workaround.     */
/******************************************************************************/
#elif defined(PLATFORM_macosx_1)

typedef int     socklen_t;

const struct in6_addr in6addr_any = IN6ADDR_ANY_INIT;

#elif defined(PLATFORM_win32)
const struct in6_addr in6addr_any = IN6ADDR_ANY_INIT;
#endif

/******************************************************************************/
/* Helper Class SocketAddress                                                 */
/*                                                                            */
/* This class provides encapsulation of the sockaddr_in and sockadd_in6       */
/* structures used by many IP socket commands.  The class allows us to set    */
/* parts of the structures and retrieve their contents.  This class is mainly */
/* used for convenience and to minimise rewriting of code to support both     */
/* IPv4 and IPv6.  The public member methods are:                             */
/*    SetIPAddressWildCard(): Sets the address to the wildcard address.  The  */
/*                            parameter signifies IPv6 or IPv4.               */
/*    SetIPAddress():         Sets the address to that specified in the given */
/*                            IPAddress class.                                */
/*    SetPortNumber():        Sets the port number to the specified value, 0  */
/*                            is the wildcard value.                          */
/*    GetIPAddress():         Sets cAddr equal to the IPAddress contained     */
/*                            within the sockaddr structure.                  */
/*    GetPortNumber():        Sets iPortNumber equal to the port number       */
/*                            contained within the sockaddr structure.        */
/*    (sockaddr *):           Cast to (sockaddr *).  Returns a pointer that   */
/*                            can be used in socket calls.                    */
/*    SizeOf():               Returns the size of the structure.              */
/******************************************************************************/
class SocketAddress
{
    public:
                            SocketAddress();

        void                SetIPAddressWildCard(bool bIPv6);
        void                SetIPAddress(IPAddress &cAddr);
        void                SetPortNumber(int iPortNumber);
        void                GetIPAddress(IPAddress &cAddr);
        void                GetPortNumber(int &iPortNumber);

        operator            sockaddr *() { return psaGeneric; }
        socklen_t           SizeOf();

    protected:
        struct sockaddr_in6 saStruct;
        struct sockaddr     *psaGeneric;
        struct sockaddr_in  *psaIP4Addr;
        struct sockaddr_in6 *psaIP6Addr;
};

/******************************************************************************/
/* SocketAddress Constructor.                                                 */
/*                                                                            */
/* Zeroes the contents of the internal sockaddr_in6 structure and then sets   */
/* all three structure pointers to point to the internal structure.  The      */
/* three pointers allow us to reference the structure as being of all types.  */
/* The actual internal structure is of type sockaddr_in6 because it is the    */
/* largest of all three types.                                                */
/******************************************************************************/
SocketAddress::SocketAddress()
{
    memset(&saStruct, 0, sizeof(saStruct));

    psaGeneric = (sockaddr *)&saStruct;
    psaIP4Addr = (sockaddr_in *)&saStruct;
    psaIP6Addr = &saStruct;
}

/******************************************************************************/
/* void SetIPAddressWildCard(bool bIPv6)                                      */
/*                                                                            */
/* If we want an IPv6 wildcard address, the sin6_addr value of the sockaddr   */
/* structure is set to in6addr_any, otherwise the sin_addr value of the       */
/* structure is set to INADDR_ANY.   We also set the family type.             */
/******************************************************************************/
void
SocketAddress::SetIPAddressWildCard(bool bIPv6)
{
    if (bIPv6)
    {
        psaIP6Addr->sin6_addr   = in6addr_any;
        psaIP6Addr->sin6_family = AF_INET6;
    } else
    {
        psaIP4Addr->sin_addr.s_addr = htonl(INADDR_ANY);
        psaIP4Addr->sin_family      = AF_INET;
    }
}

/******************************************************************************/
/* void SetIPAddress(IPAddress &cAddr)                                        */
/*                                                                            */
/* Sets the Address part of the sockaddr structure.  First we set the family  */
/* type based on the family type of the provided IP Address.  We then make a  */
/* binary copy of the address stored in cAddr to the appropriate field in the */
/* sockaddr structure.                                                        */
/******************************************************************************/
void
SocketAddress::SetIPAddress(IPAddress &cAddr)
{
    psaGeneric->sa_family = cAddr.GetAddressFamily();
    if (psaGeneric->sa_family == AF_INET6)
    {
#ifndef WIN32
        bcopy(cAddr.Get_in_addr(), &(psaIP6Addr->sin6_addr), cAddr.Get_in_addr_length());
#else
		memcpy((char*)psaIP6Addr->sin6_addr.s6_bytes, (char*)cAddr.Get_in_addr(), cAddr.Get_in_addr_length());
#endif
    } else
    {
#ifndef WIN32
        bcopy(cAddr.Get_in_addr(), &(psaIP4Addr->sin_addr), cAddr.Get_in_addr_length());
#else
		memcpy(&psaIP4Addr->sin_addr.s_addr, cAddr.Get_in_addr(), cAddr.Get_in_addr_length());
#endif
    }
}

/******************************************************************************/
/* void SetPortNumber(int iPortNumber)                                        */
/*                                                                            */
/* Sets the port number part of the sockaddr structure.  Based on the value   */
/* of the family type, we set either the sin_port or sin6_port fields of the  */
/* sockaddr structure.  If the family type is not set, we set the IP4 value.  */
/******************************************************************************/
void
SocketAddress::SetPortNumber(int iPortNumber)
{
    if (psaGeneric->sa_family == AF_INET6)
    {
        psaIP6Addr->sin6_port = htons(iPortNumber);
    } else
    {
        psaIP4Addr->sin_port = htons(iPortNumber);
    }
}

/******************************************************************************/
/* void GetIPAddress(IPAddress &cAddr)                                        */
/*                                                                            */
/* Returns the Address stored in the sockaddr structure.  Based on the family */
/* type value, we set the IPAddress parameter equal to either the sin6_addr   */
/* or sin_addr fields.  If the family type is not set, we assume it is IPv4.  */
/******************************************************************************/
void
SocketAddress::GetIPAddress(IPAddress &cAddr)
{
    if (psaGeneric->sa_family == AF_INET6)
    {
        cAddr = psaIP6Addr->sin6_addr;
    } else
    {
        cAddr = psaIP4Addr->sin_addr;
    }
}

/******************************************************************************/
/* void GetPortNumber(int &iPortNumber)                                       */
/*                                                                            */
/* Returns the port number stored in the sockaddr structure.  Based on the    */
/* family type value, we set the iPortNumber parameter equal to either the    */
/* sin6_port or sin_port fields.  If the family type is not set, assume IPv4. */
/******************************************************************************/
void
SocketAddress::GetPortNumber(int &iPortNumber)
{
    if (psaGeneric->sa_family == AF_INET6)
    {
        iPortNumber = ntohs(psaIP6Addr->sin6_port);
    } else
    {
        iPortNumber = ntohs(psaIP4Addr->sin_port);
    }
}

/******************************************************************************/
/* socklen_t SizeOf()                                                         */
/*                                                                            */
/* Returns the size of the structure pointed to by the (sockaddr *) cast.     */
/* The value returned is based on the family type value.  If the family type  */
/* is not set, we return the size of the larger sockaddr_in6 structure so     */
/* that both types are covered.                                               */
/******************************************************************************/
socklen_t
SocketAddress::SizeOf()
{
    return (psaGeneric->sa_family == AF_INET)?(sizeof(sockaddr_in)):(sizeof(sockaddr_in6));
}

/******************************************************************************/
/* End Class : SocketAddress                                                  */
/******************************************************************************/

/******************************************************************************/
/* Class SocketBase                                                           */
/******************************************************************************/
/* SocketBase Static Member Variables.                                        */
/*                                                                            */
/* mutexKernelLock : Initially set to true, once the SocketBase constructor   */
/*                   is run, the SIGIO handler will be installed and the flag */
/*                   will be reset.  This ensures that the handle is only     */
/*                   installed once.                                          */
/******************************************************************************/
bool    SocketBase::bFirstInstance = true;

/******************************************************************************/
/* SocketBase Constructor.                                                    */
/*                                                                            */
/* If this is the first time the constructor has been run in a program, then  */
/* we install the SIGIO signal handler and reset the bFirstInstance flag.     */
/* The socket() function is called to create the socket of the specified type.*/
/* The parameters indicate whether the socket will use IPv6 or IPv4 and the   */
/* type of socket (the default being SOCK_STREAM or TCP).  The returned       */
/* descriptor is stored in iSockDesc.  If an error occurs, an exception is    */
/* thrown and the class is not instantiated, the exception indicates which    */
/* error actually occured.  Default values specify an IPv4 TCP socket.        */
/******************************************************************************/
SocketBase::SocketBase(bool bSocketIsIPv6, int iSocketType)
        : mutexSocket(), bIPv6Socket(bSocketIsIPv6)
{
    if (bFirstInstance)
    {
#ifndef WIN32
		signal(SIGIO, &IOSignalHandler);
#else
		//WSADATA wsaData;
		//WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif
		bFirstInstance = false;
    }

    if ((iSockDesc = socket((bIPv6Socket)?AF_INET6:AF_INET, iSocketType, 0)) < 0)
    {
        switch (errno)
        {
#ifndef WIN32
            case EPFNOSUPPORT:
            case EAFNOSUPPORT:
            case EPROTONOSUPPORT:   OnDisconnect( SocketException(SocketException::errNoProtocolSupport));
            case ENFILE:            OnDisconnect( SocketException(SocketException::errKernelMemory));
            case EMFILE:            OnDisconnect( SocketException(SocketException::errNoDescriptors));
            case EACCES:            OnDisconnect( SocketException(SocketException::errPermissionDenied));
            case ENOMEM:            OnDisconnect( SocketException(SocketException::errMemory));
            case EINVAL:            OnDisconnect( SocketException(SocketException::errInvalidProtocol));
#else
			case WSAEPFNOSUPPORT:
			case WSAEAFNOSUPPORT:
			case WSAEPROTONOSUPPORT:   OnDisconnect( SocketException(SocketException::errNoProtocolSupport));
			case WSAEMFILE:            OnDisconnect( SocketException(SocketException::errNoDescriptors));
			case WSAEACCES:            OnDisconnect( SocketException(SocketException::errPermissionDenied));
			case WSAEINVAL:            OnDisconnect( SocketException(SocketException::errInvalidProtocol));
#endif
            default:                OnDisconnect( SocketException(SocketException::errUnknown, errno));
        }
    }
}

/******************************************************************************/
/* SocketBase Constructor.                                                    */
/*                                                                            */
/* This version of the constructor creates an instance of SocketBase given a  */
/* socket descriptor.  As for the other constructor, if this is the first     */
/* time a SocketBase instance has been created, we install the SIGIO signal   */
/* handler and reset the bFirstInstance flag.  If the provided socket         */
/* descriptor is invalid (< 0), we throw a BadDescriptor exception.  If all   */
/* is OK, we set the internal member variables iSockDesc and bIPv6Socket.     */
/******************************************************************************/
SocketBase::SocketBase(int iNewSockDesc)
        : mutexSocket()
{
    IPAddress   cLocalAddr;
    int         iLocalPort;

	if (bFirstInstance)
	{
#ifndef WIN32
		signal(SIGIO, &IOSignalHandler);
#else
		//WSADATA wsaData;
		//WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif
		bFirstInstance = false;
	}

    if (iNewSockDesc < 0) OnDisconnect(SocketException(SocketException::errBadDescriptor));

    iSockDesc = iNewSockDesc;

    GetSockName(cLocalAddr, iLocalPort);
    bIPv6Socket = (cLocalAddr.GetAddressFamily() == AF_INET6);
}

/******************************************************************************/
/* SocketBase Destructor.                                                     */
/*                                                                            */
/* If the class was successfully instantiated then the socket was created and */
/* iSockDesc is a valid descriptor.  The destructor first closes the socket.  */
/* Following this we need to ensure that any thread currently blocked on a    */
/* call to the newly closed socket successfully terminates, we try to do this */
/* by locking the socket MutEx, if this succeeds, no threads are blocked and  */
/* we can unlock the MutEx and conclude the destructor.  If the lock fails,   */
/* we send a SIGIO to the MutEx owner, this will cause the socket operation   */
/* to interrupt and then get called again, but this time iSockDesc will be    */
/* invalid and the call will terminate by throwing an exception.  Any other   */
/* threads waiting for the MutEx will also throw this exception when they try */
/* to perform a socket API call.  The destructor then blocks waiting for the  */
/* MutEx (signifying that any other threads have successfully thrown their    */
/* exception) before unlocking the MutEx and concluding the destructor.       */
/******************************************************************************/
SocketBase::~SocketBase()
{
#ifndef WIN32
    close(iSockDesc);
	if (!(mutexSocket.TryLock()))
	{
		kill(pidMutExOwner, SIGIO);
		mutexSocket.Lock();
	}

#else
	mutexSocket.Lock();
	closesocket(iSockDesc);
	//WSACleanup();
#endif
   
	mutexSocket.Unlock();
}

/******************************************************************************/
/* void Connect(IPAddress &cServAddr, int iServPortNo)                        */
/*                                                                            */
/* This method is a wrapper for the socket connect() call and connects the    */
/* socket to the specified server (ip address and port number).  A Socket     */
/* Address is created with the provided ip address and port number and this   */
/* is used to call the connect function.  If the connect() call fails, then   */
/* the correct exception is thrown based on the error that occured.           */
/******************************************************************************/
int
SocketBase::Connect(IPAddress &cServAddr, int iServPortNo)
{
    SocketAddress cServer;

    cServer.SetIPAddress(cServAddr);
    cServer.SetPortNumber(iServPortNo);

    WaitMutex();

	cocos2d::CCLog("before connect");

	long ul = 1;
	IoCntl(FIONBIO, &ul);  //设置为非阻塞模式

    if (connect(iSockDesc, (sockaddr *) cServer, cServer.SizeOf()) < 0)
    {
		//cocos2d::CCLog("connect error");
		
		bool ret = false;
		timeval tm;
		fd_set set;

		tm.tv_sec  = 8;
		tm.tv_usec = 0;
		FD_ZERO(&set);
		FD_SET(iSockDesc, &set);
		
		if (select(iSockDesc+1, NULL, &set, NULL, &tm) > 0)
		{
			int error = -1, len = sizeof(int);
			GetSockOpt(SOL_SOCKET, SO_ERROR, &error, len);
			if(error == 0) 
				ret = true;

		}
		else
			 ret = false;

		if (!ret)
		{
			cocos2d::CCLog("connect error");
			switch (errno)
			{
#ifndef WIN32
			case EBADF:
			case ENOTSOCK:      OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case EFAULT:        OnDisconnect(SocketException(SocketException::errIllegalPointer));
				break;
			case EISCONN:       OnDisconnect(SocketException(SocketException::errAlreadyConnected));
				break;
			case ECONNREFUSED:  OnDisconnect(SocketException(SocketException::errConnectRefused));
				break;
			case ETIMEDOUT:     OnDisconnect(SocketException(SocketException::errConnectTimeOut));
				break;
			case ENETUNREACH:   OnDisconnect(SocketException(SocketException::errNetUnreachable));
				break;
			case EHOSTUNREACH:  OnDisconnect(SocketException(SocketException::errHostUnreachable));
				break;
			case EADDRINUSE:    OnDisconnect(SocketException(SocketException::errAddrInUse));
				break;
			case EINPROGRESS:   OnDisconnect(SocketException(SocketException::errInProgress));
				break;
			case EALREADY:      OnDisconnect(SocketException(SocketException::errAlreadyConnecting));
				break;
			case EAFNOSUPPORT:  OnDisconnect(SocketException(SocketException::errIncorrectAddrFamily));
				break;
			case EACCES:        OnDisconnect(SocketException(SocketException::errBrdCastNotEnabled));
				break;
#else
			case WSAEBADF:
			case WSAENOTSOCK:      OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case WSAEFAULT:        OnDisconnect(SocketException(SocketException::errIllegalPointer));
				break;
			case WSAEISCONN:       OnDisconnect(SocketException(SocketException::errAlreadyConnected));
				break;
			case WSAECONNREFUSED:  OnDisconnect(SocketException(SocketException::errConnectRefused));
				break;
			case WSAETIMEDOUT:     OnDisconnect(SocketException(SocketException::errConnectTimeOut));
				break;
			case WSAENETUNREACH:   OnDisconnect(SocketException(SocketException::errNetUnreachable));
				break;
			case WSAEHOSTUNREACH:  OnDisconnect(SocketException(SocketException::errHostUnreachable));
				break;
			case WSAEADDRINUSE:    OnDisconnect(SocketException(SocketException::errAddrInUse));
				break;
			case WSAEINPROGRESS:   OnDisconnect(SocketException(SocketException::errInProgress));
				break;
			case WSAEALREADY:      OnDisconnect(SocketException(SocketException::errAlreadyConnecting));
				break;
			case WSAEAFNOSUPPORT:  OnDisconnect(SocketException(SocketException::errIncorrectAddrFamily));
				break;
			case WSAEACCES:        OnDisconnect(SocketException(SocketException::errBrdCastNotEnabled));
				break;
#endif
			default:            OnDisconnect(SocketException(SocketException::errUnknown, errno));
			}

		}
		else
			cocos2d::CCLog("connect success");
    }
	else
		cocos2d::CCLog("connect success");

	ul = 0;
	IoCntl(FIONBIO, &ul);  //设置为阻塞模式

    ClearMutex();

	return 0;
}

/*
add by yuan for test
int	CloseSocket()
*/
void				SocketBase::CloseSocket()
{
#ifndef WIN32
	close(iSockDesc);
	if (!(mutexSocket.TryLock()))
	{
		kill(pidMutExOwner, SIGIO);
		mutexSocket.Lock();
	}

#else
	mutexSocket.Lock();
	closesocket(iSockDesc);
	iSockDesc = 0;
#endif

	mutexSocket.Unlock();
}

/******************************************************************************/
/* void Bind(IPAddress &cLocalAddr, int iLocalPort = 0)                       */
/*                                                                            */
/* This member method is a simple wrapper to the sockets bind() function call.*/
/* The method is called with a local IP address and port number to bind the   */
/* socket to.  A default port number of zero is a wildcard and lets the OS    */
/* choose the port number.  If the bind() call fails then the correct         */
/* exception is thrown based on the error that occured.                       */
/******************************************************************************/
int
SocketBase::Bind(IPAddress &cLocalAddr, int iLocalPort)
{
    SocketAddress cLocal;

    cLocal.SetIPAddress(cLocalAddr);
    cLocal.SetPortNumber(iLocalPort);

    WaitMutex();

    if (bind(iSockDesc, (sockaddr *)cLocal, cLocal.SizeOf()) < 0)
    {
        switch (errno)
        {
#ifndef WIN32
            case EBADF:
			case ENOTSOCK:      OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case EINVAL:        OnDisconnect(SocketException(SocketException::errAlreadyBound));
				break;
			case EACCES:        OnDisconnect(SocketException(SocketException::errAddressProtected));
				break;
			case EADDRINUSE:    OnDisconnect(SocketException(SocketException::errAddrInUse));
				break;
#else
			case WSAEBADF:
			case WSAENOTSOCK:      OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case WSAEINVAL:        OnDisconnect(SocketException(SocketException::errAlreadyBound));
				break;
			case WSAEACCES:        OnDisconnect(SocketException(SocketException::errAddressProtected));
				break;
			case WSAEADDRINUSE:    OnDisconnect(SocketException(SocketException::errAddrInUse));
				break;
#endif
			default:            OnDisconnect(SocketException(SocketException::errUnknown, errno));
        }
    }
    ClearMutex();

	return 0;
}

/******************************************************************************/
/* void Bind(int iLocalPort = 0)                                              */
/*                                                                            */
/* This member method is a simple wrapper to the sockets bind() function call.*/
/* The method is called with a port number to bind the socket to, a wildcard  */
/* ip address is used to bind to all IP addresses on the system.  A default   */
/* port number of zero is a wildcard and lets the OS choose the port number.  */
/* If the bind() call fails then the correct exception is thrown based on the */
/* error that occured.                                                        */
/******************************************************************************/
int
SocketBase::Bind(int iLocalPort)
{
    SocketAddress cLocal;

    cLocal.SetIPAddressWildCard(bIPv6Socket);
    cLocal.SetPortNumber(iLocalPort);

    WaitMutex();

    if (bind(iSockDesc, (sockaddr *)cLocal, cLocal.SizeOf()) < 0)
    {
        switch (errno)
        {
#ifndef WIN32
		case EBADF:
		case ENOTSOCK:      OnDisconnect(SocketException(SocketException::errBadDescriptor));
			break;
		case EINVAL:        OnDisconnect(SocketException(SocketException::errAlreadyBound));
			break;
		case EACCES:        OnDisconnect(SocketException(SocketException::errAddressProtected));
			break;
		case EADDRINUSE:    OnDisconnect(SocketException(SocketException::errAddrInUse));
			break;
#else
		case WSAEBADF:
		case WSAENOTSOCK:      OnDisconnect(SocketException(SocketException::errBadDescriptor));
			break;
		case WSAEINVAL:        OnDisconnect(SocketException(SocketException::errAlreadyBound));
			break;
		case WSAEACCES:        OnDisconnect(SocketException(SocketException::errAddressProtected));
			break;
		case WSAEADDRINUSE:    OnDisconnect(SocketException(SocketException::errAddrInUse));
			break;
#endif
		default:            OnDisconnect(SocketException(SocketException::errUnknown, errno));
        }
    }
    ClearMutex();

	return 0;
}

/******************************************************************************/
/* void Listen(int iBackLog)                                                  */
/*                                                                            */
/* This member method is a simple wrapper to the sockets listen() function    */
/* call.  The iBackLog parameter indicates the number of unconnected sockets  */
/* that can be pending in the socket queue.  If the listen() call fails then  */
/* the correct exception is thrown based on the error that occured.           */
/******************************************************************************/
int
SocketBase::Listen(int iBackLog)
{
    WaitMutex();

    if (listen(iSockDesc, iBackLog) < 0)
    {
        switch (errno)
        {
#ifndef WIN32
			case EBADF:
			case ENOTSOCK:      OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case EOPNOTSUPP:    OnDisconnect(SocketException(SocketException::errCantListen));
				break;
#else
			case WSAEBADF:
			case WSAENOTSOCK:      OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case WSAEOPNOTSUPP:    OnDisconnect(SocketException(SocketException::errCantListen));
				break;
#endif
			default:            OnDisconnect(SocketException(SocketException::errUnknown, errno));
        }
    }
    ClearMutex();

	return 0;
}

/******************************************************************************/
/* int protAccept(IPAddress &cRemoteAddr, int &iRemotePortNo)                 */
/*                                                                            */
/* This protected member method is a simple wrapper to the sockets accept()   */
/* function call.  A SocketAddress class is created to accept the details of  */
/* the new connection before accpt() is called.  If the accept() call fails   */
/* then the correct exception is thrown based on the error that occured.  If  */
/* the new connection was accepted, the Remote IP Address and Port No. are    */
/* extracted from the SocketAddress structure and stored in the method        */
/* parameters for return to the caller.  Finally, we return the newly created */
/* socket descriptor.  This protected implementation is called by the         */
/* publically accessible templated Accept() member method which then creates  */
/* a socket class instance of the specified type based on the socket          */
/* descriptor returned by this method.                                        */
/******************************************************************************/
int
SocketBase::protAccept(IPAddress &cRemoteAddr, int &iRemotePortNo)
{
    int           iNewSockDesc;
    SocketAddress cRemote;
    socklen_t     slDummy = cRemote.SizeOf();

    WaitMutex();

    if ((iNewSockDesc = accept(iSockDesc, (sockaddr *) cRemote, &slDummy)) < 0)
    {
        switch (errno)
        {
#ifndef WIN32
			case EBADF:
			case ENOTSOCK:      OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case EAFNOSUPPORT:  OnDisconnect(SocketException(SocketException::errNotStreamSock));
				break;
			case EFAULT:        OnDisconnect(SocketException(SocketException::errIllegalPointer));
				break;
			case EAGAIN:        OnDisconnect(SocketException(SocketException::errNoPendingConnections));
				break;
			case EPERM:         OnDisconnect(SocketException(SocketException::errFirewall));
				break;
			case ENOMEM:        OnDisconnect(SocketException(SocketException::errMemory));
				break;
#else
			case WSAEBADF:
			case WSAENOTSOCK:      OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case WSAEAFNOSUPPORT:  OnDisconnect(SocketException(SocketException::errNotStreamSock));
				break;
			case WSAEFAULT:        OnDisconnect(SocketException(SocketException::errIllegalPointer));
				break;
			case WSAEWOULDBLOCK:        OnDisconnect(SocketException(SocketException::errNoPendingConnections));
				break;
#endif
			default:            OnDisconnect(SocketException(SocketException::errUnknown, errno));
        }
    }
    ClearMutex();

    cRemote.GetIPAddress(cRemoteAddr);
    cRemote.GetPortNumber(iRemotePortNo);

    return iNewSockDesc;
}

/******************************************************************************/
/* int Recv(void *pBuffer, int iBufLength, unsigned int uiFlags)              */
/*                                                                            */
/* This member method is a simple wrapper to the sockets recvfrom() function  */
/* call.  The recvfrom() function is called with the proper parameters, NULL  */
/* is placed for obtaining the source address information.  The number of     */
/* bytes read is returned.  If an error occurs then the correct exception is  */
/* thrown based on the error that occured.  If the method terminates normally */
/* then we return the actual number of bytes read.                            */
/******************************************************************************/
int
SocketBase::Recv(void *pBuffer, int iBufLength, unsigned int uiFlags)
{
    int     iBytesRead;

    WaitMutex();

//	cocos2d::CCLog("before Recv");
	if (!iSockDesc)
	{
		cocos2d::CCLog("have disconnect socket");
		return -1;
	}
#ifndef WIN32
    if ((iBytesRead = recvfrom(iSockDesc, pBuffer, iBufLength, uiFlags, NULL, NULL)) < 0)
#else
	if ((iBytesRead = recvfrom(iSockDesc, (char*)pBuffer, iBufLength, uiFlags, NULL, NULL)) < 0)
#endif
    {
	//	cocos2d::CCLog("Recv error");
        switch (errno)
        {
#ifndef WIN32     
			case EBADF:
			case ENOTSOCK:      OnDisconnect(SocketException(SocketException::errBadDescriptor));
            case ECONNREFUSED:
			case ENOTCONN:      OnDisconnect(SocketException(SocketException::errNotConnected));
				break;
			case EAGAIN:        OnDisconnect(SocketException(SocketException::errWouldBlock));
				break;
			case EINTR:         OnDisconnect(SocketException(SocketException::errInterrupted));
				break;
			case EFAULT:        OnDisconnect(SocketException(SocketException::errIllegalPointer));
				break;
			case EINVAL:        OnDisconnect(SocketException(SocketException::errInvalidArgument));
				break;
#else
			case WSAEBADF:
			case WSAENOTSOCK:      OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case WSAECONNRESET:
			case WSAECONNREFUSED:
			case WSAENOTCONN:      OnDisconnect(SocketException(SocketException::errNotConnected));
				break;
			case WSAEWOULDBLOCK:        OnDisconnect(SocketException(SocketException::errWouldBlock));
				break;
			case WSAEINTR:         OnDisconnect(SocketException(SocketException::errInterrupted));
				break;
			case WSAEFAULT:        OnDisconnect(SocketException(SocketException::errIllegalPointer));
				break;
			case WSAEINVAL:        OnDisconnect(SocketException(SocketException::errInvalidArgument));
				break;
#endif
			default:            OnDisconnect(SocketException(SocketException::errUnknown, errno));
        }
    }
    ClearMutex();
//	cocos2d::CCLog("Recv finish");

    if (iBytesRead >= 0) 
		return iBytesRead;
	else
	{
		OnDisconnect(SocketException(SocketException::errNotConnected));
		return -1;
	}
}

/******************************************************************************/
/* int RecvFrom(void *pBuffer, int iBufLength, unsigned int uiFlags,          */
/*              IPAddress &SourceIP, int &SourcePortNumber)                   */
/*                                                                            */
/* This member method is a simple wrapper to the sockets recvfrom() function  */
/* call.  The recvfrom() function is called with the proper parameters, with  */
/* a SocketAddress class being used to obtain the details of the source of    */
/* the data.  The number of bytes read is returned.  If an error occurs then  */
/* the correct exception is thrown based on the error that occured.  If the   */
/* method terminates normally, then the source IP Address and port number are */
/* extracted from the SocketAddress class into the parameter variables, and   */
/* we return the actual number of bytes read.                                 */
/******************************************************************************/
int
SocketBase::RecvFrom(void *pBuffer, int iBufLength, unsigned int uiFlags,
                     IPAddress &cSourceIP, int &iSourcePortNumber)
{
    SocketAddress   cSource;
    socklen_t       slDummy = cSource.SizeOf();
    int             iBytesRead;

    WaitMutex();

#ifndef WIN32
    if ((iBytesRead = recvfrom(iSockDesc, pBuffer, iBufLength, uiFlags, (sockaddr *)cSource, &slDummy)) < 0)
#else
	if ((iBytesRead = recvfrom(iSockDesc, (char*)pBuffer, iBufLength, uiFlags, (sockaddr *)cSource, &slDummy)) < 0)
#endif
	{
		switch (errno)
		{
#ifndef WIN32     
			case EBADF:
			case ENOTSOCK:      OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case ECONNREFUSED:
			case ENOTCONN:      OnDisconnect(SocketException(SocketException::errNotConnected));
				break;
			case EAGAIN:        OnDisconnect(SocketException(SocketException::errWouldBlock));
				break;
			case EINTR:         
				OnDisconnect(SocketException(SocketException::errInterrupted));
				break;
			case EFAULT:        
				OnDisconnect(SocketException(SocketException::errIllegalPointer));
				break;
			case EINVAL:       
				OnDisconnect(SocketException(SocketException::errInvalidArgument));
				break;
#else
			case WSAEBADF:
			case WSAENOTSOCK:      OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case WSAECONNREFUSED:
			case WSAENOTCONN:      OnDisconnect(SocketException(SocketException::errNotConnected));
				break;
			case WSAEWOULDBLOCK:        OnDisconnect(SocketException(SocketException::errWouldBlock));
				break;
			case WSAEINTR:         OnDisconnect(SocketException(SocketException::errInterrupted));
				break;
			case WSAEFAULT:        OnDisconnect(SocketException(SocketException::errIllegalPointer));
				break;
			case WSAEINVAL:        OnDisconnect(SocketException(SocketException::errInvalidArgument));
				break;
#endif
			default:            OnDisconnect(SocketException(SocketException::errUnknown, errno));
		}
	}
    ClearMutex();

    if (iBytesRead >= 0)
    {
        cSource.GetIPAddress(cSourceIP);
        cSource.GetPortNumber(iSourcePortNumber);
        return iBytesRead;
    }
	else 
	{
		OnDisconnect(SocketException(SocketException::errNotConnected));
		return -1;
	}
}

/******************************************************************************/
/* int Send(const void *pPayload, int iPayloadLength, unsigned int uiFlags)   */
/*                                                                            */
/* This member method is a simple wrapper to the sockets send() function call.*/
/* call, the send() function is called with the proper parameters. The number */
/* of bytes sent is returned.  If an error occurs then the correct exception  */
/* is thrown based on the error that occured.  If the method terminates       */
/* normally then we return the actual number of bytes sent.                   */
/******************************************************************************/
int
SocketBase::Send(const void *pPayload, int iPayloadLength, unsigned int uiFlags)
{
    int             iBytesRead;

    WaitMutex();

#ifndef WIN32
    if ((iBytesRead = send(iSockDesc, pPayload, iPayloadLength, uiFlags)) < 0)
#else
	if ((iBytesRead = send(iSockDesc, (const char*)pPayload, iPayloadLength, uiFlags)) < 0)
#endif
    {
        switch (errno)
        {
#ifndef WIN32     
			case EBADF:
			case ENOTSOCK:      OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case EAGAIN:        OnDisconnect(SocketException(SocketException::errWouldBlock));
				break;
			case EINTR:         OnDisconnect(SocketException(SocketException::errInterrupted));
				break;
			case EFAULT:        OnDisconnect(SocketException(SocketException::errIllegalPointer));
				break;
			case EINVAL:        OnDisconnect(SocketException(SocketException::errInvalidArgument));
				break;
			case EMSGSIZE:      OnDisconnect(SocketException(SocketException::errMessageSizeTooBig));
				break;
            case ENOBUFS:
			case ENOMEM:        OnDisconnect(SocketException(SocketException::errKernelMemory));
				break;
            case ECONNREFUSED:
			case EPIPE:         OnDisconnect(SocketException(SocketException::errNotConnected));
				break;
			case EHOSTUNREACH:  OnDisconnect(SocketException(SocketException::errHostUnreachable));
				break;
#else
			case WSAEBADF:
			case WSAENOTSOCK:      OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case WSAEWOULDBLOCK:        OnDisconnect(SocketException(SocketException::errWouldBlock));
				break;
			case WSAEINTR:         OnDisconnect(SocketException(SocketException::errInterrupted));
				break;
			case WSAEFAULT:        OnDisconnect(SocketException(SocketException::errIllegalPointer));
				break;
			case WSAEINVAL:        OnDisconnect(SocketException(SocketException::errInvalidArgument));
				break;
			case WSAEMSGSIZE:      OnDisconnect(SocketException(SocketException::errMessageSizeTooBig));
				break;
			case WSAECONNREFUSED:
			case WSAEHOSTUNREACH:  OnDisconnect(SocketException(SocketException::errHostUnreachable));
				break;
#endif		
			default:            OnDisconnect(SocketException(SocketException::errUnknown, errno));
        }
    }
    ClearMutex();

    return iBytesRead;
}

/******************************************************************************/
/* int SendTo(const void *pPayload, int iPayloadLength, unsigned int uiFlags, */
/*            IPAddress &cDestinationIP, int iDestinationPortNumber)          */
/******************************************************************************/
int
SocketBase::SendTo(const void *pPayload, int iPayloadLength, unsigned int uiFlags,
                   IPAddress &cDestinationIP, int iDestinationPortNumber)
{
    SocketAddress   cDestination;
    int             iBytesRead;

    cDestination.SetIPAddress(cDestinationIP);
    cDestination.SetPortNumber(iDestinationPortNumber);

    WaitMutex();

#ifndef WIN32
	if ((iBytesRead = sendto(iSockDesc, pPayload, iPayloadLength, uiFlags, (sockaddr *)cDestination, cDestination.SizeOf())) < 0)
#else
    if ((iBytesRead = sendto(iSockDesc, (const char*)pPayload, iPayloadLength, uiFlags, (sockaddr *)cDestination, cDestination.SizeOf())) < 0)
#endif
    {
		switch (errno)
		{
#ifndef WIN32     
			case EBADF:
			case ENOTSOCK:      OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case EAGAIN:        OnDisconnect(SocketException(SocketException::errWouldBlock));
				break;
			case EINTR:         OnDisconnect(SocketException(SocketException::errInterrupted));
				break;
			case EFAULT:        OnDisconnect(SocketException(SocketException::errIllegalPointer));
				break;
			case EINVAL:        OnDisconnect(SocketException(SocketException::errInvalidArgument));
				break;
			case EMSGSIZE:      OnDisconnect(SocketException(SocketException::errMessageSizeTooBig));
				break;
			case ENOBUFS:
			case ENOMEM:        OnDisconnect(SocketException(SocketException::errKernelMemory));
				break;
			case ECONNREFUSED:
			case EPIPE:         OnDisconnect(SocketException(SocketException::errNotConnected));
				break;
			case EHOSTUNREACH:  OnDisconnect(SocketException(SocketException::errHostUnreachable));
				break;
#else
			case WSAEBADF:
			case WSAENOTSOCK:      OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case WSAEWOULDBLOCK:        OnDisconnect(SocketException(SocketException::errWouldBlock));
				break;
			case WSAEINTR:         OnDisconnect(SocketException(SocketException::errInterrupted));
				break;
			case WSAEFAULT:        OnDisconnect(SocketException(SocketException::errIllegalPointer));
				break;
			case WSAEINVAL:        OnDisconnect(SocketException(SocketException::errInvalidArgument));
				break;
			case WSAEMSGSIZE:      OnDisconnect(SocketException(SocketException::errMessageSizeTooBig));
				break;
			case WSAECONNREFUSED:
			case WSAEHOSTUNREACH:  OnDisconnect(SocketException(SocketException::errHostUnreachable));
				break;
#endif		
			default:            OnDisconnect(SocketException(SocketException::errUnknown, errno));
		}
    }
    ClearMutex();

    return iBytesRead;
}

/******************************************************************************/
/* void GetSockName(IPAddress &cLocalAddr, int &iLocalPort)                   */
/*                                                                            */
/* This member method is a simple wrapper to the sockets getsockname()        */
/* function call.  The getsockname() function is called with a SocketAddress  */
/* class being used to obtain the details of the local side of the socket     */
/* connection.  The details (IP Address and port number) are then extracted   */
/* into the method parameters.  If an error occurs then the correct exception */
/* is thrown based on the error that occured.                                 */
/******************************************************************************/
int
SocketBase::GetSockName(IPAddress &cLocalAddr, int &iLocalPort)
{
    SocketAddress   cLocal;
    socklen_t       slDummy = cLocal.SizeOf();

    WaitMutex();

    if (getsockname(iSockDesc, (sockaddr *)cLocal, &slDummy) < 0)
    {
        switch (errno)
        {
#ifndef WIN32
            case EBADF:
			case ENOTSOCK:  OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case ENOBUFS:   OnDisconnect(SocketException(SocketException::errKernelMemory));
				break;
			case EFAULT:    OnDisconnect(SocketException(SocketException::errIllegalPointer));
				break;
#else
			case WSAEBADF:
			case WSAENOTSOCK:  OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case WSAENOBUFS:   OnDisconnect(SocketException(SocketException::errKernelMemory));
				break;
			case WSAEFAULT:    OnDisconnect(SocketException(SocketException::errIllegalPointer));
				break;
#endif
			default:        OnDisconnect(SocketException(SocketException::errUnknown, errno));
        }	
    }
    ClearMutex();

    cLocal.GetIPAddress(cLocalAddr);
    cLocal.GetPortNumber(iLocalPort);

	return 0;
}

/******************************************************************************/
/* void GetPeerName(IPAddress &cRemoteAddr, int &iRemotePort)                 */
/*                                                                            */
/* This member method is a simple wrapper to the sockets getpeername()        */
/* function call.  The getpeername() function is called with a SocketAddress  */
/* class being used to obtain the details of the remote side of the socket    */
/* connection.  The details (IP Address and port number) are then extracted   */
/* into the method parameters.  If an error occurs then the correct exception */
/* is thrown based on the error that occured.                                 */
/******************************************************************************/
int
SocketBase::GetPeerName(IPAddress &cRemoteAddr, int &iRemotePort)
{
    SocketAddress   cRemote;
    socklen_t       slDummy = cRemote.SizeOf();

    WaitMutex();

    if (getpeername(iSockDesc, (sockaddr *)cRemote, &slDummy) < 0)
    {
        switch (errno)
        {
#ifndef WIN32
            case EBADF:
			case ENOTSOCK:  OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case ENOBUFS:   OnDisconnect(SocketException(SocketException::errKernelMemory));
				break;
			case EFAULT:    OnDisconnect(SocketException(SocketException::errIllegalPointer));
				break;
#else
			case WSAEBADF:
			case WSAENOTSOCK:  OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case WSAENOBUFS:   OnDisconnect(SocketException(SocketException::errKernelMemory));
				break;
			case WSAEFAULT:    OnDisconnect(SocketException(SocketException::errIllegalPointer));
				break;
#endif
			default:        OnDisconnect(SocketException(SocketException::errUnknown, errno));
        }
    }
    ClearMutex();

    cRemote.GetIPAddress(cRemoteAddr);
    cRemote.GetPortNumber(iRemotePort);

	return 0;
}

/******************************************************************************/
/* void GetSockOpt(int iCodeLevel, int iOptionName, void *pOptionData,        */
/*                 int &iDataLength)                                          */
/*                                                                            */
/* This member method is a simple wrapper to the socket getsockopt() function */
/* call.  The getsockopt() function is called with the provided parameters to */
/* obtain the desired value.  If an error occurs then the correct exception   */
/* is thrown based on that error.                                             */
/******************************************************************************/
int
SocketBase::GetSockOpt(int iCodeLevel, int iOptionName, void *pOptionData, int &iDataLength)
{
    WaitMutex();

#ifndef WIN32
    if (getsockopt(iSockDesc, iCodeLevel, iOptionName, pOptionData, (socklen_t *)&iDataLength) < 0)
#else
	if (getsockopt(iSockDesc, iCodeLevel, iOptionName, (char*)pOptionData, (socklen_t *)&iDataLength) < 0)
#endif
    {
        switch (errno)
        {
#ifndef WIN32
            case EBADF:
			case ENOTSOCK:      OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case ENOPROTOOPT:   OnDisconnect(SocketException(SocketException::errOptionNotSupported));
				break;
			case EFAULT:        OnDisconnect(SocketException(SocketException::errIllegalPointer));
				break;
#else
			case WSAEBADF:
			case WSAENOTSOCK:      OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case WSAENOPROTOOPT:   OnDisconnect(SocketException(SocketException::errOptionNotSupported));
				break;
			case WSAEFAULT:        OnDisconnect(SocketException(SocketException::errIllegalPointer));
				break;
#endif		
			default:            OnDisconnect(SocketException(SocketException::errUnknown, errno));
        }
    }
    ClearMutex();

	return 0;
}

/******************************************************************************/
/* void SetSockOpt(int iCodeLevel, int iOptionName, const void *pOptionData,  */
/*                 int iDataLength)                                           */
/*                                                                            */
/* This member method is a simple wrapper to the socket setsockopt() function */
/* call.  The setsockopt() function is called with the provided parameters to */
/* set the desired option.  If an error occurs then the correct exception is  */
/* thrown based on that error.                                                */
/******************************************************************************/
int
SocketBase::SetSockOpt(int iCodeLevel, int iOptionName, const void *pOptionData, int iDataLength)
{
    WaitMutex();

#ifndef WIN32
    if (setsockopt(iSockDesc, iCodeLevel, iOptionName, pOptionData, iDataLength) < 0)
#else
	if (setsockopt(iSockDesc, iCodeLevel, iOptionName, (const char*)pOptionData, iDataLength) < 0)
#endif
    {
		switch(errno)
		{
#ifndef WIN32
			case EBADF:
			case ENOTSOCK:      OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case ENOPROTOOPT:   OnDisconnect(SocketException(SocketException::errOptionNotSupported));
				break;
			case EFAULT:        OnDisconnect(SocketException(SocketException::errIllegalPointer));
				break;
#else
			case WSAEBADF:
			case WSAENOTSOCK:      OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case WSAENOPROTOOPT:   OnDisconnect(SocketException(SocketException::errOptionNotSupported));
				break;
			case WSAEFAULT:        OnDisconnect(SocketException(SocketException::errIllegalPointer));
				break;
#endif		
			default:            OnDisconnect(SocketException(SocketException::errUnknown, errno));
        }
    }
    ClearMutex();

	return 0;
}

/******************************************************************************/
/* int IoCntl(int iCommand, long* lArgument)                                    */
/*                                                                            */
/* This member method is a simple wrapper for the iocntl function so that it   */
/* can be applied to the socket.  The fcntl() function is called with the     */
/* provided parameters.  If an error occurs then the correct exception is     */
/* thrown based on that error.                                                */
/******************************************************************************/
int
SocketBase::IoCntl(int iCommand, long* lArgument)
{
    int     iResult;

    WaitMutex();
#ifndef WIN32
    if ((iResult = ioctl(iSockDesc, iCommand, lArgument)) < 0)
#else
	if ((iResult = ioctlsocket(iSockDesc, iCommand, (u_long*)lArgument)) < 0)
#endif
    {
        switch (errno)
        {
#ifndef WIN32
            case EINTR:         OnDisconnect(SocketException(SocketException::errInterrupted));
				break;
            case EACCES:
			case EAGAIN:        OnDisconnect(SocketException(SocketException::errWouldBlock));
				break;
			case EBADF:         OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
				break;
            case EFAULT:
            case EINVAL:        OnDisconnect(SocketException(SocketException::errInvalidArgument));
				break;
			case EPERM:         OnDisconnect(SocketException(SocketException::errPermissionDenied));
				break;
            case EMFILE:
			case ENOLCK:        OnDisconnect(SocketException(SocketException::errNoDescriptors));
				break;
#else
			case WSAEINTR:         OnDisconnect(SocketException(SocketException::errInterrupted));
				break;
			case WSAEACCES:
			case WSAEWOULDBLOCK:        OnDisconnect(SocketException(SocketException::errWouldBlock));
				break;
			case WSAEBADF:         OnDisconnect(SocketException(SocketException::errBadDescriptor));
				break;
			case WSAEFAULT:
			case WSAEINVAL:  OnDisconnect(SocketException(SocketException::errInvalidArgument));
				break;
			case WSAEMFILE:
			case ENOLCK:        OnDisconnect(SocketException(SocketException::errNoDescriptors));
				break;
#endif
			default:            OnDisconnect(SocketException(SocketException::errUnknown, errno));
        }
    }
    ClearMutex();

    return iResult;
}

/******************************************************************************/
/* End of File: socketbase.cpp                                                */
/******************************************************************************/
