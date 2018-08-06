//
//  CLChangeString.h
//  LolAllStars1_X
//
//  Created by Zhang Qiyun on 13-5-3.
//
//

#ifndef __LolAllStars1_X__CLChangeString__
#define __LolAllStars1_X__CLChangeString__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class CLChangeString
{
public:
    CLChangeString();
    virtual ~CLChangeString();
    
    static CLChangeString* sharedChangeString();
    bool init(const char* fileName);
    std::string GetChangeString(const char* keyName);
    std::string GetTTFName(int nIndex);               //得到自定义自体名称
    
private:
    std::map<std::string, std::string> m_vStringList;
    
};


#endif /* defined(__LolAllStars1_X__CLChangeString__) */
