//
//  StampFightSelected.cpp
//  DiyalStudy
//
//  Created by YinYanlong on 13-10-27.
//
//

#include "StampFightSelected.h"
#include "Button.h"
#include "CLSceneManager.h"
#include <LGameCommonDefine.h>
#include "CLNetWorkManager.h"
#include "CLPvpReady.h"
#include "CLMainSceneLayer.h"
#include "CLPlayerManager.h"
#include "CLPvpResultLayer.h"
#include "DlgTiShiLayer.h"
#include "CLChangeString.h"
#include "CLAnimationManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

/**************************************************************************************
 * 对战房间cell创建类
 **************************************************************************************/
CLPvpRoomShowCell::CLPvpRoomShowCell()
{
    m_pPvpRoomInfo = NULL;
}

CLPvpRoomShowCell::~CLPvpRoomShowCell()
{
    
}

CLPvpRoomShowCell* CLPvpRoomShowCell::create(int nIndex)
{
    CLPvpRoomShowCell* pCell = new CLPvpRoomShowCell();
    if(pCell && pCell->init(nIndex))
    {
        pCell->autorelease();
        return pCell;
    }
    else
    {
        CC_SAFE_DELETE(pCell);
        return pCell;
    }
}

bool CLPvpRoomShowCell::init(int nIndex)
{
    m_pPvpRoomInfo = CLSceneManager::sharedSceneManager()->GetPvpRoomInfoFromIndex(nIndex);
    if(!m_pPvpRoomInfo)
        return false;
    
    int openTime = CLSceneManager::sharedSceneManager()->getnValuetime();
    
    bool isCanSelect = true; //是否还可以抢夺印花

    //印花属于发放时间
    if (openTime != 0)   //openTime 为印花发放时间，等于0则可以对战
    {
        isCanSelect =false;
    }
    
    //test  测试用
//    if (nIndex == 0) {
//        isCanSelect=false;
//    }
    
    if (isCanSelect)
    {
        if(!initWithFile("Image/PvpLayer/CP_SelRoomBG_D.png"))
            return false;
        
        canSelectCellCreate(nIndex);
    }
    else
    {
        if(!initWithFile("Image/PvpLayer/CP_SelRoomBG_N.png"))
            return false;
        
        canNotSelectCellCreate(nIndex);
    }
    
    return true;
}

void CLPvpRoomShowCell::canSelectCellCreate(int index)
{
    //添加印花图片
    std::string stampSmall_path = CCString::createWithFormat("Image/GoodIcon/StampSmall%d.png", m_pPvpRoomInfo->getpYinhuaInfo()->m_SYSNo)->getCString();
    
    CCSprite *stampPic = CCSprite::create(stampSmall_path.c_str());
    this->addChild(stampPic, 10);
    stampPic->setPosition(ccp(stampPic->getContentSize().width*0.5 + G_SF(10), this->getContentSize().height*0.5));
    
    //印花名称
    std::string c_stampName = m_pPvpRoomInfo->getpYinhuaInfo()->m_Name;
    
    CCLabelTTF *pStampName= CCLabelTTF::create(c_stampName.c_str(), "Arial", G_SF(30),CCSizeMake(G_SF(300), G_SF(50)), kCCTextAlignmentLeft);
	this->addChild(pStampName, 10);
    pStampName->setAnchorPoint(CCPointZero);
    pStampName->setColor(ccBLACK);
    pStampName->setPosition(ccp(stampPic->getPositionX() + stampPic->getContentSize().width*0.5 + G_SF(10), this->getContentSize().height*0.5));
    
    //剩余印花
    std::string c_residue = CLChangeString::sharedChangeString()->GetChangeString("LeastStampStr");
    c_residue += CCString::createWithFormat("%d", m_pPvpRoomInfo->getnReGYinhuaNum())->getCString();
    
    CCLabelTTF *tResidue= CCLabelTTF::create(c_residue.c_str(), "Arial", G_SF(20),CCSizeMake(G_SF(220), G_SF(25)), kCCTextAlignmentLeft);
	tResidue->setPosition(ccp(pStampName->getPositionX(), G_SF(27)));
    tResidue->setAnchorPoint(CCPointZero);
    tResidue->setColor(ccBLACK);
	this->addChild(tResidue, 10);
    
    //正在对战
//    std::string c_fight = CLChangeString::sharedChangeString()->GetChangeString("FightIngStr");
//    c_fight += CCString::createWithFormat("%d", m_pPvpRoomInfo->getnPvpPlayerNum())->getCString();
//    
//    CCLabelTTF *tFight= CCLabelTTF::create(c_fight.c_str(), "Arial", G_SF(20), CCSizeMake(G_SF(220), G_SF(22)), kCCTextAlignmentLeft);
//	tFight->setPosition(ccp(tResidue->getPositionX() + tResidue->getContentSize().width + G_SF(5), tResidue->getPositionY()));
//    tFight->setColor(ccBLACK);
//    tFight->setAnchorPoint(CCPointZero);
//	this->addChild(tFight, 10);
    
}

