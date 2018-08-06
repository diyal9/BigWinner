//
//  CLSceneManager.cpp
//  LolAllStars1_X
//
//  Created by Zhang Qiyun on 13-5-7.
//
//

#include "CLSceneManager.h"
#include "CLLoadImageManager.h"
#include "JsonDataManager.h"
#include "CLNetWorkManager.h"
#include "CRequestMsgConstDedine.h" 


static CLSceneManager* g_SceneManager = NULL;

CLSceneManager::CLSceneManager()
{
    m_pParentNode = NULL;
    m_pRunningScene = NULL;
    m_pReplaceNode = NULL;
    m_pFadeLayer = NULL;
    m_pReCoverNode = NULL;
    m_pTempCoverNode = NULL;
    m_pCallTarget = NULL;
    m_pCallFun = NULL;
    m_pLoadingLayer = NULL;
    m_vNetData = Json::objectValue;
    m_pNetErrorLayer = NULL;
    m_pWaitNetLayer = NULL;
    
    m_newlevel = -2;
    m_nShelfNoX = 0;
    m_nStarNumX = 0;
    m_nNum = 0;
    m_pDeviceBG = NULL;
    m_nPair = 0;
    m_nNolock = -1;
    
    m_nValuetime = 0;
    m_nOpenflg = 0;
    m_nGgaoFlg=0;
    
    m_nCheckShelf = NULL;
    m_nCheckShopNo = NULL;
    
}

CLSceneManager::~CLSceneManager()
{
    std::map<int, Json::Value*>::iterator iter_begin = m_vJsonData.begin();
    for(; iter_begin != m_vJsonData.end(); iter_begin++)
    {
        delete iter_begin->second;
    }
    
    for(int i=0; i<m_vStoreDataList.size(); i++)
    {
        delete m_vStoreDataList[i];
    }
    
    for(int i=0; i<m_vUpropList.size(); i++)
    {
        delete m_vUpropList[i];
    }
    
    for(int i=0; i<m_vUcardList.size(); i++)
    {
        delete m_vUcardList[i];
    }
    
    for(int i=0; i<m_vUroleList.size(); i++)
    {
        delete m_vUroleList[i];
    }
    
}

CLSceneManager* CLSceneManager::sharedSceneManager()
{
    if(!g_SceneManager)
    {
        g_SceneManager = new CLSceneManager();
       
    }
    return g_SceneManager;
}

void CLSceneManager::ChangeScene(CLSceneLayer* pScene)                 //切换场景
{
    ChangeDeviceBGFromSceneID(pScene->getnCTIndex());
    
    if(m_pRunningScene)
    {
        CCSize size = CCDirector::sharedDirector()->getgSceneSize();
        setpReplaceNode(pScene);
//        m_pFadeLayer = CLFadeColorLayer::create(ccc4(0, 0, 0, 0), size.width, size.height);
//        m_pFadeLayer->setAnchorPoint(CCPointZero);
//        m_pFadeLayer->setPosition(CCPointZero);
//        
//        if(m_pParentNode)
//        {
//            m_pParentNode->addChild(m_pFadeLayer, 10);
//        }
//        
//        CCFadeIn* pFadeIn = CCFadeIn::create(0.3);
//        CCCallFunc* pCall = CCCallFunc::create(this, callfunc_selector(CLSceneManager::ChangeSceneCall));
//        CCSequence* pSeq = (CCSequence*)CCSequence::create(pFadeIn, pCall, NULL);
//        m_pFadeLayer->runAction(pSeq);
        
        ChangeSceneCall();
        
    }
    else
    {
        if(m_pParentNode)
        {
            m_pRunningScene = pScene;
            m_pParentNode->addChild(pScene, 1);
        }
    }
    
}

