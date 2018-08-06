//
//  CompoundSceneMain.cpp
//  DiyalStudy
//
//  Created by YinYanlong on 13-10-13.
//
//

#include "CompoundSceneMain.h"
//#include <string>
//#include <sstream>
#include "BackPack.h"
#include "CLSceneManager.h"
#include "CLMainSceneLayer.h"
#include "JsonDataManager.h"
#include "CLNetWorkManager.h"
#include "CLChangeString.h"
#include "DlgTiShiLayer.h"
#include "CardSprite.h"
#include "CLAnimationManager.h"
#include "CLSoundManager.h"


USING_NS_CC;
USING_NS_CC_EXT;

using std::string;
using namespace Json;

CompoundSceneMain::CompoundSceneMain()
{
    m_sprList = NULL;
    cardWeight = 224;
    cardHeight = 228;
    hadAddCardNum = 0;
}

CompoundSceneMain::~CompoundSceneMain()
{
    
}

bool CompoundSceneMain::init()
{
    if ( !CCLayerColor::initWithColor(ccc4(0,0,0,0)) )
    {
        return false;
    }
    
    if (this->getChildrenCount() > 0) //初始化
        this->removeAllChildrenWithCleanup(true);
    
    tableView = NULL;
    tableViewBg = CCSprite::create(sc_tableview_bg_Path);
    this->addChild(tableViewBg);
    //    tableViewBg->setPosition(ccp(G_SF(365),G_SF(495)));
    tableViewBg->setPosition(ccp(G_SF(370),G_SF(504)));
    
    cardNumGet(); //服务器卡牌数取得
    
    getPipeInfo(); //tableview创建
    
    //题卡选中区域添加
    m_sprList = CCSprite::create(sc_cardListBg_bg_Path);
    m_sprList->setPosition( ccp(tableViewBg->getPositionX(),
                                tableViewBg->getPositionY() - tableViewBg->getContentSize().height * 0.5 - m_sprList->getContentSize().height * 0.5 - G_SF(1)));
    this->addChild(m_sprList);
    
    //合成按钮添加
    Button *combtn = Button::createBtn(sc_compoundBtn_Path);
    combtn->setPosition(ccp(tableViewBg->getPositionX(),
                            m_sprList->getPositionY() - m_sprList->getContentSize().height * 0.5 - combtn->getContentSize().height * 0.5 - G_SF(20)));
    combtn->setOnClickCallback(menu_selector(CompoundSceneMain::compoundCardSuccess),this);
    this->addChild(combtn,1,3);
    
    //开始合成文字
    CCSprite *hcwenzi = CCSprite::create("Image/BackPack/StartCompound.png");
    hcwenzi->setPosition(ccp(G_SF(0), G_SF(0)));
    combtn->addChild(hcwenzi,1,4);
    
    return true;
}


void CompoundSceneMain::getPipeInfo()
{
    
    if (tableView)
        tableView->removeFromParentAndCleanup(true);
    
	tableView = CCTableView::create(this,getTableSize());
	tableView->setDirection(kCCScrollViewDirectionVertical);

    tableView->setPosition(ccp(G_SF(0), G_SF(7)));
    tableView->setTouchPriority(tPri_Btn);
	tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    tableViewBg->addChild(tableView,1,1);
    tableView->setDelegate(this);
	tableView->reloadData();
    
}

/* table的大小 */
CCSize CompoundSceneMain::getTableSize()
{
    return CCSizeMake(tableViewBg->getContentSize().width, G_SF(cardHeight) * 2 + G_SF(3));
}

/* 每个cell的大小 */
CCSize CompoundSceneMain::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(getTableSize().width, getTableSize().height * 0.5);
}

