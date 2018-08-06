//
//  GameCommand.h
//  majainging
//
//  Created by Gao Yuan on 12年11月7日.
//  Copyright (c) 2012年 Yuan. All rights reserved.
//

#ifndef GameCommand_h
#define GameCommand_h
#include "cocos2d.h"

class GameCommand: public cocos2d::CCObject
{
    
public:
    GameCommand();
    void init(char *pBytes, const unsigned long nSize);
    ~GameCommand();
    
    
    static GameCommand* create(char *pBytes, const unsigned long nSize);
    
public:
    char* m_pBytes;
    unsigned long m_nSize;

    
};

class GameEvent : public cocos2d::CCObject
{
public:
    cocos2d::CCCallFunc*     _pSelector;      /// callback function, e.g. MyLayer::onHttpResponse(CCObject *sender, void *data)
    void*              _pUserData;      /// You can add your customed data here
    unsigned long              _size;
    
public:
    GameEvent();
    void init(void *pData, const unsigned long size, cocos2d::CCCallFunc* pSelector);
    ~GameEvent();
    
    static GameEvent* create(void *pData, const unsigned long size, cocos2d::CCCallFunc* pSelector);
};



class RecvCommand : public cocos2d::CCObject
{

private:
	void*		   _pUserData;
	unsigned short _size;
public:
    
	RecvCommand();
	~RecvCommand();

	void init(void *pData, unsigned short size);
	void* getData();
	unsigned short getSize();

	static RecvCommand* create(void *pData, unsigned short size);

};


#endif
