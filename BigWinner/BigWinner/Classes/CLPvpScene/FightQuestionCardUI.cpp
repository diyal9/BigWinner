#include "FightQuestionCardUI.h"
#include "Button.h"
#include "RichLabel.h"
#include "LGameCommonDefine.h"

#include "FightBackPack.h"
#include "CLSceneManager.h"
#include "CLMainSceneLayer.h"
#include <JsonDataManager.h>
#include "CLPvpReady.h"
#include "DlgTiShiLayer.h"


USING_NS_CC_EXT;

using std::string;
using namespace cocos2d;

bool FightQuestionCardUI::init()
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

void FightQuestionCardUI::getPipeInfo()
{
    
    //添加背景图片
    pLayer2 = CCSprite::create("Image/PvpLayer/CP_SetBG.png");
    
    tableView = CCTableView::create(this,getTableSize());
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setPosition(ccp(G_SF(0), G_SF(12)));
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    tableView->setTouchPriority(-10001);//20131020优先级设置
    
    tableView->setDelegate(this);//wangguolong
    
    pLayer2->addChild(tableView,1,11);
    pLayer2->setPosition(ccp(G_SF(380),G_SF(445)));
    this->addChild(pLayer2);
    
	tableView->reloadData();
    
}

/*
 设置TableView的大小
 */
CCSize FightQuestionCardUI::getTableSize()
{
    return CCSizeMake(pLayer2->getContentSize().width,pLayer2->getContentSize().height-G_SF(18));
}

/*
 设置TableView题卡一行的坐标
 */
CCSize FightQuestionCardUI::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(G_SF(230), getTableSize().height/2.2);

}

