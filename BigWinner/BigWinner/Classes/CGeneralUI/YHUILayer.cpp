//
//  YHUILayer.cpp
//  BigWinner
//
//  Created by LunaBox03 on 13-11-3.
//
//

#include "YHUILayer.h"

#include "Button.h"
#include "LGameCommonDefine.h"

#include "YHtopLayer.h"
#include "CLSceneManager.h"
#include "CLMainSceneLayer.h"
#include "JsonDataManager.h"
#include "MLTextFieldTTFLayer.h"
#include "RichLabel.h"
#include "DlgTiShiLayer.h"
#include "CLNetWorkManager.h"
#include "CLPlayerManager.h"
#include "CLSceneManager.h"
#include "CLChangeString.h"
#include "SystemInformationLayer.h"
#include "CLPvpSceneLayer.h"
#include "CLSoundManager.h"

USING_NS_CC_EXT;

using std::string;
using namespace cocos2d;

PrizeShow* PrizeShow::createPShow()
{
    
    PrizeShow *pRet = new PrizeShow();
	if (pRet && pRet->initShow())
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

bool PrizeShow::initShow()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::init() )
    {
        return false;
    }
    
    //    //印花数据
    //    m_YHData = jv;
    
    getPipeInfo();
    
    return true;
}

void PrizeShow::getPipeInfo()
{
    
    //添加背景图片
    pLayer2 = CCSprite::create("Image/BackPack/StampBackground2.png");
    
    //奖品展柜
    CCSprite *PrizeShow = CCSprite::create("Image/PublicUI/TitleBackground.png");
    PrizeShow->setPosition(ccp(G_SF(55), G_SF(735)));
    pLayer2->addChild(PrizeShow);
    
    //标题 奖品展柜文字
    CCSprite* PrizeStr = CCSprite::create("Image/BackPack/PrizeShow.png");
    PrizeStr->setPosition(ccp(G_SF(80), G_SF(30)));
    PrizeShow->addChild(PrizeStr);
    
    //关闭
    Button *shutdown = Button::createBtn("Image/PublicUI/Close.png");
    shutdown->setPosition(ccp(G_SF(535), G_SF(735)));
//    shutdown->setOnClickCallbackNode(callfuncN_selector(PrizeShow::menuCallBackButton), this);
    shutdown->setOnClickCallbackNode(callfuncN_selector(PrizeShow::doAction), this);
    shutdown->setHandlerPriority(-10001);
    pLayer2->addChild(shutdown);
    
    //消息
    SystemInformationLayer* _layer=SystemInformationLayer::create();
    _layer->setPosition(ccp(G_SF(150), G_SF(600)));
    pLayer2->addChild(_layer);
    
    tableView = CCTableView::create(this,getTableSize());
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setPosition(G_SF(0),G_SF(7));
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    tableView->setTouchPriority(-10001);//优先级设置
    
    tableView->setDelegate(this);
    
    pLayer2->addChild(tableView);
    pLayer2->setPosition(ccp(G_SF(370),G_SF(335)));
    this->addChild(pLayer2);
    
	tableView->reloadData();
    
}

//动作执行
void PrizeShow::doAction(CCNode* r)
{
    //窗体开闭音乐
    CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_WInClose.wav");

        CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
        
        CCFiniteTimeAction *actionIn = CCSequence::create(CCFadeTo::create(0.4f, 0), NULL); //透明度动画
        if(pMainLayer)
        {
            pMainLayer->getChildByTag(500)->runAction(actionIn);
        }
        
        //STAMPTAG
        CCSize size = CCDirector::sharedDirector()->getgSceneSize();
        CCAction *actionMoveIn = CCEaseExponentialIn::create(CCMoveTo::create(0.4f, ccp(pMainLayer->getChildByTag(STAMPTAG)->getPositionX(), size.height * (-1)))); //慢到快

        pMainLayer->getChildByTag(STAMPTAG)->runAction(actionMoveIn);
        if(pMainLayer)
        {
            pMainLayer->getChildByTag(500)->removeFromParentAndCleanup(true);
        }
    
    menuCallBackButton(r);
//    pMainLayer->getChildByTag(STAMPTAG)->scheduleOnce(schedule_selector(PrizeShow::menuCallBackButton), 0.8f);

    
}

/*
 设置TableView的大小
 */
CCSize PrizeShow::getTableSize()
{
    //    return CCSizeMake(pLayer2->getContentSize().width,pLayer2->getContentSize().height);
    return CCSizeMake(pLayer2->getContentSize().width,pLayer2->getContentSize().height-G_SF(12));
}

/*
 设置TableView题卡一行的坐标
 */
CCSize PrizeShow::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(G_SF(230), getTableSize().height/4.5);
    
}

