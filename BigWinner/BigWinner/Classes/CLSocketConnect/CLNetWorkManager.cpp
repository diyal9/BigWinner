//
//  CLNetWorkManager.cpp
//  BigWinner
//
//  Created by ZhangQiyun on 13-10-8.
//
//

#include "CLNetWorkManager.h"
#include "GameCommand.h"
#include "LGameCommonDefine.h"
#include "CRequestMsgConstDedine.h"
#include "UserInfo.h"
#include "CLPlayerManager.h"
#include "CLSceneManager.h"
#include "BackPack.h"
#include "CLMainSceneLayer.h"
#include "CompoundSceneMain.h"
#include "CLPveSceneLayer.h"
#include "Shop.h"
#include "DlgTiShiLayer.h"
#include "CLChangeString.h"
#include "YHtopLayer.h"
#include "YHUILayer.h"
#include "JsonDataManager.h"
#include "CLPvpSceneLayer.h"
#include "CLPvpReady.h"
#include "CLPvpingLayer.h"
#include "Achievements.h"
#include "CLChangeString.h"
#include "CLRealGameLayer.h"
#include "DlgLayerBtnA.h"
#include "CLShelfsSceneLayer.h"
#include "BuyMoneyLayer.h"

#pragma  断网提示
#include "DlgLayerBtnB.h"

static CLNetWorkManager* g_pNetWorkManager = NULL;

CLNetWorkManager::CLNetWorkManager()
{
    m_pClientSocket = NULL;
    m_isReconnect = false;
    m_isFirstJoin = true;
    m_backValue = NULL;
}

CLNetWorkManager::~CLNetWorkManager()
{
    if(m_pClientSocket)
    {
        CC_SAFE_RELEASE_NULL(m_pClientSocket);
    }
}

CLNetWorkManager* CLNetWorkManager::shardNetWorkManager()
{
    if(!g_pNetWorkManager)
    {
        g_pNetWorkManager = new CLNetWorkManager();
    }
    return g_pNetWorkManager;
}

//连接网络
bool CLNetWorkManager::Connect(const char* szServerIP, int wPort)
{
    
    if (m_pClientSocket)
        CC_SAFE_RELEASE_NULL(m_pClientSocket);
    
    if(!m_pClientSocket)
    {
        m_pClientSocket = CClientSocket::create();
        m_pClientSocket->retain();
    }
    if(!m_pClientSocket->Connect(szServerIP, wPort))
    {
        return false;
    }
    
   // m_bHasConnectLS = true;
    return true;
}

bool CLNetWorkManager::onsocketDicconnect()
{
    if (m_pClientSocket && (m_pClientSocket->IsDisconnecting() == false))
	{
		m_pClientSocket->Disconnect();
		CCLOGWARN("Disconnect server by client:%d", m_pClientSocket->retainCount());
		CC_SAFE_RELEASE_NULL(m_pClientSocket);
		
	}
    
    //m_bHasConnectLS = false;
    
	return true;
}

void  CLNetWorkManager::JumpSendMessage()
{
    std::string sData = "{}";
    if (m_pClientSocket)
        m_pClientSocket->sendCommand((void*)sData.c_str(), sData.size());
}

bool   CLNetWorkManager::LogonReq()
{
    
    //依据帐号信息连接登陆服务器
    std::string sAccount, sPassword, deviceId, osPlatform;
    Json::Value jv(Json::objectValue);
    
    int netStatus; //网络状态
    
    
    
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
        //是 iOS  平台
        osPlatform = "IOS";
    }
    else
    {
        //其他平台
    }
#if 0
    deviceId = GetDeviceMacAddress(); //取得设备ID
    netStatus = GetNetwrokType(); //获得网络状态
#endif
    //android平台没有接口  真机会挂
    long androidDes = getCurrentTime(); //其它平台使用时间作为Des
    deviceId = CCString::createWithFormat("%ld",androidDes)->getCString();
    netStatus = 1;
    
    
    if(!CheckHasAccountRecord(sAccount, sPassword))  //如果没有帐号纪录
    {
//#if 0
        
        sAccount = "";
        sPassword = "";
        jv["userID"]    = sAccount;       //纪录帐号
        jv["password"]  = sPassword;      //纪录密码

//#endif
        //由于每个人mac不同先暂时写死 gaoyuan
//        jv["userID"]    = "zhangqiyun";       //纪录帐号
//        jv["password"]  = "zhangqiyun";      //纪录密码
       
        //服务器回复后再去存储
//        Json::Value jvFile(Json::objectValue); //存储用
//        CCLog("send2Info->%s,%s", JsonDataManager::shardJDManager()->getsPlayerAccount().c_str(), JsonDataManager::shardJDManager()->getsPlayerPasswoed().c_str());
//        jvFile["userID"] = JsonDataManager::shardJDManager()->getsPlayerAccount();
//        jvFile["password"] = JsonDataManager::shardJDManager()->getsPlayerPasswoed();
//        
//        std::string sPath = CCFileUtils::sharedFileUtils()->getWritablePath() + "BigWinner_Account.json";
//        Json::FastWriter writer;
//        std::string sWValue = writer.write(jvFile);
//        FILE* pWfile = fopen(sPath.c_str(), "wb+");    //纪录到文件
//        fwrite(sWValue.c_str(), 1, sWValue.size(), pWfile);
//        fclose(pWfile);
        
        
    }
    else
    {
        jv["userID"]    = sAccount;       //纪录帐号
        jv["password"]  = sPassword;      //纪录密码
        //由于每个人mac不同先暂时写死 gaoyuan
    }
//    deviceId = "diyal100001"; //取得设备ID
//    netStatus = GetNetwrokType(); //获得网络状态
//    jv["userID"]    = "dragonwang";       //纪录帐号
//    jv["password"]  = "dragonwang";      //纪录密码
    //固定发送内容
    jv["deviceid"]    = deviceId;
    jv["os"] = osPlatform;
    jv["network"] = netStatus;
    
    SendNetMessage(CMD_LOGIN, jv);

    
    return true;
}

