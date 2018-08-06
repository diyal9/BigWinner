//
//  JsonDataManager.cpp
//  Yy_mobileGame3
//
//  Created by Zhang Qiyun on 13-6-17.
//
//
#include "JsonDataManager.h"

static JsonDataManager* g_pJDManager = NULL;

JsonDataManager::JsonDataManager()
{
    m_bAccountHasAvatar = false;
    m_nLoginPort = 0;
}

JsonDataManager::~JsonDataManager()
{
    //释放动态创建的场景预加载图片列表信息
    std::map<std::string, std::vector<std::string>*>::iterator iter_beg = m_vScenesImageList.begin();
    for(; iter_beg != m_vScenesImageList.end(); iter_beg++)
    {
        std::vector<std::string>* pvs = iter_beg->second;
        delete pvs;
    }
    
}

JsonDataManager* JsonDataManager::shardJDManager()
{
    if(!g_pJDManager)
    {
        g_pJDManager = new JsonDataManager();
    }
    return g_pJDManager;
}

void JsonDataManager::init()
{
    initUrlData();
    //initChangeStringData();
    initBaseData();
    initLoadItemData();
    initCommenDefineData();
    initLoadNickNameData();
    initShieldWordsList();
    initTopicData();
    
    initCardJueSeDaojuData();
    initYinHuaData();
    
    //wangguolong
    initShelfData();
    
    initAchievementInfo();
    
}

void JsonDataManager::initCommenDefineData()    // 初始化通用文件信息
{
    Json::Value pv;
    Json::Reader jsReader;
    std::string srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/JsonData/CommenDefine.json");
    
    CCString* pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/JsonData/CommenDefine.json");
        return ;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);
    
    m_sCurVersion = pv["CurVersion"].asString();
}



// 初始化Url信息
void JsonDataManager::initUrlData()
{
    Json::Value pv;
    Json::Reader jsReader;
    std::string srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/JsonData/Url.json");
    
    CCString* pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/JsonData/Url.json");
        return ;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);
    
    m_sLoginUrl = pv["IPAddress"].asString();
    m_nLoginPort = pv["Port"].asIntModestyle();
    
}

// 初始化多语言文字信息
void JsonDataManager::initChangeStringData()
{
    Json::Value pv;
    Json::Reader jsReader;
    std::string srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/JsonData/ChangeString.json");
    
    CCString* pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/JsonData/ChangeString.json");
        return;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);
    
    std::vector<std::string> pVec =  pv.getMemberNames();
    
    for(int i=0; i < pVec.size(); i++)
    {
        m_vChangeStringList[pVec[i]] = pv[pVec[i]].asString();
    }
    
}

void JsonDataManager::initLoadNickNameData()
{
    Json::Value pv;
    Json::Reader jsReader;
    std::string srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/JsonData/NickNameList1st.json");
    
    CCString* pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/JsonData/NickNameList1st.json");
        return;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);
    
    Json::Value pv1(Json::arrayValue);
    pv1 = pv["Name"];
    
    for(int i=0; i < pv1.size(); i++)
    {
        m_vNickNames1st.push_back(pv1[i].asString());
    }
    
    pv.clear();
    pv1.clear();
    
    srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/JsonData/NickNameList2ed.json");
    
    pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/JsonData/NickNameList2ed.json");
        return;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);

    pv1 = pv["Name"];
    
    for(int i=0; i < pv1.size(); i++)
    {
        m_vNickNames2ed.push_back(pv1[i].asString());
    }
    
}

void JsonDataManager::initShieldWordsList()
{
    Json::Value pv;
    Json::Reader jsReader;
    std::string srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/JsonData/ShieldWords.json");
    
    CCString* pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/JsonData/ShieldWords.json");
        return;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);
    
    Json::Value pv1(Json::arrayValue);
    pv1 = pv["ShieldWords"];
    
    for(int i=0; i < pv1.size(); i++)
    {
        m_vShieldWordsList.push_back(pv1[i].asString());
    }
}

//初始化基础数据
void JsonDataManager::initBaseData()
{
    Json::Value pv;
    Json::Reader jsReader;
}

// 根据网络通讯Id得到Url全地址
std::string JsonDataManager::GetUrlFromUrlList(int CmdUrl_Index)
{
    char keyName[20] = {0};
    sprintf(keyName, "%d", CmdUrl_Index);
    std::string sValue = "nofoundurl";
    if(m_vUrlBranchList.size() == 0 || m_vUrlBranchList.find(keyName) == m_vUrlBranchList.end())
        return sValue;
    
    return sValue;
    
}

