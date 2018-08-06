//
//  CLPveResultLayer.cpp
//  BigWinner
//
//  Created by Zhang Qiyun on 13-10-26.
//
//  继续挑战按钮逻辑实现 20131221
//

#include "CLPveResultLayer.h"
#include "Button.h"
#include "LGameCommonDefine.h"
#include "CLPveSceneLayer.h"
#include "CLPlayerManager.h"
#include "CLChangeString.h"
#include "JsonDataManager.h"
#include "CLSceneManager.h"
#include "CLShelfsSceneLayer.h"
#include "CLPveSMangerLayer.h"
#include "CLNetWorkManager.h"
#include "CLStampExamineLayer.h"
#include "CLMainSceneLayer.h"
#include "CLAnimationManager.h"
#include "CardSprite.h"
#include "UserInfoSprite.h"
#include "CLSoundManager.h"

USING_NS_CC;

PassPointsSuccess::PassPointsSuccess()
{
    m_nStarLevel = 0; //胜利星级
    m_nCorrentNum = 0; //正确个数
    m_nMoneyNum = 0; //金额
    m_nShoppingCardNum = 0; //购物卡数
    m_ntampID = 0; //印花ID
    nMeYH1 = 0;
    nMeYH2 = 0;
}

PassPointsSuccess::~PassPointsSuccess()
{
    
}

PassPointsSuccess* PassPointsSuccess::create(Json::Value& jv)
{
    PassPointsSuccess* pLayer = new PassPointsSuccess();
    if(pLayer && pLayer->init(jv))
    {
        pLayer->autorelease();
        return pLayer;
    }
    else
    {
        CC_SAFE_DELETE(pLayer);
        return NULL;
    }
}