void CompoundSceneMain::createTableViewCell(CCNode *cell,cocos2d::extension::CCTableView *table,int index)
{
    
    int cardIndex = 0;  //计算卡牌Tag用，每次根据index计算。
    cardIndex = index * 2;
    CCString *str_index = CCString::createWithFormat("%d", mapKeyGetValue(cardTNumMap, cardIndex) ); //取得卡片数量
    
    //左边UI编辑
    CCSprite *bgLeft = CCSprite::create(sc_card_bg_Path);
    bgLeft->setPosition( ccp(G_SF(125),getTableSize().height * 0.25 ) );
    
    //题卡IconId
    int ArtIconId = 0;
    ArtIconId = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[mapKeyGetValue(cardNoForTagMap, cardIndex)]->m_artIconId;
    
    //商品图标路径
    std::string LCardNameIcon_path = CCString::createWithFormat("Image/BackPack/CardIcon%d.png", ArtIconId)->getCString();
    
    //Icon
    CCSprite *pLeftIcon = CCSprite::create(LCardNameIcon_path.c_str());
    pLeftIcon->setPosition(ccp(bgLeft->getContentSize().width*0.5, bgLeft->getContentSize().height*0.5));
    bgLeft->addChild(pLeftIcon);
    
    
    int starLevel = mapKeyGetValue(starLevelMap, cardIndex);
    for (int i = 1; i <= starLevel; i++) {  //星处理
        CCSprite *star = CCSprite::create(sc_star_Path);
        star->setPosition(  ccp(G_SF(48) + star->getContentSize().width * 0.5 * ((i-1)*2 + 1), bgLeft->getContentSize().height - G_SF(55)) );
        bgLeft->addChild(star);
    }
    
    CCLabelTTF* pNum = CCLabelTTF::create(str_index->getCString(), "Arial", G_SF(30), CCSizeMake(G_SF(60), G_SF(30)), kCCTextAlignmentRight);//文本数字
    pNum->setPosition(ccp(G_SF(bgLeft->getContentSize().width - pNum->getContentSize().width * 0.5 - G_SF(15)), G_SF(35)));
    bgLeft->addChild(pNum,0,TAG_UPCARD_TTF);
    
    CCLabelTTF* pNumLabel = CCLabelTTF::create("x", "Arial", G_SF(30), CCSizeMake(G_SF(30), G_SF(30)), kCCTextAlignmentCenter);
    pNumLabel->setPosition(ccp(pNum->getPositionX() - pNumLabel->getContentSize().width * 0.5, G_SF(35)));
    bgLeft->addChild(pNumLabel);
    
    cell->addChild(bgLeft,0,cardIndex);
    
    if (isOddNum && (index == (lineNum - 1) ))//奇数卡牌个数最后一行的右边没有卡
    {
        return;
    } else {
        
        //右边UI编辑
        CCSprite *bgRight = CCSprite::create(sc_card_bg_Path);
        bgRight->setPosition( ccp(G_SF(355), getTableSize().height * 0.25) );

        
        cardIndex = index * 2 + 1;
        CCString *str_index2 = CCString::createWithFormat("%d", mapKeyGetValue(cardTNumMap, cardIndex)); //取得卡片数量
        
        int ArtIconId = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[mapKeyGetValue(cardNoForTagMap, cardIndex)]->m_artIconId;
        
        //商品图标路径
        std::string RCardNameIcon_path = CCString::createWithFormat("Image/BackPack/CardIcon%d.png", ArtIconId)->getCString();
        
        //Icon
        CCSprite *pRightIcon = CCSprite::create(RCardNameIcon_path.c_str());
        pRightIcon->setPosition(ccp(bgRight->getContentSize().width*0.5, bgRight->getContentSize().height*0.5));
        bgRight->addChild(pRightIcon);
        
        int starLevel2 = mapKeyGetValue(starLevelMap, cardIndex);
        for (int i = 1; i <= starLevel2; i++) {
            CCSprite *star2 = CCSprite::create(sc_star_Path);
            star2->setPosition(  ccp(G_SF(48) + star2->getContentSize().width * 0.5 * ((i-1)*2 + 1), bgRight->getContentSize().height - G_SF(55)) );
            bgRight->addChild(star2);
        }
        
        CCLabelTTF* pNum2 = CCLabelTTF::create(str_index2->getCString(), "Arial", G_SF(30), CCSizeMake(G_SF(60), G_SF(30)), kCCTextAlignmentRight);
        pNum2->setPosition(ccp(bgRight->getContentSize().width - pNum2->getContentSize().width * 0.5 - G_SF(15), G_SF(35)));
        bgRight->addChild(pNum2,0,TAG_UPCARD_TTF);
        
        CCLabelTTF* pNumLabel2 = CCLabelTTF::create("x", "Arial", G_SF(30), CCSizeMake(G_SF(30), G_SF(30)), kCCTextAlignmentCenter);
        pNumLabel2->setPosition(ccp(pNum2->getPositionX() - pNumLabel2->getContentSize().width * 0.5, G_SF(35)));
        bgRight->addChild(pNumLabel2);
        
        cell->addChild(bgRight,0,cardIndex);
        
    }
    
}

