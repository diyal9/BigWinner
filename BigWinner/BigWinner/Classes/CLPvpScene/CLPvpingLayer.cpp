//
//  CLPvpingLayer.cpp
//  BigWinner
//
//  Created by Zhang Qiyun on 13-11-10.
//
//

#include "CLPvpingLayer.h"
#include "LGameCommonDefine.h"
#include "CLPlayerManager.h"
#include "CLSceneManager.h"
#include "CLNetWorkManager.h"
#include "CLFightItem.h"
#include "CLChangeString.h"
#include "JsonDataManager.h"
#include "CLPvpResultLayer.h"
#include "UserInfoSprite.h"
#include "CLAnimationManager.h"
#include "CLSoundManager.h"
#include "CLSoundManager.h"

#define ANIMATION_PROS   1111

CLPvpingLayer::CLPvpingLayer()
{
    m_nSelfCurLife = 0;
    m_nRivalCurLife = 0;
    m_pGiveUpMenu = NULL;
    m_pPorpMenu = NULL;
    m_bRoundUseProp = NULL;
    m_pTopicLayer = NULL;
    m_nRoundIndex = 0;
    m_bCanUsePropCard = false;
    m_pUESelfTTF = NULL;
    m_pUERivalTTF = NULL;
    m_pSchUseProTTF = NULL;
    m_nSchUseCardTime = 0;
    m_pSelfScoreSpr = NULL;
    m_pRivalScoreSpr = NULL;
    m_pSelfScoreTTF = NULL;
    m_pRivalScoreTTF = NULL;
    m_nTurns = 0;
    m_bGiveTopic = false;
    m_pPTopicLayer = NULL;
}

CLPvpingLayer::~CLPvpingLayer()
{
    
}

