//
//  CLShelfsLayer.cpp
//  BigWinner
//
//  Created by ZhangQiyun on 13-10-21.
//
//
// modify info
// 20131217 修改头像区域，使用封装的精灵
// 20131219 挑战店长条件修改，（10）改为服务器设置

#include "CLShelfsSceneLayer.h"
#include "CLPlayerManager.h"
#include "CLSceneManager.h"
#include "CLStrokeLabelTTF.h"
#include "CLNetWorkManager.h"
#include "CLPveSceneLayer.h"
#include "CLMainSceneLayer.h"
#include "JsonDataManager.h"
#include "UserInfoSprite.h"
#include "CLPveSMangerLayer.h"
#include "CLChangeString.h"
#include "CLSoundManager.h"

CLShelfPanel::CLShelfPanel()
{
    m_pShelfData = NULL;
    m_pItem = NULL;
}

CLShelfPanel::~CLShelfPanel()
{
    delete m_pShelfData;
}

CLShelfPanel* CLShelfPanel::create(Json::Value& jv, CCNode *normalSprite, CCNode *selectedSprite, CCNode *disabledSprite, CCObject *target, SEL_MenuHandler selector)
{
    CLShelfPanel* pLayer = new CLShelfPanel();
    if(pLayer && pLayer->init(jv, normalSprite, selectedSprite, disabledSprite, target, selector))
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


bool CLShelfPanel::init(Json::Value& jv, CCNode *normalSprite, CCNode *selectedSprite, CCNode *disabledSprite, CCObject *target, SEL_MenuHandler selector)
{
    if(!CCLayer::init())
        return false;
    
    CCMenu* pMenu = CCMenu::create();
    pMenu->setAnchorPoint(CCPointZero);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 0);
    
    m_pItem = CCMenuItemSprite::create(normalSprite, selectedSprite, disabledSprite, target, selector);
    if(!m_pItem)
        return false;
    m_pItem->setAnchorPoint(CCPointZero);
    m_pItem->setScale(0.5);
    
    pMenu->addChild(m_pItem, 0);
    
    m_pShelfData = new CLShelfData(jv);
    
    pMenu->setTouchPriority(m_pShelfData->getnNomber() - 10000); //货架的按钮优先级，按照货架ID计算（防止重叠）
    
    if(m_pShelfData->getnStyle() == 0) // 说明是 货架
    {
        //这里可以根据数据里面的信息做这种操作
        CCSprite* pLabelBG = CCSprite::create("Image/ShelfLayer/SL_StarsBGUnlock.png");
        pLabelBG->setPosition(ccp(m_pItem->getContentSize().width/4, m_pItem->getContentSize().height/2));
        this->addChild(pLabelBG, 1);

        
        for(int i=0; i<3; i++)
        {
            CCSprite* pStar = NULL;
            if(i < m_pShelfData->getnStars())
            {
                pStar = CCSprite::create("Image/ShelfLayer/SL_Stars_N.png");
                //闯关成功以后 nNomber
            if(m_pShelfData->getnNomber() ==CLSceneManager::sharedSceneManager()->getnShelfNoX() && CLSceneManager::sharedSceneManager()->getnStarNumX() == 1)
                {
                    CCActionInterval* m_pAction = (CCActionInterval*)CCSequence::create(
                                                            CCDelayTime::create(1.0),
                                                            CCScaleBy::create(0.4, 0.4,0.4),
                                                            CCScaleTo::create(0.4, 1.0,1.0),
                                                            CCScaleBy::create(0.4, 1.2,1.2),
                                                            CCScaleTo::create(0.4, 1.0,1.0),
                                                            NULL
                                                            );
                    pStar->runAction(m_pAction);
                }

            }
            else
                pStar = CCSprite::create("Image/ShelfLayer/SL_Stars_D.png");
            //答题星级换成皇冠 wangguolong 20131204
            pStar->setPosition(ccp(G_SF(40)+i*G_SF(55), pLabelBG->getContentSize().height/2));
            pLabelBG->addChild(pStar,1, 100+i);
        }
        
    }
    else if(m_pShelfData->getnStyle() == 1)   //说明是店长
    {
        
    }
    else     //未知
    {
        
    }
    
    return true;
    
}