void CLPvpRoomShowCell::canNotSelectCellCreate(int index)
{
    //商品图片路径
    std::string stampSmall_path = CCString::createWithFormat("Image/GoodIcon/StampSmall%d.png", m_pPvpRoomInfo->getpYinhuaInfo()->m_SYSNo)->getCString();
    
    //添加印花图片
    CCSprite *stampPic = CCSprite::create(stampSmall_path.c_str());
    this->addChild(stampPic);
    stampPic->setPosition(ccp(stampPic->getContentSize().width * 0.5 + G_SF(10), this->getContentSize().height * 0.5));
    
    //印花名称
    std::string c_stampName = m_pPvpRoomInfo->getpYinhuaInfo()->m_Name;
    CCLabelTTF *pStampName= CCLabelTTF::create(c_stampName.c_str(), "Arial", G_SF(30),CCSizeMake(G_SF(300), G_SF(50)), kCCTextAlignmentLeft);
	this->addChild(pStampName, 10);
    pStampName->setAnchorPoint(CCPointZero);
    pStampName->setColor(ccBLACK);
    pStampName->setPosition(ccp(stampPic->getPositionX() + stampPic->getContentSize().width*0.5 + G_SF(10), this->getContentSize().height * 0.5));
    
    //字体提示
    std::string c_residue = CLChangeString::sharedChangeString()->GetChangeString("NewGiveTimeStr");
    CCLabelTTF *tResidue= CCLabelTTF::create(c_residue.c_str(), "Arial", G_SF(20),CCSizeMake(G_SF(500), G_SF(25)), kCCTextAlignmentLeft);
	tResidue->setPosition(ccp(pStampName->getPositionX(), G_SF(21)));
    tResidue->setAnchorPoint(CCPointZero);
    tResidue->setColor(ccBLACK);
	this->addChild(tResidue, 10);
    
}


/**************************************************************************************
 * 对战房间创建类
 **************************************************************************************/
StampFightSelected::StampFightSelected()
{
    m_pTabViewBgPic = NULL;
    m_pTabView = NULL;
    stampCellheight = 135;
}

StampFightSelected::~StampFightSelected()
{
    
}

bool StampFightSelected::init()
{
    if ( !CCLayerColor::initWithColor(ccc4(150,150,150,0)) )
    {
        return false;
    }
    
    //UI加载
    createUI();
    
    return true;
}

void StampFightSelected::createUI()
{
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    //bg
    CCSprite* bg = CCSprite::create("Image/BackPack/StampBackground1.png");
    this->addChild(bg);
    bg->setPosition(ccp(size.width*0.5, size.height*0.485));
    
    //title
    CCSprite *titleBg = CCSprite::create("Image/PublicUI/TitleBackground.png");
    titleBg->setPosition(ccp(G_SF(75), bg->getContentSize().height - titleBg->getContentSize().height * 0.6));
    bg->addChild(titleBg);
    
    CCSprite *titleTTF = CCSprite::create("Image/PvpLayer/FightSelectTitle.png");
    titleTTF->setPosition(ccp(titleBg->getContentSize().width * 0.5, titleBg->getContentSize().height * 0.5));
    titleBg->addChild(titleTTF,1);
    
    //关闭
    Button *shutdown = Button::createBtn("Image/PublicUI/Close.png");
    shutdown->setPosition(ccp(bg->getContentSize().width - shutdown->getContentSize().width * 0.5,
                              bg->getContentSize().height - shutdown->getContentSize().height * 0.55));
    shutdown->setOnClickCallbackNode(callfuncN_selector(StampFightSelected::menuCallBackButton), this);
    shutdown->setHandlerPriority(-10001);
    bg->addChild(shutdown);
    
    //tableview底图添加
    m_pTabViewBgPic = CCSprite::create("Image/BackPack/StampBackground2.png");
    bg->addChild(m_pTabViewBgPic);
    m_pTabViewBgPic->setPosition(ccp(bg->getContentSize().width*0.5, bg->getContentSize().height*0.465));
    
    //tableview创建
    getPipeInfo();
    
}

