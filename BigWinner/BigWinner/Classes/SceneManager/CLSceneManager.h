//
//  CLSceneManager.h
//  LolAllStars1_X
//
//  Created by Zhang Qiyun on 13-5-7.
//
//

#ifndef __LolAllStars1_X__CLSceneManager__
#define __LolAllStars1_X__CLSceneManager__

#include <iostream>
#include "cocos2d.h"
#include "CLFadeColorLayer.h"
#include "CLLoadingLayer.h"
#include "CLNetErrorLayer.h"
#include "JsonParser.h"
#include "CLWaitNetLayer.h"
#include "CLStoreShelfData.h"
#include "CLTopicData.h"
#include "LGameCommonDefine.h"
USING_NS_CC;


enum SCENEMANGELAYER_TAG
{
    BACKPACK  =  1000,
    
    SCENENULL,
    
    //商店Tag
    SHOPTAG,
    
    //印花回复 //wangguolong20131107
    STAMPTAG,
    //印花查看回复
    STAMPMESSAGETAG,
    
    ACHIEVEMENTS, //成就
    VOLUMESET//音量音效设置
    ,RMBBUYCARD //购买人民币页面

};

class CLSceneManager : public CCObject
{
public:
    CLSceneManager();
    ~CLSceneManager();
    
    static CLSceneManager* sharedSceneManager();
    //static std::vector<MessagePack*> packMsg;
    
    void ChangeScene(CLSceneLayer* pScene);                       //切换场景
    void AddTempCoverScene(CCNode* pScene);                 //增加临时覆盖场景
    void RemoveTempCoverScene(CCNode* pScene);              //还原到主场景
    void ChangeSceneCall();                                 //阴影层黑掉后的回调函数
    void ChangeSceneCallEnd();
    void ReCoverSceneCall();                                //增加临时场景阴影层黑掉后的回调函数
    void ReCoverSceneCallEnd();
    void processDataWithDic(Json::Value &dic, cocos2d::CCObject* request);
    void SetNetErrorLayer(bool bCreate, std::string sText = "");                    // 生成或者取消这个层
    
    void BeginLoadAdvImages(int nMsgId, int nLoadModeStyle = 0);                    //网络消息下达后开始预加载资源
    
    void BeginNetToChangeScene(CCObject* targetScene, SEL_CallFunc pCallFun, int nMsgId); //开始请求服务器数据来切换场景
    void SetWaitNetLayer(bool bShow);
    
    void ChangeDeviceBGFromSceneID(CLScene_TypeIndex nIndex);
    
    //添加一个网络消息数据到列表中
    void AddJsonDataToList(Json::Value* jv);
    
    //添加一个商店信息到列表
    void AddStoreDataToList(CLStoreData* pData);
    
    void ClearStoreDataList();
    
    //取出一个商店列表信息
    CLStoreData* GetStoreDataFromList(int nIndex);
    
    //得到商店列表数量
    int GetStoreDataNum();
    
    //货架场景、每一个货架星际数量记录（在货架更新之后再去修改）
//    CC_SYNTHESIZE(int, m_lastStarNum, lastStarNum);           //20131223 上次进入货架的总星级数量（判断店长解锁用）
//    CC_SYNTHESIZE(bool, m_isFirstIn, ifBossPlay);             //20131223 是否播放（判断店长解锁用）
    std::map<int, int> m_iLastStarsMap;                        //临时记录每一个货架的星数量（货架No为Key）
    void recordShelfStar(int nShelfNo, int nStarNo);      //存储星级
    int getShelfStar(int nShelfNo);          //取得星级


    //添加一个房间信息到房间列表
    void AddPvpRoomInfoToList(PvpRoomInfo* pData);
    //得到Pvp房间数量
    int GetPvpRoomNum();
    
    //清除房间列表信息
    void ClearPvpRoomList();
    
    //得到一个房间信息
    PvpRoomInfo* GetPvpRoomInfoFromIndex(int nIndex);
    
