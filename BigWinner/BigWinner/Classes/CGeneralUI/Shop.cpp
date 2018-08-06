//
//  Shop.cpp
//  BigWinner
//
//  Created by LunaBox03 on 13-11-01.
//
//

#include "Shop.h"

#include "LGameCommonDefine.h"
#include "Button.h"
#include "SpRoleUI.h"
#include "SpQuestionCardUI.h"
#include "SpPropsUI.h"
#include "SpCurrency.h"
#include "CLNetWorkManager.h"
#include "CLSceneManager.h"
#include "CLMainSceneLayer.h"
#include "JsonDataManager.h"
#include "CLPlayerManager.h"
#include "DlgTiShiLayer.h"
#include "CLChangeString.h"
#include "CardSprite.h"
#include "CardSprite.h"
#include "CLSoundManager.h"

using namespace Json;

ShopLayer* ShopLayer::create(int index, Json::Value &data)
{
    ShopLayer *pRet = new ShopLayer();
	if (pRet && pRet->init(index, data))
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

bool ShopLayer::init(int index, Json::Value &data)
{
    if(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0)))
    {
        return false;
    }
    
    //数据解析
    m_nlayerTpye = index;
    m_bkData = data;
    
    CCLOG("data:%d", m_bkData["playerrole"].size());
    
    //初始化UI
    doBaseUI();
    
    return true;
}

void ShopLayer::doBaseUI()
{
    
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();

    //sp
    CCSprite* sp = CCSprite::create("Image/BackPack/BKShop2.png");
    this->addChild(sp);
    sp->setPosition(ccp(size.width*0.5, size.height*0.43));
    
    //关闭
    Button *shutdown = Button::createBtn("Image/PublicUI/Close.png");
    shutdown->setPosition(ccp(G_SF(550), G_SF(765)));
    shutdown->setOnClickCallbackNode(callfuncN_selector(ShopLayer::menuCallBackButton), this);
    shutdown->setHandlerPriority(-10001);
    sp->addChild(shutdown,0,SP_GUANBI_TAG);
    
    //商店
    CCSprite *beibao = CCSprite::create("Image/PublicUI/TitleBackground.png");
    beibao->setPosition(ccp(G_SF(75), G_SF(765)));
    sp->addChild(beibao,0);
    
    //商店文字
    CCSprite *bbwenzi = CCSprite::create("Image/BackPack/shop.png");
    bbwenzi->setPosition(ccp(G_SF(80), G_SF(25)));
    beibao->addChild(bbwenzi,100);
    
    //题卡
    Button *tika = Button::createBtn("Image/BackPack/Titlebtn.png");
    tika->setPosition( ccp(G_SF(80), G_SF(640)) );
    tika->setOnClickCallbackNode(callfuncN_selector(ShopLayer::menuCallBackButton), this);
    tika->setHandlerPriority(-10001);
    sp->addChild(tika,0,SP_TIKA_TAG);
    
    //题卡文字
    CCSprite *tkwenzi = CCSprite::create("Image/BackPack/card.png");
    tkwenzi->setPosition(ccp(G_SF(0), G_SF(0)));
    tika->addChild(tkwenzi,1);
    
    //货币
    Button *huobi = Button::createBtn("Image/BackPack/Titlebtn.png");
    huobi->setPosition( ccp(G_SF(80), G_SF(470)) );
    huobi->setOnClickCallbackNode(callfuncN_selector(ShopLayer::menuCallBackButton), this);
    huobi->setHandlerPriority(-10001);
    sp->addChild(huobi,0,SP_HUOBI_TAG);
    
    //货币文字
    CCSprite *hbwenzi = CCSprite::create("Image/BackPack/Currency.png");
    hbwenzi->setPosition(ccp(G_SF(0), G_SF(0)));
    huobi->addChild(hbwenzi,1);
    
    //角色
    Button *juese = Button::createBtn("Image/BackPack/Titlebtn.png");
    juese->setPosition( ccp(G_SF(80), G_SF(300)) );
    juese->setOnClickCallbackNode(callfuncN_selector(ShopLayer::menuCallBackButton), this);
    juese->setHandlerPriority(-10001);
    sp->addChild(juese,0,SP_JUESE_TAG);
    
    //角色文字
    CCSprite *jswenzi = CCSprite::create("Image/BackPack/Role.png");
    jswenzi->setPosition(ccp(G_SF(0), G_SF(0)));
    juese->addChild(jswenzi,1);
    
    //道具
    Button *daoju = Button::createBtn("Image/BackPack/Titlebtn.png");
    daoju->setPosition( ccp(G_SF(80), G_SF(130)) );
    daoju->setOnClickCallbackNode(callfuncN_selector(ShopLayer::menuCallBackButton), this);
    daoju->setHandlerPriority(-10001);
    sp->addChild(daoju,0,SP_DAOJU_TAG);
    
    //道具文字
    CCSprite *djwenzi = CCSprite::create("Image/BackPack/Props.png");
    djwenzi->setPosition(ccp(G_SF(0), G_SF(0)));
    daoju->addChild(djwenzi,1);
    
    //第一次默认加载ui
    this->scheduleOnce(schedule_selector(ShopLayer::addBaseUI), 0.2);
    
}

