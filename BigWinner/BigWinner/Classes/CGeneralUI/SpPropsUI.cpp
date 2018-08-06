#include "SpPropsUI.h"
#include "Button.h"
#include "LGameCommonDefine.h"

#include "Shop.h"
#include "CLSceneManager.h"
#include "CLMainSceneLayer.h"
#include "JsonDataManager.h"
#include "CLPlayerManager.h"
#include "DlgTiShiLayer.h"
#include "CLChangeString.h"

#define OPENVIEW_TAG 1000

bool SpPropsUI::init()
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

void SpPropsUI::getPipeInfo()
{
    
    //添加背景图片
    pLayer2 = CCSprite::create("Image/BackPack/BKShop1.png");
    
    tableView = CCTableView::create(this,getTableSize());
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setPosition(ccp(G_SF(0), G_SF(7)));
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    tableView->setTouchPriority(-10001);//20131020优先级设置
    
    tableView->setDelegate(this);//wangguolong
    
    pLayer2->addChild(tableView);
    pLayer2->setPosition(ccp(G_SF(370),G_SF(380)));
    this->addChild(pLayer2);

    
	tableView->reloadData();
    
}

/*
 设置TableView的大小
 */
CCSize SpPropsUI::getTableSize()
{
	
	return CCSizeMake(pLayer2->getContentSize().width,pLayer2->getContentSize().height-G_SF(12));
}

/*
 设置TableView道具一行的坐标
 */
CCSize SpPropsUI::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(G_SF(230), getTableSize().height/3);
    
}