bool PassPointsSuccess::init(Json::Value& jv)
{
    
    if ( !CCLayerGradient::initWithColor(ccc4(0, 0, 0, 255), ccc4(102, 102, 102, 255)) )
    {  //渐变色设置
        return false;
    }
    
    serverInfoGet(jv);
    
    bool winStatus = true; //闯关状态
    if (m_nStarLevel == 0) { winStatus = false; };
    
    //获得屏幕大小
	CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    CCSprite* bg;
    if (winStatus) {
        bg = CCSprite::create("Image/PveLayer/C_PveWinBg.png");
        bg->setPosition(ccp(size.width * 0.5, size.height * 0.5));
        this->addChild(bg, 1);
    }
    else
    {
        bg = CCSprite::create("Image/PveLayer/C_PveFailBg.png");
        bg->setPosition(ccp(size.width * 0.5, size.height * 0.5));
        this->addChild(bg, 1);
    }
    
    //关闭按钮
    Button *pCloseMenu = Button::createBtn("Image/PublicUI/Close.png");
    pCloseMenu->setPosition(ccp(G_SF(574),G_SF(890)));
    pCloseMenu->setOnClickCallback(menu_selector(PassPointsSuccess::onCloseMenuPress),this);
    bg->addChild(pCloseMenu,1,3);
    
    //添加头像
    UserInfoSprite *userInfo = UserInfoSprite::create();
    userInfo->setPosition(ccp(G_SF(170), G_SF(889)));
    bg->addChild(userInfo);
    
    
    if (!winStatus) { //失败
        
        //失败音乐
        CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_PvpFail.wav");
        
        //wangguolong
        CLSceneManager::sharedSceneManager()->setnStarNumX(0);
        
        CCSprite *sSpr_Star = CCSprite::create("Image/PveLayer/C_FailedBg.png");  //背景星
        sSpr_Star->setPosition(ccp(size.width * 0.5, G_SF(1000)));
        bg->addChild(sSpr_Star,1001);
        
        CCSprite *winSpr = CCSprite::create("Image/PveLayer/C_FailedLab.png");
        winSpr->setPosition( ccp(sSpr_Star->getContentSize().width * 0.5, sSpr_Star->getContentSize().height * 0.6));
        sSpr_Star->addChild(winSpr);
        
        sSpr_Star->setAnchorPoint(ccp(0.5, 0));
        
        CCActionInterval* m_pAction = (CCActionInterval*)CCSequence::create(
                                                                            CCDelayTime::create(0.5),
                                                                            CCMoveTo::create(0.2, ccp(G_SF(320), G_SF(550))),
                                                                            CCScaleBy::create(0.1, 1.1,0.9),
                                                                            CCMoveTo::create(0.1, ccp(G_SF(320), G_SF(554))),
                                                                            CCScaleTo::create(0.1, 1.0,1.0),
                                                                            CCMoveTo::create(0.1, ccp(G_SF(320), G_SF(550))),
                                                                            NULL
                                                                            );
        sSpr_Star->runAction(m_pAction);
        
    }
    else
    {  //胜利
        
        //成功音乐
        CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_PvpWin.wav");
        
        if(m_nCorrentNum <6)
        {
            //wangguolong
            CLSceneManager::sharedSceneManager()->setnStarNumX(0);
        }else
        {
            //wangguolong
            CLSceneManager::sharedSceneManager()->setnStarNumX(1);
        }
        
        //星星播放效果
        std::string sFile;
        sFile = "flicker";
        
        CCAnimation* animation = CLAnimationManager::sharedAnimationManager()->animationWithName(sFile.c_str(),0.041);
        CCSprite* sSpr_Star = CLAnimationManager::sharedAnimationManager()->spriteWithAnimationName(sFile.c_str());
        sSpr_Star->setPosition(ccp(this->getContentSize().width*0.5,G_SF(1000)));
        this->addChild(sSpr_Star,2);
        
        CCActionInterval* m_pAction2 = (CCActionInterval*)CCSequence::create(
                                                                            CCAnimate::create(animation),
                                                                            NULL
                                                                            );
        
        sSpr_Star->runAction(CCRepeatForever::create(m_pAction2));
        
        CCSprite *winSpr = CCSprite::create("Image/PveLayer/CP_WinICon.png");
        winSpr->setPosition( ccp(sSpr_Star->getContentSize().width * 0.5, sSpr_Star->getContentSize().height * 0.6));
        sSpr_Star->addChild(winSpr);
        
        //星处理
        for (int i = 0; i < m_nStarLevel; i++) {
            CCSprite *star = CCSprite::create("Image/ShelfLayer/SL_Stars_N.png");
            star->setPosition(  ccp(G_SF(105)+ G_SF(95)*i, G_SF(80)));
            star->setScale(2.0f);
            sSpr_Star->addChild(star);
        }
        
        sSpr_Star->setAnchorPoint(ccp(0.5, 0));
        
        CCActionInterval* m_pAction = (CCActionInterval*)CCSequence::create(
                                                                            CCDelayTime::create(0.5),
                                                                            CCMoveTo::create(0.2, ccp(G_SF(320), G_SF(490))),
                                                                            CCScaleBy::create(0.1, 1.1,0.9),
                                                                            CCMoveTo::create(0.1, ccp(G_SF(320), G_SF(494))),
                                                                            CCScaleTo::create(0.1, 1.0,1.0),
                                                                            CCMoveTo::create(0.1, ccp(G_SF(320), G_SF(490))),
                                                                            NULL
                                                                            );
        sSpr_Star->runAction(m_pAction);
        
        for (int j = m_nStarLevel; j < 3; j++) {
            //            if (j < m_nStarLevel) {
            //                continue;
            //            }
            CCSprite *star = CCSprite::create("Image/ShelfLayer/SL_Stars_D.png");
            star->setPosition(  ccp(G_SF(105)+ G_SF(95)*j, G_SF(80)));
            star->setScale(2.0f);
            sSpr_Star->addChild(star);
        }
        
    }
    
    
    //小背景框添加
    CCSprite *getInfoBg = CCSprite::create("Image/PveLayer/C_BgWinLab.png");
    getInfoBg->setPosition( ccp(size.width * 0.5, size.height * 0.33));
    bg->addChild(getInfoBg, 10);
    
    //正确个数
    CCSprite *rightSpr = CCSprite::create("Image/PveLayer/C_RightLab.png");
    rightSpr->setPosition( ccp(size.width * 1 / 3, G_SF(320)));
    getInfoBg->addChild(rightSpr);
    
    std::string c_corrStr = CCString::createWithFormat("%d", m_nCorrentNum)->getCString();
    
    CCLabelTTF *corrStr = CCLabelTTF::create(c_corrStr.c_str(), "Arial", G_SF(30), CCSizeMake(G_SF(100), G_SF(40)), kCCTextAlignmentLeft);
	corrStr->setPosition(ccp(rightSpr->getPositionX() + rightSpr->getContentSize().width + G_SF(30), rightSpr->getPositionY()));
	getInfoBg->addChild(corrStr);
    
    //钞票
    CCSprite *moneySpr = CCSprite::create("Image/PveLayer/C_MoneyLab.png");
    moneySpr->setPosition( ccp(size.width * 2 / 3, rightSpr->getPositionY()));
    getInfoBg->addChild(moneySpr);
    
    std::string c_money =  "＋" ;
    c_money += CCString::createWithFormat("%d", m_nMoneyNum)->getCString();
    
    CCLabelTTF *moneyLable = CCLabelTTF::create(c_money.c_str(), "Arial", G_SF(25), CCSizeMake(G_SF(100), G_SF(35)), kCCTextAlignmentLeft);
	moneyLable->setPosition(ccp(moneySpr->getPositionX() + rightSpr->getContentSize().width + G_SF(30), rightSpr->getPositionY()));
	getInfoBg->addChild(moneyLable);
    
    //题卡
    float cardStartPo; //题卡起始位置x坐标
    
    switch (m_vTopCardMap.size()) {
        case 1:
            cardStartPo = 320; //只有一张卡牌
            break;
        case 2:
            cardStartPo = 230;
            break;
        case 3:
            cardStartPo = 150;
            break;
            
        default:
            break;
    }
    
    int i = 0;
    std::map<int,int>::iterator it;
    for(it=m_vTopCardMap.begin();it!=m_vTopCardMap.end();++it)
    {
        int cardNo = it->first;
        int cardNum = it->second;
        
        //添加闯关得到的卡牌
        CardSprite *cardBg = CardSprite::create(cardNo);
        cardBg->setPosition(ccp(G_SF(cardStartPo) + G_SF(170*i), G_SF(70)));
        cardBg->setScale(0.4f);
        getInfoBg->addChild(cardBg);
        
        std::string info = CCString::createWithFormat("x%d", cardNum)->getCString();
        
        CCLabelTTF *titleCardNum = CCLabelTTF::create(info.c_str(), "Arial", G_SF(25));
        titleCardNum->setPosition(ccp(cardBg->getPositionX() + G_SF(70), G_SF(46)));
        getInfoBg->addChild(titleCardNum);
        
        i++;
    }
    
    
    CCPoint continuePz = ccp(size.width * 0.5, G_SF(60)); //没捡到印花［继续挑战］坐标位置
    
    //查看印花 （只有在捡到印花的时候才会显示该按钮）
    if (m_ntampID && winStatus) {
        
        //商品图片路径
        std::string stampSmall_path = CCString::createWithFormat("Image/GoodIcon/StampSmall%d.png", m_ntampID)->getCString();
        
        //印花
        CCSprite *goodsSpr = CCSprite::create(stampSmall_path.c_str());
        goodsSpr->setPosition( ccp(size.width * 0.5, G_SF(200)));
        getInfoBg->addChild(goodsSpr);
        
        std::string c_stamp =  "x1" ;
        CCLabelTTF *goodsNumSpr = CCLabelTTF::create(c_stamp.c_str(), "Arial", 25);
        goodsNumSpr->setPosition(ccp(goodsSpr->getPositionX() + G_SF(70), goodsSpr->getPositionY() - G_SF(30)));
        getInfoBg->addChild(goodsNumSpr);
        
        nMeYH1 =  jv["yh1"].asIntModestyle();
        nMeYH2 =  jv["yh2"].asIntModestyle();
        //印花查看按钮
        Button *viewStampbtn = Button::createBtn("Image/PublicUI/Btn_gray.png");
        viewStampbtn->setPosition(ccp(G_SF(160), G_SF(60)));
        viewStampbtn->setOnClickCallback(menu_selector(PassPointsSuccess::ViewStampCall),this);
        bg->addChild(viewStampbtn,1,3);
        
        CCLabelTTF* lBtnTTF = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("ViewStamp").c_str(), "Arial", G_SF(30), CCSizeMake(G_SF(200), G_SF(40)), kCCTextAlignmentCenter);
        viewStampbtn->addChild(lBtnTTF, 10);
        lBtnTTF->setPosition(ccp(0, 0));
        continuePz = ccp(G_SF(480), G_SF(60));
    }
    
    //失败UI信息
    if (!winStatus) {
        
        //重新来过label
        CCLabelTTF* labelInfo = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("FailInfoLabel").c_str(), "Arial", G_SF(30), CCSizeMake(G_SF(550), G_SF(100)), kCCTextAlignmentCenter);
        labelInfo->setPosition(ccp(getInfoBg->getContentSize().width * 0.5, getInfoBg->getContentSize().height * 0.5));
        getInfoBg->addChild(labelInfo, 10);
        
        //放弃挑战按钮
        Button *viewStampbtn = Button::createBtn("Image/PublicUI/Btn_gray.png");
        viewStampbtn->setPosition(ccp(G_SF(160), G_SF(60)));
        viewStampbtn->setOnClickCallback(menu_selector(PassPointsSuccess::onCloseMenuPress),this);
        bg->addChild(viewStampbtn,1,3);
        
        CCLabelTTF* lBtnTTF = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("GiveUpPk").c_str(), "Arial", G_SF(30), CCSizeMake(G_SF(200), G_SF(50)), kCCTextAlignmentCenter);
        lBtnTTF->setPosition(ccp(G_SF(0), G_SF(0)));
        viewStampbtn->addChild(lBtnTTF, 10);
        
        continuePz = ccp(G_SF(480), G_SF(60));
    }
    
    
    //继续挑战
    Button *continueBtn = Button::createBtn("Image/BackPack/C_composite.png");
    continueBtn->setPosition(continuePz);
    continueBtn->setOnClickCallback(menu_selector(PassPointsSuccess::continueChallengeCall),this);
    bg->addChild(continueBtn,1,3);
    
    CCLabelTTF* lBtnConTTF = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("ContinuePk").c_str(), "Arial", G_SF(30), CCSizeMake(G_SF(200), G_SF(40)), kCCTextAlignmentCenter);
    lBtnConTTF->setPosition(ccp(0,0));
    continueBtn->addChild(lBtnConTTF, 10);
    
	return true;
}

