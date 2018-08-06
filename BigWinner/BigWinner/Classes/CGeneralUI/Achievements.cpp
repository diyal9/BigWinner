//
//  Achievements.cpp
//  BigWinner
//
//  Created by YinYanlong on 13-11-13.
//
//

#include "Achievements.h"
#include "LGameCommonDefine.h"
#include "Button.h"
#include "FightBackPack.h"
#include "CLSceneManager.h"
#include "CLPlayerManager.h"
#include "JsonDataManager.h"
#include "DProgressBar.h"

#include "CLPveSMangerLayer.h"
#include "CLChangeString.h"
#include "CLSceneManager.h"
#include "CLNetWorkManager.h"
#include "DlgTiShiLayer.h"
#include "RichLabel.h"
#include "CLMainSceneLayer.h"
#include "CLSoundManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace Json;

static int oneCellheight = 124;

CLAchievements::CLAchievements()
{
    
}

CLAchievements::~CLAchievements()
{
    
}

CLAchievements* CLAchievements::create(Json::Value &data)
{
    CLAchievements *pRet = new CLAchievements();
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

bool CLAchievements::init(Json::Value &data)
{
    if(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0)))
    {
        return false;
    }
    
    //窗体开闭音乐
    CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_WInClose.wav");
    
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    m_AchiData = data;
    
    int CLASSINDEX = 0; //成就类别
    int NUMINDEX = 1; //每类答对数量
    for (int i = 0; i< data["achieve"].size(); i++)
    {
        if (data["achieve"][i].size() == 0) {
            continue; //返回的成就信息为空处理
        }
        
        std::string achiClass = data["achieve"][i][CLASSINDEX].asStringModeStyle();
        int rightByClass = data["achieve"][i][NUMINDEX].asIntModestyle();
        
        rightNumByClassMap[achiClass] = rightByClass;
        
        for (int k = 2; k < data["achieve"][i].size(); k++) {
            int gettedNo = data["achieve"][i][k].asIntModestyle();
            if (gettedNo != 0) {
                notGetMap.push_back(gettedNo);
            }
            
        }
        
    }
    
    //初始化UI
    CCSprite *basicBg = CCSprite::create("Image/BackPack/BKShop2.png");
    this->addChild(basicBg);
    basicBg->setPosition(ccp(size.width*0.5, size.height*0.43));
    //    basicBg->setPosition(ccp(size.width*0.5, size.height*0.43 * (-1))); //初始设定在屏幕之外
    
    doAction(basicBg, true);
    
    
    m_pTabViewBgPic = CCSprite::create("Image/BackPack/A_achiBg.png");
    basicBg->addChild(m_pTabViewBgPic);
    m_pTabViewBgPic->setPosition(ccp(basicBg->getContentSize().width*0.5, basicBg->getContentSize().height*0.47));
    
    //titleBg
    CCSprite *titleSpr = CCSprite::create("Image/PublicUI/TitleBackground.png");
    titleSpr->setPosition(ccp(G_SF(75), G_SF(765)));
    basicBg->addChild(titleSpr,50);
    
    //title
    CCSprite *bbwenzi = CCSprite::create("Image/BackPack/Achi_Title.png");
    bbwenzi->setPosition(ccp(G_SF(80), G_SF(25)));
    titleSpr->addChild(bbwenzi,100);
    
    
    //关闭
    Button *shutdown = Button::createBtn("Image/PublicUI/Close.png");
    shutdown->setPosition(ccp(G_SF(551), G_SF(766)));
    shutdown->setOnClickCallbackNode(callfuncN_selector(CLAchievements::menuCallBackButton), this);
    shutdown->setHandlerPriority(-10001);
    basicBg->addChild(shutdown);
    
    getPipeInfo();
    
    return true;
}