CLPvpingLayer* CLPvpingLayer::create(Json::Value& jv)
{
    CLPvpingLayer* pLayer = new CLPvpingLayer();
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

bool CLPvpingLayer::init(Json::Value& jv)
{
    if(!CCLayer::init())
        return false;
    
    //添加自己的头像信息
    UserInfoSprite* pUserInfo = UserInfoSprite::create();
    pUserInfo->setAnchorPoint(CCPointZero);
    pUserInfo->setPosition(ccp(G_SF(17), G_SF(842)));
    this->addChild(pUserInfo, 1);
    
    RivalInfoSprite* pRivalInfo = RivalInfoSprite::create(jv);
    pRivalInfo->setAnchorPoint(ccp(1, 0));
    pRivalInfo->setPosition(ccp(G_SF(626), G_SF(842)));
    this->addChild(pRivalInfo, 1, 10000);
    
    
    //添加认输按钮
    m_pGiveUpMenu = CCMenu::create();
    m_pGiveUpMenu->setAnchorPoint(CCPointZero);
    m_pGiveUpMenu->setPosition(CCPointZero);
    this->addChild(m_pGiveUpMenu, 1);
    
    CCSprite* spr1 = CCSprite::create("Image/PveLayer/CP_GiveupIcon_N.png");
    CCSprite* spr2 = CCSprite::create("Image/PveLayer/CP_GiveupIcon_N.png");
    CCMenuItemSprite* pItem = CCMenuItemSprite::create(spr1, spr2, NULL, this, menu_selector(CLPvpingLayer::onGiveupMenuPress));
    pItem->setPosition(ccp(G_SF(591), G_SF(800)));
    m_pGiveUpMenu->addChild(pItem, 1);
    //内测 认输按钮屏蔽
    pItem->setVisible(false);
    
    m_pSchedueTTF = CCLabelBMFont::create("", "Data/strings/shuzu3.fnt", G_SF(22));
    m_pSchedueTTF->setPosition(ccp(G_SF(584), G_SF(728)));
    m_pSchedueTTF->setVisible(false);
    this->addChild(m_pSchedueTTF, 3);

    
    //vs字样
    CCSprite* pVsSpr = CCSprite::create("Image/PvpLayer/CP_VsIcon.png");
    pVsSpr->setPosition(ccp(G_SF(320), G_SF(842)));
    this->addChild(pVsSpr, 5);
    
    //双方的积分条
    CCSprite* pScoreBarBG1 = CCSprite::create("Image/PvpLayer/CP_ScoreBarBG.png");
    pScoreBarBG1->setAnchorPoint(CCPointZero);
    pScoreBarBG1->setPosition(ccp(G_SF(108), G_SF(824)));
    this->addChild(pScoreBarBG1, 4);
    
    m_pSelfScoreSpr = CCSprite::create("Image/PvpLayer/CP_ScoreBar.png", CCRectMake(0, 0, G_SF(190)*m_nSelfCurLife/100, G_SF(30)));
    m_pSelfScoreSpr->setAnchorPoint(CCPointZero);
    m_pSelfScoreSpr->setPosition(ccp(G_SF(1), 0));
    pScoreBarBG1->addChild(m_pSelfScoreSpr, 1);
    
    m_pSelfScoreTTF = CCLabelTTF::create(CCString::createWithFormat("%d", m_nSelfCurLife)->getCString(), "Arial", G_SF(22));
    m_pSelfScoreTTF->setPosition(ccp(G_SF(150), G_SF(15)));
    pScoreBarBG1->addChild(m_pSelfScoreTTF, 2);
    
    
    CCSprite* pScoreBarBG2 = CCSprite::create("Image/PvpLayer/CP_ScoreBarBG.png");
    pScoreBarBG2->setAnchorPoint(CCPointZero);
    pScoreBarBG2->setPosition(ccp(G_SF(350), G_SF(824)));
    this->addChild(pScoreBarBG2, 4);
    
    m_pRivalScoreSpr = CCSprite::create("Image/PvpLayer/CP_ScoreBar.png", CCRectMake(0, 0, G_SF(190)*m_nRivalCurLife/100, G_SF(30)));
    m_pRivalScoreSpr->setAnchorPoint(CCPointZero);
    m_pRivalScoreSpr->setPosition(ccp(G_SF(1), 0));
    pScoreBarBG2->addChild(m_pRivalScoreSpr, 1);
    
    m_pRivalScoreTTF = CCLabelTTF::create(CCString::createWithFormat("%d", m_nRivalCurLife)->getCString(), "Arial", G_SF(22));
    m_pRivalScoreTTF->setPosition(ccp(G_SF(40), G_SF(15)));
    pScoreBarBG2->addChild(m_pRivalScoreTTF, 2);
    
    m_pPorpMenu = CCMenu::create();
    m_pPorpMenu->setAnchorPoint(CCPointZero);
    m_pPorpMenu->setPosition(CCPointZero);
    this->addChild(m_pPorpMenu, 1);
    
    std::map<int, int> vCards = CLSceneManager::sharedSceneManager()->GetFightCards();
    std::map<int, int> vProps = CLSceneManager::sharedSceneManager()->GetFightProps();
    std::map<int, int>::iterator iter_card = vCards.begin();
    std::map<int, int>::iterator iter_prop = vProps.begin();

    //道具位置设置 wangguolong
    int nprop =0;
    for(int i=0; iter_prop != vProps.end(); iter_prop++, i++)
    {
        int nNo = iter_prop->second;
        CCLog("道具No：%i  循环的i值：%i",nNo,i);
        CLFightPropItem* pItem = CLFightPropItem::create(nNo, this, callfuncO_selector(CLPvpingLayer::onPropMenuPress));
        if(!pItem)
            continue;
        pItem->setAnchorPoint(CCPointZero);
        if(nNo == 0)
        {
            nprop = nprop;
        }else
        {
            nprop++;
        }
    
        //道具位置设置 wangguolong
//        pItem->setPosition(ccp(G_SF(12)+i*G_SF(117), G_SF(14)));
        pItem->setPosition(ccp(G_SF(-85)+nprop*G_SF(117), G_SF(14)));
        m_pPorpMenu->addChild(pItem, 0, i);
    }
    
    for(int i=vProps.size(); iter_card != vCards.end(); iter_card++, i++)
    {
        int nNo = iter_card->second;
        CCLog("题卡No：%i 循环的i值：%i",nNo,i);
        CLFightPropItem* pItem = CLFightPropItem::create(nNo, this, callfuncO_selector(CLPvpingLayer::onPropMenuPress), true);
        if(!pItem)
            continue;
        pItem->setAnchorPoint(CCPointZero);
        if(nNo == 0)
        {
            nprop = nprop;
        }else
        {
            nprop++;
        }
        //道具位置设置 wangguolong
//        pItem->setPosition(ccp(G_SF(12)+i*G_SF(117), G_SF(14)));
        pItem->setPosition(ccp(G_SF(-85)+nprop*G_SF(117), G_SF(14)));
        m_pPorpMenu->addChild(pItem, 0, i);
    }
    
    setCardMenuEnable(false);
    setPropMenuEnable(false);
    
    m_pUESelfTTF = CCLabelTTF::create("", "Verdana-Bold", G_SF(24));
    m_pUESelfTTF->setAnchorPoint(CCPointZero);
    //对方使用蓝色
    m_pUESelfTTF->setColor(ccc3(0, 150, 255));
    m_pUESelfTTF->setPosition(ccp(G_SF(16), G_SF(795)));
    this->addChild(m_pUESelfTTF, 2);
    
    m_pUERivalTTF = CCLabelTTF::create("", "Verdana-Bold", G_SF(24));
    m_pUERivalTTF->setAnchorPoint(CCPointZero);
    m_pUERivalTTF->setPosition(ccp(G_SF(16), G_SF(770)));
    //对方使用黄色
    m_pUERivalTTF->setColor(ccc3(255, 150, 0));
    this->addChild(m_pUERivalTTF, 2);
    
    m_pSchUseProTTF = CCLabelTTF::create("", "Verdana-Bold", G_SF(40));
    m_pSchUseProTTF->setPosition(ccp(G_SF(320), G_SF(400)));
    m_pSchUseProTTF->setColor(ccBLACK);
    this->addChild(m_pSchUseProTTF, 10);
    m_pSchUseProTTF->setVisible(false);
    
    Json::Value* pjv = CLSceneManager::sharedSceneManager()->GetJsonDataFromMsgID(CMD_PK1_QUESTION);
    if(pjv)
    {
        UpdateTopicLayer(*pjv);
        CLSceneManager::sharedSceneManager()->RemoveJsonDataFromMsgID(CMD_PK1_QUESTION);
    }
    
    return true;
}

void CLPvpingLayer::onPropMenuPress(CCObject* pSender)
{
    CLFightPropItem* pItem = (CLFightPropItem*)pSender;
    if(pItem->getpPropInfo()->number == 0)
        return;
    
    Json::Value jv(objectValue);
    if(pItem->getbTopicCard())
    {
        jv["card"] = pItem->getpPropInfo()->m_cardJueseInfo->m_artNo;
        CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_PK1_USECARD, jv);
        setCardMenuEnable(false);
        EndSchUseCard();
    }
    else
    {
        jv["prop"] = pItem->getpPropInfo()->m_cardJueseInfo->m_artNo;
        CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_PK1_USEPROP, jv);
        setPropMenuEnable(false);
    }
    
    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
    
}