void CompoundSceneMain::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    //    //add by gaoyuan for you
    //    //注意
    //    //首先在创建tableview方法getPipeInfo里面添加一句话 tableView->setDelegate(this);
    //
    //    //选中的cell  因为我们的cell里面装了2个Node  一个左边一个右边  所以我们取出cell没有用 要取出里面的两个sprite
    //	CCLOG("选中了: %i", cell->getIdx());
    
    //node是当前cell里面唯一的一个node 并且tag是在tableCellAtIndex方法里面你们定义的tag 把它取出来，接下来就是取左右两个sprite了
    CCNode* node = dynamic_cast<CCNode*>(cell->getChildByTag(1));
    
    //    //这里是测试 可以忽略
    //    CCArray* arr = node->getChildren();
    //    for (int i = 0; i < arr->count(); i++)
    //    {
    //        CCLog("tag:%d", dynamic_cast<CCNode*>(arr->objectAtIndex(i))->getTag());
    //
    //    }
    
    CCSprite* left = dynamic_cast<CCSprite*>(node->getChildByTag(cell->getIdx()*2)); //左边题卡
    
    if (left && touchSprite(left)) {
        menuCardBtnAddCall(left);
    }
    
    CCSprite* right = dynamic_cast<CCSprite*>(node->getChildByTag(cell->getIdx()*2 + 1 )); //右边题卡
    
    if (right && touchSprite(right)) {
        menuCardBtnAddCall(right);
    }
    
}

/* 设定每一个cell */
CCTableViewCell* CompoundSceneMain::tableCellAtIndex(CCTableView *table, unsigned int idx)
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

//根据题卡个数，计算cell个数
unsigned int CompoundSceneMain::numberOfCellsInTableView(CCTableView *table)
{
    int num = tmpCardNum;
    if (num % 2 == 0) {
        lineNum = num/2;
        isOddNum = false;
        return num/2;
    } else {
        lineNum = (num+1)/2;
        isOddNum = true;
        return (num+1)/2;
    }
    
}

/* 卡片添加回调函数 */
void CompoundSceneMain::menuCardBtnAddCall(CCNode *node)
{
    CCPoint btnWorldPoint = node->getParent()->convertToWorldSpace(node->getPosition());//获得按钮的世界坐标
    CCPoint tableBgWorld = tableViewBg->getParent()->convertToWorldSpace(tableViewBg->getPosition());//获得tableview背景的世界坐标
    
    float btnWorldPointY = btnWorldPoint.y; //按钮的世界坐标y
    float tbvYup = tableBgWorld.y + tableViewBg->getContentSize().height * 0.5;//tableview背景最上部分的世界坐标y
    float tbvYdown = tableBgWorld.y - tableViewBg->getContentSize().height * 0.5;//tableview背景最下部分的世界坐标y
    
    if (!(tbvYup > btnWorldPointY && tbvYdown<btnWorldPointY)) {
        return;//不在tableview范围内
    }
    
    if (hadAddCardNum >= 5) {
        
        TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("ComAddCheckFiveStr"));
        return;//添加卡牌数大于5。
    }
    
    if (!updateQuantityLabel(node, true)) {
        return;//或选中卡牌的数量为0。
    }
    
    Button *sCardBtn  = Button::createBtn(sc_smallCard_Path);
    sCardBtn->setOnClickCallback(menu_selector(CompoundSceneMain::menuCardBtnDelCall),this);
    sCardBtn->setPosition( ccp(xPosionGet(hadAddCardNum), m_sprList->getContentSize().height/2));
    
    //添加星
    int starLevel = mapKeyGetValue(starLevelMap, node->getTag()); //添加的小卡牌的星，根据所在卡牌Tag去取得
    for (int i = 1; i <= starLevel; i++) {
        
        CCSprite *star = CCSprite::create(sc_star_Path);
        star->setScale(0.4);
        star->setPosition(  ccp(G_SF(23) + star->getContentSize().width * 0.4 * ((i-1) - 1) - sCardBtn->getContentSize().width * 0.4, sCardBtn->getContentSize().height*0.5 - star->getContentSize().height * 0.5 - G_SF(14)) );
        sCardBtn->addChild(star,11);
    }
    
    CCLog("选中卡牌->%d", mapKeyGetValue(cardNoForTagMap, node->getTag()));
    
    //题卡IconId
    int ArtIconId = 0;
    ArtIconId = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[mapKeyGetValue(cardNoForTagMap, node->getTag())]->m_artIconId;
    
    //商品图标路径
    std::string LCardNameIcon_path = CCString::createWithFormat("Image/BackPack/CardIcon%d.png", ArtIconId)->getCString();
    
    //Icon
    CCSprite *pLeftIcon = CCSprite::create(LCardNameIcon_path.c_str());
    pLeftIcon->setPosition(ccp(G_SF(0),G_SF(0)));
    sCardBtn->addChild(pLeftIcon, 10);
    pLeftIcon->setScale(0.4f);
    
    m_sprList->addChild(sCardBtn,0,node->getTag());
    
    hadAddCardNum++;//记录当前添加的卡片个数
    
    modifyMap(false, node->getTag());//修改小卡片题卡在Map中的数量
    
    //音效
    CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_BtnClick.wav");
    
}