//动作执行
void CLAchievements::doAction(CCNode* r, bool moveIn)
{
    if (moveIn) //进场动作
    {
        CCFiniteTimeAction *actionOut = CCSequence::create(CCFadeTo::create(0.4f, 255*0.5), NULL); //透明度动画
        this->runAction(actionOut);
        
        r->setPosition(r->getPositionX(), r->getPositionY() * (-1)); //反向设置position
        CCAction *actionMoveOut = CCEaseExponentialOut::create(CCMoveTo::create(0.4f, ccp(r->getPositionX(), r->getPositionY() * (-1)))); //快到慢
        r->runAction(actionMoveOut);
    }
    else //出场动作
    {
        CCFiniteTimeAction *actionIn = CCSequence::create(CCFadeTo::create(0.4f, 0), NULL); //透明度动画
        this->runAction(actionIn);
        
        CCAction *actionMoveIn = CCEaseExponentialIn::create(CCMoveTo::create(0.4f, ccp(r->getPositionX(), r->getPositionY() * (-1)))); //慢到快
        r->runAction(actionMoveIn);
    }
    
}

//tableview刷新
void CLAchievements::getPipeInfo()
{
    if (m_pTabViewBgPic)
        m_pTabViewBgPic->removeAllChildrenWithCleanup(true);
    
	m_pTabView = CCTableView::create(this,getTableSize());
	m_pTabView->setDirection(kCCScrollViewDirectionVertical);
    
    //卡片跟背景左右之间的间隙（tableview背景图－2 ＊ 题卡宽度）/ 2
    float blankWidthSize = (m_pTabViewBgPic->getContentSize().width - cellSizeForTable(m_pTabView).width) * 0.5;
    
    m_pTabView->setPosition(ccp(G_SF(blankWidthSize), G_SF(15)));
	m_pTabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_pTabView->setTouchPriority(-10002);
    
    m_pTabView->setDelegate(this);  //将tableview的触摸代理赋给layer
    m_pTabViewBgPic->addChild(m_pTabView,1,1);
	m_pTabView->reloadData();
    
}

void CLAchievements::menuCallBackButton(CCNode* r)
{
    //窗体开闭音乐
    CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_WInClose.wav");
    
    doAction(r->getParent(), false);
    
    this->removeFromParentAndCleanup(true);
}

/* 设置table的大小 */
CCSize CLAchievements::getTableSize()
{
	return CCSizeMake(m_pTabViewBgPic->getContentSize().width - G_SF(5), m_pTabViewBgPic->getContentSize().height - G_SF(20));
}

/* 设置cell大小 */
CCSize CLAchievements::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(getTableSize().width, G_SF(oneCellheight) + G_SF(5)); //cell高，加上cell之间的间隙
}

void CLAchievements::createTableViewCell(CCNode *cell,cocos2d::extension::CCTableView *table,int index)
{
    int ZERO = 0;
    if (index == 0) {
        index = ZERO;
    }
    
    int achieveNo = index + 1;
    std::string achiClass = JsonDataManager::shardJDManager()->m_vAchievement[achieveNo]->m_pAchiClass; //成就note
    
    int achieveNum = mapKeyGetValue(rightNumByClassMap, achiClass); //成就No对应的类别答对的总数
    bool achieveFlg = false;
    if (hadNotGetted(notGetMap, achieveNo)) {
        achieveFlg = true;//没领取
    };
    
    //    CCLog("cell createInfo->%d,%d,%d,%d",index,achieveNo,achieveNum, achieveFlg);
    
    CLAchievementCell* pRoom = CLAchievementCell::create(index,achieveNo, achieveNum, achieveFlg);
    pRoom->setPosition(ccp(getTableSize().width*0.5, G_SF(oneCellheight*0.5)));
    cell->addChild(pRoom);
}

//cell选中后处理
void CLAchievements::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    //	CCLOG("选中了: %i", cell->getIdx());
}

/* 设定每一个cell */
CCTableViewCell* CLAchievements::tableCellAtIndex(CCTableView *table, unsigned int idx)
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

unsigned int CLAchievements::numberOfCellsInTableView(CCTableView *table)
{
    return JsonDataManager::shardJDManager()->m_vAchievement.size();
}


void CLAchievements::onEnter()
{
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);
}

