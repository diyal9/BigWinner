
#include <cstdlib>
//#include "conv.h"
#include "RichLabel.h"
//#include "LabelStroke.h"
//#include "Global.h"

const int FONT_HEIGHT_GAP = 6;
 
 const char *TAG_END = "</>";
 const char *TAG_VALUE = "<value>";
 
 const char *TAG_COLOR = "<color>";
 const char *TAG_EMOJ = "<emoj>";
 const char *TAG_LINK = "<link>";
 const char *TAG_FONT = "<font>";
 
 const char *TAG_NEWLINE = "<n/>";
 const char *TAG_NEWSECTION = "<p/>";
 const char *TAG_TAB = "<t/>";
 
 const char *TAG_LINKTYPE = "<linktype>";
 const char *TAG_LINKTARGET = "<targetid>";
 
 const char *TAG_FONTNAME = "<fontname>";
 const char *TAG_FONTSIZE = "<fontsize>";



RichLabel * RichLabel::create(const char* string, const char* fontName, const int fontSize, CCSize labelSize, bool appendString, bool enableShadow)
{
    //wangguolong
//	RichLabel *label = _new RichLabel();
    RichLabel *label = new RichLabel();
	if (label && label->init(string, fontName, fontSize, labelSize, appendString, enableShadow))
	{
		label->autorelease();
		return label;
	}
	CC_SAFE_DELETE(label);
	return NULL;
}

bool RichLabel::init(const char* string, const char* fontName, const int fontSize, CCSize labelSize, bool appendString, bool enableShadow)
{
	mColorTagFound = false;
	mFontTagFound = false;
	mLinkTagFound = false;
	mEmojTagFound = false;
	mEnableShadow = enableShadow;

	mSize = labelSize;
	
	std::string tmpStr = string;

	int offset = tmpStr.find('\n');
	if (offset)
	{
		while (offset > 0)
		{
			tmpStr.replace(offset, 1, TAG_NEWLINE);
			offset = tmpStr.find('\n', offset);
		}
	}
	
	mString = tmpStr;
	
	setColor(ccWHITE);

	mAllowAppendString = appendString;

	if (!CCLayerRGBA::init())
		return false;

	setContentSize(labelSize);
	setTouchEnabled(true);

	mLabelOffsetX = 0;
	mLabelOffsetY = labelSize.height - (fontSize + FONT_HEIGHT_GAP);

	mFontName = mDefaultFontName = fontName;
	mFontSize = mDefaultFontSize = fontSize;

	mContentSize.width = 0;
	mContentSize.height = mFontSize + FONT_HEIGHT_GAP;

	if (!mAllowAppendString)
	{
		parse(mString.c_str(), true);
		mParsed = true;
		//setContentSize(mContentSize);
	}

	//registerWithTouchDispatcher();

	return true;
}

void RichLabel::onEnter()
{
	if (!mParsed)
	{
		parse(mString.c_str(), true);
		mParsed = true;
		//setContentSize(mContentSize);
	}

	CCLayerRGBA::onEnter();
	//registerWithTouchDispatcher();
}

ccColor3B _color(int color)
{
	GLubyte r, g, b;

	r = (color >> 16);
	g = (color >> 8);
	b = color;

	return ccc3(r, g, b);
}

//void RichLabel::setColor(ccColor3B color)
//{
//	mColor = (color.r << 16 | color.g << 8 | color.b);
//}

void RichLabel::appendString(const char* string)
{
	mString.append(string);
}

bool RichLabel::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint point = convertToNodeSpace(pTouch->getLocation());

	std::vector<Link>::const_iterator i = mLinkMap.begin(), i_end = mLinkMap.end();
	for (; i!=i_end; ++i)
	{
		if ((*i).rect.containsPoint(point))
		{
			return true;
		}
	}
	
	return false;
}

void RichLabel::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint point = convertToNodeSpace(pTouch->getLocation());

	std::vector<Link>::const_iterator i = mLinkMap.begin(), i_end = mLinkMap.end();
	for (; i!=i_end; ++i)
	{
		if ((*i).rect.containsPoint(point))
		{
            //wangguolong
//			Global::getInstance()->playEffect(1, false);
			if (mLinkDelegate)
				mLinkDelegate->linkClick(this, (*i).linkType, (*i).linkTarget);
			break;
		}
	}
	
}