CLShelfsSceneLayer::CLShelfsSceneLayer()
{
    m_pCardFont = NULL;
    m_pMoneyFont = NULL;
    m_pShelfsMenu = NULL;
    m_nAllStars = 0;
    m_pSMMenu = NULL;
    m_bJihuo = true;
    m_nStoreNo = 0;
    m_bClickSM = false;
    m_nGSceneIndex = CLScene_TypeNull;
    
    m_pShopManSchTTF = NULL;
    m_nCoolTime = 0;
    m_iSmNo = 0;
    
    m_pShopManStrTTF=NULL;//wangguolong
    m_Scale9Sprite = NULL;
}

CLShelfsSceneLayer::~CLShelfsSceneLayer()
{
    
}


bool CLShelfsSceneLayer::init()
{
    if(!CLSceneLayer::init())
        return false;
    
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    //窗体开闭音乐
    CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_ShelfShow.wav");
    
    //添加背景
    int StoreNo = 0;
    std::string StoreBG_path;
    Json::Value* jv = CLSceneManager::sharedSceneManager()->GetJsonDataFromMsgID(8);
    
    if(jv)
    {
        StoreNo = (*jv)["storeNo"].asIntModestyle();
    }
    
    //暂时只有两个货架背景图片
//    if(StoreNo % 2 == 0){
//        StoreNo = 2;
//    }else{
//        StoreNo = 1;
//    }
    StoreNo = 3;
    //货架背景图片名称路径
    StoreBG_path = CCString::createWithFormat("Image/LoginLayer/StoreBG%d.png", StoreNo)->getCString();
    
    CCSprite* pBGSpr = CCSprite::create(StoreBG_path.c_str());
    pBGSpr->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(pBGSpr, 0, 10086);
    
    UserInfoSprite *userInfo = UserInfoSprite::create();
    userInfo->setPosition(ccp(G_SF(170), G_SF(889)));
    pBGSpr->addChild(userInfo);
    
    //添加监听事件
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CLShelfsSceneLayer::UpdateMoney), NOTICE_UPDATEMONEY1, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CLShelfsSceneLayer::UpdateCard), NOTICE_UPDATECARD1, NULL);
    
    //添加钞票条
    CCSprite* pMoneyBG = CCSprite::create("Image/LoginLayer/ML_MoneyBG.png");
    pMoneyBG->setPosition(ccp(G_SF(520), G_SF(916)));
    pBGSpr->addChild(pMoneyBG, 1);
    
    CCSprite* pMoneyIcon = CCSprite::create("Image/LoginLayer/ML_MoneyIcon.png");
    pMoneyIcon->setPosition(ccp(G_SF(-25), G_SF(34)));
    pMoneyBG->addChild(pMoneyIcon, 1);
    
    m_pMoneyFont = CCLabelBMFont::create(CCString::createWithFormat("%d", CLPlayerManager::shardPlayerManager()->getpUserInfo()->getMoneys())->getCString(), "Data/strings/shuzu3.fnt", G_SF(20));
    m_pMoneyFont->setPosition(ccp(pMoneyBG->getContentSize().width/2, pMoneyBG->getContentSize().height/2));
    pMoneyBG->addChild(m_pMoneyFont, 1);
    
    //添加购物卡条
    CCSprite* pCardBG = CCSprite::create("Image/LoginLayer/ML_MoneyBG.png");
    pCardBG->setPosition(ccp(G_SF(520), G_SF(854)));
    pBGSpr->addChild(pCardBG, 1);
    
    CCSprite* pCardIcon = CCSprite::create("Image/LoginLayer/ML_CardIcon.png");
    pCardIcon->setPosition(ccp(G_SF(-25), G_SF(32)));
    pCardBG->addChild(pCardIcon, 1);
    
    m_pCardFont = CCLabelBMFont::create(CCString::createWithFormat("%d", CLPlayerManager::shardPlayerManager()->getpUserInfo()->getJetton())->getCString(), "Data/strings/shuzu3.fnt");
    m_pCardFont->setPosition(ccp(pCardBG->getContentSize().width/2, pCardBG->getContentSize().height/2));
    pCardBG->addChild(m_pCardFont, 1);
    
    
    //增加二个添加按钮
    CCMenu* pAddMenu = CCMenu::create();
    pAddMenu->setAnchorPoint(CCPointZero);
    pAddMenu->setPosition(CCPointZero);
    pBGSpr->addChild(pAddMenu, 3);
    
    for(int i=0; i<2; i++)
    {
        CCSprite* pSprNor = CCSprite::create("Image/LoginLayer/ML_AddIcon_N.png");
        CCSprite* pSprSel = CCSprite::create("Image/LoginLayer/ML_AddIcon_S.png");
        CCMenuItemSprite* pItem = CCMenuItemSprite::create(pSprNor, pSprSel, NULL, this, callfuncO_selector(CLShelfsSceneLayer::onAddMenuPress));
        pItem->setPosition(ccp(G_SF(604), G_SF(917)-i*G_SF(64)));
        pAddMenu->addChild(pItem, 0, i);
    }
    
    //添加退出按钮
    CCMenu* pExitMenu = CCMenu::create();
    pExitMenu->setAnchorPoint(CCPointZero);
    pExitMenu->setPosition(CCPointZero);
    pBGSpr->addChild(pExitMenu, 0);
    
    CCSprite* pENor = CCSprite::create("Image/ShelfLayer/SL_ExitIcon_N.png");
    CCSprite* pESel = CCSprite::create("Image/ShelfLayer/SL_ExitIcon_S.png");
    CCMenuItemSprite* pItem = CCMenuItemSprite::create(pENor, pESel, NULL, this, menu_selector(CLShelfsSceneLayer::onExitMenuPress));
    pItem->setPosition(ccp(G_SF(320), G_SF(62)));
    pExitMenu->addChild(pItem, 0, 0);
    
    m_nCTIndex = CLScene_ShelfLayer;
    
    //根据单例网络json列表库有无添加商铺按钮
    Json::Value* pJv = CLSceneManager::sharedSceneManager()->GetJsonDataFromMsgID(8);
    if(pJv)
    {
        m_nStoreNo = (*pJv)["storeNo"].asIntModestyle();
        CLPlayerManager::shardPlayerManager()->getpUserInfo()->setnStoreNo(m_nStoreNo);
        
        for(int i=0; i < 6; i++)
        {
            CLShelfPanel* pItem = NULL;
            CCSprite* pNor = NULL;
            CCSprite* pSel = NULL;
            
            Json::Value jv = (*pJv)["shelfs"][i+1];  //数据1 是店长
            int No = 0; //货架序号 //wangguolong 20131130
            int ClassNo = 0; //货架类型序号
            std::string GoodsShelfName_path; //货架图片名称路径
            No = (*pJv)["shelfs"][i+1]["No"].asIntModestyle();
            ClassNo = JsonDataManager::shardJDManager()->m_Shelf[No]->getnClassNo(); //货架类型序号
            GoodsShelfName_path = CCString::createWithFormat("Image/ShelfLayer/GoodsShelf%d.png", ClassNo)->getCString(); //货架图片名称路径
            
            switch (i)
            {
                case 0:
                {
                    pNor = CCSprite::create(GoodsShelfName_path.c_str());
                    pItem = CLShelfPanel::create(jv, pNor, pSel, NULL, this, callfuncO_selector(CLShelfsSceneLayer::onShelfMenuPress));
                    pItem->setAnchorPoint(CCPointZero);
                    pItem->setPosition(ccp(G_SF(42),G_SF(112)));
                    pBGSpr->addChild(pItem, 3, i);
                }
                break;
                case 1:
                {
                    pNor = CCSprite::create(GoodsShelfName_path.c_str());
                    pItem = CLShelfPanel::create(jv, pNor, pSel, NULL, this, callfuncO_selector(CLShelfsSceneLayer::onShelfMenuPress));
                    pItem->setAnchorPoint(CCPointZero);
                    pItem->setPosition(ccp(G_SF(424),G_SF(112)));
                    pBGSpr->addChild(pItem, 3, i);
                }
                break;
                case 2:
                {
                    pNor = CCSprite::create(GoodsShelfName_path.c_str());
                    pItem = CLShelfPanel::create(jv, pNor, pSel, NULL, this, callfuncO_selector(CLShelfsSceneLayer::onShelfMenuPress));
                    pItem->setAnchorPoint(CCPointZero);
                    pItem->setPosition(ccp(G_SF(0),G_SF(258)));
                    pBGSpr->addChild(pItem, 2, i);
                }
                break;
                case 3:
                {
                    pNor = CCSprite::create(GoodsShelfName_path.c_str());
                    pItem = CLShelfPanel::create(jv, pNor, pSel, NULL, this, callfuncO_selector(CLShelfsSceneLayer::onShelfMenuPress));
                    pItem->setAnchorPoint(CCPointZero);
                    pItem->setPosition(ccp(G_SF(455),G_SF(258)));
                    pBGSpr->addChild(pItem, 2, i);
                }
                break;
                case 4:
                {
                    pNor = CCSprite::create(GoodsShelfName_path.c_str());
                    pItem = CLShelfPanel::create(jv, pNor, pSel, NULL, this, callfuncO_selector(CLShelfsSceneLayer::onShelfMenuPress));
                    pItem->setAnchorPoint(CCPointZero);
                    pItem->setPosition(ccp(G_SF(42),G_SF(419)));
                    pBGSpr->addChild(pItem, 1, i);
                }
                break;
                case 5:
                {
                    pNor = CCSprite::create(GoodsShelfName_path.c_str());
                    pItem = CLShelfPanel::create(jv, pNor, pSel, NULL, this, callfuncO_selector(CLShelfsSceneLayer::onShelfMenuPress));
                    pItem->setAnchorPoint(CCPointZero);
                    pItem->setPosition(ccp(G_SF(424),G_SF(419)));
                    pBGSpr->addChild(pItem, 1, i);
                }
                break;
                
                default:
                break;
            }
            m_nAllStars += pItem->getpShelfData()->getnStars();
            
            if (i > 0)
            {
                CLSceneManager::sharedSceneManager()->recordShelfStar(No, pItem->getpShelfData()->getnStars()); //存储普通货架星数
            }
        }
        
        int Zero = 0;
        Json::Value jv = (*pJv)["shelfs"][Zero];
        CCSprite* pSMNor = CCSprite::create("Image/ShelfLayer/SL_SMIcon_N.png");
        CLShelfPanel* pSMItem = CLShelfPanel::create(jv, pSMNor, NULL, NULL, this, menu_selector(CLShelfsSceneLayer::onShopManagerPress));
        pSMItem->setPosition(ccp(G_SF(234),G_SF(480)));
        pBGSpr->addChild(pSMItem, 0);
        
        pSMItem->setAnchorPoint(ccp(0.2,0));
        CCActionInterval* m_pAction = (CCActionInterval*)CCSequence::create(
                                                          CCDelayTime::create(5.0),
                                                          CCScaleBy::create(0.2, 0.95,1.05),
                                                          CCScaleTo::create(0.2, 1.0,1.0),
                                                          CCCallFunc::create(this, callfunc_selector(CLShelfsSceneLayer::AddShopManStr)),
                                                          NULL
                                                          );
        
        pSMItem->runAction(CCRepeatForever::create(m_pAction));
        
        CCSprite* pSMIcon = NULL;
        
        m_iSmNo = jv["No"].asIntModestyle(); //获取店长No
        CLSceneManager::sharedSceneManager()->setnCheckShelf(m_iSmNo); //保存到缓存
        CLSceneManager::sharedSceneManager()->setnCheckShopNo(m_nStoreNo); //保存到缓存
        
        //店长倒计时
        m_pShopManSchTTF = CCLabelTTF::create("", "Arial", G_SF(22));
        m_pShopManSchTTF->setPosition(ccp(G_SF(330), G_SF(780)));
        m_pShopManSchTTF->setColor(ccc3(102, 102, 102));
        pBGSpr->addChild(m_pShopManSchTTF, 3);
        int nTime = jv["bufftime"].asIntModestyle();
        
        StartSchTime(nTime); //开始店长倒计时
        
        int unLockCondition = (*pJv)["startnum"].asIntModestyle(); //店长挑战条件（服务器返回）
        if(m_nAllStars >= unLockCondition && m_bJihuo)    //超过10个星星 并且是 激活状态
        {
            //判断是否新解锁（当次游戏线程中）
            int oldStarNum = CLSceneManager::sharedSceneManager()->getShelfStar(m_iSmNo); //判断是否有解锁动画
            if ((oldStarNum < unLockCondition) && (oldStarNum != 0)) //满足解锁动画
            {
                CCSprite* pSMIconLock = CCSprite::create("Image/ShelfLayer/SL_SmLockIcon.png");
                CCSpawn *spaw1 = CCSpawn::create(CCScaleBy::create(0.5f, 1.5f), CCFadeOut::create(0.5f), NULL);

                pSMIconLock->setPosition(ccp(G_SF(425), G_SF(712)));
                pBGSpr->addChild(pSMIconLock, 2);
                pSMIconLock->runAction(spaw1);

                CCSpawn *spaw2 = CCSpawn::create(CCScaleTo::create(0.5f, 1.0f), CCFadeIn::create(0.5f), NULL);
                pSMIcon = CCSprite::create("Image/ShelfLayer/SL_SmYihuoIcon.png");
                pSMIcon->setScale(0.1);
                pSMIcon->runAction(spaw2);

            }
            else
            {
                pSMIcon = CCSprite::create("Image/ShelfLayer/SL_SmYihuoIcon.png");
            }
        }
        else if(m_nAllStars >= unLockCondition && !m_bJihuo) //非激活状态(挑战失败一次 并且还再冷却时间)
        {
            pSMIcon = CCSprite::create("Image/ShelfLayer/SL_SmUnlockIcon.png");
//            pSMItem->getpItem()->setEnabled(false);
        }
        else
        {
            pSMIcon = CCSprite::create("Image/ShelfLayer/SL_SmLockIcon.png");
            pSMItem->getpItem()->setEnabled(false);
        }
        pSMIcon->setPosition(ccp(G_SF(425), G_SF(712)));
        pBGSpr->addChild(pSMIcon, 1, 10010);
        
        
        //将本次货架数据存储，供下次再进来判断用
        CLSceneManager::sharedSceneManager()->recordShelfStar(m_iSmNo, m_nAllStars); //动画判断完后，存储本次店长货架星数

        
        CLSceneManager::sharedSceneManager()->RemoveJsonDataFromMsgID(8);
    }
    
    return true;
}

