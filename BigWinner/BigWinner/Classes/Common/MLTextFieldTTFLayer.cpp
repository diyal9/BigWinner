#include "MLTextFieldTTFLayer.h"
#include "LGameCommonDefine.h"

#define NO_FONT_COUNT_LIMIT -1


// 构造函数
MLTextFieldTTFCursor::MLTextFieldTTFCursor():
m_fSpeed(0.5f),
m_nLimitFontCount(NO_FONT_COUNT_LIMIT),
m_bIsNumber(false)
{
    m_pCursorTTF = NULL;
    m_fFontSize = 0.0f;
    m_nTextAlignment = kCCTextAlignmentLeft;
    m_pCallFuncND = NULL;
    m_pCallObj = NULL;
    m_bIsPassword = false;
    m_bIsCursorShowed = true;
}


// 析构函数
MLTextFieldTTFCursor::~MLTextFieldTTFCursor()
{
	CC_SAFE_DELETE(m_pDelegate);
}


// 生成一个带光标的输入框
MLTextFieldTTFCursor* MLTextFieldTTFCursor::textFieldWithWidthAndHeight(float fContentWidth, float fContentHeight, cocos2d::CCTextAlignment eAlignment, const char* pszFontName, const cocos2d::ccColor3B& fontColor)
{
	MLTextFieldTTFCursor* pTextField = new MLTextFieldTTFCursor;

	if (pTextField && pTextField->initWithWidthAndHeight(fContentWidth, fContentHeight, eAlignment, pszFontName, fontColor))
	{
		pTextField->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pTextField);
	}

	return pTextField;
}


// 初始化带光标的输入框
bool MLTextFieldTTFCursor::initWithWidthAndHeight(float fContentWidth, float fContentHeight, CCTextAlignment eAlignment, const char* pszFontName, const cocos2d::ccColor3B& fontColor)
{
	do 
	{
		CCAssert(fContentWidth > 0.0f, "输入框的宽度必须大于零");

		// 设置属性
		CCSize contentSize = CCSizeMake(fContentWidth-G_SF(4), fContentHeight-G_SF(4));
		CC_BREAK_IF(!CCTextFieldTTF::initWithString("", pszFontName, contentSize.height - G_SF(4), contentSize, eAlignment));
        m_fFontSize = contentSize.height - G_SF(4);
        m_nTextAlignment = eAlignment;
        m_sFontName = pszFontName;
		this->setContentSize(contentSize);
		this->setColor(fontColor);

		return true;
	} while (0);
	
	return false;
}

// 绘制矩形区域
void MLTextFieldTTFCursor::draw()
{
	CCTextFieldTTF::draw();

//	CCPoint startPoint = CCPointZero;
//	CCSize contentSize = this->getContentSize();
//	CCPoint endPoint = CCPointMake(startPoint.x+contentSize.width, startPoint.y+contentSize.height);
//
//	glLineWidth( 2.0f );
//    glColorMask(0.0f, 0.0f, 0.0f, 0.5f);
//	ccDrawLine( startPoint, CCPointMake(startPoint.x, endPoint.y) );
//	ccDrawLine( startPoint, CCPointMake(endPoint.x, startPoint.y) );
//	ccDrawLine( endPoint, CCPointMake(startPoint.x, endPoint.y));
//	ccDrawLine( endPoint, CCPointMake(endPoint.x, startPoint.y));
//	glLineWidth( 1.0f );
//	glColorMask(1.0f, 1.0f, 1.0f, 1.0f);
}

// 更新光标
void MLTextFieldTTFCursor::updateCursor( float dt )
{
    std::string strText = this->getString();
    
    if(m_bIsPassword)
    {
        std::string sPassword(strText.size(), '*');
        CCLabelTTF::setString(sPassword.c_str());
    }
    else
        CCLabelTTF::setString(strText.c_str());

    float fFontWidth;

    if(m_bIsPassword)
    {
        std::string sPassword(strText.size(), '*');
        fFontWidth = CCImage::GetFontsRealWidth(sPassword.c_str(), m_sFontName.c_str(), m_fFontSize, this->getContentSize().width, this->getContentSize().height);
    }
    else
        fFontWidth = CCImage::GetFontsRealWidth(strText.c_str(), m_sFontName.c_str(), m_fFontSize, this->getContentSize().width, this->getContentSize().height);
    switch (m_nTextAlignment)
    {
        case kCCTextAlignmentLeft:
        {
            if(!getTexture())
                m_pCursorTTF->setPosition(ccp(G_SF(2), this->getContentSize().height/2));
            else
            {
                if(fFontWidth <= this->getContentSize().width - G_SF(2))
                {
                    m_pCursorTTF->setPosition(ccp(fFontWidth+G_SF(2), this->getContentSize().height/2));
                }
                else
                    m_pCursorTTF->setPosition(ccp(this->getContentSize().width - G_SF(2), this->getContentSize().height/2));
            }
        }
            break;
        case kCCTextAlignmentCenter:
        {
            if(!getTexture())
                m_pCursorTTF->setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));
            else
            {
                if(fFontWidth <= this->getContentSize().width - G_SF(2))
                {
                    m_pCursorTTF->setPosition(ccp((this->getContentSize().width + fFontWidth)/2+G_SF(2), this->getContentSize().height/2));
                }
                else
                    m_pCursorTTF->setPosition(ccp(this->getContentSize().width - G_SF(2), this->getContentSize().height/2));
            }
        }
            break;
        case kCCTextAlignmentRight:
        {
            m_pCursorTTF->setPosition(ccp(this->getContentSize().width-G_SF(1), this->getContentSize().height/2));
        }
            break;
            
        default:
            break;
    }
    
	if (m_bIsCursorShowed)
	{
        m_pCursorTTF->setVisible(true);
	}
	else
	{
        m_pCursorTTF->setVisible(false);
	}
    
    

	m_bIsCursorShowed = !m_bIsCursorShowed;
}

