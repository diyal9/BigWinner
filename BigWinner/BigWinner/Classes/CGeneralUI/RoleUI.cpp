#include "RoleUI.h"
#include "Button.h"
#include "LGameCommonDefine.h"

#include "BackPack.h"
#include "CLSceneManager.h"
#include "CLMainSceneLayer.h"
#include "JsonDataManager.h"
#include "CLNetWorkManager.h"
#include "CLChangeString.h"

#define OPENVIEW_TAG 1000

USING_NS_CC_EXT;

using std::string;
using namespace cocos2d;

bool RoleUI::init()
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

void RoleUI::getPipeInfo()
{
    
    //添加背景图片
    m_pLayer = CCSprite::create("Image/BackPack/BKShop1.png");
    
    m_tableView = CCTableView::create(this,getTableSize());
    m_tableView->setDirection(kCCScrollViewDirectionVertical);
    m_tableView->setPosition(ccp(G_SF(0), G_SF(7)));
    m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tableView->setTouchPriority(-10001);//20131020优先级设置
    
    m_tableView->setDelegate(this);//wangguolong
    
    m_pLayer->addChild(m_tableView);
    m_pLayer->setPosition(ccp(G_SF(370),G_SF(380)));
    this->addChild(m_pLayer);
    
    this->setTouchEnabled(true);
    
	m_tableView->reloadData();
    
}

/*
 设置TableView的大小
 */
CCSize RoleUI::getTableSize()
{
	
	return CCSizeMake(m_pLayer->getContentSize().width,m_pLayer->getContentSize().height-G_SF(12));
    
}

/*
 设置TableView角色一行的坐标
 */
CCSize RoleUI::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(G_SF(230), getTableSize().height/3);
    
}
/*
 创建角色信息
 */