void FightQuestionCardUI::createTableViewCell(CCNode *cell,CCTableView *table,int index)
{
    
    //卡牌个数
    int nCardNumber = CLSceneManager::sharedSceneManager()->GetUcardNum();
    
    //题卡数目为零时
    if(nCardNumber == 0)
    {
        return;
    }
    
    //tag计数
    int iTag = 0;
    
    iTag = index * 2;
    
    //固定文字x
    std::string StrxLeft = "x ";
    
    int LeftNo=0;
    std::string LeftNumber;
    int LeftStart = 0;
    
    LeftNo = CLSceneManager::sharedSceneManager()->GetUcardInfoFromIndex(iTag)->getnUcardNo();
    //不同星数卡牌数量
    LeftNumber = CLSceneManager::sharedSceneManager()->GetUcardInfoFromIndex(iTag)->getnUcardNumber();
    LeftStart = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[LeftNo]->m_artQStarLevel;
    
    //左边题卡图片
    CCSprite *spriteLeft = CCSprite::create("Image/BackPack/CardBackground.png");
    spriteLeft->setPosition(ccp(G_SF(125),G_SF(119) ));
    
    //题卡IconId
    int ArtIconId = 0;
    ArtIconId = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[LeftNo]->m_artIconId;
    
    //商品图标路径
    std::string LCardNameIcon_path = CCString::createWithFormat("Image/BackPack/CardIcon%d.png", ArtIconId)->getCString();
    
    //Icon
    CCSprite *pLeftIcon = CCSprite::create(LCardNameIcon_path.c_str());
    pLeftIcon->setPosition(ccp(spriteLeft->getContentSize().width*0.5, spriteLeft->getContentSize().height*0.5));
    spriteLeft->addChild(pLeftIcon);

    if(CLSceneManager::sharedSceneManager()->FindUcardStatusMap(LeftNo) != 0)
    {
        //wangguolong
        useMap[iTag + 10000] = iTag + 10000;
    
        
        CCSprite *pSpr = CCSprite::create("Image/PvpLayer/CP_Useing.png");
        pSpr->setPosition(ccp(spriteLeft->getContentSize().width*0.5, spriteLeft->getContentSize().height*0.5));
        spriteLeft->addChild(pSpr,10,10000 + iTag);
        
    }
    
    //左边题卡星数
    for(int left = 1 ; left <= LeftStart;left++)
    {
        //卡牌星星之间的距离
        CCSprite *spriteStartLeft = CCSprite::create("Image/BackPack/C_card_star.png");
        spriteStartLeft->setPosition( ccp(G_SF(48) + spriteStartLeft->getContentSize().width * 0.5 * ((left-1)*2 + 1), spriteLeft->getContentSize().height - G_SF(55)));
        spriteLeft->addChild(spriteStartLeft);
    }
    
    //左边题卡个数
    CCLabelTTF* labelLeft = CCLabelTTF::create(StrxLeft.append(LeftNumber).c_str(), "Arial",  G_SF(30), CCSizeMake(G_SF(90), G_SF(40)), kCCTextAlignmentRight);
    labelLeft->setPosition(ccp(G_SF(165), G_SF(35)));
    spriteLeft->addChild(labelLeft);
    
    //添加题卡星数、查看按钮、题卡个数到题卡图片上
    cell->addChild(spriteLeft,1,iTag);
    
    //题卡数量为奇数时，不加载右侧最下面一张题卡
    if((index * 2 + 1) < nCardNumber)
    {
        iTag = 2 * index + 1;
        
        //固定文字x
        std::string StrxRiht = "x ";
        
        int RightNo = 0;
        std::string RightNumber;
        int RightStart = 0;
        
        RightNo = CLSceneManager::sharedSceneManager()->GetUcardInfoFromIndex(iTag)->getnUcardNo();
        
        //不同星数卡牌数量
        RightNumber = CLSceneManager::sharedSceneManager()->GetUcardInfoFromIndex(iTag)->getnUcardNumber();
        
        RightStart = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[RightNo]->m_artQStarLevel;
        
        //右边题卡图片
        CCSprite *spriteRight = CCSprite::create("Image/BackPack/CardBackground.png");
        spriteRight->setPosition(ccp(G_SF(355), G_SF(119)));
        
        //道具IconId
        ArtIconId = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[RightNo]->m_artIconId;
        
        //商品图标路径
        std::string RCardNameIcon_path = CCString::createWithFormat("Image/BackPack/CardIcon%d.png", ArtIconId)->getCString();
        
        //Icon
        CCSprite *pRightIcon = CCSprite::create(RCardNameIcon_path.c_str());
        pRightIcon->setPosition(ccp(spriteRight->getContentSize().width*0.5, spriteRight->getContentSize().height*0.5));
        spriteRight->addChild(pRightIcon);
        
        if(CLSceneManager::sharedSceneManager()->FindUcardStatusMap(RightNo) != 0)
        {

            //wangguolong
            useMap[iTag + 10000] = iTag + 10000;
            
            CCSprite *pSpr = CCSprite::create("Image/PvpLayer/CP_Useing.png");
            pSpr->setPosition(ccp(spriteRight->getContentSize().width*0.5, spriteRight->getContentSize().height*0.5));
            spriteRight->addChild(pSpr,10,10000 + iTag);
            
        }
        
        //右边题卡星数
        for(int right = 1 ; right <= RightStart;right++)
        {
            //卡牌星星之间的距离
            CCSprite *spriteStartRight = CCSprite::create("Image/BackPack/C_card_star.png");
            spriteStartRight->setPosition(  ccp(G_SF(48) + spriteStartRight->getContentSize().width * 0.5 * ((right-1)*2 +1), spriteRight->getContentSize().height - G_SF(55)) );
            spriteRight->addChild(spriteStartRight);
        }
        
        //右边题卡个数
        CCLabelTTF* labelRight = CCLabelTTF::create(StrxRiht.append(RightNumber).c_str(), "Arial",  G_SF(30), CCSizeMake(G_SF(90), G_SF(40)), kCCTextAlignmentRight);
        labelRight->setPosition(ccp(G_SF(165), G_SF(35)));
        spriteRight->addChild(labelRight);

        
        //添加题卡星数、查看按钮、题卡个数到题卡图片上
        cell->addChild(spriteRight,1,iTag);
        
    }
    
    
}

