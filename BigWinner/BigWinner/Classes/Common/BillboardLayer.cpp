//
//  BillboardLayer.cpp
//  BigWinner
//
//  Created by LunaBox on 14-1-3.
//
//

#include "BillboardLayer.h"
#include "Button.h"
#include "LGameCommonDefine.h"
#include "CLSceneManager.h"

USING_NS_CC_EXT;

using std::string;
using namespace cocos2d;

bool BillboardLayer::init()
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

void BillboardLayer::getPipeInfo()
{
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    //添加背景图片
    m_pLayer = CCScale9Sprite::create("Image/LoginLayer/Billboard_BG.png");
    m_pLayer->setPreferredSize(CCSizeMake(G_SF(600), G_SF(850)));
    m_pLayer->setPosition(ccp(size.width*0.5, size.height*0.5));
    
    //公告消息
    CCSprite* pTTFBG = CCSprite::create("Image/PveLayer/CP_GiveUpTTFBG.png");
    pTTFBG->setAnchorPoint(ccp(0, 1));
    pTTFBG->setPosition(ccp(G_SF(45), m_pLayer->getContentSize().height));
    m_pLayer->addChild(pTTFBG, 1);
    
    CCLabelTTF* pGiveTTF = CCLabelTTF::create("公告消息", "Verdana_Bold", G_SF(30), CCSizeMake(G_SF(120),G_SF(40)), kCCTextAlignmentCenter);
    pGiveTTF->setPosition(ccp(pTTFBG->getContentSize().width/2, G_SF(35)));
    pGiveTTF->setColor(ccBLACK);
    pTTFBG->addChild(pGiveTTF);
    
    //关闭
    Button *shutdown = Button::createBtn("Image/PublicUI/Close.png");
    shutdown->setPosition(ccp(G_SF(550), m_pLayer->getContentSize().height-shutdown->getContentSize().height*0.5));
    shutdown->setOnClickCallbackNode(callfuncN_selector(BillboardLayer::menuCallBackButton), this);
    shutdown->setHandlerPriority(-700001);
    m_pLayer->addChild(shutdown);
    
    m_tableView = CCTableView::create(this,getTableSize());
    m_tableView->setDirection(kCCScrollViewDirectionVertical);
    m_tableView->setPosition(ccp(G_SF(0), G_SF(20)));
    m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tableView->setTouchPriority(-700001);//20131020优先级设置
    
    m_pLayer->addChild(m_tableView,1,11);
    this->addChild(m_pLayer);
    
	m_tableView->reloadData();
    
}

void BillboardLayer::menuCallBackButton(CCNode* r)
{
    CLSceneManager::sharedSceneManager()->setnGgaoFlg(1);
    
    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);
}

/*
 设置TableView的大小
 */
CCSize BillboardLayer::getTableSize()
{
    return CCSizeMake(m_pLayer->getContentSize().width-G_SF(30),m_pLayer->getContentSize().height - G_SF(100));
}

/*
 设置TableView题卡一行的坐标
 */
CCSize BillboardLayer::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(G_SF(230), getTableSize().height+G_SF(2000));
    
}

void BillboardLayer::createTableViewCell(CCNode *cell,CCTableView *table,int index)
{

    m_pBillboard = CCSprite::create("Image/LoginLayer/Billboard.png");
    m_pBillboard->setScale(0.8);
    m_pBillboard->setPosition(ccp(m_pLayer->getContentSize().width*0.5, G_SF(1600)));
    cell->addChild(m_pBillboard);
    
}

//table cell选中
void BillboardLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
}

CCTableViewCell* BillboardLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
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
unsigned int BillboardLayer::numberOfCellsInTableView(CCTableView *table)
{
    return 1;
}

void BillboardLayer::onEnter()
{
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -700000, true);
}

void BillboardLayer::onExit()
{
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}



bool BillboardLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    
    if(this->isVisible())
        return true;
    else
        return false;
}

void BillboardLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void BillboardLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    

}