void PrizeShow::createTableViewCell(CCNode *cell,CCTableView *table,int index)
{
    //顺序为"印花商品No","印花商品标识；0：普通；1：最新；2：最热","当前拥有的金印花","当前拥有的银印花"
    int tag = 0;
    int yhNo = 0;//印花商品No
    int yhMark = 0;//印花商品标识；0：普通；1：最新；2：最热
    int yhGoldMe = 0;//当前拥有的金印花
    int yhSilverMe = 0;//当前拥有的银印花
    std::string yhPName;//印花名称
    int yhGold = 0; //兑换商品需要的金印花
    int yhSilver = 0;//兑换商品需要的银印花
    int gNumber = 0;//当前金印花可以兑换的个数
    int sNumber = 0;//当前银印花可以兑换的个数
    std::string goldMe;
    std::string silverMe;
    std::string gold;
    std::string silver;
    //当前拥有的库存量
    int StockNum = 0;
    
    //卡牌个数
    int nCardNumber = CLSceneManager::sharedSceneManager()->GetPrizeNum();
    
    //题卡数目为零时
    if(nCardNumber == 0)
    {
        return;
    }
    tag = index;
    //印花商品No
    yhNo = CLSceneManager::sharedSceneManager()->GetPrizeInfoFromIndex(tag)->getnStampNo();
    //印花商品标识；0：普通；1：最新；2：最热
    yhMark = CLSceneManager::sharedSceneManager()->GetPrizeInfoFromIndex(tag)->getnStampMark();
    //当前拥有的金印花
    yhGoldMe = CLSceneManager::sharedSceneManager()->GetPrizeInfoFromIndex(tag)->getnHaveGoldStampNum();
    //当前拥有的银印花
    yhSilverMe = CLSceneManager::sharedSceneManager()->GetPrizeInfoFromIndex(tag)->getnHaveSilverStampNum();
    //当前拥有的库存量
    StockNum = CLSceneManager::sharedSceneManager()->GetPrizeInfoFromIndex(tag)->getnStockNum();
    
    //印花名称
    yhPName = JsonDataManager::shardJDManager()->m_yinHua[yhNo]->m_Name;
    //兑换商品需要的金印花
    yhGold = JsonDataManager::shardJDManager()->m_yinHua[yhNo]->m_YH1;
    //兑换商品需要的银印花
    yhSilver = JsonDataManager::shardJDManager()->m_yinHua[yhNo]->m_YH2;
    
    CCLog("印花No: %i  印花名称: %s",yhNo,yhPName.c_str());
    CCLog("yhGoldMe/yhGold : %i,%i",yhGoldMe,yhGold);
    
    //库存量大于零
    if(StockNum > 0){
        //商品背景图片
        CCSprite *bj = CCSprite::create("Image/BackPack/PrizePictureBG.png");
        bj->setPosition(ccp(G_SF(285), G_SF(65)));
        cell->addChild(bj,0,yhNo);
        
        //商品图片路径
        std::string stamp_path = CCString::createWithFormat("Image/GoodIcon/Stamp%d.png", yhNo)->getCString();
        
        CCSprite *spTupian= CCSprite::create(stamp_path.c_str());
        spTupian->setPosition(ccp(G_SF(210), G_SF(60)));
        bj->addChild(spTupian);
        
        //最热商品
        if(yhMark == 2)
        {
            CCSprite *pHot = CCSprite::create("Image/BackPack/hot.png");
            pHot->setPosition(ccp(G_SF(30), G_SF(70)));
            spTupian->addChild(pHot,1);
        }
        //最新
        else if(yhMark == 1)
        {
            CCSprite *pHot = CCSprite::create("Image/BackPack/new.png");
            pHot->setPosition(ccp(G_SF(30), G_SF(70)));
            spTupian->addChild(pHot,1);
        }
        //    else
        //    {
        //        CCLog("等待其他类型的商品加入");
        //    }
        
        //固定文字
        std::string name = "...";
        //商品名称处理
        if(yhPName.length() > 18){
            yhPName = yhPName.substr(0,15).append(name);
        }
        
        //商品民称
        CCLabelTTF* mingcheng = CCLabelTTF::create(yhPName.c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(126), G_SF(30)), kCCTextAlignmentLeft);
        mingcheng->setColor(ccc3(51, 51, 51));
        mingcheng->setPosition(ccp(G_SF(490), G_SF(95)));
        bj->addChild(mingcheng);
        
        /*如果金银印花不够，显示现有的金印花个数和需要的金印花个数，现有的银印花个数和需要的银印花个数和查看按钮。否则就显示去兑换按钮*/
        //当前金印花可以兑换的个数
        gNumber = yhGoldMe/yhGold;
        //当前银印花可以兑换的个数
        sNumber = yhSilverMe/yhSilver;
        //金印花和银印花 个数可以兑换一个商品以上
        if(gNumber > 0 && sNumber > 0){
            //兑换
            Button *dHBtn = Button::createBtn("Image/BackPack/ExchangeBtn.png");
            dHBtn->setPosition(ccp(G_SF(485), G_SF(30)));
            //跳转到兑换页面
            dHBtn->setOnClickCallbackNode(callfuncN_selector(PrizeShow::openExchangeUI), this);
            bj->addChild(dHBtn,1,tag);
            
            //去兑换文字
            CCLabelTTF* exchang = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("ToExchange").c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(54), G_SF(30)), kCCTextAlignmentLeft);
            exchang->setColor(ccc3(255, 255, 255));
            exchang->setPosition(ccp(G_SF(10), G_SF(-5)));
            dHBtn->addChild(exchang,50);
            
            //可兑换商品数目，兑换页面用
            if(gNumber>sNumber){
                
                exchangMap[tag] = sNumber;
            }else{
                exchangMap[tag] = gNumber;
            }
            
        }else
        {
            
            if((gNumber < 1 && sNumber <1) || (gNumber < 1 && sNumber >=1))
            {
                //金印花：
                CCLabelTTF* exchang = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("GoldStr").c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(72), G_SF(30)), kCCTextAlignmentLeft);
                exchang->setColor(ccc3(51, 51, 51));
                exchang->setPosition(ccp(G_SF(465), G_SF(65)));
                bj->addChild(exchang,50);
                
                //转换用
                char t[256];
                sprintf(t, "%d", yhGold);
                gold = t;
                
                std::string str = "/";
                
                //拥有的金印花数量
                sprintf(t,  "%d", yhGoldMe);
                goldMe = t;
                
                //金印花
                CCLabelTTF* pJin = CCLabelTTF::create(goldMe.append(str).append(gold).c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(90), G_SF(20)), kCCTextAlignmentLeft);
                pJin->setColor(ccc3(255, 150, 0));
                pJin->setPosition(ccp(G_SF(545), G_SF(70)));
                bj->addChild(pJin);
                
            }
            if(gNumber >= 1 && sNumber < 1)
            {
                //银印花：
                CCLabelTTF* exchang = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("SilverStr").c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(72), G_SF(30)), kCCTextAlignmentLeft);
                exchang->setColor(ccc3(51, 51, 51));
                exchang->setPosition(ccp(G_SF(465), G_SF(65)));
                bj->addChild(exchang,50);
                
                //转换用
                char t[256];
                sprintf(t, "%d", yhSilver);
                silver = t;
                std::string str = "/";
                
                //拥有的银印花数量
                sprintf(t,  "%d", yhSilverMe);
                silverMe = t;
                
                //银印花
                CCLabelTTF* pYin = CCLabelTTF::create(silverMe.append(str).append(silver).c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(90), G_SF(20)), kCCTextAlignmentLeft);
                pYin->setColor(ccc3(255, 150, 0));
                pYin->setPosition(ccp(G_SF(545), G_SF(70)));
                bj->addChild(pYin);
            }
            
            //查看按钮
            Button *dHBtn = Button::createBtn("Image/BackPack/SeeBtn.png");
            dHBtn->setPosition(ccp(G_SF(485), G_SF(30)));
            //跳转到查看页面
            dHBtn->setOnClickCallbackNode(callfuncN_selector(PrizeShow::getNo), this);
            bj->addChild(dHBtn,1,tag);
            
            //查看
            CCLabelTTF* chankan = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("See").c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(54), G_SF(30)), kCCTextAlignmentLeft);
            chankan->setColor(ccc3(255, 255, 255));
            chankan->setPosition(ccp(G_SF(10), G_SF(-5)));
            dHBtn->addChild(chankan,2);
        }
    }
}

//table cell选中
void PrizeShow::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
    //印花商品No
    int No = 0;
    No = CLSceneManager::sharedSceneManager()->GetPrizeInfoFromIndex(cell->getIdx())->getnStampNo();
    
    openMessageUI(No,cell->getIdx());
    
    
}

CCTableViewCell* PrizeShow::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell *cell = table->dequeueCell();
	if (!cell)
	{
		cell = new CCTableViewCell();
		cell->autorelease();
        
		CCNode *cellNode = CCNode ::create();
		cellNode->setAnchorPoint(CCPointZero);
		cellNode->setPosition(CCPointZero);
        
		createTableViewCell(cellNode,table,idx);
		cellNode->setTag(1);
		cell->addChild(cellNode);
        
	}
	else
	{
		CCNode *cellNode = (CCNode *)cell->getChildByTag(1);
		cellNode->removeAllChildrenWithCleanup(true);
		createTableViewCell(cellNode,table,idx);
	}
	return cell;
    
    
}

//卡牌数量
unsigned int PrizeShow::numberOfCellsInTableView(CCTableView *table)
{
    //获取印花商品数量
    return CLSceneManager::sharedSceneManager()->GetPrizeNum();
}

//打开兑换页面
void PrizeShow::openExchangeUI(CCNode* pSender)
{
    if(CLSceneManager::sharedSceneManager()->getnOpenflg() == 0)
    {
        //兑换功能开启以后删除
        TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("NoExchange"));
        return;
    }
    
    //获得按钮的世界坐标
    CCPoint btnWorldPoint = pSender->getParent()->convertToWorldSpace(pSender->getPosition());
    
    CCLog("btnWorldPoint.y : %f",btnWorldPoint.y);
    
    if (btnWorldPoint.y <G_SF(50)) {
        //选中的tableview内的按钮不在可视区域内，则不进行下面逻辑处理。
        return;
    }
    
    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
    {
        CCLOG("Error: The cur scene is not the main scene!!!");
        //        return;
    }
    YHtopLayer* bl = (YHtopLayer*)pMainLayer->getChildByTag(STAMPTAG);
    
    //选择了那个cell
    bl->setnYHCellTag(pSender->getTag());
    //库存数量
    bl->setnStockOne(CLSceneManager::sharedSceneManager()->GetPrizeInfoFromIndex(pSender->getTag())->getnStockNum());
    
    //印花商品No
    int exchangNo = 0;
    //兑换页面可兑换个数
    int exchangNumber = 0;
    
    exchangNo = CLSceneManager::sharedSceneManager()->GetPrizeInfoFromIndex(pSender->getTag())->getnStampNo();
    exchangNumber = exchangMap[pSender->getTag()];
    
    
    GoodsExchangeMain *exChangeMain = GoodsExchangeMain::create(exchangNo, exchangNumber);
    exChangeMain->setPosition(ccp(G_SF(0), G_SF(-94)));
    this->getParent()->addChild(exChangeMain, 10, EXCHANGE_TAG_MAIN );
    
    this->removeFromParentAndCleanup(true);
}


//关闭使用
void PrizeShow::menuCallBackButton(CCNode* r)
{
    
//    this->getParent()->removeFromParent();
//    this->getParent()->removeAllChildrenWithCleanup(true);
    
    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer){
        pMainLayer->getChildByTag(STAMPTAG)->removeFromParent();
    }
    return;
}

//打开奖品详细页面
void PrizeShow::getNo(CCNode* pSender)
{
    //获得按钮的世界坐标
    CCPoint btnWorldPoint = pSender->getParent()->convertToWorldSpace(pSender->getPosition());
    
    CCLog("btnWorldPoint.y : %f",btnWorldPoint.y);
    
    if (btnWorldPoint.y <G_SF(50)) {
        //选中的tableview内的按钮不在可视区域内，则不进行下面逻辑处理。
        return;
    }
    
    //印花商品No
    int No = 0;
    
    No = CLSceneManager::sharedSceneManager()->GetPrizeInfoFromIndex(pSender->getTag())->getnStampNo();
    
    
    openMessageUI(No,pSender->getTag());
}

//打开奖品详情
void PrizeShow::openMessageUI(int YHNo,int tag)
{
    
    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
    {
        CCLOG("Error: The cur scene is not the main scene!!!");
        //        return;
    }
    YHtopLayer* bl = (YHtopLayer*)pMainLayer->getChildByTag(STAMPTAG);
    
    //选择了那个cell
    bl->setnYHCellTag(tag);
    
    //发送印花信息请求
    //发送内容
    Json::Value data;
    data["msgid"].append("87"); //指令码
    data["No"] = YHNo;  //点击查看印花商品No
    
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_PRIZEINFO, data);
    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
    
    //remove
    this->removeFromParentAndCleanup(true);
    
}

