//
//  CLSoundManager.h
//  LolAllStars1_X
//
//  Created by Zhang Qiyun on 13-5-13.
//
//

#ifndef __LolAllStars1_X__CLSoundManager__
#define __LolAllStars1_X__CLSoundManager__

#include <iostream>
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

class CLSoundManager
{
public:
    CLSoundManager();
    ~CLSoundManager();
    
    static CLSoundManager* sharedSoundManager();
    void init();
    void PlayBackGroundMusic(const char* filepath);
    void PlayEffectSound(const char* filepath);
    
    void StopBackGroundMusic();
    void StopEffectSound(const char* filepath);
    
    void SetBgMusicVolume(int iVoice);   //设置背景音乐大小
    void SetEffectVolume(int iVoice);    //设置音效大小

private:
    std::vector<std::string> m_vBackMusicList;
    std::map<std::string, unsigned int> m_vBackEffectList;
    std::string m_sPlayingBackMusic;
    
    float fBgMusicVolume; //背景音量大小
    float fEffectVolume;  //音效大小
};

#endif /* defined(__LolAllStars1_X__CLSoundManager__) */