void ShopLayer::addBaseUI()
{
    //加载默认UI 题卡
    SpQuestionCardUI* hechengLay = SpQuestionCardUI::create();
    this->addChild(hechengLay,10, SP_GENERAL_TAG);
    hechengLay->setPosition(ccp(G_SF(10), G_SF(10)));
    m_nlayerTpye = SP_TIKA;
    
    if(this->getntag()!= 0)
    {
        this->getChildByTag(this->getntag())->removeFromParentAndCleanup(true);
    }
    this->setntag(SP_TIKA_TAG2);
    //提卡
    Button *tika = Button::createBtn("Image/BackPack/Titlebtn2.png");
    tika->setPosition( ccp(G_SF(90), G_SF(650)) );
    tika->setOnClickCallbackNode(callfuncN_selector(ShopLayer::menuCallBackButton), this);
    tika->setHandlerPriority(-10001);
    this->addChild(tika,0,SP_TIKA_TAG2);
    
    //题卡文字
    CCSprite *tkwenzi = CCSprite::create("Image/BackPack/card.png");
    tkwenzi->setScaleX(1.1);
    tkwenzi->setPosition(ccp(G_SF(0), G_SF(0)));
    tika->addChild(tkwenzi,1);
    
    //刷新位置
    setIndexPosition();
    
    doAction(this, true);
}

//动作执行
void ShopLayer::doAction(CCNode* r, bool moveIn)
{
    
    if (moveIn) //进场动作
    {
        CCLayerColor *fadeLayer = CCLayerColor::create();
        this->getParent()->addChild(fadeLayer,9,500);
        
        CCFiniteTimeAction *actionOut = CCSequence::create(CCFadeTo::create(0.4f, 255*0.5), NULL); //透明度动画
        fadeLayer->runAction(actionOut);
        
        CCAction *actionMoveOut = CCEaseExponentialOut::create(CCMoveTo::create(0.4f, ccp(0, 0))); //快到慢
        this->runAction(actionMoveOut);
        
        //窗体开闭音乐
        CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_WInClose.wav");
    }
    else //出场动作
    {
        //窗体开闭音乐
        CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_WInClose.wav");
        
        CCFiniteTimeAction *actionIn = CCSequence::create(CCFadeTo::create(0.4f, 0), NULL); //透明度动画
        this->getParent()->getChildByTag(500)->runAction(actionIn);
        
        CCAction *actionMoveIn = CCEaseExponentialIn::create(CCMoveTo::create(0.4f, ccp(r->getPositionX(), r->getPositionY() * (-1)))); //慢到快
        r->runAction(actionMoveIn);
        
        this->getParent()->getChildByTag(500)->removeFromParentAndCleanup(true);
    }
    
}