void PassPointsSuccess::onCloseMenuPress(CCObject* pSender)
{
    CLPveSceneLayer* pParNode = (CLPveSceneLayer*)this->getParent();
    pParNode->ReturnToShelfsLayer();
}

//服务器数据取得
void PassPointsSuccess::serverInfoGet(Json::Value& jv)
{
    
    Json::Value data;
    data["msgid"].append("87"); //指令码
    data["No"] = jv["gdsno"].asIntModestyle();  //点击查看印花商品No
    
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_PRIZEINFO, data);
    
    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
    
    m_nStarLevel = jv["stars"].asIntModestyle();
    m_nCorrentNum = jv["rights"].asIntModestyle();
    if(jv.isMember("gdsno"))
        m_ntampID = jv["gdsno"].asIntModestyle();
    if(jv.isMember("jetton_add"))
        m_nMoneyNum = jv["jetton_add"].asIntModestyle();
    if(jv.isMember("Moneys"))
        m_nShoppingCardNum = jv["Moneys"].asIntModestyle();
    
    
    if(jv.isMember("cards"))
    {
        for(int i=0; i<jv["cards"].size(); i++)
        {
            
            m_vTopicCards.push_back(jv["card"][i].asIntModestyle());
            
            int caseCardNo = jv["cards"][i].asIntModestyle();
            
            std::map<int,int>::iterator it = m_vTopCardMap.find(caseCardNo);
            if (it != m_vTopCardMap.end()) {
                //map中存在该No，则只增加数量
                CCLog("info->%d", (*it).second);
                
                m_vTopCardMap[caseCardNo] = (*it).second + 1;
                
            } else {
                m_vTopCardMap[caseCardNo] = 1;
            }
            
        }
    }
    
}