void SpPropsUI::createTableViewCell(CCNode *cell,CCTableView *table,int index)
{
//    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene && pMainLayer->getnCTIndex() != CLScene_ShelfLayer )
    {
        CCLOG("Error: The cur scene is not the main scene!!!");
        return;
    }
    ShopLayer* bl = (ShopLayer*)pMainLayer->getChildByTag(SHOPTAG);

    //道具数量
    int nCardNumber = bl->m_bkData["Shop4"].size();
    
    //tag计数
    int iTag = 0;
    int ZERO = 0;
    
    //道具数目为零时
    if(nCardNumber == 0)
    {
        return;
    }
    
    iTag = index * 2;
    //价格类型
    int LeftSalaType = 0;
    //单价
    std::string LeftMoney ;
    
    //价格类型
    LeftSalaType = bl->m_bkData["Shop4"][iTag][1].asIntModestyle();
    
    //单价
    LeftMoney = bl->m_bkData["Shop4"][iTag][2].asStringModeStyle();
    
    
    std::map<int,int>::iterator itBack = backMap.find(iTag + 1000);
    if(itBack != backMap.end()){
        
        //道具卡背面
        CCSprite *BackSprite = CCSprite::create("Image/BackPack/Back.png");
        //查看左边道具信息，左边坐标设置
        BackSprite->setPosition(ccp(G_SF(125),G_SF(113.5) ));
        cell->addChild(BackSprite,1,iTag + 1000);
        
        int No=0;
        std::string Note;
        
        No = bl->m_bkData["Shop4"][iTag][ZERO].asIntModestyle();
        
        //名称
        std::string ClassName = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artCaption;
        
        //名称
        CCLabelTTF* name = CCLabelTTF::create(ClassName.c_str(), "Arial", G_SF(25));
        name->setColor(ccc3(220,100,0));
        name->setPosition(ccp(G_SF(10)+name->getContentSize().width*0.5, BackSprite->getContentSize().height - G_SF(35)));
        BackSprite->addChild(name);
        
        //道具卡描述
        Note = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artNote;
        
        //道具说明文字
        CCLabelTTF* label = CCLabelTTF::create(Note.c_str(), "Arial", G_SF(25), CCSizeMake(BackSprite->getContentSize().width-G_SF(20), G_SF(150)), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
        //道具说明文字设置为红色
        label->setColor(ccc3(110, 27, 0));
        label->setPosition(ccp(G_SF(115), G_SF(100)));
        BackSprite->addChild(label);
        
        //设置正面是否显示
        backfag = false;
        //设置卡牌为信息面时查看按钮不可用
        btn= false;
    }
    
    
    //左边道具图片
    CCSprite *spriteLeft = CCSprite::create("Image/BackPack/PropsBackground.png");
    spriteLeft->setPosition(ccp(G_SF(125),G_SF(113.5) ));
    cell->addChild(spriteLeft,1,iTag);
    spriteLeft->setVisible(backfag);
    backfag = true;
    
    //道具IconId
    int ArtIconId = 0;
    ArtIconId = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[bl->m_bkData["Shop4"][iTag][ZERO].asIntModestyle()]->m_artIconId;
    
    //道具Icon路径
    std::string LCardName_path = CCString::createWithFormat("Image/BackPack/Props%d.png", ArtIconId)->getCString();
    
    //道具Icon
    CCSprite *PropsIcon = CCSprite::create(LCardName_path.c_str());
    PropsIcon->setPosition(ccp(spriteLeft->getContentSize().width*0.5, spriteLeft->getContentSize().height*0.5));
    spriteLeft->addChild(PropsIcon);
    
    //左边查看道具信息按钮
    Button *btnLeft = Button::createBtn("Image/PublicUI/explainBtn.png");
    btnLeft->setPosition(ccp(btnLeft->getContentSize().width*0.5+G_SF(10), spriteLeft->getContentSize().height - btnLeft->getContentSize().height*0.5 - G_SF(10)));
    if(btn){
        btnLeft->setIsShow(true);
    }else{
        btnLeft->setIsShow(false);
        btn = true;
    }
    btnLeft->setOnClickCallbackNode(callfuncN_selector(SpPropsUI::openView), this);
    spriteLeft->addChild(btnLeft,1,iTag+100000);
    
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
    
    if((index * 2 + 1) < nCardNumber){
        
        iTag = 2 * index + 1;
        
        //价格类型
        int RightSalaType = 0;
        //单价
        std::string RightMoney ;
        
        //价格类型
        RightSalaType = bl->m_bkData["Shop4"][iTag][1].asIntModestyle();
        
        //单价
        RightMoney = bl->m_bkData["Shop4"][iTag][2].asStringModeStyle();
        
        std::map<int,int>::iterator itBack = backMap.find(iTag + 1000);
        if(itBack != backMap.end()){
            
            //道具卡背面
            CCSprite *BackSprite = CCSprite::create("Image/BackPack/Back.png");
            //查看左边道具信息，左边坐标设置
            BackSprite->setPosition(ccp(G_SF(355), G_SF(113.5)));
            cell->addChild(BackSprite,1,iTag + 1000);
            
            int No=0;
            std::string Note;
            
            No = bl->m_bkData["Shop4"][iTag][ZERO].asIntModestyle();
            
            //名称
            std::string ClassName = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artCaption;
            
            //名称
            CCLabelTTF* name = CCLabelTTF::create(ClassName.c_str(), "Arial", G_SF(25));
            name->setColor(ccc3(220,100,0));
            name->setPosition(ccp(G_SF(10)+name->getContentSize().width*0.5, BackSprite->getContentSize().height - G_SF(35)));
            BackSprite->addChild(name);
            
            //道具卡描述
            Note = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artNote;
            
            //道具说明文字
            CCLabelTTF* label = CCLabelTTF::create(Note.c_str(), "Arial", G_SF(25), CCSizeMake(BackSprite->getContentSize().width-G_SF(20), G_SF(150)), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
            //道具说明文字设置为红色
            label->setColor(ccc3(110, 27, 0));
            label->setPosition(ccp(G_SF(115), G_SF(100)));
            BackSprite->addChild(label);
            
            //设置正面是否显示
            backfag = false;
            //设置卡牌为信息面时查看按钮不可用
            btn= false;
        }
        
        //右边道具图片
        CCSprite *spriteRight = CCSprite::create("Image/BackPack/PropsBackground.png");
        spriteRight->setPosition(ccp(G_SF(355), G_SF(113.5)));
        cell->addChild(spriteRight,1,iTag);
        spriteRight->setVisible(backfag);
        backfag = true;
        
        ArtIconId = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[bl->m_bkData["Shop4"][iTag][ZERO].asIntModestyle()]->m_artIconId;
        
        //道具Icon路径
        std::string RCardName_path = CCString::createWithFormat("Image/BackPack/Props%d.png", ArtIconId)->getCString();
        
        //道具Icon
        CCSprite *PropsIcon = CCSprite::create(RCardName_path.c_str());
        PropsIcon->setPosition(ccp(spriteRight->getContentSize().width*0.5, spriteRight->getContentSize().height*0.5));
        spriteRight->addChild(PropsIcon);
        
        //右边查看道具信息按钮
        Button *btnRight = Button::createBtn("Image/PublicUI/explainBtn.png");
        btnRight->setPosition(ccp(btnRight->getContentSize().width*0.5+G_SF(10), spriteRight->getContentSize().height - btnRight->getContentSize().height*0.5 - G_SF(10)));
        if(btn){
            btnRight->setIsShow(true);
        }else{
            btnRight->setIsShow(false);
            btn = true;
        }
        btnRight->setOnClickCallbackNode(callfuncN_selector(SpPropsUI::openView), this);
        spriteRight->addChild(btnRight,1,iTag+100000);
        
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
        
    }
    
}

//table cell选中
void SpPropsUI::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
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
    
    
    n_Tag = 0;
    
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
    salaType = bl->m_bkData["Shop4"][n_Tag][1].asIntModestyle();
    
    cardID = bl->m_bkData["Shop4"][n_Tag][ZERO].asIntModestyle();
    
    price = bl->m_bkData["Shop4"][n_Tag][2].asIntModestyle();
    
    //玩家当前拥有数量
    haveNumber = bl->m_bkData["Shop4"][n_Tag][3].asIntModestyle();
    
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
        //钱不足
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
        //购物卡不足
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
    
    //判断选中的是卡片正面还是卡牌背面
    if(dynamic_cast<CCNode*>(node->getChildByTag(n_Tag)) && dynamic_cast<CCNode*>(node->getChildByTag(n_Tag+1000)))
    {
        
        CCLog("选中背面");
        
        CCSprite* pSender = dynamic_cast<CCSprite*>(node->getChildByTag(backMap[n_Tag+1000]));
        CCSprite* pSender2 = dynamic_cast<CCSprite*>(node->getChildByTag(n_Tag));
        
        //返回正面
        ClickView(pSender,pSender2,n_Tag);
        
    }else{
        CCLog("选中正面");
        
        if(salaType != 3){
            //钱不够购买一个商品
            if(userCurrentMoney < price){
                
                TiShiLayer::create(strMe);
            }else{
                //可购买个数为0
                if(buyMaxNumber <= 0)
                {
                    //该道具达到购买数量上限！
                    TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("PropsUpperLimit"));
                    
                }else{
                    ShopBuyLayer *lBuy = ShopBuyLayer::createLayer(cardID,salaType ,price, true,buyMaxNumber,"Shop4");
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
}

CCTableViewCell* SpPropsUI::tableCellAtIndex(CCTableView *table, unsigned int idx)
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
unsigned int SpPropsUI::numberOfCellsInTableView(CCTableView *table)
{
//    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    ShopLayer* bl = (ShopLayer*)pMainLayer->getChildByTag(SHOPTAG);
    
    //道具数量
    int num = bl->m_bkData["Shop4"].size();
    
    if(num % 2 == 0)
    {
        return num/2;
        
    }else{
        return (num+1)/2;
        
    }
    
}

//查看道具信息
void SpPropsUI::openView(CCNode* pSender)
{
    CCPoint btnWorldPoint = pSender->getParent()->convertToWorldSpace(pSender->getPosition());//获得按钮的世界坐标
    CCPoint tableBgWorld = pLayer2->getParent()->convertToWorldSpace(pLayer2->getPosition());//获得tableview背景的世界坐标
    
    float btnWorldPointY = btnWorldPoint.y; //按钮的世界坐标y
    float tbvYup = tableBgWorld.y + pLayer2->getContentSize().height * 0.5;//tableview背景最上部分的世界坐标y
    float tbvYdown = tableBgWorld.y - pLayer2->getContentSize().height * 0.5;//tableview背景最下部分的世界坐标y
    
    if (!(tbvYup > btnWorldPointY && tbvYdown<btnWorldPointY)) {
        return;//不在tableview范围内
    }
    
    //设置正面按钮消失不可用
    Button *button =   dynamic_cast<Button*>(pSender);
    button->setIsShow(false);
    
//    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene && pMainLayer->getnCTIndex() != CLScene_ShelfLayer)
    {
        CCLOG("Error: The cur scene is not the main scene!!!");
        return;
    }
    ShopLayer* bl = (ShopLayer*)pMainLayer->getChildByTag(SHOPTAG);
    
    int No=0;
    std::string Note;
    int ZERO = 0;
    
    No = bl->m_bkData["Shop4"][pSender->getParent()->getTag()][ZERO].asIntModestyle();
    
    //角色卡描述
    Note = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artNote;
    
    //道具卡背面
    CCSprite *BackSprite = CCSprite::create("Image/BackPack/Back.png");
    
    if(pSender->getParent()->getTag() % 2 == 0)
    {
        //查看左边道具信息，左边坐标设置
        BackSprite->setPosition(ccp(G_SF(122.5), G_SF(113.5)));
    }else
    {
        
        BackSprite->setPosition(ccp(G_SF(355), G_SF(113.5)));
    }
    
    pSender->getParent()->getParent()->addChild(BackSprite,1,OPENVIEW_TAG + pSender->getParent()->getTag());
    BackSprite->setVisible(false);
    
    //添加信息卡牌tag到map中
    backMap[OPENVIEW_TAG + pSender->getParent()->getTag()] = OPENVIEW_TAG + pSender->getParent()->getTag();
    
    //名称
    std::string ClassName = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artCaption;
    
    //名称
//    CCLabelTTF* name = CCLabelTTF::create(ClassName.c_str(), "Arial",  G_SF(25), CCSizeMake(BackSprite->getContentSize().width-G_SF(20), G_SF(25)), kCCTextAlignmentLeft);
//    
//    name->setColor(ccc3(220,100,0));
//    name->setPosition(ccp(G_SF(115), BackSprite->getContentSize().height - G_SF(35)));
    CCLabelTTF* name = CCLabelTTF::create(ClassName.c_str(), "Arial", G_SF(25));
    name->setColor(ccc3(220,100,0));
    name->setPosition(ccp(G_SF(10)+name->getContentSize().width*0.5, BackSprite->getContentSize().height - G_SF(35)));
    BackSprite->addChild(name);
    
    //道具说明文字
//    CCLabelTTF* label = CCLabelTTF::create(Note.c_str(), "Arial",  G_SF(25), CCSizeMake(BackSprite->getContentSize().width-G_SF(20), G_SF(150)), kCCTextAlignmentLeft);
    CCLabelTTF* label = CCLabelTTF::create(Note.c_str(), "Arial", G_SF(25), CCSizeMake(BackSprite->getContentSize().width-G_SF(20), G_SF(150)), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    //道具说明文字设置为红色
    label->setColor(ccc3(110, 27, 0));
    label->setPosition(ccp(G_SF(115), G_SF(100)));
    BackSprite->addChild(label);
    
    float inAngleZ = 270;  //起始Z轴角度
    float inDeltaZ = 90;    //旋转的Z角差
    float outDeltaZ = 90;
    float outAngleZ = 0;
    float duration = 0.25f;//旋转时间
    
    CCActionInterval* inA = (CCActionInterval*)CCSequence::create(
                                                                  CCDelayTime::create(duration /2),
                                                                  CCShow::create(),
                                                                  CCOrbitCamera::create(duration/2, 1, 0, inAngleZ, inDeltaZ, 0, 0),NULL
                                                                  );
    
    
    CCActionInterval* outA = (CCActionInterval *)CCSequence::create(
                                                                    CCOrbitCamera::create(duration/2, 1, 0, outAngleZ, outDeltaZ, 0, 0),
                                                                    CCHide::create(),
                                                                    CCDelayTime::create(duration/2),
                                                                    NULL
                                                                    );
    
    BackSprite->runAction(inA);
    pSender->getParent()->runAction(outA);
    
    
}

//返回正面
void SpPropsUI::ClickView(CCNode* pSender,CCNode* pSender2,int tag)
{
    
    //设置正面按钮显示可用
    Button *button =   dynamic_cast<Button*>(pSender2->getChildByTag(tag+100000));
    button->setIsShow(true);
    
    if(m_touchPoint.x < G_SF(365))
    {
        //查看左边角色信息，左边坐标设置
        pSender->setPosition(ccp(G_SF(122.5), G_SF(113.5)));
        
    }else if(m_touchPoint.x >= G_SF(387))
    {
        
        pSender->setPosition(ccp(G_SF(355), G_SF(113.5)));
    }else
    {
        CCLog("没有选中任何");
        return;
    }
    
    float inAngleZ = 270;  //起始Z轴角度
    float inDeltaZ = 90;    //旋转的Z角差
    float outDeltaZ = 90;
    float outAngleZ = 0;
    float duration = 0.25f;//旋转时间
    
    CCActionInterval* inA = (CCActionInterval*)CCSequence::create(
                                                                  CCDelayTime::create(duration /2),
                                                                  CCShow::create(),
                                                                  CCOrbitCamera::create(duration/2, 1, 0, inAngleZ, inDeltaZ, 0, 0),NULL
                                                                  );
    
    
    CCActionInterval* outA = (CCActionInterval *)CCSequence::create(
                                                                    CCOrbitCamera::create(duration/2, 1, 0, outAngleZ, outDeltaZ, 0, 0),
                                                                    CCHide::create(),
                                                                    CCDelayTime::create(duration/2),
                                                                    NULL
                                                                    );
    
    //角色卡正面动作
    pSender2->runAction(inA);
    //角色卡信息面动作
    pSender->runAction(outA);
    
    //移除角色说明卡
    //    removeChild(pSender);
    backMap.erase(pSender->getTag());
    pSender->removeFromParent();
}

void SpPropsUI::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);
}

void SpPropsUI::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool SpPropsUI::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    // 记录触摸起始点的位置
    m_touchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
    
    return true;
}


void SpPropsUI::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void SpPropsUI::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