void ShopLayer::menuCallBackButton(CCNode* r)
{
    
    CCNode* node = this->getChildByTag(SP_GENERAL_TAG);
    
    if (node)
    {
        node->removeAllChildrenWithCleanup(true);
        node->removeFromParentAndCleanup(true);
        m_nlayerTpye = 0;
        
    }
    
    switch (r->getTag())
    {
            
            
        case SP_TIKA_TAG:
        case SP_TIKA_TAG2:
        {
            if(this->getntag()!= 0)
            {
                this->getChildByTag(this->getntag())->removeFromParentAndCleanup(true);
            }
            
            this->setntag(SP_TIKA_TAG2);
            //题卡选中
            Button *tika = Button::createBtn("Image/BackPack/Titlebtn2.png");
            tika->setPosition( ccp(G_SF(90), G_SF(650)) );
            tika->setOnClickCallbackNode(callfuncN_selector(ShopLayer::menuCallBackButton), this);
            tika->setHandlerPriority(-10001);
            this->addChild(tika,0,SP_TIKA_TAG2);
            
            //题卡文字选中
            CCSprite *tkwenzi = CCSprite::create("Image/BackPack/card.png");
            tkwenzi->setScaleX(1.1);
            tkwenzi->setPosition(ccp(G_SF(0), G_SF(0)));
            tika->addChild(tkwenzi,1);
            
            CCLOG("提卡");
            if (m_nlayerTpye == SP_TIKA)
                return;
            
            SpQuestionCardUI* hechengLay = SpQuestionCardUI::create();
            this->addChild(hechengLay,10, SP_GENERAL_TAG);
            hechengLay->setPosition(ccp(G_SF(10), G_SF(10)));
            
            m_nlayerTpye = SP_TIKA;
        }
            break;
            
        case SP_HUOBI_TAG:
        case SP_HUOBI_TAG2:
        {
            if(this->getntag()!= 0)
            {
                this->getChildByTag(this->getntag())->removeFromParentAndCleanup(true);
            }
            
            this->setntag(SP_HUOBI_TAG2);
            //货币选中
            Button *huobi = Button::createBtn("Image/BackPack/Titlebtn2.png");
            huobi->setPosition( ccp(G_SF(90), G_SF(480)) );
            huobi->setOnClickCallbackNode(callfuncN_selector(ShopLayer::menuCallBackButton), this);
            huobi->setHandlerPriority(-10001);
            this->addChild(huobi,0,SP_HUOBI_TAG2);
            
            //货币文字选中
            CCSprite *hbwenzi = CCSprite::create("Image/BackPack/Currency.png");
            hbwenzi->setScaleX(1.1);
            hbwenzi->setPosition(ccp(G_SF(0), G_SF(0)));
            huobi->addChild(hbwenzi,1);
            
            CCLOG("货币");
            if (m_nlayerTpye == SP_HUOBI)
                return;
            
            SpCurrency* hechengLay = SpCurrency::create();
            this->addChild(hechengLay,10, SP_GENERAL_TAG);
            hechengLay->setPosition(ccp(G_SF(10), G_SF(10)));
            
            m_nlayerTpye = SP_HUOBI;
            
        }
            break;
            
        case SP_JUESE_TAG:
        case SP_JUESE_TAG2:
        {
            if(this->getntag()!= 0)
            {
                this->getChildByTag(this->getntag())->removeFromParentAndCleanup(true);
            }
            this->setntag(SP_JUESE_TAG2);
            //角色选中
            Button *juese = Button::createBtn("Image/BackPack/Titlebtn2.png");
            juese->setPosition( ccp(G_SF(90), G_SF(310)) );
            juese->setOnClickCallbackNode(callfuncN_selector(ShopLayer::menuCallBackButton), this);
            juese->setHandlerPriority(-10001);
            this->addChild(juese,0,SP_JUESE_TAG2);
            
            //角色文字选中
            CCSprite *jswenzi = CCSprite::create("Image/BackPack/Role.png");
            jswenzi->setScaleX(1.1);
            jswenzi->setPosition(ccp(G_SF(0), G_SF(0)));
            juese->addChild(jswenzi,1);
            
            
            CCLOG("角色");
            if (m_nlayerTpye == SP_JUESE)
                return;
            
            SpRoleUI* hechengLay = SpRoleUI::create();
            this->addChild(hechengLay,10, SP_GENERAL_TAG);
            hechengLay->setPosition(ccp(G_SF(10), G_SF(10)));
            
            m_nlayerTpye = SP_JUESE;
        }
            break;
            
        case SP_DAOJU_TAG:
        case SP_DAOJU_TAG2:
        {
            if(this->getntag()!= 0)
            {
                this->getChildByTag(this->getntag())->removeFromParentAndCleanup(true);
            }
            
            this->setntag(SP_DAOJU_TAG2);
            //道具选中
            Button *daoju = Button::createBtn("Image/BackPack/Titlebtn2.png");
            daoju->setPosition( ccp(G_SF(90), G_SF(140)) );
            daoju->setOnClickCallbackNode(callfuncN_selector(ShopLayer::menuCallBackButton), this);
            daoju->setHandlerPriority(-10001);
            this->addChild(daoju,0,SP_DAOJU_TAG2);
            
            //道具文字选中
            CCSprite *djwenzi = CCSprite::create("Image/BackPack/Props.png");
            djwenzi->setScaleX(1.1);
            djwenzi->setPosition(ccp(G_SF(0), G_SF(0)));
            daoju->addChild(djwenzi,1);
            
            CCLOG("道具");
            if (m_nlayerTpye == SP_DAOJU)
                return;
            
            SpPropsUI* hechengLay = SpPropsUI::create();
            this->addChild(hechengLay,10, SP_GENERAL_TAG);
            hechengLay->setPosition(ccp(G_SF(10), G_SF(10)));
            
            m_nlayerTpye = SP_DAOJU;
        }
            break;
            
        case SP_GUANBI_TAG:
        {
            doAction(this, false);
            
            this->removeAllChildrenWithCleanup(true);
            this->removeFromParentAndCleanup(true);
            return;
        }
        default:
            break;
    }
    
    setIndexPosition();
    
    
}