//添加店长话语
void CLShelfsSceneLayer::AddShopManStr()
{
    m_pShopManStrTTF = CCLabelTTF::create("", "Arial", G_SF(22));
    m_pShopManStrTTF->setColor(ccRED);

    m_Scale9Sprite = ::CCScale9Sprite::create("Image/ShelfLayer/ShopManDialogBox_BG.png");
    m_Scale9Sprite->setAnchorPoint(ccp(0.5,0.5));
    m_Scale9Sprite->setPreferredSize(CCSizeMake(G_SF(200), G_SF(70)));
    m_Scale9Sprite->setPosition(ccp(G_SF(200), G_SF(770)));
    this->addChild(m_Scale9Sprite);
    m_Scale9Sprite->addChild(m_pShopManStrTTF);
    m_pShopManStrTTF->setString(CLChangeString::sharedChangeString()->GetChangeString("ShopManTalk").c_str());
    m_pShopManStrTTF->setPosition(ccp(m_Scale9Sprite->getContentSize().width*0.5,m_Scale9Sprite->getContentSize().height*0.5));
    
    this->schedule(schedule_selector(CLShelfsSceneLayer::removeShopManStr), 2.0);
}
//删除店长话语
void CLShelfsSceneLayer::removeShopManStr()
{
    if(m_pShopManStrTTF){
        m_pShopManStrTTF->removeFromParentAndCleanup(true);
        m_pShopManStrTTF = NULL;
    }
    
    if(m_Scale9Sprite)
    {
        m_Scale9Sprite->removeFromParentAndCleanup(true);
        
        m_Scale9Sprite = NULL;
    }
    
}

