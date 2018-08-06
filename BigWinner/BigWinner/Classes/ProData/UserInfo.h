//
//  UserInfo.h
//  BigWinner
//
//  Created by LunaBox on 13-10-13.
//
//

#ifndef __BigWinner__UserInfo__
#define __BigWinner__UserInfo__

#include "cocos2d.h"
#include "JsonParser.h"

USING_NS_CC;
using namespace Json;

class CardJueseDaoju;
class playerCardJueseInfo;

//用户基本信息类
class UserInfo:public cocos2d::CCObject
{
    //用户基本信息
    CC_SYNTHESIZE(bool, m_Gender, Gender);              //性别
    CC_SYNTHESIZE(int,  m_Jetton,  Jetton);             //购物卡
    CC_SYNTHESIZE(int, m_Moneys, Moneys);               //钞票
    CC_SYNTHESIZE(int, m_RoleID, RoleID);              //角色ID
    CC_SYNTHESIZE(int, m_UserNo, UserNo);              //用户序号
    CC_SYNTHESIZE(int, m_Yinhua1, Yinhua1);            //银印花数量
    CC_SYNTHESIZE(int, m_Yinhua2, Yinhua2);            //金印花数量
    CC_SYNTHESIZE(int, m_nShelfNo, nShelfNo);            //上次断线前所在货架号
    CC_SYNTHESIZE(int, m_nStoreNo, nStoreNo);            //上次断线前所在商铺号
    CC_SYNTHESIZE(std::string, m_sUsername, sUsername);        //用户昵称
    CC_SYNTHESIZE(std::string, m_sSculptureID, sSculptureID);  //头像
    CC_SYNTHESIZE(std::string, m_sUserID, sUserID);            //用户id
    CC_SYNTHESIZE(std::string, m_sPhone,  sPhone );            //phone
    CC_SYNTHESIZE(std::string, m_sPwd,  sPwd );            //pwd

    
    CC_SYNTHESIZE(int, m_nMode, nMode);                //纪录玩家上次状态 1.闯关 2. 对战
    CC_SYNTHESIZE(int, m_nRoomNo, nRoomNo);            //当前所在的 1.货架号  2. 房间号
    
public:
    
    std::map<int, playerCardJueseInfo*> m_vPlayerCardProInfo;   //玩家拥有的道具、角色、提卡
    
public:
    UserInfo();
    virtual ~UserInfo();
    
    CREATE_FUNC(UserInfo);
    bool init();
    bool initUserInfo(Json::Value & value);
    bool initUserCardInfo(Json::Value & value);
    playerCardJueseInfo* GetPropFromId(int nPropId);
    std::vector<playerCardJueseInfo*> GetPveProps();
    void RemovePropFromID(int nPropID);
    void AddItemToList(playerCardJueseInfo* pItem);
    
};

#endif /* defined(__BigWinner__UserInfo__) */