void ShopLayer::setIndexPosition()
{
    
    //    switch (m_nlayerTpye)
    //    {
    //
    //            //题卡
    //        case SP_TIKA:
    //        {
    //            m_indesSprite->setPosition(ccp(m_indesSprite->getParent()->getContentSize().width*0.5, G_SF(250)));
    //        }
    //            break;
    //            //角色
    //        case SP_JUESE:
    //        {
    //            m_indesSprite->setPosition(ccp(m_indesSprite->getParent()->getContentSize().width*0.5, G_SF(370)));
    //        }
    //            break;
    //
    //            //道具
    //        case SP_DAOJU:
    //        {
    //            m_indesSprite->setPosition(ccp(m_indesSprite->getParent()->getContentSize().width*0.5, G_SF(120)));
    //        }
    //            break;
    //            //货币
    //        case SP_HUOBI:
    //        {
    //            m_indesSprite->setPosition(ccp(m_indesSprite->getParent()->getContentSize().width*0.5, G_SF(500)));
    //        }
    //            break;
    //
    //        default:
    //            break;
    //    }
    
    
    //    m_indesSprite->setVisible(true);
}


void ShopLayer::onEnter()
{
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);
}

void ShopLayer::onExit()
{
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}



bool ShopLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(this->isVisible())
        return true;
    else
        return false;
}