// 添加输入框时的操作
void MLTextFieldTTFCursor::onEnter()
{
	CCTextFieldTTF::onEnter();
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10003, true);
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -100778777777703, true);
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10003, true);
}

// 移除输入框时的操作
void MLTextFieldTTFCursor::onExit()
{
	CCTextFieldTTF::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

CCRect MLTextFieldTTFCursor::rect()
{
	return CCRectMake(0, 0,this->getContentSize().width,this->getContentSize().height);
}

bool MLTextFieldTTFCursor::containsTouchLocation(CCTouch* touch)
{
    CCPoint point = this->convertToNodeSpace(touch->getLocation());
    return rect().containsPoint(point);
}

bool MLTextFieldTTFCursor::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if (!isVisible())
	{
		return false;
	}
    if ( containsTouchLocation(pTouch) )
    {
        this->attachWithIME();
        return true;

    }
    this->detachWithIME();
    return false;
}

void MLTextFieldTTFCursor::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

}

// 打开键盘 开始输入文本
bool MLTextFieldTTFCursor::attachWithIME()
{
	bool bResult = false;

	bResult = CCTextFieldTTF::attachWithIME();

	// 开始更新光标
//	if (bResult)
	{
        this->unschedule(schedule_selector(MLTextFieldTTFCursor::updateCursor));
		this->schedule(schedule_selector(MLTextFieldTTFCursor::updateCursor), m_fSpeed);
	}

	this->setVisible(true);
    if(!m_pCursorTTF)
    {
        m_pCursorTTF = CCLabelTTF::create("|", "Arial", m_fFontSize);
        m_pCursorTTF->setColor(ccBLACK);
        this->addChild(m_pCursorTTF, 1);
        
        switch (m_nTextAlignment)
        {
            case kCCTextAlignmentLeft:
            {
                if(!getTexture())
                    m_pCursorTTF->setPosition(ccp(G_SF(2), this->getContentSize().height/2));
                else
                {
                    if(getTexture()->getContentSize().width <= this->getContentSize().width - G_SF(2))
                    {
                        m_pCursorTTF->setPosition(ccp(getTexture()->getContentSize().width+G_SF(2), this->getContentSize().height/2));
                    }
                    else
                        m_pCursorTTF->setPosition(ccp(this->getContentSize().width - G_SF(2), this->getContentSize().height/2));
                }
            }
                break;
            case kCCTextAlignmentCenter:
            {
                if(!getTexture())
                    m_pCursorTTF->setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));
                else
                {
                    if(getTexture()->getContentSize().width <= this->getContentSize().width - G_SF(2))
                    {
                        m_pCursorTTF->setPosition(ccp((this->getContentSize().width + getTexture()->getContentSize().width)/2+G_SF(2), this->getContentSize().height/2));
                    }
                    else
                        m_pCursorTTF->setPosition(ccp(this->getContentSize().width - G_SF(2), this->getContentSize().height/2));
                }
            }
                break;
            case kCCTextAlignmentRight:
            {
                m_pCursorTTF->setPosition(ccp(this->getContentSize().width-G_SF(1), this->getContentSize().height/2));
            }
                break;
                
            default:
                break;
        }
        m_pCursorTTF->setVisible(false);
    }
    
    bool b = true;
    if(m_pCallFuncND)
        (m_pCallObj->*m_pCallFuncND)(NULL, &b);
    
    MLTextFieldTTFLayer* pLayer = (MLTextFieldTTFLayer*)getParent();
    pLayer->getpDefultFont()->setVisible(false);

	return bResult;
}

