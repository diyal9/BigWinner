#include "SpQuestionCardUI.h"
#include "Button.h"
#include "LGameCommonDefine.h"

#include "Shop.h"
#include "CLSceneManager.h"
#include "CLMainSceneLayer.h"
#include "JsonDataManager.h"
#include "CLPlayerManager.h"
#include "DlgTiShiLayer.h"
#include "CLChangeString.h"

bool SpQuestionCardUI::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::init() )
    {
        return false;
    }
    
    getPipeInfo();
    
    return true;
}

void SpQuestionCardUI::getPipeInfo()
{
    
    //添加背景图片
    pLayer2 = CCSprite::create("Image/BackPack/BKShop1.png");
    
    tableView = CCTableView::create(this,getTableSize());
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setPosition(ccp(G_SF(0), G_SF(7)));
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    tableView->setTouchPriority(-10001);//20131020优先级设置
    
    tableView->setDelegate(this);//wangguolong
    
    pLayer2->addChild(tableView,1,11);
    pLayer2->setPosition(ccp(G_SF(370),G_SF(380)));
    this->addChild(pLayer2);
    
	tableView->reloadData();
    
}

/*
 设置TableView的大小
 */
CCSize SpQuestionCardUI::getTableSize()
{
	return CCSizeMake(pLayer2->getContentSize().width,pLayer2->getContentSize().height-G_SF(12));
}

/*
 设置TableView题卡一行的坐标
 */
CCSize SpQuestionCardUI::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(G_SF(230), getTableSize().height/3);
    
}