/**************************************************
 * 印花详情UI
 ***************************************************/
PrizeMessage* PrizeMessage::create(Json::Value &data)
{
    PrizeMessage *pRet = new PrizeMessage();
	if (pRet && pRet->init(data))
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

bool PrizeMessage::init(Json::Value &data)
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::init() )
    {
        return false;
    }
    
    getMessageInfo(data);
    
    return true;
    
}

void PrizeMessage::getMessageInfo(Json::Value &data)
{
    //取得印花数量
    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
    {
        CCLOG("Error: The cur scene is not the main scene!!!");
        //        return;
    }
    YHtopLayer* bl = (YHtopLayer*)pMainLayer->getChildByTag(STAMPTAG);
    
    //选择了那个cell
    int cell= 0;
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
    
    //选择了那样商品
    cell = bl->getnYHCellTag();
    
    //当前拥有的金印花
    yhGoldMe = CLSceneManager::sharedSceneManager()->GetPrizeInfoFromIndex(cell)->getnHaveGoldStampNum();
    //当前拥有的银印花
    yhSilverMe = CLSceneManager::sharedSceneManager()->GetPrizeInfoFromIndex(cell)->getnHaveSilverStampNum();
    
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
    
    //添加背景图片
    CCSprite *pSprite = CCSprite::create("Image/BackPack/PrizeDetailsBG.png");
    pSprite->setPosition(ccp(G_SF(320),G_SF(435)));
    this->addChild(pSprite);
    
    //返回按钮
    Button *back = Button::createBtn("Image/PublicUI/Back.png");
    back->setPosition(ccp(G_SF(535), G_SF(640)));
    back->setOnClickCallbackNode(callfuncN_selector(PrizeMessage::backTOPrize), this);
    back->setHandlerPriority(-10001);
    pSprite->addChild(back,2);
    
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
    
    CCSprite *pStamp = CCSprite::create(stampSmall_path.c_str());
    pStamp->setPosition(ccp(G_SF(80), G_SF(525)));
    pSprite->addChild(pStamp);
    
    //商品名称
    CCLabelTTF* nameLabel = CCLabelTTF::create(yhPName.c_str(), "Arial", G_SF(36), CCSizeMake(G_SF(360), G_SF(100)), kCCTextAlignmentLeft);
    nameLabel->setColor(ccc3(51, 51, 51));
    nameLabel->setPosition(ccp(G_SF(352), G_SF(525)));
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
    
    //金印花和银印花 个数可以兑换一个商品以上
    if(gNumber > 0 && sNumber > 0){
        //兑换
        Button *dHBtn = Button::createBtn("Image/BackPack/ExchangeBtn.png");
//        dHBtn->setPosition(ccp(G_SF(510), G_SF(555)));
        dHBtn->setPosition(ccp(G_SF(500), G_SF(480)));
        //跳转到兑换页面
        dHBtn->setOnClickCallbackNode(callfuncN_selector(PrizeMessage::openExchangeUI), this);
        pSprite->addChild(dHBtn,1,yhNo);
        
        //去兑换文字
        CCLabelTTF* exchang = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("ToExchange").c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(54), G_SF(30)), kCCTextAlignmentLeft);
        exchang->setColor(ccc3(255, 255, 255));
        exchang->setPosition(ccp(G_SF(10), G_SF(-5)));
        dHBtn->addChild(exchang,50);
        
        //可兑换商品数目，兑换页面用
        if(gNumber>sNumber){
            
            Number = sNumber;
        }else{
            Number = gNumber;
        }
        
    }else
    {
        
        //转换用
        char CGold[256];
        sprintf(CGold, "%d", yhGold);
        //所需金印花数目
        gold = CGold;
        
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
        pYin->setPosition(ccp(G_SF(350), G_SF(510)));
        pSprite->addChild(pYin);
    }
    
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
    scrollView->setTouchPriority(-10001);//20131020优先级设置
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
    pDescription->setPosition(ccp(G_SF(20), G_SF(195) ));
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
    //赞助商
    std::string buyName = CLChangeString::sharedChangeString()->GetChangeString("Sponsor");
    CCLabelTTF* pBuyName= CCLabelTTF::create(buyName.append(m_BuyName).c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(300), G_SF(25)), kCCTextAlignmentLeft);
    pBuyName->setColor(ccc3(102, 102, 102));
    pBuyName->setPosition(ccp(G_SF(20), G_SF(155)));
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
    pExchangeName->setPosition(ccp(G_SF(20), G_SF(55)));
    scrollView->addChild(pExchangeName);
    
    //兑换历史
    CCLabelTTF* pHistory= CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("ExchangeHistory").c_str(), "Arial", G_SF(18), CCSizeMake(G_SF(80), G_SF(25)), kCCTextAlignmentLeft);
    pHistory->setColor(ccc3(102, 102, 102));
    pHistory->setPosition(ccp(G_SF(20), G_SF(5)));
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
            pExchangeCode->setPosition(ccp(G_SF(20), G_SF(-15)));
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
            pExchangeSuccess->setPosition(ccp(G_SF(20),  G_SF(-35)));
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
            pExchangeCode->setPosition(ccp(G_SF(20), G_SF(-55)));
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
            pExchangeSuccess->setPosition(ccp(G_SF(20), G_SF(-75)));
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
            pExchangeCode->setPosition(ccp(G_SF(20), G_SF(-95)));
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
            pExchangeSuccess->setPosition(ccp(G_SF(20), G_SF(-115)));
            scrollView->addChild(pExchangeSuccess);
        }
        
    }
    
//    //创建一个scrollView
//    CCScrollView *scrollView=CCScrollView::create();
//    scrollView->setPosition(ccp(G_SF(20), G_SF(110)));
//    scrollView->setTouchPriority(-10001);//20131020优先级设置
    
//    //scrollView背景Layer
//    CCLayerColor* scrollViewBJ=CCLayerColor::create(ccc4(255, 255, 255, 255));
//    scrollViewBJ->setContentSize(CCSizeMake(G_SF(530), G_SF(300)));
//    scrollViewBJ->setPosition(ccp(G_SF(20), G_SF(110)));
//    pSprite->addChild(scrollViewBJ);
//    
//    
//    //显示的区域
//    scrollView->setViewSize(CCSizeMake(G_SF(530), G_SF(300)));//设置view的大小
//    //    scrollView->setContentOffset(CCPointZero);
//    
//    continerLayer->setContentSize(CCSizeMake(G_SF(530), G_SF(600)));//设置滚动区域的大小
//    //显示滑动的区域大小 scrollview的实际大小
//    scrollView->setContentSize(CCSizeMake(G_SF(530), G_SF(600)));//设置scrollview区域的大小
//    scrollView->setContainer(continerLayer);
//    
//    //实现触摸消息
//    scrollView->setTouchEnabled(true);
//    scrollView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向
//    scrollView->setDelegate(this);
//    
//    pSprite->addChild(scrollView);
    
    //银印花赚取按钮
    Button *silverBtn = Button::createBtn("Image/BackPack/StampEarnBtn.png");
    silverBtn->setPosition(ccp(G_SF(55) + silverBtn->getContentSize().width*0.5, G_SF(-55)));
    //跳转到主场景
    silverBtn->setOnClickCallbackNode(callfuncN_selector(PrizeMessage::JumpMainscene), this);
    pSprite->addChild(silverBtn);
    
    //按钮文字：
    CCLabelTTF* labelSilver = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("FindSilverStamp").c_str(), "Arial", G_SF(30), CCSizeMake(G_SF(150), G_SF(40)), kCCTextAlignmentLeft);
    labelSilver->setPosition(ccp(G_SF(0), G_SF(0)));
    silverBtn->addChild(labelSilver,1);
    
    //金印花赚取按钮
    Button *goldBtn = Button::createBtn("Image/BackPack/StampEarnBtn.png");
    goldBtn->setPosition(ccp(G_SF(305) + goldBtn->getContentSize().width*0.5 , G_SF(-55)));
    //跳转到兑换页面
    goldBtn->setOnClickCallbackNode(callfuncN_selector(PrizeMessage::JumpFightCene), this);
    pSprite->addChild(goldBtn);
    
    //按钮文字
    CCLabelTTF* labelGold = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("FindGoldStamp").c_str(), "Arial", G_SF(30), CCSizeMake(G_SF(150), G_SF(40)), kCCTextAlignmentLeft);
    labelGold->setPosition(ccp(G_SF(0), G_SF(0)));
    goldBtn->addChild(labelGold,1);
}