//查看印花回调
void PassPointsSuccess::ViewStampCall()
{
    CCLog("view");
    CLStampExamineLayer *CLStampExamineLayer = CLStampExamineLayer::create(nMeYH1,nMeYH2);
    CLStampExamineLayer->setPosition(0, 0);
    this->addChild(CLStampExamineLayer,11);
}

//继续挑战回调
void PassPointsSuccess::continueChallengeCall()
{
    CLPveSceneLayer* pParNode = (CLPveSceneLayer*)this->getParent();
    pParNode->ContiueToPve();
}


void PassPointsSuccess::onEnter()
{
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);
}

void PassPointsSuccess::onExit()
{
    //wangguolong 删除对战印花查看按钮信息
    CLSceneManager::sharedSceneManager()->RemoveJsonDataFromMsgID(CMD_PRIZEINFO_REPLY);
    
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool PassPointsSuccess::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(this->isVisible())
        return true;
    else
        return false;
}




CLPveResultLayer* CLPveResultLayer::SuccessBoos(Json::Value& jv)
{
	CLPveResultLayer *btn = new CLPveResultLayer;
	if (btn && btn->initSuccess(jv))
	{
		btn->autorelease();
		return btn;
	}
	CC_SAFE_DELETE(btn);
	return NULL;
}