void CLAchievements::onExit()
{
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool CLAchievements::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(this->isVisible())
        return true;
    else
        return false;
}

/* 判断是否兑换（true：未兑换） */
bool CLAchievements::hadNotGetted(std::vector<int> vect, int key)
{
    for(unsigned int i = 0;i<vect.size();++i)
    {
        if(vect[i]==key)
        {
            return true; //未兑换
        }
    }
    return false;
}

/* 根据key取得Map的值） */
int CLAchievements::mapKeyGetValue(std::map<std::string, int> map, std::string key)
{
    std::map<std::string, int>::iterator it = map.find(key);
    if (it != map.end()) {
        return (*it).second;
        
    } else {
        //        return NULL;
        return 0;
        //找不到返回0
    }
    
}

void CLAchievements::doActionOne(int No)
{
    CLSceneManager::sharedSceneManager()->DelKey(No);
    
    std::string prizeType = JsonDataManager::shardJDManager()->m_vAchievement[No]->m_pAchiPrizeType; //奖励类别

    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
    {
        CCLOG("Error: achieve page can not see the main scene!!!");
    }
    
    //成就动画 区分ICON
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    std::string sStarPicFile = "Image/PublicUI/attrIcon0"+prizeType+".png";
    CCSprite* icon = CCSprite::create(sStarPicFile.c_str());
    icon->setScale(0.1f);
    pMainLayer->addChild(icon,100000,111);
    icon->setPosition(ccp(size.width*0.55, size.height*0.5));
    
    CCActionInterval* acOne = CCScaleTo::create(0.6f, 1);
    CCActionInterval* acTwo = CCFadeOut::create(0.6f);
    CCActionInterval* acThr = CCMoveTo::create(0.8f, ccp(size.width*0.6,size.height * 1.2));
    CCActionInterval* acSca = CCScaleTo::create(0.6f, 0.1);
    CCSpawn* acFour = CCSpawn::create(acTwo, acThr, acSca, NULL);
    CCCallFunc * func = CCCallFunc::create(this, callfunc_selector(CLAchievements::doActionTwo));
    CCSequence * sequenece = CCSequence::create(acOne, acFour, func,NULL);
    
    icon->runAction(sequenece);
}
void CLAchievements::doActionTwo()
{
    this->removeChildByTag(111, true);
    
    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
    {
        CCLOG("Error: achieve page can not see the main scene!!!");
    }
    
    //修改用户信息
    if (m_getAfter.isMember("moneys"))
    {
        int money = m_getAfter["moneys"].asIntModestyle();
        CLPlayerManager::shardPlayerManager()->getpUserInfo()->setJetton(money); //钱
    }
    
    if (m_getAfter.isMember("jetton"))
    {
        int card = m_getAfter["jetton"].asIntModestyle();
        CLPlayerManager::shardPlayerManager()->getpUserInfo()->setMoneys(card);  //购物卡
    }
    
    CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_GetGoods.wav");
    
    pMainLayer->getpMoneyFont()->setString(CCString::createWithFormat("%d", CLPlayerManager::shardPlayerManager()->getpUserInfo()->getMoneys())->getCString());
    
    pMainLayer->getpCardFont()->setString(CCString::createWithFormat("%d", CLPlayerManager::shardPlayerManager()->getpUserInfo()->getJetton())->getCString());
    
    
    pMainLayer->UpdateNewAchi();
    
}

void CLAchievements::_getSuccessBack(Json::Value& jv)
{
    m_getAfter = jv;
    int pAchiNo = jv["No"].asIntModestyle();
    doActionOne(pAchiNo);
    
    //    CCLog("领取(%d)成功回调函数", pAchiNo);
    //    hadGetMap.erase(pAchiNo);//领取成功后在MAP中添加该条记录
    for(std::vector<int>::iterator it=notGetMap.begin(); it!=notGetMap.end(); )
    {
        if(* it == pAchiNo)
        {
            it = notGetMap.erase(it); //不能写成arr.erase(it);
        }
        else
        {
            ++it;
        }
    }
    
    TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("GetAchiSucc"));
    
    getPipeInfo();//刷新成就UI
    
}