void PrizeMessage::backTOPrize()
{
    
    this->removeFromParentAndCleanup(true);
    
    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
    {
        CCLOG("Error: The cur scene is not the main scene!!!");
        //        return;
    }
    
    PrizeShow* prize = PrizeShow::createPShow();
    
    prize->setPosition(ccp(G_SF(-50), G_SF(0)));
    pMainLayer->getChildByTag(STAMPTAG)->addChild(prize);
    
    
}

//跳转到主页面
void PrizeMessage::JumpMainscene()
{
    CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer)
    {
        pMainLayer->removeChildByTag(500);
    }
    
    this->getParent()->removeFromParentAndCleanup(true);
}

//跳转到对战
void PrizeMessage::JumpFightCene()
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

//跳转到兑换页面
void PrizeMessage::openExchangeUI(CCNode* pSender)
{
    if(CLSceneManager::sharedSceneManager()->getnOpenflg() == 0)
    {
        //兑换功能开启以后删除
        TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("NoExchange"));
        return;
    }
    
    int exchangNo = 0;
    int exchangNumber = 0;
    //印花商品No
    exchangNo = pSender->getTag();
    //兑换页面可兑换个数
    exchangNumber = Number;
    
    GoodsExchangeMain *exChangeMain = GoodsExchangeMain::create(exchangNo, exchangNumber);
    exChangeMain->setPosition(ccp(G_SF(0), G_SF(-94)));
    this->getParent()->addChild(exChangeMain, 10, EXCHANGE_TAG_MAIN );
    
    this->removeFromParentAndCleanup(true);
}

void PrizeMessage::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);
}

void PrizeMessage::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool PrizeMessage::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    // 记录触摸起始点的位置
    //    m_touchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
    
    return true;
}


void PrizeMessage::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void PrizeMessage::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}


/**************************************************
 * 兑换逻辑Action控制类
 ***************************************************/