//tableview创建
void StampFightSelected::getPipeInfo()
{
	m_pTabView = CCTableView::create(this,getTableSize());
	m_pTabView->setDirection(kCCScrollViewDirectionVertical);
    
    //卡片跟背景左右之间的间隙（tableview背景图－2 ＊ 题卡宽度）/ 2
    int blankWidthSize = (m_pTabViewBgPic->getContentSize().width - cellSizeForTable(m_pTabView).width)*0.5;
    
    m_pTabView->setPosition(ccp(G_SF(blankWidthSize), G_SF(15)));
	m_pTabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_pTabView->setTouchPriority(-10002);
    
    m_pTabView->setDelegate(this);  //将tableview的触摸代理赋给layer
    m_pTabViewBgPic->addChild(m_pTabView,1,1);
	m_pTabView->reloadData();
    
    //触摸代理设置
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
    
}

void StampFightSelected::menuCallBackButton(CCNode* r)
{
    //    Json::Value value;
    //    CLPvpSuccLayer *pvpSucc = CLPvpSuccLayer::create(value);
    //    this->addChild(pvpSucc);
    
    
    CLSceneManager::sharedSceneManager()->setpCallTarget(this);
    CLSceneManager::sharedSceneManager()->setpCallFun(callfunc_selector(StampFightSelected::AdvImageLoadEnd));
    CLSceneManager::sharedSceneManager()->BeginLoadAdvImages(CMD_LOGIN_REPLY);
}

void StampFightSelected::AdvImageLoadEnd()              //异步加载图片完成后的回调 一般都是开始切换场景
{
    CLMainSceneLayer* pLayer = CLMainSceneLayer::create();
    pLayer->setAnchorPoint(CCPointZero);
    pLayer->setPosition(CCPointZero);
    CLSceneManager::sharedSceneManager()->ChangeScene(pLayer);
}

/* 设置table的大小 */
CCSize StampFightSelected::getTableSize()
{
	return CCSizeMake(m_pTabViewBgPic->getContentSize().width - G_SF(5), m_pTabViewBgPic->getContentSize().height - G_SF(20));
}

/* 设置cell大小 */
CCSize StampFightSelected::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(getTableSize().width, G_SF(stampCellheight)); //cell高，加上cell之间的间隙
}

void StampFightSelected::createTableViewCell(CCNode *cell,cocos2d::extension::CCTableView *table,int index)
{
    CLPvpRoomShowCell* pRoom = CLPvpRoomShowCell::create(index);
    pRoom->setPosition(ccp(getTableSize().width*0.5, G_SF(stampCellheight) * 0.5));
    cell->addChild(pRoom, 0, index);
}

//cell选中后处理
void StampFightSelected::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	CCLOG("选中了: %i", cell->getIdx());
    
    
    CLPvpRoomShowCell* pCell = (CLPvpRoomShowCell*)cell->getChildByTag(1)->getChildByTag(cell->getIdx());
    
    //test  测试用
//    if (cell->getIdx() == 0)
//    {
//        TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("NoStampMsg"));
//        return;
//    }
    
    int openTime = CLSceneManager::sharedSceneManager()->getnValuetime();
    if (openTime != 0)
    {
        TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("NoStampMsg"));
        return;
    }
    
    Json::Value jv(Json::objectValue);
    jv["gdsno"] = pCell->getpPvpRoomInfo()->getpYinhuaInfo()->m_SYSNo;
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_PK1_JOIN, jv);
    CLPvpReady* pParNode = (CLPvpReady*)this->getParent();
    pParNode->ShowWaitRivalLayer(true);
    
//    //印花数量不足
//    if(CLSceneManager::sharedSceneManager()->getnPair() == 1)
//    {
////        TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("StampNotEnough"));
//        if(this->getChildByTag(100))
//        {
//            this->getChildByTag(100)->stopAllActions();
//            this->getChildByTag(100)->removeFromParentAndCleanup(true);
//        }
//
//    }
//    else
//    {
////        CLPvpReady* pParNode = (CLPvpReady*)this->getParent();
////        pParNode->getpWaitingLayer()->setVisible(true);
//        
//        CCSprite* bg = CCSprite::create("Image/PvpLayer/05.png");
//        bg->setPosition(ccp(this->getContentSize().width*0.5, this->getContentSize().height*0.5));
//        this->addChild(bg,1,100);
//        
//        std::string sFile;
//        sFile = "matching";
//        
//        CCAnimation* animation = CLAnimationManager::sharedAnimationManager()->animationWithName(sFile.c_str(),0.2);
//        CCSprite* magic = CLAnimationManager::sharedAnimationManager()->spriteWithAnimationName(sFile.c_str());
//        magic->setPosition(ccp(this->getContentSize().width*0.5, this->getContentSize().height*0.48));
//        bg->addChild(magic, 10);
//
//        m_pAction = CCAnimate::create(animation);
//        
//        magic->runAction(CCRepeatForever::create(m_pAction));
//    }
    
}

