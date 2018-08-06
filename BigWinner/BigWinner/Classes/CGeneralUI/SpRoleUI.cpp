#include "SpRoleUI.h"
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

bool SpRoleUI::init()
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

void SpRoleUI::getPipeInfo()
{
    
    //添加背景图片
    this->removeChildByTag(1001, true);
    
    pLayer2 = CCSprite::create("Image/BackPack/BKShop1.png");
    
    tableView = CCTableView::create(this,getTableSize());
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setPosition(ccp(G_SF(0), G_SF(7)));
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    tableView->setTouchPriority(-10001);//20131020优先级设置
    
    tableView->setDelegate(this);//wangguolong
    
    pLayer2->addChild(tableView);
    pLayer2->setPosition(ccp(G_SF(370),G_SF(380)));
    this->addChild(pLayer2,1,1001);

    
    this->setTouchEnabled(true);
    
	tableView->reloadData();
    
}

/*
 设置TableView的大小
 */
CCSize SpRoleUI::getTableSize()
{
    return CCSizeMake(pLayer2->getContentSize().width,pLayer2->getContentSize().height-G_SF(12));
    
}

/*
 设置TableView角色一行的坐标
 */
CCSize SpRoleUI::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(getTableSize().width, getTableSize().height/3);
    
}
/*
 创建角色信息
 */