// 结束输入文本 关闭键盘
bool MLTextFieldTTFCursor::detachWithIME()
{
	bool bResult = false;

	bResult = CCTextFieldTTF::detachWithIME();

	// 停止更新光标
//	if (bResult)
	{
		this->unschedule(schedule_selector(MLTextFieldTTFCursor::updateCursor));
	}
	// 直接现实文本 以防光标停留
    
    if(m_bIsPassword)
    {
        std::string strText = this->getString();
        std::string sPassword(strText.size(), '*');
        CCLabelTTF::setString(sPassword.c_str());
    }
    else
        CCLabelTTF::setString(this->getString());
    
    std::string s = this->getString();
    if(s.size() == 0)
    {
        MLTextFieldTTFLayer* pLayer = (MLTextFieldTTFLayer*)getParent();
        pLayer->getpDefultFont()->setVisible(true);
    }
    
    if(m_pCursorTTF)
        m_pCursorTTF->setVisible(false);
    
    bool b = false;
    if(m_pCallFuncND)
        (m_pCallObj->*m_pCallFuncND)(NULL, &b);

	return bResult;
}

// 插入字符串
void MLTextFieldTTFCursor::insertText( const char * text, int len )
{
    std::string sText = text;
    if(sText.compare("\n") == 0)
    {
        //by wang 20131219
//        CCTextFieldTTF::insertText(text, len);
//        m_pCursorTTF->setVisible(false);
        return;
    }
    
	if ((NO_FONT_COUNT_LIMIT == m_nLimitFontCount) || (this->m_nCharCount + len <= m_nLimitFontCount) )
	{
		if (m_bIsNumber == true) //只输入数字
		{
			if (*text< 48 || *text>57) //len为1
			{
				CCLOG("InputLimted is number is True");
				return;
			}
		}
        
        if( m_bIsPassword == true) //输入密码模式
        {
            if(*text < 0)
            {
                CCLOG("InputLimted is password is True");
                return;
            }
        }
		CCTextFieldTTF::insertText(text, len);
        updateCursor(0.0);
        
	}
}

// 删除字符串
void MLTextFieldTTFCursor::deleteBackward()
{
	CCTextFieldTTF::deleteBackward();
    
    updateCursor(0.0);
    
}






//以下是对外的字符输入框接口实现

MLTextFieldTTFLayer::MLTextFieldTTFLayer()
{
    m_pTTFCursor = NULL;
    m_pDefultFont = NULL;
}

MLTextFieldTTFLayer::~MLTextFieldTTFLayer()
{
    
}

MLTextFieldTTFLayer* MLTextFieldTTFLayer::create(const char* sSprName, float fWidth, float fHeight, CCTextAlignment eAlignment, const char* pszFontName, const ccColor3B& fontColor)
{
    MLTextFieldTTFLayer* pTTFLayer = new MLTextFieldTTFLayer();
    if(pTTFLayer && pTTFLayer->init(sSprName, fWidth, fHeight, eAlignment, pszFontName, fontColor))
    {
        pTTFLayer->autorelease();
        return pTTFLayer;
    }
    else
    {
        CC_SAFE_DELETE(pTTFLayer);
        return NULL;
    }
    
}

bool MLTextFieldTTFLayer::init(const char* sSprName, float fWidth, float fHeight, CCTextAlignment eAlignment, const char* pszFontName, const ccColor3B& fontColor)
{
    if(!initWithFile(sSprName))
        return false;
    
    setPreferredSize(CCSizeMake(fWidth, fHeight));
    
    m_pTTFCursor = MLTextFieldTTFCursor::textFieldWithWidthAndHeight(fWidth, fHeight, eAlignment, pszFontName, fontColor);
    m_pTTFCursor->setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));
    
    this->addChild(m_pTTFCursor, 1);
    
    m_pDefultFont = CCLabelTTF::create("", "Arial", m_pTTFCursor->getfFontSize());
    
    if(eAlignment == kCCTextAlignmentLeft)
    {
        m_pDefultFont->setAnchorPoint(ccp(0, 0.5));
        m_pDefultFont->setPosition(ccp(0, m_pTTFCursor->getContentSize().height/2));
    }
    else if( eAlignment == kCCTextAlignmentCenter)
    {
        m_pDefultFont->setAnchorPoint(ccp(0.5, 0.5));
        m_pDefultFont->setPosition(ccp(m_pTTFCursor->getContentSize().width/2, m_pTTFCursor->getContentSize().height/2));
    }
    else
    {
        m_pDefultFont->setAnchorPoint(ccp(1, 0.5));
        m_pDefultFont->setPosition(ccp(m_pTTFCursor->getContentSize().width, m_pTTFCursor->getContentSize().height/2));
    }
    m_pDefultFont->setColor(ccc3(122, 122, 122));
    
    this->addChild(m_pDefultFont, 2);
    
    return true;
    
}