void CLSceneManager::ChangeSceneCall()
{
    if(m_pTempCoverNode)
    {
        m_pTempCoverNode->removeFromParentAndCleanup(true);
        m_pTempCoverNode = NULL;
        m_pReCoverNode = NULL;
    }
    if(m_pRunningScene)
    {
        m_pRunningScene->removeFromParentAndCleanup(true);
        m_pRunningScene = NULL;
    }
    
    if(m_pParentNode)
    {
        m_pRunningScene = m_pReplaceNode;
        m_pParentNode->addChild(m_pReplaceNode, 1);
        setpReplaceNode(NULL);
    }
    
    if(m_pLoadingLayer->isVisible())
        m_pLoadingLayer->setVisible(false);
    
//    CCFadeOut* pFadeOut = CCFadeOut::create(0.3);
//    CCCallFunc* pCallEnd = CCCallFunc::create(this, callfunc_selector(CLSceneManager::ChangeSceneCallEnd));
//    CCSequence* pSeq = (CCSequence*)CCSequence::create(pFadeOut, pCallEnd, NULL);
//    m_pFadeLayer->runAction(pSeq);
    
}

void CLSceneManager::ChangeSceneCallEnd()
{
    m_pParentNode->removeChild(m_pFadeLayer, true);
    m_pFadeLayer = NULL;
}


void CLSceneManager::AddTempCoverScene(CCNode* pScene)                 //增加临时覆盖场景
{
    if(m_pRunningScene)
    {
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        m_pFadeLayer = CLFadeColorLayer::create(ccc4(0, 0, 0, 255), size.width, size.height);
        m_pFadeLayer->setAnchorPoint(CCPointZero);
        m_pFadeLayer->setPosition(CCPointZero);
        setpReCoverNode(pScene);
        
        if(m_pParentNode)
        {
            m_pParentNode->addChild(m_pFadeLayer, 10);
        }
        
        CCFadeIn* pFadeIn = CCFadeIn::create(0.3);
        CCCallFunc* pCall = CCCallFunc::create(this, callfunc_selector(CLSceneManager::ReCoverSceneCall));
        CCSequence* pSeq = (CCSequence*)CCSequence::create(pFadeIn, pCall, NULL);
        m_pFadeLayer->runAction(pSeq);
    }

}

void CLSceneManager::RemoveTempCoverScene(CCNode* pScene)              //还原到主场景
{
    if(pScene != m_pTempCoverNode )
    {
        CCLOG("CoverScene Layer is wrong!!!!!!!");
        return;
    }
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    m_pFadeLayer = CLFadeColorLayer::create(ccc4(0, 0, 0, 255), size.width, size.height);
    m_pFadeLayer->setAnchorPoint(CCPointZero);
    m_pFadeLayer->setPosition(CCPointZero);
    setpReCoverNode(pScene);
    
    if(m_pParentNode)
    {
        m_pParentNode->addChild(m_pFadeLayer, 10);
    }
    
    CCFadeIn* pFadeIn = CCFadeIn::create(0.3);
    CCCallFunc* pCall = CCCallFunc::create(this, callfunc_selector(CLSceneManager::ReCoverSceneCallEnd));
    CCSequence* pSeq = (CCSequence*)CCSequence::create(pFadeIn, pCall, NULL);
    m_pFadeLayer->runAction(pSeq);
    
}

void CLSceneManager::ReCoverSceneCall()               //增加临时场景阴影层黑掉后的回调函数
{
    if(m_pTempCoverNode && m_pParentNode)
    {
        m_pParentNode->removeChild(m_pTempCoverNode, true);
        m_pTempCoverNode = NULL;
    }
        
    if(m_pParentNode)
    {
        m_pParentNode->addChild(m_pReCoverNode, m_pRunningScene->getZOrder()+1);
        m_pTempCoverNode = m_pReCoverNode;
    }
    m_pRunningScene->setVisible(false);
    setpReCoverNode(NULL);
    
    CCFadeOut* pFadeOut = CCFadeOut::create(0.3);
    CCCallFunc* pCallEnd = CCCallFunc::create(this, callfunc_selector(CLSceneManager::ChangeSceneCallEnd));
    CCSequence* pSeq = (CCSequence*)CCSequence::create(pFadeOut, pCallEnd, NULL);
    m_pFadeLayer->runAction(pSeq);
}