bool CLPveResultLayer::initSuccess(Json::Value& jv)
{
    
    //成功音乐
    CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_PvpWin.wav");
    
    intTag = 100;
    
    CCSprite *spriteBg = CCSprite::create("Image/PveLayer/C_PveWinBg.png");
    spriteBg->setPosition(ccp(G_SF(320), G_SF(480)));
    this->addChild(spriteBg, 1, 10086);
    
    CCMenu* pCloseMenu = CCMenu::create();
    pCloseMenu->setAnchorPoint(CCPointZero);
    pCloseMenu->setPosition(CCPointZero);
    pCloseMenu->setTouchPriority(-10000);
    spriteBg->addChild(pCloseMenu, 1);
    
    CCSprite* pSpr1 = CCSprite::create("Image/PublicUI/Close.png");
    CCSprite* pSpr2 = CCSprite::create("Image/PublicUI/Close.png");
    CCMenuItemSprite* pItem = CCMenuItemSprite::create(pSpr1, pSpr2, NULL, this, menu_selector(CLPveResultLayer::onCloseMenuPress));
    pItem->setPosition(ccp(G_SF(574),G_SF(920)));
    pCloseMenu->addChild(pItem, 1);
    
    //胜利图片
    CCSprite *winSprite = CCSprite::create("Image/PveLayer/CP_WinICon.png");
    winSprite->setPosition(ccp(G_SF(320), G_SF(1000)));
    spriteBg->addChild(winSprite);
    
    winSprite->setAnchorPoint(ccp(0.5, 0));
    //胜利文字动画
    CCActionInterval* m_pAction = (CCActionInterval*)CCSequence::create(
                                                                        CCDelayTime::create(0.5),
                                                                        CCMoveTo::create(0.2, ccp(G_SF(320), G_SF(590))),
                                                                        CCScaleBy::create(0.1, 1.1,0.9),
                                                                        CCMoveTo::create(0.1, ccp(G_SF(320), G_SF(594))),
                                                                        CCScaleTo::create(0.1, 1.0,1.0),
                                                                        CCMoveTo::create(0.1, ccp(G_SF(320), G_SF(590))),CCCallFunc::create(this, callfunc_selector(CLPveResultLayer::actionPatic)),
                                                                        NULL
                                                                        );
    winSprite->runAction(m_pAction);
    
    
    //固定文字
    CCLabelTTF* Str1 = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("PveSuccessStr").c_str(), "Arial", G_SF(40), CCSizeMake(G_SF(400),G_SF(200)), kCCTextAlignmentLeft);
    //颜色设置
    //    Str1->setColor(ccc3(255, 0, 0));
    Str1->setPosition(ccp(G_SF(340), G_SF(440)));
    spriteBg->addChild(Str1);
    
    return true;
}


