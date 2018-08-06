//
//  UserInfoSprite.cpp
//  BigWinner
//
//  Created by YinYanlong on 13-12-17.
//
//

#include "UserInfoSprite.h"
#include "LGameCommonDefine.h"
#include "CLPlayerManager.h"
#include "JsonDataManager.h"
#include "CLSceneManager.h"
#include "Button.h"

//用户头像信息精灵
UserInfoSprite::UserInfoSprite()
{
    
}

UserInfoSprite::~UserInfoSprite()
{
    
}

UserInfoSprite* UserInfoSprite::create()
{
    UserInfoSprite* pItem = new UserInfoSprite();
    if(pItem && pItem->init())
    {
        pItem->autorelease();
        return pItem;
    }
    else
    {
        CC_SAFE_DELETE(pItem);
        return NULL;
    }
}

bool UserInfoSprite::init()
{
    if(!initWithFile("Image/LoginLayer/ML_Head_BG.png")) //头像背景
        return false;
    
    //添加头像
    CCSprite* pPlayerHeadSmallBG = CCSprite::create("Image/PublicUI/RoleHeadBG.png");
    pPlayerHeadSmallBG->setPosition(ccp(pPlayerHeadSmallBG->getContentSize().width * 0.5 + G_SF(10),this->getContentSize().height * 0.5));
    this->addChild(pPlayerHeadSmallBG, 1);
    
    CCSprite* pPlayerHead = CCSprite::create("Image/LoginLayer/ML_RoleHead.png");
    pPlayerHead->setPosition(ccp(pPlayerHeadSmallBG->getContentSize().width * 0.5, pPlayerHeadSmallBG->getContentSize().height * 0.5));
    pPlayerHeadSmallBG->addChild(pPlayerHead, 2);
    
//    Button *headBtn = Button::createBtn("Image/LoginLayer/ML_RoleHead.png");
//    headBtn->setPosition(ccp(pPlayerHeadSmallBG->getContentSize().width * 0.5, pPlayerHeadSmallBG->getContentSize().height * 0.5));
//    headBtn->setOnClickCallbackNode(callfuncN_selector(UserInfoSprite::onHeadBtnPress), this);
//    headBtn->setHandlerPriority(-10002);
//    pPlayerHeadSmallBG->addChild(headBtn, 2);
    
    //添加昵称背景区
    CCSprite* pNamePanel = CCSprite::create("Image/LoginLayer/ML_NameBG.png");
    pNamePanel->setPosition(ccp(G_SF(210), G_SF(90)));
    this->addChild(pNamePanel, 1);
    
    //添加昵称信息
    CCLabelTTF* pNameTTF = CCLabelTTF::create(CLPlayerManager::shardPlayerManager()->getpUserInfo()->getsUsername().c_str(), "Arial", G_SF(24));
    pNameTTF->setPosition(ccp(pNamePanel->getContentSize().width/2, pNamePanel->getContentSize().height * 0.5));
    pNameTTF->setColor(ccc3(200, 100, 50));
    pNamePanel->addChild(pNameTTF, 1);
    
    //成就（最近3个成就）
    for (int i = 0; i < CLSceneManager::sharedSceneManager()->m_vUserInfoAchi.size(); i++) {
        int achiNo = CLSceneManager::sharedSceneManager()->m_vUserInfoAchi[i];
        std::string achiClassType = JsonDataManager::shardJDManager()->m_vAchievement[achiNo]->m_pAchiClass; //成就类型
        std::string sPicFile = "Image/BackPack/AchiIcon_" + achiClassType + ".png"; //图片路径
        CCSprite* pSmedalSpr = CCSprite::create(sPicFile.c_str());
        pSmedalSpr->setPosition(ccp(G_SF(150)+ i * G_SF(53), G_SF(38)));
        pSmedalSpr->setScale(0.5f);
        this->addChild(pSmedalSpr, 1);
        
        //级别
        int achiLevel = JsonDataManager::shardJDManager()->m_vAchievement[achiNo]->m_pAchiLevel; //成就类型
        
        std::string sStarPicFile = CCString::createWithFormat("Image/BackPack/AchStar_%d.png",achiLevel)->getCString();
        CCSprite *achiStar = CCSprite::create(sStarPicFile.c_str());
        pSmedalSpr->addChild(achiStar);
        achiStar->setPosition(ccp(pSmedalSpr->getContentSize().width * 0.5, G_SF(17)));
    }
    
    return true;
}