void CLShelfsSceneLayer::UpdateMoney(CCObject* pSender)
{
    m_pMoneyFont->setString(CCString::createWithFormat("%d", CLPlayerManager::shardPlayerManager()->getpUserInfo()->getMoneys())->getCString());
}

void CLShelfsSceneLayer::UpdateCard(CCObject* pSender)
{
    m_pCardFont->setString(CCString::createWithFormat("%d", CLPlayerManager::shardPlayerManager()->getpUserInfo()->getJetton())->getCString());
}

void CLShelfsSceneLayer::onAddMenuPress(CCObject* pSender)       //2个增加按钮的对接事件
{
    CCMenuItemSprite* pItem = (CCMenuItemSprite*)pSender;
    switch (pItem->getTag())
    {
        case 0:
        {
            Json::Value value;
            CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_SHOPSHOW, value);
            CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
        }
            break;
        case 1:
        {
            
            //CMD_RMBBUYCARD 109 请求RMB购买购物卡信息
            Json::Value value;
            CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_RMBBUYCARD, value);
            CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
            
        }
            break;
            
        default:
            break;
    }
}

void CLShelfsSceneLayer::onShopManagerPress(CCObject* pSender)   //店长按钮点击事件
{
    CCMenuItemSprite* pItem = (CCMenuItemSprite*)pSender;
    CLShelfPanel* pPanel = (CLShelfPanel*)pItem->getParent()->getParent();
    
    //判断店长是否激活
    if(!m_bJihuo)
    {
        //花钱取消冷却
        CLPveSMangerLayer *sManager = CLPveSMangerLayer::create(pPanel->getpShelfData()->getnNomber(), 1);
        sManager->setAnchorPoint(CCPointZero);
        sManager->setPosition(CCPointZero);
        this->addChild(sManager, 100, 100);
        
        return;
    }
    else
    {
        Json::Value jv(Json::objectValue);
        jv["storeNo"] = m_nStoreNo;
        jv["shelfNo"] = pPanel->getpShelfData()->getnNomber();
        jv["mode"]    = 1;
        CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_JOIN, jv);
        
        CLSceneManager::sharedSceneManager()->setpCallTarget(this);
        CLSceneManager::sharedSceneManager()->setpCallFun(callfunc_selector(CLShelfsSceneLayer::AdvImageLoadEnd));
        
        CLSceneManager::sharedSceneManager()->getpLoadingLayer()->setVisible(true);
        m_bClickSM = true;
        m_nGSceneIndex = CLScene_PveLayer;
    }
    
}