/* 回调函数，卡片移除 */
void CompoundSceneMain::menuCardBtnDelCall(CCNode *node)
{
    //卡片移除、整体左移动逻辑
    CCArray * pChildrenArray = m_sprList->getChildren();
    CCSprite* child = NULL;
    CCObject* pObject = NULL;
    
    modifyMap(true, node->getTag());//修改删除卡牌在Map中的数量
    
    if (node)
        node->removeFromParent();//从父节点删除
    
    //重新排列卡牌位置
    int cardAtListIndex = 0;  //列表中卡牌号
    CCARRAY_FOREACH(pChildrenArray, pObject)
    {
        cardAtListIndex++;
        
        child = (CCSprite*)pObject;
        
        if(!child)
            break;
        
        child->setPosition( ccp(xPosionGet(cardAtListIndex - 1), child->getPositionY()) );
    }
    
    getPipeInfo();//删除卡片后，刷新Tableview
    
    hadAddCardNum--;//删除卡片后，已选中卡计数器减1
}


/* 取得服务器中卡牌信息 */
void CompoundSceneMain::cardNumGet()
{
    
//    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
//    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
//    {
//        CCLOG("Error: The cur scene is not the main scene!!!");
//        return;
//    }
//    BackPackLayer* bl = (BackPackLayer*)pMainLayer->getChildByTag(BACKPACK);
//    
//    int number = bl->m_bkData["card"].size();//数组个数
//    
//    tmpCardNum = number; //总卡牌数量
//    CCLog("CardNumSunm->%d", tmpCardNum);
//    
//    for (int i = 0; i< number; i++)
//    {
//        
//        int NO = bl->m_bkData["card"][i]["No"].asIntModestyle();
//        int Number = bl->m_bkData["card"][i]["Number"].asIntModestyle();
//        
//        cardTNumMap[i] = Number;//卡牌数量存储
//        
//        cardNoForTagMap[i] = NO;//卡牌No key的存储
//        
//        starLevelMap[i] = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[NO]->m_artQStarLevel;//星级存储
//        
//        //        CCLog("CardInfo-> NO:%d  Number:%d  starLevel:%d destri:%s artType:%d", NO, Number, JsonDataManager::shardJDManager()->m_vCardJueseDaoju[NO]->m_artQStarLevel
//        //              ,JsonDataManager::shardJDManager()->m_vCardJueseDaoju[NO]->m_artNote.c_str()
//        //              ,JsonDataManager::shardJDManager()->m_vCardJueseDaoju[NO]->m_artType
//        //              );
//        
//    }
    
    
    
    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
    {
        CCLOG("Error: The cur scene is not the main scene!!!");
        return;
    }
    BackPackLayer* bl = (BackPackLayer*)pMainLayer->getChildByTag(BACKPACK);

    int number = bl->m_pCardList.size();//数组个数

    tmpCardNum = number; //总卡牌数量
    CCLog("CardNumSunm->%d", tmpCardNum);

    for (int i = 0; i< number; i++)
    {
        int NO = bl->m_pCardList[i];
        
        int Number = bl->getNumByNo(NO);

        cardTNumMap[i] = Number;//卡牌数量存储

        cardNoForTagMap[i] = NO;//卡牌No key的存储

        starLevelMap[i] = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[NO]->m_artQStarLevel;//星级存储
    }
    
}

