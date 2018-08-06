//
//  PersonalInfoLayer.cpp
//  BigWinner
//
//  Created by LunaBox on 13-12-18.
//
//

#include "PersonalInfoLayer.h"
#include "LGameCommonDefine.h"
#include "Button.h"
#include "CLChangeString.h"
#include "DlgTiShiLayer.h"
#include "CLPlayerManager.h"
#include "CLNetWorkManager.h"
#include "CLSceneManager.h"

PersonalInfoLayer* PersonalInfoLayer::create()
{
    PersonalInfoLayer *pRet = new PersonalInfoLayer();
	if (pRet && pRet->init())
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

bool PersonalInfoLayer::init()
{
    if(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0)))
    {
        return false;
    }
    
    //初始化 取得上次设定的名称和电话号码
    m_sONeNameStr = CLPlayerManager::shardPlayerManager()->getpUserInfo()->getsUsername();  //初始化名称
    m_sONePhoneNumStr = CLPlayerManager::shardPlayerManager()->getpUserInfo()->getsPhone();
    m_sONeNameStr = CLPlayerManager::shardPlayerManager()->getpUserInfo()->getsUsername();
    
    if(m_sONePhoneNumStr.compare("") == 0)
    {
        m_sONePhoneNumStr ="没有绑定电话号！";
    }
    
    //初始化UI
    doBaseUI();
    
    return true;
}

