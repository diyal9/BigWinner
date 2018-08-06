//
//  CLPlayerManager.cpp
//  BigWinner
//
//  Created by ZhangQiyun on 13-10-14.
//
//

#include "CLPlayerManager.h"

static CLPlayerManager* g_pPlayerManager = NULL;

CLPlayerManager::CLPlayerManager()
{
    m_pUserInfo = NULL;
}

CLPlayerManager::~CLPlayerManager()
{
    delete m_pUserInfo;
}

CLPlayerManager* CLPlayerManager::shardPlayerManager()
{
    if(!g_pPlayerManager)
    {
        g_pPlayerManager = new CLPlayerManager();
    }
    return g_pPlayerManager;
}

void CLPlayerManager::init(Json::Value& jv)
{
    m_pUserInfo = new UserInfo();
    m_pUserInfo->initUserInfo(jv);
}


