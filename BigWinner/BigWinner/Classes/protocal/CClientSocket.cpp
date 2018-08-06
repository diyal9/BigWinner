
#include "CClientSocket.h"
#include "GameCommand.h"
#include "CRequestMsgConstDedine.h"
#include "CLNetWorkManager.h"

USING_NS_CC;
using namespace std;

/*
TCPRecvThread 
接收线程
*/
class TCPRecvThread : public ThreadBase
{
public:
	TCPRecvThread() : ThreadBase() {}
	virtual   ~TCPRecvThread() {  m_Socket = NULL; }

	void	SetSocket(CClientSocket*	pSocket) {m_Socket = pSocket; m_Socket->retain();};

	virtual void *      Execute();
	virtual void        CleanUp();
private:
	CClientSocket*	m_Socket;
};

/*
TCPSendThread
*/
class TCPSendThread : public ThreadBase
{
public:
	TCPSendThread() : ThreadBase() {}
	virtual   ~TCPSendThread() { m_Socket = NULL; }

	void	SetSocket(CClientSocket*	pSocket) {m_Socket = pSocket; m_Socket->retain();};

	virtual void *      Execute();
	virtual void        CleanUp();

private:
	CClientSocket*	m_Socket;
};

void * TCPRecvThread::Execute()
{
	while(m_Socket && !m_Socket->IsDisconnecting())
	{
		if (!ThreadRunning())
		{
			break;
		}

		if (m_Socket)
		{
			m_Socket->ThreadWorkRecvData();
		}
	}

	return NULL;
}

void TCPRecvThread::CleanUp()
{
	CC_SAFE_RELEASE_NULL(m_Socket);
}


void * TCPSendThread::Execute()
{
	while(m_Socket && !m_Socket->IsDisconnecting())
	{
		if (!ThreadRunning())
		{
			break;
		}

		if (m_Socket)
		{
			m_Socket->ThreadWorkSendData();
		}
	}

	return NULL;
}

void TCPSendThread::CleanUp()
{
	CC_SAFE_RELEASE_NULL(m_Socket);
	
}

RecvCommand* CClientSocket::getRecvCommand()
{
	
	RecvCommand* pRecv = NULL;
	pthread_mutex_lock(&m_arrRecvMutex);
	if (m_arrRecv->count() > 0) {

		pRecv = (RecvCommand*) m_arrRecv->objectAtIndex(0);
	}
	pthread_mutex_unlock(&m_arrRecvMutex);

	return pRecv;
}

void CClientSocket::removeRecvCommand()
{
	pthread_mutex_lock(&m_arrRecvMutex);
	if (m_arrRecv->count() > 0) {
		m_arrRecv->removeObjectAtIndex(0);
		//CCLOGWARN("current recvArr queue count:%d", m_arrRecv->count());
	}
	pthread_mutex_unlock(&m_arrRecvMutex);
}


bool CClientSocket::init()
{

	return true;
}

TCPClientSocket* CClientSocket::getClientSocket()
{
	return m_pClientSocket;
}

CClientSocket::CClientSocket()
:m_wRecvSize(0)
,m_pClientSocket(NULL)
{

	m_RecvThread = new TCPRecvThread();
	m_SendThread = new TCPSendThread();


	m_arrRecv = CCArray::create();
	m_arrRecv->retain();

	m_arrSend = CCArray::create();
	m_arrSend->retain();

	pthread_mutex_init(&m_arrSendMutex, NULL);
	pthread_mutex_init(&m_arrRecvMutex, NULL);

	m_Disconnecting = true;

}

CClientSocket::~CClientSocket()
{
	CC_SAFE_RELEASE_NULL(m_arrRecv);
	CC_SAFE_RELEASE_NULL(m_arrSend);
	
	Release();

}