std::string JsonDataManager::GetCheckVerUrl(int CmdUrl_Index)
{
    char keyName[20] = {0};
    sprintf(keyName, "%d", CmdUrl_Index);
    std::string sValue = "nofoundurl";
    if(m_vUrlBranchList.size() == 0 || m_vUrlBranchList.find(keyName) == m_vUrlBranchList.end())
        return sValue;
    
    sValue = m_sLoginUrl + m_vUrlBranchList[keyName];
    return sValue;
}

void JsonDataManager::initLoadItemData()                // 初始化异步加载的图片数据
{
    Json::Value pv;
    Json::Reader jsReader;
    std::string srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/ImageData/NetScene.json");
    
    CCString* pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/ImageData/NetScene.json");
        return;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);
    std::vector<std::string> pVec =  pv.getMemberNames();
    
    for(int i=0; i < pVec.size(); i++)
    {
        m_vIamgeSceneFileList[pVec[i]] = pv[pVec[i]].asString();
        
        Json::Value pv1;
        Json::Reader jsReader1;
        std::string srcPath1 = CCFileUtils::sharedFileUtils()->fullPathForFilename(pv[pVec[i]].asString().c_str());
        
        CCString* pcstr1 = CCString::createWithContentsOfFile(srcPath1.c_str());
        if(pcstr1 == NULL)
        {
            CCLOG("This file %s is not exist!!!", pv[pVec[i]].asString().c_str());
            continue;
        }
        jsReader1.parse(pcstr1->getCString(), pv1, false);
        Json::Value pv2(Json::arrayValue);
        pv2 = pv1["data"];
        
        std::vector<std::string>* pVImageList = new std::vector<std::string>();
        for(int j=0; j < pv2.size(); j++)
        {
            pVImageList->push_back(pv2[j].asString());
        }
        m_vScenesImageList[pVec[i]] = pVImageList;
    }
    
}

std::vector<std::string>* JsonDataManager::GetSceneImagesArray(int nNetMsgId)
{
    char sNetMsgId[20] = {0};
    sprintf(sNetMsgId, "%d", nNetMsgId);
    
    if(m_vScenesImageList.find(sNetMsgId) != m_vScenesImageList.end())
        return m_vScenesImageList[sNetMsgId];
    else
        return NULL;
}

//随机从昵称库 获取昵称信息
std::string JsonDataManager::GetNickNameFromRandom()
{
    std::string sNickName = "";
    srand(time(NULL));
    
    if(m_vNickNames1st.size() != 0)
    {
        sNickName += m_vNickNames1st[rand()%m_vNickNames1st.size()];
    }
    
    if(m_vNickNames2ed.size() != 0)
    {
        sNickName += m_vNickNames2ed[rand()%m_vNickNames2ed.size()];
    }
    
    return sNickName;
    
}

bool JsonDataManager::CheckHasShieldWord(std::string& sCheckWord)
{
    for(int i=0; i< m_vShieldWordsList.size(); i++)
    {
        if(sCheckWord.find(m_vShieldWordsList[i]) != sCheckWord.npos)
        {
            return true;
        }
    }
    
    return false;
}

//初始化印花信息
void JsonDataManager::initYinHuaData()
{
    Json::Value pv;
    Json::Reader jsReader;
    std::string srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/CardData/YinHuaData.json");
    
    CCString* pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/CardData/YinHuaData.json");
        return;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);
    
    CCLOG("GAOYUAN-cardJson:%s", pv.toStyledString().c_str());
    
    for(int i=0; i < pv["data"].size(); i++)
    {
        YinHuaInfo* pItem = new YinHuaInfo(pv["data"][i]);
        m_yinHua[pv["data"][i]["SYSNo"].asIntModestyle()] = pItem;
    }

}

//wangguolong
//初始化货架信息
void JsonDataManager::initShelfData()
{
    Json::Value pv;
    Json::Reader jsReader;
    std::string srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/CardData/ShelfData.json");
    
    CCString* pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/CardData/ShelfData.json");
        return;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);
    
    for(int i=0; i < pv["data"].size(); i++)
    {
        ShelfInfo* pItem = new ShelfInfo(pv["data"][i]);
        m_Shelf[pv["data"][i]["SYS_No"].asIntModestyle()] = pItem;
    }
    
}