void CLPvpingLayer::BeginToPvp()
{
    Json::Value jv(objectValue);
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_PK1_ANSWER, jv);
    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
}


void CLPvpingLayer::StartSchTime(int nTime)
{
    m_nSchTime = nTime;
    
    this->schedule(schedule_selector(CLPvpingLayer::onTimeFrame), 1.0);
}

void CLPvpingLayer::onTimeFrame(float ft)
{
    m_nSchTime--;
    char sTime[20] = {0};
    sprintf(sTime, "%d", m_nSchTime);
    m_pSchedueTTF->setString(sTime);
    
    if(m_nSchTime <= 0)
    {
        this->unschedule(schedule_selector(CLPvpingLayer::onTimeFrame));
//        EndSchTime();
    }
}

void CLPvpingLayer::EndSchTime(int nAnswer)
{
    this->unschedule(schedule_selector(CLPvpingLayer::onTimeFrame));
    Json::Value jv(objectValue);
    if(nAnswer != -1)
        jv["submit"] = nAnswer;
    
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_PK1_ANSWER, jv);
    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
    
}

void CLPvpingLayer::UpdateTopicLayer(Json::Value &jv)
{
    std::string sTurns = jv["type"].asStringModeStyle();
    if(sTurns.compare("must") == 0)
    {
        m_nTurns = 0;
    }
    else if (sTurns.compare("turns") == 0)
    {
        m_nTurns = 1;
    }
    else if(sTurns.compare("snatch") == 0)
    {
        m_nTurns = 2;
    }
    
    if(m_nTurns == 0)
    {
        if(!m_pTopicLayer)
        {
            m_pTopicLayer = CLPveTopicLayer::create(jv, false);
            m_pTopicLayer->setPosition(ccp(G_SF(320), G_SF(571)));
            this->addChild(m_pTopicLayer, 1);
            m_pSchedueTTF->setVisible(true);
            m_pGiveUpMenu->setVisible(true);
        }
        UpdateBlood(jv["goal"].asIntModestyle(), jv["opponent"]["goal"].asIntModestyle());
        m_pTopicLayer->UpdateTopicData(jv);
        m_pTopicLayer->ReSetAnswerState();
        int nTime = jv["times"].asIntModestyle();
        StartSchTime(nTime);
        m_nRoundIndex++;
    }
    else if(m_nTurns == 1)   //轮流出题阶段
    {
        if(jv.isMember("replier") )  //表示有我答题
        {
            if(m_pPTopicLayer)
            {
                m_pPTopicLayer->removeFromParent();
                m_pPTopicLayer = NULL;
                m_pTopicLayer->setVisible(true);
            }
            UpdateBlood(jv["goal"].asIntModestyle(), jv["opponent"]["goal"].asIntModestyle());
            m_pTopicLayer->UpdateTopicData(jv);
            m_pTopicLayer->ReSetAnswerState();
            int nTime = jv["times"].asIntModestyle();
            StartSchTime(nTime);
            m_nRoundIndex++;
            
            setPropMenuEnable(true);
            setCardMenuEnable(false);
            EndSchUseCard();
            
        }
        else
        {
            
            m_pTopicLayer->setVisible(false);
            
            m_pPTopicLayer = CLPvpTopicLayer::create(jv);
            m_pPTopicLayer->setPosition(ccp(G_SF(320), G_SF(571)));
            this->addChild(m_pPTopicLayer, 1);
            
            UpdateBlood(jv["goal"].asIntModestyle(), jv["opponent"]["goal"].asIntModestyle());
            int nTime = jv["times"].asIntModestyle();
            StartSchTime(nTime);
        }
    }
    else if(m_nTurns == 2) //抢答阶段
    {
        
        if(m_pPTopicLayer)
        {
            m_pPTopicLayer->removeFromParent();
            m_pPTopicLayer = NULL;
            m_pTopicLayer->setVisible(true);
        }
        
        UpdateBlood(jv["goal"].asIntModestyle(), jv["opponent"]["goal"].asIntModestyle());
        m_pTopicLayer->UpdateTopicData(jv);
        m_pTopicLayer->ReSetAnswerState();
        int nTime = jv["times"].asIntModestyle();
        StartSchTime(nTime);
        m_nRoundIndex++;
        
        if(m_bCanUsePropCard)
        {
            setPropMenuEnable(true);
            setCardMenuEnable(false);
            EndSchUseCard();
        }
        
        CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_Catchphrase.wav");
    }

       CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_QuestionChange.wav"); //题目切换音效
}