void SpQuestionCardUI::createTableViewCell(CCNode *cell,CCTableView *table,int index)
{
//    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene && pMainLayer->getnCTIndex() != CLScene_ShelfLayer)
    {
        CCLOG("Error: The cur scene is not the main scene!!!");
        return;
    }
    ShopLayer* bl = (ShopLayer*)pMainLayer->getChildByTag(SHOPTAG);
    
    //卡牌个数
    int nCardNumber = bl->m_bkData["Shop1"].size();
    
    //题卡数目为零时
    if(nCardNumber == 0)
    {
        return;
    }
    
    //tag计数
    int iTag = 0;
    
    iTag = index * 2;
    
    //物品No
    int LeftNo=0;
    //单价
    std::string LeftMoney ;
    //价格类型
    int LeftSalaType = 0;
    int LeftStart = 0;
    
    int ZERO = 0;
    //No
    LeftNo = bl->m_bkData["Shop1"][iTag][ZERO].asIntModestyle();
    
    //单价
    LeftMoney = bl->m_bkData["Shop1"][iTag][2].asStringModeStyle();
    
    //价格类型
    LeftSalaType = bl->m_bkData["Shop1"][iTag][1].asIntModestyle();

    
    //不同星数卡牌数量
    LeftStart = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[LeftNo]->m_artQStarLevel;
    
    //左边题卡图片
    CCSprite *spriteLeft = CCSprite::create("Image/BackPack/CardBackground.png");
    spriteLeft->setPosition(ccp(G_SF(125),G_SF(113.5) ));
    
    //题卡IconId
    int ArtIconId = 0;
    ArtIconId = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[LeftNo]->m_artIconId;
    
    //商品图标路径
    std::string LCardNameIcon_path = CCString::createWithFormat("Image/BackPack/CardIcon%d.png", ArtIconId)->getCString();
    
    //Icon
    CCSprite *pLeftIcon = CCSprite::create(LCardNameIcon_path.c_str());
    pLeftIcon->setPosition(ccp(spriteLeft->getContentSize().width*0.5, spriteLeft->getContentSize().height*0.5));
    spriteLeft->addChild(pLeftIcon);

    //左边题卡星数
    for(int left = 1 ; left <= LeftStart;left++)
    {
        //卡牌星星之间的距离
        CCSprite *spriteStartLeft = CCSprite::create("Image/BackPack/C_card_star.png");
        spriteStartLeft->setPosition( ccp(G_SF(48) + spriteStartLeft->getContentSize().width * 0.5 * ((left-1)*2 + 1), spriteLeft->getContentSize().height - G_SF(55)));
        spriteLeft->addChild(spriteStartLeft);
    }
    
    
    //价格类型
    if(LeftSalaType == 1)
    {
        //钞票
        CCSprite * pSalaType = CCSprite::create("Image/PublicUI/currency.png");
        pSalaType->setPosition(ccp(G_SF(30), G_SF(30)));
        spriteLeft->addChild(pSalaType);
    }
    else if (LeftSalaType == 2)
    {
        //购物卡
        CCSprite * pSalaType = CCSprite::create("Image/PublicUI/ShopCard.png");
        pSalaType->setPosition(ccp(G_SF(30), G_SF(30)));
        spriteLeft->addChild(pSalaType);
    }
    else if (LeftSalaType == 3)
    {
        //人民币
        CCSprite * pSalaType = CCSprite::create("Image/PublicUI/money.png");
        pSalaType->setPosition(ccp(G_SF(30), G_SF(30)));
        spriteLeft->addChild(pSalaType);
    }
    else
    {
        CCLog("其他价格类型！留以后继续加");
    }
    
    //题卡单价
    CCLabelTTF* labelLeft = CCLabelTTF::create(LeftMoney.c_str(), "Arial",  G_SF(18), CCSizeMake(G_SF(72), G_SF(25)), kCCTextAlignmentLeft);
    labelLeft->setPosition(ccp(G_SF(90), G_SF(32)));
    spriteLeft->addChild(labelLeft);
    labelLeft->setColor(ccc3(110, 27, 0));
    
    //添加题卡星数、查看按钮、题卡个数到题卡图片上
    cell->addChild(spriteLeft,1,iTag);
    
    //题卡数量为奇数时，不加载右侧最下面一张题卡
    if((index * 2 + 1) < nCardNumber)
    {
        iTag = 2 * index + 1;
        
        int RightNo = 0;
        int RightStart = 0;
        //价格类型
        int RightSalaType = 0;
        //单价
        std::string RightMoney ;
        
        RightNo = bl->m_bkData["Shop1"][iTag][ZERO].asIntModestyle();
        //不同星数卡牌数量
        RightStart = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[RightNo]->m_artQStarLevel;
        
        //价格类型
        RightSalaType = bl->m_bkData["Shop1"][iTag][1].asIntModestyle();
        
        //单价
        RightMoney = bl->m_bkData["Shop1"][iTag][2].asStringModeStyle();
        
        //右边题卡图片
        CCSprite *spriteRight = CCSprite::create("Image/BackPack/CardBackground.png");
        spriteRight->setPosition(ccp(G_SF(355), G_SF(113.5)));
        
        //道具IconId
        int ArtIconId = 0;
        ArtIconId = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[RightNo]->m_artIconId;
        
        //商品图标路径
        std::string RCardNameIcon_path = CCString::createWithFormat("Image/BackPack/CardIcon%d.png", ArtIconId)->getCString();
        
        //Icon
        CCSprite *pRightIcon = CCSprite::create(RCardNameIcon_path.c_str());
        pRightIcon->setPosition(ccp(spriteRight->getContentSize().width*0.5, spriteRight->getContentSize().height*0.5));
        spriteRight->addChild(pRightIcon);
        
        //右边题卡星数
        for(int right = 1 ; right <= RightStart;right++)
        {
            //卡牌星星之间的距离
            CCSprite *spriteStartRight = CCSprite::create("Image/BackPack/C_card_star.png");
            spriteStartRight->setPosition(  ccp(G_SF(48) + spriteStartRight->getContentSize().width * 0.5 * ((right-1)*2 +1), spriteRight->getContentSize().height - G_SF(55)) );
            spriteRight->addChild(spriteStartRight);
        }
        
        //价格类型
        if(RightSalaType == 1)
        {
            //钞票
            CCSprite * pSalaType = CCSprite::create("Image/PublicUI/currency.png");
            pSalaType->setPosition(ccp(G_SF(30), G_SF(30)));
            spriteRight->addChild(pSalaType);
        }
        else if (RightSalaType == 2)
        {
            //购物卡
            CCSprite * pSalaType = CCSprite::create("Image/PublicUI/ShopCard.png");
            pSalaType->setPosition(ccp(G_SF(30), G_SF(30)));
            spriteRight->addChild(pSalaType);
        }
        else if (RightSalaType == 3)
        {
            //人民币
            CCSprite * pSalaType = CCSprite::create("Image/PublicUI/money.png");
            pSalaType->setPosition(ccp(G_SF(30), G_SF(30)));
            spriteRight->addChild(pSalaType);
        }
        else
        {
            CCLog("其他价格类型！留以后继续加");
        }
        
        //题卡单价
        CCLabelTTF* labelLeft = CCLabelTTF::create(RightMoney.c_str(), "Arial",  G_SF(18), CCSizeMake(G_SF(72), G_SF(25)), kCCTextAlignmentLeft);
        labelLeft->setPosition(ccp(G_SF(90), G_SF(32)));
        spriteRight->addChild(labelLeft);
        labelLeft->setColor(ccc3(110, 27, 0));
        
        //添加题卡星数、查看按钮、题卡个数到题卡图片上
        cell->addChild(spriteRight,1,iTag);
        
    }
    
    
}

