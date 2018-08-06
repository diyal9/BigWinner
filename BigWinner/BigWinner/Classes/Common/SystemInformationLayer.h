//
//  SystemInformationLayer.h
//  BigWinner
//
//  Created by LunaBox on 13-11-27.
//
//

#ifndef __BigWinner__SystemInformationLayer__
#define __BigWinner__SystemInformationLayer__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class SystemInformationLayer :
public CCLayer
{
public:
	SystemInformationLayer(void);
	~SystemInformationLayer(void);
    
	CREATE_FUNC(SystemInformationLayer);
	virtual bool init();
    
	virtual void update(float delta);
    //	CCLabelTTF* text1;
	CCLabelTTF* text2;
    CC_SYNTHESIZE(std::string, m_STTFStr, STTFStr);
    CCSprite* textbase;
    
};

#endif /* defined(__BigWinner__SystemInformationLayer__) */
