//
//  BuyMoneyLayer.cpp
//  BigWinner
//
//  Created by LunaBox on 13-12-28.
//
//

#include "BuyMoneyLayer.h"
#include "Button.h"
#include "LGameCommonDefine.h"
#include "CLSceneManager.h"
#include "JsonDataManager.h"
#include "DlgTiShiLayer.h"
#include "CLChangeString.h"
#include "CLSoundManager.h"

#define OPENVIEW_TAG 1000

BuyMoneyLayer* BuyMoneyLayer::create(Json::Value &data)
{
    BuyMoneyLayer *pRet = new BuyMoneyLayer();
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

bool BuyMoneyLayer::init(Json::Value &data)
{
    if(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0)))
    {
        return false;
    }
    
//    Json::Value *data = CLSceneManager::sharedSceneManager()->GetJsonDataFromMsgID(CMD_RMBBUYCARD_REPLY);
    m_BuyData = data;
    
    //初始化UI
    getPipeInfo();
    
    return true;
}

void BuyMoneyLayer::getPipeInfo()
{
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    //背景图片
    CCSprite* buyMoneyBG = CCSprite::create("Image/BackPack/BKShop2.png");
    buyMoneyBG->setPosition(ccp(size.width*0.5, size.height*0.43));
    this->addChild(buyMoneyBG);

    //关闭
    Button *shutdown = Button::createBtn("Image/PublicUI/Close.png");
    shutdown->setPosition(ccp(G_SF(550), G_SF(765)));
    shutdown->setOnClickCallbackNode(callfuncN_selector(BuyMoneyLayer::menuCallBackButton), this);
    shutdown->setHandlerPriority(-20001);
    buyMoneyBG->addChild(shutdown);
    
    //充值背景
    CCSprite *chongzhi = CCSprite::create("Image/PublicUI/TitleBackground.png");
    chongzhi->setPosition(ccp(G_SF(75), G_SF(765)));
    buyMoneyBG->addChild(chongzhi,0);
    
    //充值背景文字
    CCSprite *bbwenzi = CCSprite::create("Image/BackPack/BuyShopType.png");
    bbwenzi->setPosition(ccp(G_SF(80), G_SF(25)));
    chongzhi->addChild(bbwenzi,100);
    
    //添加tableview背景图片
    pLayer2 = CCSprite::create("Image/BackPack/BuyMoney_BG.png");
    
    tableView = CCTableView::create(this,getTableSize());
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setPosition(ccp(G_SF(0), G_SF(7)));
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    tableView->setTouchPriority(-20001);//20131020优先级设置
    
    tableView->setDelegate(this);//wangguolong
    
    pLayer2->addChild(tableView);
    pLayer2->setPosition(ccp(buyMoneyBG->getContentSize().width*0.5,G_SF(380)));
    buyMoneyBG->addChild(pLayer2);
    
	tableView->reloadData();
    
    doAction(this, true);

}

/*
 设置TableView的大小
 */
CCSize BuyMoneyLayer::getTableSize()
{
    return CCSizeMake(pLayer2->getContentSize().width,pLayer2->getContentSize().height-G_SF(12));
    
}

/*
 设置TableView货币一行的坐标
 */
CCSize BuyMoneyLayer::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(G_SF(230), getTableSize().height/2.8);
    
}

