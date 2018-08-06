#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "cocos2d.h"


USING_NS_CC;

// typedef  void (*OnClickFunc)();
// void onClick();
/******************************
  Button使用实例
  1、使用单个图片资源创建Button
    Button *btn1 = Button::createBtn("Images/btn-about-normal.png");
    btn1->setPosition(ccp(bgSize.width / 2.1 , 42));  //设置坐标
    btn1->setIsCanTouch(false);  //设置是否能触摸
    btn1->setOnClickCallback(menu_selector(GroupActions::btnClickPlay), this); //设置回调函数
    btn1->setLockMode(true); //设置锁模式
    btn1->setLockDt(3.0f);  //设置锁定时间
    msgLayer->addChild(btn1); //添加节点
  2、使用两张图片资源创建组合图标的Button
    Button *btn1 = Button::createBtn("Images/btn-about-normal.png","Images/btn-about-normal.png");
    btn1->setPosition(ccp(bgSize.width / 2.1 , 42));  //设置坐标
    btn1->setIsCanTouch(false);  //设置是否能触摸
    btn1->setOnClickCallback(menu_selector(GroupActions::btnClickPlay), this); //设置回调函数
    btn1->setLockMode(true); //设置锁模式
    btn1->setLockDt(3.0f);  //设置锁定时间
    msgLayer->addChild(btn1); //添加节点
*******************************/

class Button : public CCNode, public CCTargetedTouchDelegate
{
public:
	Button() : mClickCallback(NULL) , clickObj(NULL),
        mClickCallbacNode(NULL), clockNode(NULL)
		,isCanTouch(true)
		,isEffecttype(false)
		, mScale(1)
		,mIsLock(false)
		,mLockMode(false)
		,m_bIsSpecialEffect(false)
		,mLockDt(0.65f){}
	virtual ~Button();
    
    //使用图片创建Button
	static Button* createBtn(std::string iconStr);
    //使用两张图创建一个组合的button
	static Button* createBtn(std::string baseStr,std::string upStr);
//yin
//	//创建两个叠加精灵按钮
//	static Button* createBtnWith2SpriteFrameName(std::string baseStr,std::string upStr, bool bPlist = true);
//	// add by sanmor 创建可以显示数字的按钮
//	static Button *createBtn(std::string baseStr,std::string upStr,std::string moneyIcon,const char* numStr);
//	//创建两个特效的按钮
//	static Button* createEffectBtn(const char* baseEffectXmls,const char* baseFirstFrameName,const char* baseAnimateName,const char* upEffectXmls,const char* upFirstFrameName,const char* upAnimateName,const char* upWords);
//	//创建昆仑秘境专用按钮kk
//	static Button* createHomeBtn(std::string baseStr,std::string upStr,std::string wordIcon);

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	void onEnter();
	void onExit();
	virtual CCRect rect();

	bool containsTouchLocation(CCTouch* pTouch);
	void click(CCNode *node);

    //设置按钮按下后的回调函数
	void setOnClickCallback(SEL_CallFuncO cb, CCObject * obj){ mClickCallback = cb; clickObj = obj;/*clickObj->retain();*/}
    void setOnClickCallbackNode(SEL_CallFuncN cb, CCNode * obj){ mClickCallbacNode = cb; clockNode = obj;/*clickObj->retain();*/}
	//设置是否显示
	void setIsShow(bool  isShow);

    //触摸优先级设置
	void setHandlerPriority(int nHandler) {m_HandlerPriority = nHandler;}
	int getHandlerPriority(){return m_HandlerPriority;};

//yin    
//	CC_SYNTHESIZE(int,m_nCurClickedIndex,CurClickedIndex);

	
	bool init(std::string iconStr);
	bool init(std::string baseStr,std::string upStr);
//yin 
//	bool init(std::string baseStr,std::string upStr,std::string moneyIcon,const char* numStr);
//	bool initWithFrameName(std::string iconStr,bool bPlist = true);
//	bool initWith2FrameName(std::string baseStr,std::string upStr,bool bPlist = true);
//	bool init(std::string baseStr,std::string upStr,std::string wordIcon);
	//创建两个特效的按钮
//	bool init(const char* baseEffectXmls,const char* baseFirstFrameName,const char* baseAnimateName,const char* upEffectXmls,const char* upFirstFrameName,const char* upAnimateName,const char* upWords);
	
	void setBgScale(float scale) { mScale = scale; bg->setScale(mScale);}

	
	//设置锁住模式
	CC_SYNTHESIZE(bool,mLockMode,LockMode);
	//设置锁住时间
	CC_SYNTHESIZE(float,mLockDt,LockDt);
	//设置是否可点击
	CC_SYNTHESIZE(bool,isCanTouch,IsCanTouch);

	//是否为特殊音效
	CC_SYNTHESIZE(bool ,m_bIsSpecialEffect,IsSpecialEffect);

protected:
	CCSprite *bg;
	float mScale;
	int m_HandlerPriority;
	bool mIsLock;
private:
	SEL_CallFuncO mClickCallback;
	CCObject *clickObj;
    
    SEL_CallFuncN mClickCallbacNode;
    CCNode*   clockNode;

	const char* mbaseEffectXmls;	
	const char* mupEffectXmls;
	bool isEffecttype;


	void loadLock(float dt);
	void lockState();
	void unLockState();
	CCSprite* loadSp;//锁住图标

	/*bool isCanTouch;*/
	
	
};


#endif