void SpRoleUI::createTableViewCell(CCNode *cell,CCTableView *table,int index)
{

    CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene && pMainLayer->getnCTIndex() != CLScene_ShelfLayer)
    {
        CCLOG("Error: The cur scene is not the main scene!!!");
        return;
    }
    ShopLayer* bl = (ShopLayer*)pMainLayer->getChildByTag(SHOPTAG);
    
    //角色数量
    int nCardNumber = bl->m_bkData["Shop3"].size();
    
    //角色数目为零时
    if(nCardNumber == 0)
    {
        return;
    }
    
    //tag计数
    int iTag = 0;
    
    iTag = index * 2;
    //价格类型
    int LeftSalaType = 0;
    //单价
    std::string LeftMoney ;
    //玩家拥有角色数量
    int roleNumberLeft = 0;
    
    int ZERO = 0;
    
    //价格类型
    LeftSalaType = bl->m_bkData["Shop3"][iTag][1].asIntModestyle();
    
    //单价
    LeftMoney = bl->m_bkData["Shop3"][iTag][2].asStringModeStyle();
    
    //玩家拥有角色数量
    roleNumberLeft = bl->m_bkData["Shop3"][iTag][3].asIntModestyle();
    
    std::map<int,int>::iterator itBack = backMap.find(iTag + 1000);
    if(itBack != backMap.end()){
        
        //角色卡背面
        CCSprite *BackSprite = CCSprite::create("Image/BackPack/Back.png");
        //查看左边角色信息，左边坐标设置
        BackSprite->setPosition(ccp(G_SF(125),G_SF(113.5) ));
        cell->addChild(BackSprite,1,iTag + 1000);
        
        std::string Note;
        int No = 0;
        
        No = bl->m_bkData["Shop3"][iTag][ZERO].asIntModestyle();
        
        //名称
        std::string ClassName = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artCaption;
        
        //名称
        CCLabelTTF* name = CCLabelTTF::create(ClassName.c_str(), "Arial", G_SF(25));
        name->setColor(ccc3(220,100,0));
        name->setPosition(ccp(G_SF(10)+name->getContentSize().width*0.5, BackSprite->getContentSize().height - G_SF(35)));
        BackSprite->addChild(name);
        
        //角色卡描述
        Note = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artNote;
        
        //角色说明文字
        CCLabelTTF* label = CCLabelTTF::create(Note.c_str(), "Arial", G_SF(25), CCSizeMake(BackSprite->getContentSize().width-G_SF(20), G_SF(150)), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
        label->setColor(ccc3(110, 27, 0));
        label->setPosition(ccp(G_SF(115), G_SF(100)));
        BackSprite->addChild(label);
        
        //设置正面是否显示
        backfag = false;
        //设置卡牌为信息面时查看按钮不可用
        btn= false;
    }
    
    
    //左边角色图片
    CCSprite *spriteLeft = CCSprite::create("Image/BackPack/RoleBackground.png");
    spriteLeft->setPosition(ccp(G_SF(125),G_SF(113.5) ));
    cell->addChild(spriteLeft,1,iTag);
    spriteLeft->setVisible(backfag);
    backfag = true;
    
    //道具IconId
    int ArtIconId = 0;
    ArtIconId = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[bl->m_bkData["Shop3"][iTag][ZERO].asIntModestyle()]->m_artIconId;
    
    //人物名称路径
    std::string LRoleName_path = CCString::createWithFormat("Image/BackPack/RoleName%d.png", ArtIconId)->getCString();
    
    //人物
    CCSprite *RoleIcon = CCSprite::create(LRoleName_path.c_str());
    RoleIcon->setPosition(ccp(spriteLeft->getContentSize().width*0.5, spriteLeft->getContentSize().height*0.5));
    spriteLeft->addChild(RoleIcon);
    
    //左边查看角色信息按钮
    Button *btnLeft = Button::createBtn("Image/PublicUI/explainBtn.png");
    btnLeft->setPosition(ccp(btnLeft->getContentSize().width*0.5+G_SF(10), spriteLeft->getContentSize().height - btnLeft->getContentSize().height*0.5 - G_SF(10)));
    if(btn){
        btnLeft->setIsShow(true);
    }else{
        btnLeft->setIsShow(false);
        btn = true;
    }
    btnLeft->setOnClickCallback(callfuncO_selector(SpRoleUI::openView),this);
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
    
    //wangguolong
    std::map<int,int>::iterator roleLeft = roleMap.find(iTag);
    if(roleNumberLeft>0 || roleLeft != roleMap.end())
    {
        CCSprite *pSpr = CCSprite::create("Image/BackPack/BuyEnd.png");
        pSpr->setPosition(CCPoint(G_SF(115), G_SF(30)));
        spriteLeft->addChild(pSpr,10,10000 + iTag);
        
    }
    
    if((index * 2 + 1) < nCardNumber){
        
        iTag = 2 * index + 1;
        
        //价格类型
        int RightSalaType = 0;
        //单价
        std::string RightMoney ;
        //玩家拥有角色数量
        int roleNumberRirht = 0;
        
        //价格类型
        RightSalaType = bl->m_bkData["Shop3"][iTag][1].asIntModestyle();
        
        //单价
        RightMoney = bl->m_bkData["Shop3"][iTag][2].asStringModeStyle();
        
        //玩家拥有角色数量
        roleNumberRirht = bl->m_bkData["Shop3"][iTag][3].asIntModestyle();
        
        
        std::map<int,int>::iterator itBack = backMap.find(iTag + 1000);
        if(itBack != backMap.end()){
            
            //角色卡背面
            CCSprite *BackSprite = CCSprite::create("Image/BackPack/Back.png");
            //查看左边角色信息，左边坐标设置
            BackSprite->setPosition(ccp(G_SF(355), G_SF(113.5)));
            cell->addChild(BackSprite,1,iTag + 1000);
            
            std::string Note;
            int No = 0;
            
            No = bl->m_bkData["Shop3"][iTag][ZERO].asIntModestyle();
            
            //名称
            std::string ClassName = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artCaption;
            
            //名称
            CCLabelTTF* name = CCLabelTTF::create(ClassName.c_str(), "Arial", G_SF(25));
            name->setColor(ccc3(220,100,0));
            name->setPosition(ccp(G_SF(10)+name->getContentSize().width*0.5, BackSprite->getContentSize().height - G_SF(35)));
            BackSprite->addChild(name);
            
            //角色卡描述
            Note = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artNote;
            
            //角色说明文字
            CCLabelTTF* label = CCLabelTTF::create(Note.c_str(), "Arial", G_SF(25), CCSizeMake(BackSprite->getContentSize().width-G_SF(20), G_SF(150)), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
            label->setColor(ccc3(110, 27, 0));
            label->setPosition(ccp(G_SF(115), G_SF(100)));
            BackSprite->addChild(label);
            
            //设置正面是否显示
            backfag = false;
            //设置卡牌为信息面时查看按钮不可用
            btn= false;
        }
        
        //右边角色图片
        CCSprite *spriteRight = CCSprite::create("Image/BackPack/RoleBackground.png");
        spriteRight->setPosition(ccp(G_SF(355), G_SF(113.5)));
        cell->addChild(spriteRight,1,iTag);
        spriteRight->setVisible(backfag);
        backfag = true;
        
        ArtIconId = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[bl->m_bkData["Shop3"][iTag][ZERO].asIntModestyle()]->m_artIconId;
        
        //人物名称路径
        std::string RRoleName_path = CCString::createWithFormat("Image/BackPack/RoleName%d.png", ArtIconId)->getCString();
        
        //人物
        CCSprite *RoleIcon = CCSprite::create(RRoleName_path.c_str());
        RoleIcon->setPosition(ccp(spriteLeft->getContentSize().width*0.5, spriteLeft->getContentSize().height*0.5));
        spriteRight->addChild(RoleIcon);
        
        //右边查看角色信息按钮
        Button *btnRight = Button::createBtn("Image/PublicUI/explainBtn.png");
        btnRight->setPosition(ccp(btnRight->getContentSize().width*0.5+G_SF(10), spriteRight->getContentSize().height - btnRight->getContentSize().height*0.5 - G_SF(10)));
        if(btn){
            btnRight->setIsShow(true);
        }else{
            btnRight->setIsShow(false);
            btn = true;
        }
        btnRight->setOnClickCallback(callfuncO_selector(SpRoleUI::openView), this);
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

        //wangguolong
        std::map<int,int>::iterator roleRight = roleMap.find(iTag);
        if(roleNumberRirht>0 || roleRight != roleMap.end())
        {
            CCSprite *pSpr = CCSprite::create("Image/BackPack/BuyEnd.png");
            pSpr->setPosition(CCPoint(G_SF(115), G_SF(30)));
            spriteRight->addChild(pSpr,10,10000 + iTag);
            
        }
        
    }
    
    
}

void SpRoleUI::removeUI()
{
    fagR = true;
}

//table cell选中
void SpRoleUI::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
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
    int haveNumber = 0;//拥有数量
    int buyNumber = 0;//数据库标中商品购买上限
    
    //选中的cell  因为我们的cell里面装了2个Node  一个左边一个右边  所以我们取出cell没有用 要取出里面的两个sprite
	CCLOG("选中了: %i", cell->getIdx());
    
    //node是当前cell里面唯一的一个node 并且tag是在tableCellAtIndex方法里面你们定义的tag 把它取出来，接下来就是取左右两个sprite了
    CCNode* node = dynamic_cast<CCNode*>(cell->getChildByTag(1));
    
    //这里是取出左边的sprite 可以拿se在Visible屏蔽的代码测试  具体里面的操作就看你们自己了，这里的tag也是你们再createTableViewCell里面定义的
    CCSprite* left = dynamic_cast<CCSprite*>(node->getChildByTag(cell->getIdx()*2));
    //  left->setVisible(false);
    
    //这里是取出右边的sprite
    CCSprite* right = dynamic_cast<CCSprite*>(node->getChildByTag(cell->getIdx()*2 + 1 ));
    
    n_Tag = 0;
    
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
    salaType = bl->m_bkData["Shop3"][n_Tag][1].asIntModestyle();
    
    cardID = bl->m_bkData["Shop3"][n_Tag][ZERO].asIntModestyle();
    
    price = bl->m_bkData["Shop3"][n_Tag][2].asIntModestyle();
    
    haveNumber = bl->m_bkData["Shop3"][n_Tag][3].asIntModestyle();
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
            //玩家拥有金钱为0时
            if(userCurrentMoney == 0)
            {
                buyMaxNumber = 0;
            }else
            {
                buyMaxNumber = userCurrentMoney/price;
            }
            
        }else
        {
            buyMaxNumber = buyNumber - haveNumber;
            if(buyMaxNumber <= 0)
            {
                buyMaxNumber = 0;
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
            //玩家拥有购物卡为0时
            if(userCurrentMoney == 0)
            {
                buyMaxNumber = 0;
            }else
            {
                buyMaxNumber = userCurrentMoney/price;
            }
            
        }else
        {
            buyMaxNumber = buyNumber - haveNumber;
            if(buyMaxNumber <= 0)
            {
                buyMaxNumber = 0;
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
        TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("NoDisparkMsg"));
//        CCLog("选中正面");
//        
//        //已经购买过了 弹出层
//        if(dynamic_cast<CCNode*>(node->getChildByTag(n_Tag)->getChildByTag(n_Tag+10000))){
//            
//            if(fagR){
//                
//                //已拥有,大侠请购买其他角色！
//                TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("RoleUpperLimit"));
//
//                //设置不同时存在两个弹出层
//                fagR = false;
//                this->scheduleOnce(schedule_selector(SpRoleUI::removeUI), 2.5);
//            }
//
//
//        }else{
//            if(salaType != 3){
//                //钱不够购买一个商品
//                if(userCurrentMoney < price){
//                    
//                    TiShiLayer::create(strMe);
//                }else{
//                    
//                    //wangguolong
//                    roleMap[n_Tag] = n_Tag;
//                    //设置购买了那张［No］人物卡
//                    bl->setnRoleNo(cardID);
//                    
//                    if(bl->BuyRoleNoMap[cardID] == 1){
//                        return; //购买过该人物卡以后 返回
//                    }
//                    ShopBuyLayer *lBuy = ShopBuyLayer::createLayer(cardID, salaType ,price, false,haveNumber,"Shop3");
//                    lBuy->setPosition(ccp(G_SF(-10), G_SF(-10)));
//                    this->addChild(lBuy,10,SHOPBUYPALYER_TAG);
//                }
//            }else{
//                CCLog("---价格购买类型为3,返回--");
//                TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("MoneyShop"));
//                return;
//            }
//
//        }
        

    }

}