void CLPvpingLayer::UpdateBlood(int nSelfLife1, int nRivalLife1)
{
    m_nSelfCurLife = nSelfLife1*10;
    m_nRivalCurLife = nRivalLife1*10;
    
    m_pSelfScoreTTF->setString(CCString::createWithFormat("%d", m_nSelfCurLife)->getCString());
    m_pRivalScoreTTF->setString(CCString::createWithFormat("%d", m_nRivalCurLife)->getCString());
    
    float fSelfPre, fRivalPre;
    
    if(m_nSelfCurLife/100.0 <= 1.0)
        fSelfPre = m_nSelfCurLife/100.0;
    else
        fSelfPre = 1.0;
    
    if(m_nRivalCurLife/100.0 <= 1.0)
        fRivalPre = m_nRivalCurLife/100.0;
    else
        fRivalPre = 1.0;
    
    m_pSelfScoreSpr->setTextureRect(CCRectMake(0, 0, G_SF(190)*fSelfPre, G_SF(30)));
    m_pRivalScoreSpr->setTextureRect(CCRectMake(0, 0, G_SF(190)*fRivalPre, G_SF(30)));
    
}

void CLPvpingLayer::CanUsePropCard(Json::Value& jv)
{
    bool bReady = (bool)jv["ready"].asIntModestyle();
    UpdateBlood(jv["goal"].asIntModestyle(), jv["opponent"]["goal"].asIntModestyle());
    if(bReady)  //说明我答题 那么 我什么都不能做 等着
    {
        CCLOG("等待对方使用题卡。。。");
    }
    else
    {
        m_bCanUsePropCard = true;
        setPropMenuEnable(false);
        setCardMenuEnable(true);
        m_nSchUseCardTime = jv["times"].asIntModestyle();
        StartSchUseCard();
        
        CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_PleaseCheckCard.wav"); //答题完后，给对方出题音效
    }

}