void RichLabel::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority - 20, false);
}

const char * RichLabel::parse(const char* string, bool recurse)
{
	int pos = 0;
	TagType tag = None;
	const char * temp = string;
	
	do
	{
		if (temp == NULL || strlen(temp) == 0)
			return NULL;

		tag = searchTag(temp, pos);
		switch (tag)
		{
		case None:
			createLabel(temp, -1, !mFontTagFound);
			return NULL;
		case EndTag:
			{
				if (pos)
					createLabel(temp, pos, !mFontTagFound);
				return (temp + pos);
			}
			break;
		case FontTag:
			{
				if (pos)
					createLabel(temp, pos, !mFontTagFound);
				temp = parseFont(temp + pos);
			}
			break;
		case ColorTag:
			{
				if (pos)
					createLabel(temp, pos, !mFontTagFound);
				temp = parseColor(temp + pos);
			}
			break;
		case LinkTag:
			{
				if (pos)
					createLabel(temp, pos, !mFontTagFound);
				temp = parseLink(temp + pos);
			}
			break;
		case EmojTag:
			{
				if (pos)
					createLabel(temp, pos, !mFontTagFound);
				temp = parseEmoj(temp + pos);
			}
			break;
		case LineTag:
			{
				if (pos)
					createLabel(temp, pos, !mFontTagFound);
				temp = parseLine(temp + pos);

			}
			break;
		case SectionTag:
			{
				if (pos)
					createLabel(temp, pos, !mFontTagFound);
				temp = parseSection(temp + pos);
			}
			break;
		case TabTag:
			{
				if (pos)
					createLabel(temp, pos, !mFontTagFound);
				temp = parseTab(temp + pos);
			}
			break;
		}
	}
	while (recurse && temp && strlen(temp) != 0);

	return temp;
}

RichLabel::TagType RichLabel::searchTag(const char* string, int & pos)
{
	const char * temp = string;
	int i = 0, len = strlen(string);

	while (i < len)
	{
		if (temp[i] == '<')
		{
			pos = i;
			if (strncmp(TAG_FONT, temp+i, strlen(TAG_FONT)) == 0)
				return FontTag;
			else if (strncmp(TAG_COLOR, temp+i, strlen(TAG_COLOR)) == 0)
				return ColorTag;
			else if (strncmp(TAG_EMOJ, temp+i, strlen(TAG_EMOJ)) == 0)
				return EmojTag;
			else if (strncmp(TAG_LINK, temp+i, strlen(TAG_LINK)) == 0)
				return LinkTag;
			else if (strncmp(TAG_NEWLINE, temp+i, strlen(TAG_NEWLINE)) == 0)
				return LineTag;
			else if (strncmp(TAG_NEWSECTION, temp+i, strlen(TAG_NEWSECTION)) == 0)
				return SectionTag;
			else if (strncmp(TAG_TAB, temp+i, strlen(TAG_TAB)) == 0)
				return TabTag;
			else if (strncmp(TAG_END, temp+i, strlen(TAG_END)) == 0)
				return EndTag;
			else
				i++;
		}
		else
			i++;
	}

	return None;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
#include "jni/JniHelper.h"
#include <android/log.h>
#include <string.h>
#include <jni.h>

static int measureWidth = 0;
static int measurenHeight = 0;

extern "C"
{
	void Java_org_cocos2dx_lib_Cocos2dxBitmap_nativeTextProperty(JNIEnv*  env, jobject thiz, int width, int height)
	{
		measureWidth = width;
		measurenHeight = height;
	}
};
#endif

void RichLabel::measureString(const char* string, float &width, float &height)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
	measureWidth = width;
	measurenHeight = height;

	JniMethodInfo methodInfo;
        if (! JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/lib/Cocos2dxBitmap", "computeTextProperty", 
            "(Ljava/lang/String;Ljava/lang/String;IIII)V"))
        {
            CCLOG("%s %d: error to get methodInfo", __FILE__, __LINE__);
            return;
        }

        jstring jstrText = methodInfo.env->NewStringUTF(string);
        jstring jstrFont = methodInfo.env->NewStringUTF(mFontName.c_str());

        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jstrText, 
            jstrFont, (int)mFontSize, CCImage::kAlignTopLeft, 0, 0);

        methodInfo.env->DeleteLocalRef(jstrText);
        methodInfo.env->DeleteLocalRef(jstrFont);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);

	width = measureWidth;
	height = measurenHeight;
