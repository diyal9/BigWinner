#ifndef __test__DlgLayerBtnA__
#define __test__DlgLayerBtnA__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

/*
 创建有一个按钮的提示框
 */
class Button;

class DlgLayerBtnA : public CCLayerColor

{
public:
    DlgLayerBtnA(){
        pButton = NULL;
    };
    ~DlgLayerBtnA(){};
    
    //创建 一个具有一个按钮和一个提示信息的弹出层 。参数：按钮名称,提示信息,提示信息的文字类型。
    static DlgLayerBtnA* createBtn(std::string btnName,std::string word);
    bool init(std::string btnName ,std::string word);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    Button *pButton;
    void QueRen(CCNode* r);
};


#endif /* defined(__test__PopLayerBtn1__) */
