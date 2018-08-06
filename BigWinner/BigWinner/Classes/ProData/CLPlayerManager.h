//
//  CLPlayerManager.h
//  BigWinner
//
//  Created by ZhangQiyun on 13-10-14.
//
//

#ifndef __BigWinner__CLPlayerManager__
#define __BigWinner__CLPlayerManager__

#include "cocos2d.h"
#include "UserInfo.h"
#include "JsonParser.h"
USING_NS_CC;

class CLPlayerManager
{
public:
    CLPlayerManager();
    ~CLPlayerManager();
    
    static CLPlayerManager* shardPlayerManager();
    
    void init(Json::Value& jv);
    
    CC_SYNTHESIZE(UserInfo*, m_pUserInfo, pUserInfo);   //储存玩家信息的结构指针
};

#endif /* defined(__BigWinner__CLPlayerManager__) */
