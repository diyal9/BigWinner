//
//  CLStoreShelfData.cpp
//  BigWinner
//
//  Created by Zhang Qiyun on 13-10-20.
//
//

#include "CLStoreShelfData.h"

CLStoreData::CLStoreData()
{
    m_bLocked = true;
    m_nNomber = 0;
    m_nSortBy = 0;
    m_newlevel = 0;
    m_nOneLevel = 0;
}

CLStoreData::CLStoreData(Json::Value& jv)
{
    CLStoreData();
    m_bLocked = (bool)jv["Locked"].asIntModestyle();
    m_sCaption = jv["Caption"].asStringModeStyle();
    m_sStoreID = jv["ID"].asStringModeStyle();
    m_nNomber  = jv["No"].asIntModestyle();
    m_sText    = jv["Text"].asStringModeStyle();
    m_nSortBy  = jv["SortBy"].asIntModestyle();
}

CLStoreData::~CLStoreData()
{

}


CLShelfData::CLShelfData()
{
    m_nSortBy = 0;
    m_nNomber = 0;
    m_nStars = 0;
    m_nInterval = 0;
    m_nStyle = 0;

}

CLShelfData::CLShelfData(Json::Value& jv)
{
    CLShelfData();
    m_sCaption = jv["Caption"].asStringModeStyle();
    m_sClassName = jv["ClassName"].asStringModeStyle();
    m_sShelfID = jv["ID"].asStringModeStyle();
    m_nInterval = jv["Interval"].asIntModestyle();
    m_nNomber  = jv["No"].asIntModestyle();
    m_sText    = jv["Text"].asStringModeStyle();
    m_nSortBy  = jv["SortBy"].asIntModestyle();
    m_nStyle   = jv["Style"].asIntModestyle();
    m_nStars   = jv["Stars"].asIntModestyle();
}

CLShelfData::~CLShelfData()
{
    
}