void CClientSocket::Release()
{
	try
	{
	
		if (m_RecvThread)
		{
			delete m_RecvThread;
			m_RecvThread = NULL;
		}

		if (m_SendThread)
		{
			delete m_SendThread;
			m_SendThread = NULL;
		}

		if (m_pClientSocket)
		{
			delete m_pClientSocket;
			m_pClientSocket = NULL;
		}
	}

	catch (SocketException &excep)
	{
		cocos2d::CCLog("yuan-Release-Socket Exception :\n\n");
		return ;
	}
	catch (...)
	{
		cocos2d::CCLog("yuan-Release-Socket Other Error\n\n");
		return ;
	}

}

//此处连接成功的话开启线程队列和网络队列
bool CClientSocket::Connect(const char* szServerIP, int wPort)
{
    IPAddress address;
    
    address.SetHostName(szServerIP, false);
    
    m_pClientSocket = new TCPClientSocket(address,wPort);
    if (!m_pClientSocket)
    {
        //异常处理
        return false;
    }
    
    //设置当前网络类状态 目前暂不需要
    //		setcurrentStatus(NS_IDLE);
    
    cocos2d::CCLog(" - Family : %s\n", (address.GetAddressFamily() == AF_INET6)?"IPv6":
                   
                   ((address.GetAddressFamily() == AF_INET)?"IPv4":"Unknown"));
    
    cocos2d::CCLog(" - Address : %s\n  Port:%d", address.GetAddressString(), wPort);
    
    
    SocketException & exception = m_pClientSocket->getException();
    if (SocketException::ErrorCodes(exception) != SocketException::errNone && m_Disconnecting)
    {
        //网络异常处理
        CCLOG("yuan-Connect-error");
        return false;
    }
    
    //当前网络连接成功
    m_Disconnecting = false;
    
    //开启收、发线程
    m_RecvThread->SetSocket(this);
    m_RecvThread->LaunchThread();
    
    m_SendThread->SetSocket(this);
    m_SendThread->LaunchThread();

    //设置IP复用 暂不需要
#if 0
	int resume = 1;
	m_pClientSocket->SetSockOpt(SOL_SOCKET,SO_REUSEADDR,(char *)&resume,sizeof(resume));

#endif
	return true;
} 

bool CClientSocket::IsDisconnecting()
{
	return m_Disconnecting;
}

//主动断开连接 此处并没有销毁网络类和线程 只是简单的停止线程以及关闭当前网络 那么再次请求连接时只是重新建立了m_pClientSocket
void CClientSocket::Disconnect()
{

	pthread_mutex_lock(&m_arrSendMutex);
	m_arrSend->removeAllObjects();
	pthread_mutex_unlock(&m_arrSendMutex);

	pthread_mutex_lock(&m_arrRecvMutex);
	m_arrRecv->removeAllObjects();
	pthread_mutex_unlock(&m_arrRecvMutex);

    
	m_Disconnecting = true;
	
	m_pClientSocket->CloseSocket();

}

void CClientSocket::ThreadWorkSendData()
{
	int count = 0;
	pthread_mutex_lock(&m_arrSendMutex);
	count = m_arrSend->count();
	pthread_mutex_unlock(&m_arrSendMutex);

    //说明队列有消息需要发送
	if (count > 0)
	{
		pthread_mutex_lock(&m_arrSendMutex);
		GameEvent* pEvent = (GameEvent*)m_arrSend->objectAtIndex(0);
		pthread_mutex_unlock(&m_arrSendMutex);

        //发送成功删掉
		if (0 == SendDataBuffer(pEvent->_pUserData, pEvent->_size))
		{
			pthread_mutex_lock(&m_arrSendMutex);
			m_arrSend->removeObjectAtIndex(0);
			pthread_mutex_unlock(&m_arrSendMutex);
			pEvent = NULL;

		}
	}
    else
    {
#ifdef WIN32
		Sleep(300);
#else
		usleep(300000);
#endif
    }

	return;
}

