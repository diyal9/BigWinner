//
//  CLTopicData.cpp
//  BigWinner
//
//  Created by ZhangQiyun on 13-10-18.
//
//

#include "CLTopicData.h"


CLTopicData::CLTopicData()
{
    m_nDifficulty = 0;
    m_nTopicId = 0;
    
}

CLTopicData::CLTopicData(Json::Value& jv)
{
    CLTopicData();
    
    m_nTopicId = jv["SYS_No"].asIntModestyle();
    m_nDifficulty = jv["QST_Level"].asIntModestyle();

    m_sTypeId = jv["FIELD4"].asStringModeStyle();
    m_sTopicDesc = jv["SYS_Text"].asStringModeStyle();
    
    for(int i=0; i<4; i++)
    {
        std::string sTitle;
        switch (i)
        {
            case 0:
            {
                sTitle = "QST_A";
            }
                break;
            case 1:
            {
                sTitle = "QST_B";
            }
                break;
            case 2:
            {
                sTitle = "QST_C";
            }
                break;
            case 3:
            {
                sTitle = "QST_D";
            }
                break;
                
            default:
                break;
        }
        std::string s = jv[sTitle.c_str()].asStringModeStyle();
        std::string *pAS = new std::string(s);
        m_vTopicAnswer.push_back(pAS);
    }
    
}

CLTopicData::~CLTopicData()
{
    for(int i=0; i<m_vTopicAnswer.size(); i++)
    {
        delete m_vTopicAnswer[i];
    }
}

#pragma 题卡 道具 角色 属性类 gaoyuan

CardJueseDaoju::CardJueseDaoju()
{
    m_artNo = 0;
    m_artID = 0;
    m_artType = 0;
    m_artQStarLevel = 0;
    
    //20131105 modify by yin
    m_artIconId = 0;
    m_BuyLimit = 0;
    m_artPropType=0;
    
}


CardJueseDaoju::CardJueseDaoju(Json::Value& jv)
{
    
    CCLOG("gao:%s", jv.toStyledString().c_str());
    
    CardJueseDaoju();
    
    m_artNo = jv["ArtNo"].asIntModestyle();
    m_artType = jv["ArtType"].asIntModestyle();
    //20131029 modify by yin @ as json file been modify  start
//    m_artQStarLevel = jv["ArtQStarLevel"].asIntModestyle();
    m_artQStarLevel = jv["ArtClassValue"].asIntModestyle();
    //20131029 modify by yin @ as json file been modify  end
    m_artCaption = jv["ArtCaption"].asStringModeStyle();
    m_artClassName = jv["ArtClassName"].asStringModeStyle();
    m_artNote = jv["ArtNote"].asStringModeStyle();
    
    //20131105 modify by yin
    m_artIconId = jv["ArtIconId"].asIntModestyle();
    m_BuyLimit = jv["ArtBuyLimit"].asIntModestyle();
    
    //20131128 by wang
    m_artPropType = jv["ArtPropType"].asIntModestyle();
    
//    CCLOG("yuan:%s", m_artCaption.c_str());
}

CardJueseDaoju::~CardJueseDaoju()
{
    
}



#pragma 印花类 gaoyuan

YinHuaInfo::YinHuaInfo()
{
       
    m_SYSNo = 0;                                          //印花ID
    m_YH1 = 0;                                                 //金印花
    m_YH2 = 0;                                              //银银花
    m_Image = 0;
    
}


YinHuaInfo::YinHuaInfo(Json::Value& jv)
{
    
    CCLOG("gao:%s", jv.toStyledString().c_str());
    
    YinHuaInfo();
    
    m_SYSNo = jv["SYSNo"].asIntModestyle();
    m_YH1 = jv["YH1"].asIntModestyle();
    m_YH2 = jv["YH2"].asIntModestyle();
    m_Image = jv["Image"].asIntModestyle();
    m_Name = jv["Name"].asStringModeStyle();
    m_Description = jv["Description"].asStringModeStyle();
    m_BuyName = jv["BuyName"].asStringModeStyle();
}

YinHuaInfo::~YinHuaInfo()
{
    
}

#pragma 货架类 wangguolong

ShelfInfo::ShelfInfo()
{
    
    m_nShelfNo = 0; //货架ID
    m_nClassNo = 0;

}


ShelfInfo::ShelfInfo(Json::Value& jv)
{
    
    CCLOG("wangguolong:%s", jv.toStyledString().c_str());
    
    ShelfInfo();
    
    m_nShelfNo = jv["SYS_No"].asIntModestyle();
    m_nShelfCaption = jv["SYS_Text"].asStringModeStyle();//货架名称
    m_nClassNo = jv["SYS_ClassNo"].asIntModestyle();//货架类型序号

}