#else//if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	CCImage image;
	CCImage::ETextAlign eAlign = CCImage::kAlignTopLeft;

	if (!image.initWithString(string, 0, 0, eAlign, mFontName.c_str(), (int)mFontSize))
	{
		width = getContentSize().width;
		height = mFontSize + FONT_HEIGHT_GAP;
	}
	else
	{
		width = image.getWidth();
		height = image.getHeight();
	}
#endif
}

int RichLabel::searchTruncatePos(const char *string, int pixelWidth)
{
	int pos = 0;
	int len = strlen(string);
	//CCLOG("%s, length=%d, left width=%d", string, len, pixelWidth);

	if (len == 0)
		return 0;

	uint16_t *wdest = new uint16_t[len+2];
	memset(wdest, 0, (len+2)*sizeof(uint16_t));
    //wangguolong
//	utf82ucs2((char*)string, len, (char*)wdest, len*sizeof(uint16_t));  // 多字节字符串在折半查找过程中无法判断字符编码边界，因此统一转成ucs2宽字符进行折半查找
	
	int wlen = 0;
	for(int i=0; i<len+2; i++)
		if (wdest[i] == 0)
		{
			wlen = i;
			break;
		}
	
#if !defined(WIN32)
	// libicuuc.so - first unicode char is 0xFEFF
	wlen -= 1;
	uint16_t *wchar = wdest + 1; 
#else
	uint16_t *wchar = wdest; 
#endif

	if (wlen <= 1)
	{
		delete []wdest;

		//CCLabelTTF * label = CCLabelTTF::create(string, mFontName.c_str(), mFontSize);
		//float width = label->getContentSize().width;
		//CCLOG("len=%d, width=%f", wlen, width);
		//label->release();
		float width = 0, height = 0;
		measureString(string, width, height);
		if (width > pixelWidth)
			return 0;
		else
			return strlen(string);
	}
	
	//CCLOG("wlen=%d", wlen);
	
	uint16_t truncateChar = wchar[wlen/2];
	wchar[wlen/2] = 0;
	
#if 0
	CCLOG("part1:");
	for(int i=0; i<wlen/2; i++)
	{
		CCLOG("%X", wchar[i]);
	}
	CCLOG("part2:");
	for(int i=wlen/2 + 1; i<wlen; i++)
	{
		CCLOG("%X", wchar[i]);
	}
#endif

	char *dest = new char[len+1];
	memset(dest, 0, len+1);
    //wangguolong
//	ucs22utf8((char*)wdest, wlen*sizeof(uint16_t), dest, len);

#if 0
	CCLOG("1:");
	for(int i=0; i<len; i++)
	{
		CCLOG("%X", dest[i]);
	}
#endif

	//CCLabelTTF * label = CCLabelTTF::create(dest, mFontName.c_str(), mFontSize);
	//float width = label->getContentSize().width;
	//label->release();
	float width = 0, height = 0;
	measureString(dest, width, height);

	//CCLOG("width=%f", width);
	if (width > pixelWidth)
	{
		pos = searchTruncatePos(dest, pixelWidth);
		delete []wdest;
		delete []dest;
	}
	else if (width < pixelWidth)
	{
		pos = strlen(dest);
		
		wchar[wlen/2] = truncateChar;
#if !defined(WIN32)
		// libicuuc.so - first unicode char is 0xFEFF
		wchar[wlen/2-1] = 0xFEFF;
		wchar += (wlen/2-1);
#else
		wchar += wlen/2;
#endif

		memset(dest, 0, len+1);
        //wangguolong
//		ucs22utf8((char*)wchar, (wlen/2)*sizeof(uint16_t), dest, len);
		
#if 0
		CCLOG("2:");
		for(int i=0; i<len; i++)
		{
			CCLOG("%X", dest[i]);
		}
#endif

		int width1 = (int)width;
		pos += searchTruncatePos(dest, pixelWidth - width1);

		delete []wdest;
		delete []dest;
	}
	else
	{
		pos = strlen(dest);
		delete []wdest;
		delete []dest;
	}

	return pos;
}