bool CLNetWorkManager::CheckHasAccountRecord(std::string& sAccount, std::string& sPassword)
{
    std::string sPath = CCFileUtils::sharedFileUtils()->getWritablePath() + "BigWinner_Account.json";
    FILE* pFile = fopen(sPath.c_str(), "rb");
    if(!pFile)                //说明这个文件并不存在 那么返回false
    {
        return false;
    }
    else                      //如果此文件存在 那么先读 帐号信息
    {
        fseek(pFile,0,SEEK_END);
        int nSize = ftell(pFile);
        fseek(pFile,0,SEEK_SET);
        char* pBuffer = new char[nSize];
        nSize = fread(pBuffer,sizeof(char), nSize,pFile);
        fclose(pFile);
        
        Json::Value pv;
        Json::Reader jsReader;
        jsReader.parse(pBuffer, pv, false);
        
        if(pv.isMember("userID") && pv["userID"].asStringModeStyle().compare("") != 0 && pv.isMember("password") && pv["password"].asStringModeStyle().compare("") != 0)   //如果文件里面有帐号和密码信息
        {
            delete pBuffer;
            sAccount = pv["userID"].asStringModeStyle();
            sPassword = pv["password"].asStringModeStyle();
            
            JsonDataManager::shardJDManager()->setsPlayerAccount(sAccount);
            JsonDataManager::shardJDManager()->setsPlayerPasswoed(sPassword);
            
            return true;
        }
        else
        {
            delete pBuffer;
            return false;
        }
    }
}

bool CLNetWorkManager::ReConnectNoti()
{
    CCNode* node = CLSceneManager::sharedSceneManager()->getpParentNode()->getChildByTag(11111);
    if (!node)
    {
        CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
        CLSceneManager::sharedSceneManager()->getpLoadingLayer()->setVisible(false);
        
        DlgLayerBtnB * dlg = DlgLayerBtnB::createBtn(CLChangeString::sharedChangeString()->GetChangeString("quxiao"),
                                                     CLChangeString::sharedChangeString()->GetChangeString("queren"),
                                                     CLChangeString::sharedChangeString()->GetChangeString("noconnectReq"));
        
        CLSceneManager::sharedSceneManager()->getpParentNode()->addChild(dlg, DLG_GENERAL_ZORDER, 11111);
        dlg->pBtnRight->setOnClickCallback(callfuncO_selector(CLNetWorkManager::reconnectOk), this);
    }
    
    
    
    return true;
}

void CLNetWorkManager::reconnectOk(CCObject* r)
{
    
    CLSceneManager::sharedSceneManager()->getpParentNode()->removeChildByTag(11111, true);
    
    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);

    
    if(!Connect(JsonDataManager::shardJDManager()->getsLoginUrl().c_str(), JsonDataManager::shardJDManager()->getnLoginPort()))
    {
        //此处需要一个屏蔽后面操作的Dielog 点击后重新连接； 暂时先打印一个错误的信息1314
        CCLOG("Connect Login Server is error");
        ReConnectNoti();
    }
    
    //重连
    m_isReconnect = true;
    
    //开启心跳
    CLRealGameLayer* layer =  (CLRealGameLayer*)CLSceneManager::sharedSceneManager()->getpParentNode();
    layer->StartJumpNetWork();
    
    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
    
    //登陆请求
    LogonReq();
    
}



//发送网络数据
void CLNetWorkManager::SendNetMessage(int nMsgID, Json::Value& value)
{
	//封装json ID
    value["msgid"] = nMsgID;

	//最快速json模式
	Json::FastWriter writer;
    std::string sData(writer.write(value));
    m_pClientSocket->sendCommand((void*)sData.c_str(), sData.size());
}