void CLShelfsSceneLayer::onShelfMenuPress(CCObject* pSender)     //6个货架按钮的接口事件
{
    CCMenuItemSprite* pItem = (CCMenuItemSprite*)pSender;
    switch (pItem->getTag())
    {
        case 0:
        {
            
        }
            break;
        case 1:
        {
            
        }
            break;
        case 2:
        {
            
        }
            break;
        case 3:
        {
            
        }
            break;
        case 4:
        {
            
        }
            break;
        case 5:
        {
            
        }
            break;
        
        default:
            break;
    }
    
    CLShelfPanel* pPanel = (CLShelfPanel*)pItem->getParent()->getParent();
    
    Json::Value jv(Json::objectValue);
    jv["storeNo"] = m_nStoreNo;
    jv["shelfNo"] = pPanel->getpShelfData()->getnNomber();
    

//    char t[256];
//    std::string s;
//    
//    sprintf(t, "%d", pPanel->getpShelfData()->getnNomber());
//    s = t;
//    
//    //wangguolong 记录选择的货架
//    CLSceneManager::sharedSceneManager()->setnShelfNoX(s);
    CLSceneManager::sharedSceneManager()->setnShelfNoX(pPanel->getpShelfData()->getnNomber());
    
    jv["mode"]    = 1;
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_JOIN, jv);
    
    CLSceneManager::sharedSceneManager()->setpCallTarget(this);
    CLSceneManager::sharedSceneManager()->setpCallFun(callfunc_selector(CLShelfsSceneLayer::AdvImageLoadEnd));
    
    CLSceneManager::sharedSceneManager()->getpLoadingLayer()->setVisible(true);
    m_bClickSM = false;
    m_nGSceneIndex = CLScene_PveLayer;
}