/**
 *  成就列表cell类
 */
CLAchievementCell::CLAchievementCell()
{
    m_pAchieveInfo = NULL;
}

CLAchievementCell::~CLAchievementCell()
{
    
}

CLAchievementCell* CLAchievementCell::create(int nIndex, int achiNo, int nowAchiNum, bool aflg)
{
    CLAchievementCell* pCell = new CLAchievementCell();
    if(pCell && pCell->init(nIndex, achiNo, nowAchiNum, aflg))
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

bool CLAchievementCell::init(int nIndex, int achiNo, int nowAchiNum, bool aflg)
{
    
    //本地JSON数据
    std::string achiName = JsonDataManager::shardJDManager()->m_vAchievement[achiNo]->m_pAchiName; //成就名称
    std::string achiNote = JsonDataManager::shardJDManager()->m_vAchievement[achiNo]->m_pAchiNote; //成就note
    int achiValue = JsonDataManager::shardJDManager()->m_vAchievement[achiNo]->m_pAchiValue; //成就达成条件
    std::string prizeType = JsonDataManager::shardJDManager()->m_vAchievement[achiNo]->m_pAchiPrizeType; //奖励类别
    int prizeNum = JsonDataManager::shardJDManager()->m_vAchievement[achiNo]->m_pAchiPrizeNum; //奖励数量
    
    std::string achiClassType = JsonDataManager::shardJDManager()->m_vAchievement[achiNo]->m_pAchiClass; //成就类型
    int achiLevel = JsonDataManager::shardJDManager()->m_vAchievement[achiNo]->m_pAchiLevel; //成就类型
    
    //cell背景
    if(!initWithFile("Image/BackPack/A_achiCellBg.png"))
        return false;
    
    //成就ICon
    std::string sPicFile = "Image/BackPack/AchiIcon_" + achiClassType + ".png";
    
    CCSprite *achiBg = CCSprite::create(sPicFile.c_str());
    this->addChild(achiBg);
    achiBg->setPosition(ccp(achiBg->getContentSize().width * 0.6, this->getContentSize().height * 0.5));

    std::string sStarPicFile = CCString::createWithFormat("Image/BackPack/AchStar_%d.png",achiLevel)->getCString();
    CCSprite *achiStar = CCSprite::create(sStarPicFile.c_str());
    achiBg->addChild(achiStar);
    achiStar->setPosition(ccp(achiBg->getContentSize().width * 0.5, G_SF(17)));
    
    //成就名称
    std::string c_achiName = achiName;
    
    CCLabelTTF *pAchiName= CCLabelTTF::create(c_achiName.c_str(), "Arial", G_SF(30));
	this->addChild(pAchiName);
    pAchiName->setAnchorPoint(CCPointZero);
    pAchiName->setColor(ccBLACK);
    pAchiName->setPosition(ccp(achiBg->getPositionX() + achiBg->getContentSize().width*0.5 + G_SF(15), this->getContentSize().height*0.5));
    
    //答对label
    std::string c_residue = achiNote;
    CCLabelTTF *tResidue= CCLabelTTF::create(c_residue.c_str(), "Arial", G_SF(20));
    tResidue->setAnchorPoint(CCPointZero);
	tResidue->setPosition(ccp(achiBg->getPositionX() + achiBg->getContentSize().width*0.5 + G_SF(15), this->getContentSize().height*0.25));
    tResidue->setColor(ccBLACK);
	this->addChild(tResidue);
    
    if (!aflg && (nowAchiNum >= achiValue)) {   //领取过
        
        CCSprite *hadChangeed = CCSprite::create("Image/BackPack/A_achiGetted.png");
        this->addChild(hadChangeed);
        hadChangeed->setPosition(ccp(G_SF(480), this->getContentSize().height*0.35));
        
    }
    else
    { //没取过
        CCSprite *prizeIcon;
        if (prizeType == "1") {
            prizeIcon = CCSprite::create("Image/LoginLayer/ML_CardIcon.png");
        }
        else if
            (prizeType == "2")
        {
            prizeIcon = CCSprite::create("Image/LoginLayer/ML_MoneyIcon.png");
        }
        this->addChild(prizeIcon);
        prizeIcon->setPosition(ccp(G_SF(447), G_SF(90)));
        
        //数量
        char str[20] = {0};
        sprintf(str, "x%u", prizeNum);
        CCLabelTTF *ttfPro= CCLabelTTF::create(str, "Arial", G_SF(20), CCSizeMake(G_SF(80), G_SF(28)), kCCTextAlignmentLeft);
        ttfPro->setColor(ccBLACK);
        ttfPro->setPosition(ccp(G_SF(525), G_SF(85)));
        this->addChild(ttfPro);
        
        if (nowAchiNum >= achiValue) { //现在答题数大于该成就的标准
            
            Button *exchangeed = Button::createBtn("Image/BackPack/A_achiToGet.png");
            exchangeed->setPosition(ccp(G_SF(480), this->getContentSize().height*0.25));
            exchangeed->setOnClickCallbackNode(callfuncN_selector(CLAchievementCell::menuCallBackGet), this);
            exchangeed->setHandlerPriority(-10003);
            this->addChild(exchangeed, 1, achiNo); //把成就No设置成TAG，领取的时候使用

        }
        else
        {
            DProgressBar *processBar = DProgressBar::create("Image/BackPack/A_achiProBg.png", "Image/BackPack/A_achiProPro.png");
            processBar->setPosition(ccp(G_SF(480), this->getContentSize().height*0.25));
            processBar->setMinimumValue(0); //设置最小值
            processBar->setMaximumValue(achiValue); //设置最大值
            processBar->setValue(nowAchiNum); //设置进度值
            
            std::string c_Str1 = CCString::createWithFormat("%d/%d",nowAchiNum,achiValue)->getCString();
            
            //创建一个跟进度条一般大小的标签，文字居中
            CCLabelTTF *ttfPro= CCLabelTTF::create(c_Str1.c_str(), "Arial", G_SF(22));
            ttfPro->setColor(ccBLACK);
            ttfPro->setPosition(ccp(processBar->getContentSize().width * 0.5, processBar->getContentSize().height* 0.45));
            processBar->addChild(ttfPro);
            
            this->addChild(processBar);
            
        }
    }
    
    
    return true;
}

//领取回调
void CLAchievementCell::menuCallBackGet(CCNode* r)
{
    //发送内容
    Value data;
    data["msgid"].append(CMD_GETACHIEVE); //指令码
    data["No"] = r->getTag();  //兑换的成就No
    
    //    CCLog("send(%d)successLog", r->getTag());
    
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_GETACHIEVE, data);
    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
}