GoodsExchangeMain* GoodsExchangeMain::create(int goodsID, int canExchangeNum)
{
    GoodsExchangeMain* pLayer = new GoodsExchangeMain();
    
    if(pLayer && pLayer->init(goodsID, canExchangeNum))
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

bool GoodsExchangeMain::init(int goodsID, int canExchangeNum)
{
    if ( !CCLayerColor::initWithColor(ccc4(255,255,255,0)) )
    {
        return false;
    }
    //    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCSize size = ccp(G_SF(640), G_SF(960));
    
    this->setTouchPriority(-100002);
    
    //商品固定信息记录
    stampNo = goodsID; //印花No
    maxStampNum = canExchangeNum; //最大可兑换数量
    oneCostGoldStamp = JsonDataManager::shardJDManager()->m_yinHua[stampNo]->m_YH1; //单个商品需要的金印花
    oneCostSilverStampNum = JsonDataManager::shardJDManager()->m_yinHua[stampNo]->m_YH2; //单个商品需要的银印花
    
    pCanExchangeNum = canExchangeNum; //可兑换数量
    pCostGoldStampNum = oneCostGoldStamp;
    pCostSilverStampNum = oneCostSilverStampNum;
    
    CCLog("canshu->%d,%d,%d,%d",stampNo,pCanExchangeNum,oneCostGoldStamp,oneCostSilverStampNum );
    
    //窗体背景图片
    m_pBasicBg = CCSprite::create("Image/BackPack/B_ExWriBg.png");
    m_pBasicBg->setPosition( ccp(size.width * 0.5, size.height * 0.5));
    this->addChild(m_pBasicBg);
    
    pPhoneNo = CLPlayerManager::shardPlayerManager()->getpUserInfo()->getsPhone(); //取得用户手机号码
    if (pPhoneNo != "") {
        hadPhoneNo = true;
    } else {
        hadPhoneNo = false;
    }
    
    //默认固定UI
    defaultUI();
    
    //可变UI
    variableUI();
    
    return true;
}

void GoodsExchangeMain::defaultUI()
{
    //标题
    CCSprite *titleBg = CCSprite::create("Image/PublicUI/TitleBackground.png");
    titleBg->setPosition(ccp(G_SF(55), G_SF(735)));
    m_pBasicBg->addChild(titleBg);
    
    CCSprite *bgTitle = CCSprite::create("Image/BackPack/B_stampeCtit.png");
    bgTitle->setPosition( ccp(titleBg->getContentSize().width * 0.5, titleBg->getContentSize().height * 0.5));
    titleBg->addChild(bgTitle);
    
    //商品title
    std::string c_stampName =  JsonDataManager::shardJDManager()->m_yinHua[stampNo]->m_Name;
    CCLabelTTF *titleTTF= CCLabelTTF::create(c_stampName.c_str(), "Arial", G_SF(43),CCSizeMake(G_SF(300), G_SF(50)), kCCTextAlignmentLeft);
    titleTTF->setPosition(ccp(titleTTF->getContentSize().width * 0.5 + G_SF(30), G_SF(645)));
    titleTTF->setColor(ccc3(100, 50, 50));
    m_pBasicBg->addChild(titleTTF);
    
    //可兑换数量
    std::string c_NumName = CLChangeString::sharedChangeString()->GetChangeString("CanExchangeStr").c_str();
    CCLabelTTF *canExchangeNum= CCLabelTTF::create(c_NumName.c_str(), "Arial", G_SF(28),CCSizeMake(G_SF(300),G_SF(33)), kCCTextAlignmentLeft);
    canExchangeNum->setPosition(ccp(titleTTF->getPositionX(), titleTTF->getPositionY() - G_SF(85)));
    canExchangeNum->setColor(ccc3(100, 50, 0));
    m_pBasicBg->addChild(canExchangeNum);
    
    //加按钮
    Button *plusBtn = Button::createBtn("Image/BackPack/A_PlusBtn.png");
    plusBtn->setPosition(ccp(canExchangeNum->getPositionX() + G_SF(80), canExchangeNum->getPositionY()));
    plusBtn->setOnClickCallbackNode(callfuncN_selector(GoodsExchangeMain::plusBtnCallback), this);
    plusBtn->setHandlerPriority(-10001);
    m_pBasicBg->addChild(plusBtn);
    
    CCSprite *canCgBg = CCSprite::create("Image/BackPack/B_ExcNumBg.png");
    canCgBg->setPosition( ccp(plusBtn->getPositionX()+ plusBtn->getContentSize().width * 0.5 + canCgBg->getContentSize().width * 0.5 + G_SF(10), canExchangeNum->getPositionY()));
    m_pBasicBg->addChild(canCgBg);
    
    char strNumName[20] = {0};
    sprintf(strNumName, "%u", pCanExchangeNum);
    m_pCanExchange = CCLabelTTF::create(strNumName, "Arial", G_SF(48),CCSizeMake(G_SF(100), G_SF(54)), kCCTextAlignmentCenter);
    m_pCanExchange->setPosition(ccp(canCgBg->getContentSize().width * 0.5, canCgBg->getContentSize().height * 0.5));
    m_pCanExchange->setColor(ccWHITE);
    canCgBg->addChild(m_pCanExchange, 10);
    
    //减按钮
    Button *substBtn = Button::createBtn("Image/BackPack/A_SubtrBtn.png");
    substBtn->setPosition(ccp(canCgBg->getPositionX() + canCgBg->getContentSize().width * 0.5 + plusBtn->getContentSize().width * 0.5 + G_SF(10), canExchangeNum->getPositionY()));
    substBtn->setOnClickCallbackNode(callfuncN_selector(GoodsExchangeMain::reduceBtnCallback), this);
    substBtn->setHandlerPriority(-10001);
    m_pBasicBg->addChild(substBtn);
    
    //消耗金印花数
    std::string c_StampNumName = CLChangeString::sharedChangeString()->GetChangeString("CostGoldStamp").c_str();
    CCLabelTTF *costGoldStamp= CCLabelTTF::create(c_StampNumName.c_str(), "Arial", G_SF(28),CCSizeMake(G_SF(300), G_SF(32)), kCCTextAlignmentLeft);
    costGoldStamp->setPosition(ccp(titleTTF->getPositionX(), canExchangeNum->getPositionY() - G_SF(80)));
    costGoldStamp->setColor(ccc3(100, 50, 0));
    m_pBasicBg->addChild(costGoldStamp);
    
    char strGoldNum[20] = {0};
    pCostGoldStampNum = pCanExchangeNum * oneCostGoldStamp;
    sprintf(strGoldNum, "%u", pCostGoldStampNum);
    m_pCostGoldStamp = CCLabelTTF::create(strGoldNum, "Arial", G_SF(28),CCSizeMake(G_SF(80), G_SF(32)), kCCTextAlignmentCenter);
    m_pCostGoldStamp->setPosition(ccp(plusBtn->getPositionX(), costGoldStamp->getPositionY()));
    m_pCostGoldStamp->setColor(ccc3(100, 50, 0));
    m_pBasicBg->addChild(m_pCostGoldStamp);
    
    //消耗银印花数
    std::string c_StampNumName2 = CLChangeString::sharedChangeString()->GetChangeString("CostSilverStamp").c_str();
    CCLabelTTF *costSilverStamp= CCLabelTTF::create(c_StampNumName2.c_str(), "Arial", G_SF(28),CCSizeMake(G_SF(300),G_SF(32)), kCCTextAlignmentLeft);
    costSilverStamp->setPosition(ccp(titleTTF->getPositionX(), costGoldStamp->getPositionY() - G_SF(80)));
    costSilverStamp->setColor(ccc3(100, 50, 0));
    m_pBasicBg->addChild(costSilverStamp);
    
    char strSilverNum[20] = {0};
    pCostSilverStampNum = pCanExchangeNum * oneCostSilverStampNum;
    sprintf(strSilverNum, "%u", pCostSilverStampNum);
    m_pCostSilverStamp = CCLabelTTF::create(strSilverNum, "Arial", G_SF(28),CCSizeMake(G_SF(80),G_SF(32)), kCCTextAlignmentCenter);
    m_pCostSilverStamp->setPosition(ccp(plusBtn->getPositionX(), costSilverStamp->getPositionY()));
    m_pCostSilverStamp->setColor(ccc3(100, 50, 0));
    m_pBasicBg->addChild(m_pCostSilverStamp);
    
    //关闭
    Button *shutdown = Button::createBtn("Image/PublicUI/Back.png");
    shutdown->setPosition(ccp(G_SF(535),
                              G_SF(735)));
    shutdown->setOnClickCallbackNode(callfuncN_selector(GoodsExchangeMain::menuCallBackButton), this);
    shutdown->setHandlerPriority(-10001);
    m_pBasicBg->addChild(shutdown);
    
}

void GoodsExchangeMain::variableUI()
{
    
    if (hadPhoneNo) {
        
        //发送到文字
        //        std::ostringstream confirmStr;
        //        confirmStr << "<font><fontname>Arial</><fontsize>30</><color><value>2626570</>" << CLChangeString::sharedChangeString()->GetChangeString("SendTo").c_str()<< "</></>";
        //        confirmStr << "<font><fontname>Arial</><fontsize>30</><color><value>2626570</>" << pPhoneNo.substr(0, 3) << "* * * *" <<pPhoneNo.substr(pPhoneNo.length()-4,pPhoneNo.length())  <<"</></>";
        //
        //        RichLabel *confirmLabel = RichLabel::create(confirmStr.str().c_str(), "Arial", G_SF(130), CCSizeMake(G_SF(400), G_SF(30)), false, false);
        //        confirmLabel->setPosition(ccp(G_SF(30), G_SF(380)));
        //        m_pBasicBg->addChild(confirmLabel);
        
        std::string confiStr = CLChangeString::sharedChangeString()->GetChangeString("SendTo").c_str();
        if (pPhoneNo.length() != 0)
        {
           confiStr = confiStr + pPhoneNo.substr(0, 3) +  "* * * *" + pPhoneNo.substr(pPhoneNo.length()-4,pPhoneNo.length());

        }
        CCLabelTTF *confirmLabel= CCLabelTTF::create(confiStr.c_str(), "Arial", G_SF(30),CCSizeMake(G_SF(400),G_SF(35)), kCCTextAlignmentLeft);
        confirmLabel->setPosition(ccp(G_SF(240), G_SF(300)));
        confirmLabel->setColor(ccc3(100, 50, 0));
        m_pBasicBg->addChild(confirmLabel);
        
        //更换号码
        Button *changeBtn = Button::createBtn("Image/BackPack/StampComBtnSmall.png");
        changeBtn->setPosition(ccp(confirmLabel->getPositionX() + G_SF(200), confirmLabel->getPositionY()));
        changeBtn->setOnClickCallbackNode(callfuncN_selector(GoodsExchangeMain::changePhone), this);
        m_pBasicBg->addChild(changeBtn,0,10);
        
        std::string c_sendTTF = CLChangeString::sharedChangeString()->GetChangeString("ChangePhone").c_str();
        CCLabelTTF *btnLabelTTF= CCLabelTTF::create(c_sendTTF.c_str(), "Arial", G_SF(20));
        btnLabelTTF->setPosition(CCSizeZero);
        btnLabelTTF->setColor(ccWHITE);
        changeBtn->addChild(btnLabelTTF,20);
        
        //确认兑换
        CCSize size = CCDirector::sharedDirector()->getgSceneSize();
        Button *exchageBtn = Button::createBtn("Image/BackPack/C_composite.png");
        exchageBtn->setPosition(ccp(size.width*0.45, G_SF(confirmLabel->getPositionY() - G_SF(130))));
        exchageBtn->setOnClickCallbackNode(callfuncN_selector(GoodsExchangeMain::exchangeSure), this);
        m_pBasicBg->addChild(exchageBtn,0,10);
        
        std::string c_exchangeTTF = CLChangeString::sharedChangeString()->GetChangeString("ConfirmChange").c_str();
        CCLabelTTF *exchangeTTF= CCLabelTTF::create(c_exchangeTTF.c_str(), "Arial", G_SF(36));
        exchangeTTF->setPosition(CCSizeZero);
        btnLabelTTF->setColor(ccWHITE);
        exchageBtn->addChild(exchangeTTF,20);
        
        //协议确认
        CCSprite* spriteNorml = CCSprite::create("Image/BackPack/B_checkBg.png");
        CCMenuItemSprite* mItem_sprite = CCMenuItemSprite::create(spriteNorml,spriteNorml,this, menu_selector(GoodsExchangeMain::checkbox));
        mItem_sprite->setContentSize( CCSizeMake(spriteNorml->getContentSize().width, spriteNorml->getContentSize().height));
        CCMenu* menu = CCMenu::create(mItem_sprite, NULL);
        menu->setPosition(G_SF(155),G_SF(50));
        menu->setTouchPriority(-10003);
        m_pBasicBg->addChild(menu);
        
        //初始勾选
        CCSprite* checkSprNol = CCSprite::create("Image/BackPack/B_checked.png");
        checkSprNol->setPosition(ccp(spriteNorml->getContentSize().width * 0.5, spriteNorml->getContentSize().height * 0.5));
        mItem_sprite->addChild(checkSprNol, 10, 3);
        
        std::string c_explain = CLChangeString::sharedChangeString()->GetChangeString("ConfirmXieYi").c_str();
        CCLabelTTF *explainTTF= CCLabelTTF::create(c_explain.c_str(), "Arial", G_SF(20), CCSizeMake(G_SF(300), G_SF(30)), kCCTextAlignmentCenter);
        explainTTF->setColor(ccc3(200,100,50));
        explainTTF->setAnchorPoint( ccp(G_SF(0), G_SF(0.5)));
        explainTTF->setPosition(ccp(G_SF(175),G_SF(47)));
        m_pBasicBg->addChild(explainTTF);
        
        frameFlgForNote = 1;
        
    }
    else
    {
        
        //输入手机号
        std::string c_inputLabel = CLChangeString::sharedChangeString()->GetChangeString("InputPhone").c_str();
        CCLabelTTF *phoneNoLabel= CCLabelTTF::create(c_inputLabel.c_str(), "Arial", G_SF(28),CCSizeMake(G_SF(270),G_SF(32)), kCCTextAlignmentLeft);
        phoneNoLabel->setPosition(ccp(G_SF(170), G_SF(320)));
        phoneNoLabel->setColor(ccBLACK);
        m_pBasicBg->addChild(phoneNoLabel);
        
        //输入框
        m_pInputLabel = MLTextFieldTTFLayer::create("Image/BackPack/B_inputBg.png", G_SF(280), G_SF(53), kCCTextAlignmentLeft, "Arial", ccBLACK);
        m_pInputLabel->setPosition(ccp(phoneNoLabel->getPositionX(), phoneNoLabel->getPositionY() - G_SF(70)));
        
        m_pInputLabel->getpTTFCursor()->setnLimitFontCount(11);
        m_pInputLabel->getpTTFCursor()->setpCallObj(this);
        m_pInputLabel->getpTTFCursor()->setpCallFuncND(callfuncND_selector(                                                                GoodsExchangeMain::MoveLayerUp));
        m_pInputLabel->getpTTFCursor()->setString(pPhoneNo.c_str()); //设置电话号码
        
        m_pBasicBg->addChild(m_pInputLabel,0,10);
        
        
        Button *sendBtn = Button::createBtn("Image/BackPack/StampComBtnSmall.png");
        sendBtn->setPosition(ccp(m_pInputLabel->getPositionX() + m_pInputLabel->getContentSize().width* 0.5 + sendBtn->getContentSize().width* 0.5 + G_SF(5), phoneNoLabel->getPositionY() - G_SF(70)));
        sendBtn->setOnClickCallbackNode(callfuncN_selector(GoodsExchangeMain::SendTmpPhoneCallBack), this);
        m_pBasicBg->addChild(sendBtn,0,10);
        
        std::string c_sendTTF = CLChangeString::sharedChangeString()->GetChangeString("SendPhone").c_str();
        CCLabelTTF *btnLabelTTF= CCLabelTTF::create(c_sendTTF.c_str(), "Arial", G_SF(20));
        btnLabelTTF->setColor(ccWHITE);
        btnLabelTTF->setPosition(CCSizeZero);
        sendBtn->addChild(btnLabelTTF,20);
        
        std::string c_explain = CLChangeString::sharedChangeString()->GetChangeString("ExchangeMsgInfo").c_str();
        CCLabelTTF *explainTTF= CCLabelTTF::create(c_explain.c_str(), "Arial", G_SF(20), CCSizeMake(G_SF(510), G_SF(70)), kCCTextAlignmentLeft);
        explainTTF->setColor(ccc3(100, 50, 0));
        explainTTF->setPosition(ccp(G_SF(290),G_SF(170)));
        m_pBasicBg->addChild(explainTTF);
        
        frameFlgForNote = 2;
        
    }
    
}

//checkbox处理
void GoodsExchangeMain::checkbox(CCNode *r)
{
    
    //修改是否选中变量
    
    if (r->getChildByTag(3)) {
        r->removeChildByTag(3, true);
        
        isChecked = false;
    }
    else {
        
        CCSprite* checkSpr = CCSprite::create("Image/BackPack/B_checked.png");
        checkSpr->setPosition(ccp(r->getContentSize().width * 0.5, r->getContentSize().height * 0.5));
        r->addChild(checkSpr, 10, 3);
        
        isChecked = true;
        
    }
    
}

//关闭按钮统一管理回调函数
void GoodsExchangeMain::menuCallBackButton(CCNode* r)
{
    
    //（1：兑换画面（号码有） 2:兑换画面（号码无）3:更换号码发送手机画面 4:输入验证码画面 5:兑换成功）
    //    CCLog("要返回到->%d", frameFlgForNote);
    
    switch (frameFlgForNote) {
        case 3:  //更改手机号码画面，会退到有手机号的兑换主页
        {
            
            m_pBasicBg->removeAllChildrenWithCleanup(true);
            
            pPhoneNo = CLPlayerManager::shardPlayerManager()->getpUserInfo()->getsPhone(); //取得用户手机号码
            if (pPhoneNo != "") {
                hadPhoneNo = true;
            } else {
                hadPhoneNo = false;
            }
            
            //默认固定UI
            defaultUI();
            
            //取得手机号测试用
//            hadPhoneNo = true;
            
            //可变UI
            variableUI();
            
            break;
        }
        case 4:  //输入验证码画面
        {
            
            if (hadPhoneNo) {
                changePhone(this);
            } else {
                m_pBasicBg->removeAllChildrenWithCleanup(true);
                
                //                pPhoneNo = CLPlayerManager::shardPlayerManager()->getpUserInfo()->getsPhone(); //取得用户手机号码
                //                if (pPhoneNo != "") {
                //                    hadPhoneNo = true;
                //                } else {
                //                    hadPhoneNo = false;
                //                }
                
                //默认固定UI
                defaultUI();
                
                //可变UI
                variableUI();
            }
            
            break;
        }
        default: //兑换画面，默认是回到印花展柜
        {
            
            this->removeFromParentAndCleanup(true);
            
            CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
            if(pMainLayer->getnCTIndex() != CLScene_MainScene)
            {
                CCLOG("Error: The cur scene is not the main scene!!!");
                //        return;
            }
            YHtopLayer* bl = (YHtopLayer*)pMainLayer->getChildByTag(STAMPTAG);
            
            PrizeShow* prize = PrizeShow::createPShow();
            
            prize->setPosition(ccp(G_SF(-50), G_SF(0)));
            pMainLayer->getChildByTag(STAMPTAG)->addChild(prize);
        }
            break;
    }
    
}

void GoodsExchangeMain::MoveLayerUp(CCNode*, void* value)
{
    
    bool *b = (bool*)(value);
    if (*b)
    {
        if (isTouchWrite == true)
        {
            return;
        }
        this->getParent()->runAction(CCMoveBy::create(0.2, ccp(G_SF(0), G_SF(200))));
        isTouchWrite = true;
    }
    else
    {
        if (isTouchWrite == false)
        {
            return;
        }
        this->getParent()->runAction(CCMoveBy::create(0.2 ,ccp(G_SF(0), G_SF(-200))));
        isTouchWrite = false;
    }
}

//更换号码回调
void GoodsExchangeMain::changePhone(CCNode* r)
{
    
    m_pBasicBg->removeAllChildrenWithCleanup(true);
    
    //标题
    CCSprite *titleBg = CCSprite::create("Image/PublicUI/TitleBackground.png");
    titleBg->setPosition(ccp(G_SF(55), G_SF(735)));
    m_pBasicBg->addChild(titleBg);
    
    CCSprite *bgTitle = CCSprite::create("Image/BackPack/B_stampeCtit.png");
    bgTitle->setPosition( ccp(titleBg->getContentSize().width * 0.5, titleBg->getContentSize().height * 0.5));
    titleBg->addChild(bgTitle);
    
    //商品title
    std::string c_stampName = CLChangeString::sharedChangeString()->GetChangeString("CheckPhone").c_str();
    CCLabelTTF *titleTTF= CCLabelTTF::create(c_stampName.c_str(), "Arial",G_SF(43),CCSizeMake(G_SF(300),G_SF(48)), kCCTextAlignmentLeft);
    titleTTF->setPosition(ccp(titleTTF->getContentSize().width* 0.5 + G_SF(30), G_SF(645)));
    titleTTF->setColor(ccc3(100, 50, 50));
    m_pBasicBg->addChild(titleTTF);
    
    //发送到文字
    std::string c_inputLabel = CLChangeString::sharedChangeString()->GetChangeString("SendTo").c_str();
    CCLabelTTF *phoneNoLabel= CCLabelTTF::create(c_inputLabel.c_str(), "Arial", G_SF(28),CCSizeMake(G_SF(270),G_SF(33)), kCCTextAlignmentLeft);
    phoneNoLabel->setPosition(ccp(G_SF(165), G_SF(560)));
    phoneNoLabel->setColor(ccBLACK);
    m_pBasicBg->addChild(phoneNoLabel);
    
    //输入框
    m_pInputForChange = MLTextFieldTTFLayer::create("Image/BackPack/B_inputBg.png", G_SF(280), G_SF(53), kCCTextAlignmentLeft, "Arial", ccBLACK);
    m_pInputForChange->setPosition(ccp(phoneNoLabel->getPositionX() + G_SF(5), phoneNoLabel->getPositionY() - G_SF(70)));
    m_pInputForChange->getpTTFCursor()->setnLimitFontCount(11);
    
    if (frameFlgForNote == 4) {  //输入验证码页面返回需要设置当前号码
        m_pInputForChange->getpTTFCursor()->setString(pPhoneNo.c_str());
    }
    
    m_pInputForChange->getpTTFCursor()->setpCallObj(this);
    m_pBasicBg->addChild(m_pInputForChange,0,10);
    
    //发送手机号按钮
    Button *sendCodeBtn = Button::createBtn("Image/BackPack/StampComBtnSmall.png");
    sendCodeBtn->setPosition(ccp(m_pInputForChange->getPositionX() + m_pInputForChange->getContentSize().width* 0.5 + sendCodeBtn->getContentSize().width* 0.5 + G_SF(5), phoneNoLabel->getPositionY() - G_SF(70)));
    sendCodeBtn->setOnClickCallbackNode(callfuncN_selector(GoodsExchangeMain::SendPhoneCallBack), this);
    m_pBasicBg->addChild(sendCodeBtn,0,10);
    
    std::string c_sendTTF = CLChangeString::sharedChangeString()->GetChangeString("SendPhone").c_str();
    CCLabelTTF *btnLabelTTF= CCLabelTTF::create(c_sendTTF.c_str(), "Arial", G_SF(20),CCSizeMake(G_SF(100), G_SF(28)), kCCTextAlignmentLeft);
    btnLabelTTF->setPosition(CCSizeZero);
    btnLabelTTF->setColor(ccWHITE);
    sendCodeBtn->addChild(btnLabelTTF, 20);
    
    //关闭
    Button *shutdown = Button::createBtn("Image/PublicUI/Back.png");
    shutdown->setPosition(ccp(G_SF(520), G_SF(735)));
    shutdown->setOnClickCallbackNode(callfuncN_selector(GoodsExchangeMain::menuCallBackButton), this);
    shutdown->setHandlerPriority(-10001);
    m_pBasicBg->addChild(shutdown);
    
    //描述
    std::string c_explain = CLChangeString::sharedChangeString()->GetChangeString("ExchangeMsgInfo").c_str();
    CCLabelTTF *explainTTF= CCLabelTTF::create(c_explain.c_str(), "Arial", G_SF(20), CCSizeMake(G_SF(510), G_SF(70)), kCCTextAlignmentLeft);
    explainTTF->setColor(ccc3(100, 50, 0));
    explainTTF->setPosition(ccp(G_SF(290),G_SF(400)));
    m_pBasicBg->addChild(explainTTF);
    
    frameFlgForNote = 3;
    
}


//确认兑换回调函数
void GoodsExchangeMain::exchangeSure(CCNode* r)
{
    if (!isChecked) {
        TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("PleaseReadTreaty").c_str());
        return; //如果没有勾选阅读协议，不执行下列操作
    }
    
    //发送内容
    Json::Value data;
    data["msgid"].append("85"); //指令码
    data["mphone"] = pPhoneNo;  //购买的卡牌No
    data["No"] = stampNo;      //印花商品No
    data["excnm"] = pCanExchangeNum;   //选择兑换数量
    
    CCLog("sendLog->%s", pPhoneNo.c_str());
    
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_PRIZEEXCHANGEOK, data);
    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
    
}