void CLShelfsSceneLayer::onExit()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTICE_UPDATEMONEY1);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTICE_UPDATECARD1);
    
    CCLayer::onExit();
}


void CLShelfsSceneLayer::onExitMenuPress(CCObject* pSender)
{
    m_nGSceneIndex = CLScene_MainScene;
    CLSceneManager::sharedSceneManager()->setpCallTarget(this);
    CLSceneManager::sharedSceneManager()->setpCallFun(callfunc_selector(CLShelfsSceneLayer::AdvImageLoadEnd));
    
    CLSceneManager::sharedSceneManager()->getpLoadingLayer()->setVisible(true);
    CLSceneManager::sharedSceneManager()->BeginLoadAdvImages(CMD_LOGIN_REPLY);
}


void CLShelfsSceneLayer::AdvImageLoadEnd()
{
    switch (m_nGSceneIndex)
    {
        case CLScene_MainScene:
        {
            CLMainSceneLayer* pLayer = CLMainSceneLayer::create();
            pLayer->setAnchorPoint(CCPointZero);
            pLayer->setPosition(CCPointZero);
            CLSceneManager::sharedSceneManager()->ChangeScene(pLayer);
        }
            break;
        case CLScene_PveLayer:
        {
            //店长答题
            CLPveSceneLayer* pLayer = CLPveSceneLayer::create(m_bClickSM);
            pLayer->setAnchorPoint(CCPointZero);
            pLayer->setPosition(CCPointZero);

            CLSceneManager::sharedSceneManager()->ChangeScene(pLayer);
        }
            break;
            
        default:
            break;
    }

}

