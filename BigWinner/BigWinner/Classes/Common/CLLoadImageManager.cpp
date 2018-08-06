//
//  CLLoadImageManager.cpp
//  Yy_mobileGame3
//
//  Created by Zhang Qiyun on 13-6-20.
//
//

#include "CLLoadImageManager.h"
#include "JsonDataManager.h"
#include "CLSceneManager.h"
#include "CLRealGameLayer.h"

static CLLoadImageManager* g_pLoadImageManager = NULL;
static bool g_bFirstEntergame = true;

CLLoadImageManager::CLLoadImageManager()
{
    m_nCurNetMsgId = 0;
    m_nHaveLoadIndex = 0;
    m_nNeedLoadCount = 0;
    m_nBeginLoadIndex = 0;
    m_pNeedAdvImageArray = NULL;
    m_pLastAdvImageArray = NULL;
    m_nLoadModeStyle = 0;
}

CLLoadImageManager::~CLLoadImageManager()
{
    CC_SAFE_DELETE(m_pSceneIrDic);
    
    std::map<std::string, Json::Value*>::iterator iter_beg = m_pLoadImageList.begin();
    for(; iter_beg != m_pLoadImageList.end(); iter_beg++)
    {
        Json::Value* pvalue = iter_beg->second;
        CC_SAFE_DELETE(pvalue);
    }
}

CLLoadImageManager* CLLoadImageManager::shardLIManager()
{
    if(!g_pLoadImageManager)
    {
        g_pLoadImageManager = new CLLoadImageManager();
        g_pLoadImageManager->init();
    }
    return g_pLoadImageManager;
}


void CLLoadImageManager::init()
{
    //前期加载卡片资源和UI 资源
    
}

bool CLLoadImageManager::CheckNeedAdvLoad(int nNetMsdId, int nLoadModeStyle)
{
    m_pNeedAdvImageArray = NULL;
    m_nNeedLoadCount = 0;
    if(m_nCurNetMsgId == nNetMsdId)
        return false;
    
    m_nLoadModeStyle = nLoadModeStyle;
    if(g_bFirstEntergame)   //第一次异步加载图片资源 就把要用到的不会删除的资源加载近来
    {
        Json::Value pv1;
        Json::Reader jsReader1;
        std::string srcPath1 = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/ImageData/UIImageList.json");
        
        CCString* pcstr1 = CCString::createWithContentsOfFile(srcPath1.c_str());
        if(pcstr1 == NULL)
        {
            CCLOG("This file %s is not exist!!!", "Data/ImageData/UIImageList.json");
        }
        jsReader1.parse(pcstr1->getCString(), pv1, false);
        Json::Value pv2(Json::arrayValue);
        pv2 = pv1["data"];
        
        for(int i=0; i<pv2.size(); i++)
        {
            m_vFirstAdvLoadImages.push_back(pv2[i].asCString());
            m_nNeedLoadCount++;
        }
        
        // 加载卡片资源
        Json::Value pv3;
        Json::Reader jsReader2;
        std::string srcPath2 = CCFileUtils::sharedFileUtils()->fullPathForFilename("Data/ImageData/CardImageList.json");
        
        CCString* pcstr2 = CCString::createWithContentsOfFile(srcPath2.c_str());
        if(pcstr2 == NULL)
        {
            CCLOG("This file %s is not exist!!!", "Data/ImageData/CardImageList.json");
        }
        jsReader2.parse(pcstr2->getCString(), pv3, false);
        Json::Value pv4(Json::arrayValue);
        pv4 = pv3["data"];
        
        for(int i=0; i<pv4.size(); i++)
        {
            m_vFirstAdvLoadImages.push_back(pv4[i].asCString());
            m_nNeedLoadCount++;
        }
        
    }
    
    std::vector<std::string>* pvs = JsonDataManager::shardJDManager()->GetSceneImagesArray(nNetMsdId);
    if(!pvs)
        return false;
    else
    {
        if(g_bFirstEntergame)
        {
            for(int i=0; i < pvs->size(); i++)
            {
                m_vFirstAdvLoadImages.push_back((*pvs)[i]);
                m_nNeedLoadCount++;
            }
        }
        else
            m_nNeedLoadCount = pvs->size();
        
        m_pNeedAdvImageArray = pvs;
        m_nCurNetMsgId = nNetMsdId;
        return true;
    }
}

