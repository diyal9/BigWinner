//
//  PersonalInfoLayer.h
//  BigWinner
//
//  Created by LunaBox on 13-12-18.
//
//

#ifndef __BigWinner__PersonalInfoLayer__
#define __BigWinner__PersonalInfoLayer__

#include <iostream>
#include "cocos2d.h"

#include "cocos-ext.h"
#include "MLTextFieldTTFLayer.h"

USING_NS_CC;
using namespace cocos2d::extension;

/*
 游戏音量音效设置
 */
class PersonalInfoLayer:public CCLayerColor
{
    
public:
	PersonalInfoLayer(){
        m_spriteBg = NULL;
        m_pNameLabel = NULL;
        m_pPhoneNumLabel = NULL;
        m_pInputName =NULL;
        m_pInputPhoneNum = NULL;
        m_pPhoneNumberBG = NULL;
        m_pNameBG = NULL;
        
    };
	~PersonalInfoLayer(){};
    
    static PersonalInfoLayer* create();
	virtual bool init();
    
    //UI
    void doBaseUI();
    
    //取消
    void CanceBtn();
    
    //确定
    void AffirmBtn();
    
    //修改名称
    void alterName();
    
    //修改电话号码
    void alterPhoneNumber();
    
    //关闭按钮
    void menuCallBackButton();
    
    void NameManage(CCNode*, void* value); //电话号码输入框处理
    
    void PhoneNumManage(CCNode*, void* value); //名称输入框处理
    
    int isPhoneNum(std::string phone);//验证手机号码的正确性
    
    bool isMailAddress(std::string mailAddress);//验证邮箱地址是否正确
    
    std::string strSpace(std::string str);//去掉字符串前后空格
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    //背景
    
    CC_SYNTHESIZE(CCSprite* , m_spriteBg, spriteBg)
    
    CC_SYNTHESIZE(CCLabelTTF* , m_pNameLabel, pNameLabel); //test 用
    CC_SYNTHESIZE(CCLabelTTF* , m_pPhoneNumLabel, pPhoneNumLabel); //test 用
    
    CC_SYNTHESIZE(MLTextFieldTTFLayer *, m_pInputName, m_pInputName); //手机号码输入框（兑换无手机号码页面时输入框）
    CC_SYNTHESIZE(MLTextFieldTTFLayer *, m_pInputPhoneNum, pInputPhoneNum); //手机号码输入框（兑换无手机号码页面时输入框）
    
    CC_SYNTHESIZE(std::string, m_sNameStr, sNameStr);
    CC_SYNTHESIZE(std::string, m_sPhoneNumStr, sPhoneNumStr);
    CC_SYNTHESIZE(std::string, m_sMailStr, sMailStr);
    
    CC_SYNTHESIZE(std::string, m_sONeNameStr, sONeNameStr);
    CC_SYNTHESIZE(std::string, m_sONePhoneNumStr, sONePhoneNumStr);
    CC_SYNTHESIZE(std::string, m_sONeMailStr, sONeMailStr);
    
    //电话号码精灵
    CC_SYNTHESIZE(CCSprite* , m_pPhoneNumberBG, pPhoneNumberBG);
    //名称精灵
    CC_SYNTHESIZE(CCSprite* , m_pNameBG, pNameBG);

};


#endif /* defined(__BigWinner__PersonalInfoLayer__) */