void RichLabel::createLabel(const char * string, int length, bool defaultFont)
{
	std::string str;

	if (length == -1)
		str = string;
	else
		str = std::string(string, length);

	if (defaultFont)
	{
		mFontName = mDefaultFontName;
		mFontSize = mDefaultFontSize;
	}

	if (str.length())
	{
		//CCLabelTTF * label = CCLabelTTF::create(str.c_str(), mFontName.c_str(), mFontSize);
		//CCLOG("row width=%f, label left width=%f", label->getContentSize().width, mSize.width - mLabelOffsetX);
		float width = 0, height = 0;
		measureString(str.c_str(), width, height);
		//if (label->getContentSize().width > mSize.width - mLabelOffsetX) // 字符串显示超过行边界，折断分成两行分别用两个label处理
		if (width > mSize.width - mLabelOffsetX) // 字符串显示超过行边界，折断分成两行分别用两个label处理
		{
			//label->release();
			int width = (int)(mSize.width - mLabelOffsetX);
			int pos = searchTruncatePos(str.c_str(), width);
			if (pos == 0) // 此行已无法完全显示一个字符,则从下一行创建label
			{
				mLabelOffsetX = 0;
				mLabelOffsetY -= (mFontSize + FONT_HEIGHT_GAP);
				mContentSize.width = mSize.width;
				mContentSize.height += mFontSize + FONT_HEIGHT_GAP;

				createLabel(str.c_str(), -1, false); //wangguolong
				
				if (mLabelOffsetY < 0)
					setContentSize(CCSizeMake(mSize.width, mSize.height - mLabelOffsetY + FONT_HEIGHT_GAP));
			}
			else
			{
				createLabel(str.c_str(), pos, false);
				//mLabelOffsetX = 0;
				//mLabelOffsetY -= (mFontSize + FONT_HEIGHT_GAP);
				createLabel(str.c_str()+pos, -1, false);
			}
		}
		else // 单行即可完全显示
		{
			CCSize labelSize;

//			if (mEnableShadow == true)
//
//			{
//                //wangguolong
//                CCLOG("LabelStroke is delete");
////				LabelStroke * label = LabelStroke::create(str.c_str(), mFontName.c_str(), mFontSize);
////				//label->setAnchorPoint(CCPointZero);
////				label->setPosition(ccp(mLabelOffsetX + getAnchorPoint().x * label->getContentSize().width, 
////					mLabelOffsetY + getAnchorPoint().y * label->getContentSize().height));
////				addChild(label);
////
////				//if (mColorTagFound)
////				label->setColor(_color(mColor));
////				//else
////				//	label->setColor(ccWHITE);
////				labelSize = label->getContentSize();
//			}
//			else
//			{
				CCLabelTTF * label = CCLabelTTF::create(str.c_str(), mFontName.c_str(), mFontSize);
				label->setPosition(ccp(mLabelOffsetX + getAnchorPoint().x * label->getContentSize().width, 
					mLabelOffsetY + getAnchorPoint().y * label->getContentSize().height));
				addChild(label);

				label->setColor(_color(mColor));
				labelSize = label->getContentSize();
//			}
			

			//CCSize labelSize = label->getContentSize();
			mContentSize.width += labelSize.width;
			if (mContentSize.width > getContentSize().width)
				mContentSize.width = getContentSize().width;

			if (mLinkTagFound)
			{
				CCRect rect = CCRectMake(mLabelOffsetX /*+ getAnchorPoint().x * label->getContentSize().width*/,
						mLabelOffsetY /*+ getAnchorPoint().y * label->getContentSize().height*/, labelSize.width, labelSize.height);
				mLink.rect = rect;
				mLinkMap.push_back(Link(mLink));
			}

			mLabelOffsetX += labelSize.width;

			if (mLabelOffsetX >= getContentSize().width)
			{
				mLabelOffsetX = 0;
				mLabelOffsetY -= (mFontSize + FONT_HEIGHT_GAP);

				mContentSize.height += mFontSize + FONT_HEIGHT_GAP;
			}
		}
		
	}
}