/* 设定每一个cell */
CCTableViewCell* StampFightSelected::tableCellAtIndex(CCTableView *table, unsigned int idx)
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

//根据题卡个数，计算cell个数
unsigned int StampFightSelected::numberOfCellsInTableView(CCTableView *table)
{
    return CLSceneManager::sharedSceneManager()->GetPvpRoomNum();
}

bool StampFightSelected::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}


/**************************************************************************************
 * 排行榜
 **************************************************************************************/
RankingList::RankingList()
{
    m_pTabViewBgPic = NULL;
    m_pTabView = NULL;
    selfInfoSum = 0;
    cellheight = 40;
}

RankingList::~RankingList()
{
    
}

RankingList* RankingList::create(Json::Value &data)
{
    RankingList *pRet = new RankingList();
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

bool RankingList::init(Json::Value &data)
{
    if ( !CCLayerColor::initWithColor(ccc4(150,150,150,0)) )
    {
        return false;
    }
    
    m_RankData = data;
    
    createUI(); //UI编辑
    
    return true;
}

void RankingList::createUI()
{
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    //bg
    CCSprite* m_pBg = CCSprite::create("Image/BackPack/StampBackground1.png");
    this->addChild(m_pBg);
    m_pBg->setPosition(ccp(size.width * 0.5, size.height * 0.485));
    
    //关闭
    Button *shutdown = Button::createBtn("Image/PublicUI/Close.png");
    shutdown->setPosition(ccp(m_pBg->getContentSize().width - shutdown->getContentSize().width * 0.5,
                              m_pBg->getContentSize().height - shutdown->getContentSize().height * 0.55));
    shutdown->setOnClickCallbackNode(callfuncN_selector(RankingList::closeCallBackBtn), this);
    shutdown->setHandlerPriority(-10001);
    m_pBg->addChild(shutdown);
    
    //title
    CCSprite *titleBg = CCSprite::create("Image/PublicUI/TitleBackground.png");
    titleBg->setPosition(ccp(G_SF(75), m_pBg->getContentSize().height - titleBg->getContentSize().height * 0.6));
    m_pBg->addChild(titleBg);
    
    CCSprite *titleTTF = CCSprite::create("Image/PvpLayer/FightRankTitle.png");
    titleTTF->setPosition(ccp(titleBg->getContentSize().width * 0.5, titleBg->getContentSize().height * 0.5));
    titleBg->addChild(titleTTF,1);
    
    //tableview底图添加
    m_pTabViewBgPic = CCSprite::create("Image/PvpLayer/CP_PvpRankBg.png");
    m_pBg->addChild(m_pTabViewBgPic);
    m_pTabViewBgPic->setPosition(ccp(m_pBg->getContentSize().width * 0.5, m_pBg->getContentSize().height * 0.485));
    
    selfInfoListUI(); //自排名
    
    getPipeInfo();//tableview创建
    
}

//tableview刷新
void RankingList::getPipeInfo()
{
    
	m_pTabView = CCTableView::create(this,getTableSize());
	m_pTabView->setDirection(kCCScrollViewDirectionVertical);
    
    //卡片跟背景左右之间的间隙（tableview背景图－2 ＊ 题卡宽度）/ 2
    int blankWidthSize = (m_pTabViewBgPic->getContentSize().width - cellSizeForTable(m_pTabView).width) * 0.5;
    
    m_pTabView->setPosition(ccp(blankWidthSize, G_SF(150)));
	m_pTabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_pTabView->setTouchPriority(-10002);
    
    m_pTabViewBgPic->addChild(m_pTabView,1,1);
	m_pTabView->reloadData();
    
}

/* 设置table的大小 */
CCSize RankingList::getTableSize()
{
	return CCSizeMake(m_pTabViewBgPic->getContentSize().width, G_SF(400));
}

/* 设置cell大小 */
CCSize RankingList::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(getTableSize().width, G_SF(cellheight));
}