/* Map中取得卡牌数量 */
int CompoundSceneMain::mapKeyGetValue(std::map<int, int> map, int key)
{
    std::map<int,int>::iterator it = map.find(key);
    if (it != cardTNumMap.end()) {
        return (*it).second;
        
    } else {
        return NULL;
    }
    
}

//map值加减
void CompoundSceneMain::modifyMap(bool isAddNum, int key)
{
    
    if (isAddNum) {
        cardTNumMap[key] = cardTNumMap[key] + 1;
    } else {
        cardTNumMap[key] = cardTNumMap[key] - 1;
    }
}

/* 卡牌数量更新 */
bool CompoundSceneMain::updateQuantityLabel(CCNode *node, bool isAdd)
{
    CCLabelTTF *infoLabel;
    
    infoLabel = (CCLabelTTF *) node->getChildByTag(TAG_UPCARD_TTF);
    
    int nCardNum = mapKeyGetValue(cardTNumMap, node->getTag());
    
    if (nCardNum == 0) {
        return false;
    }
    
    char str[20] = {0};
    sprintf(str, "%u", nCardNum - 1);
    infoLabel->setString(str);
    
    return true;
}

/* 合成按钮按下逻辑处理 */
void CompoundSceneMain::compoundCardSuccess()
{
    
    if (m_sprList->getChildrenCount() < 2) {
        TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("ComAddCheckTwoStr"));
        return;//判断是否有选中卡牌
    }
    
    //组装json
    Value data; //发送内容
    
    CCArray * pChildrenArray = m_sprList->getChildren();
    CCSprite* child = NULL;
    CCObject* pObject = NULL;
    
    int cardAtListIndex;  //列表中的第几个卡片
    CCARRAY_FOREACH(pChildrenArray, pObject)
    {
        cardAtListIndex++;
        
        child = (CCSprite*)pObject;
        if(!child)
            break;
        //        CCLog("cardInfoAbout->%d,%d", child->getTag(), mapKeyGetValue(cardNoForTagMap, child->getTag()));
        data["card"].append(mapKeyGetValue(cardNoForTagMap, child->getTag()));
    }
    
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_CARDCOMPOSE, data);
    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
    
}

/* 列表卡片x坐标计算 */
int CompoundSceneMain::xPosionGet(int cellNum)
{
    //按照卡牌的相对比例计算，背景图－10（前后空隙） 分成10等分， add的图片按照奇数等比设置x坐标 //前端空出5px 每个图片起点加上5
    return ((m_sprList->getContentSize().width  - G_SF(10)) / 10) * (cellNum * 2 + 1) + G_SF(5);
}

//TODO Map测试
//void CompoundSceneMain::showMap()
//{
//    for (int i = 0; i < tmpCardNum; i++) {
//
//        CCLog("this_get%d", mapKeyGetValue(cardTNumMap, i));
//    }
//
//}