//发送要兑换的手机号
void GoodsExchangeMain::SendPhoneCallBack(CCNode* r)
{
    //发送验证码
    //    CCLog("string %s", m_pInputLabel->getpTTFCursor()->getString());
    //
    
    pPhoneNo = m_pInputForChange->getpTTFCursor()->getString();
    
    if (!isPhoneNum(pPhoneNo)) {
        return;
    }
    
    frameFlgForNote = 2; //前一页面是无手机号的兑换主页
    
    //发送内容
    Json::Value data;
    data["msgid"].append("89"); //指令码
    data["mphone"] = pPhoneNo;  //购买的卡牌No
    data["dflg"] = 1;  //默认手机
    
    data["No"] = stampNo;  //默认手机
    
    //    CCLog("sendLog->%s", pPhoneNo.c_str());
    
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_SENDPHONE, data);
    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
    
}

//发送要更改的兑换手机号
void GoodsExchangeMain::SendTmpPhoneCallBack(CCNode* r)
{
    frameFlgForNote = 1;  //前一页面是有手机号的兑换主页
    pPhoneNo = m_pInputLabel->getpTTFCursor()->getString();
    
    if (!isPhoneNum(pPhoneNo)) {
        return;
    }
    
    //发送内容
    Json::Value data;
    data["msgid"].append("89"); //指令码
    data["mphone"] = pPhoneNo;  //购买的卡牌No
    data["dflg"] = 2;  //临时手机
    data["No"] = stampNo;  //默认手机
    
    CCLog("sendLog->%s", pPhoneNo.c_str());
    
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_SENDPHONE, data);
    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
    
}

