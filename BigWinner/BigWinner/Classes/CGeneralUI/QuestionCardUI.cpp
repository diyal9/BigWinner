#include "QuestionCardUI.h"
#include "Button.h"
#include "LGameCommonDefine.h"

#include "BackPack.h"
#include "CLSceneManager.h"
#include "CLMainSceneLayer.h"
#include "JsonDataManager.h"
#include "CLNetWorkManager.h"
#include "CLChangeString.h"

USING_NS_CC_EXT;

using std::string;
using namespace cocos2d;

bool QuestionCardUI::init()
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

void QuestionCardUI::getPipeInfo()
{
    
    //添加背景图片
    m_pLayer = CCSprite::create("Image/BackPack/BKShop1.png");
    
    m_tableView = CCTableView::create(this,getTableSize());
    m_tableView->setDirection(kCCScrollViewDirectionVertical);
    m_tableView->setPosition(ccp(G_SF(0), G_SF(7)));
    m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tableView->setTouchPriority(-10001);//20131020优先级设置
    
    m_pLayer->addChild(m_tableView,1,11);
    m_pLayer->setPosition(ccp(G_SF(370),G_SF(380)));
    this->addChild(m_pLayer);
    
	m_tableView->reloadData();
    
}

/*
 设置TableView的大小
 */
CCSize QuestionCardUI::getTableSize()
{
    return CCSizeMake(m_pLayer->getContentSize().width,m_pLayer->getContentSize().height-G_SF(12));
}

/*
 设置TableView题卡一行的坐标
 */
CCSize QuestionCardUI::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(G_SF(230), getTableSize().height/3);
    
}

void QuestionCardUI::createTableViewCell(CCNode *cell,CCTableView *table,int index)
{
    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
    {
        CCLOG("Error: The cur scene is not the main scene!!!");
        return;
    }
    BackPackLayer* bl = (BackPackLayer*)pMainLayer->getChildByTag(BACKPACK);
    
    //卡牌个数
//    int nCardNumber = bl->m_bkData["card"].size();
    int nCardNumber = bl->m_pCardList.size();
    
    //题卡数目为零时
    if(nCardNumber == 0)
    {
        return;
    }
    
    //固定文字x
    std::string StrxLeft = "x ";
    
    int LeftNo=0;
    std::string LeftNumber;
    int LeftStart = 0;
    LeftNo = bl->m_pCardList[index*2];
    int LeftNumber2 = bl->getNumByNo(LeftNo);
    
    //转换用
    char Left[256];
    sprintf(Left, "%d", LeftNumber2);
    LeftNumber = Left;
    
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
    
    //左边题卡个数
    CCLabelTTF* labelLeft = CCLabelTTF::create(StrxLeft.append(LeftNumber).c_str(), "Arial", G_SF(30), CCSizeMake(G_SF(90), G_SF(40)), kCCTextAlignmentRight);
    labelLeft->setPosition(ccp(G_SF(165), G_SF(35)));
    spriteLeft->addChild(labelLeft);

    //添加题卡星数、查看按钮、题卡个数到题卡图片上
    cell->addChild(spriteLeft);
    
    //题卡数量为奇数时，不加载右侧最下面一张题卡
    if((index * 2 + 1) < nCardNumber)
    {
        //固定文字x
        std::string StrxRiht = "x ";
        
        int RightNo = 0;
        std::string RightNumber;
        int RightStart = 0;
        
        RightNo = bl->m_pCardList[index*2+1];
        int RightNumber2 = bl->getNumByNo(RightNo);
        
        //转换用
        char Right[256];
        sprintf(Right, "%d", RightNumber2);
        RightNumber = Right;
        
        RightStart = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[RightNo]->m_artQStarLevel;
        
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
        
        //右边题卡个数
        CCLabelTTF* labelRight = CCLabelTTF::create(StrxRiht.append(RightNumber).c_str(), "Arial", G_SF(30), CCSizeMake(G_SF(90), G_SF(40)), kCCTextAlignmentRight);
        labelRight->setPosition(ccp(G_SF(165), G_SF(35)));
        spriteRight->addChild(labelRight);
        
        //添加题卡星数、查看按钮、题卡个数到题卡图片上
        cell->addChild(spriteRight);
        
    }
    
}

//table cell选中
void QuestionCardUI::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
}

CCTableViewCell* QuestionCardUI::tableCellAtIndex(CCTableView *table, unsigned int idx)
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
unsigned int QuestionCardUI::numberOfCellsInTableView(CCTableView *table)
{
    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
    {
        CCLOG("Error: The cur scene is not the main scene!!!");
//        return;
    }
    BackPackLayer* bl = (BackPackLayer*)pMainLayer->getChildByTag(BACKPACK);
    
    //卡牌个数
//    int num = bl->m_bkData["card"].size();
    int num = bl->m_pCardList.size();
    
    
    if(m_layerColor != NULL){
        m_layerColor->removeFromParentAndCleanup(true);
    }
    
    //题卡个数为0的时候
    if(num == 0)
    {
        m_layerColor = CCLayerColor::create(ccc4(0, 0, 0, 150));
        m_layerColor->setContentSize(ccp(G_SF(480), G_SF(690)));
        m_layerColor->setPosition(ccp(G_SF(130), G_SF(40)));
        
        //提示文字
        CCLabelTTF* labelLeft = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("NoQuestionCard").c_str(), "Arial", G_SF(30), CCSizeMake(G_SF(320), G_SF(250)), kCCTextAlignmentCenter);
        labelLeft->setColor(ccc3(200, 100, 0));
        labelLeft->setPosition(ccp(m_layerColor->getContentSize().width*0.5, G_SF(550)));
        m_layerColor->addChild(labelLeft);
        
        Button *btnLeft = Button::createBtn("Image/BackPack/C_composite.png");
        btnLeft->setPosition(ccp(m_layerColor->getContentSize().width*0.5, G_SF(300)));
        btnLeft->setOnClickCallback(callfuncO_selector(QuestionCardUI::ToShop),this);
        m_layerColor->addChild(btnLeft);
        
        //按钮文字
        CCLabelTTF* btnStr = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("ToShop").c_str(), "Arial", G_SF(40), CCSizeMake(G_SF(120), G_SF(60)), kCCTextAlignmentCenter);
        btnStr->setColor(ccc3(0, 0, 0));
        btnStr->setPosition(ccp(G_SF(0), G_SF(0)));
        btnLeft->addChild(btnStr,10);
        
        this->addChild(m_layerColor);

        return 0;
    }else{
        //卡牌个数;
        if(num % 2 == 0)
        {
            return num/2;
            
        }else{
            return (num+1)/2;
            
        }
    }
    
}

void QuestionCardUI::ToShop()
{
    Json::Value value;
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_SHOPSHOW, value);
    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
    
    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
    {
        CCLOG("Error: The cur scene is not the main scene!!!");
        return;
    }
    BackPackLayer* bl = (BackPackLayer*)pMainLayer->getChildByTag(BACKPACK);
    
    bl->removeAllChildrenWithCleanup(true);
    bl->removeFromParentAndCleanup(true);
    
    pMainLayer->getChildByTag(500)->removeFromParentAndCleanup(true);
}