void BuyMoneyLayer::createTableViewCell(CCNode *cell,CCTableView *table,int index)
{
    //货币数量
    int nCardNumber = m_BuyData["Shop5"].size();
    
    //tag计数
    int iTag = 0;
    int ZERO = 0;
    
    //货币数目为零时
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
    LeftSalaType = m_BuyData["Shop5"][iTag][1].asIntModestyle();
    
    //单价
    LeftMoney = m_BuyData["Shop5"][iTag][2].asStringModeStyle();
    
    
    std::map<int,int>::iterator itBack = backMap.find(iTag + 1000);
    if(itBack != backMap.end()){
        
        //货币卡背面
        CCSprite *BackSprite = CCSprite::create("Image/BackPack/Back.png");
        //查看左边货币信息，左边坐标设置
        BackSprite->setPosition(ccp(G_SF(155),G_SF(100) ));
        cell->addChild(BackSprite,1,iTag + 1000);
        
        int No=0;
        std::string Note;
        
        No = m_BuyData["Shop5"][iTag][ZERO].asIntModestyle();
        
        //名称
        std::string ClassName = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artCaption;
        
        //名称
        CCLabelTTF* name = CCLabelTTF::create(ClassName.c_str(), "Arial", G_SF(25));
        name->setColor(ccc3(220,100,0));
        name->setPosition(ccp(G_SF(10)+name->getContentSize().width*0.5, BackSprite->getContentSize().height - G_SF(35)));
        BackSprite->addChild(name);
        
        //货币卡描述
        Note = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artNote;
        
        //货币说明文字
        CCLabelTTF* label = CCLabelTTF::create(Note.c_str(), "Arial", G_SF(25), CCSizeMake(BackSprite->getContentSize().width-G_SF(20), G_SF(150)), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
        label->setColor(ccc3(110, 27, 0));
        label->setPosition(ccp(G_SF(115), G_SF(100)));
        BackSprite->addChild(label);
        
        //设置正面是否显示
        backfag = false;
        //设置卡牌为信息面时查看按钮不可用
        btn= false;
    }
    
    //左边货币图片
    CCSprite *spriteLeft = CCSprite::create("Image/BackPack/CurrencyBackground.png");
    spriteLeft->setPosition(ccp(G_SF(155),G_SF(100) ));
    cell->addChild(spriteLeft,1,iTag);
    spriteLeft->setVisible(backfag);
    backfag = true;
    
    //道具IconId
    int ArtIconId = 0;
    ArtIconId = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[m_BuyData["Shop5"][iTag][ZERO].asIntModestyle()]->m_artNo;
    
    //道具名称路径
    std::string CurrencyName_path = CCString::createWithFormat("Image/BackPack/CurrencyIcon%d.png", ArtIconId)->getCString();
    
    //道具Icon
    CCSprite *PropsIcon = CCSprite::create(CurrencyName_path.c_str());
    PropsIcon->setPosition(ccp(spriteLeft->getContentSize().width*0.5, spriteLeft->getContentSize().height*0.5));
    spriteLeft->addChild(PropsIcon);
    
    //左边查看货币信息按钮
    Button *btnLeft = Button::createBtn("Image/PublicUI/explainBtn.png");
    btnLeft->setPosition(ccp(btnLeft->getContentSize().width*0.5+G_SF(10), spriteLeft->getContentSize().height - btnLeft->getContentSize().height*0.5 - G_SF(10)));
    if(btn){
        btnLeft->setIsShow(true);
    }else{
        btnLeft->setIsShow(false);
        btn = true;
    }
    btnLeft->setOnClickCallbackNode(callfuncN_selector(BuyMoneyLayer::openView), this);
    btnLeft->setHandlerPriority(-20001);
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
    
    //货币单价
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
        RightSalaType = m_BuyData["Shop5"][iTag][1].asIntModestyle();
        
        //单价
        RightMoney = m_BuyData["Shop5"][iTag][2].asStringModeStyle();
        
        std::map<int,int>::iterator itBack = backMap.find(iTag + 1000);
        if(itBack != backMap.end()){
            
            //货币卡背面
            CCSprite *BackSprite = CCSprite::create("Image/BackPack/Back.png");
            //查看左边货币信息，左边坐标设置
            BackSprite->setPosition(ccp(G_SF(425), G_SF(100)));
            cell->addChild(BackSprite,1,iTag + 1000);
            
            int No=0;
            std::string Note;
            
            No = m_BuyData["Shop5"][iTag][ZERO].asIntModestyle();
            
            //名称
            std::string ClassName = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artCaption;
            
            //名称
            CCLabelTTF* name = CCLabelTTF::create(ClassName.c_str(), "Arial", G_SF(25));
            name->setColor(ccc3(220,100,0));
            name->setPosition(ccp(G_SF(10)+name->getContentSize().width*0.5, BackSprite->getContentSize().height - G_SF(35)));
            BackSprite->addChild(name);
            
            //货币卡描述
            Note = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artNote;
            
            //货币说明文字
            CCLabelTTF* label = CCLabelTTF::create(Note.c_str(), "Arial", G_SF(25), CCSizeMake(BackSprite->getContentSize().width-G_SF(20), G_SF(150)), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
            //货币说明文字设置为红色
            label->setColor(ccc3(110, 27, 0));
            label->setPosition(ccp(G_SF(115), G_SF(100)));
            BackSprite->addChild(label);
            
            //设置正面是否显示
            backfag = false;
            //设置卡牌为信息面时查看按钮不可用
            btn= false;
        }
        
        //右边货币图片
        CCSprite *spriteRight = CCSprite::create("Image/BackPack/CurrencyBackground.png");
        spriteRight->setPosition(ccp(G_SF(425), G_SF(100)));
        cell->addChild(spriteRight,1,iTag);
        spriteRight->setVisible(backfag);
        backfag = true;
        
        ArtIconId = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[m_BuyData["Shop5"][iTag][ZERO].asIntModestyle()]->m_artNo;
        
        //道具名称路径
        std::string CurrencyName_path = CCString::createWithFormat("Image/BackPack/CurrencyIcon%d.png", ArtIconId)->getCString();
        
        //道具Icon
        CCSprite *PropsIcon = CCSprite::create(CurrencyName_path.c_str());
        PropsIcon->setPosition(ccp(spriteRight->getContentSize().width*0.5, spriteRight->getContentSize().height*0.5));
        spriteRight->addChild(PropsIcon);
        
        
        //右边查看货币信息按钮
        Button *btnRight = Button::createBtn("Image/PublicUI/explainBtn.png");
        btnRight->setPosition(ccp(btnRight->getContentSize().width*0.5+G_SF(10), spriteRight->getContentSize().height - btnRight->getContentSize().height*0.5 - G_SF(10)));
        if(btn){
            btnRight->setIsShow(true);
        }else{
            btnRight->setIsShow(false);
            btn = true;
        }
        btnRight->setOnClickCallbackNode(callfuncN_selector(BuyMoneyLayer::openView), this);
        btnRight->setHandlerPriority(-20001);
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
        
        //货币单价
        CCLabelTTF* labelLeft = CCLabelTTF::create(RightMoney.c_str(), "Arial",  G_SF(18), CCSizeMake(G_SF(72), G_SF(25)), kCCTextAlignmentLeft);
        labelLeft->setPosition(ccp(G_SF(90), G_SF(32)));
        spriteRight->addChild(labelLeft);
        labelLeft->setColor(ccc3(110, 27, 0));
        
    }
    
}


//动作执行
void BuyMoneyLayer::doAction(CCNode* r, bool moveIn)
{
    CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    
    if (moveIn) //进场动作
    {
        CCLayerColor *fadeLayer = CCLayerColor::create();
        pMainLayer->addChild(fadeLayer,9,500);
        
        CCFiniteTimeAction *actionOut = CCSequence::create(CCFadeTo::create(0.4f, 255*0.5), NULL); //透明度动画
        fadeLayer->runAction(actionOut);
        
        CCAction *actionMoveOut = CCEaseExponentialOut::create(CCMoveTo::create(0.4f, ccp(0, 0))); //快到慢
        this->runAction(actionMoveOut);
        
        //窗体开闭音乐
        CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_WInClose.wav");
    }
    else //出场动作
    {
        CCFiniteTimeAction *actionIn = CCSequence::create(CCFadeTo::create(0.4f, 0), NULL); //透明度动画
        pMainLayer->getChildByTag(500)->runAction(actionIn);
        
        CCAction *actionMoveIn = CCEaseExponentialIn::create(CCMoveTo::create(0.4f, ccp(r->getPositionX(), r->getPositionY() * (-1)))); //慢到快
        r->runAction(actionMoveIn);
        
        pMainLayer->getChildByTag(500)->removeFromParentAndCleanup(true);
    }
    
}

//table cell选中
void BuyMoneyLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
    //价格类型
    int salaType = 0;
    int ZERO = 0;
    int cardID =0;//No
    int price = 0;//单价
    
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
    salaType = m_BuyData["Shop5"][n_Tag][1].asIntModestyle();
    
    cardID = m_BuyData["Shop5"][n_Tag][ZERO].asIntModestyle();
    
    price = m_BuyData["Shop5"][n_Tag][2].asIntModestyle();
    
    
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
        
    }
}