void GoodsExchangeMain::plusBtnCallback(CCNode *r)
{
    if (pCanExchangeNum == maxStampNum) {
        //添加dlg
        TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("MsgJustExchange").c_str());
        return;
    }
    
    char str[20] = {0};
    pCanExchangeNum = pCanExchangeNum + 1;
    sprintf(str, "%u", pCanExchangeNum);
    m_pCanExchange->setString(str);
    
    char str1[20] = {0};
    pCostGoldStampNum = pCostGoldStampNum + oneCostGoldStamp;
    sprintf(str1, "%u", pCostGoldStampNum);
    m_pCostGoldStamp->setString(str1);
    
    char str2[20] = {0};
    pCostSilverStampNum =  pCostSilverStampNum + oneCostSilverStampNum;
    sprintf(str2, "%u", pCostSilverStampNum);
    m_pCostSilverStamp->setString(str2);
}

void GoodsExchangeMain::reduceBtnCallback(CCNode *r)
{
    
    if (pCanExchangeNum == 1) {
        TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("MsgShouldOne").c_str());
        return;
    }
    
    char str[20] = {0};
    pCanExchangeNum = pCanExchangeNum - 1;
    sprintf(str, "%u", pCanExchangeNum);
    m_pCanExchange->setString(str);
    
    char str1[20] = {0};
    pCostGoldStampNum = pCostGoldStampNum - oneCostGoldStamp;
    sprintf(str1, "%u", pCostGoldStampNum);
    m_pCostGoldStamp->setString(str1);
    
    char str2[20] = {0};
    pCostSilverStampNum = pCostSilverStampNum - oneCostSilverStampNum;
    sprintf(str2, "%u", pCostSilverStampNum);
    m_pCostSilverStamp->setString(str2);
}

//发送手机号后的服务器反馈回调函数
void GoodsExchangeMain::_sendBackVerificationCode(std::string verificationCode, std::string phoneNo)
{
    m_pBasicBg->removeAllChildrenWithCleanup(true);
    
    CodeCheckUI(verificationCode); //弹出验证码UI
}

//验证码输入UI
void GoodsExchangeMain::CodeCheckUI(std::string verificationCode)
{
    //标题
    CCSprite *titleBg = CCSprite::create("Image/PublicUI/TitleBackground.png");
    titleBg->setPosition(ccp(G_SF(55), G_SF(735)));
    m_pBasicBg->addChild(titleBg);
    
    CCSprite *bgTitle = CCSprite::create("Image/BackPack/B_stampeCtit.png");
    bgTitle->setPosition( ccp(titleBg->getContentSize().width * 0.5, titleBg->getContentSize().height * 0.5));
    titleBg->addChild(bgTitle);
    
    //输入验证码titile
    std::string c_stampName = CLChangeString::sharedChangeString()->GetChangeString("InputCode").c_str();
    CCLabelTTF *titleTTF= CCLabelTTF::create(c_stampName.c_str(), "Arial", G_SF(43),CCSizeMake(G_SF(300), G_SF(47)), kCCTextAlignmentLeft);
	titleTTF->setPosition(ccp(titleTTF->getContentSize().width* 0.5 + G_SF(30), G_SF(645)));
    titleTTF->setColor(ccc3(100, 50, 50));
	m_pBasicBg->addChild(titleTTF);
    
    //验证码确认Label
    //    std::ostringstream confirmStr;
    //    confirmStr << "<font><fontname>Arial</><fontsize>24</><color><value>2626570</>" << CLChangeString::sharedChangeString()->GetChangeString("SendToJust").c_str()<< "</></>";
    //    confirmStr << "<font><fontname>Arial</><fontsize>24</><color><value>2626570</>" << pPhoneNo.substr(0, 3) << "* * * *" <<pPhoneNo.substr(pPhoneNo.length()-4,pPhoneNo.length())  <<"</></>";
    //    confirmStr << "<font><fontname>Arial</><fontsize>24</><color><value>2626570</>" << CLChangeString::sharedChangeString()->GetChangeString("DeCode").c_str() <<"</></>";
    //    RichLabel *confirmLabel = RichLabel::create(confirmStr.str().c_str(), "Arial", G_SF(120), CCSizeMake(G_SF(400), G_SF(120)), false, false);
    //    confirmLabel->setPosition(ccp(G_SF(30), titleTTF->getPositionY() - G_SF(70)));
    //    m_pBasicBg->addChild(confirmLabel);
    
    std::string confiStr = CLChangeString::sharedChangeString()->GetChangeString("SendToJust").c_str();
    if (pPhoneNo.length() != 0)
    {
        confiStr = confiStr + pPhoneNo.substr(0, 3) + "* * * *" + pPhoneNo.substr(pPhoneNo.length()-4,pPhoneNo.length())+CLChangeString::sharedChangeString()->GetChangeString("DeCode").c_str();
    }

    
    CCLabelTTF *confirmLabel= CCLabelTTF::create(confiStr.c_str(), "Arial", G_SF(24),CCSizeMake(G_SF(450), G_SF(35)), kCCTextAlignmentLeft);
	confirmLabel->setPosition(ccp(G_SF(250), titleTTF->getPositionY() - G_SF(80)));
    confirmLabel->setColor(ccc3(100, 50, 0));
	m_pBasicBg->addChild(confirmLabel);
    
    //验证码输入框
    m_pCodeInputLabel = MLTextFieldTTFLayer::create("Image/BackPack/B_inputBg.png", G_SF(350), G_SF(53), kCCTextAlignmentLeft, "Arial", ccBLACK);
    m_pCodeInputLabel->setPosition(ccp(titleTTF->getPositionX() + G_SF(23), confirmLabel->getPositionY() - G_SF(90)));
    m_pCodeInputLabel->getpTTFCursor()->setnLimitFontCount(11);
    m_pCodeInputLabel->getpTTFCursor()->setpCallObj(this);
//    m_pCodeInputLabel->getpTTFCursor()->setString(verificationCode.c_str());
    m_pBasicBg->addChild(m_pCodeInputLabel,0,10);
    
    Button *sendConfirmBtn = Button::createBtn("Image/BackPack/StampComBtnSmall.png");
    sendConfirmBtn->setPosition(ccp(m_pCodeInputLabel->getPositionX() + m_pCodeInputLabel->getContentSize().width* 0.5 + sendConfirmBtn->getContentSize().width* 0.5 + G_SF(5), m_pCodeInputLabel->getPositionY()));
    sendConfirmBtn->setOnClickCallbackNode(callfuncN_selector(GoodsExchangeMain::SendCodeConfirmCall), this);
    m_pBasicBg->addChild(sendConfirmBtn,0,10);
    
    std::string c_sendTTF = CLChangeString::sharedChangeString()->GetChangeString("CheckCode").c_str();
    CCLabelTTF *btnLabelTTF= CCLabelTTF::create(c_sendTTF.c_str(), "Arial", G_SF(20));
	btnLabelTTF->setPosition(CCSizeZero);
    btnLabelTTF->setColor(ccWHITE);
	sendConfirmBtn->addChild(btnLabelTTF,20);
    
    //关闭
    Button *shutdown = Button::createBtn("Image/PublicUI/Back.png");
    shutdown->setPosition(ccp(G_SF(535),G_SF(735)));
    shutdown->setOnClickCallbackNode(callfuncN_selector(GoodsExchangeMain::menuCallBackButton), this);
    shutdown->setHandlerPriority(-10001);
    m_pBasicBg->addChild(shutdown);
    
    
    std::string c_explain = CLChangeString::sharedChangeString()->GetChangeString("ExchangeMsgInfo").c_str();
    CCLabelTTF *explainTTF= CCLabelTTF::create(c_explain.c_str(), "Arial", G_SF(20), CCSizeMake(G_SF(510), G_SF(70)), kCCTextAlignmentLeft);
    explainTTF->setColor(ccc3(100, 50, 0));
    explainTTF->setPosition(ccp(G_SF(290),G_SF(380)));
    m_pBasicBg->addChild(explainTTF);
    
    frameFlgForNote = 4;
    
}