/**************************************************
 * 主动推送成就弹出框（仅主场景）
 *
 * 主场景 根据CLSceneManager的Map，值为0的就弹出。说明为对战，闯关中主动推送的成就
 * 每次进来遍历MAP中值为0的显示，显示完后，将该值设置为1
 *
 * Context:  使用m_vShowMsgVect记录单例中m_iNotGetAchiList的闯关对战中的成就，并将其值全设为1，说明已经在主场景中显示过了。
 * UICreate()函数为成就中变动的UI编辑。
 * nowShowIndex为遍历m_vShowMsgVect的索引，在每次调用UICreate函数后自增（用做是否还有成就显示的逻辑辨别符）。
 * onGetPress函数，领取按钮的回调。只负责给服务器发送领取请求
 * closeMenuPress函数为关闭按钮按下回调，只根据nowShowIndex判断是否还有成就可显示。（小于m_vShowMsgVect的size）
 * _getAchiSucc函数 为领取成功后回调。里面调用执行动作。
 * doActionTwo函数  动作一结束后，修改可见区域的用户信息。并且判断领取后是否还存在可显示的成就
***************************************************/
CLAchiReachLayer::CLAchiReachLayer()
{
    m_pWriteBg = NULL;
    iAcniNo = 0;
}

CLAchiReachLayer::~CLAchiReachLayer()
{
    
}

