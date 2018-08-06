//
//  CLSoundManager.cpp
//  LolAllStars1_X
//
//  Created by Zhang Qiyun on 13-5-13.
//
//

#include "CLSoundManager.h"
#include "JsonParser.h"
#include "JsonDataManager.h"
#include "CLSceneManager.h"

USING_NS_CC;

static CLSoundManager* g_SoundManager = NULL;

CLSoundManager::CLSoundManager()
{
    fBgMusicVolume = 0;
    fEffectVolume = 0;
}

CLSoundManager::~CLSoundManager()
{
    
}

CLSoundManager* CLSoundManager::sharedSoundManager()
{
    if(!g_SoundManager)
    {
        g_SoundManager = new CLSoundManager();
    }
    return g_SoundManager;
}

void CLSoundManager::init()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic( CCFileUtils::sharedFileUtils()->fullPathForFilename("Sound/Sl_BackMusic.mp3").c_str() ); //主场景背景音乐

//    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic( CCFileUtils::sharedFileUtils()->fullPathForFilename("Sound/LolFightBGMusic.mp3").c_str() );
//    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect( CCFileUtils::sharedFileUtils()->fullPathForFilename("Sound/GUI_Click_Effcet.caf").c_str() );
    
    m_vBackMusicList.push_back("Sound/Sl_BackMusic.mp3");
//    m_vBackMusicList.push_back("Sound/LolFightBGMusic.mp3");
//    m_vBackEffectList["Sound/GUI_Click_Effcet.caf"] = 0;
    m_vBackEffectList["Sound/Sl_WInClose.wav"] = 0;          //窗体开关闭
    m_vBackEffectList["Sound/Sl_CountDown.wav"] = 0;         //倒计时声音
    m_vBackEffectList["Sound/Sl_BtnClick.wav"] = 0;          //点击按钮
    m_vBackEffectList["Sound/Sl_ClickShelf.wav"] = 0;        //点亮货架
    m_vBackEffectList["Sound/Sl_PvPStart.wav"] = 0;          //对战开始
    m_vBackEffectList["Sound/Sl_CompoundSuccess.wav"] = 0;   //合成成功
    m_vBackEffectList["Sound/Sl_AnswerError.wav"] = 0;       //回答错误
    m_vBackEffectList["Sound/Sl_AnswerSuccess.wav"] = 0;     //回答正确
    m_vBackEffectList["Sound/Sl_ShelfShow.wav"] = 0;         //货架出场
    m_vBackEffectList["Sound/Sl_GetGoods.wav"] = 0;          //获得物品
    m_vBackEffectList["Sound/Sl_AddTimeCard.wav"] = 0;       //加时卡
    m_vBackEffectList["Sound/Sl_AddBloodCard.wav"] = 0;      //加血卡
    m_vBackEffectList["Sound/Sl_Handssors.wav"] = 0;         //剪刀手
    m_vBackEffectList["Sound/Sl_ComeInShop.wav"] = 0;        //进入商铺
    m_vBackEffectList["Sound/Sl_RefuseAnswerCard.wav"] = 0;  //剪刀手
    m_vBackEffectList["Sound/Sl_WolfGmother.wav"] = 0;       //狼外婆
    m_vBackEffectList["Sound/Sl_MatchSuccess.wav"] = 0;      //匹配成功
    m_vBackEffectList["Sound/Sl_MatchingChange.wav"] = 0;    //匹配中切换
    m_vBackEffectList["Sound/Sl_ShopUnLock.wav"] = 0;        //商铺解锁
    m_vBackEffectList["Sound/Sl_RiseCard.wav"] = 0;          //升杯卡
    m_vBackEffectList["Sound/Sl_QuestionChange.wav"] = 0;    //题目切换
    m_vBackEffectList["Sound/Sl_NewMsgPrompt.wav"] = 0;      //新消息提示
    m_vBackEffectList["Sound/Sl_PvpWin.wav"] = 0;            //战斗胜利
    m_vBackEffectList["Sound/Sl_PvpFail.wav"] = 0;           //战斗失败
    m_vBackEffectList["Sound/Sl_SpoilsGive.wav"] = 0;        //战利品发放
    
    Json::Value value;
    JsonDataManager::shardJDManager()->GetMusicInfo(value);
    CCLog("SoundVoice->%d, %d", value["bgMusic"].asIntModestyle(),value["bgEfect"].asIntModestyle());
    fBgMusicVolume = value["bgMusic"].asIntModestyle();
    fEffectVolume = value["bgEfect"].asIntModestyle();

}

void CLSoundManager::PlayBackGroundMusic(const char* filepath)
{
    if(CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    }
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath).c_str(), true);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(fBgMusicVolume); //设置声音大小
}

void CLSoundManager::PlayEffectSound(const char* filepath)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath).c_str());
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(fEffectVolume); //设置声音大小
}

void CLSoundManager::StopBackGroundMusic()
{
    if(CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    }
}

void CLSoundManager::StopEffectSound(const char* filepath)
{

}

void CLSoundManager::SetBgMusicVolume(int iVoice)
{
    fBgMusicVolume = iVoice / 100.00;
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(fBgMusicVolume); //设置声音大小
}

void CLSoundManager::SetEffectVolume(int iVoice)
{
    fEffectVolume = iVoice / 100.00;
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(fEffectVolume); //设置声音大小
}