CCTableViewCell* BuyMoneyLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
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
unsigned int BuyMoneyLayer::numberOfCellsInTableView(CCTableView *table)
{
    
    int num = 0;
    num = m_BuyData["Shop5"].size();
    
    if(num % 2 == 0)
    {
        return num/2;
        
    }else{
        return (num+1)/2;
        
    }
    
}

//查看货币信息
void BuyMoneyLayer::openView(CCNode* pSender)
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
    
    int No=0;
    std::string Note;
    int ZERO = 0;
    
    No = m_BuyData["Shop5"][pSender->getParent()->getTag()][ZERO].asIntModestyle();
    
    //货币卡描述
    Note = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artNote;
    
    //货币卡背面
    CCSprite *BackSprite = CCSprite::create("Image/BackPack/Back.png");
    
    if(pSender->getParent()->getTag() % 2 == 0)
    {
        //查看左边货币信息，左边坐标设置
        BackSprite->setPosition(ccp(G_SF(155), G_SF(100)));
    }else
    {
        
        BackSprite->setPosition(ccp(G_SF(425), G_SF(100)));
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
    
    //货币说明文字
    CCLabelTTF* label = CCLabelTTF::create(Note.c_str(), "Arial", G_SF(25), CCSizeMake(BackSprite->getContentSize().width-G_SF(20), G_SF(150)), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    //货币说明文字颜色
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
void BuyMoneyLayer::ClickView(CCNode* pSender,CCNode* pSender2,int tag)
{
    
    //设置正面按钮显示可用
    Button *button =   dynamic_cast<Button*>(pSender2->getChildByTag(tag+100000));
    button->setIsShow(true);
    
    if(m_touchPoint.x < G_SF(365))
    {
        //查看左边角色信息，左边坐标设置
        pSender->setPosition(ccp(G_SF(155), G_SF(100)));
        
    }else if(m_touchPoint.x >= G_SF(387))
    {
        
        pSender->setPosition(ccp(G_SF(425), G_SF(100)));
    }else
    {
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

//关闭按钮
void BuyMoneyLayer::menuCallBackButton()
{
    doAction(this, false);
    
    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);
}


void BuyMoneyLayer::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -20000, true);
}

void BuyMoneyLayer::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool BuyMoneyLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    // 记录触摸起始点的位置
    m_touchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
    
    return true;
}


void BuyMoneyLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void BuyMoneyLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}