const char * RichLabel::parseFont(const char* string)
{
	const char * tempBegin = strstr(string, TAG_FONT);  // <font>
	const char * tempEnd = NULL;

    //wangguolong
    if (tempEnd = strstr(tempBegin, TAG_FONTNAME)) //<fontname>
//	if (tempEnd == strstr(tempBegin, TAG_FONTNAME)) //<fontname>
	{
		tempBegin = tempEnd + strlen(TAG_FONTNAME);
		tempEnd = strstr(tempBegin, TAG_END);

		mFontName = std::string(tempBegin, tempEnd - tempBegin);
        
		if (tempEnd = strstr(tempEnd+strlen(TAG_END), TAG_FONTSIZE)) //<fontsize>
//		if (tempEnd == strstr(tempEnd+strlen(TAG_END), TAG_FONTSIZE)) //<fontsize>
		{
			tempBegin = tempEnd + strlen(TAG_FONTSIZE);
			tempEnd = strstr(tempBegin, TAG_END);

			std::string fontSize(tempBegin, tempEnd - tempBegin);
			std::istringstream is(fontSize);
			is >> mFontSize;
		}
	}
	
	mFontTagFound = true;
	tempBegin = tempEnd + strlen(TAG_END);
	//tempEnd = strstr(tempBegin, TAG_END);

	//std::string str(tempBegin/*, tempEnd-tempBegin*/);
	tempEnd = parse(tempBegin, true);
	mFontTagFound = false;

	tempEnd = strstr(tempEnd, TAG_END);
	return tempEnd + strlen(TAG_END);
}

const char * RichLabel::parseLink(const char* string)
{
	const char * tempBegin = strstr(string, TAG_LINK);  // <link>
	const char * tempEnd = NULL;

	int linktype, linktarget;
    
    //wangguolong
    if (tempEnd = strstr(tempBegin, TAG_LINKTYPE)) //<linktype>
//	if (tempEnd == strstr(tempBegin, TAG_LINKTYPE)) //<linktype>
	{
		tempBegin = tempEnd + strlen(TAG_LINKTYPE);
		tempEnd = strstr(tempBegin, TAG_END);

		std::string linkType(tempBegin, tempEnd - tempBegin);
		std::istringstream is(linkType);
		is >> linktype;

        //wangguolong
        if (tempEnd = strstr(tempEnd+strlen(TAG_END), TAG_LINKTARGET)) //<targetid>
//		if (tempEnd == strstr(tempEnd+strlen(TAG_END), TAG_LINKTARGET)) //<targetid>
		{
			tempBegin = tempEnd + strlen(TAG_LINKTARGET);
			tempEnd = strstr(tempBegin, TAG_END);

			std::string linkTarget(tempBegin, tempEnd - tempBegin);
			std::istringstream is(linkTarget);
			is >> linktarget;
		}
	}

	mLink.linkType = (LinkType)linktype;
	mLink.linkTarget = linktarget;
	mLinkTagFound = true;

	tempBegin = tempEnd + strlen(TAG_END);
	//tempEnd = strstr(tempBegin, TAG_END);

	//std::string str(tempBegin/*, tempEnd-tempBegin*/);
	tempEnd = parse(tempBegin, true);
	mLinkTagFound = false;

	tempEnd = strstr(tempEnd, TAG_END);
	return tempEnd + strlen(TAG_END);
}

