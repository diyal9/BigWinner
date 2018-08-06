//
//  CardSprite.h
//  BigWinner
//
//  Created by LunaBox on 13-11-29.
//
//

#ifndef __BigWinner__CardSprite__
#define __BigWinner__CardSprite__

#include <iostream>
#include <cocos2d.h>

USING_NS_CC;


class CardSprite:public CCLayer
{
    
public:
    CardSprite(){};
    ~CardSprite(){};
    static CardSprite* create(int No);
	virtual bool init(int No);
    
};

#endif /* defined(__BigWinner__CardSprite__) */
