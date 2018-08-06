#include "DlgLayerBtnA.h"
#include "Button.h"
#include "LGameCommonDefine.h"
#include <CLSceneManager.h>
#include <CLChangeString.h>

USING_NS_CC;


DlgLayerBtnA* DlgLayerBtnA::createBtn(std::string btnName ,std::string word)
{
	DlgLayerBtnA *btn = new DlgLayerBtnA;
	if (btn && btn->init(btnName,word))
	{
		btn->autorelease();
		return btn;
	}
	CC_SAFE_DELETE(btn);
	return NULL;
}

bool DlgLayerBtnA::init(std::string btnName ,std::string word)
{
    
    if(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0)))
    {
        return false;
    }
    
    CLSceneManager::sharedSceneManager()->getpParentNode()->addChild(this, DLG_GENERAL_ZORDER);
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    bool bRet = false;
	do
	{
		CCSprite *pSprite = CCSprite::create("Image/PveLayer/CP_GiveupLayerBG.png");
        pSprite->setPosition(ccp(size.width *0.5, size.height *0.5));
		this->addChild(pSprite,1,1);
        
        float len=strlen(word.c_str());
        
        int nlen = ceil(((len * 30) /400)/3);
        
        int lenY = nlen * 40;
        
        CCSprite* pTTFBG = CCSprite::create("Image/PveLayer/CP_GiveUpTTFBG.png");
        pTTFBG->setAnchorPoint(ccp(0, 1));
        pTTFBG->setPosition(ccp(G_SF(45), pSprite->getContentSize().height));
        pSprite->addChild(pTTFBG, 1);
        
        CCLabelTTF* pGiveTTF = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("PromptMessage").c_str(), "Arial", 30, CCSizeMake(G_SF(120),G_SF(30)), kCCTextAlignmentCenter);
        pGiveTTF->setPosition(ccp(pTTFBG->getContentSize().width/2, G_SF(39)));
        pGiveTTF->setColor(ccBLUE);
        pTTFBG->addChild(pGiveTTF);
        
        CCSprite* pGiveBG1 = CCSprite::create("Image/PveLayer/CP_GiveUpBG1.png");
        pGiveBG1->setPosition(ccp(pSprite->getContentSize().width/2, G_SF(240)));
        pSprite->addChild(pGiveBG1, 1);
        
        //提示消息
        CCLabelTTF* StrBtn = CCLabelTTF::create(word.c_str(), "Arial", 30, CCSizeMake(G_SF(400),G_SF(lenY)), kCCTextAlignmentCenter);
        StrBtn->setPosition(ccp(pGiveBG1->getContentSize().width * 0.5, pGiveBG1->getContentSize().height * 0.6 - lenY/2));
        StrBtn->setColor(ccBLACK);
        pGiveBG1->addChild(StrBtn);
        
        //按钮
        pButton = Button::createBtn("Image/PublicUI/CommonIcon_Ok.png","Image/PublicUI/CommonIcon_Ok.png");
        pButton->setPosition(ccp(pSprite->getContentSize().width * 0.5, pSprite->getContentSize().height * 0.18));
        pButton->setOnClickCallbackNode(callfuncN_selector(DlgLayerBtnA::QueRen), this);
        pSprite->addChild(pButton);
        pButton->setHandlerPriority(-200000);
        
        //按钮名称
        CCLabelTTF* label = CCLabelTTF::create(btnName.c_str(), "Arial", 40, CCSizeMake(G_SF(170), G_SF(120)), kCCTextAlignmentCenter);
        //颜色设置
        label->setPosition(ccp(G_SF(0), G_SF(-35)));
        pButton->addChild(label);
        
		bRet = true;
	} while (0);
	return bRet;
    
}

void DlgLayerBtnA::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10002, true);
}

void DlgLayerBtnA::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool DlgLayerBtnA::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    return true;
}


void DlgLayerBtnA::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void DlgLayerBtnA::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void DlgLayerBtnA::QueRen(CCNode* r)
{
    this->removeFromParentAndCleanup(true);
}