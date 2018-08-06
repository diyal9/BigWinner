//
//  CClientSocket_h.h
//  
//
//  Created by yuan
//


#ifndef CClientSocket_h
#define CClientSocket_h


#include "socketcc.h"
#include "cocos2d.h"

class  TCPRecvThread;
class  TCPSendThread;
class  RecvCommand;
/*
	SOCKET类
 */
class CClientSocket:public cocos2d::CCObject
{

    //当前网络状态
	CC_SYNTHESIZE(int, currentStatus, currentStatus);
	

public:

	CREATE_FUNC(CClientSocket);
	CClientSocket();
	~CClientSocket();

    //初始化SOCKET类
	bool init();

	//调用请求一个真正的网络请求 包括创建线程
	bool            Connect(const char* szServerIP, int wPort);
	
    //断开连接
	void            Disconnect();

    //是否断开连接
	bool	        IsDisconnecting();


    //获取TCP对象
	TCPClientSocket* getClientSocket();

	
public:
    
    //获取接收的队列内容，每次取出一条
	RecvCommand*	getRecvCommand();

    //删除接收队列内容，每次删除一条
	void			removeRecvCommand();
    
    //发送数据包
	bool sendCommand(void* const pData, int wDataSize);

    //发送线程
	void			ThreadWorkSendData();
    
    //接收线程
	void			ThreadWorkRecvData();
    
protected:
    //真正的发送数据包
	int SendDataBuffer(void * pBuffer, int wSendSize);
    
    //接收数据内容接口
	int recvData(char *buffer, int offset, int length);
    
    //清除线程网络 数据等
	void			Release();
    
    

public:
	cocos2d::CCArray*				m_arrSend;
	pthread_mutex_t					m_arrSendMutex;

	cocos2d::CCArray*				m_arrRecv;
	pthread_mutex_t					m_arrRecvMutex;

	unsigned short					m_wRecvSize;			//接收长度
	char							m_cbRecvBuf[81900];		//接收缓冲
public://修改为public
	
	TCPClientSocket*				m_pClientSocket;
	
private:


	TCPRecvThread*					m_RecvThread;
	TCPSendThread*					m_SendThread;
	bool							m_Disconnecting;


};

#endif /* defined(__ModeStyle_X__CDownloadManger__) */