void PersonalInfoLayer::doBaseUI()
{
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    //背景
    m_spriteBg = CCSprite::create("Image/PublicUI/PersonalInfo_BG.png");
    m_spriteBg->setPosition(ccp(size.width*0.5, size.height*0.5));
    this->addChild(m_spriteBg);
    
    //页面名称背景
    CCSprite *pInfoNameBg = CCSprite::create("Image/PublicUI/TitleBackground.png");
    pInfoNameBg->setPosition(ccp(G_SF(80), m_spriteBg->getContentSize().height*0.965));
    m_spriteBg->addChild(pInfoNameBg);
    
    //页面名称文字
    CCSprite* BGNameStr = CCSprite::create("Image/PublicUI/PInfo_name.png");
    BGNameStr->setPosition(ccp(pInfoNameBg->getContentSize().width*0.5, pInfoNameBg->getContentSize().height*0.5));
    pInfoNameBg->addChild(BGNameStr);
    
    //关闭
    Button *shutdown = Button::createBtn("Image/PublicUI/Close.png");
    shutdown->setPosition(ccp(G_SF(545), m_spriteBg->getContentSize().height*0.965));
    shutdown->setOnClickCallbackNode(callfuncN_selector(PersonalInfoLayer::menuCallBackButton), this);
    shutdown->setHandlerPriority(this->getTouchPriority()-1);
    m_spriteBg->addChild(shutdown);

    //头像背景区
    CCSprite* pNamePanel = CCSprite::create("Image/PublicUI/RoleHeadBG.png");
    pNamePanel->setScale(2);
    pNamePanel->setPosition(ccp(G_SF(140), m_spriteBg->getContentSize().height *0.8));
    m_spriteBg->addChild(pNamePanel);
    
    //头像
    CCSprite* pRole = CCSprite::create("Image/PublicUI/RoleHeadIcon.png");
    pRole->setPosition(ccp(pNamePanel->getContentSize().width*0.5, pNamePanel->getContentSize().height*0.5));
    pNamePanel->addChild(pRole);
    
    //名称背景
    m_pNameBG = CCSprite::create("Image/PublicUI/Info_BG.png");
    m_pNameBG->setPosition(ccp(m_spriteBg->getContentSize().width*0.65, m_spriteBg->getContentSize().height *0.87));
    m_spriteBg->addChild(m_pNameBG);
    
    //名称 用
    m_pNameLabel = CCLabelTTF::create(m_sONeNameStr.c_str(), "", G_SF(18));
    m_pNameLabel->setPosition(ccp(G_SF(20)+m_pNameLabel->getContentSize().width*0.5, m_pNameBG->getContentSize().height *0.5));
    m_pNameLabel->setColor(ccc3(255, 204, 102));
    m_pNameBG->addChild(m_pNameLabel);
    
    //名称修改按钮
    Button* pNameBtn = Button::createBtn("Image/PublicUI/Modify_Btn.png");
    pNameBtn->setPosition(ccp(G_SF(220), m_pNameBG->getContentSize().height*0.5));
    pNameBtn->setOnClickCallback(callfuncO_selector(PersonalInfoLayer::alterName),this);
    m_pNameBG->addChild(pNameBtn);
    pNameBtn->setHandlerPriority(this->getTouchPriority()-1);
    
    //用户ID背景
    CCSprite *userIDBG = CCSprite::create("Image/PublicUI/Info_BG.png");
    userIDBG->setPosition(ccp(m_spriteBg->getContentSize().width*0.65, m_spriteBg->getContentSize().height *0.79));
    m_spriteBg->addChild(userIDBG);
    
    std::string sUserID = CLPlayerManager::shardPlayerManager()->getpUserInfo()->getsUserID();
    //用户ID名称
    CCLabelTTF* userID = CCLabelTTF::create(sUserID.c_str(), "Arial", G_SF(18));
    userID->setPosition(ccp(userIDBG->getContentSize().width*0.28, userIDBG->getContentSize().height*0.5));
    userID->setColor(ccc3(255, 204, 102));
    userIDBG->addChild(userID);
    
    //电话号码背景
    m_pPhoneNumberBG = CCSprite::create("Image/PublicUI/Info_BG.png");
    m_pPhoneNumberBG->setPosition(ccp(m_spriteBg->getContentSize().width*0.65, m_spriteBg->getContentSize().height *0.5));
    m_spriteBg->addChild(m_pPhoneNumberBG);
    
    //电话号码 用
    m_pPhoneNumLabel = CCLabelTTF::create(m_sONePhoneNumStr.c_str(), "Arial", G_SF(18));
    m_pPhoneNumLabel->setPosition(ccp(G_SF(20) + m_pPhoneNumLabel->getContentSize().width*0.5, m_pPhoneNumberBG->getContentSize().height *0.5));
    m_pPhoneNumLabel->setColor(ccc3(255, 204, 102));
    m_pPhoneNumberBG->addChild(m_pPhoneNumLabel);
    
    //电话号码修改按钮
    Button* pPhoneNBtn = Button::createBtn("Image/PublicUI/Modify_Btn.png");
    pPhoneNBtn->setPosition(ccp(G_SF(220), m_pPhoneNumberBG->getContentSize().height*0.5));
    pPhoneNBtn->setOnClickCallback(callfuncO_selector(PersonalInfoLayer::alterPhoneNumber),this);
    m_pPhoneNumberBG->addChild(pPhoneNBtn);
    pPhoneNBtn->setHandlerPriority(this->getTouchPriority()-1);
    
}

void PersonalInfoLayer::alterName()
{
    if(!m_pInputPhoneNum){
        //输入框
        m_pInputName = MLTextFieldTTFLayer::create("Image/PublicUI/Info_BG.png", m_pPhoneNumberBG->getContentSize().width, G_SF(46), kCCTextAlignmentLeft, "Arial", ccBLACK);
        m_pInputName->setPosition(ccp(m_spriteBg->getContentSize().width*0.65, m_spriteBg->getContentSize().height *0.87));
        m_spriteBg->addChild(m_pInputName);
        
        m_pInputName->getpTTFCursor()->setnLimitFontCount(11);
        m_pInputName->getpTTFCursor()->setpCallFuncND(callfuncND_selector(PersonalInfoLayer::NameManage));
        m_pInputName->getpTTFCursor()->setpCallObj(this);
        m_pInputName->getpTTFCursor()->setString(CLChangeString::sharedChangeString()->GetChangeString("InputName").c_str()); //设置 "请输入名字"
    }

}