void CClientSocket::ThreadWorkRecvData()
{

	do
	{
		char recvbuff[SOCKET_PACKET] = {0};
		memset(recvbuff, 0, SOCKET_PACKET);
//		// 先取出包头
        int nRecv = 0;
        
		nRecv = recvData(recvbuff, 0, 4);
		if (nRecv == SOCKET_ERROR) 
		{ 
			// error
			CCLOGWARN("yuan-ThreadWorkRecvData-packetHead failed\n");
			break;
		} 

		// 获取数据包内容长度
        int dataContentLen = atoi(recvbuff);
        
		//244 0244
		CCLOG("gaoyuan NET recv len:%d",dataContentLen );
		//读取内容包体 略过包头
        
		nRecv = recvData(recvbuff, 4, dataContentLen);
		if (nRecv == SOCKET_ERROR) 
		{ 
			// error
			CCLOGWARN("yuan-ThreadWorkRecvData-packetTails failed\n");
			break;
		} 
        
        //添加到队列
        void*  pDataBuffer = new char[dataContentLen];
        memcpy(pDataBuffer, recvbuff + 4, dataContentLen);
        RecvCommand *pRecv = NULL;
        pRecv = RecvCommand::create(pDataBuffer, dataContentLen);
        pthread_mutex_lock(&m_arrRecvMutex);
        m_arrRecv->addObject(pRecv);
        pthread_mutex_unlock(&m_arrRecvMutex);
		
		CCLOG("gaoyuan NET recv realyNew:%s",(char*)pDataBuffer );

	} while (0);	

	return;

}


//发送命令
bool CClientSocket::sendCommand(void* const pData, int wDataSize)
{

	assert(wDataSize<=SOCKET_PACKET);
	if (wDataSize>SOCKET_PACKET)
		return false;

    char* cbDataBuffer = new char[SOCKET_BUFFER];
	memset(cbDataBuffer, 0, SOCKET_BUFFER);
    
    char nDataLen[4] = {0};
    sprintf(nDataLen, "%04d", wDataSize);
    std::string sData = nDataLen;
    sData += (char*)pData;

	//添加内容
	if (wDataSize>0)
	{
		assert(pData!=NULL);
		memcpy(cbDataBuffer, sData.c_str(), sData.size());
	}

	//添加到发送队列
	GameEvent *event = GameEvent::create(cbDataBuffer, strlen(cbDataBuffer), NULL);
	pthread_mutex_lock(&m_arrSendMutex);
	m_arrSend->addObject(event);
	pthread_mutex_unlock(&m_arrSendMutex);
    
	return true;

}

//真正的接收处理
int CClientSocket::recvData(char*buffer, int offset, int length)
{
    
	int recvLen = 0, tempRecvLen = 0;
	int serverAddr_size = sizeof(sockaddr);
	int needLen = length;
    
	while (needLen > 0)
	{
		
		tempRecvLen = m_pClientSocket->RecvData((char*)buffer + offset,needLen);
        //接收失败处理
		if (tempRecvLen == SOCKET_ERROR)//EAGAIN
		{
			CCLOGWARN("yuan- recvData errCode");
			m_Disconnecting = true;
			return -1;
		}
        else if (tempRecvLen == 0)
        {
#ifdef WIN32
            Sleep(300);
#else
            usleep(300000);
#endif
        }
        
		offset += tempRecvLen;
		recvLen += tempRecvLen;
		needLen -= tempRecvLen;
        
	}
    
	return recvLen;
}
//真正的发送处理
int CClientSocket::SendDataBuffer(void * pBuffer, int wSendSize)
{
	
	int wTotalCount=0;

	while (wTotalCount<wSendSize)
	{
		
		int nSendCount = m_pClientSocket->SendData((char *)pBuffer+wTotalCount,wSendSize-wTotalCount);

		
		if (nSendCount== -1)
		{
            //异常处理
            CCLOG("gaoyuan:send to server failed");
            m_Disconnecting = true;
			return 1;
		}
		else
		{
			
			wTotalCount+=nSendCount;

		}
	}
    CCLOGWARN("gaoyuan- NET SEND len:%d content:%s",wSendSize, (char*)pBuffer );
	return 0;

}