//初始化背包、角色、提卡、道具等信息
void JsonDataManager::initCardJueSeDaojuData()
{
    
    Json::Value pv;
    Json::Reader jsReader;
    std::string srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/CardData/ArticleData.json");
    
    CCString* pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/CardData/ArticleData.json");
        return;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);
    
//    CCLOG("GAOYUAN-cardJson:%s", pv.toStyledString().c_str());
    
    for(int i=0; i < pv["data"].size(); i++)
    {
        CardJueseDaoju* pItem = new CardJueseDaoju(pv["data"][i]);
        
        m_vCardJueseDaoju[pv["data"][i]["ArtNo"].asIntModestyle()] = pItem;
        
        CCLOG("id:%d name:%s",pv["data"][i]["ArtNo"].asIntModestyle(),m_vCardJueseDaoju[pv["data"][i]["ArtNo"].asIntModestyle()]->m_artClassName.c_str() );
//        CCLOG("size:%s",m_vCardJueseDaoju[pv["data"][i]["ArtNo"].asIntModestyle()]->m_artCaption.c_str());
    }
    
}

//获得 音乐、音效 bgMusic bgEfect
////var/mobile/Applications/831A6276-1483-4CF2-8D5C-4CA3E3F9D959/BigWinner.app/Data/JsonData/setSound.json
////var/mobile/Applications/831A6276-1483-4CF2-8D5C-4CA3E3F9D959/BigWinner.app/Data/JsonData/setSound.json
void JsonDataManager::GetMusicInfo(Json::Value &dataInfo)
{
    
    
    std::string sPath = CCFileUtils::sharedFileUtils()->getWritablePath() + "setSound.json";
    FILE* pFile = fopen(sPath.c_str(), "rb");
    if(!pFile)                //说明这个文件并不存在 那么返回false
    {
        dataInfo["bgMusic"] = 50;
        dataInfo["bgEfect"] =  50;
        return;
    }
    else
    {
        Json::Reader jsReader;
        CCString* pcstr = CCString::createWithContentsOfFile(sPath.c_str());
        if(pcstr == NULL)
        {
            CCLOG("®This file %s is not exist!!!", "Data/CardData/setSound.json");
            return;
        }
        
        jsReader.parse(pcstr->getCString(), dataInfo, false);
    }
    
}
//设置音乐音效
void JsonDataManager::SetMusicInfo(Json::Value &dataInfo)
{
    
    std::string sPath = CCFileUtils::sharedFileUtils()->getWritablePath() + "setSound.json";
    FILE* pFile = fopen(sPath.c_str(), "w");
    if(!pFile)                //说明这个文件并不存在 那么返回false
    {
        return;
    }
    Json::FastWriter writer;
    std::string sData(writer.write(dataInfo));
    fwrite(sData.c_str(), 1, sData.size(), pFile);
    fclose(pFile);
    
    
}