void PersonalInfoLayer::PhoneNumManage(CCNode*, void* value)
{
    
    bool *b = (bool*)(value);
    
    if (*b)
    {
        m_pInputPhoneNum->getpTTFCursor()->setString("");
        
        
    }
    else
    {
        //输入的手机号正确
        if(isPhoneNum(m_pInputPhoneNum->getpTTFCursor()->getString()) == 0 ){
            
            m_sPhoneNumStr = m_pInputPhoneNum->getpTTFCursor()->getString();
            m_pPhoneNumLabel->setString(m_sPhoneNumStr.c_str());
            m_pPhoneNumLabel->setPosition(ccp(G_SF(20) + m_pPhoneNumLabel->getContentSize().width*0.5, m_pPhoneNumberBG->getContentSize().height *0.5));
            m_pInputPhoneNum->removeFromParentAndCleanup(true);
            m_pInputPhoneNum = NULL; //wangguolong
            
        }
        else
        {
            
            m_sPhoneNumStr = m_pInputPhoneNum->getpTTFCursor()->getString();
            //没有输入内容
            if(m_sPhoneNumStr.compare(CLChangeString::sharedChangeString()->GetChangeString("InputPhoneNum")) == 0)
            {
//                m_pInputPhoneNum->removeFromParentAndCleanup(true);
                return;
            }
            //没有输入电话号码
            if(isPhoneNum(m_pInputPhoneNum->getpTTFCursor()->getString()) == 1){
                
                TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("PleaseInputPhone").c_str());

            }
            //输入的电话号码格式不正确
            else
            {
                TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("PleaseInputRight").c_str());

            }
            
            
            m_sPhoneNumStr = m_pPhoneNumLabel->getString();
            m_pPhoneNumLabel->setString(m_sPhoneNumStr.c_str());
//            m_pInputPhoneNum->removeFromParentAndCleanup(true);
            
        }

    }
}


void PersonalInfoLayer::NameManage(CCNode*, void* value)
{
    
    bool *b = (bool*)(value);
    
    CCLog("输入内容：%s",m_pInputName->getpTTFCursor()->getString());
    if (*b)
    {
        m_pInputName->getpTTFCursor()->setString("");

    }
    else
    {

            m_sNameStr= m_pInputName->getpTTFCursor()->getString();
        
            m_sNameStr = strSpace(m_sNameStr);
        
            if(m_sNameStr.compare(CLChangeString::sharedChangeString()->GetChangeString("InputName")) == 0){
                return;
            }
        
            CCLog("m_sNameStr: %s",m_sNameStr.c_str());
        
            //没有输入内容或者输入的内容是空格
            if(m_sNameStr.compare("") == 0)
            {
                TiShiLayer::create(CLChangeString::sharedChangeString()->GetChangeString("InputName").c_str());
//                m_pInputName->removeFromParentAndCleanup(true);
                m_pInputName->getpTTFCursor()->setString(CLChangeString::sharedChangeString()->GetChangeString("InputName").c_str());
                m_pNameLabel->setString(m_sONeNameStr.c_str());
                return;
            }
        
            m_pNameLabel->setString(m_sNameStr.c_str());
            m_pNameLabel->setPosition(ccp(G_SF(20)+m_pNameLabel->getContentSize().width*0.5, m_pNameBG->getContentSize().height *0.5));
            m_pInputName->removeFromParentAndCleanup(true);
            m_pInputName = NULL; //wangguolong
        
    }
}



void PersonalInfoLayer::alterPhoneNumber()
{
    if(!m_pInputName){
        //输入框
        m_pInputPhoneNum = MLTextFieldTTFLayer::create("Image/PublicUI/Info_BG.png", m_pPhoneNumberBG->getContentSize().width, G_SF(46), kCCTextAlignmentLeft, "Arial", ccBLACK);
        m_pInputPhoneNum->setPosition(ccp(m_spriteBg->getContentSize().width*0.65, m_spriteBg->getContentSize().height *0.5));
        m_spriteBg->addChild(m_pInputPhoneNum);
        
        m_pInputPhoneNum->getpTTFCursor()->setnLimitFontCount(11);
        m_pInputPhoneNum->getpTTFCursor()->setpCallFuncND(callfuncND_selector(PersonalInfoLayer::PhoneNumManage));
        m_pInputPhoneNum->getpTTFCursor()->setpCallObj(this);
        m_pInputPhoneNum->getpTTFCursor()->setString(CLChangeString::sharedChangeString()->GetChangeString("InputPhoneNum").c_str()); //设置显示 "请输入电话号码"
    }

    
}