//table cell选中
void FightQuestionCardUI::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
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
    
    int No = 0;
    No = CLSceneManager::sharedSceneManager()->GetUcardInfoFromIndex(n_Tag)->getnUcardNo();
    
    if(lspRect.containsPoint(lp) && useMap.size() < 3 && CLSceneManager::sharedSceneManager()->FindUcardStatusMap(No) == 0 && left != NULL){
        
        n_Tag = left->getTag();
        
        CCSprite *pSpr = CCSprite::create("Image/PvpLayer/CP_Useing.png");
        pSpr->setPosition(CCPoint(left->getContentSize().width*0.5, left->getContentSize().height*0.5));
        left->addChild(pSpr,10,10000 + n_Tag);
        //选择效果
        CCActionInterval* m_pAction = (CCActionInterval*)CCSequence::create(
                                                                            CCHide::create(),
                                                                            CCFadeTo::create(0.01, 255),
                                                                            CCScaleTo::create(0.01, 1.5,1.5),
                                                                            CCShow::create(),
                                                                            CCScaleTo::create(0.1, 1.0,1.0),
                                                                            NULL
                                                                            );
        pSpr->runAction(m_pAction);
        
        b_Useflg = false;
        useMap[n_Tag + 10000] = n_Tag + 10000;
        
        CLSceneManager::sharedSceneManager()->AddUcardStatusMap(No, No);
        
    }
    
    if (rspRect.containsPoint(rp) && useMap.size() < 3 && CLSceneManager::sharedSceneManager()->FindUcardStatusMap(No) == 0 && right != NULL){
        
        n_Tag = right->getTag();
        
        CCSprite *pSpr = CCSprite::create("Image/PvpLayer/CP_Useing.png");
        pSpr->setPosition(CCPoint(right->getContentSize().width*0.5, right->getContentSize().height*0.5));
        right->addChild(pSpr,10,10000 + n_Tag);
        
        //选择效果
        CCActionInterval* m_pAction = (CCActionInterval*)CCSequence::create(
                                                                            CCHide::create(),
                                                                            CCFadeTo::create(0.01, 255),
                                                                            CCScaleTo::create(0.01, 1.5,1.5),
                                                                            CCShow::create(),
                                                                            CCScaleTo::create(0.1, 1.0,1.0),
                                                                            NULL
                                                                            );
        pSpr->runAction(m_pAction);
        
        b_Useflg = false;
        useMap[n_Tag + 10000] = n_Tag + 10000;
        
        CLSceneManager::sharedSceneManager()->AddUcardStatusMap(No, No);
        
    }
    
    if(CLSceneManager::sharedSceneManager()->FindUcardStatusMap(No) != 0 && b_Useflg){
        
        if(lspRect.containsPoint(lp) && left != NULL){
            
            n_Tag = left->getTag() + 10000;
            
            
        }else if (rspRect.containsPoint(rp) && right != NULL){
            
            n_Tag = right->getTag() + 10000;
            
        }
        
        
        if(CLSceneManager::sharedSceneManager()->FindUcardStatusMap(No) != 0){
            
            if(lspRect.containsPoint(lp)){
                
                b_Useflg2 = true;
                
            }else if (rspRect.containsPoint(rp)){
                
                b_Useflg2 = false;
            }
            
            int ZERO = 0;
            if(b_Useflg2 && left != NULL){
                //移除左边添加的“使用中”
                left->removeChildByTag(n_Tag);
                useMap.erase(n_Tag);
                CLSceneManager::sharedSceneManager()->AddUcardStatusMap(No, ZERO);//wangguolong
                
                
            }else{
                if(right != NULL){
                    //移除右边添加的“使用中”
                    right->removeChildByTag(n_Tag);
                    useMap.erase(n_Tag);
                    CLSceneManager::sharedSceneManager()->AddUcardStatusMap(No, ZERO);//wangguolong
                }

                
            }
            
        } 
        
    }
    
    b_Useflg = true;
    
    if(lspRect.containsPoint(lp)){
        
        n_Tag = left->getTag() + 10000;
        
    }else if (rspRect.containsPoint(rp)){
        
        n_Tag = right->getTag() + 10000;
    }
    
    std::map<int,int>::iterator it10 = useMap.find(n_Tag);
    
    if(useMap.size() == 3 && it10 == useMap.end()){

        if(fagR){
            
            TiShiLayer::create("题卡最多只能带三种哟！");
            
            //设置不同时存在两个弹出层
            fagR = false;
            this->scheduleOnce(schedule_selector(FightQuestionCardUI::removeUI), 2.0);
        }
        
    }
    
}

void FightQuestionCardUI::removeUI()
{
    fagR = true;
}

CCTableViewCell* FightQuestionCardUI::tableCellAtIndex(CCTableView *table, unsigned int idx)
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
unsigned int FightQuestionCardUI::numberOfCellsInTableView(CCTableView *table)
{
    //卡牌个数
    int num = CLSceneManager::sharedSceneManager()->GetUcardNum();
    
    //卡牌个数
    //    int num = 25;
    if(num % 2 == 0)
    {
        return num/2;
        
    }else{
        return (num+1)/2;
        
    }
    
}

void FightQuestionCardUI::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);
}

void FightQuestionCardUI::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool FightQuestionCardUI::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    // 记录触摸起始点的位置
    m_touchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
    
    return true;
}


void FightQuestionCardUI::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void FightQuestionCardUI::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}



