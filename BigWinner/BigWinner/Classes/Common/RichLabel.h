#ifndef __RICHLABEL_H__
#define __RICHLABEL_H__

#include "cocos2d.h"



USING_NS_CC;

enum LinkType
{
	LinkItem = 0,
	LinkBeing = 1,
};

/* 
 创建Label 可以实现不同大小、颜色的字体同时显示。
 std::ostringstream roleStr;
 roleStr << "<font><fontname>Arial</><fontsize>20</><color><value>1356820</>" << "绿色23333" <<"</></>";//玩家名 绿色
 roleStr << "<font><fontname>Arial</><fontsize>20</><color><value>2626570</>" << "黑色" << "</></>";//玩家发送的消息
 RichLabel * pRoleStr = RichLabel::create(roleStr.str().c_str(), "Arial", 20, CCSizeMake(270,200), false, false);
 pRoleStr->setPosition(ccp(50, 100));
 color_layer->addChild(pRoleStr);
 */

class RichLabel;
/*
 实现一个可点击的Label
 */
class RichLabelDelegate
{
public:
	virtual ~RichLabelDelegate() {}
	virtual void linkClick(RichLabel* label, const LinkType linkType, const int targetId) = 0;
};
/*
 创建Label
 */
class RichLabel : public CCLayerRGBA /*public CCTouchDelegate, public CCRGBAProtocol*/
{
public:
	RichLabel() : mLinkDelegate(NULL), mParsed(false), mAllowAppendString(false) {}
	~RichLabel() {}

    /*
     创建一个Label
     参数：名称，字体种类，字体尺寸
     */
	static RichLabel * create(const char* string, const char* fontName, const int fontSize, CCSize labelSize, bool appendString = false, bool enableShadow = true);
    /*
     创建一个Label的具体实现
     参数：名称，字体种类，字体尺寸
     */
	bool init(const char* string, const char* fontName, const int fontSize, CCSize labelSize, bool appendString = false, bool enableShadow = true);

    /*获取Label的名称*/
	const char * getRichString() { return mString.c_str();}
    /*获取Label的尺寸*/
	CCSize getTextSize() { return mContentSize;};
    /*把Label做成可点击*/
	void setLinkDelegate(RichLabelDelegate * delegate) { mLinkDelegate = delegate; }

	//void setColor(ccColor3B color);
    /*在字符串后添加另外的字符串*/
	void appendString(const char* string);

	//virtual void setOpacity(GLubyte opacity);
	//virtual GLubyte getOpacity(void);

	/** Opacity: conforms to CCRGBAProtocol protocol */
	CC_PROPERTY(GLubyte, m_nOpacity, Opacity)
	/** Color: conforms with CCRGBAProtocol protocol */
	CC_PROPERTY_PASS_BY_REF(ccColor3B, m_sColor, Color);
	// RGBAProtocol
	/** opacity: conforms to CCRGBAProtocol protocol */
	virtual void setOpacityModifyRGB(bool bValue);
	virtual bool isOpacityModifyRGB(void);

	virtual void registerWithTouchDispatcher();

	virtual void onEnter();

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {}
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	enum TagType {
		None,
		FontTag,
		LinkTag,
		ColorTag,
		EmojTag,
		LineTag,
		SectionTag,
		TabTag,
		EndTag,
	};



private:
	const char *  parse(const char* string, bool recurse = false);
	const char * parseFont(const char* string);
	const char * parseLink(const char* string);
	const char * parseColor(const char* string);
	const char * parseEmoj(const char* string);
	const char * parseLine(const char* string);
	const char * parseSection(const char* string);
	const char * parseTab(const char* string);
	
	TagType searchTag(const char* string, int & pos);
	void createLabel(const char * string, int length = -1, bool defaultFont = true);
	int searchTruncatePos(const char *string, int pixelWidth);
	
	void measureString(const char* string, float &width, float &height);

	CCSize mSize;
	bool mParsed;

	std::string mString;
	RichLabelDelegate * mLinkDelegate;

	struct Link {
		LinkType linkType;
		int linkTarget;
		CCRect rect;

		Link() {}
		Link(CCRect _rect, LinkType _type, int target) 
		{
			rect = _rect;
			linkType = _type;
			linkTarget = target;
		}
		Link(const Link & link) 
		{
			rect = link.rect;
			linkType = link.linkType;
			linkTarget = link.linkTarget;
		}
	};

	std::vector<Link> mLinkMap;

	std::string mDefaultFontName;
	int mDefaultFontSize;
	int mDefaultColor;

	std::string mFontName;
	int mFontSize;
	int mColor;

	bool mNoneTagFound;
	bool mColorTagFound;
	bool mFontTagFound;
	bool mLinkTagFound;
	bool mEmojTagFound;

	float mLabelOffsetX;
	float mLabelOffsetY;

	Link mLink;

	CCSize mContentSize;

	bool mAllowAppendString;
	bool mEnableShadow;
};

#endif // __RICHLABEL_H__