//接收网络数据
void CLNetWorkManager::RecvNetMessage()
{
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    RecvCommand* pRecvData = NULL;
    pRecvData = m_pClientSocket->getRecvCommand();
    if(!pRecvData)
        return;
    
    if(pRecvData->getSize() <= 0)
    {
        m_pClientSocket->removeRecvCommand();
        return;
    }
    
    //json parse
    Json::Value root;
    Json::Reader reader;
        
    std::string str((char*)pRecvData->getData());
    Json::FastWriter writer;

    if (!reader.parse(str.c_str(), root,false))
    {
        return;
    }
    
    switch (root["result"].asIntModestyle())
    {
        case ERR_SUCCEED:  //正常
        {
            //分消息ID类型分别处理
            switch (root["msgid"].asIntModestyle())
            {
                    
                case CMD_LOGIN_REPLY:         //登陆回复
                {
                    CCLOG("gaoyuanMsgID: CMD_LOGIN_REPLY content:%s", root.toStyledString().c_str());
                    CLPlayerManager::shardPlayerManager()->init(root);
                    
                    //断线重连不需要重新加载场景
                    if (m_isReconnect == false && m_isFirstJoin == true)
                    {
                        m_isFirstJoin = false;//不是第一次进游戏了
                        CLSceneManager::sharedSceneManager()->BeginLoadAdvImages(CMD_LOGIN_REPLY);
                    }
                    
                    std::string sAccount, sPassword;
                    if (!CheckHasAccountRecord(sAccount, sPassword)) {
                        Json::Value jvFile(Json::objectValue); //存储用
                        CCLog("send2Info->%s,%s", root["userID"].asStringModeStyle().c_str(), root["password"].asStringModeStyle().c_str());
                        jvFile["userID"] = root["userID"];
                        jvFile["password"] = root["password"];
                        
                        std::string sPath = CCFileUtils::sharedFileUtils()->getWritablePath() + "BigWinner_Account.json";
                        Json::FastWriter writer;
                        std::string sWValue = writer.write(jvFile);
                        FILE* pWfile = fopen(sPath.c_str(), "wb+");    //纪录到文件
                        fwrite(sWValue.c_str(), 1, sWValue.size(), pWfile);
                        fclose(pWfile);
                    }
                    
                    break;
                    
                }
                case CMD_MYINFO_REPLY:   //我的信息
                {
                    CCLOG("gaoyuanMsgID: CMD_MYINFO_REPLY content:%s", root.toStyledString().c_str());
                    break;
                }
                case CMD_BACKPACK_REPLY:  //背包回复
                {
                    CCLOG("gaoyuanMsgID: CMD_BACKPACK_REPLY content:%s", root.toStyledString().c_str());
                    
                    //初始化个人背包信息
                    CLPlayerManager::shardPlayerManager()->getpUserInfo()->initUserCardInfo(root);
                    
                    //跳转背包层
                    BackPackLayer* bp = BackPackLayer::create(BK_TIKA, root);
                    bp->setPosition(ccp(0, size.height * (-1)));
                    CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
                    {
                        CCLOG("Error: The cur scene is not the main scene!!!");
                        break;
                    }
                    pMainLayer->addChild(bp,10, BACKPACK);
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                    break;
                }
                case CMD_STORES_REPLY:  //商铺回复
                {
                    CCLOG("gaoyuanMsgID: CMD_STORES_REPLY content:%s", root.toStyledString().c_str());
                    
                    CLSceneManager::sharedSceneManager()->ClearStoreDataList();
                    for(int i=0; i<root["stores"].size(); i++)
                    {
                        CLStoreData* pData = new CLStoreData(root["stores"][i]);
                        CLSceneManager::sharedSceneManager()->AddStoreDataToList(pData);
                    }
                    
                    
                    if(root["stores"][1]["Locked"].asIntModestyle() == 0)
                    {
                        //设置第一个商铺状态
                        CLSceneManager::sharedSceneManager()->GetStoreDataFromList(1)->setnOneLevel(0);
                        
                    }else
                    {
                        //设置第一个商铺状态
                        CLSceneManager::sharedSceneManager()->GetStoreDataFromList(1)->setnOneLevel(1);
                    }
                    
                    CLSceneLayer* pLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    if(pLayer->getnCTIndex() == CLScene_MainScene)
                    {
                        CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)pLayer;
                        if(!pMainLayer->getpStoreMenu())
                        {
                            pMainLayer->CreateStoresMenu();
                        }
                    }
                    break;
                }
                case CMD_SHELFS_REPLY: //查询货架回复
                {
                    CCLOG("gaoyuanMsgID: CMD_SHELFS_REPLY content:%s", root.toStyledString().c_str());
                    Json::Value* pJv = new Json::Value(root);
                    CLSceneManager::sharedSceneManager()->AddJsonDataToList(pJv);
                    CLSceneManager::sharedSceneManager()->BeginLoadAdvImages(CMD_SHELFS_REPLY);
                    break;
                }
                case CMD_JOIN_REPLY: //进入货架后回复
                {
                    CCLOG("gaoyuanMsgID: CMD_JOIN_REPLY content:%s", root.toStyledString().c_str());
                    Json::Value* pJv = new Json::Value(root);
                    CLSceneManager::sharedSceneManager()->AddJsonDataToList(pJv);
                    CLSceneManager::sharedSceneManager()->BeginLoadAdvImages(CMD_JOIN_REPLY);
                }
                    break;
                    
                case CMD_CARDCOMPOSE_REPLY: //合成回复
                {
                    CCLOG("gaoyuanMsgID: CMD_CARDCOMPOSE_REPLY content:%s", root.toStyledString().c_str());
                    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
                    {
                        CCLOG("Error: The cur scene is not the main scene!!!");
                        return;
                    }
                    BackPackLayer* bl = (BackPackLayer*)pMainLayer->getChildByTag(BACKPACK);
                    if (bl)
                    {
                        CompoundSceneMain* lay = dynamic_cast<CompoundSceneMain*>(bl->getChildByTag(BK_GENERAL_TAG));
                        if (lay)
                        {
                            lay->_CardHeCallBack(root);
                        }
                        else
                        {
                            CCLOG("合成界面不存在");
                        }
                    }
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                    
                }
                    break;
                case CMD_QUESTION: //答题回复
                {
                    CCLOG("gaoyuanMsgID: CMD_Answer_REPLY content:%s", root.toStyledString().c_str());
                    CLSceneLayer* pLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    if(pLayer->getnCTIndex() ==  CLScene_PveLayer)  //如果在闯关界面
                    {
                        CLPveSceneLayer* pPveLayer =(CLPveSceneLayer*)pLayer;
                        pPveLayer->UpdateTopicLayer(root);
                    }
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                    
                }
                    break;
                case CMD_PVEROUNDEND: //货架闯关答题结束
                {
                    CCLOG("gaoyuanMsgID: CMD_PVEROUNDEND content:%s", root.toStyledString().c_str());
                    CLSceneLayer* pLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    if(pLayer->getnCTIndex() == CLScene_PveLayer)  //如果在闯关界面
                    {
                        CLPveSceneLayer* pPveLayer =(CLPveSceneLayer*)pLayer;
                        pPveLayer->ShowPveResultLayer(root);
                        
                        //记录解锁的店铺
                        CLSceneManager::sharedSceneManager()->setnNolock(root["newlevel"].asIntModestyle());
                    }
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                }
                    break;
                    
                case CMD_SHOPSHOW_REPLY: //商店查询回复
                {
//                    CCLOG("gaoyuanMsgID: CMD_SHOPSHOW_REPLY content:%s zipBefore:%d", root.toStyledString().c_str(), root.toStyledString().size());
                    
                    Json::FastWriter writer;
                    std::string sData(writer.write(root));
//                    CCLog("zipAfterData:%s  zipAfter:%d ", sData.c_str(), sData.size() );
                    
                    ShopLayer* bp = ShopLayer::create(1, root);
                    bp->setPosition(ccp(0, size.height * (-1)));
                    
                    CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    
                    if(pMainLayer->getnCTIndex() == CLScene_ShelfLayer)
                    {
                        pMainLayer->addChild(bp,10, SHOPTAG);
                        
                    }
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                    
                    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
                    {
                        CCLOG("Error: The cur scene is not the main scene!!!");
                        break;
                    }
                    pMainLayer->addChild(bp,10, SHOPTAG);
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                }
                    break;
                case CMD_SHOPBUY_REPLY: //商店购买成功回复
                {
                    
                    CCLOG("gaoyuanMsgID: CMD_SHOPBUY_REPLY content:%s", root.toStyledString().c_str());
                    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                     CLShelfsSceneLayer* pShelfLayer = (CLShelfsSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    if(pMainLayer->getnCTIndex() != CLScene_MainScene && pShelfLayer->getnCTIndex() != CLScene_ShelfLayer)
                    {
                        CCLOG("Error: The cur scene is not the main scene!!!");
                        return;
                    }

                    if (root["result"].asIntModestyle() != 0)
                    {
                        //服务器返回结果为0，表示购买成功,否则为出现异常
                        return;
                    }
                
                    ShopLayer* bl = (ShopLayer*)pMainLayer->getChildByTag(SHOPTAG);
                    if (bl)
                    {
//                        //根据购买画面天假的通用节点从父节点删掉
//                        CCNode* general = dynamic_cast<CCNode*>(bl->getChildByTag(SP_GENERAL_TAG));
//                        general->removeChildByTag(SHOPBUYPALYER_TAG, true);
                
                        bl->_netCallBack();  //回调商店
  
                        //添加购买商品展示页面
                        ShowGoodsLayer *pShowGoods = ShowGoodsLayer::create(root["no"].asIntModestyle());
                        pShowGoods->setPosition(ccp(0,0));
                        bl->addChild(pShowGoods,20);
                        
//                        //添加dlg
//                        TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("ShopBuySuccess"));
                        
                        //记录购买的是那张角色 wangguolong
                        bl->BuyRoleNoMap[bl->getnRoleNo()] = 1;
                    }
                    
                    //修改用户信息
                    int money = root["jetton"].asIntModestyle();
                    int card = root["moneys"].asIntModestyle();
                    
                    CLPlayerManager::shardPlayerManager()->getpUserInfo()->setJetton(card); //购物卡
                    CLPlayerManager::shardPlayerManager()->getpUserInfo()->setMoneys(money);  //钱
                    
                    if(pMainLayer->getnCTIndex() == CLScene_MainScene)
                    {
                        pMainLayer->getpMoneyFont()->setString(CCString::createWithFormat("%d", CLPlayerManager::shardPlayerManager()->getpUserInfo()->getMoneys())->getCString());
                        
                        pMainLayer->getpCardFont()->setString(CCString::createWithFormat("%d", CLPlayerManager::shardPlayerManager()->getpUserInfo()->getJetton())->getCString());
                    }
                    if(pShelfLayer->getnCTIndex() == CLScene_ShelfLayer)
                    {
//                        pShelfLayer->getpMoneyFont()->setString(CCString::createWithFormat("%d", 12)->getCString());
//                        
//                        pShelfLayer->getpCardFont()->setString(CCString::createWithFormat("%d", 12)->getCString());
                    }

                    
                    //修改用户信息的道具信息
                    int sysItemNo = root["no"].asIntModestyle(); //系统物品No
                    int newNum = root["num"].asIntModestyle();
                    
                    playerCardJueseInfo* pItem = CLPlayerManager::shardPlayerManager()->getpUserInfo()->GetPropFromId(sysItemNo);
                    if (!pItem)
                    {
                        playerCardJueseInfo* newItem = new playerCardJueseInfo();
                        CardJueseDaoju* pBaseItem = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[sysItemNo];
                        newItem->m_cardJueseInfo = pBaseItem;
                        newItem->number = newNum;
                        CLPlayerManager::shardPlayerManager()->getpUserInfo()->AddItemToList(newItem);
                    }
                        CLPlayerManager::shardPlayerManager()->getpUserInfo()->GetPropFromId(sysItemNo)->number = newNum;
                    
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                }
                    break;
                
                    //印花回复 //wangguolong 20131104
                case CMD_PRIZESHOW_REPLY:
                {
                    //判断兑换功能是否开启
                    if(root.isMember("openflg"))
                    {
                    
                        //为0没有开启
                        if(root["openflg"].asIntModestyle() == 0)
                        {
                            CLSceneManager::sharedSceneManager()->setnOpenflg(0);
                        }
                        else
                        {
                            CLSceneManager::sharedSceneManager()->setnOpenflg(1);
                        }
                    }
                    
                    
                    int ZERO = 0;
                    CLSceneManager::sharedSceneManager()->ClearPrizeList();
                    for(int i=0; i<root["Prize"].size(); i++)
                    {
                        Json::Value& jv = root["Prize"][i];
                        PrizeInfo* pPrizeData = new PrizeInfo();
                        
                        //印花商品No
                        pPrizeData->setnStampNo(jv[ZERO].asIntModestyle());
                        //印花商品标识；0：普通；1：最新；2：最热"
                        pPrizeData->setnStampMark(jv[1].asIntModestyle());
                        //当前拥有的金印花数
                        pPrizeData->setnHaveGoldStampNum(jv[2].asIntModestyle());
                        //当前拥有的金印花数
                        pPrizeData->setnHaveSilverStampNum(jv[3].asIntModestyle());
                        //当前库存数量
                        pPrizeData->setnStockNum(jv[4].asIntModestyle());
                        
                        CLSceneManager::sharedSceneManager()->AddPrizeInfoToList(pPrizeData);
                        
                        if(!pPrizeData)
                        {
                            delete pPrizeData;
                        }

                    }
                    
                    CCLOG("wangguolong: CMD_PRIZESHOW_REPLY content:%s", root.toStyledString().c_str());

                    CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
                    {
                        CCLOG("Error: The cur scene is not the main scene!!!");
                        break;
                    }
                    YHtopLayer *yh = YHtopLayer::create();
                    //                    yh->setPosition(ccp(0, 0));
                    yh->setPosition(ccp(0, size.height * (-1)));
                    pMainLayer->addChild(yh,10, STAMPTAG);
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);

                }
                    break;
                case CMD_PRIZEINFO_REPLY: //印花查看回复
                {
                    CCLOG("wangguolong: CMD_PRIZEINFO_REPLY content:%s", root.toStyledString().c_str());
                    CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    
                    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
                    {
                        //闯关印花查看按钮
                        Json::Value* pJv = new Json::Value(root);
                        CLSceneManager::sharedSceneManager()->AddJsonDataToList(pJv);
                        
                        CCLOG("Error: The cur scene is not the main scene!!!");
                        break;
                    }
                    YHtopLayer* bl = (YHtopLayer*)pMainLayer->getChildByTag(STAMPTAG);
                    if(bl)
                    {
                        PrizeMessage *yh = PrizeMessage::create(root);
                        yh->setPosition(ccp(0, 0));
                        bl->addChild(yh,10, STAMPMESSAGETAG);
                    }else
                    {
                        CCLOG("界面不存在");
                    }
                    
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                    
                }
                    break;
                    
                //发送手机号码回复
                case CMD_SENDPHONE_REPLY:
                {
                    CCLOG("gaoyuan: CMD_SENDPHONE_REPLY content:%s", root.toStyledString().c_str());
                    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
                    {
                        CCLOG("Error: The cur scene is not the main scene!!!");
                        return;
                    }
                    
                    if (root["result"].asIntModestyle() != 0)
                    {
                        //服务器返回结果为0，表示购买成功,否则为出现异常
                        return;
                    }
                    
                    YHtopLayer* bl = (YHtopLayer*)pMainLayer->getChildByTag(STAMPTAG);
                    if (bl)
                    {

                        GoodsExchangeMain* general = dynamic_cast<GoodsExchangeMain*>(bl->getChildByTag(EXCHANGE_TAG_MAIN));
        
                        std::string verificationCode = root["mcode"].asStringModeStyle();
                        std::string phoneNo = root["mphone"].asStringModeStyle();
                        
                        if (general)
                        {
                            general->_sendBackVerificationCode(verificationCode, phoneNo);  
                            
                        }
                        else
                        {
                            CCLOG("界面不存在");
                        }
         
                    }
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                    break;
                }
                    
                //发送验证码回复
                case CMD_SENDCHECKCODE_REPLY:
                {
                    CCLOG("gaoyuan: CMD_SENDCHECKCODE_REPLY content:%s", root.toStyledString().c_str());
                    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
                    {
                        CCLOG("Error: The cur scene is not the main scene!!!");
                        return;
                    }
                    
                    if (root["result"].asIntModestyle() != 0)
                    {
                        //服务器返回结果为0，表示购买成功,否则为出现异常
                        return;
                    }
                    
                    YHtopLayer* bl = (YHtopLayer*)pMainLayer->getChildByTag(STAMPTAG);
                    if (bl)
                    {
                        
                        GoodsExchangeMain* general = dynamic_cast<GoodsExchangeMain*>(bl->getChildByTag(EXCHANGE_TAG_MAIN));
                        
                        std::string verificationCode = root["result"].asStringModeStyle();
                        std::string phoneNo = root["mphone"].asStringModeStyle();
                        
                        if (general)
                        {
                            general->_sendCodeBack(verificationCode, phoneNo);  //
                        }
                        else
                        {
                            CCLOG("界面不存在");
                        }
                        
                    }
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                    break;
                }
                //确认兑换回复
                case CMD_PRIZEEXCHANGEOK_REPLY:
                {
                    CCLOG("gaoyuan: CMD_SENDCHECKCODE_REPLY content:%s", root.toStyledString().c_str());
                    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
                    {
                        CCLOG("Error: The cur scene is not the main scene!!!");
                        return;
                    }
                    
                    if (root["result"].asIntModestyle() != 0)
                    {
                        //服务器返回结果为0，表示购买成功,否则为出现异常
                        return;
                    }
                    
                    YHtopLayer* bl = (YHtopLayer*)pMainLayer->getChildByTag(STAMPTAG);
                    if (bl)
                    {
                        
                        GoodsExchangeMain* general = dynamic_cast<GoodsExchangeMain*>(bl->getChildByTag(EXCHANGE_TAG_MAIN));
                        
                        std::string verificationCode = root["result"].asStringModeStyle();
                        std::string phoneNo = root["mphone"].asStringModeStyle();
                        
                        //印花商品No
                        int No = root["No"].asIntModestyle();
                        //印花商品 剩余可兑换数量
                        int leavenum = root["leavenum"].asIntModestyle();
                        
                        if (general)
                        {
                            general->_sureExchangeSuccess(No,leavenum);  //
                            
                        }
                        else
                        {
                            CCLOG("界面不存在");
                        }
                        
                    }
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                    break;
                }
                case CMD_PK1_ROOMS_REPLY:
                {
                    CCLOG("gaoyuan: CMD_PK1_ROOMS_REPLY content:%s", root.toStyledString().c_str());
                    
                    // valuetime 有此键表示还没有到对战时间，没有表示在对战时间中
                    if(root.isMember("valuetime"))
                    {
                        CLSceneManager::sharedSceneManager()->setnValuetime(root["valuetime"].asIntModestyle());
                    }else
                    {
                        CLSceneManager::sharedSceneManager()->setnValuetime(0);
                    }
                    
                    CLSceneManager::sharedSceneManager()->ClearPvpRoomList();
                    for(int i=0; i<root["grooms"].size(); i++)
                    {
                        Json::Value& jv = root["grooms"][i];
                        PvpRoomInfo* pRoomData = new PvpRoomInfo();
                        pRoomData->setnPvpPlayerNum(jv["pnum"].asIntModestyle());
                        pRoomData->setnReGYinhuaNum(jv["yh2"].asIntModestyle());
                        
                        int nYinhuaId = jv["gdsno"].asIntModestyle();
                        YinHuaInfo* pYinhuaInfo = JsonDataManager::shardJDManager()->GetYinhuaInfoFromID(nYinhuaId);   //取出印花的模版ID
                        if(!pYinhuaInfo)
                        {
                            delete pRoomData;
                            continue;
                        }
                        else
                        {
                            pRoomData->setpYinhuaInfo(pYinhuaInfo);
                            CLSceneManager::sharedSceneManager()->AddPvpRoomInfoToList(pRoomData);
                        }
                    }
                    CLSceneManager::sharedSceneManager()->BeginLoadAdvImages(CMD_PK1_ROOMS_REPLY);
                    break;
                }
                    
                case CMD_PK1_RANKINGLIST_REPLY:  //返回排行榜信息
                {
                    CCLOG("diyal: CMD_PK1_RANKINGLIST_REPLY content:%s", root.toStyledString().c_str());
                    
                    CLPvpSceneLayer* pPvpLayer = (CLPvpSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();//找到对战主节点
                    if(pPvpLayer->getnCTIndex() != CLScene_PvpLayer)
                    {
                        CCLOG("Error: The cur scene is not the main scene!!!");
                        return;
                    }
                    
                    if (pPvpLayer->getbSubReady())
                    { //判断是否备战准备界面
                        
                        CLPvpReady *pReadLayer = (CLPvpReady*)pPvpLayer->getpShowNode();
                        
                        if(pReadLayer && pReadLayer->getpCenterNode())
                        {
                            pReadLayer->getpCenterNode()->removeFromParent();
                            pReadLayer->setpCenterNode(NULL);
                        }
                        
                        RankingList* pLayer = RankingList::create(root);
                        pLayer->setAnchorPoint(CCPointZero);
                        pLayer->setPosition(CCPointZero);
                        
                        pReadLayer->addChild(pLayer);
                        
                        pReadLayer->setpCenterNode(pLayer);
                        pReadLayer->setIndexPosition(PVP_PAIHANG);

                    }

                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                    
   
                    break;
                }
                    
                    //返回信息
                case CMD_PK1_EQUIP_REPLY:
                {
                    CCLOG("wangguolong: CMD_PRIZEINFO_REPLY content:%s", root.toStyledString().c_str());
                    //获取对战选择道具信息
                    
                    CLSceneManager::sharedSceneManager()->AddUcardStatusMap(root["card1"].asIntModestyle(), root["card1"].asIntModestyle());
                    CLSceneManager::sharedSceneManager()->AddUcardStatusMap(root["card2"].asIntModestyle(), root["card2"].asIntModestyle());
                    CLSceneManager::sharedSceneManager()->AddUcardStatusMap(root["card3"].asIntModestyle(), root["card3"].asIntModestyle());
                    CLSceneManager::sharedSceneManager()->AddUpropStatusMap(root["prop1"].asIntModestyle(), root["prop1"].asIntModestyle());
                    CLSceneManager::sharedSceneManager()->AddUpropStatusMap(root["prop2"].asIntModestyle(), root["prop2"].asIntModestyle());
                    CLSceneManager::sharedSceneManager()->AddUroleStatusMap(root["role"].asIntModestyle(), root["role"].asIntModestyle());
                    
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                    
                    break;
                }
                
                    //备战包返回
                case CMD_PK1_BACKPACK_REPLY:
                {
                    CCLOG("wangguolong: CMD_PK1_BACKPACK_REPLY content:%s", root.toStyledString().c_str());
                    
                    //获取对战设置题卡信息
                    CLSceneManager::sharedSceneManager()->ClearUcardList();
                    for(int i=0; i<root["ucard"].size(); i++)
                    {
                        Json::Value& jv = root["ucard"][i];
                        UcardInfo* pUcardData = new UcardInfo();
                        
                        pUcardData->setnUcardNo(jv["No"].asIntModestyle());

                        pUcardData->setnUcardNumber(jv["Number"].asStringModeStyle());
                        
                        CLSceneManager::sharedSceneManager()->AddUcardInfoToList(pUcardData);
                        
                        if(!pUcardData){
                            delete pUcardData;
                        }
                        
                    }
                    
                    
                    //获取对战设置道具信息
                    CLSceneManager::sharedSceneManager()->ClearUpropList();
                    for(int i=0; i<root["uprop"].size(); i++)
                    {
                        Json::Value& jv = root["uprop"][i];
                        UpropInfo* pUpropData = new UpropInfo();
                        
                        pUpropData->setnUpropNo(jv["No"].asIntModestyle());
                        
                        pUpropData->setnUpropNumber(jv["Number"].asStringModeStyle());
                        
                        CLSceneManager::sharedSceneManager()->AddUpropInfoToList(pUpropData);
                        
                        if(!pUpropData){
                            delete pUpropData;
                        }
                    }
                    
                    //获取对战设置角色信息
                    CLSceneManager::sharedSceneManager()->ClearUroleList();
                    for(int i=0; i<root["urole"].size(); i++)
                    {
                        Json::Value& jv = root["urole"][i];
                        UroleInfo* pUroleData = new UroleInfo();
                        
                        pUroleData->setnUroleNo(jv["No"].asIntModestyle());
                        
                        pUroleData->setnUroleNumber(jv["Number"].asStringModeStyle());
                        
                        CLSceneManager::sharedSceneManager()->AddUroleInfoToList(pUroleData);
                        
                        if(!pUroleData){
                            delete pUroleData;
                        }
                    }
                    
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                    
                    break;
                }
                case CMD_PK1_PAIR:  //匹配成功
                {
                    if(CLSceneManager::sharedSceneManager()->getpRunningScene()->getnCTIndex() != CLScene_PvpLayer)
                        break;
                    Json::Value* pData = new Json::Value(root);
                    CLSceneManager::sharedSceneManager()->AddJsonDataToList(pData);
                    CLPvpSceneLayer* pLayer = (CLPvpSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    
                    if(pLayer->getnCTIndex() == CLScene_PvpLayer)  //如果在对战界面
                    {
                        CLPvpSceneLayer* pPvpLayer =(CLPvpSceneLayer*)pLayer;
                        if(pPvpLayer->getbSubReady())
                        {
                            CLPvpReady* pSubLayer = (CLPvpReady*)pPvpLayer->getpShowNode();
                            pSubLayer->WaitRivalSuccess(root);
                        }
                    }
                    break;
                }
                case CMD_PK1_QUESTION:
                {
                    CCLOG("gaoyuanMsgID: CMD_Pk1Answer_REPLY content:%s", root.toStyledString().c_str());
                    CLSceneLayer* pLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    if(pLayer->getnCTIndex() == CLScene_PvpLayer)  //如果在对战界面
                    {
                        CLPvpSceneLayer* pPvpLayer =(CLPvpSceneLayer*)pLayer;
                        if(!pPvpLayer->getbSubReady())
                        {
                            CLPvpingLayer* pSubLayer = (CLPvpingLayer*)pPvpLayer->getpShowNode();
                            pSubLayer->UpdateTopicLayer(root);
                        }
                        else
                        {
                            Json::Value* pData = new Json::Value(root);
                            CLSceneManager::sharedSceneManager()->AddJsonDataToList(pData);
                        }
                    }
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                    break;
                }
                case CMD_PK1_PROP_START:    //对战时候通知客户端可以使用题卡
                {
                    CCLOG("zhangMsgID: PK1_PROP_START content:%s", root.toStyledString().c_str());
                    CLSceneLayer* pLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    if(pLayer->getnCTIndex() == CLScene_PvpLayer)  //如果在对战界面
                    {
                        CLPvpSceneLayer* pPvpLayer =(CLPvpSceneLayer*)pLayer;
                        if(!pPvpLayer->getbSubReady())
                        {
                            CLPvpingLayer* pSubLayer = (CLPvpingLayer*)pPvpLayer->getpShowNode();
                            pSubLayer->CanUsePropCard(root);
                        }
                    }
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                    
                }
                    break;
                case CMD_PK1_SUBMITED:
                {
                    CCLOG("zhangMsgID: CMD_PK1_SUBMITED content:%s", root.toStyledString().c_str());
                    CLSceneLayer* pLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    if(pLayer->getnCTIndex() == CLScene_PvpLayer)  //如果在对战界面
                    {
                        CLPvpSceneLayer* pPvpLayer =(CLPvpSceneLayer*)pLayer;
                        if(!pPvpLayer->getbSubReady())
                        {
                            CLPvpingLayer* pSubLayer = (CLPvpingLayer*)pPvpLayer->getpShowNode();
                            pSubLayer->ShowRivalSelAnswer(root);
                        }
                    }
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                }
                    break;
                case CMD_PLYACHIEVE_REPLY:  //返回成就信息
                {

                    CCLOG("gaoyuan: CMD_PLYACHIEVE_REPLY content:%s", root.toStyledString().c_str());
                    
                    CLSceneManager::sharedSceneManager()->ClearPvpRoomList();
                    
                    CLAchievements* achi = CLAchievements::create(root);
                    achi->setPosition(CCPointZero);
                    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
                    {
                        CCLOG("Error: The cur scene is not the main scene!!!");
                        break;
                    }
                    pMainLayer->addChild(achi, 1, ACHIEVEMENTS);
                    
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                                        
                    break;
                }
                    
                case CMD_GETACHIEVE_REPLY:  //返回领取成功信息
                {
                    //成就菜单下领取回调
                    CCLOG("gaoyuan: 领取成功 content:%s", root.toStyledString().c_str());
                    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
                    {
                        CCLOG("Error: The cur scene is not the main scene!!!");
                        return;
                    }
                    //                    int pAchiNo = root["No"].asIntModestyle();
                    CLAchievements* bl = (CLAchievements*)pMainLayer->getChildByTag(ACHIEVEMENTS);
                    if (bl)
                    {
                        
                        
                        bl->_getSuccessBack(root);
                    }
                    
                    CLAchiReachLayer* achiMainGet = (CLAchiReachLayer*)pMainLayer->getChildByTag(ACHIEVEMENTS + 1);
                    if (achiMainGet)
                    {
                        achiMainGet->_getAchiSucc(root);
                    }
                    
                    //修改用户信息
                    //                    int money = root["jetton"].asIntModestyle();
                    //                    int card = root["moneys"].asIntModestyle();
                    //
                    //                    CLPlayerManager::shardPlayerManager()->getpUserInfo()->setJetton(money); //钱
                    //                    CLPlayerManager::shardPlayerManager()->getpUserInfo()->setMoneys(card);  //购物卡
                    //
                    //                    pMainLayer->getpMoneyFont()->setString(CCString::createWithFormat("%d", CLPlayerManager::shardPlayerManager()->getpUserInfo()->getJetton())->getCString());
                    //
                    //                    pMainLayer->getpCardFont()->setString(CCString::createWithFormat("%d", CLPlayerManager::shardPlayerManager()->getpUserInfo()->getMoneys())->getCString());
                    
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                    
                    break;
                }
                case CMD_USEPROP_REPLY:  //返回领取成功信息
                {
                    
                    CCLOG("zhangqiyun: Pve使用物品成功 content:%s", root.toStyledString().c_str());
                    CLPveSceneLayer* pPveLayer = (CLPveSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    if(pPveLayer->getnCTIndex() != CLScene_PveLayer)
                    {
                        CCLOG("Error: The cur scene is not the Pve scene!!!");
                        return;
                    }
                    pPveLayer->UsePropSucces(root);
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                    
                    break;
                    
                }
                case CMD_PK1_USEPROP_REPLY: //对战使用道具成功
                {
                    CCLOG("zhangMsgID: CMD_PK1_USEPROP content:%s", root.toStyledString().c_str());
                    CLSceneLayer* pLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    if(pLayer->getnCTIndex() == CLScene_PvpLayer)  //如果在对战界面
                    {
                        CLPvpSceneLayer* pPvpLayer =(CLPvpSceneLayer*)pLayer;
                        if(!pPvpLayer->getbSubReady())
                        {
                            CLPvpingLayer* pSubLayer = (CLPvpingLayer*)pPvpLayer->getpShowNode();
                            pSubLayer->UsePropSucces(root);
                        }
                    }
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                }
                    break;
                case CMD_PK1_USECARD_REPLY: //对战使用卡牌成功
                {
                    CCLOG("zhangMsgID: CMD_PK1_USECARD content:%s", root.toStyledString().c_str());
                    CLSceneLayer* pLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    if(pLayer->getnCTIndex() == CLScene_PvpLayer)  //如果在对战界面
                    {
                        CLPvpSceneLayer* pPvpLayer =(CLPvpSceneLayer*)pLayer;
                        if(!pPvpLayer->getbSubReady())
                        {
                            CLPvpingLayer* pSubLayer = (CLPvpingLayer*)pPvpLayer->getpShowNode();
                            pSubLayer->UseCardSucces(root);
                        }
                    }
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                }
                    break;
                case CMD_PK1_PASS:    //对战结束
                {
                    CCLOG("zhangMsgID: CMD_PK1_PASS content:%s", root.toStyledString().c_str());
                    CLSceneLayer* pLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    if(pLayer->getnCTIndex() == CLScene_PvpLayer)  //如果在对战界面
                    {
                        CLPvpSceneLayer* pPvpLayer =(CLPvpSceneLayer*)pLayer;
                        if(!pPvpLayer->getbSubReady())
                        {
                            CLPvpingLayer* pSubLayer = (CLPvpingLayer*)pPvpLayer->getpShowNode();
                            pSubLayer->EndPvping(root);
                        }
                    }
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                }
                    break;
                case CMD_PK1_JOIN_REPLY:  //返回匹配失败
                {
                    CCLOG("zhangMsgID: CMD_PK1_JOIN_REPLY content:%s", root.toStyledString().c_str());
                    CLSceneLayer* pLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    if(pLayer->getnCTIndex() == CLScene_PvpLayer)  //如果在对战界面
                    {
                        CLPvpSceneLayer* pPvpLayer =(CLPvpSceneLayer*)pLayer;
                        if(pPvpLayer->getbSubReady())
                        {
                            CLPvpReady* pReadLayer = (CLPvpReady*)pPvpLayer->getpShowNode();
                            pReadLayer->ShowWaitRivalLayer(false);
                            //添加DLG
                            
                            //......
                        }
                    }
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                }
                    break;
                case CMD_REMOVETIME_REPLY:    //返回消除缓冲时间结果
                {
                    CCLOG("MsgID22: CMD_REMOVETIME_REPLY content:%s", root.toStyledString().c_str());
                    CLSceneLayer* pLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    
                    CLShelfsSceneLayer* pShelfLayer =(CLShelfsSceneLayer*)pLayer;
                    if(pShelfLayer)
                    {
                        pShelfLayer->_CanceSmCool();
                    }
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                }
                    break;
                case CMD_DIRECTPASS_REPLY: //送礼过关
                {
                    CCLOG("MsgID28: CMD_DIRECTPASS_REPLY content:%s", root.toStyledString().c_str());
                    CLSceneLayer* pLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    if(pLayer->getnCTIndex() == CLScene_PveLayer)  //如果在闯关界面
                    {
                        CLPveSceneLayer* pPveLayer =(CLPveSceneLayer*)pLayer;
                        pPveLayer->setbSMStyle(true); //送礼过关的时候，设置变量
                        pPveLayer->_GiftBossResultLayer(root);
                        
                        //记录解锁的店铺
                        CLSceneManager::sharedSceneManager()->setnNolock(root["newlevel"].asIntModestyle());
                    }
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                }
                    break;
                case CMD_ACHIEVE_SEND://服务器主动推送成就
                {
                    int ZARO = 0;
                    if (root.isMember("achno") && root["achno"].size() > 0)
                    {
                        
                        for(int i = 0; i < root["achno"].size(); i++) //头像区域成就No取得
                        {
                            int achiNo;
                            
                            if (i == 0)
                            {
                                achiNo = root["achno"][ZARO].asIntModestyle();
                            }
                            else
                            {
                                achiNo = root["achno"][i].asIntModestyle();
                            }
                            
                            if (achiNo != 0)
                            {
                               CLSceneManager::sharedSceneManager()->AddAchieveInfoToList(achiNo, 0); //答题过程中的成就推送
                            }
                        }
                        
                    }
                    
                    break;
                }
                case CMD_SHOWACHIEVE_REPLY://返回玩家最新的成就信息
                {
                    int ZARO = 0;
                    if (root.isMember("achno") && root["achno"].size() > 0)
                    {
                    
                      for(int i = 0; i < root["achno"].size(); i++) //头像区域成就No取得
                      {
                          int achiNo;
                          if (i == 0)
                          {
                            achiNo = root["achno"][ZARO].asIntModestyle();
                          }
                          else if (i < 3)
                          {
                            achiNo = root["achno"][i].asIntModestyle();
                          }
                          CLSceneManager::sharedSceneManager()->AddUserAchiInfo(achiNo);
                      }
                    
                    }
                    if (root.isMember("nogetno") && root["nogetno"].size() > 0)
                    {
                        
                        for(int i = 0; i < root["nogetno"].size(); i++) //头像区域成就No取得
                        {
                            int achiNo;

                            if (i == 0)
                            {
                                achiNo = root["nogetno"][ZARO].asIntModestyle();
                            }
                            else
                            {
                                achiNo = root["nogetno"][i].asIntModestyle();
                            }
                            CLSceneManager::sharedSceneManager()->AddAchieveInfoToList(achiNo, 1); //登录的时候推送的成就，设置不需要成就领取
                        }
                        
                    }
                    
                    break;
                    
                }
                    
                case CMD_RMBBUYCARD_REPLY: //返回RMB购买购物卡信息
                {
                    CCLOG("gaoyuanMsgID: CMD_RMBBUYCARD_REPLY content:%s", root.toStyledString().c_str());
                    CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    
//                    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
//                    {
//                        CCLOG("Error: The cur scene is not the main scene!!!");
//                        break;
//                    }
                    if(pMainLayer)
                    {
                        BuyMoneyLayer *buyMoney = BuyMoneyLayer::create(root);
                        pMainLayer->addChild(buyMoney,10, RMBBUYCARD);
                        CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                    }

                    break;
                }
                    
                case CMD_UPPLYINFO_REPLY: //修改的信息
                {
                    CCLOG("gaoyuanMsgID: CMD_UPPLYINFO_REPLY content:%s", root.toStyledString().c_str());
                    
                    break;
                }
                    
                default:
                    break;
            }
            
            break;
        }
        
        default:   //错误
        {
            //result 结果不为0时，提示消息，点击确认从新连接
            
            // 有此键表示匹配失败，没有表示缺少印花
            if(root["msgid"].asIntModestyle() == 34 && !root.isMember("pair") && root["result"].asIntModestyle() == 9999)
            {
                CLSceneManager::sharedSceneManager()->setnPair(1);
                TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("StampNotEnough"));
            }else
            {
                CLSceneManager::sharedSceneManager()->setnPair(0);
            }
            
            if(root["msgid"].asIntModestyle() == 34 && root["result"].asIntModestyle() == 9998)
            {
                CLSceneManager::sharedSceneManager()->setnPair(1);
                TiShiLayer::create("对战时间还没有到");
            }

            if(root["result"].asIntModestyle() != 0 && CLSceneManager::sharedSceneManager()->getnPair() !=1 )
            {
                CCNode* node = CLSceneManager::sharedSceneManager()->getpParentNode()->getChildByTag(11111);
                if (!node)
                {
                    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(false);
                    
                    DlgLayerBtnB * dlg = DlgLayerBtnB::createBtn(CLChangeString::sharedChangeString()->GetChangeString("quxiao"),
                                                                 CLChangeString::sharedChangeString()->GetChangeString("queren"),
                                                                 CLChangeString::sharedChangeString()->GetChangeString("SysMessage"));
                    
                    CLSceneManager::sharedSceneManager()->getpParentNode()->addChild(dlg, DLG_GENERAL_ZORDER, 11111);
                    dlg->pBtnRight->setOnClickCallback(callfuncO_selector(CLNetWorkManager::reconnectOk), this);
                }
            }
            
            CCLOG("This is recv data is errorCode:%d!!!!!!!!",root["result"].asIntModestyle() );
            //分消息ID类型分别处理
            switch (root["msgid"].asIntModestyle())
            {
                case CMD_PK1_JOIN_REPLY:
                {
                    if(CLSceneManager::sharedSceneManager()->getpRunningScene()->getnCTIndex() != CLScene_PvpLayer)
                        break;
                    CLPvpSceneLayer* pLayer = (CLPvpSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
                    if(pLayer->getbSubReady())
                    {
                        CLPvpReady* pReadLayer = (CLPvpReady*)pLayer->getpShowNode();
                        pReadLayer->ShowWaitRivalLayer(false);
                        CCLOG(" Has no Rival......");
                        
                        // 下面弹出界面框（匹配不成功）
                    }
                    
                }
                    break;
                case CMD_PK1_PAIR:  //匹配失败
                {
                    CCLOG(" Pair Failed......");
                }
                    break;
            }
            break;
        }
            
    }
    
    m_pClientSocket->removeRecvCommand();
    
}

void CLNetWorkManager::removeDlgA(CCNode* r)
{
    r->removeFromParentAndCleanup(true);
}

long CLNetWorkManager::getCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}