void CLSceneManager::ReCoverSceneCallEnd()
{
    m_pParentNode->removeChild(m_pReCoverNode, true);
    m_pRunningScene->setVisible(true);
    setpReCoverNode(NULL);
    m_pTempCoverNode = NULL;
    
    CCFadeOut* pFadeOut = CCFadeOut::create(0.3);
    CCCallFunc* pCallEnd = CCCallFunc::create(this, callfunc_selector(CLSceneManager::ChangeSceneCallEnd));
    CCSequence* pSeq = (CCSequence*)CCSequence::create(pFadeOut, pCallEnd, NULL);
    m_pFadeLayer->runAction(pSeq);
    
}

void CLSceneManager::BeginLoadAdvImages(int nMsgId, int nLoadModeStyle)                 //网络消息下达后开始预加载资源
{
    if(!m_pLoadingLayer->isVisible())
        m_pLoadingLayer->setVisible(true);
    
    if(!CLLoadImageManager::shardLIManager()->CheckNeedAdvLoad(nMsgId, nLoadModeStyle)) //如果不需要预加载资源
    {
        (m_pCallTarget->*m_pCallFun)();
    }
    else
    {
        CLLoadImageManager::shardLIManager()->StartLoadAdvImages();
    }
    
}

void CLSceneManager::processDataWithDic(Json::Value &dic, cocos2d::CCObject* request)
{
    
}

void CLSceneManager::BeginNetToChangeScene(CCObject* targetScene, SEL_CallFunc pCallFun, int nMsgId)//开始请求服务器数据来切换场景
{
    m_pCallTarget = targetScene;
    m_pCallFun    = pCallFun;
    Json::Value jv(Json::objectValue);
    m_pLoadingLayer->setVisible(true);
    
}

void CLSceneManager::SetNetErrorLayer(bool bCreate, std::string sText)             // 生成或者取消这个层
{
    if(m_pNetErrorLayer)
    {
        m_pNetErrorLayer->removeFromParentAndCleanup(true);
        m_pNetErrorLayer = NULL;
    }
    if(bCreate)
    {

        m_pNetErrorLayer = CLNetErrorLayer::create(sText.c_str());
        m_pNetErrorLayer->setAnchorPoint(CCPointZero);
        m_pNetErrorLayer->setPosition(CCPointZero);
        m_pParentNode->addChild(m_pNetErrorLayer, 10);
    }
    
} 

void CLSceneManager::SetWaitNetLayer(bool bShow)
{
    if(bShow)
    {
        if(!m_pWaitNetLayer)
        {
            m_pWaitNetLayer = CLWaitNetLayer::create();
            m_pWaitNetLayer->setAnchorPoint(CCPointZero);
            m_pWaitNetLayer->setPosition(CCPointZero);
            m_pParentNode->addChild(m_pWaitNetLayer, 10);
        }
        else
            m_pWaitNetLayer->setVisible(true);
    }
    else
    {
        if(m_pWaitNetLayer)
            m_pWaitNetLayer->setVisible(false);
    }

}

//添加一个网络消息数据到列表中
void CLSceneManager::AddJsonDataToList(Json::Value* jv)
{
    int nMsgId = (*jv)["msgid"].asIntModestyle();
    
    m_vJsonData[nMsgId] = jv;
}

//取出一个网络数据
Json::Value* CLSceneManager::GetJsonDataFromMsgID(int nMsgId)
{
    Json::Value* pValue = NULL;
    std::map<int, Json::Value*>::iterator iter = m_vJsonData.find(nMsgId);
    if(iter != m_vJsonData.end())
    {
        pValue = iter->second;
    }
    
    return pValue;
}

void CLSceneManager::RemoveJsonDataFromMsgID(int nMsgId)
{
    std::map<int, Json::Value*>::iterator iter = m_vJsonData.find(nMsgId);
    if(iter != m_vJsonData.end())
    {
        delete iter->second;
        m_vJsonData.erase(iter);
    }
}