void CLPvpingLayer::onGiveupMenuPress(CCObject* pSender)
{
    
}

void CLPvpingLayer::setCardMenuEnable(bool b)
{
    CCArray* pArray = m_pPorpMenu->getChildren();
    
    if (!pArray)
    { //add by yin
        return; //没有道具的时候，pArray ＝ null
    }
    
    if(b)   //开启
    {
        for(int i=0; i<pArray->count(); i++)
        {
            CLFightPropItem* pItem = (CLFightPropItem*)pArray->objectAtIndex(i);
            if(pItem->getbTopicCard())
                pItem->setEnabled(true);
        }
    }
    else    //关闭
    {
        for(int i=0; i<pArray->count(); i++)
        {
            CLFightPropItem* pItem = (CLFightPropItem*)pArray->objectAtIndex(i);
            if(pItem->getbTopicCard())
                pItem->setEnabled(false);
        }
    }
}

void CLPvpingLayer::setPropMenuEnable(bool b)
{
    CCArray* pArray = m_pPorpMenu->getChildren();
    
    if (!pArray)
    { //add by yin
        return; //没有道具的时候，pArray ＝ null
    }
    
    if(b)   //开启
    {
        for(int i=0; i<pArray->count(); i++)
        {
            CLFightPropItem* pItem = (CLFightPropItem*)pArray->objectAtIndex(i);
            if(!pItem->getbTopicCard())
                pItem->setEnabled(true);
        }
    }
    else    //关闭
    {
        for(int i=0; i<pArray->count(); i++)
        {
            CLFightPropItem* pItem = (CLFightPropItem*)pArray->objectAtIndex(i);
            if(!pItem->getbTopicCard())
                pItem->setEnabled(false);
        }
    }
}