//table cell选中
void SpQuestionCardUI::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
//    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene && pMainLayer->getnCTIndex() != CLScene_ShelfLayer)
    {
        CCLOG("Error: The cur scene is not the main scene!!!");
        return;
    }
    ShopLayer* bl = (ShopLayer*)pMainLayer->getChildByTag(SHOPTAG);
    
    //价格类型
    int salaType = 0;
    int ZERO = 0;
    int cardID =0;//No
    int price = 0;//单价
    int buyNumber = 0;//数据库标中商品购买上限
    int haveNumber = 0;//玩家当前拥有数量

    //node是当前cell里面唯一的一个node 并且tag是在tableCellAtIndex方法里面你们定义的tag 把它取出来，接下来就是取左右两个sprite了
    CCNode* node = dynamic_cast<CCNode*>(cell->getChildByTag(1));
    
    //这里是取出左边的sprite 可以拿se在Visible屏蔽的代码测试  具体里面的操作就看你们自己了，这里的tag也是你们再createTableViewCell里面定义的
    CCSprite* left = dynamic_cast<CCSprite*>(node->getChildByTag(cell->getIdx()*2));
    //  left->setVisible(false);
    
    //这里是取出右边的sprite
    CCSprite* right = dynamic_cast<CCSprite*>(node->getChildByTag(cell->getIdx()*2 + 1 ));
    
    CCRect lspRect;
    CCPoint lp;
    CCRect rspRect;
    CCPoint rp;
    if(left != NULL){
        
        //相对于精灵锚点坐标，size
        lspRect = CCRectMake(-left->getContentSize().width * 0.5, -left->getContentSize().height * 0.5, left->getContentSize().width, left->getContentSize().height);
        
        lp = left->convertToNodeSpaceAR(m_touchPoint); //相对于精灵的锚点坐标
    }
    
    if(right != NULL){
        //相对于精灵锚点坐标，size
        rspRect = CCRectMake(-right->getContentSize().width * 0.5, -right->getContentSize().height * 0.5, right->getContentSize().width, right->getContentSize().height);
        
        rp = right->convertToNodeSpaceAR(m_touchPoint); //相对于精灵的锚点坐标
    }
    
    
    //选择左边时，添加使用中
    if(left != NULL && lspRect.containsPoint(lp) ){
        
        n_Tag = left->getTag();

        //选择右边时，添加使用中
    }else if (right != NULL && rspRect.containsPoint(rp)){
        
        n_Tag = right->getTag();

    }else
    {
        CCLog("没有选中任何");
        return;
    }
    
    //价格类型
    salaType = bl->m_bkData["Shop1"][n_Tag][1].asIntModestyle();
    
    cardID = bl->m_bkData["Shop1"][n_Tag][ZERO].asIntModestyle();
    
    price = bl->m_bkData["Shop1"][n_Tag][2].asIntModestyle();
    
    //玩家当前拥有数量
    haveNumber = bl->m_bkData["Shop1"][n_Tag][3].asIntModestyle();
    
    //物品购买个数限制
    buyNumber = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[cardID]->m_BuyLimit;
    
    //用户当前钱币数量
    int userCurrentMoney = 0;
    //钱币为0时 弹出消息
    std::string strMe;
    
    //玩家可购买个数
    int buyMaxNumber = 0;
    
    if(salaType == 1) {
        //钱
        userCurrentMoney = CLPlayerManager::shardPlayerManager()->getpUserInfo()->getMoneys();
        //钱不足,大侠你是个穷屌丝！
        strMe = CLChangeString::sharedChangeString()->GetChangeString("LittleMoney");
        
        //计算钱可购买个数
        if((buyNumber - haveNumber) >= (userCurrentMoney/price))
        {
            buyMaxNumber = userCurrentMoney/price;
            
        }else
        {
            buyMaxNumber = buyNumber - haveNumber;
            //购买限制为无限制
            if(buyMaxNumber <= 0 && buyNumber == 0)
            {
                buyMaxNumber = 99;
            }
        }
        
    }
    else if(salaType == 2)
    {
        
        //购物卡
        userCurrentMoney = CLPlayerManager::shardPlayerManager()->getpUserInfo()->getJetton();
        //购物卡不足,大侠你是个穷屌丝！
        strMe = CLChangeString::sharedChangeString()->GetChangeString("LittleShopCard");
        
        //计算购物卡可购买个数
        if((buyNumber - haveNumber) >= (userCurrentMoney/price))
        {

            buyMaxNumber = userCurrentMoney/price;
            
        }else
        {
            buyMaxNumber = buyNumber - haveNumber;
            //购买限制为无限制
            if(buyMaxNumber <= 0 && buyNumber == 0)
            {
                buyMaxNumber = 99;
            }
        }
    }
    else if(salaType == 3)
    {
        CCLog("人民币");
    }
    else{
        CCLog("其他购买类型");
    }
    
    if(salaType != 3){
        
        //钱不够购买一个商品
        if(userCurrentMoney < price){
            
            TiShiLayer::create(strMe);
        }else{
            CCLog("弹出购买页面");
            
            //可购买个数为0
            if(buyMaxNumber <= 0)
            {
                //该题卡达到购买数量上限！
                TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("QuestionCardUpperLimit"));
                
            }else{
                ShopBuyLayer *lBuy = ShopBuyLayer::createLayer(cardID, salaType ,price, true,buyMaxNumber,"Shop1");
                lBuy->setPosition(ccp(G_SF(-10), G_SF(-10)));
                this->addChild(lBuy,10,SHOPBUYPALYER_TAG);
            }
        }

    }else{
        CCLog("---价格购买类型为3,返回--");
        TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("MoneyShop"));
        return;
    }
    
}

CCTableViewCell* SpQuestionCardUI::tableCellAtIndex(CCTableView *table, unsigned int idx)
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
unsigned int SpQuestionCardUI::numberOfCellsInTableView(CCTableView *table)
{
//    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene && pMainLayer->getnCTIndex() != CLScene_ShelfLayer)
    {
        CCLOG("Error: The cur scene is not the main scene!!!");
        //        return;
    }
    ShopLayer* bl = (ShopLayer*)pMainLayer->getChildByTag(SHOPTAG);
    
    //卡牌个数
    int num = bl->m_bkData["Shop1"].size();
    
    //卡牌个数
    //    int num = 25;
    if(num % 2 == 0)
    {
        return num/2;
        
    }else{
        return (num+1)/2;
        
    }
    
}

void SpQuestionCardUI::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);
}

void SpQuestionCardUI::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool SpQuestionCardUI::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    // 记录触摸起始点的位置
    m_touchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
    
    return true;
}


void SpQuestionCardUI::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void SpQuestionCardUI::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}



