//
//  CLTopicData.h
//  BigWinner
//
//  Created by ZhangQiyun on 13-10-18.
//
//

#ifndef __BigWinner__CLTopicData__
#define __BigWinner__CLTopicData__

#include "cocos2d.h"
#include "JsonParser.h"
USING_NS_CC;

class CLTopicData
{
public:
    CLTopicData();
    CLTopicData(Json::Value& jv);
    ~CLTopicData();
    
    CC_SYNTHESIZE(std::string, m_sTypeId, sTypeId);               //题目类别ID
    CC_SYNTHESIZE(int, m_nDifficulty, nDifficulty);               //题目星级(难度)
    CC_SYNTHESIZE(int, m_nTopicId, nTopicId);                     //题目ID
    CC_SYNTHESIZE(std::string, m_sTopicDesc, sTopicDesc);         //题目内容
    
    std::vector<std::string*> GetAnswerList(){return m_vTopicAnswer; }  //外部得到题目的答案列表
    
private:
    std::vector<std::string*> m_vTopicAnswer;                     //题目答案列表
    
};


//卡牌 角色 道具 数据类
class CardJueseDaoju
{
public:
    CardJueseDaoju();
    CardJueseDaoju(Json::Value& jv);
    ~CardJueseDaoju();
public:
    int m_artNo;                                                //题卡ID
    int m_artID;
    int m_artType;                                              //题卡类型  1 题卡  2 道具 3 角色
    int m_artQStarLevel;                                          //星级
    std::string m_artCaption;                                   //
    std::string m_artClassName;                                 //名字
    std::string m_artNote;                                      //描述
    //20131105 modify by yin
    int m_artIconId;                                            //物品图片ID
    int m_BuyLimit;                                             //物品购买限制
    int m_artPropType;                                             //物品类型属性
};

//印花类
class YinHuaInfo
{
public:
    YinHuaInfo();
    YinHuaInfo(Json::Value& jv);
    ~YinHuaInfo();
public:
    int m_SYSNo;                                                //印花ID
    int m_YH1;                                                  //金印花
    int m_YH2;                                              //银银花
    int m_Image;                                          //图片id
    std::string m_Name;                                   //印花名字
    std::string m_Description;                                 //描述
    std::string m_BuyName;                                      //卖家名字
};

//货架类
class ShelfInfo
{
public:
    ShelfInfo();
    ShelfInfo(Json::Value& jv);
    ~ShelfInfo();
public:
    CC_SYNTHESIZE(int, m_nShelfNo, nShelfNo);   //货架ID
    CC_SYNTHESIZE(std::string, m_nShelfCaption, nShelfCaption);   //货架名称
    CC_SYNTHESIZE(int, m_nClassNo, nClassNo);   //货架类型序号
//    int m_SYS_No;   //货架ID
//    std::string m_SYS_Caption;  //货架名称
};

//对战房间信息类
class PvpRoomInfo
{
public:
    PvpRoomInfo();
    ~PvpRoomInfo();
    
    CC_SYNTHESIZE(YinHuaInfo*, m_pYinhuaInfo, pYinhuaInfo);   //印花模版数据
    CC_SYNTHESIZE(int, m_nReGYinhuaNum, nReGYinhuaNum);       //剩余金印花的数量
    CC_SYNTHESIZE(int, m_nPvpPlayerNum, nPvpPlayerNum);       //房间对战人数
};


//印花信息类
class PrizeInfo
{
public:
    PrizeInfo();
    ~PrizeInfo();
    
    CC_SYNTHESIZE(int, m_nStampNo, nStampNo);                           //印花商品No
    CC_SYNTHESIZE(int, m_nStampMark, nStampMark);                       //印花商品标识；0：普通；1：最新；2：最热"
    CC_SYNTHESIZE(int, m_nHaveGoldStampNum, nHaveGoldStampNum);         //当前拥有的金印花数
    CC_SYNTHESIZE(int, m_nHaveSilverStampNum, nHaveSilverStampNum);     //当前拥有的金印花数
    CC_SYNTHESIZE(int, m_nStockNum, nStockNum);     //当前拥有的金印花数
};

//备战设置 题卡
class UcardInfo
{
public:
    UcardInfo();
    ~UcardInfo();
    
    CC_SYNTHESIZE(int, m_nUcardNo, nUcardNo);                           //题卡No
    CC_SYNTHESIZE(std::string, m_nUcardNumber, nUcardNumber);           //拥有题卡数量
    CC_SYNTHESIZE(int, m_nUcardStatus, nUcardStatus);           //拥有题卡数量
};