CLAchiReachLayer* CLAchiReachLayer::create()
{
    CLAchiReachLayer* pItem = new CLAchiReachLayer();
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

bool CLAchiReachLayer::init()
{
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    if(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 100), size.width, size.height))
        return false;
    
    nowShowIndex = 0;
    
    //取得成就队列
    int i = 0;
    std::map<int, int>::iterator  it = CLSceneManager::sharedSceneManager()->m_iNotGetAchiList.begin();
    for(;it!=CLSceneManager::sharedSceneManager()->m_iNotGetAchiList.end();++it)
    {

        if (it->second == 0)
        {
            m_vShowMsgVect.push_back(it->first);
            CLSceneManager::sharedSceneManager()->logicRemove(it->first);
        }
        i++;
    }
    
    CCSprite* pBgSpr = CCSprite::create("Image/BackPack/GameVolumeBG01.png");
    pBgSpr->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(pBgSpr, 1);
    
    CCSprite* pTTFBG = CCSprite::create("Image/PveLayer/CP_GiveUpTTFBG.png");
    pTTFBG->setAnchorPoint(ccp(0, 1));
    pTTFBG->setPosition(ccp(G_SF(45), pBgSpr->getContentSize().height));
    pBgSpr->addChild(pTTFBG, 1);

    CCSprite* pGiveTTF = CCSprite::create("Image/BackPack/AchiGetTItle.png");
    pGiveTTF->setPosition(ccp(pTTFBG->getContentSize().width/2, G_SF(39)));
    pTTFBG->addChild(pGiveTTF);
    
    //白色背景
    m_pWriteBg = CCSprite::create("Image/BackPack/GameVolumeBG02.png");
    m_pWriteBg->setPosition(ccp(pBgSpr->getContentSize().width*0.5, pBgSpr->getContentSize().height*0.56));
    pBgSpr->addChild(m_pWriteBg, 1);
    
    CCMenu* pMenu = CCMenu::create();
    pMenu->setAnchorPoint(CCPointZero);
    pMenu->setPosition(CCPointZero);
    pMenu->setTouchPriority(-1000001);
    pBgSpr->addChild(pMenu, 1);
    
    
    //关闭
    Button *shutdown = Button::createBtn("Image/PublicUI/Close.png");
    shutdown->setPosition(ccp(G_SF(550), G_SF(490)));
    shutdown->setOnClickCallbackNode(callfuncN_selector(CLAchiReachLayer::closeMenuPress), this);
    shutdown->setHandlerPriority(-1000001);
    pBgSpr->addChild(shutdown);
    
    //确定
    Button *okBtn = Button::createBtn("Image/PublicUI/CommonIcon_Ok.png");
    okBtn->setPosition(ccp(G_SF(440), G_SF(83)));
    okBtn->setOnClickCallbackNode(callfuncN_selector(CLAchiReachLayer::onGetPress), this);
    okBtn->setHandlerPriority(-1000001);
    pBgSpr->addChild(okBtn);
    
    CCLabelTTF* pLabel = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("ToGet").c_str(), "Arial", G_SF(40));
    pLabel->setPosition(ccp(0, 0));
    okBtn->addChild(pLabel, 1);
    
    UICreate(); //中间部分类容
    
    return true;
}

