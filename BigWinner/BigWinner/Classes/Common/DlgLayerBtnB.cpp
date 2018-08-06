#include "DlgLayerBtnB.h"
#include "Button.h"
#include "LGameCommonDefine.h"
#include <CLSceneManager.h>
#include <CLChangeString.h>

USING_NS_CC;

DlgLayerBtnB* DlgLayerBtnB::createBtn(std::string btnNameLeft,std::string btnNameRight,std::string word)
{
	DlgLayerBtnB *btn = new DlgLayerBtnB();
	if (btn && btn->init(btnNameLeft,btnNameRight,word))
	{
		btn->autorelease();
		return btn;
	}
	CC_SAFE_DELETE(btn);
	return NULL;
}

bool DlgLayerBtnB::init(std::string btnNameLeft,std::string btnNameRight,std::string word)
{
    if(!CCLayer::init())
    {
        return false;
    }
                                                                    //
    
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    bool bRet = false;
	do
	{
		CCSprite *pSprite = CCSprite::create("Image/PveLayer/CP_GiveupLayerBG.png");
        pSprite->setPosition(ccp(size.width *0.5, size.height *0.5));
		this->addChild(pSprite,1,1);
        
        float len=strlen(word.c_str());
        
        int nlen = ceil(((len * 30) /400)/3);
        
        int lenY = nlen * 50;
        
        CCSprite* pTTFBG = CCSprite::create("Image/PveLayer/CP_GiveUpTTFBG.png");
        pTTFBG->setAnchorPoint(ccp(0, 1));
        pTTFBG->setPosition(ccp(G_SF(45), pSprite->getContentSize().height));
        pSprite->addChild(pTTFBG, 1);
        
        CCLabelTTF* pGiveTTF = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("PromptMessage").c_str(), "Arial", G_SF(30), CCSizeMake(G_SF(120),G_SF(40)), kCCTextAlignmentCenter);
        pGiveTTF->setPosition(ccp(pTTFBG->getContentSize().width/2, G_SF(39)));
        pGiveTTF->setColor(ccBLUE);
        pTTFBG->addChild(pGiveTTF);
        
        CCSprite* pGiveBG1 = CCSprite::create("Image/PveLayer/CP_GiveUpBG1.png");
        pGiveBG1->setPosition(ccp(pSprite->getContentSize().width/2, G_SF(240)));
        pSprite->addChild(pGiveBG1, 1);
        
        //提示消息
        CCLabelTTF* StrBtn = CCLabelTTF::create(word.c_str(), "Arial", G_SF(30), CCSizeMake(G_SF(400),G_SF(lenY)), kCCTextAlignmentCenter);
        StrBtn->setPosition(ccp(pGiveBG1->getContentSize().width * 0.5, pGiveBG1->getContentSize().height * 0.8 -G_SF(lenY/2)));
        StrBtn->setColor(ccBLACK);
        pGiveBG1->addChild(StrBtn);
        
        //左按钮
        pBtnLeft = Button::createBtn("Image/PublicUI/CommonIcon_Cancel.png","Image/PublicUI/CommonIcon_Cancel.png");
        pBtnLeft->setPosition(ccp(pSprite->getContentSize().width * 0.3, pSprite->getContentSize().height * 0.18));
        pSprite->addChild(pBtnLeft);
        pBtnLeft->setOnClickCallbackNode(callfuncN_selector(DlgLayerBtnB::leftCancle), this);
        pBtnLeft->setHandlerPriority(-100000001);
        
        //左按钮名称
        CCLabelTTF* labelLeft = CCLabelTTF::create(btnNameLeft.c_str(), "Arial", G_SF(40), CCSizeMake(G_SF(170), G_SF(120)), kCCTextAlignmentCenter);
        //颜色设置
        labelLeft->setPosition(ccp(G_SF(0), G_SF(-35)));
        pBtnLeft->addChild(labelLeft);
        
        //右边按钮
        pBtnRight = Button::createBtn("Image/PublicUI/CommonIcon_Ok.png","Image/PublicUI/CommonIcon_Ok.png");
        pBtnRight->setPosition(ccp(pSprite->getContentSize().width * 0.7, pSprite->getContentSize().height * 0.18));
        pSprite->addChild(pBtnRight);
        pBtnRight->setHandlerPriority(-100000001);
        
        //右边按钮名称
        CCLabelTTF* labelRight = CCLabelTTF::create(btnNameRight.c_str(), "Arial", G_SF(40), CCSizeMake(G_SF(170), G_SF(120)), kCCTextAlignmentCenter);
        //颜色设置
        labelRight->setPosition(ccp(G_SF(0), G_SF(-35)));
        pBtnRight->addChild(labelRight);

        
		bRet = true;
	} while (0);
	return bRet;
}


void DlgLayerBtnB::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -100003, true);
}

void DlgLayerBtnB::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool DlgLayerBtnB::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    return true;
}


void DlgLayerBtnB::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void DlgLayerBtnB::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void DlgLayerBtnB::leftCancle(CCNode* r)
{
    this->removeFromParentAndCleanup(true);
}