//备战设置 道具
class UpropInfo
{
public:
    UpropInfo();
    ~UpropInfo();
    
    CC_SYNTHESIZE(int, m_nUpropNo, nUpropNo);                           //道具No
    CC_SYNTHESIZE(std::string, m_nUpropNumber, nUpropNumber);                   //拥有道具数量
};

//备战设置 角色
class UroleInfo
{
public:
    UroleInfo();
    ~UroleInfo();
    
    CC_SYNTHESIZE(int, m_nUroleNo, nUroleNo);                           //角色No
    CC_SYNTHESIZE(std::string, m_nUroleNumber, nUroleNumber);                   //拥有角色数量
};

//备战设置 选择的对战道具信息
class PK1BPMessageInfo
{
public:
    PK1BPMessageInfo();
    ~PK1BPMessageInfo();
    
    CC_SYNTHESIZE(int, m_nRoleNo, nRoleNo);                           //选择角色No  0未选择
    CC_SYNTHESIZE(int, m_nCard1No, nCard1No);                         //选择题卡1No 0未选择
    CC_SYNTHESIZE(int, m_nCard2No, nCard2No);                         //选择题卡2No 0未选择
    CC_SYNTHESIZE(int, m_nCard3No, nCard3No);                         //选择题卡3No 0未选择
    CC_SYNTHESIZE(int, m_nProp1No, nProp1No);                         //选择道具1No 0未选择
    CC_SYNTHESIZE(int, m_nProp2No, nProp2No);                         //选择道具2No 0未选择

    
};

//备战设置 选择的对战道具信息
class UcardStatus
{
public:
    UcardStatus();
    ~UcardStatus();
    
    CC_SYNTHESIZE(int, m_nCard1No, nCard1No);                         //选择题卡1No 0未选择
    CC_SYNTHESIZE(int, m_nCard2No, nCard2No);                         //选择题卡2No 0未选择
    CC_SYNTHESIZE(int, m_nCard3No, nCard3No);                         //选择题卡3No 0未选择
    
};

//备战设置 选择的对战道具信息
class UpropStatus
{
public:
    UpropStatus();
    ~UpropStatus();
    
    CC_SYNTHESIZE(int, m_nProp1No, nProp1No);                         //选择道具1No 0未选择
    CC_SYNTHESIZE(int, m_nProp2No, nProp2No);                         //选择道具2No 0未选择
    
};

//备战设置 选择的对战道具信息
class UroleStatus
{
public:
    UroleStatus();
    ~UroleStatus();
    
    CC_SYNTHESIZE(int, m_nRoleNo, nRoleNo);                           //选择角色No  0未选择
    
};

//成就列表持久对象类
class AchievementBean
{
public:
    AchievementBean();
    ~AchievementBean();
    
    AchievementBean(Json::Value& jv);
    
//    CC_SYNTHESIZE(int, m_pAchiNo, pAchiNo);                        //成就ID
//    CC_SYNTHESIZE(std::string, m_pAchiName, pAchiName);            //成就名称
//    CC_SYNTHESIZE(std::string, m_pAchiType, pAchiType);            //成就类别No
//    CC_SYNTHESIZE(std::string, m_pAchiClass, pAchiClass);          //成就类型
//    CC_SYNTHESIZE(int, m_pAchiLevel, pAchiLevel);                  //成就等级
//    CC_SYNTHESIZE(int, m_pAchiValue, pAchiValue);                  //成就条件
//    CC_SYNTHESIZE(std::string, m_pAchiPrizeType, pAchiPrizeType);  //奖励类型
//    CC_SYNTHESIZE(int, m_pAchiPrizeNum, pAchiPrizeNum);            //奖励数量
//    CC_SYNTHESIZE(std::string, m_pAchiNote, pAchiNote);            //成就说明
    
    int m_pAchiNo;                        //成就ID
    std::string m_pAchiName;            //成就名称
    std::string m_pAchiType;            //成就类别No
    std::string m_pAchiClass;          //成就类型
    int m_pAchiLevel;                  //成就等级
    int m_pAchiValue;                  //成就条件
    std::string m_pAchiPrizeType;  //奖励类型
    int m_pAchiPrizeNum;            //奖励数量
    std::string m_pAchiNote;            //成就说明

};

//个人背包信息 包括数量
class playerCardJueseInfo
{
public:
    playerCardJueseInfo(){
        m_cardJueseInfo = NULL;
        number = 0;
    }
    ~playerCardJueseInfo(){
        
    }
public:
    CardJueseDaoju *m_cardJueseInfo;
    int             number;
};

#endif /* defined(__BigWinner__CLTopicData__) */