    //wangguolong 20131109
    //添加一个印花信息到房间列表
    void AddPrizeInfoToList(PrizeInfo* pData);
    
    //得到印花数量
    int GetPrizeNum();
    
    //印花列表信息
    void ClearPrizeList();
    
    //得到一个印花信息
    PrizeInfo* GetPrizeInfoFromIndex(int nIndex);
    
    //wangguolong 20131111
    //添加对战设置背包题卡信息到list
    void AddUcardInfoToList(UcardInfo* pData);
    //得到对战设置背包题卡数量
    int GetUcardNum();
    //对战设置背包题卡信息
    void ClearUcardList();
    //得到对战设置背包题卡信息
    UcardInfo* GetUcardInfoFromIndex(int nIndex);
    
    //添加对战设置背包道具信息到list
    void AddUpropInfoToList(UpropInfo* pData);
    //得到对战设置背包道具数量
    int GetUpropNum();
    //对战设置背包道具信息
    void ClearUpropList();
    //得到对战设置背包道具信息
    UpropInfo* GetUpropInfoFromIndex(int nIndex);
    
    //添加对战设置背包角色信息到list
    void AddUroleInfoToList(UroleInfo* pData);
    //得到对战设置背包角色数量
    int GetUroleNum();
    //对战设置背包角色信息
    void ClearUroleList();
    //得到对战设置背包角色信息
    UroleInfo* GetUroleInfoFromIndex(int nIndex);
    
    //获取选择的道具 发送给服务器选择的道具信息
    void getStatusMessage();

    void AddUcardStatusMap(int nKey,int nValue);
    int FindUcardStatusMap(int nKey);
    void AddUpropStatusMap(int nKey,int nValue);
    int FindUpropStatusMap(int nKey);
    void AddUroleStatusMap(int nKey,int nValue);
    int FindUroleStatusMap(int nKey);
    
    //未领取成就相关持久化 （游戏加载的时候）add by yin
    std::vector<int> m_vUserInfoAchi;     //用户成就（最新3个）
    std::map<int, int> m_iNotGetAchiList;   //以Map记录未领取的成就(0为游戏中推送的成就，回到主场景的时候要弹出领取框， 1不显示)
    
    void AddAchieveInfoToList(int nAchiNo, int needConfirm); //添加未领取成就信息(bool回到主场景的时候要弹出领取框)
    void AddUserAchiInfo(int nAchiNo);    //添加用户最新成就（头像区域使用）
    int GetUngetAchiNum(); //未领取的成就数量
    void DelKey(int key);  //清空未领取成就容器（每次主场景加载后清空）
    void logicRemove(int key);  //逻辑删除未领取成就（显示后）
    bool GetIfHadMsgShow(); //判断推送队列中是否存在需要在主场景提示（也就是bool为true的key）
    
    std::map<int,int> GetFightCards(){ return UcardStatusMap; }
    std::map<int,int> GetFightProps(){ return UpropStatusMap; }
    std::map<int,int> GetFightRoles(){ return UroleStatusMap; }
    
    //wangguolong 点亮皇冠操作使用
    CC_SYNTHESIZE(int, m_nShelfNoX, nShelfNoX);//记录选择的货架ID
    CC_SYNTHESIZE(int, m_nStarNumX, nStarNumX);//记录是否是闯关胜利 1胜利 0失败
    CC_SYNTHESIZE(int, m_nNum, nNum); //记录货架
    
    //取出一个网络数据
    Json::Value* GetJsonDataFromMsgID(int nMsgId);
    void RemoveJsonDataFromMsgID(int nMsgId);
    
    void UpdateNewStoreLevel(int nOpenIndex);  //打开一个商铺开启权限
    