void CLAchiReachLayer::UICreate()  //白色背景类容填充
{
    m_pWriteBg->removeAllChildrenWithCleanup(true);
    
    if (nowShowIndex < m_vShowMsgVect.size())
    {
        iAcniNo = m_vShowMsgVect[nowShowIndex]; //取得当前容器中的第一个元素显示
    }

    std::string achiName = JsonDataManager::shardJDManager()->m_vAchievement[iAcniNo]->m_pAchiName; //成就名称
    std::string achiNote = JsonDataManager::shardJDManager()->m_vAchievement[iAcniNo]->m_pAchiNote; //成就note
    //    int achiValue = JsonDataManager::shardJDManager()->m_vAchievement[iAcniNo]->m_pAchiValue; //成就达成条件
    std::string prizeType = JsonDataManager::shardJDManager()->m_vAchievement[iAcniNo]->m_pAchiPrizeType; //奖励类别
    int prizeNum = JsonDataManager::shardJDManager()->m_vAchievement[iAcniNo]->m_pAchiPrizeNum; //奖励数量
    
    std::string achiClassType = JsonDataManager::shardJDManager()->m_vAchievement[iAcniNo]->m_pAchiClass; //成就类型
    int achiLevel = JsonDataManager::shardJDManager()->m_vAchievement[iAcniNo]->m_pAchiLevel; //成就类型
    
    //成就ICon
    std::string sPicFile = "Image/BackPack/AchiIcon_" + achiClassType + ".png";
    CCSprite *achiBg = CCSprite::create(sPicFile.c_str());
    m_pWriteBg->addChild(achiBg);
    achiBg->setPosition(ccp(G_SF(80), G_SF(215)));
    
    //级别
    std::string sStarPicFile = CCString::createWithFormat("Image/BackPack/AchStar_%d.png",achiLevel)->getCString();
    CCSprite *achiStar = CCSprite::create(sStarPicFile.c_str());
    achiBg->addChild(achiStar);
    achiStar->setPosition(ccp(achiBg->getContentSize().width * 0.5, G_SF(17)));
    
    
    //成就名称
    std::string c_achiName = achiName;
    CCLabelTTF *pAchiName= CCLabelTTF::create(c_achiName.c_str(), "Arial Bold", G_SF(40));
	m_pWriteBg->addChild(pAchiName);
    pAchiName->setAnchorPoint(CCPointZero);
    pAchiName->setColor(ccRED);
    pAchiName->setPosition(ccp(G_SF(150), G_SF(200)));
    
    
    //答对label
    std::string reachLab = CLChangeString::sharedChangeString()->GetChangeString("ReachStr") + achiNote;
    CCLabelTTF *labelReach = CCLabelTTF::create(reachLab.c_str(), "Arial", G_SF(30));
    labelReach->cocos2d::CCNode::setAnchorPoint(CCPointZero);
    labelReach->setPosition(ccp(G_SF(150), G_SF(140)));
    labelReach->setColor(ccc3(59, 0, 0));
    m_pWriteBg->addChild(labelReach);
    
    //奖励
    std::string prizeStr = CLChangeString::sharedChangeString()->GetChangeString("PrizeStr");
    CCLabelTTF *prizeLab = CCLabelTTF::create(prizeStr.c_str(), "Arial", G_SF(30));
    prizeLab->cocos2d::CCNode::setAnchorPoint(CCPointZero);
    prizeLab->setPosition(ccp(G_SF(150), G_SF(70)));
    prizeLab->setColor(ccc3(59, 0, 0));
    m_pWriteBg->addChild(prizeLab);
    
    
    CCSprite *prizeIcon;
    if (prizeType == "1") {
        prizeIcon = CCSprite::create("Image/LoginLayer/ML_CardIcon.png");
    }
    else if
        (prizeType == "2")
    {
        prizeIcon = CCSprite::create("Image/LoginLayer/ML_MoneyIcon.png");
    }
    m_pWriteBg->addChild(prizeIcon);
    prizeIcon->setPosition(ccp(G_SF(320), G_SF(88)));
    
    std::string c_money =  "x" ;
    c_money += CCString::createWithFormat("%d", prizeNum)->getCString();
    
    CCLabelTTF *moneyLable = CCLabelTTF::create(c_money.c_str(), "Arial", G_SF(25));
	moneyLable->setPosition(ccp(G_SF(380), G_SF(75)));
    moneyLable->setColor(ccRED);
	m_pWriteBg->addChild(moneyLable);
    
    nowShowIndex++; //未领取队列Index自增，说明要判断下一个了
}