CLPveResultLayer* CLPveResultLayer::FailBoos(Json::Value& jv)
{
	CLPveResultLayer *btn = new CLPveResultLayer;
	if (btn && btn->initFail(jv))
	{
		btn->autorelease();
		return btn;
	}
	CC_SAFE_DELETE(btn);
	return NULL;
}

bool CLPveResultLayer::initFail(Json::Value& jv)
{
    
    //失败音乐
    CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_PvpFail.wav");
    
    intTag = 101;
    
    //C_PveFailBg
    CCSprite *spriteBg = CCSprite::create("Image/PveLayer/C_PveFailBg.png");
    spriteBg->setPosition(ccp(G_SF(320), G_SF(480)));
    
    
    CCMenu* pCloseMenu = CCMenu::create();
    pCloseMenu->setAnchorPoint(CCPointZero);
    pCloseMenu->setPosition(CCPointZero);
    pCloseMenu->setTouchPriority(-10000);
    spriteBg->addChild(pCloseMenu, 1);
    
    CCSprite* pSpr1 = CCSprite::create("Image/PublicUI/Close.png");
    CCSprite* pSpr2 = CCSprite::create("Image/PublicUI/Close.png");
    CCMenuItemSprite* pItem = CCMenuItemSprite::create(pSpr1, pSpr2, NULL, this, menu_selector(CLPveResultLayer::back));
    pItem->setPosition(ccp(G_SF(574),G_SF(920)));
    pCloseMenu->addChild(pItem, 1);
    
    //失败图片
    CCSprite *sSpr_Star = CCSprite::create("Image/PveLayer/C_FailedBg.png");  //背景星
    sSpr_Star->setPosition(ccp(G_SF(320), G_SF(1000)));
    spriteBg->addChild(sSpr_Star);
    
    //叶子
    CCSprite *winSpr = CCSprite::create("Image/PveLayer/C_FailedLab.png");
    winSpr->setPosition( ccp(sSpr_Star->getContentSize().width * 0.5, sSpr_Star->getContentSize().height * 0.6));
    sSpr_Star->addChild(winSpr);
    
    sSpr_Star->setAnchorPoint(ccp(0.5, 0));
    
    CCActionInterval* m_pAction = (CCActionInterval*)CCSequence::create(
                                                                        CCDelayTime::create(0.5),
                                                                        CCMoveTo::create(0.2, ccp(G_SF(320), G_SF(540))),
                                                                        CCScaleBy::create(0.1, 1.1,0.9),
                                                                        CCMoveTo::create(0.1, ccp(G_SF(320), G_SF(544))),
                                                                        CCScaleTo::create(0.1, 1.0,1.0),
                                                                        CCMoveTo::create(0.1, ccp(G_SF(320), G_SF(540))),
                                                                        NULL
                                                                        );
    sSpr_Star->runAction(m_pAction);
    
    //固定文字
    CCLabelTTF* Str1 = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("PveFailStr").c_str(), "Arial", G_SF(40), CCSizeMake(G_SF(400),G_SF(200)), kCCTextAlignmentLeft);
    //颜色设置
    //    Str1->setColor(ccc3(255, 0, 0));
    Str1->setPosition(ccp(G_SF(330), G_SF(340)));
    spriteBg->addChild(Str1);
    
    Button *button = Button::createBtn("Image/BackPack/C_composite.png");
    button->setPosition(ccp(G_SF(320), G_SF(210)));
    button->setOnClickCallback(callfuncO_selector(CLPveResultLayer::continueCallBack),this);
    spriteBg->addChild(button);
    
    CCLabelTTF *ButtonStr = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("ContinuePk").c_str(), "Arial", G_SF(36), CCSizeMake(G_SF(150),G_SF(45)), kCCTextAlignmentCenter);
    ButtonStr->setColor(ccc3(0, 0, 0));
    ButtonStr->setPosition(ccp(G_SF(0), G_SF(0)));
    button->addChild(ButtonStr,2);
    
    this->addChild(spriteBg,1,intTag);
    
    return true;
}

