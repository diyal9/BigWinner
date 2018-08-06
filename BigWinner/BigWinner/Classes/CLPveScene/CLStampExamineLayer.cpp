//
//  CLStampExamineLayer.cpp
//  BigWinner
//
//  Created by LunaBox on 13-12-22.
//
//

#include "CLStampExamineLayer.h"

#include "LGameCommonDefine.h"
#include "Button.h"
#include "JsonDataManager.h"
#include "CLSceneManager.h"
#include "CLChangeString.h"
#include "CLMainSceneLayer.h"
#include "CLPvpSceneLayer.h"
#include "CLNetWorkManager.h"

USING_NS_CC_EXT;

using std::string;
using namespace cocos2d;

using namespace Json;

/**************************************************
 * 印花详情UI
 ***************************************************/
CLStampExamineLayer* CLStampExamineLayer::create(int nMeyh1,int nMeyh2)
{
    
    Json::Value* jv = CLSceneManager::sharedSceneManager()->GetJsonDataFromMsgID(CMD_PRIZEINFO_REPLY);
    
    CLStampExamineLayer *pRet = new CLStampExamineLayer();
	if (pRet && pRet->init(*jv,nMeyh1,nMeyh2))
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

bool CLStampExamineLayer::init(Json::Value &data,int nMeyh1,int nMeyh2)
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::initWithColor(ccc4(0,0,0,0)) )
    {
        return false;
    }
    
    getMessageInfo(data,nMeyh1,nMeyh2);
    
    return true;
    
}