void RoleUI::createTableViewCell(CCNode *cell,CCTableView *table,int index)
{
    
    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
    {
        CCLOG("Error: The cur scene is not the main scene!!!");
        return;
    }
    BackPackLayer* bl = (BackPackLayer*)pMainLayer->getChildByTag(BACKPACK);
    
    //角色数量
    int nCardNumber = bl->m_bkData["playerrole"].size();
    
    //角色数目为零时
    if(nCardNumber == 0)
    {
        return;
    }
    
    //tag计数
    int iTag = 0;
    
    iTag = index * 2;
    
    //wangguolong
    std::map<int,int>::iterator itBack = backMap.find(iTag + 1000);
    if(itBack != backMap.end()){
        
        //角色卡背面
        CCSprite *BackSprite = CCSprite::create("Image/BackPack/Back.png");
        //查看左边角色信息，左边坐标设置
        BackSprite->setPosition(ccp(G_SF(125),G_SF(113.5) ));
        cell->addChild(BackSprite,1,iTag + 1000);
        
        std::string Note;
        int No = 0;
        
        No = bl->m_bkData["playerrole"][iTag]["No"].asIntModestyle();
        
        //角色名称
        std::string ClassName = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artCaption;
        
        //角色卡描述
        Note = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artNote;
        
        //角色名称
        CCLabelTTF* name = CCLabelTTF::create(ClassName.c_str(), "Arial", G_SF(25));
        //角色说明文字设置为红色
        name->setColor(ccc3(220,100,0));
        name->setPosition(ccp(G_SF(10)+name->getContentSize().width*0.5, BackSprite->getContentSize().height - G_SF(35)));
        BackSprite->addChild(name);
        
        //角色说明文字
        CCLabelTTF* label = CCLabelTTF::create(Note.c_str(), "Arial", G_SF(25), CCSizeMake(BackSprite->getContentSize().width-G_SF(20), G_SF(150)), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
        //角色说明文字设置为红色
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
    
    //人物IconId
    int ArtIconId = 0;
    ArtIconId = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[bl->m_bkData["playerrole"][iTag]["No"].asIntModestyle()]->m_artIconId;
    
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
    btnLeft->setOnClickCallback(callfuncO_selector(RoleUI::openView),this);
    spriteLeft->addChild(btnLeft,1,iTag+100000);
    
    if((index * 2 + 1) < nCardNumber){
        iTag = 2 * index + 1;
        
        std::map<int,int>::iterator itBack = backMap.find(iTag + 1000);
        if(itBack != backMap.end()){
            
            //角色卡背面
            CCSprite *BackSprite = CCSprite::create("Image/BackPack/Back.png");
            //查看左边角色信息，左边坐标设置
            BackSprite->setPosition(ccp(G_SF(355), G_SF(113.5)));
            cell->addChild(BackSprite,1,iTag + 1000);
            
            std::string Note;
            int No = 0;
            
            No = bl->m_bkData["playerrole"][iTag]["No"].asIntModestyle();
            
            //角色名称
            std::string ClassName = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artCaption;
            
            //角色卡描述
            Note = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artNote;
            
            //角色名称
            CCLabelTTF* name = CCLabelTTF::create(ClassName.c_str(), "Arial", G_SF(25));
            //角色说明文字设置为红色
            name->setColor(ccc3(220,100,0));
            name->setPosition(ccp(G_SF(10)+name->getContentSize().width*0.5, BackSprite->getContentSize().height - G_SF(35)));
            BackSprite->addChild(name);
            
            //角色说明文字
            CCLabelTTF* label = CCLabelTTF::create(Note.c_str(), "Arial", G_SF(25), CCSizeMake(BackSprite->getContentSize().width-G_SF(20), G_SF(150)), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
            //角色说明文字设置为红色
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
        
        //人物IconId
        int ArtIconId = 0;
        ArtIconId = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[bl->m_bkData["playerrole"][iTag]["No"].asIntModestyle()]->m_artIconId;
        
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
        btnRight->setOnClickCallback(callfuncO_selector(RoleUI::openView), this);
        spriteRight->addChild(btnRight,1,iTag+100000);
        
    }
    
    
}

//table cell选中
void RoleUI::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
    //选中的cell  因为我们的cell里面装了2个Node  一个左边一个右边  所以我们取出cell没有用 要取出里面的两个sprite
	CCLOG("选中了: %i", cell->getIdx());
    
    //node是当前cell里面唯一的一个node 并且tag是在tableCellAtIndex方法里面你们定义的tag 把它取出来，接下来就是取左右两个sprite了
    CCNode* node = dynamic_cast<CCNode*>(cell->getChildByTag(1));
    
    //    //这里是测试 可以忽略
    //    CCArray* arr = node->getChildren();
    //    for (int i = 0; i < arr->count(); i++)
    //    {
    //        CCLog("tag:%d", dynamic_cast<CCNode*>(arr->objectAtIndex(i))->getTag());
    //
    ////        int t = dynamic_cast<CCNode*>(arr->objectAtIndex(i))->getTag();
    ////        CCLog("t:%d", t);
    ////        if(t >= 1000){
    ////            fag = false;
    ////        }
    //    }
    
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
    
    if(left != NULL && lspRect.containsPoint(lp)){
        
        n_Tag = left->getTag();
        
    }else if (right != NULL && rspRect.containsPoint(rp)){
        
        n_Tag = right->getTag();
        
    }else
    {
        CCLog("没有选中任何");
        return;
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
        return;
    }
    
}

CCTableViewCell* RoleUI::tableCellAtIndex(CCTableView *table, unsigned int idx)
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
unsigned int RoleUI::numberOfCellsInTableView(CCTableView *table)
{
    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    
    BackPackLayer* bl = (BackPackLayer*)pMainLayer->getChildByTag(BACKPACK);
    
    //角色数量
    int num = bl->m_bkData["playerrole"].size();
    
    if(m_layerColor != NULL){
        m_layerColor->removeFromParentAndCleanup(true);
    }
    
    //道具个数为0的时候
    if(num == 0)
    {
        m_layerColor = CCLayerColor::create(ccc4(0, 0, 0, 150));
        m_layerColor->setContentSize(ccp(G_SF(480), G_SF(690)));
        m_layerColor->setPosition(ccp(G_SF(130), G_SF(40)));
        
        //提示文字
        CCLabelTTF* labelLeft = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("NoRole").c_str(), "Arial", G_SF(30), CCSizeMake(G_SF(320), G_SF(140)), kCCTextAlignmentCenter);
        labelLeft->setColor(ccc3(200, 100, 0));
        labelLeft->setPosition(ccp(m_layerColor->getContentSize().width*0.5, G_SF(550)));
        m_layerColor->addChild(labelLeft);
        
        Button *btnLeft = Button::createBtn("Image/BackPack/C_composite.png");
        btnLeft->setPosition(ccp(m_layerColor->getContentSize().width*0.5, G_SF(300)));
        btnLeft->setOnClickCallback(callfuncO_selector(RoleUI::ToShop),this);
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

void RoleUI::ToShop()
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

//查看角色信息
void RoleUI::openView(CCNode* pSender)
{
 
    CCPoint btnWorldPoint = pSender->getParent()->convertToWorldSpace(pSender->getPosition());//获得按钮的世界坐标
    CCPoint tableBgWorld = m_pLayer->getParent()->convertToWorldSpace(m_pLayer->getPosition());//获得tableview背景的世界坐标
    
    float btnWorldPointY = btnWorldPoint.y; //按钮的世界坐标y
    float tbvYup = tableBgWorld.y + m_pLayer->getContentSize().height * 0.5;//tableview背景最上部分的世界坐标y
    float tbvYdown = tableBgWorld.y - m_pLayer->getContentSize().height * 0.5;//tableview背景最下部分的世界坐标y
    
    if (!(tbvYup > btnWorldPointY && tbvYdown<btnWorldPointY)) {
        return;//不在tableview范围内
    }
    
    //设置正面按钮消失不可用
    Button *button =   dynamic_cast<Button*>(pSender);
    button->setIsShow(false);
    
    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
    {
        CCLOG("Error: The cur scene is not the main scene!!!");
        return;
    }
    BackPackLayer* bl = (BackPackLayer*)pMainLayer->getChildByTag(BACKPACK);
    
    int No=0;
    std::string Note;
    
    No = bl->m_bkData["playerrole"][pSender->getParent()->getTag()]["No"].asIntModestyle();
    
    //角色卡描述
    Note = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artNote;
    
    //角色卡背面
    CCSprite *BackSprite = CCSprite::create("Image/BackPack/Back.png");
    
    if(pSender->getParent()->getTag() % 2 == 0)
    {
        //查看左边角色信息，左边坐标设置
        BackSprite->setPosition(ccp(G_SF(122.5), G_SF(113.5)));
    }else
    {
        
        BackSprite->setPosition(ccp(G_SF(355), G_SF(113.5)));
    }
    
    pSender->getParent()->getParent()->addChild(BackSprite,1,OPENVIEW_TAG + pSender->getParent()->getTag());
    BackSprite->setVisible(false);
    
    //添加信息卡牌tag到map中
    backMap[OPENVIEW_TAG + pSender->getParent()->getTag()] = OPENVIEW_TAG + pSender->getParent()->getTag();
    
    //角色名称
    std::string ClassName = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artCaption;

    //角色名称
    CCLabelTTF* name = CCLabelTTF::create(ClassName.c_str(), "Arial", G_SF(25));
    //角色说明文字设置为红色
    name->setColor(ccc3(220,100,0));
    name->setPosition(ccp(G_SF(10)+name->getContentSize().width*0.5, BackSprite->getContentSize().height - G_SF(35)));
    BackSprite->addChild(name);
    
    //角色说明文字
    CCLabelTTF* label = CCLabelTTF::create(Note.c_str(), "Arial", G_SF(25), CCSizeMake(BackSprite->getContentSize().width-G_SF(20), G_SF(150)), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    //角色说明文字设置为红色
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

void RoleUI::ClickView(CCNode* pSender,CCNode* pSender2,int tag)
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

void RoleUI::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);
}

void RoleUI::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool RoleUI::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    // 记录触摸起始点的位置
    m_touchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
    
    return true;
}


void RoleUI::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void RoleUI::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}




