//
//  BackPack.cpp
//  BigWinner
//
//  Created by LunaBox03 on 13-10-19.
//
//

#include "BackPack.h"

#include "LGameCommonDefine.h"
#include "Button.h"
#include "RoleUI.h"
#include "QuestionCardUI.h"
#include "PropsUI.h"
#include "CompoundSceneMain.h"
#include "CLSoundManager.h"


using namespace Json;

BackPackLayer* BackPackLayer::create(int index, Json::Value &data)
{
    BackPackLayer *pRet = new BackPackLayer();
	if (pRet && pRet->init(index, data))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool BackPackLayer::init(int index, Json::Value &data)
{
    if(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0)))
    {
        return false;
    }
    
    //数据解析
    m_nlayerTpye = index;
    m_bkData = data;
    
    cardInfoSet(data); //题卡信息
    
    CCLOG("data:%d", m_bkData["playerrole"].size());
    
    //初始化UI
    doBaseUI();
    
    return true;
}

void BackPackLayer::doBaseUI()
{
    
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();

    //bg
    CCSprite* bg = CCSprite::create("Image/BackPack/BKShop2.png");
    this->addChild(bg);
    bg->setPosition(ccp(size.width*0.5, size.height*0.43));
    
    //关闭
    Button *shutdown = Button::createBtn("Image/PublicUI/Close.png");
    shutdown->setPosition(ccp(G_SF(550), G_SF(765)));
    shutdown->setOnClickCallbackNode(callfuncN_selector(BackPackLayer::menuCallBackButton), this);
    shutdown->setHandlerPriority(-10001);
    bg->addChild(shutdown,0,BK_GUANBI_TAG);
    
    //背包
    CCSprite *beibao = CCSprite::create("Image/PublicUI/TitleBackground.png");
    beibao->setPosition(ccp(G_SF(75), G_SF(765)));
    bg->addChild(beibao,0);
    
    //背包文字
    CCSprite *bbwenzi = CCSprite::create("Image/BackPack/BackPack.png");
    bbwenzi->setPosition(ccp(G_SF(80), G_SF(25)));
    beibao->addChild(bbwenzi,100);
    
    //合成
    Button *hecheng = Button::createBtn("Image/BackPack/Titlebtn.png");
    hecheng->setPosition( ccp(G_SF(80), G_SF(640)) );
    hecheng->setOnClickCallbackNode(callfuncN_selector(BackPackLayer::menuCallBackButton), this);
    hecheng->setHandlerPriority(-10001);
    bg->addChild(hecheng,0,BK_HECHENG_TAG);
    
    //合成文字
    CCSprite *chwenzi = CCSprite::create("Image/BackPack/Compound.png");
    chwenzi->setPosition(ccp(G_SF(0), G_SF(0)));
    hecheng->addChild(chwenzi,1);
    
    //角色
    Button *juese = Button::createBtn("Image/BackPack/Titlebtn.png");
    juese->setPosition( ccp(G_SF(80), G_SF(470)) );
    juese->setOnClickCallbackNode(callfuncN_selector(BackPackLayer::menuCallBackButton), this);
    juese->setHandlerPriority(-10001);
    bg->addChild(juese,0,BK_JUESE_TAG);
    
    //角色文字
    CCSprite *jswenzi = CCSprite::create("Image/BackPack/Role.png");
    jswenzi->setPosition(ccp(G_SF(0), G_SF(0)));
    juese->addChild(jswenzi,1);
    
    //提卡
    Button *tika = Button::createBtn("Image/BackPack/Titlebtn.png");
    tika->setPosition( ccp(G_SF(80), G_SF(300)) );
    tika->setOnClickCallbackNode(callfuncN_selector(BackPackLayer::menuCallBackButton), this);
    tika->setHandlerPriority(-10001);
    bg->addChild(tika,0,BK_TIKA_TAG);
    
    //题卡文字
    CCSprite *tkwenzi = CCSprite::create("Image/BackPack/card.png");
    tkwenzi->setPosition(ccp(G_SF(0), G_SF(0)));
    tika->addChild(tkwenzi,1);
    
    //道具
    Button *daoju = Button::createBtn("Image/BackPack/Titlebtn.png");
    daoju->setPosition( ccp(G_SF(80), G_SF(130)) );
    daoju->setOnClickCallbackNode(callfuncN_selector(BackPackLayer::menuCallBackButton), this);
    daoju->setHandlerPriority(-10001);
    bg->addChild(daoju,0,BK_DAOJU_TAG);
    
    //道具文字
    CCSprite *djwenzi = CCSprite::create("Image/BackPack/Props.png");
    djwenzi->setPosition(ccp(G_SF(0), G_SF(0)));
    daoju->addChild(djwenzi,1);

    //第一次默认加载ui
    this->scheduleOnce(schedule_selector(BackPackLayer::addBaseUI), 0);
    
}