    CC_SYNTHESIZE(CLSceneLayer*, m_pRunningScene, pRunningScene); //当前正在运行的场景 在这里并非真正意义上cocos2d-x提供的场景层 基本只是游戏场景基层的Layer
    CC_SYNTHESIZE(CCNode*, m_pParentNode, pParentNode);     //新场景需要添加的父节点
    CC_SYNTHESIZE(CCNode*, m_pTempCoverNode, pTempCoverNode);     //在主场景上覆盖的另外一层临时场景，比如说进入pve战斗场景 那么战斗前进场景隐藏, 战斗结束后显示出来
    CC_SYNTHESIZE_RETAIN(CLSceneLayer*, m_pReplaceNode, pReplaceNode);     //等待替换的场景
    CC_SYNTHESIZE_RETAIN(CCNode*, m_pReCoverNode, pReCoverNode);     //等待临时覆盖的场景层
    CC_SYNTHESIZE(CLLoadingLayer*, m_pLoadingLayer, pLoadingLayer);     //loading 层
    CC_SYNTHESIZE(CLFadeColorLayer*, m_pFadeLayer, pFadeLayer);     //用来过渡的阴影层
    CC_SYNTHESIZE(CCObject*, m_pCallTarget, pCallTarget);     //回调用的加载对象
    CC_SYNTHESIZE(SEL_CallFunc, m_pCallFun, pCallFun);        //回调用的加载函数
    CC_SYNTHESIZE(Json::Value, m_vNetData, vNetData);         //网络下发暂存的value
    CC_SYNTHESIZE(CLNetErrorLayer*, m_pNetErrorLayer, pNetErrorLayer);         //网络错误的提示层
    CC_SYNTHESIZE(CLWaitNetLayer*, m_pWaitNetLayer, pWaitNetLayer);   //等待网络消息屏蔽层
    CC_SYNTHESIZE(CCSprite*, m_pDeviceBG, pDeviceBG);         //设备图指针
    
    CC_SYNTHESIZE(std::vector<UcardInfo*>, m_vUcardList, vUcardList);
    CC_SYNTHESIZE(std::vector<UpropInfo*>, m_vUpropList, vUpropList);
    CC_SYNTHESIZE(std::vector<UroleInfo*>, m_vUroleList, vUroleList);
    
    CC_SYNTHESIZE(int, m_newlevel, newlevel); //wangguolong 挑战店长成功以后的数据
    
    CC_SYNTHESIZE(int, m_nPair, nPair); // 有此键表示匹配失败，没有表示缺少印花 1表示缺少印花 。0为不缺少。
    
    CC_SYNTHESIZE(int, m_nNolock, nNolock); //记录解锁的店铺
    
    CC_SYNTHESIZE(int, m_nValuetime, nValuetime); //还有多长时间可以对战 0表示可以对战 其它是距离对战的时间
    
    CC_SYNTHESIZE(int, m_nOpenflg, nOpenflg); //印花兑换功能是否开放
    
    CC_SYNTHESIZE(int, m_nGgaoFlg, nGgaoFlg); //公告用 判断公告是否查看,查看为1 否则为0
    
    CC_SYNTHESIZE(int, m_nCheckShopNo, nCheckShopNo); //当前选中的商铺
    
    CC_SYNTHESIZE(int, m_nCheckShelf, nCheckShelf); //当前选中的货架

private:
    std::map<int, Json::Value*> m_vJsonData;                  //需要缓存的网络json数据，每次用完记得删除
    std::vector<CLStoreData*> m_vStoreDataList;               //商店列表
    std::vector<PvpRoomInfo*> m_vPvpRoomList;                 //对战房间列表信息
    std::vector<PrizeInfo*> m_vPrizeList;                     //奖品橱窗信息 //wangguolong

    
    std::map<int,int> UcardStatusMap;                         //对战 备战设置选择的题卡 //wangguolong 20131112
    std::map<int,int> UpropStatusMap;                         //对战 备战设置选择的道具 //wangguolong 20131112
    std::map<int,int> UroleStatusMap;                         //对战 备战设置选择的角色 //wangguolong 20131112
    
};

#endif /* defined(__LolAllStars1_X__CLSceneManager__) */