void CLPvpingLayer::UsePropSucces(Json::Value& jv)    //使用道具成功
{
    bool bUseSelf = jv["useself"].asBool();
    std::string sData;
    
    int pPropID = jv["prop"].asIntModestyle();
    if(bUseSelf)
    {
        
        CCArray* pArray = m_pPorpMenu->getChildren();
        CLFightPropItem* pUseItem = NULL;
        
        for(int i=0; i<pArray->count(); i++)
        {
            CLFightPropItem* pItem = (CLFightPropItem*)pArray->objectAtIndex(i);
            if(pItem->getpPropInfo()->m_cardJueseInfo->m_artNo == pPropID)
            {
                pItem->getpPropInfo()->number--;
                if(pItem->getpPropInfo()->number <= 0)
                {
                    pItem->removeFromParent();
                    CLPlayerManager::shardPlayerManager()->getpUserInfo()->RemovePropFromID(pPropID);
                }
                else
                {
                    pUseItem = pItem;
                    pItem->getpNumLabel()->setString(CCString::createWithFormat("%d", pItem->getpPropInfo()->number)->getCString());
                }
                break;
            }
        }
        
        if(pUseItem)
            pUseItem->setnCoolRound(jv["cooling"].asIntModestyle());
        
        std::string sClass = jv["class"].asStringModeStyle();
        
        //使用道具动画播放
        animationWithPro(pPropID,true, 0.3, 1.1, 1);
        
        if(sClass.compare("D1") == 0)    //更新剩余时间  对战没有
        {
            int nAddTime = jv["arg2"].asIntModestyle();
            m_nSchTime += nAddTime;
            char sTime[20] = {0};
            sprintf(sTime, "%d", m_nSchTime);
            m_pSchedueTTF->setString(sTime);
            
        }
        else if(sClass.compare("D2") == 0 )   //一个错误答案的选项
        {
            int nAnswerIndex = jv["arg1"].asIntModestyle();
            if(m_pTopicLayer)
                m_pTopicLayer->GiveAnanswerIndex(false, nAnswerIndex);
            
        }
        else if(sClass.compare("D3") == 0)   //增加血量
        {
            UpdateBlood(m_nRivalCurLife + jv["arg1"].asIntModestyle(), m_nRivalCurLife);
        }
        else if(sClass.compare("D4") == 0)   //正确的答案 去错
        {
            //加上金手指指向正确答案...或者在正确答案上加上文字效果
            //.......
            
            int nAnswerIndex = jv["arg1"].asIntModestyle();
            if(m_pTopicLayer)
                m_pTopicLayer->GiveAnanswerIndex(true, nAnswerIndex);
        }
        else if(sClass.compare("D5") == 0)//拒答案卡
        {

        
        }
        else
        {
            
        }
        
        sData = CLChangeString::sharedChangeString()->GetChangeString("IUse");
        sData += CLChangeString::sharedChangeString()->GetChangeString("Prop");
        sData += JsonDataManager::shardJDManager()->m_vCardJueseDaoju[pPropID]->m_artClassName;
        m_pUESelfTTF->setString(sData.c_str());

    }
    else
    {
        sData = CLChangeString::sharedChangeString()->GetChangeString("RUse");
        sData += CLChangeString::sharedChangeString()->GetChangeString("Prop");
        sData += JsonDataManager::shardJDManager()->m_vCardJueseDaoju[pPropID]->m_artClassName;
        m_pUERivalTTF->setString(sData.c_str());
    }
    
}

//播放动画
//proID 道具ID isShutDown 是否关闭  delayTime每帧延迟时间 shutTime 总关闭时间 repeatTimes  重复次数 0代表永远执行
void CLPvpingLayer::animationWithPro(int proID, int isShutDown, float delayTime, float shutTime, int repeatTimes )
{
    //音乐效果以后加
    std::string musicStr;
    if(proID == 46)//买个表
    {
        CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_AddTimeCard.wav");
    }
    else if(proID == 47)//剪刀手
    {
        CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_Handssors.wav");
    }
    else if(proID == 48)//加血
    {
        CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_AddBloodCard.wav");
    }
    else if(proID == 49)//金手指
    {
        //暂时没有
    }
    else if(proID == 50)//拒答
    {
        CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_RefuseAnswerCard.wav");
    }
    else//其它
    {
        
    }
    
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    std::string sFile;
    sFile = CCString::createWithFormat("proAnimation%d", proID)->getCString();
    
    CCAnimation* animation = CLAnimationManager::sharedAnimationManager()->animationWithName(sFile.c_str(),delayTime);
    CCSprite* magic = CLAnimationManager::sharedAnimationManager()->spriteWithAnimationName(sFile.c_str());
    magic->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(magic, 10, ANIMATION_PROS);
    
    if (repeatTimes)
        magic->runAction(CCRepeat::create(CCAnimate::create(animation), repeatTimes));
    else
        magic->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
    
    
    //播放完成后关闭
    if (isShutDown)
    {
        this->scheduleOnce(schedule_selector(CLPvpingLayer::animationCancle), shutTime);
    }
    
    
}
//取消动画
void CLPvpingLayer::animationCancle(float t)
{
    this->removeChildByTag(ANIMATION_PROS, true);
}


