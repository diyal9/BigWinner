#include "FightRoleUI.h"
#include "Button.h"
#include "RichLabel.h"
#include "LGameCommonDefine.h"

#include "FightBackPack.h"
#include "CLSceneManager.h"
#include "CLMainSceneLayer.h"
#include <JsonDataManager.h>
#include "DlgTiShiLayer.h"

#define OPENVIEW_TAG 1000

USING_NS_CC_EXT;

using std::string;
using namespace cocos2d;

bool FightRoleUI::init()
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

void FightRoleUI::getPipeInfo()
{
    
    //添加背景图片
    pLayer2 = CCSprite::create("Image/PvpLayer/CP_SetBG.png");
    
    tableView = CCTableView::create(this,getTableSize());
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setPosition(ccp(G_SF(0), G_SF(12)));
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    tableView->setTouchPriority(-10001);//20131020优先级设置
    
    tableView->setDelegate(this);//wangguolong
    
    pLayer2->addChild(tableView);

    pLayer2->setPosition(ccp(G_SF(380),G_SF(445)));
    this->addChild(pLayer2);
    
    this->setTouchEnabled(true);
    
	tableView->reloadData();
    
}

/*
 设置TableView的大小
 */
CCSize FightRoleUI::getTableSize()
{
    return CCSizeMake(pLayer2->getContentSize().width,pLayer2->getContentSize().height-G_SF(18));
}

/*
 设置TableView角色一行的坐标
 */
CCSize FightRoleUI::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(G_SF(230), getTableSize().height/2.2);
}
/*
 创建角色信息
 */