void BackPackLayer::addBaseUI()
{
    //加载默认UI
    QuestionCardUI* hechengLay = QuestionCardUI::create();
    this->addChild(hechengLay,10, BK_GENERAL_TAG);
    hechengLay->setPosition(ccp(G_SF(10), G_SF(10)));
    m_nlayerTpye = BK_TIKA;
    
    if(this->getntag()!= 0)
    {
        this->getChildByTag(this->getntag())->removeFromParentAndCleanup(true);
    }
    this->setntag(BK_TIKA_TAG2);
    //提卡
    Button *tika = Button::createBtn("Image/BackPack/Titlebtn2.png");
    tika->setPosition( ccp(G_SF(90), G_SF(310)) );
    tika->setOnClickCallbackNode(callfuncN_selector(BackPackLayer::menuCallBackButton), this);
    tika->setHandlerPriority(-10001);
    this->addChild(tika,0,BK_TIKA_TAG2);
    
    //题卡文字
    CCSprite *tkwenzi = CCSprite::create("Image/BackPack/card.png");
    tkwenzi->setScaleX(1.1);
    tkwenzi->setPosition(ccp(G_SF(0), G_SF(0)));
    tika->addChild(tkwenzi,1);
    
       
    //刷新位置
    setIndexPosition();
    
    doAction(this, true);

}

//动作执行
void BackPackLayer::doAction(CCNode* r, bool moveIn)
{

    if (moveIn) //进场动作
    {
        CCLayerColor *fadeLayer = CCLayerColor::create();
        this->getParent()->addChild(fadeLayer,9,500);
        
        CCFiniteTimeAction *actionOut = CCSequence::create(CCFadeTo::create(0.4f, 255*0.5), NULL); //透明度动画
        fadeLayer->runAction(actionOut);
        
        CCAction *actionMoveOut = CCEaseExponentialOut::create(CCMoveTo::create(0.4f, ccp(0, 0))); //快到慢
        this->runAction(actionMoveOut);
        
        //窗体开闭音乐
        CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_WInClose.wav");
    }
    else //出场动作
    {
        CCFiniteTimeAction *actionIn = CCSequence::create(CCFadeTo::create(0.4f, 0), NULL); //透明度动画
        this->getParent()->getChildByTag(500)->runAction(actionIn);
        
        CCAction *actionMoveIn = CCEaseExponentialIn::create(CCMoveTo::create(0.4f, ccp(r->getPositionX(), r->getPositionY() * (-1)))); //慢到快
        r->runAction(actionMoveIn);
        
        this->getParent()->getChildByTag(500)->removeFromParentAndCleanup(true);
    }
    
}