void JsonDataManager::initTopicData()           // 初始化题目信息
{
    //新闻类题目初始化
    Json::Value pv;
    Json::Reader jsReader;
    std::string srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/TopicData/XWTopicData.json");
    
    CCString* pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/TopicData/XWTopicData.json");
        return;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);
    
    for(int i=0; i < pv["data"].size(); i++)
    {
        CLTopicData* pItem = new CLTopicData(pv["data"][i]);
        
        m_vXWTopicDataList[pv["data"][i]["SYS_No"].asIntModestyle()] = pItem;
    }
    
    //文史类题目初始化
    pv.clear();
    srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/TopicData/WSTopicData.json");
    
    pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/TopicData/WSTopicData.json");
        return;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);
    
    for(int i=0; i < pv["data"].size(); i++)
    {
        CLTopicData* pItem = new CLTopicData(pv["data"][i]);
        
        m_vWSTopicDataList[pv["data"][i]["SYS_No"].asIntModestyle()] = pItem;
    }
    
    //地理类题目初始化
    pv.clear();
    srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/TopicData/DLTopicData.json");
    
    pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/TopicData/DLTopicData.json");
        return;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);
    
    for(int i=0; i < pv["data"].size(); i++)
    {
        CLTopicData* pItem = new CLTopicData(pv["data"][i]);
        
        m_vDLTopicDataList[pv["data"][i]["SYS_No"].asIntModestyle()] = pItem;
    }
    
    //体育类题目初始化
    pv.clear();
    srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/TopicData/TYTopicData.json");
    
    pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/TopicData/TYTopicData.json");
        return;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);
    
    for(int i=0; i < pv["data"].size(); i++)
    {
        CLTopicData* pItem = new CLTopicData(pv["data"][i]);
        
        m_vTYTopicDataList[pv["data"][i]["SYS_No"].asIntModestyle()] = pItem;
    }
    
    //科学类题目初始化
    pv.clear();
    srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/TopicData/KXTopicData.json");
    
    pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/TopicData/KXTopicData.json");
        return;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);
    
    for(int i=0; i < pv["data"].size(); i++)
    {
        CLTopicData* pItem = new CLTopicData(pv["data"][i]);
        
        m_vKXTopicDataList[pv["data"][i]["SYS_No"].asIntModestyle()] = pItem;
    }
    
    //时尚类题目初始化
    pv.clear();
    srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/TopicData/SSTopicData.json");
    
    pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/TopicData/SSTopicData.json");
        return;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);
    
    for(int i=0; i < pv["data"].size(); i++)
    {
        CLTopicData* pItem = new CLTopicData(pv["data"][i]);
        
        m_vSSTopicDataList[pv["data"][i]["SYS_No"].asIntModestyle()] = pItem;
    }
    
    //娱乐类题目初始化
    pv.clear();
    srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/TopicData/YLTopicData.json");
    
    pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/TopicData/YLTopicData.json");
        return;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);
    
    for(int i=0; i < pv["data"].size(); i++)
    {
        CLTopicData* pItem = new CLTopicData(pv["data"][i]);
        
        m_vYLTopicDataList[pv["data"][i]["SYS_No"].asIntModestyle()] = pItem;
    }
    
    //生活类题目初始化
    pv.clear();
    srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/TopicData/SHTopicData.json");
    
    pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/TopicData/SHTopicData.json");
        return;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);
    
    for(int i=0; i < pv["data"].size(); i++)
    {
        CLTopicData* pItem = new CLTopicData(pv["data"][i]);
        
        m_vSHTopicDataList[pv["data"][i]["SYS_No"].asIntModestyle()] = pItem;
    }
    
    //IQ类题目初始化
    pv.clear();
    srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/TopicData/IQTopicData.json");
    
    pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/TopicData/IQTopicData.json");
        return;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);
    
    for(int i=0; i < pv["data"].size(); i++)
    {
        CLTopicData* pItem = new CLTopicData(pv["data"][i]);
        
        m_vIQTopicDataList[pv["data"][i]["SYS_No"].asIntModestyle()] = pItem;
    }
    
    //坑爹类题目初始化
    pv.clear();
    srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/TopicData/KDTopicData.json");
    
    pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/TopicData/KDTopicData.json");
        return;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);
    
    for(int i=0; i < pv["data"].size(); i++)
    {
        CLTopicData* pItem = new CLTopicData(pv["data"][i]);
        
        m_vKDTopicDataList[pv["data"][i]["SYS_No"].asIntModestyle()] = pItem;
    }
    
    //文史类题目初始化
    pv.clear();
    srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/TopicData/PPTopicData.json");
    
    pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/TopicData/PPTopicData.json");
        return;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);
    
    for(int i=0; i < pv["data"].size(); i++)
    {
        CLTopicData* pItem = new CLTopicData(pv["data"][i]);
        
        m_vPPTopicDataList[pv["data"][i]["SYS_No"].asIntModestyle()] = pItem;
    }
    
    // 初始化题目类型转换表
//    pv.clear();
//    srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/TopicData/TopicTypeData.json");
//    
//    pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
//    if(pcstr == NULL)
//    {
//        CCLOG("This file %s is not exist!!!", "Data/TopicData/TopicTypeData.json");
//        return;
//    }
//    
//    jsReader.parse(pcstr->getCString(), pv, false);
//    
//    for(int i=0; i < pv["data"].size(); i++)
//    {
//        m_vTpoicTypeList[pv["data"][i]["QST_Catalog"].asStringModeStyle()] = pv["data"][i]["SYS_Caption"].asStringModeStyle();
//    }
    
}