void ShopLayer::_netCallBack()
{
    CCLog("购买成功后，回调函数");
    
//    CLMainSceneLayer* pMainLayer = (CLMainSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    if(pMainLayer->getnCTIndex() != CLScene_MainScene && pMainLayer->getnCTIndex() != CLScene_ShelfLayer)
    {
        CCLOG("Error: The cur scene is not the main scene!!!");
        return;
    }
    ShopLayer* bl = (ShopLayer*)pMainLayer->getChildByTag(SHOPTAG);
    
    if (dynamic_cast<SpQuestionCardUI*>(bl->getChildByTag(SP_GENERAL_TAG))) {
//        SpQuestionCardUI *question = dynamic_cast<SpQuestionCardUI*>(bl->getChildByTag(SP_GENERAL_TAG));
//        question->_ShopBuyCallBack(true);
    }
    
    if (dynamic_cast<SpRoleUI*>(getChildByTag(SP_GENERAL_TAG)))
    {
        //角色购买成功回调
        SpRoleUI *role = dynamic_cast<SpRoleUI*>(getChildByTag(SP_GENERAL_TAG));
        role->_ShopBuyCallBack(true);
    }
    
    if (dynamic_cast<SpCurrency*>(bl->getChildByTag(SP_GENERAL_TAG))) {
        //        SpCurrency *currency = dynamic_cast<SpCurrency*>(bl->getChildByTag(SP_GENERAL_TAG));
        //        currency->_ShopBuyCallBack(true);
    }
    
}






/*********************************************************************************************
 * 购买layer
 *********************************************************************************************/
