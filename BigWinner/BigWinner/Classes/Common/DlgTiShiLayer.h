//
//  TiShiLayer.h
//  BigWinner
//
//  Created by LunaBox on 13-11-3.
//
//

#ifndef __BigWinner__DlgTiShiLayer__
#define __BigWinner__DlgTiShiLayer__

#include <iostream>
#include <cocos2d.h>
#include "cocos-ext.h"

class TiShiLayer : public cocos2d::CCLayerColor
{
public:
    TiShiLayer(){fagR = true;}
    ~TiShiLayer(){};
    
    static TiShiLayer* create(std::string Str);
    
	virtual bool init(std::string Str);
    
    //悬浮提示层
    void tiShiView();
    
    //去掉悬浮层
    void removeUI();
    
    //设置该弹出层不可同时存在两个
    bool fagR;
    
};

#endif /* defined(__BigWinner__TiShiLayer__) */
