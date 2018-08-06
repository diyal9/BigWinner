//
//  CLLoadImageManager.h
//  Yy_mobileGame3
//
//  Created by Zhang Qiyun on 13-6-20.
//
//

#ifndef __Yy_mobileGame3__CLLoadImageManager__
#define __Yy_mobileGame3__CLLoadImageManager__

#include <iostream>
#include "cocos2d.h"
#include "JsonParser.h"
USING_NS_CC;

class CLLoadImageManager : public CCObject
{
public:
    CLLoadImageManager();
    ~CLLoadImageManager();
    static CLLoadImageManager* shardLIManager();
    void init();
    
    bool CheckNeedAdvLoad(int nNetMsdId, int nLoadModeStyle);
    void StartLoadAdvImages();                           // 开始加载
    void LoadingAdvImages();                             // 加载进行中的回调函数
    void CallFunHaveLoaded(CCTexture2D* texture);        // 异步单个资源加载完成后的回调函数
    
    CC_SYNTHESIZE(int, m_nNeedLoadCount, nNeedLoadCount);        //此场景需要后台加载的资源列表
    CC_SYNTHESIZE(int, m_nHaveLoadIndex, nHaveLoadIndex);        //后台已经加载完成的数目
    CC_SYNTHESIZE(int, m_nCurNetMsgId, nCurNetMsgId);            //当前场景对应的网络序列号
    CC_SYNTHESIZE(int, m_nBeginLoadIndex, nBeginLoadIndex);      //开始需要异步加载的资源索引编号
    CC_SYNTHESIZE(std::vector<std::string>*, m_pNeedAdvImageArray, pNeedAdvImageArray);      //得到的需要预加载的资源名称列表
    CC_SYNTHESIZE(std::vector<std::string>*, m_pLastAdvImageArray, pLastAdvImageArray);      //要转换新场景之前需要删除上一场景的特有资源内存的列表
    CC_SYNTHESIZE(int, m_nLoadModeStyle, nLoadModeStyle);        //后台加载模式 0.普通模式(既加又删, 默认模式) 1.只加不删   2.只删不加
    
private:
    Json::Value* m_pSceneIrDic;                //网络消息号对应场景名称
    std::map<std::string, Json::Value*> m_pLoadImageList;   //每个场景需要加载的资源列表
    std::vector<std::string> m_vFirstAdvLoadImages;  //第一次需要异步加载的全部图片数据
};

#endif /* defined(__Yy_mobileGame3__CLLoadImageManager__) */
