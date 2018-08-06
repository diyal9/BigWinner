//
//  GameCommand.cpp
//  majainging
//
//  Created by Gao Yuan on 12年11月7日.
//  Copyright (c) 2012年 Yuan. All rights reserved.
//

#include <iostream>
#include "GameCommand.h"


GameCommand::GameCommand()
{}

void GameCommand::init(char *pBytes, const unsigned long nSize)
{
    m_pBytes = pBytes;
    m_nSize = nSize;
}

GameCommand* GameCommand::create(char *pBytes, const unsigned long nSize)
{
    GameCommand* p = new GameCommand;
    p->init(pBytes, nSize);
    p->autorelease();
    return p;
}

GameCommand::~GameCommand()
{
    //CC_SAFE_DELETE_ARRAY(m_pBytes);
    delete (m_pBytes);
    m_pBytes = NULL;
}


GameEvent::GameEvent()
{
    
}

void GameEvent::init(void *pData, const unsigned long size, cocos2d::CCCallFunc* pSelector)
{
    _pUserData = pData;
    _size = size;
    _pSelector = pSelector;
}

GameEvent* GameEvent::create(void *pData, const unsigned long size, cocos2d::CCCallFunc* pSelector)
{
    GameEvent* p = new GameEvent;
    p->init(pData, size, pSelector);
    p->autorelease();
    return p;
}

GameEvent::~GameEvent()
{
    //CC_SAFE_DELETE_ARRAY(m_pBytes);
    delete (_pUserData);
    _pUserData = NULL;
}

RecvCommand::RecvCommand()
:_pUserData(NULL),
_size(0)
{

}

void RecvCommand::init(void *pData, unsigned short size)
{
    _pUserData = pData;
    _size = size;
}

RecvCommand* RecvCommand::create(void *pData, unsigned short size)
{
    RecvCommand* p = new RecvCommand;
    p->init(pData, size);
    p->autorelease();
    return p;
}

RecvCommand::~RecvCommand()
{
    delete _pUserData;
    _pUserData = NULL;
}


void* RecvCommand::getData()
{
    return _pUserData;
}

unsigned short RecvCommand::getSize()
{
    return _size;
}