void CLStampExamineLayer::getMessageInfo(Json::Value &data,int nMeyh1,int nMeyh2)
{

    int yhNo = 0;//印花商品No
    int yhGoldMe = 0;//当前拥有的金印花
    int yhSilverMe = 0;//当前拥有的银印花
    std::string yhPName;//印花名称  36号字体 51 51 51
    int yhGold = 0; //兑换商品需要的金印花
    int yhSilver = 0;//兑换商品需要的银印花
    int gNumber = 0;//当前金印花可以兑换的个数
    int sNumber = 0;//当前银印花可以兑换的个数
    std::string description;//商品说明
    std::string goldMe;
    std::string silverMe;
    std::string gold;
    std::string silver;
    std::string m_BuyName;//卖家名字
    //印花商品No
    yhNo = data["No"].asIntModestyle();
    
    //当前拥有的金印花
    yhGoldMe = nMeyh2;
    //当前拥有的银印花
    yhSilverMe = nMeyh1;
    
    //印花名称
    yhPName = JsonDataManager::shardJDManager()->m_yinHua[yhNo]->m_Name;
    //兑换商品需要的金印花
    yhGold = JsonDataManager::shardJDManager()->m_yinHua[yhNo]->m_YH1;
    //兑换商品需要的银印花
    yhSilver = JsonDataManager::shardJDManager()->m_yinHua[yhNo]->m_YH2;
    
    //商家名称
    m_BuyName= JsonDataManager::shardJDManager()->m_yinHua[yhNo]->m_BuyName;
    
    //商品说明
    description = JsonDataManager::shardJDManager()->m_yinHua[yhNo]->m_Description;
    
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();

    //印花
    CCSprite* yh = CCSprite::create("Image/BackPack/BKShop2.png");
    this->addChild(yh);
    yh->setPosition(ccp(size.width*0.5, size.height*0.43));

    //添加背景图片
    CCSprite *pSprite = CCSprite::create("Image/BackPack/PrizeDetailsBG.png");
    pSprite->setPosition(ccp(yh->getContentSize().width*0.5,G_SF(435)));
    yh->addChild(pSprite);
    
    //关闭按钮
    Button *closeBtn = Button::createBtn("Image/PublicUI/Close.png");
    closeBtn->setPosition(ccp(G_SF(535), G_SF(640)));
    closeBtn->setOnClickCallbackNode(callfuncN_selector(CLStampExamineLayer::closeBtn), this);
    closeBtn->setHandlerPriority(-20000);
    pSprite->addChild(closeBtn,2);
    
    //奖品详情
    CCSprite *PrizeDetails = CCSprite::create("Image/PublicUI/TitleBackground.png");
    PrizeDetails->setPosition(ccp(G_SF(55), G_SF(640)));
    pSprite->addChild(PrizeDetails);
    
    //标题 奖品详情
    CCSprite* PrizeStr = CCSprite::create("Image/BackPack/PrizeDetails.png");
    PrizeStr->setPosition(ccp(G_SF(80), G_SF(30)));
    PrizeDetails->addChild(PrizeStr);
    
    //商品图片路径
    std::string stampSmall_path = CCString::createWithFormat("Image/GoodIcon/StampSmall%d.png", yhNo)->getCString();
    
    //商品图片
    CCSprite *pStamp = CCSprite::create(stampSmall_path.c_str());
    pStamp->setPosition(ccp(G_SF(80), G_SF(525)));
    pSprite->addChild(pStamp);
    
    //商品名称
    CCLabelTTF* nameLabel = CCLabelTTF::create(yhPName.c_str(), "Arial", G_SF(36), CCSizeMake(G_SF(360), G_SF(50)), kCCTextAlignmentLeft);
    nameLabel->setColor(ccc3(51, 51, 51));
    nameLabel->setPosition(ccp(G_SF(352), G_SF(550)));
    pSprite->addChild(nameLabel);
    
    //当前金印花可以兑换的个数
    gNumber = yhGoldMe/yhGold;
    //当前银印花可以兑换的个数
    sNumber = yhSilverMe/yhSilver;
    
    //转换用
    char CGold[256];
    sprintf(CGold, "%d", yhGoldMe);
    //拥有的金印花数目
    goldMe = CGold;
    
    sprintf(CGold, "%d", yhSilverMe);
    //拥有的银印花数目
    silverMe = CGold;
    
    //转换用
    char CGold2[256];
    sprintf(CGold2, "%d", yhGold);
    //所需金印花数目
    gold = CGold2;
    
    std::string strGold = "/";
    
    std::string goldName = CLChangeString::sharedChangeString()->GetChangeString("GoldStamp");
    
    //金印花
    CCLabelTTF* pJin = CCLabelTTF::create(goldName.append(goldMe).append(strGold).append(gold).c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(126), G_SF(25)), kCCTextAlignmentLeft);
    pJin->setColor(ccc3(102, 102, 102));
    pJin->setPosition(ccp(G_SF(235), G_SF(510)));
    pSprite->addChild(pJin);
    
    //转换用
    char CSilver[256];
    sprintf(CSilver, "%d", yhSilver);
    silver = CSilver;
    std::string strSilver = "/";
    std::string silverName = CLChangeString::sharedChangeString()->GetChangeString("SilverStamp");
    
    //银印花
    CCLabelTTF* pYin= CCLabelTTF::create(silverName.append(silverMe).append(strSilver).append(silver).c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(126), G_SF(25)), kCCTextAlignmentLeft);
    pYin->setColor(ccc3(102, 102, 102));
    pYin->setPosition(ccp(G_SF(360), G_SF(510)));
    pSprite->addChild(pYin);
    
    
    //当前查看印花属于哪个货架
    std::string hjName ;
    //银印花收集地点
    std::string strS = CLChangeString::sharedChangeString()->GetChangeString("SilverStampPlace");
    int shelfNo = 0;
    int zero = 0;
    //印花收集点存在的情况
    if(data["shelfNo"].size() > 0)
    {
        shelfNo = data["shelfNo"][zero].asIntModestyle();
        hjName = JsonDataManager::shardJDManager()->m_Shelf[shelfNo]->getnShelfCaption();
        
    }
    
    //银印花收集点
    CCLabelTTF* pSilver= CCLabelTTF::create(strS.append(hjName).c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(300), G_SF(25)), kCCTextAlignmentLeft);
    pSilver->setColor(ccc3(102, 102, 102));
    pSilver->setPosition(ccp(G_SF(323), G_SF(480)));
    pSprite->addChild(pSilver);
    
    //...............................
    
    //创建一个scrollView
    CCScrollView *scrollView=CCScrollView::create();
    scrollView->setPosition(ccp(G_SF(20), G_SF(110)));
    scrollView->setTouchPriority(-20001);//20131020优先级设置
    scrollView->setContentSize(CCSizeMake(G_SF(530), G_SF(350))); //设置容器大小（内容尺寸）
    //显示的区域
    scrollView->setViewSize(CCSizeMake(G_SF(530), G_SF(300)));//设置view的大小
    
    //实现触摸消息
    scrollView->setTouchEnabled(true);
    scrollView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向
    scrollView->setDelegate(this);
    
    pSprite->addChild(scrollView);
    
    //...............................
    
    //商品说明
    std::string descriptionStr = CLChangeString::sharedChangeString()->GetChangeString("PrizeExplain");
    std::string lenStr;
    
    lenStr = descriptionStr.append(description);
    
    float len=strlen(lenStr.c_str());
    
    int nlen = ceil(((len * 18) /500)/3);
    
    int lenY = nlen * 30;
    
    CCLabelTTF* pDescription= CCLabelTTF::create(lenStr.c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(500), G_SF(lenY)), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    pDescription->setColor(ccc3(102, 102, 102));
    pDescription->setPosition(ccp(G_SF(20), G_SF(240-25*nlen) ));
    scrollView->addChild(pDescription);
    
    int lenC = 0;
    if(nlen == 1){
        lenC = 22;
    }else if(nlen ==2)
    {
        lenC = 10+nlen*11;
    }else{
        lenC =nlen*11;
    }