void FightRoleUI::createTableViewCell(CCNode *cell,CCTableView *table,int index)
{
    //角色数量
    int nCardNumber = CLSceneManager::sharedSceneManager()->GetUroleNum();
    
    //角色数目为零时
    if(nCardNumber == 0)
    {
        return;
    }
    
    //tag计数
    int iTag = 0;
    
    iTag = index * 2;
    int NoLeft = 0;
    
    NoLeft = CLSceneManager::sharedSceneManager()->GetUroleInfoFromIndex(iTag)->getnUroleNo();
    
    //wangguolong
    std::map<int,int>::iterator itBack = backMap.find(iTag + 1000);
    if(itBack != backMap.end()){
        
        //角色卡背面
        CCSprite *BackSprite = CCSprite::create("Image/BackPack/Back.png");
        //查看左边角色信息，左边坐标设置
        BackSprite->setPosition(ccp(G_SF(pLayer2->getContentSize().width/2 - BackSprite->getContentSize().width/2 - 2.5),G_SF(120) ));
        cell->addChild(BackSprite,1,iTag + 1000);
        
        //名称
        std::string ClassName = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[NoLeft]->m_artCaption;
        
        //名称
        CCLabelTTF* name = CCLabelTTF::create(ClassName.c_str(), "Arial", G_SF(25));
        name->setColor(ccc3(220,100,0));
        name->setPosition(ccp(G_SF(10)+name->getContentSize().width*0.5, BackSprite->getContentSize().height - G_SF(35)));
        BackSprite->addChild(name);
        
        std::string Note;
        
        //角色卡描述
        Note = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[NoLeft]->m_artNote;
        
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
    spriteLeft->setPosition(ccp(pLayer2->getContentSize().width/2 - spriteLeft->getContentSize().width/2 - G_SF(2.5),G_SF(119) ));
    cell->addChild(spriteLeft,1,iTag);
    spriteLeft->setVisible(backfag);
    backfag = true;
    
    //人物IconId
    int ArtIconId = 0;
    ArtIconId = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[NoLeft]->m_artIconId;
    
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
    btnLeft->setOnClickCallback(callfuncO_selector(FightRoleUI::openView),this);
    spriteLeft->addChild(btnLeft,1,iTag+100000);
    
    if(CLSceneManager::sharedSceneManager()->FindUroleStatusMap(NoLeft) != 0)
    {
        CCSprite *pSprLetf = CCSprite::create("Image/PvpLayer/CP_Useing.png");
        pSprLetf->setPosition(ccp(spriteLeft->getContentSize().width*0.5, spriteLeft->getContentSize().height*0.5));
        spriteLeft->addChild(pSprLetf,10,iTag + 10000);
        
        useMap[iTag + 10000] = iTag + 10000;
        
    }
    
    if((index * 2 + 1) < nCardNumber){
        iTag = 2 * index + 1;
        
        int NoRight = 0;
        
        NoRight = CLSceneManager::sharedSceneManager()->GetUroleInfoFromIndex(iTag)->getnUroleNo();
        
        std::map<int,int>::iterator itBack = backMap.find(iTag + 1000);
        if(itBack != backMap.end()){
            
            //角色卡背面
            CCSprite *BackSprite = CCSprite::create("Image/BackPack/Back.png");
            //查看左边角色信息，左边坐标设置
            BackSprite->setPosition(ccp(G_SF(355), G_SF(119)));
            cell->addChild(BackSprite,1,iTag + 1000);
            
            //名称
            std::string ClassName = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[NoRight]->m_artCaption;
            
            //名称
            CCLabelTTF* name = CCLabelTTF::create(ClassName.c_str(), "Arial", G_SF(25));
            name->setColor(ccc3(220,100,0));
            name->setPosition(ccp(G_SF(10)+name->getContentSize().width*0.5, BackSprite->getContentSize().height - G_SF(35)));
            BackSprite->addChild(name);
            
            std::string Note;
            
            //角色卡描述
            Note = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[NoRight]->m_artNote;
            
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
        spriteRight->setPosition(ccp(G_SF(355), G_SF(119)));
        cell->addChild(spriteRight,1,iTag);
        spriteRight->setVisible(backfag);
        backfag = true;
        
        ArtIconId = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[NoRight]->m_artIconId;
        
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
        btnRight->setOnClickCallback(callfuncO_selector(FightRoleUI::openView), this);
        spriteRight->addChild(btnRight,1,iTag+100000);
        
        if(CLSceneManager::sharedSceneManager()->FindUroleStatusMap(NoRight) != 0)
        {
            CCSprite *pSprRight = CCSprite::create("Image/PvpLayer/CP_Useing.png");
            pSprRight->setPosition(ccp(spriteRight->getContentSize().width*0.5, spriteRight->getContentSize().height*0.5));
            spriteRight->addChild(pSprRight,10,iTag + 10000);
            
            useMap[iTag + 10000] = iTag + 10000;
        }
        
    }
    
    
}

void FightRoleUI::removeUI()
{
    fagR = true;
}

//table cell选中
void FightRoleUI::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
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
    No = CLSceneManager::sharedSceneManager()->GetUroleInfoFromIndex(n_Tag)->getnUroleNo();
    
    //判断选中的是卡片正面还是卡牌背面
    if(dynamic_cast<CCNode*>(node->getChildByTag(n_Tag)) && dynamic_cast<CCNode*>(node->getChildByTag(n_Tag+1000)))
    {
        
        CCLog("选中背面");
        
        CCSprite* pSender = dynamic_cast<CCSprite*>(node->getChildByTag(backMap[n_Tag+1000]));
        CCSprite* pSender2 = dynamic_cast<CCSprite*>(node->getChildByTag(n_Tag));
        
        //返回正面
        ClickView(pSender,pSender2,n_Tag);
        
    }else{
        int nMap =0;
        std::map<int,int>::iterator it = useMap.find(n_Tag + 10000);
        for(it=useMap.begin();it!=useMap.end();++it)
        {
            nMap = it->second;
        }
        
        if(lspRect.containsPoint(lp) && nMap == 0 && left != NULL){
            
            n_Tag = left->getTag();
            
            CCSprite *pSpr = CCSprite::create("Image/PvpLayer/CP_Useing.png");
            pSpr->setPosition(ccp(left->getContentSize().width*0.5, left->getContentSize().height*0.5));
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
            
            CLSceneManager::sharedSceneManager()->AddUroleStatusMap(No, No);
            
        }
        
        if (rspRect.containsPoint(rp) && nMap == 0 && right != NULL){
            
            n_Tag = right->getTag();
            
            CCSprite *pSpr = CCSprite::create("Image/PvpLayer/CP_Useing.png");
            pSpr->setPosition(ccp(right->getContentSize().width*0.5, right->getContentSize().height*0.5));
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
            
            CLSceneManager::sharedSceneManager()->AddUroleStatusMap(No, No);
            
        }
        
        if(CLSceneManager::sharedSceneManager()->FindUroleStatusMap(No) != 0 && b_Useflg){
            
            if(lspRect.containsPoint(lp)){
                
                n_Tag = left->getTag() + 10000;
                
                
            }else if (rspRect.containsPoint(rp)){
                
                n_Tag = right->getTag() + 10000;
                
            }
            
            
            if(CLSceneManager::sharedSceneManager()->FindUroleStatusMap(No) != 0){
                
                if(lspRect.containsPoint(lp)){
                    
                    b_Useflg2 = true;
                    
                }else if (rspRect.containsPoint(rp)){
                    
                    b_Useflg2 = false;
                }
                
                int ZERO = 0;
                if(b_Useflg2){
                    //移除左边添加的“使用中”
                    left->removeChildByTag(n_Tag);
                    useMap.erase(n_Tag);
                    CLSceneManager::sharedSceneManager()->AddUroleStatusMap(No, ZERO);//wangguolong
                    
                    
                }else{
                    //移除右边添加的“使用中”
                    right->removeChildByTag(n_Tag);
                    useMap.erase(n_Tag);
                    CLSceneManager::sharedSceneManager()->AddUroleStatusMap(No, ZERO);//wangguolong
                    
                    
                }
                
            }
            
        }
        
        b_Useflg = true;//wangguolong
        
        if(nMap != 0 && useMap.size() == 1){

            if(fagR){
                
                TiShiLayer::create("只能选择1种角色参战哟！");
                
                //设置不同时存在两个弹出层
                fagR = false;
                this->scheduleOnce(schedule_selector(FightRoleUI::removeUI), 2.0);
            }
            
        }
        
    }
    
}


CCTableViewCell* FightRoleUI::tableCellAtIndex(CCTableView *table, unsigned int idx)
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
unsigned int FightRoleUI::numberOfCellsInTableView(CCTableView *table)
{
    //角色数量
    int num = CLSceneManager::sharedSceneManager()->GetUroleNum();
    
    if(num % 2 == 0)
    {
        return num/2;
        
    }else{
        return (num+1)/2;
        
    }
    
}

//查看角色信息
void FightRoleUI::openView(CCNode* pSender)
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
    
    No = CLSceneManager::sharedSceneManager()->GetUroleInfoFromIndex(pSender->getParent()->getTag())->getnUroleNo();
    
    //角色卡描述
    Note = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artNote;
    
    //角色卡背面
    CCSprite *BackSprite = CCSprite::create("Image/BackPack/Back.png");
    
    if(pSender->getParent()->getTag() % 2 == 0)
    {
        //查看左边角色信息，左边坐标设置
        BackSprite->setPosition(ccp(G_SF(122.5), G_SF(119)));
    }else
    {
        
        BackSprite->setPosition(ccp(G_SF(355), G_SF(119)));
    }
    
    pSender->getParent()->getParent()->addChild(BackSprite,1,OPENVIEW_TAG + pSender->getParent()->getTag());
    BackSprite->setVisible(false);
    
    //添加信息卡牌tag到map中
    backMap[OPENVIEW_TAG + pSender->getParent()->getTag()] = OPENVIEW_TAG + pSender->getParent()->getTag();
    
    //名称
    std::string ClassName = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artCaption;
    
    //名称
//    CCLabelTTF* name = CCLabelTTF::create(ClassName.c_str(), "Arial",  G_SF(25), CCSizeMake(BackSprite->getContentSize().width-G_SF(20), G_SF(25)), kCCTextAlignmentLeft);

    CCLabelTTF* name = CCLabelTTF::create(ClassName.c_str(), "Arial", G_SF(25));
    name->setColor(ccc3(220,100,0));
    name->setPosition(ccp(G_SF(10)+name->getContentSize().width*0.5, BackSprite->getContentSize().height - G_SF(35)));
    BackSprite->addChild(name);
    
    //角色说明文字
//    CCLabelTTF* label = CCLabelTTF::create(Note.c_str(), "Arial",  G_SF(25), CCSizeMake(BackSprite->getContentSize().width-G_SF(20), G_SF(120)), kCCTextAlignmentLeft);
    
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

void FightRoleUI::ClickView(CCNode* pSender,CCNode* pSender2,int tag)
{
    
    //设置正面按钮显示可用
    Button *button =   dynamic_cast<Button*>(pSender2->getChildByTag(tag+100000));
    button->setIsShow(true);
    
    if(m_touchPoint.x < G_SF(365))
    {
        //查看左边角色信息，左边坐标设置
        pSender->setPosition(ccp(G_SF(122.5), G_SF(120)));
        
    }else if(m_touchPoint.x >= G_SF(387))
    {
        
        pSender->setPosition(ccp(G_SF(355), G_SF(120)));
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

void FightRoleUI::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);
}

void FightRoleUI::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool FightRoleUI::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    // 记录触摸起始点的位置
    m_touchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
    
    return true;
}


void FightRoleUI::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void FightRoleUI::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}