ShopBuyLayer* ShopBuyLayer::createLayer(int index,
                                        int costType, //购买类型
                                        int unitPrice, //单价
                                        bool canChangeNum //是否可以选择购买个数
                                        ,int buyMaxNum //购买最大数量
                                        ,std::string shop
)
{
    
    ShopBuyLayer *pRet = new ShopBuyLayer();
	if (pRet && pRet->init(index,costType,unitPrice,canChangeNum,buyMaxNum,shop))
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

bool ShopBuyLayer::init(int cardId,
                        int costType, //购买类型
                        int unitPrice, //单价
                        bool canChangeNum //是否可以选择购买个数
                        ,int buyMaxNum //购买最大数量
                        ,std::string shop
)
{
    
    if ( !CCLayerColor::initWithColor(ccc4(0,0,0,191.25)) )
    {
        return false;
    }
    
    
    int ArtIconId = 0;  //题卡IconId
    ArtIconId = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[cardId]->m_artIconId;
    
    pCardId = cardId; //接收值

    pCostType = costType;
    pUnitPrice = unitPrice;
    pCanChangeNum = canChangeNum;
    
    //用户当前钱币数量
    userCurrentMoney = 0;
    
    switch (pCostType) {
        case 1:
            //钞票
            userCurrentMoney = CLPlayerManager::shardPlayerManager()->getpUserInfo()->getMoneys();
            pBuyTypePath = "Image/LoginLayer/ML_MoneyIcon.png";
            break;
            
        case 2:
            //购物卡
            userCurrentMoney = CLPlayerManager::shardPlayerManager()->getpUserInfo()->getJetton();
            pBuyTypePath = "Image/LoginLayer/ML_CardIcon.png";
            break;
            //        case 3:
            //            //人民币
            //            userCurrentMoney = CLPlayerManager::shardPlayerManager()->getpUserInfo()->;
            //            break;
        case 4:
            //其它
            break;
        default:
            break;
    }
    
    if (unitPrice != 0 ) {
        //可购买数，调用购买层的类应该判断至少大与一张卡
        scollNumMax = buyMaxNum; //最大可购买数参数传过来，购买页面现在不需要判断
        if ( scollNumMax > 99 ) {
            scollNumMax = 99; //最大购买数为99
        }
    }
    
    
    buyCardNum = 1; //初始化的时候，变量设置为1
    
    //    CCLog("tmpInfo->卡号%d,商品路径%s,星级%d,购买类型路径%s,购买类型%d",pCardId,pCardPath.c_str(),pStarLevel,pBuyTypePath.c_str(),pCostType);
    
    createInitUI(); //初始化UI
    
    doBuyAction(this, true);
    
	return true;
}

void ShopBuyLayer::doBuyAction(CCNode* r, bool moveIn)
{

    CCNode *bgSpr = this->getChildByTag(10086);
    if (moveIn) //进场动作
    {
        bgSpr->setPositionY(bgSpr->getPositionY() * (-1)); //反向设置坐标
        
        CCFiniteTimeAction *actionOut = CCSequence::create(CCFadeTo::create(0.4f, 255*0.5), NULL); //透明度动画
        this->runAction(actionOut);
        
        CCAction *actionMoveOut = CCEaseExponentialOut::create(CCMoveTo::create(0.4f, ccp(bgSpr->getPositionX(), bgSpr->getPositionY() * (-1)))); //快到慢
        bgSpr->runAction(actionMoveOut);
        
        //窗体开闭音乐
        CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_WInClose.wav");
    }
    else //出场动作
    {
        CCFiniteTimeAction *actionIn = CCSequence::create(CCFadeTo::create(0.4f, 0), NULL); //透明度动画
        this->runAction(actionIn);
        
        CCAction *actionMoveIn = CCEaseExponentialIn::create(CCMoveTo::create(0.4f, ccp(r->getPositionX(), r->getPositionY() * (-1)))); //慢到快
        bgSpr->runAction(actionMoveIn);
        
        bgSpr->removeFromParentAndCleanup(true);
    }
}

void ShopBuyLayer::createInitUI()
{
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    //背景图添加
    CCSprite *mainBg = CCSprite::create("Image/BackPack/B_BuyBg.png");
    mainBg->setPosition( ccp(size.width * 0.5, size.height * 0.5));
    this->addChild(mainBg, 1, 10086);
     
    //title
    CCSprite *title = CCSprite::create("Image/BackPack/B_BuyBtn.png");
    title->setPosition( ccp(G_SF(125), mainBg->getContentSize().height - title->getContentSize().height * 0.5));
    mainBg->addChild(title);
    
    CCSprite *titleFont = CCSprite::create("Image/BackPack/B_BuyTitle.png");
    titleFont->setPosition( ccp(title->getContentSize().width * 0.5, title->getContentSize().height * 0.5));
    title->addChild(titleFont, 10);
    
    //商品图标
    //wangguolong
    CardSprite *sGoodCard = CardSprite::create(pCardId);
    if (!pCanChangeNum) {
        sGoodCard->setPosition(ccp(mainBg->getContentSize().width * 0.5, G_SF(450)));
    } else {
        sGoodCard->setPosition(ccp(G_SF(170), G_SF(450)));
    }
    mainBg->addChild(sGoodCard,2);
    
    if (pCanChangeNum)
    {
        //数字可选择画面
        CCSprite *sXBg = CCSprite::create("Image/BackPack/B_XOfbg.png");
        sXBg->setPosition(ccp(G_SF(310), sGoodCard->getPositionY()));
        mainBg->addChild(sXBg);
        
        CCSprite *selectBg = CCSprite::create("Image/BackPack/B_SelectBg.png");
        selectBg->setPosition(ccp(G_SF(440), sGoodCard->getPositionY()));
        mainBg->addChild(selectBg);
        
        //数字控件中间TTF
        m_pScollNumSprc = CCLabelTTF::create("1", "Arial", G_SF(60));
        m_pScollNumSprc->setPosition(ccp(selectBg->getContentSize().width * 0.5, selectBg->getContentSize().height * 0.5));
        selectBg->addChild(m_pScollNumSprc,10,SCROLLITEMTAG);
        
         Button *plusBtn = Button::createBtn("Image/BackPack/A_PlusBtn.png");
        plusBtn->setPosition(ccp(selectBg->getContentSize().width * 0.5, G_SF(193)));
        plusBtn->setOnClickCallbackNode(callfuncN_selector(ShopBuyLayer::plusBtnCall), this);
        plusBtn->setHandlerPriority(BASICTOUCHPRO-2);
        selectBg->addChild(plusBtn);
        
        Button *subBtn = Button::createBtn("Image/BackPack/A_SubtrBtn.png");
        subBtn->setPosition(ccp(selectBg->getContentSize().width * 0.5, G_SF(47)));
        subBtn->setOnClickCallbackNode(callfuncN_selector(ShopBuyLayer::subBtnCall), this);
        subBtn->setHandlerPriority(BASICTOUCHPRO-2);
        selectBg->addChild(subBtn);
    }
    
    
    //购买类型
    CCSprite *sBuyType = CCSprite::create(pBuyTypePath.c_str());
    sBuyType->setPosition(ccp(G_SF(180), G_SF(223)));
    mainBg->addChild(sBuyType);
    
    //花费总额
    char strSumAmount[20] = {0};
    sprintf(strSumAmount, "%u", pUnitPrice); //初始的时候，花费直接用单价
    
    m_pSumCostLabel= CCLabelTTF::create(strSumAmount, "Arial", G_SF(50), ccp(G_SF(200), G_SF(60)),kCCTextAlignmentLeft);
	m_pSumCostLabel->setPosition(ccp(sBuyType->getPositionX() + G_SF(150), sBuyType->getPositionY()));
	mainBg->addChild(m_pSumCostLabel);
    
    
    //确定
    Button *pConfirm= Button::createBtn("Image/BackPack/B_SureBtn.png");
    pConfirm->setPosition(ccp(mainBg->getContentSize().width * 0.72, G_SF(76)));
    pConfirm->setOnClickCallbackNode(callfuncN_selector(ShopBuyLayer::confirmBtnCall), this);
    pConfirm->setHandlerPriority(BASICTOUCHPRO-1);
    mainBg->addChild(pConfirm);
    
    Button *pCance= Button::createBtn("Image/BackPack/B_CanceBtn.png");
    pCance->setPosition(ccp(mainBg->getContentSize().width * 0.28, G_SF(76)));
    pCance->setOnClickCallbackNode(callfuncN_selector(ShopBuyLayer::canceBtnCall), this);
    pCance->setHandlerPriority(BASICTOUCHPRO-1);
    mainBg->addChild(pCance);
    
}

//加回调
void ShopBuyLayer::plusBtnCall()
{
    if (buyCardNum == scollNumMax) { //加到最大可购买数
        TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("BuyMax"));
        return;
    }
    
    buyCardNum++;
    
    //中间数字更改
    char str[20] = {0};
    sprintf(str, "%u", buyCardNum);
    m_pScollNumSprc->setString(str);
    
    //花费
    char str3[20] = {0};
    sprintf(str3, "%u", buyCardNum * pUnitPrice);
    m_pSumCostLabel->setString(str3);
}

