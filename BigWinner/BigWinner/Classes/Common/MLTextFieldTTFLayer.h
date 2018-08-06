#ifndef __MM_TEXTFIELDTTF_WITH_CURSOR_H__
#define __MM_TEXTFIELDTTF_WITH_CURSOR_H__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
/** 
  * @brief 带光标的输入框
  */
class MLTextFieldTTFCursor : public cocos2d::CCTextFieldTTF , public CCTargetedTouchDelegate
{
public:

	/**
	 * @brief 生成一个带光标的输入框
	 * @param [in] fContentWidth 输入框的宽
	 * @param [in] fContentHeight 输入框的高
	 * @param [in] eAlignment 字体对齐方式 默认左对齐
	 * @param [in] pszFontName 字体名 默认值 Arial
	 * @param [in] fFontSize 字体大小默认 20.0f
	 * @param [in] fontColor 输入框的字体颜色默认白色
	 * @retval MMTextFieldTTFWithCursor* 带光标的输入框对象指针
	 * @retval NULL 生成失败
	 */
	static MLTextFieldTTFCursor* textFieldWithWidthAndHeight(float fContentWidth, float fContentHeight, CCTextAlignment eAlignment = kCCTextAlignmentLeft, const char* pszFontName = "Arial", const cocos2d::ccColor3B& fontColor = cocos2d::ccWHITE);


	bool initWithWidthAndHeight(float fContentWidth, float fContentHeight, CCTextAlignment eAlignment, const char* pszFontName, const ccColor3B& fontColor);
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	CCRect rect();

	bool containsTouchLocation(CCTouch* touch);

	/** @brief 绘制 */
	void draw();

	/** @brief 析构函数 */
	virtual ~MLTextFieldTTFCursor();


	/** @brief 更新光标 */
	virtual void updateCursor(float dt);


	/** @brief 添加输入框时的操作 */
	virtual void onEnter();


	/** @brief 移除输入框时的操作*/
	virtual void onExit();


	/**
     * @brief  打开键盘 开始文本输入
     */
    virtual bool attachWithIME();


    /**
     * @brief  结束文本输入 关闭键盘
     */
    virtual bool detachWithIME();


	/** 
	 * @brief 插入字符串
	 * @param [in] text 要插入的字符串
	 * @param [in] len  字符串长度
	 */
	virtual void insertText(const char * text, int len);


	/** 
	 * @brief 删除字符串
	 */
	virtual void deleteBackward();
 
    CC_SYNTHESIZE(bool, m_bIsNumber, bIsNumber); //是否是数字
    CC_SYNTHESIZE(bool, m_bIsPassword, bIsPassword);  //是否是密码形势
#if (COCOS2D_DEBUG == 1)
	/** @brief 绘制矩形区域*/
	//virtual void draw();
#endif

protected:

	/** 构造函数 */
	MLTextFieldTTFCursor();

	/** 光标的闪烁速度 */
	CC_SYNTHESIZE(float, m_fSpeed, fSpeed);

	/** 字数限制 */
	CC_SYNTHESIZE(int, m_nLimitFontCount, nLimitFontCount);
    
    //光标TTF
    CC_SYNTHESIZE(CCLabelTTF*, m_pCursorTTF, pCursorTTF);
    
    //字体大小
    CC_SYNTHESIZE(float, m_fFontSize, fFontSize);
    
    //对齐方式
    CC_SYNTHESIZE(CCTextAlignment, m_nTextAlignment, nTextAlignment);
    
    //字体名字
    CC_SYNTHESIZE(std::string, m_sFontName, sFontName);
    
    //回调函数
    CC_SYNTHESIZE(SEL_CallFuncND, m_pCallFuncND, pCallFuncND);
    
    //回调指针
    CC_SYNTHESIZE(CCObject*, m_pCallObj, pCallObj);
    
    CC_SYNTHESIZE(bool, m_bIsCursorShowed, bIsCursorShowed);
    

	/** 输入完成后是否隐藏字符串*/ 
	//CC_SYNTHESIZE(bool, m_bIsHideTheTextAfterInput, IsHideTheTextAfterInput);

	virtual void touchDelegateRetain(){return;}
	virtual void touchDelegateRelease(){return;}

};

class MLTextFieldTTFLayer : public cocos2d::extension::CCScale9Sprite
{
public:
    MLTextFieldTTFLayer();
    ~MLTextFieldTTFLayer();
    static MLTextFieldTTFLayer* create(const char* sSprName, float fWidth, float fHeight, CCTextAlignment eAlignment = kCCTextAlignmentLeft, const char* pszFontName = "Arial", const ccColor3B& fontColor = ccWHITE);
    
    bool init(const char* sSprName, float fWidth, float fHeight, CCTextAlignment eAlignment = kCCTextAlignmentLeft, const char* pszFontName = "Arial", const ccColor3B& fontColor = ccWHITE);
    
    CC_SYNTHESIZE(MLTextFieldTTFCursor*, m_pTTFCursor, pTTFCursor);
    
    //开始时候的默认字体
    CC_SYNTHESIZE(CCLabelTTF*, m_pDefultFont, pDefultFont);
    
};

#endif //__MM_TEXTFIELDTTF_WITH_CURSOR_H__


/*   使用shili
 m_pInput1 = MLTextFieldTTFLayer::create("Image/PublicUI/TextFieldBG.png", G_SF(342), G_SF(34), kCCTextAlignmentLeft);
 m_pInput1->setAnchorPoint(ccp(0,0));
 m_pInput1->getpTTFCursor()->setnLimitFontCount(15);
 m_pInput1->setPosition(ccp(G_SF(196), G_SF(380)));
 m_pInput1->getpDefultFont()->setString("请输入6-15位密码");
 m_pInput1->getpDefultFont()->setVisible(false);
 m_pInput1->getpTTFCursor()->setString(sPassword.c_str());
 */