void CLLoadImageManager::StartLoadAdvImages()                 // 开始加载的回调函数
{
    CLRealGameLayer* pLayer = (CLRealGameLayer*)CLSceneManager::sharedSceneManager()->getpParentNode();
    if(!pLayer)
        return;
    if(m_nLoadModeStyle == 2)    //只删不加模式
    {
        m_nBeginLoadIndex = 0;
        m_nNeedLoadCount = 0;
        m_nHaveLoadIndex = 0;
        m_nNeedLoadCount = 0;
        CallFunHaveLoaded(NULL);
        return;
    }
    pLayer->StartSchedule(0.03);
}

void CLLoadImageManager::LoadingAdvImages()
{
    
    if(m_nBeginLoadIndex < m_nNeedLoadCount)
    {
        if(g_bFirstEntergame)
        {
            CCLOG("Load %d Image index is %s", m_nBeginLoadIndex, m_vFirstAdvLoadImages[m_nBeginLoadIndex].c_str());
            CCTextureCache::sharedTextureCache()->addImageAsync(m_vFirstAdvLoadImages[m_nBeginLoadIndex].c_str(), this, callfuncO_selector(CLLoadImageManager::CallFunHaveLoaded));
        }
        else
        {
            CCLOG("Load %d Image index is %s", m_nBeginLoadIndex, (*m_pNeedAdvImageArray)[m_nBeginLoadIndex].c_str());
            CCTextureCache::sharedTextureCache()->addImageAsync((*m_pNeedAdvImageArray)[m_nBeginLoadIndex].c_str(), this, callfuncO_selector(CLLoadImageManager::CallFunHaveLoaded));
            
        }
        m_nBeginLoadIndex++;

    }
    else
    {
        if(g_bFirstEntergame)
        {
            g_bFirstEntergame = false;
            m_vFirstAdvLoadImages.clear();
        }
        CLRealGameLayer* pLayer = (CLRealGameLayer*)CLSceneManager::sharedSceneManager()->getpParentNode();
        if(!pLayer)
            return;
        pLayer->EndSchedule();
        m_nBeginLoadIndex = 0;
    }
}

void CLLoadImageManager::CallFunHaveLoaded(CCTexture2D* texture)
{
    m_nHaveLoadIndex++;
//    CCLOG("AdvLoadImage name is %d is loaded", texture->getName());
    if(m_nHaveLoadIndex >= m_nNeedLoadCount)
    {
        if(m_pLastAdvImageArray && m_nLoadModeStyle != 1) //如果要删除 那么则 需要当前模式不是 只加不删模式
        {
            for(int i=0; i < m_pLastAdvImageArray->size(); i++)
            {
                std::vector<std::string>::iterator iter_begin = m_pNeedAdvImageArray->begin();
                bool bClear = true;
                for(; iter_begin != m_pNeedAdvImageArray->end(); iter_begin++)
                {
                    std::string s_now = *iter_begin;
                    if(s_now.compare((*m_pLastAdvImageArray)[i]) == 0)  //说明有一样的
                    {
                        CCLOG("Have the same texture is %s", (*m_pLastAdvImageArray)[i].c_str());
                        bClear = false;
                        break;
                    }
                }
                if(bClear)
                    CCTextureCache::sharedTextureCache()->removeTextureForKey(CCFileUtils::sharedFileUtils()->fullPathForFilename((*m_pLastAdvImageArray)[i].c_str()).c_str());
            }
        }
        m_pLastAdvImageArray = m_pNeedAdvImageArray;
        
        SEL_CallFunc pCallFun = CLSceneManager::sharedSceneManager()->getpCallFun();
        CCObject* pCallTarget = CLSceneManager::sharedSceneManager()->getpCallTarget();
        (pCallTarget->*pCallFun)();
        m_nHaveLoadIndex = 0;
        m_nNeedLoadCount = 0;
    }
}