//减回调
void ShopBuyLayer::subBtnCall()
{
    if (buyCardNum == 1) { //减到1
        TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("BuyMin"));
        return;
    }
    
    buyCardNum--;
    
    //中间数字更改
    char str[20] = {0};
    sprintf(str, "%u", buyCardNum);
    m_pScollNumSprc->setString(str);
    
    //花费
    char str3[20] = {0};
    sprintf(str3, "%u", buyCardNum * pUnitPrice);
    m_pSumCostLabel->setString(str3);
}


//取消回调
void ShopBuyLayer::canceBtnCall()
{
    //窗体开闭音乐
    CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_WInClose.wav");
    this->removeFromParentAndCleanup(true);
}

//确认回调
void ShopBuyLayer::confirmBtnCall()
{
    this->removeFromParentAndCleanup(true);
    
    //发送内容
    Value data;
    data["msgid"].append("107"); //指令码
    data["No"] = pCardId;  //购买的卡牌No
    data["Num"] = buyCardNum;
    
    CCLog("sendLog->%d,%d", pCardId,buyCardNum);
    
    CLNetWorkManager::shardNetWorkManager()->SendNetMessage(CMD_SHOPBUY, data);
    CLSceneManager::sharedSceneManager()->SetWaitNetLayer(true);
    
}

bool ShopBuyLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    //弹出层上的继续按钮吞噬所有的触摸
    return true;
}

void ShopBuyLayer::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, BASICTOUCHPRO, true);
}

void ShopBuyLayer::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}


