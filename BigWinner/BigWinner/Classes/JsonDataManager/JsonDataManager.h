//
//  JsonDataManager.h
//  Yy_mobileGame3
//
//  Created by Zhang Qiyun on 13-6-17.
//
//

#ifndef __Yy_mobileGame3__JsonDataManager__
#define __Yy_mobileGame3__JsonDataManager__

#include <iostream>
#include "cocos2d.h"
#include "JsonParser.h"
#include "CLTopicData.h"
USING_NS_CC;

class JsonDataManager
{
public:
    JsonDataManager();
    ~JsonDataManager();
    
    static JsonDataManager* shardJDManager();
    void init();
    void initUrlData();                                     // 初始化Url信息
    void initChangeStringData();                            // 初始化多语言文字信息
    void initBaseData();                                    // 初始化基础数据
    void initLoadItemData();                                // 初始化异步加载的图片数据
    void initLoadNickNameData();                            // 初始化名字库
    void initShieldWordsList();                             // 初始化屏蔽字库
    std::vector<std::string>* GetSceneImagesArray(int nNetMsgId);  //得到场景预加载图片信息， 如果是空 就是返回NULl 那就说明不需要 预加载
    
    std::string GetUrlFromUrlList(int CmdUrl_Index);        // 根据网络通讯Id得到Url全地址
    std::string GetCheckVerUrl(int CmdUrl_Index);           // 根据网络通讯Id得到获得监测版本的url
    
    std::string GetNickNameFromRandom();                    // 随机读取昵称
    bool CheckHasShieldWord(std::string& sCheckWord);       // 检测是否含屏蔽字符
    
    void initCommenDefineData();                            // 初始化通用文件信息
    void initTopicData();                                   // 初始化题目信息
    CLTopicData* GetTopicDataFromID(const char* sType, int nTopicId);  //通过类别索引和题目id得到题目信息
    
    void initCardJueSeDaojuData();                          //初始化背包、角色、提卡、道具等信息
    void initYinHuaData();                                  //初始化印花信息
    //wangguolong 
    void initShelfData();                                  //初始化货架信息
    void GetMusicInfo(Json::Value &dataInfo);                                      //获得 音乐、音效
    void SetMusicInfo(Json::Value &dataInfo);                                      //设置音乐音效
    
    std::string GetTopicTypeFromString(std::string sType);  //找到题库中文显示字样
    YinHuaInfo* GetYinhuaInfoFromID(int nId);               //根据印花模版ID得到印花模版数据
    
    void initAchievementInfo();       //初始化成就信息

public:
    //卡牌、角色、道具信息
    std::map<int, CardJueseDaoju*> m_vCardJueseDaoju;   //品牌类
    
    //印花信息
    std::map<int, YinHuaInfo*> m_yinHua;
    
    //wangguolong 20131109
    std::map<int, ShelfInfo*> m_Shelf;
    
    //成就信息 add by yin
    std::map<int, AchievementBean*> m_vAchievement; 
    
private:
    std::map<std::string, std::string> m_vUrlBranchList;
    std::map<std::string, std::string> m_vChangeStringList;
    
    //需要预加载的场景数据文件列表
    std::map<std::string, std::string> m_vIamgeSceneFileList;
    
    //每一个需要预加载的场景所需加载的资源列表
    std::map<std::string, std::vector<std::string>*> m_vScenesImageList;
    
    //昵称第一部分
    std::vector<std::string> m_vNickNames1st;
    
    //昵称第二部分
    std::vector<std::string> m_vNickNames2ed;
    
    //屏蔽字库列表
    std::vector<std::string> m_vShieldWordsList;
    
    //以下是全局的题库列表，每一个类型，分成一张表
    std::map<int, CLTopicData*> m_vXWTopicDataList;   //新闻类
    std::map<int, CLTopicData*> m_vWSTopicDataList;   //文史类
    std::map<int, CLTopicData*> m_vDLTopicDataList;   //地理类
    std::map<int, CLTopicData*> m_vTYTopicDataList;   //体育类
    std::map<int, CLTopicData*> m_vKXTopicDataList;   //科学类
    std::map<int, CLTopicData*> m_vSSTopicDataList;   //时尚类
    std::map<int, CLTopicData*> m_vYLTopicDataList;   //娱乐类
    std::map<int, CLTopicData*> m_vSHTopicDataList;   //生活类
    std::map<int, CLTopicData*> m_vIQTopicDataList;   //IQ类
    std::map<int, CLTopicData*> m_vKDTopicDataList;   //坑爹类
    std::map<int, CLTopicData*> m_vPPTopicDataList;   //品牌类
    
    //题型中文对应表
    std::map<std::string, std::string> m_vTpoicTypeList; 
    
    
    CC_SYNTHESIZE(std::string, m_sCurVersion, sCurVersion);
    CC_SYNTHESIZE(std::string, m_sLoginUrl, sLoginUrl);        //登陆服务器 的IP
    CC_SYNTHESIZE(int, m_nLoginPort, nLoginPort);              //登陆服务器 的端口
    
    CC_SYNTHESIZE(std::string, m_sPlayerAccount, sPlayerAccount); //游戏帐号
    CC_SYNTHESIZE(std::string, m_sPlayerPasswoed, sPlayerPasswoed); //游戏密码


    CC_SYNTHESIZE(bool, m_bAccountHasAvatar, bAccountHasAvatar);   //帐号中是否已经有角色

    
};

#endif /* defined(__Yy_mobileGame3__JsonDataManager__) */