void CLPveResultLayer::actionPatic()
{
    //粒子效果
    CCParticleSystem*  m_emitter = CCParticleExplosion::create();
    m_emitter->setPosition(ccp(G_SF(400), G_SF(600)));
    this->getChildByTag(10086)->addChild(m_emitter);
    m_emitter->setGravity(ccp(G_SF(-50), G_SF(-100)));
    m_emitter->setRotation(10);
    m_emitter->setRadialAccel(50);
    m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("Image/PveLayer/PT_stars.png") );
    m_emitter->setAutoRemoveOnFinish(true); //自动删除
}

void CLPveResultLayer::continueCallBack()
{
    
    CLPveSceneLayer* pParNode = (CLPveSceneLayer*)this->getParent();
    
    //挑战失败后，询问是否消除冷却时间
    CLPveSMangerLayer *sManager = CLPveSMangerLayer::create(pParNode->getiBossShelfNo(), 3);
    sManager->setAnchorPoint(CCPointZero);
    sManager->setPosition(CCPointZero);
    this->getParent()->addChild(sManager, 100, 100);
}

void CLPveResultLayer::removeCallback()
{
    CLPveSceneLayer* pParNode = (CLPveSceneLayer*)this->getParent();
    pParNode->ReturnToShelfsLayer();
    
}

bool CLPveResultLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void CLPveResultLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	
}

void CLPveResultLayer::onEnter()
{
	CCNode::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-9999,true);
}
void CLPveResultLayer::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCNode::onExit();
}

//跳转到主场景
void CLPveResultLayer::onCloseMenuPress(CCObject* pSender)
{
    CLSceneManager::sharedSceneManager()->setpCallTarget(this);
    
    CLSceneManager::sharedSceneManager()->getpLoadingLayer()->setVisible(true);
    CLSceneManager::sharedSceneManager()->BeginLoadAdvImages(CMD_LOGIN_REPLY);

    CLMainSceneLayer* pLayer = CLMainSceneLayer::create();
    pLayer->setAnchorPoint(CCPointZero);
    pLayer->setPosition(CCPointZero);
    CLSceneManager::sharedSceneManager()->ChangeScene(pLayer);
}

//返回到闯关
void CLPveResultLayer::back()
{
    CLPveSceneLayer* pParNode = (CLPveSceneLayer*)this->getParent();
    pParNode->ReturnToShelfsLayer();
}

//返回主场景
void CLPveResultLayer::returnToMain()
{
    //
    //    CLPveSceneLayer* pParNode = (CLPveSceneLayer*)this->getParent();
    //    
    //    m_nGSceneIndex = CLScene_MainScene;
    CLSceneManager::sharedSceneManager()->setpCallTarget(this);
    CLSceneManager::sharedSceneManager()->setpCallFun(callfunc_selector(CLShelfsSceneLayer::AdvImageLoadEnd));
}