CCTableViewCell* SpRoleUI::tableCellAtIndex(CCTableView *table, unsigned int idx)
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
unsigned int SpRoleUI::numberOfCellsInTableView(CCTableView *table)
{
    CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    
    ShopLayer* bl = (ShopLayer*)pMainLayer->getChildByTag(SHOPTAG);
    
    //角色数量
    int num = bl->m_bkData["Shop3"].size();
    
    if(num % 2 == 0)
    {
        return num/2;
        
    }else{
        return (num+1)/2;
        
    }
    
}

//查看角色信息
void SpRoleUI::openView(CCNode* pSender)
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
    
    No = bl->m_bkData["Shop3"][pSender->getParent()->getTag()][ZERO].asIntModestyle();
    
    //角色卡描述
    Note = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artNote;

    //角色卡背面
    CCSprite *BackSprite = CCSprite::create("Image/BackPack/Back.png");
    
    if(pSender->getParent()->getTag() % 2 == 0)
    {
        //查看左边角色信息，左边坐标设置
        BackSprite->setPosition(ccp(G_SF(122.5), G_SF(115)));
    }else
    {
        
        BackSprite->setPosition(ccp(G_SF(355), G_SF(115)));
    }
    
    pSender->getParent()->getParent()->addChild(BackSprite,1,OPENVIEW_TAG + pSender->getParent()->getTag());
    BackSprite->setVisible(false);
    
    //添加信息卡牌tag到map中
    backMap[OPENVIEW_TAG + pSender->getParent()->getTag()] = OPENVIEW_TAG + pSender->getParent()->getTag();
    
    //名称
    std::string ClassName = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artCaption;
    
    //名称
    CCLabelTTF* name = CCLabelTTF::create(ClassName.c_str(), "Arial", G_SF(25));
    name->setColor(ccc3(220,100,0));
    name->setPosition(ccp(G_SF(10)+name->getContentSize().width*0.5, BackSprite->getContentSize().height - G_SF(35)));
    BackSprite->addChild(name);
    
    //角色说明文字
    CCLabelTTF* label = CCLabelTTF::create(Note.c_str(), "Arial", G_SF(25), CCSizeMake(BackSprite->getContentSize().width-G_SF(20), G_SF(150)), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
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

void SpRoleUI::ClickView(CCNode* pSender,CCNode* pSender2,int tag)
{
    //设置正面按钮显示可用
    Button *button =   dynamic_cast<Button*>(pSender2->getChildByTag(tag+100000));
    button->setIsShow(true);
    
    if(m_touchPoint.x < G_SF(365))
    {
        //查看左边角色信息，左边坐标设置
        pSender->setPosition(ccp(G_SF(122.5), G_SF(115)));
        
    }else if(m_touchPoint.x >= G_SF(387))
    {
        
        pSender->setPosition(ccp(G_SF(355), G_SF(115)));
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

void SpRoleUI::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);
}

void SpRoleUI::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool SpRoleUI::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    // 记录触摸起始点的位置
    m_touchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
    CCLog("点击点x坐标：%f",m_touchPoint.x);
    return true;
}


void SpRoleUI::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void SpRoleUI::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void SpRoleUI::_ShopBuyCallBack(bool buyStatus)
{
    //购买成功以后刷新页面
    getPipeInfo();
}



