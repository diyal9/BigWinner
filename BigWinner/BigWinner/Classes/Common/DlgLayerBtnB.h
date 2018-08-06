#ifndef __test__DlgLayerBtnB__
#define __test__DlgLayerBtnB__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;


class Button;

/*
 创建有两个按钮的提示框
 */
class DlgLayerBtnB : public cocos2d::CCLayer

{
public:
    DlgLayerBtnB()
    {
        pBtnLeft = NULL;
        pBtnRight = NULL;
    };
    ~DlgLayerBtnB(){};
    
    //创建 一个具有两个按钮和一个提示信息的弹出层 。参数：左边按钮名称，右边按钮名称，提示信息，提示信息的文字类型。
    static DlgLayerBtnB* createBtn(std::string btnNameLeft,std::string btnNameRight,std::string word);
    bool init(std::string btnNameLeft,std::string btnNameRight,std::string word);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    Button *pBtnLeft;
    Button *pBtnRight;
    
    void leftCancle(CCNode* r);
    
};


#endif /* defined(__test__PopLayerBtn2__) */