//添加一个商店信息到列表
void CLSceneManager::AddStoreDataToList(CLStoreData* pData)
{
    m_vStoreDataList.push_back(pData);
}

void CLSceneManager::ClearStoreDataList()
{
    for(int i=0; i<m_vStoreDataList.size(); i++)
        delete m_vStoreDataList[i];
    
    m_vStoreDataList.clear();
}

//取出一个商店列表信息
CLStoreData* CLSceneManager::GetStoreDataFromList(int nIndex)
{
    if(nIndex >= m_vStoreDataList.size())
        return NULL;
    
    return m_vStoreDataList[nIndex];
}

int CLSceneManager::GetStoreDataNum()
{
    return m_vStoreDataList.size();
}

//添加一个房间信息到房间列表
void CLSceneManager::AddPvpRoomInfoToList(PvpRoomInfo* pData)
{
    m_vPvpRoomList.push_back(pData);
}

//得到Pvp房间数量
int CLSceneManager::GetPvpRoomNum()
{
    return m_vPvpRoomList.size();
}

//得到一个房间信息
PvpRoomInfo* CLSceneManager::GetPvpRoomInfoFromIndex(int nIndex)
{
    if(nIndex >= m_vPvpRoomList.size())
        return NULL;
    return m_vPvpRoomList[nIndex];
}


void CLSceneManager::ClearPvpRoomList()
{
    for(int i=0; i<m_vPvpRoomList.size(); i++)
    {
        delete m_vPvpRoomList[i];
    }
    m_vPvpRoomList.clear();
}


////添加一个印花信息到房间列表
void CLSceneManager::AddPrizeInfoToList(PrizeInfo* pData)
{
    m_vPrizeList.push_back(pData);

}

//得到印花数量
int CLSceneManager::GetPrizeNum()
{
    return m_vPrizeList.size();
}

//印花列表信息
void CLSceneManager::ClearPrizeList()
{
    for(int i=0; i<m_vPrizeList.size(); i++)
    {
        delete m_vPrizeList[i];
    }
    m_vPvpRoomList.clear();
}

//得到一个印花信息
PrizeInfo* CLSceneManager::GetPrizeInfoFromIndex(int nIndex)
{
    if(nIndex >= m_vPrizeList.size())
    return NULL;
    return m_vPrizeList[nIndex];
}

//wangguolong 20131111
void CLSceneManager::AddUcardInfoToList(UcardInfo* pData)
{
    m_vUcardList.push_back(pData);
}

//得到对战设置背包题卡数量
int CLSceneManager::GetUcardNum()
{
    return m_vUcardList.size();
}

//对战设置背包题卡信息
void CLSceneManager::ClearUcardList()
{
    for(int i=0; i<m_vUcardList.size(); i++)
    {
        delete m_vUcardList[i];
    }
    m_vUcardList.clear();
}

//得到对战设置背包题卡信息
UcardInfo* CLSceneManager::GetUcardInfoFromIndex(int nIndex)
{
    if(nIndex >= m_vUcardList.size())
        return NULL;
    return m_vUcardList[nIndex];
}


//添加对战设置背包道具信息到list
void CLSceneManager::AddUpropInfoToList(UpropInfo* pData)
{
    m_vUpropList.push_back(pData);
}
//得到对战设置背包道具数量
int CLSceneManager::GetUpropNum()
{
    return m_vUpropList.size();
}
//对战设置背包道具信息
void CLSceneManager::ClearUpropList()
{
    for(int i=0; i<m_vUpropList.size(); i++)
    {
        delete m_vUpropList[i];
    }
    m_vUpropList.clear();
    
}
//得到对战设置背包道具信息
UpropInfo* CLSceneManager::CLSceneManager::GetUpropInfoFromIndex(int nIndex)
{
    if(nIndex >= m_vUpropList.size())
        return NULL;
    return m_vUpropList[nIndex];
}