CLTopicData* JsonDataManager::GetTopicDataFromID(const char* sType, int nTopicId)  //通过类别索引和题目id得到题目信息
{
    CLTopicData* pTopicData = NULL;
    std::map<int, CLTopicData*>::iterator iter_begin;
    std::string sTopicType = sType;
    
    if(sTopicType.compare("XW") == 0)
    {
        iter_begin = m_vXWTopicDataList.find(nTopicId);
        if(iter_begin != m_vXWTopicDataList.end())
            pTopicData = iter_begin->second;
    }
    else if(sTopicType.compare("WS") == 0)
    {
        iter_begin = m_vWSTopicDataList.find(nTopicId);
        if(iter_begin != m_vWSTopicDataList.end())
            pTopicData = iter_begin->second;
    }
    else if(sTopicType.compare("DL") == 0)
    {
        iter_begin = m_vDLTopicDataList.find(nTopicId);
        if(iter_begin != m_vDLTopicDataList.end())
            pTopicData = iter_begin->second;
    }
    else if(sTopicType.compare("TY") == 0)
    {
        iter_begin = m_vTYTopicDataList.find(nTopicId);
        if(iter_begin != m_vTYTopicDataList.end())
            pTopicData = iter_begin->second;
    }
    else if(sTopicType.compare("KX") == 0)
    {
        iter_begin = m_vKXTopicDataList.find(nTopicId);
        if(iter_begin != m_vKXTopicDataList.end())
            pTopicData = iter_begin->second;
    }
    else if(sTopicType.compare("SS") == 0)
    {
        iter_begin = m_vSSTopicDataList.find(nTopicId);
        if(iter_begin != m_vSSTopicDataList.end())
            pTopicData = iter_begin->second;
    }
    else if(sTopicType.compare("YL") == 0)
    {
        iter_begin = m_vYLTopicDataList.find(nTopicId);
        if(iter_begin != m_vYLTopicDataList.end())
            pTopicData = iter_begin->second;
    }
    else if(sTopicType.compare("SH") == 0)
    {
        iter_begin = m_vSHTopicDataList.find(nTopicId);
        if(iter_begin != m_vSHTopicDataList.end())
            pTopicData = iter_begin->second;
    }
    else if(sTopicType.compare("IQ") == 0)
    {
        iter_begin = m_vIQTopicDataList.find(nTopicId);
        if(iter_begin != m_vIQTopicDataList.end())
            pTopicData = iter_begin->second;
    }
    else if(sTopicType.compare("KD") == 0)
    {
        iter_begin = m_vKDTopicDataList.find(nTopicId);
        if(iter_begin != m_vKDTopicDataList.end())
            pTopicData = iter_begin->second;
    }
    else if(sTopicType.compare("PP") == 0)
    {
        iter_begin = m_vPPTopicDataList.find(nTopicId);
        if(iter_begin != m_vPPTopicDataList.end())
            pTopicData = iter_begin->second;
    }
    
    return pTopicData;
}


std::string JsonDataManager::GetTopicTypeFromString(std::string sType)
{
    std::string s = "";
    std::map<std::string, std::string>::iterator iter = m_vTpoicTypeList.find(sType);
    if(iter != m_vTpoicTypeList.end())
    {
        s = iter->second;
    }
    return s;
}

YinHuaInfo* JsonDataManager::GetYinhuaInfoFromID(int nId)
{
    std::map<int, YinHuaInfo*>::iterator iter = m_yinHua.find(nId);
    if(iter != m_yinHua.end())
        return iter->second;
    else
        return NULL;
}

//初始化成就信息
void JsonDataManager::initAchievementInfo()
{
    
    Json::Value pv;
    Json::Reader jsReader;
    std::string srcPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/CardData/AchievementData.json");
    
    CCString* pcstr = CCString::createWithContentsOfFile(srcPath.c_str());
    if(pcstr == NULL)
    {
        CCLOG("This file %s is not exist!!!", "Data/CardData/AchievementData.json");
        return;
    }
    
    jsReader.parse(pcstr->getCString(), pv, false);
    
    CCLOG("AchievementInfoJson:%s", pv.toStyledString().c_str());
    
    for(int i=0; i < pv["data"].size(); i++)
    {
        AchievementBean* pItem = new AchievementBean(pv["data"][i]);
        
        m_vAchievement[pv["data"][i]["ACH_No"].asIntModestyle()] = pItem;
        
    }
    
}