//    //赞助商
//    std::string buyName = CLChangeString::sharedChangeString()->GetChangeString("Sponsor");
//    CCLabelTTF* pBuyName= CCLabelTTF::create(buyName.append(m_BuyName).c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(300), G_SF(25)), kCCTextAlignmentLeft);
//    pBuyName->setColor(ccc3(102, 102, 102));
//    pBuyName->setPosition(ccp(G_SF(20), G_SF(290-20*nlen-40)));
//    scrollView->addChild(pBuyName);
//    
//    //兑换终端
//    std::string exchangeName = CLChangeString::sharedChangeString()->GetChangeString("ExchangeTerminal");
//    //兑换终端网址
//    std::string exchangSite;
//    exchangSite = data["site"].asStringModeStyle();
//    
//    //兑换终端
//    CCLabelTTF* pExchangeName= CCLabelTTF::create(exchangeName.append(exchangSite).c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(300), G_SF(25)), kCCTextAlignmentLeft);
//    pExchangeName->setColor(ccc3(102, 102, 102));
//    pExchangeName->setPosition(ccp(G_SF(20), G_SF(290-20*nlen - 75)));
//    scrollView->addChild(pExchangeName);
//    
//    //兑换历史
//    CCLabelTTF* pHistory= CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("ExchangeHistory").c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(80), G_SF(25)), kCCTextAlignmentLeft);
//    pHistory->setColor(ccc3(102, 102, 102));
//    pHistory->setPosition(ccp(G_SF(20), G_SF(255-lenY/2 - lenC-50)));
//    scrollView->addChild(pHistory);
    
    //赞助商
    std::string buyName = CLChangeString::sharedChangeString()->GetChangeString("Sponsor");
    CCLabelTTF* pBuyName= CCLabelTTF::create(buyName.append(m_BuyName).c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(300), G_SF(25)), kCCTextAlignmentLeft);
    pBuyName->setColor(ccc3(102, 102, 102));
    pBuyName->setPosition(ccp(G_SF(20), G_SF(225-20*nlen-40)));
    scrollView->addChild(pBuyName);
    
    //    //兑换终端
    //    std::string exchangeName = CLChangeString::sharedChangeString()->GetChangeString("ExchangeTerminal");
    //    //兑换终端网址
    //    std::string exchangSite;
    //    exchangSite = data["site"].asStringModeStyle();
    
    //    //兑换终端
    //    CCLabelTTF* pExchangeName= CCLabelTTF::create(exchangeName.append(exchangSite).c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(300), G_SF(25)), kCCTextAlignmentLeft);
    //    pExchangeName->setColor(ccc3(102, 102, 102));
    //    pExchangeName->setPosition(ccp(G_SF(20), G_SF(290-20*nlen - 75)));
    //    scrollView->addChild(pExchangeName);
    
    std::string exchangeName = "声明：本游戏内测期间，由于后台兑换系统尚在完善中，您所兑换的商品将顺延至系统完善后承兑。请保存好您的兑换码，我们会在开启兑换后通知您凭码兑换。";
    CCLabelTTF* pExchangeName= CCLabelTTF::create(exchangeName.c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(500), G_SF(80)), kCCTextAlignmentLeft);
    //    pExchangeName->setColor(ccc3(102, 102, 102));
    pExchangeName->setColor(ccRED);
    pExchangeName->setPosition(ccp(G_SF(20), G_SF(205-20*nlen - 120)));
    scrollView->addChild(pExchangeName);
    
    //兑换历史
    CCLabelTTF* pHistory= CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("ExchangeHistory").c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(80), G_SF(25)), kCCTextAlignmentLeft);
    pHistory->setColor(ccc3(102, 102, 102));
    pHistory->setPosition(ccp(G_SF(20), G_SF(165-lenY/2 - lenC-95)));
    scrollView->addChild(pHistory);
    
    //顺序为"印花商品兑换码","兑换时间","兑换状态 0：已兑换；1：已使用","使用兑换地点序号"
    int ZERO = 0;
    //商品兑换码
    std::string exchangeCode1;
    std::string exchangeCode2;
    std::string exchangeCode3;
    
    //兑换年月日
    std::string exchangeYMD1;
    std::string exchangeYMD2;
    std::string exchangeYMD3;
    
    //兑换状态
    int exchangeStatus1;
    int exchangeStatus2;
    int exchangeStatus3;
    
    
    //存在兑换历史
    if(data["history"].size() >0){
        
        CCLog("data %i",data["history"].size());
        
        //存在第一条兑换历史
        if(data["history"][ZERO].size()>0){
            
            //日兑换成功
            std::string exchangeSuccess = CLChangeString::sharedChangeString()->GetChangeString("ExchangeSuccess");
            std::string str = "/";
            std::string exchangeStatus;
            
            //兑换码
            exchangeCode1 = data["history"][ZERO][ZERO].asStringModeStyle();
            //兑换年月日
            exchangeYMD1 = data["history"][ZERO][1].asStringModeStyle();
            //兑换状态
            exchangeStatus1 = data["history"][ZERO][2].asIntModestyle();
            //兑换码文字
            std::string exchangeCode = CLChangeString::sharedChangeString()->GetChangeString("ConversionCode");
            
            
            //兑换码
            CCLabelTTF* pExchangeCode= CCLabelTTF::create(exchangeCode.append(exchangeCode1).c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(300), G_SF(25)), kCCTextAlignmentLeft);
            pExchangeCode->setColor(ccc3(102, 102, 102));
            pExchangeCode->setPosition(ccp(G_SF(20), G_SF(255-lenY/2 - lenC-75)));
            scrollView->addChild(pExchangeCode);
            
            if(exchangeStatus1 == 0)
            {
                //以兑换文字
                exchangeStatus = CLChangeString::sharedChangeString()->GetChangeString("ToConvert");
            }else if(exchangeStatus1 == 1)
            {
                //以使用文字
                exchangeStatus = CLChangeString::sharedChangeString()->GetChangeString("ToUse");
            }
            
            std::string strYMD1;
            //商品名称处理
            if(exchangeYMD1.length() > 0){
                strYMD1 = exchangeYMD1.replace(4,1, CLChangeString::sharedChangeString()->GetChangeString("Year"));
                strYMD1 = strYMD1.replace(9, 1, CLChangeString::sharedChangeString()->GetChangeString("Month"));
            }
            
            //兑换年月日和状态
            CCLabelTTF* pExchangeSuccess= CCLabelTTF::create(strYMD1.append(exchangeSuccess).append(str).append(exchangeStatus).c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(300), G_SF(25)), kCCTextAlignmentLeft);
            pExchangeSuccess->setColor(ccc3(102, 102, 102));
            pExchangeSuccess->setPosition(ccp(G_SF(20), G_SF(255-lenY/2 - lenC-100)));
            scrollView->addChild(pExchangeSuccess);
            
        }
        //存在第二条兑换历史
        if(data["history"][1].size() >0)
        {
            //日兑换成功
            std::string exchangeSuccess = CLChangeString::sharedChangeString()->GetChangeString("ExchangeSuccess");
            std::string str = "/";
            std::string exchangeStatus;
            
            //兑换码
            exchangeCode2 = data["history"][1][ZERO].asStringModeStyle();
            //兑换年月日
            exchangeYMD2 = data["history"][1][1].asStringModeStyle();
            //兑换状态
            exchangeStatus2 = data["history"][1][2].asIntModestyle();
            //兑换码文字
            std::string exchangeCode = CLChangeString::sharedChangeString()->GetChangeString("ConversionCode");
            
            //兑换码
            CCLabelTTF* pExchangeCode= CCLabelTTF::create(exchangeCode.append(exchangeCode2).c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(300), G_SF(25)), kCCTextAlignmentLeft);
            pExchangeCode->setColor(ccc3(102, 102, 102));
            pExchangeCode->setPosition(ccp(G_SF(20), G_SF(255-lenY/2 - lenC-125)));
            scrollView->addChild(pExchangeCode);
            
            if(exchangeStatus2 == 0)
            {
                //以兑换文字
                exchangeStatus = CLChangeString::sharedChangeString()->GetChangeString("ToConvert");
            }else if(exchangeStatus2 == 1)
            {
                //以使用文字
                exchangeStatus = CLChangeString::sharedChangeString()->GetChangeString("ToUse");
            }
            
            std::string strYMD2;
            //商品名称处理
            if(exchangeYMD1.length() > 0){
                strYMD2 = exchangeYMD2.replace(4,1, CLChangeString::sharedChangeString()->GetChangeString("Year"));
                strYMD2 = strYMD2.replace(9, 1, CLChangeString::sharedChangeString()->GetChangeString("Month"));
            }
            
            //兑换年月日和状态
            CCLabelTTF* pExchangeSuccess= CCLabelTTF::create(strYMD2.append(exchangeSuccess).append(str).append(exchangeStatus).c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(300), G_SF(25)), kCCTextAlignmentLeft);
            pExchangeSuccess->setColor(ccc3(102, 102, 102));
            pExchangeSuccess->setPosition(ccp(G_SF(20), G_SF(255-lenY/2 - lenC-150)));
            scrollView->addChild(pExchangeSuccess);
            
            
        }
        //存在第三条兑换历史
        if(data["history"][2].size() >0)
        {
            //日兑换成功
            std::string exchangeSuccess = CLChangeString::sharedChangeString()->GetChangeString("ExchangeSuccess");
            std::string str = "/";
            std::string exchangeStatus;
            
            //兑换码
            exchangeCode3 = data["history"][2][ZERO].asStringModeStyle();
            //兑换年月日
            exchangeYMD3 = data["history"][2][1].asStringModeStyle();
            //兑换状态
            exchangeStatus3 = data["history"][2][2].asIntModestyle();
            //兑换码文字
            std::string exchangeCode = CLChangeString::sharedChangeString()->GetChangeString("ConversionCode");
            
            //兑换码
            CCLabelTTF* pExchangeCode= CCLabelTTF::create(exchangeCode.append(exchangeCode3).c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(300), G_SF(25)), kCCTextAlignmentLeft);
            pExchangeCode->setColor(ccc3(102, 102, 102));
            pExchangeCode->setPosition(ccp(G_SF(20), G_SF(255-lenY/2 - lenC-175)));
            scrollView->addChild(pExchangeCode);
            
            if(exchangeStatus3 == 0)
            {
                //以兑换文字
                exchangeStatus = CLChangeString::sharedChangeString()->GetChangeString("ToConvert");
            }else if(exchangeStatus3 == 1)
            {
                //以使用文字
                exchangeStatus = CLChangeString::sharedChangeString()->GetChangeString("ToUse");
            }
            
            std::string strYMD3;
            //商品名称处理
            if(exchangeYMD1.length() > 0){
                strYMD3 = exchangeYMD3.replace(4,1, CLChangeString::sharedChangeString()->GetChangeString("Year"));
                strYMD3 = strYMD3.replace(9, 1, CLChangeString::sharedChangeString()->GetChangeString("Month"));
            }
            
            //兑换年月日和状态
            CCLabelTTF* pExchangeSuccess= CCLabelTTF::create(strYMD3.append(exchangeSuccess).append(str).append(exchangeStatus).c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(300), G_SF(25)), kCCTextAlignmentLeft);
            pExchangeSuccess->setColor(ccc3(102, 102, 102));
            pExchangeSuccess->setPosition(ccp(G_SF(20), G_SF(255-lenY/2 - lenC-200)));
            scrollView->addChild(pExchangeSuccess);
        }
        
    }
    
    //银印花赚取按钮
    Button *silverBtn = Button::createBtn("Image/BackPack/StampEarnBtn.png");
    silverBtn->setPosition(ccp(G_SF(55) + silverBtn->getContentSize().width*0.5, G_SF(-55)));
    //跳转到主场景
    silverBtn->setOnClickCallbackNode(callfuncN_selector(CLStampExamineLayer::JumpMainscene), this);
    silverBtn->setHandlerPriority(-20000);
    pSprite->addChild(silverBtn);
    
    //按钮文字：
    CCLabelTTF* labelSilver = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("FindSilverStamp").c_str(), "Arial", G_SF(30), CCSizeMake(G_SF(150), G_SF(40)), kCCTextAlignmentLeft);
    labelSilver->setPosition(ccp(G_SF(0), G_SF(0)));
    silverBtn->addChild(labelSilver,1);
    
    //金印花赚取按钮
    Button *goldBtn = Button::createBtn("Image/BackPack/StampEarnBtn.png");
    goldBtn->setPosition(ccp(G_SF(305) + goldBtn->getContentSize().width*0.5 , G_SF(-55)));
    //跳转到兑换页面
    goldBtn->setOnClickCallbackNode(callfuncN_selector(CLStampExamineLayer::JumpFightCene), this);
    goldBtn->setHandlerPriority(-20000);
    pSprite->addChild(goldBtn);
    
    //按钮文字
    CCLabelTTF* labelGold = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("FindGoldStamp").c_str(), "Arial", G_SF(30), CCSizeMake(G_SF(150), G_SF(40)), kCCTextAlignmentLeft);
    labelGold->setPosition(ccp(G_SF(0), G_SF(0)));
    goldBtn->addChild(labelGold,1);
}