//添加对战设置背包角色信息到list
void CLSceneManager::AddUroleInfoToList(UroleInfo* pData)
{
    m_vUroleList.push_back(pData);
}
//得到对战设置背包角色数量
int CLSceneManager::GetUroleNum()
{
    return m_vUroleList.size();
}
//对战设置背包角色信息
void CLSceneManager::ClearUroleList()
{
    for(int i=0; i<m_vUroleList.size(); i++)
    {
        delete m_vUroleList[i];
    }
    m_vUroleList.clear();
}
//得到对战设置背包角色信息
UroleInfo* CLSceneManager::GetUroleInfoFromIndex(int nIndex)
{
    if(nIndex >= m_vUroleList.size())
        return NULL;
    return m_vUroleList[nIndex];
}

void CLSceneManager::getStatusMessage()
{
    int ZERO = 0;
    //发送内容
    Json::Value data;
    data["msgid"].append("39"); //指令码
    
    //选择的题卡
    std::map<int,int>::iterator UCard;
    int nCard = 0;
    for(UCard=UcardStatusMap.begin();UCard!=UcardStatusMap.end();UCard++)
    {
        std::string sCard;
        
        CCLog("UCard->second: %i",UCard->second);
        
        if(UCard->second != 0)
        {
            nCard++;
            sCard = CCString::createWithFormat("card%d", nCard)->getCString();
            
            data[sCard] = UCard->second;
        }

    }
    
    if(nCard == 1)
    {
        data["card2"] = ZERO;
        data["card3"] = ZERO;
    }
    
    if(nCard == 2)
    {
        data["card3"] = ZERO;
    }
    
    //选择的道具
    std::map<int,int>::iterator Uprop;
    int nProp = 0;
    for(Uprop=UpropStatusMap.begin();Uprop!=UpropStatusMap.end();Uprop++)
    {
        std::string sProp;
        

        CCLog("Uprop->second: %i",Uprop->second);
        
        if(Uprop->second != 0)
        {
            nProp++;
            
            sProp = CCString::createWithFormat("prop%d", nProp)->getCString();
            data[sProp] = Uprop->second;
        }
    }
    
    if(nProp == 1)
    {
        data["prop2"] = ZERO;
    }
    
    //选择的角色
    std::map<int,int>::iterator Urole;

    for(Urole=UroleStatusMap.begin();Urole!=UroleStatusMap.end();Urole++)
    {

        CCLog("Urole->second: %i",Urole->second);
        data["role"] = Urole->second;
    }
    
    //CMD_PK1_EQUIP
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_PK1_EQUIP, data);
    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
    
    
}




void CLSceneManager::AddUcardStatusMap(int nKey,int nValue)
{
    UcardStatusMap[nKey] = nValue;
}
int CLSceneManager::FindUcardStatusMap(int nKey)
{
    int value = 0;
    std::map<int,int>::iterator it = UcardStatusMap.find(nKey);
    if(it != UcardStatusMap.end()){
        
        value = UcardStatusMap[nKey];
    }else{
        value = 0;
    }
    
    return value;
}
void CLSceneManager::AddUpropStatusMap(int nKey,int nValue)
{
    UpropStatusMap[nKey] = nValue;
}
int CLSceneManager::FindUpropStatusMap(int nKey)
{
    int value = 0;
    std::map<int,int>::iterator it = UpropStatusMap.find(nKey);
    if(it != UpropStatusMap.end()){
        
        value = UpropStatusMap[nKey];
    }else{
        value = 0;
    }
    
    return value;
}
void CLSceneManager::AddUroleStatusMap(int nKey,int nValue)
{

    UroleStatusMap[nKey] = nValue;
}
int CLSceneManager::FindUroleStatusMap(int nKey)
{
    int value = 0;
    std::map<int,int>::iterator it = UroleStatusMap.find(nKey);
    if(it != UroleStatusMap.end()){
        
        value = UroleStatusMap[nKey];
    }else{
        value = 0;
    }
    
    return value;
}

/**
 *  成就信息
 */
//添加未领取成就队列
void CLSceneManager::AddAchieveInfoToList(int nAchiNo,int needConfirm)
{
    m_iNotGetAchiList[nAchiNo] = needConfirm;
}

