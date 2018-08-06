//
//  UserInfo.cpp
//  BigWinner
//
//  Created by LunaBox on 13-10-13.
//
//

#include "UserInfo.h"
#include <map>
#include <string>
#include "CLTopicData.h"
#include <JsonDataManager.h>

using namespace std;

std::map<int, string> personnel;
typedef map<int, string> UDT_MAP_INT_CSTRING;
UDT_MAP_INT_CSTRING enumMap;

UserInfo::UserInfo()
{
    m_Gender = 0;
    m_Jetton = 0;
    m_Moneys = 0;
    m_RoleID = 0;
    m_UserNo = 0;
    m_Yinhua1 = 0;
    m_Yinhua2 = 0;
    m_nShelfNo = 0;
    m_nStoreNo = 0;
    
    m_sUsername = "无敌金刚";
    m_nMode = 0;
    m_nRoomNo  = 0;
    
}

UserInfo::~UserInfo()
{
    std::map<int, playerCardJueseInfo*>::iterator iter_b = m_vPlayerCardProInfo.begin();
    for(; iter_b != m_vPlayerCardProInfo.end(); iter_b++)
    {
        delete iter_b->second;
    }
}

bool UserInfo::init()
{
    return true;
}

//初始化用户信息
bool UserInfo::initUserInfo(Json::Value & value)
{
    m_Gender = value["Gender"].asIntModestyle();
    //add by yin (按照协议 钱->Jetton, 卡->Moneys)
//    m_Jetton = value["Jetton"].asIntModestyle();
//    m_Moneys = value["Moneys"].asIntModestyle();
    m_Jetton = value["Moneys"].asIntModestyle();
    m_Moneys = value["Jetton"].asIntModestyle();
    
    m_RoleID = value["RoleID"].asIntModestyle();
    m_UserNo = value["UserNo"].asIntModestyle();
    m_Yinhua1 = value["Yinhua1"].asIntModestyle();
    m_Yinhua2 = value["Yinhua2"].asIntModestyle();
    m_sUserID = value["userID"].asStringModeStyle();
    m_sUsername = value["Username"].asStringModeStyle();
    m_sSculptureID = value["SculptureID"].asStringModeStyle();
    m_sPhone =  value["Phone"].asStringModeStyle();
    m_sPwd =  value["password"].asStringModeStyle();

    
    //目前没有
//    m_shelfNo = value["Gender"].asIntModestyle();
//    m_storeNo = value["Gender"].asIntModestyle();
    
    
    return true;
}

bool UserInfo::initUserCardInfo(Json::Value & value)
{
    std::map<int, playerCardJueseInfo*>::iterator iter_b = m_vPlayerCardProInfo.begin();
    for(; iter_b != m_vPlayerCardProInfo.end(); iter_b++)
    {
        delete iter_b->second;
    }
    m_vPlayerCardProInfo.clear();

    //提卡
    for (int i = 0; i < value["card"].size(); i++)
    {
        int index = value["card"][i]["No"].asIntModestyle();
        if (index == 0)
           continue;
        CCLOG("name:%s",JsonDataManager::shardJDManager()->m_vCardJueseDaoju[index]->m_artClassName.c_str() );
        
        playerCardJueseInfo* realPItem = new playerCardJueseInfo();
        CardJueseDaoju* pItem = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[value["card"][i]["No"].asIntModestyle()];
        realPItem->m_cardJueseInfo = pItem;
        realPItem->number = value["card"][i]["Number"].asIntModestyle();
        m_vPlayerCardProInfo[value["card"][i]["No"].asIntModestyle()] = realPItem;
        CCLOG("number:%d  con:%s", m_vPlayerCardProInfo[value["card"][i]["No"].asIntModestyle()]->number,
              m_vPlayerCardProInfo[value["card"][i]["No"].asIntModestyle()]->m_cardJueseInfo->m_artClassName.c_str());
    }
    //道具
    for (int i = 0; i < value["playerprop"].size(); i++)
    {
        int index = value["playerprop"][i]["No"].asIntModestyle();
        if (index == 0)
            continue;
        playerCardJueseInfo* realPItem = new playerCardJueseInfo();
        CardJueseDaoju* pItem = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[value["playerprop"][i]["No"].asIntModestyle()];
        realPItem->m_cardJueseInfo = pItem;
        realPItem->number = value["playerprop"][i]["Number"].asIntModestyle();
        m_vPlayerCardProInfo[value["playerprop"][i]["No"].asIntModestyle()] = realPItem;
        CCLOG("number:%d  con:%s", m_vPlayerCardProInfo[value["playerprop"][i]["No"].asIntModestyle()]->number,
              m_vPlayerCardProInfo[value["playerprop"][i]["No"].asIntModestyle()]->m_cardJueseInfo->m_artClassName.c_str());

    }
    //角色
    for (int i = 0; i < value["playerrole"].size(); i++)
    {
        int index = value["playerrole"][i]["No"].asIntModestyle();
        if (index == 0)
            continue;
        playerCardJueseInfo* realPItem = new playerCardJueseInfo();
        CardJueseDaoju* pItem = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[value["playerrole"][i]["No"].asIntModestyle()];
        realPItem->m_cardJueseInfo = pItem;
        realPItem->number = value["playerrole"][i]["Number"].asIntModestyle();
        m_vPlayerCardProInfo[value["playerrole"][i]["No"].asIntModestyle()] = realPItem;
        CCLOG("number:%d  con:%s", m_vPlayerCardProInfo[value["playerrole"][i]["No"].asIntModestyle()]->number,
              m_vPlayerCardProInfo[value["playerrole"][i]["No"].asIntModestyle()]->m_cardJueseInfo->m_artClassName.c_str());

    }
    
    
    return true;
}

void UserInfo::AddItemToList(playerCardJueseInfo* pItem)
{
    m_vPlayerCardProInfo[pItem->m_cardJueseInfo->m_artNo] = pItem;
}

playerCardJueseInfo* UserInfo::GetPropFromId(int nPropId)
{
    std::map<int, playerCardJueseInfo*>::iterator iter = m_vPlayerCardProInfo.find(nPropId);
    if( iter != m_vPlayerCardProInfo.end())
        return iter->second;
    else
        return NULL;
}

std::vector<playerCardJueseInfo*> UserInfo::GetPveProps()
{
    std::vector<playerCardJueseInfo*> vList;
    
    int nCount = 0;
    std::map<int, playerCardJueseInfo*>::iterator iter = m_vPlayerCardProInfo.begin();
    for(; iter != m_vPlayerCardProInfo.end(); iter++)
    {
        playerCardJueseInfo* pInfo = iter->second;
        if(pInfo->m_cardJueseInfo->m_artPropType == 1 || pInfo->m_cardJueseInfo->m_artPropType == 3)
        {
            vList.push_back(pInfo);
            nCount++;
            if(nCount >= 2)
                break;
        }
    }
    
    return vList;
}

void UserInfo::RemovePropFromID(int nPropID)
{
    std::map<int, playerCardJueseInfo*>::iterator iter = m_vPlayerCardProInfo.find(nPropID);
    
    if(iter != m_vPlayerCardProInfo.end())
    {
        delete iter->second;
        m_vPlayerCardProInfo.erase(iter);
    }
}