void UserInfoSprite::onHeadBtnPress()
{
    
}



RivalInfoSprite::RivalInfoSprite()
{
    m_pNameTTF = NULL;
}

RivalInfoSprite::~RivalInfoSprite()
{
    
}

RivalInfoSprite* RivalInfoSprite::create(Json::Value& jv)
{
    RivalInfoSprite* pLayer = new RivalInfoSprite();
    if(pLayer&&pLayer->init(jv))
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

bool RivalInfoSprite::init(Json::Value& jv)
{
    if(!initWithFile("Image/LoginLayer/ML_Head_BG.png")) //头像背景
        return false;
    
    //添加头像
    CCSprite* pPlayerHeadSmallBG = CCSprite::create("Image/PublicUI/RoleHeadBG.png");
    pPlayerHeadSmallBG->setPosition(ccp(pPlayerHeadSmallBG->getContentSize().width * 0.5 + G_SF(188),this->getContentSize().height * 0.5));
    this->addChild(pPlayerHeadSmallBG, 1);
    
    CCSprite* pPlayerHead = CCSprite::create("Image/LoginLayer/ML_RoleHead.png");
    pPlayerHead->setPosition(ccp(pPlayerHeadSmallBG->getContentSize().width * 0.5, pPlayerHeadSmallBG->getContentSize().height * 0.5));
    pPlayerHeadSmallBG->addChild(pPlayerHead, 2);
    
    
    //添加昵称背景区
    CCSprite* pNamePanel = CCSprite::create("Image/LoginLayer/ML_NameBG.png");
    pNamePanel->setPosition(ccp(G_SF(86), G_SF(90)));
    this->addChild(pNamePanel, 1);
    
    //添加昵称信息
    std::string sName = jv["opponent"]["name"].asStringModeStyle();
    m_pNameTTF = CCLabelTTF::create(sName.c_str(), "Arial", G_SF(24));
    m_pNameTTF->setPosition(ccp(pNamePanel->getContentSize().width/2, pNamePanel->getContentSize().height * 0.5));
    m_pNameTTF->setColor(ccc3(200, 100, 50));
    pNamePanel->addChild(m_pNameTTF, 1);
    
//    //成就（最近3个成就）临时 具体数据 json中取目前没有
//    for (int i = 0; i < CLSceneManager::sharedSceneManager()->m_vUserInfoAchi.size(); i++)
//    {
//        int achiNo = CLSceneManager::sharedSceneManager()->m_vUserInfoAchi[i];
//        std::string achiClassType = JsonDataManager::shardJDManager()->m_vAchievement[achiNo]->m_pAchiClass; //成就类型
//        std::string sPicFile = "Image/BackPack/AchiIcon_" + achiClassType + ".png"; //图片路径
//        CCSprite* pSmedalSpr = CCSprite::create(sPicFile.c_str());
//        pSmedalSpr->setPosition(ccp(G_SF(42 + i * 53), G_SF(38)));
//        pSmedalSpr->setScale(0.5f);
//        this->addChild(pSmedalSpr, 1);
//        
//        //级别
//        int achiLevel = JsonDataManager::shardJDManager()->m_vAchievement[achiNo]->m_pAchiLevel; //成就类型
//        std::string sStarPicFile = CCString::createWithFormat("Image/BackPack/AchStar_%d.png",achiLevel)->getCString();
//        CCSprite *achiStar = CCSprite::create(sStarPicFile.c_str());
//        pSmedalSpr->addChild(achiStar);
//        achiStar->setPosition(ccp(pSmedalSpr->getContentSize().width * 0.5, G_SF(17)));
//    }
    
    return true;
}






