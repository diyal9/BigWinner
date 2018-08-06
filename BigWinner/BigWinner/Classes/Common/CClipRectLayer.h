//
//  CClipRectLayer.h
//  ModeStyle_X
//
//  Created by qiyun.zhang on 13-1-15.
//
//

#ifndef __ModeStyle_X__CClipRectLayer__
#define __ModeStyle_X__CClipRectLayer__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class CClipRectLayer : public CCLayerColor
{
public:
    CClipRectLayer(){}
    virtual ~CClipRectLayer(){}
    
    virtual void visit();
    virtual bool init(float fwidth, float fhight);
    
    static CClipRectLayer* create(float fwidth, float fhight);
};

#endif /* defined(__ModeStyle_X__CClipRectLayer__) */