//跳转到主页面
void CLStampExamineLayer::JumpMainscene()
{
    CLSceneManager::sharedSceneManager()->setpCallTarget(this);
    
    CLSceneManager::sharedSceneManager()->getpLoadingLayer()->setVisible(true);
    CLSceneManager::sharedSceneManager()->BeginLoadAdvImages(CMD_LOGIN_REPLY);
    
    CLMainSceneLayer* pLayer = CLMainSceneLayer::create();
    pLayer->setAnchorPoint(CCPointZero);
    pLayer->setPosition(CCPointZero);
    CLSceneManager::sharedSceneManager()->ChangeScene(pLayer);
}

//跳转到对战
void CLStampExamineLayer::JumpFightCene()
{
    CLSceneManager::sharedSceneManager()->setpCallTarget(this);
    CLSceneManager::sharedSceneManager()->setpCallFun(callfunc_selector(CLMainSceneLayer::AdvImageLoadEnd));
    CLSceneManager::sharedSceneManager()->getpLoadingLayer()->setVisible(true);
    
    Json::Value value;
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_PK1_ROOMS, value);
    
    CLPvpSceneLayer* pLayer = CLPvpSceneLayer::create();
    pLayer->setAnchorPoint(CCPointZero);
    pLayer->setPosition(CCPointZero);
    CLSceneManager::sharedSceneManager()->ChangeScene(pLayer);
}

void CLStampExamineLayer::closeBtn()
{
    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);
}

void CLStampExamineLayer::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -19995, true);
}

void CLStampExamineLayer::onExit()
{

    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool CLStampExamineLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}


void CLStampExamineLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CLStampExamineLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}