void BackPackLayer::menuCallBackButton(CCNode* r)
{
    //窗体开闭音乐
    CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_WInClose.wav");

    CCNode* node = this->getChildByTag(BK_GENERAL_TAG);
    if (node)
    {
        node->removeAllChildrenWithCleanup(true);
        node->removeFromParentAndCleanup(true);
        m_nlayerTpye = 0;
        
    }
    
    switch (r->getTag())
    {
       
        case BK_HECHENG_TAG:
        case BK_HECHENG_TAG2:
        {
            if(this->getntag()!= 0)
            {
                this->getChildByTag(this->getntag())->removeFromParentAndCleanup(true);
            }
            
            this->setntag(BK_HECHENG_TAG2);
            //合成选中
            Button *hecheng = Button::createBtn("Image/BackPack/Titlebtn2.png");
            hecheng->setPosition( ccp(G_SF(90), G_SF(650)) );
            hecheng->setOnClickCallbackNode(callfuncN_selector(BackPackLayer::menuCallBackButton), this);
            hecheng->setHandlerPriority(-10001);
            this->addChild(hecheng,0,BK_HECHENG_TAG2);
            
            //合成文字选中
            CCSprite *hcwenzi = CCSprite::create("Image/BackPack/Compound.png");
            hcwenzi->setScaleX(1.1);
            hcwenzi->setPosition(ccp(G_SF(0), G_SF(0)));
            hecheng->addChild(hcwenzi,1);

            CCLOG("合成");
            if (m_nlayerTpye == BK_HECHENG)
                return;
            
            CompoundSceneMain* hechengLay = CompoundSceneMain::create();
            this->addChild(hechengLay,10, BK_GENERAL_TAG);
            hechengLay->setPosition(ccp(G_SF(10), G_SF(0)));
            
            m_nlayerTpye = BK_HECHENG;
            
        }
            break;
        case BK_JUESE_TAG:
        case BK_JUESE_TAG2:
        {
            if(this->getntag()!= 0)
            {
                this->getChildByTag(this->getntag())->removeFromParentAndCleanup(true);
            }
            
            this->setntag(BK_JUESE_TAG2);
            //角色选中
            Button *juese = Button::createBtn("Image/BackPack/Titlebtn2.png");
            juese->setPosition( ccp(G_SF(90), G_SF(480)) );
            juese->setOnClickCallbackNode(callfuncN_selector(BackPackLayer::menuCallBackButton), this);
            juese->setHandlerPriority(-10001);
            this->addChild(juese,0,BK_JUESE_TAG2);
            
            //角色文字选中
            CCSprite *jswenzi = CCSprite::create("Image/BackPack/Role.png");
            jswenzi->setScaleX(1.1);
            jswenzi->setPosition(ccp(G_SF(0), G_SF(0)));
            juese->addChild(jswenzi,1);
            
             CCLOG("角色");
            if (m_nlayerTpye == BK_JUESE)
                return;
            
            RoleUI* hechengLay = RoleUI::create();
            this->addChild(hechengLay,10, BK_GENERAL_TAG);
            hechengLay->setPosition(ccp(G_SF(10), G_SF(10)));
            
            m_nlayerTpye = BK_JUESE;
        }
            break;
        case BK_TIKA_TAG:
        case BK_TIKA_TAG2:
        {

            if(this->getntag()!= 0)
            {
                this->getChildByTag(this->getntag())->removeFromParentAndCleanup(true);
            }
            this->setntag(BK_TIKA_TAG2);
            //提卡选中
            Button *tika = Button::createBtn("Image/BackPack/Titlebtn2.png");
            tika->setPosition( ccp(G_SF(90), G_SF(310)) );
            tika->setOnClickCallbackNode(callfuncN_selector(BackPackLayer::menuCallBackButton), this);
            tika->setHandlerPriority(-10001);
            this->addChild(tika,0,BK_TIKA_TAG2);
            
            //题卡文字选中
            CCSprite *tkwenzi = CCSprite::create("Image/BackPack/card.png");
            tkwenzi->setScaleX(1.1);
            tkwenzi->setPosition(ccp(G_SF(0), G_SF(0)));
            tika->addChild(tkwenzi,1);
            
             CCLOG("提卡");
            if (m_nlayerTpye == BK_TIKA)
                return;
            
            QuestionCardUI* hechengLay = QuestionCardUI::create();
            this->addChild(hechengLay,10, BK_GENERAL_TAG);
            hechengLay->setPosition(ccp(G_SF(10), G_SF(10)));
            
            m_nlayerTpye = BK_TIKA;
        }
            break;
        case BK_DAOJU_TAG:
        case BK_DAOJU_TAG2:
        {
            if(this->getntag()!= 0)
            {
                this->getChildByTag(this->getntag())->removeFromParentAndCleanup(true);
            }
            
            this->setntag(BK_DAOJU_TAG2);
            //道具选中
            Button *daoju = Button::createBtn("Image/BackPack/Titlebtn2.png");
            daoju->setPosition( ccp(G_SF(90), G_SF(140)) );
            daoju->setOnClickCallbackNode(callfuncN_selector(BackPackLayer::menuCallBackButton), this);
            daoju->setHandlerPriority(-10001);
            this->addChild(daoju,0,BK_DAOJU_TAG2);
            
            //道具文字选中
            CCSprite *djwenzi = CCSprite::create("Image/BackPack/Props.png");
            djwenzi->setScaleX(1.1);
            djwenzi->setPosition(ccp(G_SF(0), G_SF(0)));
            daoju->addChild(djwenzi,1);
            
             CCLOG("道具");
            if (m_nlayerTpye == BK_DAOJU)
                return;
            
            PropsUI* hechengLay = PropsUI::create();
            this->addChild(hechengLay,10, BK_GENERAL_TAG);
            hechengLay->setPosition(ccp(G_SF(10), G_SF(10)));
            
            m_nlayerTpye = BK_DAOJU;
        }
            break;
            
        case BK_GUANBI_TAG:
        {
            doAction(this, false);
            
            this->removeAllChildrenWithCleanup(true);
            this->removeFromParentAndCleanup(true);
            return;
        }
        default:
            break;
    }
    
    setIndexPosition();
}