ShelfInfo::~ShelfInfo()
{
    
}


PvpRoomInfo::PvpRoomInfo()
{
    m_nPvpPlayerNum = 0;
    m_nReGYinhuaNum = 0;
    m_pYinhuaInfo = NULL;
}

PvpRoomInfo::~PvpRoomInfo()
{
    
}


PrizeInfo::PrizeInfo()
{
    m_nStampNo = 0;             //印花商品No
    m_nStampMark = 0;           //印花商品标识；0：普通；1：最新；2：最热"
    m_nHaveGoldStampNum = 0;    //当前拥有的金印花数
    m_nHaveSilverStampNum = 0;  //当前拥有的金印花数
    m_nStockNum = 0 ;           //当前库存数量
}
PrizeInfo::~PrizeInfo()
{

}

//wangguolong
//备战设置 题卡
UcardInfo::UcardInfo()
{
    m_nUcardNo = 0;         //题卡No
    m_nUcardStatus = 0;     //题卡状态
//    m_nUcardNumber = 0;     //拥有题卡数量
}
UcardInfo::~UcardInfo()
{
    
}

//备战设置 道具
UpropInfo::UpropInfo()
{
    m_nUpropNo = 0;         //角色No
//    m_nUpropNumber = 0;     //拥有角色数量
}
UpropInfo::~UpropInfo()
{

}

//备战设置 角色
UroleInfo::UroleInfo()
{
    m_nUroleNo = 0;         //角色No
//    m_nUroleNumber = 0;       //拥有角色数量
}
UroleInfo::~UroleInfo()
{

}

//备战设置 选择的对战道具信息
PK1BPMessageInfo::PK1BPMessageInfo()
{
    m_nRoleNo=0;                           //选择角色No  0未选择
    m_nCard1No=0;                         //选择题卡1No 0未选择
    m_nCard2No=0;                        //选择题卡2No 0未选择
    m_nCard3No=0;                         //选择题卡3No 0未选择
    m_nProp1No=0;                         //选择道具1No 0未选择
    m_nProp2No=0;                         //选择道具2No 0未选择

}
PK1BPMessageInfo::~PK1BPMessageInfo()
{

}



//备战设置 选择的对战道具信息
UcardStatus::UcardStatus()
{
    m_nCard1No = 0;                         //选择题卡1No 0未选择
    m_nCard2No = 0;                         //选择题卡2No 0未选择
    m_nCard3No = 0;                         //选择题卡3No 0未选择
}
UcardStatus::~UcardStatus()
{

}


//备战设置 选择的对战道具信息
UpropStatus::UpropStatus()
{
    m_nProp1No = 0;                         //选择道具1No 0未选择
    m_nProp2No = 0;                         //选择道具2No 0未选择
}
UpropStatus::~UpropStatus()
{
    
}


//备战设置 选择的对战道具信息

UroleStatus::UroleStatus()
{
    m_nRoleNo = 0;                           //选择角色No  0未选择
}
UroleStatus::~UroleStatus()
{

}


#pragma 成就类 yin

AchievementBean::AchievementBean()
{

    m_pAchiNo = 0;
    m_pAchiLevel = 0;
    m_pAchiValue = 0;
    m_pAchiPrizeNum = 0;
}


AchievementBean::AchievementBean(Json::Value& jv)
{
    
    CCLOG("gao:%s", jv.toStyledString().c_str());
    
    AchievementBean();
    
    m_pAchiNo = jv["ACH_No"].asIntModestyle();//成就ID
    m_pAchiName = jv["ACH_Name"].asStringModeStyle();//成就名称
    m_pAchiType = jv["ACH_TypeNo"].asStringModeStyle();//成就类别No
    m_pAchiClass = jv["ACH_Class"].asStringModeStyle();//成就类型
    m_pAchiLevel = jv["ACH_LevelClass"].asIntModestyle();//成就等级
    m_pAchiValue = jv["ACH_Value"].asIntModestyle();//成就条件
    m_pAchiPrizeType = jv["ACH_PrizeType"].asStringModeStyle();//奖励类型
    m_pAchiPrizeNum = jv["ACH_PrizeNum"].asIntModestyle();//奖励数量
    m_pAchiNote = jv["ACH_Note"].asStringModeStyle();//成就说明
    
}

AchievementBean::~AchievementBean()
{
    
}