void CompoundSceneMain::_CardHeCallBack(Json::Value &data)
{
    
    tmpCardNum = data["card"].size(); //总卡牌数量
    CCLog("CardNumSunm->%d", tmpCardNum);
    
    //背包卡牌信息刷新
    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene)
    {
        CCLOG("Error: The cur scene is not the main scene!!!");
        return;
    }
    BackPackLayer* bl = (BackPackLayer*)pMainLayer->getChildByTag(BACKPACK);
    bl->cardInfoSet(data);
    
    cardTNumMap.clear();
    cardNoForTagMap.clear();
    starLevelMap.clear();
    
    int successNo; //合成成功的cardNo
    int succStarLevel; //星级
    
    for (int i = 0; i< bl->m_pCardList.size(); i++)
    {
        
        int NO = bl->m_pCardList[i];
        int Number = bl->getNumByNo(NO);
        int starLevel = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[NO]->m_artQStarLevel;
        
        if(i == 0){ //json中第一个cardno为合成成功的no
            successNo = NO;
            succStarLevel = starLevel;
        }

        cardTNumMap[i] = Number; //卡牌数量存储
        cardNoForTagMap[i] = NO;//卡牌No key的存储
        starLevelMap[i] = starLevel;//星级存储
        
    }
    

    if (m_sprList)
        m_sprList->removeAllChildrenWithCleanup(true); //初始化操作
    
    hadAddCardNum = 0; //选中题卡个数置0，必须步骤
    
    getPipeInfo();  //刷新
    
    CompoundSucLayer *comp = CompoundSucLayer::create(successNo, succStarLevel);
    comp->setPosition(ccp(G_SF(-10), G_SF(0)));
    this->addChild(comp, 50);
    
}

bool CompoundSceneMain::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_tPoint = pTouch->getLocation(); //获得触摸点位置（转换成GL座标）

    return true; //吞噬触摸
}

void CompoundSceneMain::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);
}

void CompoundSceneMain::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool CompoundSceneMain::touchSprite(CCSprite* sprite)
{
    
    bool bRet = false;
    
    CCSize sprSize = sprite->getContentSize();
    //相对于精灵锚点坐标，size
    CCRect spRect = CCRectMake(-sprSize.width * 0.5, -sprSize.height * 0.5, sprSize.width, sprSize.height);

    CCPoint p = sprite->convertToNodeSpaceAR(m_tPoint); //相对于精灵的锚点坐标
    
    
    if (spRect.containsPoint(p)) {
        bRet = true;
    }

    
    return bRet;
}



CompoundSucLayer* CompoundSucLayer::create(int succCardNo, int succStarLevel)
{
    CompoundSucLayer* pCell = new CompoundSucLayer();
    if(pCell && pCell->init(succCardNo, succStarLevel))
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

bool CompoundSucLayer::init(int succCardNo, int succStarLevel)
{
    if ( !CCLayerColor::initWithColor(ccc4(0,0,0,191.5)) )
    {
        return false;
    }
    
   	CCSize size = CCDirector::sharedDirector()->getgSceneSize();

    //题卡title
    CCSprite *sTitleLab = CCSprite::create("Image/BackPack/A_CompandSucc.png");  //title
    sTitleLab->setPosition(ccp(size.width * 0.5, size.height * 0.65));
    this->addChild(sTitleLab);

    CardSprite *pcard = CardSprite::create(succCardNo);
    pcard->setPosition(ccp(this->getContentSize().width*0.5, this->getContentSize().height*0.4));
    pcard->setScale(1.3);
    this->addChild(pcard,1);
    
    //星星播放效果
    std::string sFile;
    sFile = "flicker";
    
    CCAnimation* animation = CLAnimationManager::sharedAnimationManager()->animationWithName(sFile.c_str(),0.041);
    CCSprite* magic = CLAnimationManager::sharedAnimationManager()->spriteWithAnimationName(sFile.c_str());
    magic->setPosition(ccp(this->getContentSize().width*0.5, this->getContentSize().height*0.48));
    this->addChild(magic,2);
    
    CCActionInterval* m_pAction = (CCActionInterval*)CCSequence::create(
                                                                        CCAnimate::create(animation),
                                                                        NULL
                                                                        );
    
    magic->runAction(CCRepeatForever::create(m_pAction));
    
    //题卡效果
    CCAction*  m_pAction2 = CCSpawn::create(
                                        CCFadeTo::create(0.1, 255),
                                        CCScaleTo::create(0.1, 1.0,1.0),
                                        NULL);
    pcard->runAction(m_pAction2);
    
    //继续按钮
    this->scheduleOnce(schedule_selector(CompoundSucLayer::continueCallBack), 2.5);
    
    //音效
    CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_CompoundSuccess.wav");
    
    return true;
}

void CompoundSucLayer::continueCallBack(CCNode *node)
{
    this->removeFromParentAndCleanup(true);
    
}

void CompoundSucLayer::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10002, true);
}

void CompoundSucLayer::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}
bool CompoundSucLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true; //吞噬触摸
}