void BackPackLayer::setIndexPosition()
{
//    m_indesSprite->setVisible(true);
    
//    switch (m_nlayerTpye)
//    {
//        case BK_HECHENG:
//        {
//            m_indesSprite->setPosition(ccp(m_indesSprite->getParent()->getContentSize().width*0.5, G_SF(500)));
//        }
//            break;
//        case BK_JUESE:
//        {
//            m_indesSprite->setPosition(ccp(m_indesSprite->getParent()->getContentSize().width*0.5, G_SF(370)));
//        }
//            break;
//        case BK_TIKA:
//        {
//            m_indesSprite->setPosition(ccp(m_indesSprite->getParent()->getContentSize().width*0.5, G_SF(250)));
//        }
//            break;
//        case BK_DAOJU:
//        {
//          m_indesSprite->setPosition(ccp(m_indesSprite->getParent()->getContentSize().width*0.5, G_SF(120)));
//        }
//            break;
//            
//        default:
//            break;
//    }

}


void BackPackLayer::onEnter()
{
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);
}

void BackPackLayer::onExit()
{
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}



bool BackPackLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(this->isVisible())
        return true;
    else
        return false;
}

void BackPackLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void BackPackLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}


void BackPackLayer::cardInfoSet(Json::Value &value)
{
    m_pCardList.clear();
    
    for (int i = 0; i < value["card"].size(); i++)
    {
        int No = value["card"][i]["No"].asIntModestyle();
        
        int Number = value["card"][i]["Number"].asIntModestyle();
        
        if (index == 0)
            continue;
        
        insertData(No); //存储序列数据
        
        m_pCardMap[No] = Number; //存储卡牌数量
        
//        CCLog("TmpCardInfo->%d", No);
    }
}

void BackPackLayer::insertData(int No)
{
    if (No == 0) {
        return;
    }
    m_pCardList.push_back(No);
}

int BackPackLayer::getNumByNo(int key)
{
    std::map<int,int>::iterator it = m_pCardMap.find(key);
    if (it != m_pCardMap.end()) {
        return (*it).second;
        
    } else {
        return NULL;
    }
    
}