//确认验证码回调
void GoodsExchangeMain::SendCodeConfirmCall(CCNode* r)
{
    //发送验证码
    //    CCLog("string %s", m_pCodeInputLabel->getpTTFCursor()->getString());
    std::string mCode = m_pCodeInputLabel->getpTTFCursor()->getString();
    
    if (mCode == "") {
        //添加dlg PleaseDeCode
        TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("PleaseDeCode").c_str());
        return;
    }
    
    //发送内容
    Json::Value data;
    data["msgid"].append("90"); //指令码
    data["mphone"] = pPhoneNo;  //购买的卡牌No
    data["mcode"] = mCode;  //验证码
    
    CCLog("sendLog->%s,%s", pPhoneNo.c_str(),mCode.c_str());
    
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_SENDCHECKCODE, data);
    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
}

//发送验证码后回调
void GoodsExchangeMain::_sendCodeBack(std::string verificationCode, std::string phoneNo)
{
    //发送验证码后回调，迁移到兑换主页
    
    m_pBasicBg->removeAllChildrenWithCleanup(true);
    
    //取得手机号
    hadPhoneNo = true;
    
    //默认固定UI
    defaultUI();
    
    //可变UI
    variableUI();
}

//兑换成功后回调
void GoodsExchangeMain::_sureExchangeSuccess(int No,int leavenum)
{
    //商品图片路径
    std::string stamp_path = CCString::createWithFormat("Image/GoodIcon/Stamp%d.png", No)->getCString();
    
    ExchangeSuccLayer *succLayer = ExchangeSuccLayer::createLayer(stamp_path.c_str());
    succLayer->setPosition(ccp(G_SF(0), G_SF(95)));
    this->addChild(succLayer, 30);
    
    //更新奖品展柜信息
    setPrizeList(No,leavenum);
    
}

//wangguolong 更新奖品展柜信息
void GoodsExchangeMain::setPrizeList(int No,int leavenum)
{
    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    
    YHtopLayer* bl = (YHtopLayer*)pMainLayer->getChildByTag(STAMPTAG);
    
    //    int num = bl->getnStockOne()-leavenum;
    int num=pCanExchangeNum;
    
    //兑换商品需要的金印花
    int yhGold = JsonDataManager::shardJDManager()->m_yinHua[No]->m_YH1;
    //兑换商品需要的银印花
    int yhSilver = JsonDataManager::shardJDManager()->m_yinHua[No]->m_YH2;
    
    //当前拥有的金印花
    int yhGoldMe = CLSceneManager::sharedSceneManager()->GetPrizeInfoFromIndex(bl->getnYHCellTag())->getnHaveGoldStampNum();
    //当前拥有的银印花
    int yhSilverMe = CLSceneManager::sharedSceneManager()->GetPrizeInfoFromIndex(bl->getnYHCellTag())->getnHaveSilverStampNum();
    
    //更新当前拥有的金印花
    CLSceneManager::sharedSceneManager()->GetPrizeInfoFromIndex(bl->getnYHCellTag())->setnHaveGoldStampNum(yhGoldMe-yhGold*num);
    //更新当前拥有的银印花
    CLSceneManager::sharedSceneManager()->GetPrizeInfoFromIndex(bl->getnYHCellTag())->setnHaveSilverStampNum(yhSilverMe-yhSilver*num);
    //更新剩余印花数
    CLSceneManager::sharedSceneManager()->GetPrizeInfoFromIndex(bl->getnYHCellTag())->setnStockNum(leavenum);
    
}


bool GoodsExchangeMain::isPhoneNum(std::string phone)
{
    if (phone == "") {
        //添加dlg
        TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("PleaseInputPhone").c_str());
        return false;
    }
    
    if (phone.length() != 11)
    {
        TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("PleaseInputRight").c_str());
        return false;
    }
    else
    {
        
        for (int i = 0; phone[i]!='\0'; i++)
        {
            if (!(phone[i] >= '0' && phone[i] <= '9'))
            {
                TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("PleaseInputRight").c_str());
                return false;
            }
        }
        
        
    }
    return true;
    
}

bool GoodsExchangeMain::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    //弹出层上的继续按钮吞噬所有的触摸
    return true;
}

void GoodsExchangeMain::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);
}

void GoodsExchangeMain::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}




/*********************************************************************************************
 * 兑换成功layer
 *********************************************************************************************/
ExchangeSuccLayer* ExchangeSuccLayer::createLayer(std::string cardPath) //商品图片路径

{
    
    ExchangeSuccLayer *pRet = new ExchangeSuccLayer();
	if (pRet && pRet->init(cardPath))
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

bool ExchangeSuccLayer::init(std::string cardPath) //商品图片路径
{
    
    if ( !CCLayerColor::initWithColor(ccc4(0,0,0,191.25)) )
    {
        return false;
    }
    
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    CCSprite *sTitleLab = CCSprite::create("Image/BackPack/A_ExchangeSucc.png");  //title
    sTitleLab->setPosition(ccp(size.width * 0.5, size.height * 0.75));
    this->addChild(sTitleLab);
    
    CCSprite *sSuccCard = CCSprite::create(cardPath.c_str());
    sSuccCard->setPosition( ccp(size.width * 0.5,size.height * 0.5));
    
    
    //星背景
    CCSprite *sSpr_Star = CCSprite::create("Image/BackPack/card_compound_pft.png");
    sSpr_Star->setPosition(ccp(sSuccCard->getContentSize().width * 0.5, sSuccCard->getContentSize().height * 0.5));
    sSuccCard->addChild(sSpr_Star);
    
    this->addChild(sSuccCard);
    
    
    //继续按钮
    Button *menuAgain = Button::createBtn("Image/BackPack/C_composite.png");
    menuAgain->setPosition(ccp(size.width * 0.5, G_SF(140)));
    menuAgain->setOnClickCallback(menu_selector(ExchangeSuccLayer::okBackButton),this);
    menuAgain->setHandlerPriority(-10003);
    this->addChild(menuAgain,1,3);
    
    CCLabelTTF* lBtnConTTF = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("OkBtn").c_str(), "Arial", G_SF(30), CCSizeMake(G_SF(200), G_SF(35)), kCCTextAlignmentCenter);
    menuAgain->addChild(lBtnConTTF, 10);
    
    
    return true;
}

bool ExchangeSuccLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    //弹出层上的继续按钮吞噬所有的触摸
    return true;
}

void ExchangeSuccLayer::okBackButton(CCNode* r)
{
    
    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
    {
        CCLOG("Error: The cur scene is not the main scene!!!");
        //        return;
    }
    //    YHtopLayer* bl = (YHtopLayer*)pMainLayer->getChildByTag(STAMPTAG);
    
    PrizeShow* prize = PrizeShow::createPShow();
    
    prize->setPosition(ccp(G_SF(-50), G_SF(0)));
    pMainLayer->getChildByTag(STAMPTAG)->addChild(prize);
    
    this->getParent()->removeFromParentAndCleanup(true);
}

void ExchangeSuccLayer::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10002, true);
}

void ExchangeSuccLayer::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}