void CLPvpingLayer::UseCardSucces(Json::Value& jv)    //使用卡牌成功
{
    bool bUseSelf = jv["useself"].asBool();
    std::string sData;
    int pCardID = jv["card"].asIntModestyle();
    
    if(bUseSelf)
    {
        CCArray* pArray = m_pPorpMenu->getChildren();
        CLFightPropItem* pUseItem = NULL;
        
        for(int i=0; i<pArray->count(); i++)
        {
            CLFightPropItem* pItem = (CLFightPropItem*)pArray->objectAtIndex(i);
            if(pItem->getpPropInfo()->m_cardJueseInfo->m_artNo == pCardID)
            {
                pItem->getpPropInfo()->number--;
                if(pItem->getpPropInfo()->number <= 0)
                {
                    pItem->removeFromParent();
                    CLPlayerManager::shardPlayerManager()->getpUserInfo()->RemovePropFromID(pCardID);
                }
                else
                {
                    pUseItem = pItem;
                    pItem->getpNumLabel()->setString(CCString::createWithFormat("%d", pItem->getpPropInfo()->number)->getCString());
                }
                break;
            }
        }
        
        if(pUseItem)
            pUseItem->setnCoolRound(jv["cooling"].asIntModestyle());
        
        sData = CLChangeString::sharedChangeString()->GetChangeString("IUse");
        sData += CLChangeString::sharedChangeString()->GetChangeString("Card");
        sData += JsonDataManager::shardJDManager()->m_vCardJueseDaoju[pCardID]->m_artClassName;
        m_pUESelfTTF->setString(sData.c_str());
    }
    else
    {
        sData = CLChangeString::sharedChangeString()->GetChangeString("RUse");
        sData += CLChangeString::sharedChangeString()->GetChangeString("Card");
        sData += JsonDataManager::shardJDManager()->m_vCardJueseDaoju[pCardID]->m_artClassName;
        m_pUERivalTTF->setString(sData.c_str());
    }

}

void CLPvpingLayer::EndPvping(Json::Value& jv)
{
    std::string rivalName;
    RivalInfoSprite* rivalnameSpr= (RivalInfoSprite*)this->getChildByTag(10000);
    if (rivalnameSpr) {
        rivalName = rivalnameSpr->getpNameTTF()->getString();
    }
    
    CLPvpSuccLayer *pvpResult = CLPvpSuccLayer::create(jv, rivalName);
    this->addChild(pvpResult, 50);
    pvpResult->setPosition(CCPointZero);
}

void CLPvpingLayer::StartSchUseCard()
{
    this->schedule(schedule_selector(CLPvpingLayer::SchUseCardFrame), 1);
    std::string s = CLChangeString::sharedChangeString()->GetChangeString("SchUseCard");
    std::string s1 = CCString::createWithFormat(s.c_str(), m_nSchUseCardTime)->getCString();
    m_pSchUseProTTF->setString(s1.c_str());
    m_pSchUseProTTF->setVisible(true);
}

void CLPvpingLayer::EndSchUseCard()
{
    setCardMenuEnable(false);
    m_nSchUseCardTime = 0;
    this->unschedule(schedule_selector(CLPvpingLayer::SchUseCardFrame));
    m_pSchUseProTTF->setVisible(false);
}

void CLPvpingLayer::SchUseCardFrame()
{
    m_nSchUseCardTime--;
    std::string s = CLChangeString::sharedChangeString()->GetChangeString("SchUseCard");
    std::string s1 = CCString::createWithFormat(s.c_str(), m_nSchUseCardTime)->getCString();
    m_pSchUseProTTF->setString(s1.c_str());
    
    if(m_nSchUseCardTime <= 0)
    {
        EndSchUseCard();
    }
}

void CLPvpingLayer::ShowRivalSelAnswer(Json::Value& jv)
{
    if(m_nTurns == 1 && m_pPTopicLayer)   //轮流出题阶段
    {
        m_pPTopicLayer->StartShowAnswer(jv["opponent"]["submited"].asIntModestyle());
    }
}



