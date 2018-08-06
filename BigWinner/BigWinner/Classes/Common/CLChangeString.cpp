//
//  CLChangeString.cpp
//  LolAllStars1_X
//
//  Created by Zhang Qiyun on 13-5-3.
//
//

#include "CLChangeString.h"
#include "JsonParser.h"

static CLChangeString* g_pChangeString = NULL;

CLChangeString::CLChangeString()
{
    
}


CLChangeString::~CLChangeString()
{
    
}



CLChangeString* CLChangeString::sharedChangeString()
{
    if(!g_pChangeString)
    {
        g_pChangeString = new CLChangeString();
        g_pChangeString->init("Data/JsonData/ChangeString.json");
    }
    
    return g_pChangeString;
}


bool CLChangeString::init(const char* fileName)
{
    Json::Value  pv;
    Json::Reader jsReader;
    std::string srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName);
    
    CCString* pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", fileName);
        return false;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);

    std::vector<std::string> pVec =  pv.getMemberNames();
    
    for(int i=0; i < pVec.size(); i++)
    {
        m_vStringList[pVec[i]] = pv[pVec[i]].asString();
    }
    
    return true;
    
}

std::string CLChangeString::GetChangeString(const char* keyName)
{
    std::string sValue = "nofound";
    if(m_vStringList.size() == 0 || m_vStringList.find(keyName) == m_vStringList.end())
        return sValue;
    
    sValue = m_vStringList[keyName];
    return sValue;
}

std::string CLChangeString::GetTTFName(int nIndex)
{
    std::string sName = "nofound";
    
    char sIndex[20] = {0};
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    sprintf(sIndex, "%d_ios", nIndex);
#else
    sprintf(sIndex, "%d_android", nIndex);
#endif
    if( GetChangeString(sIndex).compare(sName) == 0)
    {
        return sName;
    }
    else
    {
        sName = "Data/strings/" + GetChangeString(sIndex);
        return sName;
    }
}