void CLShelfsSceneLayer::StartSchTime(int nTime)
{
    if (nTime <= 0) {
       m_pShopManSchTTF->setVisible(false);
        return;
    }
    else
    {
       m_pShopManSchTTF->setVisible(true);
       m_bJihuo = false;  //nTime大于0则还处于锁定状态
    }
    
    char sTime[20] = {0};
    sprintf(sTime, "%02d:%02d:%02d", nTime/3600, nTime/60%60, nTime%60);
    m_pShopManSchTTF->setString(sTime);

    m_nCoolTime = nTime;
    this->schedule(schedule_selector(CLShelfsSceneLayer::CoolTimeSch), 1.0);
}

void CLShelfsSceneLayer::CoolTimeSch(float ft)
{
    m_nCoolTime--;
    
    char sTime[20] = {0};
    sprintf(sTime, "%02d:%02d:%02d", m_nCoolTime/3600, m_nCoolTime/60%60, m_nCoolTime%60);
    m_pShopManSchTTF->setString(sTime);
    
    if(m_nCoolTime <= 0)
    {
        this->unschedule(schedule_selector(CLShelfsSceneLayer::EndSchTime));
        m_pShopManSchTTF->setVisible(false);
        
        //倒计时结束，冷却消除
        m_bJihuo = true;
        
        if (m_nCoolTime == 0) {
            this->getChildByTag(10086)->getChildByTag(10010)->removeFromParentAndCleanup(true);
            CCSprite* pSMIcon = CCSprite::create("Image/ShelfLayer/SL_SmYihuoIcon.png");
            pSMIcon->setPosition(ccp(G_SF(425), G_SF(712)));
            this->getChildByTag(10086)->addChild(pSMIcon, 1);
        }
    }
}

void CLShelfsSceneLayer::EndSchTime()
{
    this->unschedule(schedule_selector(CLShelfsSceneLayer::CoolTimeSch));

}

void CLShelfsSceneLayer::_CanceSmCool()
{

    if (m_pShopManSchTTF) //如果有倒计时
    {
        m_pShopManSchTTF->setVisible(false);
    }
    
//    CLShelfPanel* pPanel = (CLShelfPanel*)this->getChildByTag(10086)->getChildByTag(0);
    Json::Value jv(Json::objectValue);
//    jv["storeNo"] = m_nStoreNo;
//    jv["shelfNo"] = m_iSmNo;   //
    CCLog("冷却后->,%d,%d",CLSceneManager::sharedSceneManager()->getnCheckShopNo(),CLSceneManager::sharedSceneManager()->getnCheckShelf());
    jv["storeNo"] = CLSceneManager::sharedSceneManager()->getnCheckShopNo();
    jv["shelfNo"] = CLSceneManager::sharedSceneManager()->getnCheckShelf();
    jv["mode"]    = 1;
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_JOIN, jv);
    
    CLSceneManager::sharedSceneManager()->setpCallTarget(this);
    CLSceneManager::sharedSceneManager()->setpCallFun(callfunc_selector(CLShelfsSceneLayer::AdvImageLoadEnd));
    
    CLSceneManager::sharedSceneManager()->getpLoadingLayer()->setVisible(true);
    m_bClickSM = true;
    m_nGSceneIndex = CLScene_PveLayer;
}







