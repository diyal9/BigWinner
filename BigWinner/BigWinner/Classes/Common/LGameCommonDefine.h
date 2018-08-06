//
//  LGameCommonDefine.h
//  LolAllStars1_X
//
//  Created by Zhang Qiyun on 13-5-2.
//
//

#ifndef LolAllStars1_X_LGameCommonDefine_h
#define LolAllStars1_X_LGameCommonDefine_h

#include "cocos2d.h"
#include "CRequestMsgConstDedine.h"

#define P(x, y) ccp(CCDirector::sharedDirector()->getWinSize().width*x, CCDirector::sharedDirector()->getWinSize().height*y) //得到相对设备的坐标，（x ， y）的取值范围在0.0 ~1.0之间

#define G_INDEX (CCDirector::sharedDirector()->getWinSize().width/640.0)                 //因为是按宽 640的标准尺寸设计 所以缩放比例按这个来

#define G_SF(x) (x/CCDirector::sharedDirector()->getContentScaleFactor())              //适配各种设备分辨率的统一的尺寸大小(原始尺寸按640 960来)

#define NOTICE_UPDATEMONEY_NUM            "Update_Money"
#define NOTICE_UPDATEYUANBAO_NUM          "Update_Yuanbao"

enum SkillRangeType
{
    Range_self = 0,  //自己
    Range_monomer,   //单体
    Range_row,           //排
    Range_list,          //列
    Range_all,           //群体
    Range_rowlist        //十字
    
};

enum Fun_SelectStyle_Index
{
    SelStyle_FunSelLayer   = 0,    //功能大厅模式
    SelStyle_CardsLayer,           //卡册模式
    SelStyle_SkillsLayer,          //技能模式
    SelStyle_GuildLayer,           //工会模式
    SelStyle_BattleLayer,          //竞技模式
    SelStyle_FubenLayer,           //副本模式
    SelStyle_TiantiLayer           //天梯模式
    
};

enum CLScene_TypeIndex
{
    CLScene_TypeNull  =0,          //初始
    CLScene_TopLayer,              //Top页面
    CLScene_LoginGame ,            //游戏登陆界面
    CLScene_MainScene,             //游戏主界面
    CLScene_ShelfLayer,            //货架界面
    CLScene_PveLayer,              //闯关界面
    CLScene_PvpLayer               //对战界面
};

#define DLG_GENERAL_ZORDER        8   //通用UI Z坐标

// 新建一个继承于CCLayer的场景类,以后凡是挂到场景管理器下的场景节点类都需要继承此类
class CLSceneLayer : public cocos2d::CCLayer
{
public:
    CLSceneLayer(){m_nCTIndex = CLScene_TypeNull;}
    ~CLSceneLayer(){}
    
    CC_SYNTHESIZE(CLScene_TypeIndex, m_nCTIndex, nCTIndex);
};

#define NOTICE_UPDATEPOWER "UpdatePower"
#define NOTICE_UPDATEMONEY "UpdateMoney"
#define NOTICE_UPDATECARD  "UpdateCard"
#define NOTICE_UPDATEMONEY1 "UpdateMoney1"
#define NOTICE_UPDATECARD1 "UpdateCard1"

#endif



