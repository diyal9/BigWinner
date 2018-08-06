//
//  CClipRectLayer.cpp
//  ModeStyle_X
//
//  Created by qiyun.zhang on 13-1-15.
//
//

#include "CClipRectLayer.h"

void CClipRectLayer::visit()
{
#define CLIPFUNCTION_ENABLED   1
#if(CLIPFUNCTION_ENABLED)
    if (!this->isVisible())
		return;
    
    GLboolean alreadyEnabled = glIsEnabled(GL_SCISSOR_TEST);
    if (!alreadyEnabled) {
        glEnable(GL_SCISSOR_TEST);
    }
	//glEnable(GL_SCISSOR_TEST);
    CCPoint origin = this->convertToWorldSpace(CCPointZero);
    //    float scaleXY = CCApplication::sharedApplication()->getSceneScaleFactor();
    const CCSize& size = this->getContentSize();
    CCPoint dst = this->convertToWorldSpace(ccp(size.width, size.height));
    //    float scaleXY = CCApplication::sharedApplication()->getClipScaleFactor();
    //    CCLOG("sunjiajia--->>clipSize==:%f   width=:%f   height:%f",scaleXY,this->getContentSize().width,this->getContentSize().height);
    CCRect scissorRect = CCRectMake(origin.x,
                                    origin.y,
                                    dst.x-origin.x,
                                    dst.y-origin.y);
    
    GLint param[4];
    glGetIntegerv(GL_SCISSOR_BOX, param);
    
    GLint clip[4];
    clip[0] = scissorRect.origin.x;
    clip[1] = scissorRect.origin.y;
    clip[2] = scissorRect.size.width;
    clip[3] = scissorRect.size.height;
    
    if (alreadyEnabled) {
        CCRect::rectIntersection(clip, param);
    }
    
    glScissor(clip[0], clip[1], clip[2], clip[3]);
    
    //	glScissor((GLint) scissorRect.origin.x, (GLint) scissorRect.origin.y,
    //              (GLint) scissorRect.size.width, (GLint) scissorRect.size.height);
#endif
	CCLayer::visit();
#if(CLIPFUNCTION_ENABLED)
    if (!alreadyEnabled) {
        glDisable(GL_SCISSOR_TEST);
    } else {
        glScissor(param[0], param[1], param[2], param[3]);
    }
	//glDisable(GL_SCISSOR_TEST);
#endif
}

bool CClipRectLayer::init(float fwidth, float fhight)
{
    if(!CCLayerColor::init())
        return false;
    
    setContentSize(CCSizeMake(fwidth, fhight));
    return true;
}

CClipRectLayer* CClipRectLayer::create(float fwidth, float fhight)
{
    CClipRectLayer* player = new CClipRectLayer();
    if(player && player->init(fwidth, fhight))
    {
        player->autorelease();
        return player;
    }
    else
    {
        CC_SAFE_DELETE(player);
        return NULL;
    }

}