//确认按钮回调
void CLAchiReachLayer::onGetPress(CCObject* pSender)
{
    //发送当前领取内容
    Value data;
    data["msgid"].append(CMD_GETACHIEVE); //指令码
    data["No"] = iAcniNo;  //兑换的成就No

    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_GETACHIEVE, data);
    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
    
}

//取消按钮回调
void CLAchiReachLayer::closeMenuPress()
{
    if (nowShowIndex < m_vShowMsgVect.size())
    {
        UICreate();
    }
    else
    {
        deleteFromParent();
    }
}

//取消按钮回调
void CLAchiReachLayer::deleteFromParent()
{
    this->removeFromParentAndCleanup(true);
}

void CLAchiReachLayer::_getAchiSucc(Json::Value& jv)
{
    CLSceneManager::sharedSceneManager()->DelKey(iAcniNo); //领取成功后将存在单例中的未领取的成就No删掉
    
    m_AData = jv;
    
    doActionOne(); //领取成功后播放动画
}

void CLAchiReachLayer::onEnter()
{
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);
}

void CLAchiReachLayer::onExit()
{
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}


bool CLAchiReachLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(this->isVisible())
        return true;
    else
        return false;
}

void CLAchiReachLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CLAchiReachLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
}

void CLAchiReachLayer::doActionOne()
{
    //成就动画 区分ICON
    std::string prizeType = JsonDataManager::shardJDManager()->m_vAchievement[iAcniNo]->m_pAchiPrizeType; //奖励类别
    
    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
    {
        CCLOG("Error: achieve page can not see the main scene!!!");
    }
    
    //成就动画 区分ICON
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    std::string sStarPicFile = "Image/PublicUI/attrIcon0"+prizeType+".png";
    CCSprite* icon = CCSprite::create(sStarPicFile.c_str());
    icon->setScale(0.1f);
    pMainLayer->addChild(icon,100000,111);
    icon->setPosition(ccp(size.width*0.55, size.height*0.5));
    
    CCActionInterval* acOne = CCScaleTo::create(0.6f, 1);
    CCActionInterval* acTwo = CCFadeOut::create(0.6f);
    CCActionInterval* acThr = CCMoveTo::create(0.8f, ccp(size.width*0.6,size.height * 1.2));
    CCActionInterval* acSca = CCScaleTo::create(0.6f, 0.1);
    CCSpawn* acFour = CCSpawn::create(acTwo, acThr, acSca, NULL);
    CCCallFunc * func = CCCallFunc::create(this, callfunc_selector(CLAchiReachLayer::doActionTwo));
    CCSequence * sequenece = CCSequence::create(acOne, acFour, func,NULL);
    
    icon->runAction(sequenece);
    
}

void CLAchiReachLayer::doActionTwo()
{
    this->removeChildByTag(111, true);
    
    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
    {
        CCLOG("Error: achieve page can not see the main scene!!!");
    }
    
    //修改用户信息
    if (m_AData.isMember("moneys"))
    {
        int money = m_AData["moneys"].asIntModestyle();
        CLPlayerManager::shardPlayerManager()->getpUserInfo()->setJetton(money); //卡
    }
    
    if (m_AData.isMember("jetton"))
    {
        int card = m_AData["jetton"].asIntModestyle();
        CLPlayerManager::shardPlayerManager()->getpUserInfo()->setMoneys(card);  //钱
    }

    CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_GetGoods.wav");
    
    pMainLayer->getpMoneyFont()->setString(CCString::createWithFormat("%d", CLPlayerManager::shardPlayerManager()->getpUserInfo()->getMoneys())->getCString());
    
    pMainLayer->getpCardFont()->setString(CCString::createWithFormat("%d", CLPlayerManager::shardPlayerManager()->getpUserInfo()->getJetton())->getCString());
    
    pMainLayer->UpdateNewAchi();
    
    if (nowShowIndex < m_vShowMsgVect.size())
    {
        UICreate();
    }
    else
    {
        deleteFromParent();
    }
}