void RankingList::createTableViewCell(CCLayerColor *cell,cocos2d::extension::CCTableView *table,int index)
{
    std::string nickName = m_RankData["ranking"][index][6].asStringModeStyle(); //昵称
    std::string integral = m_RankData["ranking"][index][3].asStringModeStyle(); //积分
    float  winNum = m_RankData["ranking"][index][3].asIntModestyle(); //玩家胜场数
    float  failNum = m_RankData["ranking"][index][4].asIntModestyle(); //玩家失败数
    
    //排名
    char str[20] = {0};
    sprintf(str, "%u", index + 1);
    
    CCLabelTTF *pRank= CCLabelTTF::create(str, "Arial", G_SF(20), ccp(G_SF(50), G_SF(25)), kCCTextAlignmentLeft);
	pRank->setPosition(ccp( G_SF(60), G_SF(cellheight * 0.5)));
    pRank->setColor(ccBLACK);
	cell->addChild(pRank);
    
    //昵称
    CCLabelTTF *pNickName = CCLabelTTF::create(nickName.c_str(), "Arial", G_SF(20),ccp(G_SF(160), G_SF(25)), kCCTextAlignmentLeft);
	pNickName->setPosition(ccp(G_SF(220), G_SF(cellheight * 0.5)));
    pNickName->setHorizontalAlignment(kCCTextAlignmentLeft);
    pNickName->setColor(ccBLACK);
	cell->addChild(pNickName);
    
    //积分
    CCLabelTTF *pIntegral = CCLabelTTF::create(integral.c_str(), "Arial", G_SF(20),ccp(G_SF(100), G_SF(25)), kCCTextAlignmentLeft);
	pIntegral->setPosition(ccp(G_SF(417), G_SF(cellheight * 0.5)));
    pIntegral->setColor(ccBLACK);
	cell->addChild(pIntegral);
    
    //胜率
    float winPer;
    if ((winNum + failNum) == 0) {
        winPer = 0;
    }
    else
    {
        winPer = winNum / (winNum + failNum) * 100.00;
    }
    
    char c_WinRate[20] = {0};
    sprintf(c_WinRate, "%.2f", winPer);
    //    CCLog("百分比%s", c_WinRate);
    std::string c_WinRateStr =  c_WinRate ;
    c_WinRateStr.append("%");
    CCLabelTTF *pWinRate = CCLabelTTF::create(c_WinRateStr.c_str() , "Arial", G_SF(20),ccp(G_SF(100), G_SF(25)), kCCTextAlignmentLeft);
	pWinRate->setPosition(ccp(G_SF(535), G_SF(cellheight * 0.5)));
    pWinRate->setColor(ccBLACK);
	cell->addChild(pWinRate);
    
    
}