const char * RichLabel::parseColor(const char* string)
{
	const char * tempBegin = strstr(string, TAG_COLOR);  // <color>
	const char * tempEnd = NULL;

    //wangguolong
    if (tempEnd = strstr(tempBegin, TAG_VALUE)) //<value>
//	if (tempEnd == strstr(tempBegin, TAG_VALUE)) //<value>
	{
		tempBegin = tempEnd + strlen(TAG_VALUE);
		tempEnd = strstr(tempBegin, TAG_END);

		std::string color(tempBegin, tempEnd - tempBegin);
		std::istringstream is(color);
		is >> mColor;
	}
	
	mColorTagFound = true;

	tempBegin = tempEnd + strlen(TAG_END);
	//tempEnd = strstr(tempBegin, TAG_END);

	//std::string str(tempBegin/*, tempEnd-tempBegin*/);
	tempEnd = parse(tempBegin, true);
	mColorTagFound = false;
	mColor = mDefaultColor;

	tempEnd = strstr(tempEnd, TAG_END);
	return tempEnd + strlen(TAG_END);
}
const char * RichLabel::parseEmoj(const char* string)
{
	const char * tempBegin = strstr(string, TAG_EMOJ);  // <emoj>
	const char * tempEnd = NULL;
    

    //wangguolong
    if (tempEnd = strstr(tempBegin, TAG_VALUE)) //<value>
//	if (tempEnd == strstr(tempBegin, TAG_VALUE)) //<value>
	{
		tempBegin = tempEnd + strlen(TAG_VALUE);
		tempEnd = strstr(tempBegin, TAG_END);

		tempBegin = tempEnd + strlen(TAG_END);
	}
	else
		tempBegin = tempBegin + strlen(TAG_EMOJ);

	tempEnd = strstr(tempBegin, TAG_END);

	std::string emoj(tempBegin, tempEnd - tempBegin);
	
	CCSprite * _emoj = NULL;
	CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(emoj.c_str());
	if (frame)
		_emoj = CCSprite::createWithSpriteFrameName(emoj.c_str());
	else
		_emoj = CCSprite::create(emoj.c_str());
	
	_emoj->setAnchorPoint(CCPointZero);

	mFontSize = mFontTagFound ? mFontSize : mDefaultFontSize;
	
	if (/*mFontSize*/_emoj->getContentSize().width + mLabelOffsetX > mSize.width)
	{
		mLabelOffsetX = 0;
		mLabelOffsetY -= (/*mFontSize*/_emoj->getContentSize().width + FONT_HEIGHT_GAP);
		mContentSize.height += _emoj->getContentSize().width + FONT_HEIGHT_GAP;
		mContentSize.width = mSize.width;
	}
	else
	{
		mContentSize.width += _emoj->getContentSize().width*((float)mFontSize/_emoj->getContentSize().height) + 5.0f;
	}

	_emoj->setPosition(ccp(mLabelOffsetX, mLabelOffsetY/*-FONT_HEIGHT_GAP*/));
	_emoj->setScale((float)mFontSize/_emoj->getContentSize().height);
	addChild(_emoj);
	if (mLinkTagFound)
	{
		CCRect rect = CCRectMake(mLabelOffsetX+getAnchorPoint().x*_emoj->getContentSize().width, 
			(mLabelOffsetY/*-FONT_HEIGHT_GAP*/) + getAnchorPoint().y*_emoj->getContentSize().height, _emoj->getContentSize().width, _emoj->getContentSize().height);
		mLink.rect = rect;
		mLinkMap.push_back(Link(mLink));
	}

	mLabelOffsetX += _emoj->getContentSize().width*((float)mFontSize/_emoj->getContentSize().height) + 5.0f;

	return tempEnd + strlen(TAG_END);
}

const char * RichLabel::parseLine(const char* string)
{
	mLabelOffsetX = 0;
	mLabelOffsetY -= (mFontSize + FONT_HEIGHT_GAP);
	mContentSize.height += mFontSize + FONT_HEIGHT_GAP;

	return string + strlen(TAG_NEWLINE);
}

const char * RichLabel::parseSection(const char* string)
{
	mLabelOffsetX = 0;
	mLabelOffsetY -= 2*(mFontSize + FONT_HEIGHT_GAP);
	mContentSize.height += 2*(mFontSize + FONT_HEIGHT_GAP);

	return string + strlen(TAG_NEWSECTION);
}

const char * RichLabel::parseTab(const char* string)
{
	mLabelOffsetX += 40;

	return string + strlen(TAG_TAB);
}

GLubyte RichLabel::getOpacity(void)
{
	CCObject* child;
	CCARRAY_FOREACH(getChildren(), child)
	{
		CCRGBAProtocol* pNode = dynamic_cast<CCRGBAProtocol*>(child);
		if (pNode)
		{
			return pNode->getOpacity();
		}
	}

	return 0;
}

void RichLabel::setOpacity(GLubyte opacity)
{
	CCObject* child;
	CCARRAY_FOREACH(getChildren(), child)
	{
		CCRGBAProtocol* pNode = dynamic_cast<CCRGBAProtocol*>(child);
		if (pNode)
		{
			pNode->setOpacity(opacity);
		}
	}
}

const ccColor3B& RichLabel::getColor(void)
{
	return m_sColor;
}

void RichLabel::setColor(const ccColor3B& color3)
{
	m_sColor = color3;
	mColor = (color3.r << 16 | color3.g << 8 | color3.b);
	mDefaultColor = mColor;
}

void RichLabel::setOpacityModifyRGB(bool bValue)
{
}

bool RichLabel::isOpacityModifyRGB(void)
{
	return true;
}