int PersonalInfoLayer::isPhoneNum(std::string phone)
{
    if (phone == "") {
        //添加dlg
        return 1;
    }
    
    if (phone.length() != 11)
    {
        return 2;
    }
    else
    {
        
        for (int i = 0; phone[i]!='\0'; i++)
        {
            if (!(phone[i] >= '0' && phone[i] <= '9'))
            {
                return 2;
            }
        }
        
        
    }
    return 0;
    
}

std::string PersonalInfoLayer::strSpace(std::string str)
{

    for(int i = 0;i<str.size();i++)
    {
        if(' ' != str[i])
        {
            break;
        }
        else if(' ' == str[i])
        {
            
            str.erase(i,1);
        }
    }
    
    while (' ' == str[m_sNameStr.size()-1]) {
        str.erase(str.size()-1,1);
    }
    
    return str;
}


bool PersonalInfoLayer::isMailAddress(std::string mailAddress)
{
    mailAddress = strSpace(mailAddress);
    if(mailAddress.compare("") == 0){
        //没有输入
        return false;
    }
    
    char *myMailAddress = new char[255];
    int i;
    for(i=0;i <=mailAddress.length();i++)
        myMailAddress[i]=mailAddress[i];
    myMailAddress[i] = '\0';
    
    
    int len,atindex,dotindex;
    int atflat=0,dotflag=0;
    len = strlen(mailAddress.c_str());
    
        for(int i=0;i<len; i++)
        {
            if(myMailAddress[i]==' ')
            {
                return false;
            }
            if(myMailAddress[i]=='@')
            {
                if(!atflat)
                {
                    if(i<1||(i+4)==len)
                    {
//                        cout<< "error1"<<endl;
                        return false;
                    }
                    atindex=i;
                    atflat=1;
                }
                else
                {
//                    cout<<"only one @!"<<endl;
                    return false;
                }

            }
            else if(myMailAddress[i]=='.')
            {
                if((i-dotindex)==1)
                {
//                    cout<<"0 space!"<<endl;
                    return false;
                }
                dotindex=i;
                int a =dotindex+2;
                if(a>=len)
                {
//                    cout<<".后面不正确"<<endl;
                    return false;
                }
                dotflag++;
                if(dotflag>3)
                {
//                    cout<<"can not exceed 3!"<<endl;
                    return false;
                }
            }
        }
    
    delete myMailAddress;
    
    if(dotflag && atflat){
        return true;
    }else{
//        CCLog("输入的邮件地址不正确！");
        TiShiLayer::create("输入的邮件地址不正确！");
        return false;
    }
    
}

void PersonalInfoLayer::menuCallBackButton()
{
    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);
}


void PersonalInfoLayer::onEnter()
{
    CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    pMainLayer->getTouchPriority();
    
    CCLog("触摸优先级: %i" ,pMainLayer->getTouchPriority());
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1000000778777, true);
}

void PersonalInfoLayer::onExit()
{

        //发送个人信息
        Json::Value value;
        value["msgid"] = "57";
        if(m_sNameStr.compare("") == 0)
        {
            value["username"] = m_sONeNameStr;
        }
        else
        {
            value["username"] = m_sNameStr;
        }
        
        if(m_sPhoneNumStr.compare("") == 0)
        {
            value["mphone"] =  m_sONePhoneNumStr;
        }
        else
        {
            value["mphone"] =  m_sPhoneNumStr;
        }
        
        CLNetWorkManager::shardNetWorkManager()->SendNetMessage(57, value);
    
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool PersonalInfoLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    return true;
}


void PersonalInfoLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void PersonalInfoLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