void RankingList::selfInfoListUI()
{
    //自排名背景添加
    int ZERO = 0;
    
    //绘制自排名区域纹理
    CCLayerColor *cellNode1 = CCLayerColor::create();
    cellNode1->initWithColor(ccc4(213,213,213,255));
    cellNode1->setPosition(ccp(G_SF(0), G_SF(65)));
    cellNode1->setContentSize(ccp(m_pTabViewBgPic->getContentSize().width, G_SF(cellheight)));
    m_pTabViewBgPic->addChild(cellNode1);
    
    int selfId = CLPlayerManager::shardPlayerManager()->getpUserInfo()->getUserNo(); //取得用户NO
    
    //倒取3位判断排名情况
    selfInfoSum = 3; //取后几条信息
    int cellIndex = 0;
    
    if ( (m_RankData["ranking"][ZERO][1].asIntModestyle() == selfId) ||
        (m_RankData["ranking"][m_RankData["ranking"].size() - 1][1].asIntModestyle() == selfId)
        ) { //取出json数据的第一条的用户ID
        //当前用户排第一
        selfInfoSum = 2;
    }
    
    for (int i = m_RankData["ranking"].size() - selfInfoSum; i < m_RankData["ranking"].size(); i++) { //取后3条数据
        
        std::string rankNo = m_RankData["ranking"][i][ZERO].asStringModeStyle(); //排名
        std::string nickName = m_RankData["ranking"][i][6].asStringModeStyle(); //昵称
        std::string integral = m_RankData["ranking"][i][3].asStringModeStyle(); //积分
        float  winNum = m_RankData["ranking"][i][3].asIntModestyle(); //玩家胜场数
        float  failNum = m_RankData["ranking"][i][4].asIntModestyle(); //玩家失败数
        
        //排名
        CCLabelTTF *pRank= CCLabelTTF::create(rankNo.c_str(), "Arial", G_SF(20), ccp(G_SF(50), G_SF(25)), kCCTextAlignmentLeft);
        pRank->setPosition(ccp( G_SF(60), G_SF(18) + G_SF(33) * (selfInfoSum - cellIndex)));
        pRank->setColor(ccBLACK);
        m_pTabViewBgPic->addChild(pRank, 10);
        
        //昵称
        CCLabelTTF *pNickName = CCLabelTTF::create(nickName.c_str(), "Arial", G_SF(20),ccp(G_SF(160), G_SF(25)), kCCTextAlignmentLeft);
        pNickName->setPosition(ccp(G_SF(220), G_SF(18) + G_SF(33) * (selfInfoSum - cellIndex)));
        pNickName->setColor(ccBLACK);
        pNickName->setHorizontalAlignment(kCCTextAlignmentLeft);
        m_pTabViewBgPic->addChild(pNickName, 10);
        
        //积分
        CCLabelTTF *pIntegral = CCLabelTTF::create(integral.c_str(), "Arial", G_SF(20),ccp(G_SF(100), G_SF(25)), kCCTextAlignmentLeft);
        pIntegral->setPosition(ccp(G_SF(417), G_SF(18) + G_SF(33) * (selfInfoSum - cellIndex)));
        pIntegral->setColor(ccBLACK);
        m_pTabViewBgPic->addChild(pIntegral, 10);
        
        //胜率
        float winPer;
        if ((winNum + failNum) == 0) {
            winPer = 0;
        }
        else
        {
            winPer = winNum / (winNum + failNum) * 100.00;
        }
        
        char c_WinRate[20] = {0};
        sprintf(c_WinRate, "%.2f", winPer);
        //        CCLog("百分比%s", c_WinRate);
        std::string c_WinRateStr =  c_WinRate ;
        c_WinRateStr.append("%");
        CCLabelTTF *pWinRate = CCLabelTTF::create(c_WinRateStr.c_str(), "Arial", G_SF(20),ccp(G_SF(100), G_SF(25)), kCCTextAlignmentLeft);
        pWinRate->setPosition(ccp(G_SF(535), G_SF(18) + G_SF(33) * (selfInfoSum - cellIndex)));
        pWinRate->setColor(ccBLACK);
        m_pTabViewBgPic->addChild(pWinRate, 10);
        
        cellIndex++;
    }
    
}

//cell选中后处理
void RankingList::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    //	CCLOG("选中了: %i", cell->getIdx());
    
}

/* 设定每一个cell */
CCTableViewCell* RankingList::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell *cell = table->dequeueCell();
	if (!cell)
	{
		cell = new CCTableViewCell();
		cell->autorelease();
        
		CCLayerColor *cellNode = CCLayerColor::create();
        if (idx % 2 == 0)
        {
            //白色
            cellNode->initWithColor(ccc4(150,150,150,150));
        }
        else
        {
            //灰色
            cellNode->initWithColor(ccc4(213,213,213,255));
        }
        cellNode->setContentSize(ccp(m_pTabViewBgPic->getContentSize().width, G_SF(cellheight)));
        
        
        cellNode->setAnchorPoint(CCPointZero);
		cellNode->setPosition(CCPointZero);
        
		createTableViewCell(cellNode,table,idx);
		cellNode->setTag(1);
		cell->addChild(cellNode);
	}
	else
	{
		CCLayerColor *cellNode = (CCLayerColor *)cell->getChildByTag(1);
        if (idx % 2 == 0)
        {
            cellNode->initWithColor(ccc4(150,150,150,150));
        }
        else
        {
            cellNode->initWithColor(ccc4(213,213,213,0));
        }
        cellNode->setContentSize(ccp(m_pTabViewBgPic->getContentSize().width, G_SF(cellheight)));
        
		cellNode->removeAllChildrenWithCleanup(true);
		createTableViewCell(cellNode,table,idx);
	}
	return cell;
}

//根据题卡个数，计算cell个数
unsigned int RankingList::numberOfCellsInTableView(CCTableView *table)
{
    return m_RankData["ranking"].size() - selfInfoSum;
    
}

void RankingList::closeCallBackBtn(CCNode* r)
{
    CLPvpReady* pParNode = (CLPvpReady*)getParent();
    pParNode->ReturnRoomSelLayer();
}