BuySucLayer* BuySucLayer::create(int succCardNo, int succStarLevel)
{
    BuySucLayer* pCell = new BuySucLayer();
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

bool BuySucLayer::init(int succCardNo, int succStarLevel)
{
    if ( !CCLayerColor::initWithColor(ccc4(0,0,0,191.5)) )
    {
        return false;
    }
    
    
   	CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    CCSprite *sSuccCard = CCSprite::create("Image/BackPack/CardBackground.png");
    sSuccCard->setPosition( ccp(size.width * 0.5,size.height * 0.5));
    
    for (int i = 1; i <= succStarLevel; i++) {  //题卡星处理
        CCSprite *star2 = CCSprite::create("Image/BackPack/CardBackground.png");
        star2->setPosition(  ccp(G_SF(5) + star2->getContentSize().width * 0.5 * ((i-1)*2 + 1), sSuccCard->getContentSize().height - G_SF(33)) );
        sSuccCard->addChild(star2);
    }
    
    CCSprite *sTitleLab = CCSprite::create("Image/BackPack/A_CompandSucc.png");  //title
    sTitleLab->setPosition(ccp(size.width * 0.5, size.height * 0.75));
    this->addChild(sTitleLab);
    
    CCSprite *sSpr_Star = CCSprite::create("Image/BackPack/card_compound_pft.png");  //背景星
    sSpr_Star->setPosition(ccp(sSuccCard->getContentSize().width * 0.5, sSuccCard->getContentSize().height * 0.5));
    sSuccCard->addChild(sSpr_Star);
    this->addChild(sSuccCard,0);
    
    //继续按钮
    Button *continueBtn = Button::createBtn("Image/BackPack/C_composite.png");
    continueBtn->setPosition(ccp(size.width * 0.5, G_SF(140)));
    continueBtn->setOnClickCallback(menu_selector(BuySucLayer::continueCallBack),this);
    continueBtn->setHandlerPriority(-10003);
    this->addChild(continueBtn,1,3);
    
    CCLabelTTF* lBtnConTTF = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("ContinuePk").c_str(), "Arial", G_SF(30), CCSizeMake(G_SF(200), G_SF(30)), kCCTextAlignmentCenter);
    continueBtn->addChild(lBtnConTTF, 10);
    
    return true;
}

void BuySucLayer::continueCallBack(CCNode *node)
{
    this->removeFromParentAndCleanup(true);
    
}

void BuySucLayer::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10002, true);
}

void BuySucLayer::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}
bool BuySucLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true; //吞噬触摸
}


/*
 购买的商品展示
 */
ShowGoodsLayer* ShowGoodsLayer::create(int No)
{
    ShowGoodsLayer* pCell = new ShowGoodsLayer();
    if(pCell && pCell->init(No))
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

bool ShowGoodsLayer::init(int No)
{
    if ( !CCLayerColor::initWithColor(ccc4(0,0,0,225*0.5)) )
    {
        return false;
    }
    
    //购买文字
    CCSprite *BuyName = CCSprite::create("Image/BackPack/BuyName.png");
    BuyName->setPosition(ccp(this->getContentSize().width*0.5,this->getContentSize().height*0.65));
    this->addChild(BuyName);
    
    CardSprite *sprite = CardSprite::create(No);
    sprite->setPosition(ccp(this->getContentSize().width*0.5, this->getContentSize().height*0.4));
    sprite->setScale(1.3);
    this->addChild(sprite);
    
    //题卡购买动作
    CCAction*  action = CCSpawn::create(
                                         CCFadeTo::create(0.1, 255),
                                         CCScaleTo::create(0.1, 1.0,1.0),
                                         NULL);
    
    sprite->runAction(action);
    
    this->scheduleOnce(schedule_selector(ShowGoodsLayer::CallBack), 2.0);

    return true;
}

void ShowGoodsLayer::CallBack()
{
    this->removeFromParentAndCleanup(true);
}

void ShowGoodsLayer::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10002, true);
}

void ShowGoodsLayer::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}
bool ShowGoodsLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}









