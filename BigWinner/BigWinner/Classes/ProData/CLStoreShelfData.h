//
//  CLStoreShelfData.h
//  BigWinner
//
//  Created by Zhang Qiyun on 13-10-20.
//  商铺和货架信息类
//
//

#ifndef __BigWinner__CLStoreShelfData__
#define __BigWinner__CLStoreShelfData__

#include "cocos2d.h"
#include "JsonParser.h"
USING_NS_CC;

//商铺数据
class CLStoreData
{
public:
    CLStoreData();
    CLStoreData(Json::Value& jv);
    ~CLStoreData();
    
    CC_SYNTHESIZE(std::string, m_sCaption, sCaption);         //商店名称
    CC_SYNTHESIZE(std::string, m_sStoreID, sStoreID);         //字符ID
    CC_SYNTHESIZE(bool, m_bLocked, bLocked);                  //是否锁定
    CC_SYNTHESIZE(int, m_nNomber, nNomber);                   //商店编号
    CC_SYNTHESIZE(int, m_nSortBy, nSortBy);                   //排序号
    CC_SYNTHESIZE(std::string, m_sText, sText);               //商店说明等
    CC_SYNTHESIZE(int, m_newlevel, newlevel);               //newlevel
    CC_SYNTHESIZE(int, m_nOneLevel, nOneLevel);
};



//货架数据
class CLShelfData
{
public:
    CLShelfData();
    CLShelfData(Json::Value& jv);
    ~CLShelfData();

    CC_SYNTHESIZE(std::string, m_sCaption, sCaption);         //货架名称
    CC_SYNTHESIZE(std::string, m_sClassName, sClassName);     //货架类别
    CC_SYNTHESIZE(std::string, m_sShelfID, sShelfID);         //字符ID
    CC_SYNTHESIZE(int, m_nInterval, nInterval);               //
    CC_SYNTHESIZE(int, m_nStars, nStars);                     //获得星星数量
    CC_SYNTHESIZE(int, m_nNomber, nNomber);                   //货架编号
    CC_SYNTHESIZE(int, m_nSortBy, nSortBy);                   //排序号
    CC_SYNTHESIZE(int, m_nStyle, nStyle);                     //类型 是 店长 还是货架 0为货架 1为店长
    CC_SYNTHESIZE(std::string, m_sText, sText);               //货架说明等
};





#endif /* defined(__BigWinner__CLStoreShelfData__) */