//添加用户成就
void CLSceneManager::AddUserAchiInfo(int nAchiNo)
{

    bool hadKey = false;
    for (int i=0; i<m_vUserInfoAchi.size(); i++) {
        if (m_vUserInfoAchi[i] == nAchiNo)
        {
            hadKey = true;
        }
    }
    if (!hadKey)
    {
        m_vUserInfoAchi.push_back(nAchiNo);
    }
    
}

//取得
int CLSceneManager::GetUngetAchiNum()
{
    return m_iNotGetAchiList.size();
}

//清除未领取容器
void CLSceneManager::DelKey(int key)
{
    std::map<int, int>::iterator it = m_iNotGetAchiList.find(key);
    if (it != m_iNotGetAchiList.end()) {
        m_iNotGetAchiList.erase(it);
    }
}

//逻辑删除
void CLSceneManager::logicRemove(int key)
{
    std::map<int, int>::iterator it = m_iNotGetAchiList.find(key);
    if (it != m_iNotGetAchiList.end()) {
        m_iNotGetAchiList[key] = 1;
    }
}

bool CLSceneManager::GetIfHadMsgShow()
{
    bool hadThisKey = false;
    std::map<int, int>::iterator   it=m_iNotGetAchiList.begin();
    for(;it!=m_iNotGetAchiList.end();++it)
    {
//        cout<<"key:"<<it->first
//        <<"value:"<<it->second<<end1;
        if (it->second == 0)
        {
            hadThisKey = true;
        }
    }

    return hadThisKey;
}

void CLSceneManager::UpdateNewStoreLevel(int nOpenIndex)
{
    if(nOpenIndex >= m_vStoreDataList.size())
        return;
    
    m_vStoreDataList[nOpenIndex]->setbLocked(false);
    
    //wangguolong 20131218
    m_vStoreDataList[nOpenIndex]->setnewlevel(nOpenIndex);
}


/**
 *  货架星级存储
 */
void CLSceneManager::recordShelfStar(int nShelfNo,int nStarNo)
{
    m_iLastStarsMap[nShelfNo] = nStarNo;
}

//得到货架星数
int CLSceneManager::getShelfStar(int nShelfNo)
{
//    if(nShelfNo > m_AchiDataMap.size())
//        return 0;
    
    int starNo = 0;
    
    std::map<int, int>::iterator itAch = m_iLastStarsMap.find(nShelfNo);
    if(itAch != m_iLastStarsMap.end())
    {
        starNo = itAch->second;
    }
    
    return starNo;
}

void CLSceneManager::ChangeDeviceBGFromSceneID(CLScene_TypeIndex nIndex)
{
    switch (nIndex)
    {
//        case CLScene_MainScene:    //主场景
//        {
//            CCTexture2D* pTex = CCTextureCache::sharedTextureCache()->addImage("Image/LoginLayer/Dv_MainLayer.png");
//            m_pDeviceBG->setTexture(pTex);
//        }
//            break;
//        case CLScene_LoginGame:    //登陆
//        {
//            CCTexture2D* pTex = CCTextureCache::sharedTextureCache()->addImage("Image/LoginLayer/Dv_MainLayer.png");
//            m_pDeviceBG->setTexture(pTex);
//        }
//            break;
//        case CLScene_TopLayer:     //Top页面
//        {
//            CCTexture2D* pTex = CCTextureCache::sharedTextureCache()->addImage("Image/LoginLayer/Dv_LoginBg.png");
//            m_pDeviceBG->setTexture(pTex);
//        }
//            break;
//        case CLScene_ShelfLayer:   //货架场景
//        {
//            CCTexture2D* pTex = CCTextureCache::sharedTextureCache()->addImage("Image/LoginLayer/Dv_PveBg.png");
//            m_pDeviceBG->setTexture(pTex);
//        }
//            break;
//        case CLScene_PveLayer:     //闯关
//        {
//
//        }
//            break;
//        case CLScene_PvpLayer:     //对战
//        {
//            
//        }
//            break;
//            
//        default:
//            break;
    }
}


