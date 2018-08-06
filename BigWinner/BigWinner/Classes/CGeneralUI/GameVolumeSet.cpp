//
//  GameVolumeSet.cpp
//  BigWinner
//
//  Created by LunaBox on 13-12-16.
//
//

#include "GameVolumeSet.h"
#include "LGameCommonDefine.h"
#include "Button.h"
#include "CLChangeString.h"
#include "JsonDataManager.h"
#include "JsonParser.h"
#include "CLSoundManager.h"
#include "CLSceneManager.h"
#include "CLSoundManager.h"

using namespace Json;

GameVolumeSet* GameVolumeSet::create()
{
    GameVolumeSet *pRet = new GameVolumeSet();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool GameVolumeSet::init()
{
    if(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0)))
    {
        return false;
    }
    
    //取得上次设定的音量和音效值
    Json::Value value;
    JsonDataManager::shardJDManager()->GetMusicInfo(value);
    this->setfMusic(value["bgMusic"].asIntModestyle());
    this->setfSoundEffect(value["bgEfect"].asIntModestyle());
    
    //初始化UI
    doBaseUI();
    
    //窗体开闭音乐
    CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_WInClose.wav");
    
    doAction(this, true);
    
    return true;
}

void GameVolumeSet::doBaseUI()
{
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    //背景
    CCSprite* spriteBg = CCSprite::create("Image/BackPack/GameVolumeBG01.png");
    spriteBg->setPosition(ccp(size.width*0.5, size.height*0.5));
    this->addChild(spriteBg);
    
    //标题
    CCSprite* pTTFBG = CCSprite::create("Image/PveLayer/CP_GiveUpTTFBG.png");
    pTTFBG->setAnchorPoint(ccp(0, 1));
    pTTFBG->setPosition(ccp(G_SF(45), spriteBg->getContentSize().height));
    spriteBg->addChild(pTTFBG, 1);
    
    //标题文字
    CCSprite* biaoti = CCSprite::create("Image/BackPack/GameSet.png");
    biaoti->setPosition(ccp(biaoti->getContentSize().width,biaoti->getContentSize().height+G_SF(10)));
    pTTFBG->addChild(biaoti);
    
    //白色背景
    m_pGiveBG1 = CCSprite::create("Image/BackPack/GameVolumeBG02.png");
    m_pGiveBG1->setPosition(ccp(spriteBg->getContentSize().width*0.5, spriteBg->getContentSize().height*0.55));
    spriteBg->addChild(m_pGiveBG1, 1);

    //背景音乐
    createCCControlSlider(CLChangeString::sharedChangeString()->GetChangeString("BackgroundMusic").c_str(),0.75);

    //背景音效
    createCCControlSlider(CLChangeString::sharedChangeString()->GetChangeString("BackgroundSound").c_str(),0.4);
    
    //确认按钮
    Button* qurenBtn = Button::createBtn("Image/PublicUI/CommonIcon_Cancel.png","Image/PublicUI/CommonIcon_Ok.png");
    qurenBtn->setPosition(ccp(spriteBg->getContentSize().width * 0.72, spriteBg->getContentSize().height * 0.16));
    qurenBtn->setOnClickCallbackNode(callfuncN_selector(GameVolumeSet::AffirmBtn), this);
    spriteBg->addChild(qurenBtn);
    
    //确认按钮名称
    CCLabelTTF* labelQR = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("OkBtn").c_str(), "Arial", G_SF(40));
    labelQR->setPosition(ccp(G_SF(0), G_SF(0)));
    qurenBtn->addChild(labelQR);
    
    //取消按钮
    Button *quxiaoBtn = Button::createBtn("Image/PublicUI/CommonIcon_Ok.png","Image/PublicUI/CommonIcon_Cancel.png");
    quxiaoBtn->setPosition(ccp(spriteBg->getContentSize().width * 0.28, spriteBg->getContentSize().height * 0.16));
    quxiaoBtn->setOnClickCallbackNode(callfuncN_selector(GameVolumeSet::CanceBtn), this);
    spriteBg->addChild(quxiaoBtn);
    
    //取消按钮名称
    CCLabelTTF* labelQX = CCLabelTTF::create(CLChangeString::sharedChangeString()->GetChangeString("CancleBtn").c_str(), "Arial", G_SF(40));
    labelQX->setPosition(ccp(G_SF(0), G_SF(0)));
    quxiaoBtn->addChild(labelQX);
    
}

//动作执行
void GameVolumeSet::doAction(CCNode* r, bool moveIn)
{
    CCSize size = CCDirector::sharedDirector()->getgSceneSize();
    
    CLSceneLayer* pMainLayer = (CLSceneLayer*)CLSceneManager::sharedSceneManager()->getpRunningScene();
    
    if (moveIn) //进场动作
    {
        CCLayerColor *fadeLayer = CCLayerColor::create();
        pMainLayer->addChild(fadeLayer,9,500);
        
        CCFiniteTimeAction *actionOut = CCSequence::create(CCFadeTo::create(0.4f, 255*0.5), NULL); //透明度动画
        fadeLayer->runAction(actionOut);
        
        CCAction *actionMoveOut = CCEaseExponentialOut::create(CCMoveTo::create(0.4f, ccp(0, size.height))); //快到慢
        this->runAction(actionMoveOut);
        
        //窗体开闭音乐
        CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_WInClose.wav");
    }
    else //出场动作
    {
        CCFiniteTimeAction *actionIn = CCSequence::create(CCFadeTo::create(0.4f, 0), NULL); //透明度动画
        pMainLayer->getChildByTag(500)->runAction(actionIn);
        
        CCAction *actionMoveIn = CCEaseExponentialIn::create(CCMoveTo::create(0.4f, ccp(r->getPositionX(), r->getPositionY() * (-1)))); //慢到快
        r->runAction(actionMoveIn);
        
        pMainLayer->getChildByTag(500)->removeFromParentAndCleanup(true);
    }
    
}

void GameVolumeSet::createCCControlSlider(std::string name,float fscale)
{
    //背景音乐
    m_VolumeMusic = CCLabelTTF::create(name.c_str(), "Arial", G_SF(24));
    m_VolumeMusic->setColor(ccc3(100, 50, 0));
    m_VolumeMusic->setPosition(ccp(m_pGiveBG1->getContentSize().width*0.15, m_pGiveBG1->getContentSize().height*fscale));
    m_pGiveBG1->addChild(m_VolumeMusic);
    
    
    //背景音乐大小控制
    m_slider = CCControlSlider::create("Image/BackPack/VolumeBackground.png", "Image/BackPack/VolumeBackground.png", "Image/BackPack/VolumeBtn.png");
    m_slider->setPosition(ccp(m_pGiveBG1->getContentSize().width*0.52,m_pGiveBG1->getContentSize().height*fscale));
    m_slider->setTouchPriority(-10004);
    //设置滑动条的范围
    m_slider->setMinimumValue(-15);
    m_slider->setMaximumValue(115);
    
    //设置滑动条的当前值
    m_slider->setMaximumAllowedValue(100);
    m_slider->setMinimumAllowedValue(0);
    m_pGiveBG1->addChild(m_slider);
    
    //背景音乐
    if(fscale == 0.75){
        
        //取得系统保存的音乐值
        m_slider->setValue(m_fMusic);
        
        m_slider->addTargetWithActionForControlEvents(this, cccontrol_selector(GameVolumeSet::changeValue), CCControlEventValueChanged);
        
        //音量为0
        if(!this->getfMusic()){
            //无声
            m_NoVoice = CCSprite::create("Image/BackPack/NoVoice.png");
            m_NoVoice->setPosition(ccp(m_pGiveBG1->getContentSize().width*0.9, m_pGiveBG1->getContentSize().height*fscale));
            m_pGiveBG1->addChild(m_NoVoice,1);
        }else{
        
            //背景音量的设置
            if(this->getfMusic() <= 33.3 ||(this->getfMusic() > 33.3 && this->getfMusic() <= 66.6) || (this->getfMusic() > 66.6))
            {
                if(m_VoiceSizeMusic)
                {
                    m_VoiceSizeMusic->removeFromParentAndCleanup(true);
                    m_VoiceSizeMusic = NULL;
                }
                
                if(m_VoiceSizeMusic2){
                    m_VoiceSizeMusic2->removeFromParentAndCleanup(true);
                    m_VoiceSizeMusic2 = NULL;
                }
                
                if(m_VoiceSizeMusic3){
                    m_VoiceSizeMusic3->removeFromParentAndCleanup(true);
                    m_VoiceSizeMusic3 = NULL;
                }
                
                m_VoiceSizeMusic = CCSprite::create("Image/BackPack/VoiceSize.png");
                m_VoiceSizeMusic->setScaleY(0.5);
                m_VoiceSizeMusic->setPosition(ccp(m_pGiveBG1->getContentSize().width*0.87, m_pGiveBG1->getContentSize().height*0.75));
                m_pGiveBG1->addChild(m_VoiceSizeMusic,1);
            }
            
            if( (this->getfMusic() > 33.3 && this->getfMusic() <= 66.6) || (this->getfMusic() > 66.6))
            {
                if(m_VoiceSizeMusic2)
                {
                    m_VoiceSizeMusic2->removeFromParentAndCleanup(true);
                    m_VoiceSizeMusic2 = NULL;
                }
                
                if(m_VoiceSizeMusic3)
                {
                    m_VoiceSizeMusic3->removeFromParentAndCleanup(true);
                    m_VoiceSizeMusic3 = NULL;
                }
                
                m_VoiceSizeMusic2 = CCSprite::create("Image/BackPack/VoiceSize.png");
                m_VoiceSizeMusic2->setScaleY(0.75);
                m_VoiceSizeMusic2->setPosition(ccp(m_pGiveBG1->getContentSize().width*0.9, m_pGiveBG1->getContentSize().height*0.75));
                m_pGiveBG1->addChild(m_VoiceSizeMusic2,2);
            }
            
            if(this->getfMusic() > 66.6)
            {
                if(m_VoiceSizeMusic3)
                {
                    m_VoiceSizeMusic3->removeFromParentAndCleanup(true);
                    m_VoiceSizeMusic3 = NULL;
                }
                
                m_VoiceSizeMusic3 = CCSprite::create("Image/BackPack/VoiceSize.png");
                m_VoiceSizeMusic3->setPosition(ccp(m_pGiveBG1->getContentSize().width*0.93, m_pGiveBG1->getContentSize().height*0.75));
                m_pGiveBG1->addChild(m_VoiceSizeMusic3,3);
            }
        }
    
    //背景音效
    }else{

        //取得系统保存的音效值
        m_slider->setValue(m_fSoundEffect);
        
        m_slider->addTargetWithActionForControlEvents(this, cccontrol_selector(GameVolumeSet::changeValue2), CCControlEventValueChanged);
        
        //音效为0
        if(!this->getfSoundEffect()){
            //无声
            m_NoVoice2 = CCSprite::create("Image/BackPack/NoVoice.png");
            m_NoVoice2->setPosition(ccp(m_pGiveBG1->getContentSize().width*0.9, m_pGiveBG1->getContentSize().height*fscale));
            m_pGiveBG1->addChild(m_NoVoice2,1);
        }else{
        
            //背景音效大小的设置
            if(this->getfSoundEffect() <= 33.3 ||(this->getfSoundEffect() > 33.3 && this->getfSoundEffect() <= 66.6) || (this->getfSoundEffect() > 66.6))
            {
                if(m_VoiceSizeYinxiao)
                {
                    m_VoiceSizeYinxiao->removeFromParentAndCleanup(true);
                    m_VoiceSizeYinxiao = NULL;
                }
                
                if(m_VoiceSizeYinxiao2){
                    m_VoiceSizeYinxiao2->removeFromParentAndCleanup(true);
                    m_VoiceSizeYinxiao2 = NULL;
                }
                
                if(m_VoiceSizeYinxiao3){
                    m_VoiceSizeYinxiao3->removeFromParentAndCleanup(true);
                    m_VoiceSizeYinxiao3 = NULL;
                }
                
                m_VoiceSizeYinxiao = CCSprite::create("Image/BackPack/VoiceSize.png");
                m_VoiceSizeYinxiao->setScaleY(0.5);
                m_VoiceSizeYinxiao->setPosition(ccp(m_pGiveBG1->getContentSize().width*0.87, m_pGiveBG1->getContentSize().height*0.4));
                m_pGiveBG1->addChild(m_VoiceSizeYinxiao,1);
            }
            
            if( (this->getfSoundEffect() > 33.3 && this->getfSoundEffect() <= 66.6) || (this->getfSoundEffect() > 66.6))
            {
                if(m_VoiceSizeYinxiao2)
                {
                    m_VoiceSizeYinxiao2->removeFromParentAndCleanup(true);
                    m_VoiceSizeYinxiao2 = NULL;
                }
                
                if(m_VoiceSizeYinxiao3)
                {
                    m_VoiceSizeYinxiao3->removeFromParentAndCleanup(true);
                    m_VoiceSizeYinxiao3 = NULL;
                }
                
                m_VoiceSizeYinxiao2 = CCSprite::create("Image/BackPack/VoiceSize.png");
                m_VoiceSizeYinxiao2->setScaleY(0.75);
                m_VoiceSizeYinxiao2->setPosition(ccp(m_pGiveBG1->getContentSize().width*0.9, m_pGiveBG1->getContentSize().height*0.4));
                m_pGiveBG1->addChild(m_VoiceSizeYinxiao2,2);
            }
            
            if(this->getfSoundEffect() > 66.6)
            {
                if(m_VoiceSizeYinxiao3)
                {
                    m_VoiceSizeYinxiao3->removeFromParentAndCleanup(true);
                    m_VoiceSizeYinxiao3 = NULL;
                }
                
                m_VoiceSizeYinxiao3 = CCSprite::create("Image/BackPack/VoiceSize.png");
                m_VoiceSizeYinxiao3->setPosition(ccp(m_pGiveBG1->getContentSize().width*0.93, m_pGiveBG1->getContentSize().height*0.4));
                m_pGiveBG1->addChild(m_VoiceSizeYinxiao3,3);
            }
        }
    
    }

    //音量图标
    m_VolumeIcon01Music = CCSprite::create("Image/BackPack/VolumeIcon01.png");
    m_VolumeIcon01Music->setPosition(ccp(m_pGiveBG1->getContentSize().width*0.82, m_pGiveBG1->getContentSize().height*fscale));
    m_pGiveBG1->addChild(m_VolumeIcon01Music);
    
}

//添加回调函数
void GameVolumeSet::changeValue(CCObject *sender, CCControlEvent controlEvent)
{

    CCControlSlider *pSlider = (CCControlSlider *)sender;
    
    //游戏音量大小保存
    this->setfMusic(pSlider->getValue());
    
    CLSoundManager::sharedSoundManager()->SetBgMusicVolume(pSlider->getValue());

    //调节音量为零时 显示无声
    if(!this->getfMusic())
    {
        if(m_NoVoice)
        {
            m_NoVoice->removeFromParentAndCleanup(true);
            m_NoVoice = NULL;
        }
        
        if(m_VoiceSizeMusic)
        {
            m_VoiceSizeMusic->removeFromParentAndCleanup(true);
            m_VoiceSizeMusic = NULL;
        }
        if(m_VoiceSizeMusic2)
        {
            m_VoiceSizeMusic2->removeFromParentAndCleanup(true);
            m_VoiceSizeMusic2 = NULL;
        }
        if(m_VoiceSizeMusic3)
        {
            m_VoiceSizeMusic3->removeFromParentAndCleanup(true);
            m_VoiceSizeMusic3 = NULL;
        }
        
        //无声 NoVoice.png
        m_NoVoice = CCSprite::create("Image/BackPack/NoVoice.png");
        m_NoVoice->setPosition(ccp(m_pGiveBG1->getContentSize().width*0.9, m_pGiveBG1->getContentSize().height*0.75));
        m_pGiveBG1->addChild(m_NoVoice);
    }else
    {
        if(m_NoVoice)
        {
            m_NoVoice->removeFromParentAndCleanup(true);
            m_NoVoice = NULL;
        }
        
        //背景音量大小的设置
        if(this->getfMusic() <= 33.3 ||(this->getfMusic() > 33.3 && this->getfMusic() <= 66.6) || (this->getfMusic() > 66.6))
        {
            if(m_VoiceSizeMusic)
            {
                m_VoiceSizeMusic->removeFromParentAndCleanup(true);
                m_VoiceSizeMusic = NULL;
            }
            
            if(m_VoiceSizeMusic2){
                m_VoiceSizeMusic2->removeFromParentAndCleanup(true);
                m_VoiceSizeMusic2 = NULL;
            }
            
            if(m_VoiceSizeMusic3){
                m_VoiceSizeMusic3->removeFromParentAndCleanup(true);
                m_VoiceSizeMusic3 = NULL;
            }
            
            m_VoiceSizeMusic = CCSprite::create("Image/BackPack/VoiceSize.png");
            m_VoiceSizeMusic->setScaleY(0.5);
            m_VoiceSizeMusic->setPosition(ccp(m_pGiveBG1->getContentSize().width*0.87, m_pGiveBG1->getContentSize().height*0.75));
            m_pGiveBG1->addChild(m_VoiceSizeMusic,1);
        }
        
        if( (this->getfMusic() > 33.3 && this->getfMusic() <= 66.6) || (this->getfMusic() > 66.6))
        {
            if(m_VoiceSizeMusic2)
            {
                m_VoiceSizeMusic2->removeFromParentAndCleanup(true);
                m_VoiceSizeMusic2 = NULL;
            }
            
            if(m_VoiceSizeMusic3)
            {
                m_VoiceSizeMusic3->removeFromParentAndCleanup(true);
                m_VoiceSizeMusic3 = NULL;
            }
            
            m_VoiceSizeMusic2 = CCSprite::create("Image/BackPack/VoiceSize.png");
            m_VoiceSizeMusic2->setScaleY(0.75);
            m_VoiceSizeMusic2->setPosition(ccp(m_pGiveBG1->getContentSize().width*0.9, m_pGiveBG1->getContentSize().height*0.75));
            m_pGiveBG1->addChild(m_VoiceSizeMusic2,2);
        }
        
        if(this->getfMusic() > 66.6)
        {
            if(m_VoiceSizeMusic3)
            {
                m_VoiceSizeMusic3->removeFromParentAndCleanup(true);
                m_VoiceSizeMusic3 = NULL;
            }
            
            m_VoiceSizeMusic3 = CCSprite::create("Image/BackPack/VoiceSize.png");
            m_VoiceSizeMusic3->setPosition(ccp(m_pGiveBG1->getContentSize().width*0.93, m_pGiveBG1->getContentSize().height*0.75));
            m_pGiveBG1->addChild(m_VoiceSizeMusic3,3);
        }
    }
    
}

//添加回调函数
void GameVolumeSet::changeValue2(CCObject *sender, CCControlEvent controlEvent)
{
    CCControlSlider *pSlider = (CCControlSlider *)sender;
    
    //游戏音效大小保存
    this->setfSoundEffect(pSlider->getValue());
    
    CLSoundManager::sharedSoundManager()->SetEffectVolume(pSlider->getValue());
    
    //调节音效为零时 显示无声
    if(!this->getfSoundEffect())
    {
        if(m_NoVoice2)
        {
            m_NoVoice2->removeFromParentAndCleanup(true);
            m_NoVoice2 = NULL;
        }
        
        if(m_VoiceSizeYinxiao)
        {
            m_VoiceSizeYinxiao->removeFromParentAndCleanup(true);
            m_VoiceSizeYinxiao = NULL;
        }
        if(m_VoiceSizeYinxiao2)
        {
            m_VoiceSizeYinxiao2->removeFromParentAndCleanup(true);
            m_VoiceSizeYinxiao2 = NULL;
        }
        if(m_VoiceSizeYinxiao3)
        {
            m_VoiceSizeYinxiao3->removeFromParentAndCleanup(true);
            m_VoiceSizeYinxiao3 = NULL;
        }
        
        //无声 NoVoice.png
        m_NoVoice2 = CCSprite::create("Image/BackPack/NoVoice.png");
        m_NoVoice2->setPosition(ccp(m_pGiveBG1->getContentSize().width*0.9, m_pGiveBG1->getContentSize().height*0.4));
        m_pGiveBG1->addChild(m_NoVoice2);
    }else
    {
        if(m_NoVoice2)
        {
            m_NoVoice2->removeFromParentAndCleanup(true);
            m_NoVoice2 = NULL;
        }
        
        //音效大小的设置
        if(this->getfSoundEffect() <= 33.3 ||(this->getfSoundEffect() > 33.3 && this->getfSoundEffect() <= 66.6) || (this->getfSoundEffect() > 66.6))
        {
            if(m_VoiceSizeYinxiao)
            {
                m_VoiceSizeYinxiao->removeFromParentAndCleanup(true);
                m_VoiceSizeYinxiao = NULL;
            }
            
            if(m_VoiceSizeYinxiao2){
                m_VoiceSizeYinxiao2->removeFromParentAndCleanup(true);
                m_VoiceSizeYinxiao2 = NULL;
            }
            
            if(m_VoiceSizeYinxiao3){
                m_VoiceSizeYinxiao3->removeFromParentAndCleanup(true);
                m_VoiceSizeYinxiao3 = NULL;
            }
            
            m_VoiceSizeYinxiao = CCSprite::create("Image/BackPack/VoiceSize.png");
            m_VoiceSizeYinxiao->setScaleY(0.5);
            m_VoiceSizeYinxiao->setPosition(ccp(m_pGiveBG1->getContentSize().width*0.87, m_pGiveBG1->getContentSize().height*0.4));
            m_pGiveBG1->addChild(m_VoiceSizeYinxiao,1);
        }
        
        if( (this->getfSoundEffect() > 33.3 && this->getfSoundEffect() <= 66.6) || (this->getfSoundEffect() > 66.6))
        {
            if(m_VoiceSizeYinxiao2)
            {
                m_VoiceSizeYinxiao2->removeFromParentAndCleanup(true);
                m_VoiceSizeYinxiao2 = NULL;
            }
            
            if(m_VoiceSizeYinxiao3)
            {
                m_VoiceSizeYinxiao3->removeFromParentAndCleanup(true);
                m_VoiceSizeYinxiao3 = NULL;
            }
            
            m_VoiceSizeYinxiao2 = CCSprite::create("Image/BackPack/VoiceSize.png");
            m_VoiceSizeYinxiao2->setScaleY(0.75);
            m_VoiceSizeYinxiao2->setPosition(ccp(m_pGiveBG1->getContentSize().width*0.9, m_pGiveBG1->getContentSize().height*0.4));
            m_pGiveBG1->addChild(m_VoiceSizeYinxiao2,2);
        }
        
        if(this->getfSoundEffect() > 66.6)
        {
            if(m_VoiceSizeYinxiao3)
            {
                m_VoiceSizeYinxiao3->removeFromParentAndCleanup(true);
                m_VoiceSizeYinxiao3 = NULL;
            }
            
            m_VoiceSizeYinxiao3 = CCSprite::create("Image/BackPack/VoiceSize.png");
            m_VoiceSizeYinxiao3->setPosition(ccp(m_pGiveBG1->getContentSize().width*0.93, m_pGiveBG1->getContentSize().height*0.4));
            m_pGiveBG1->addChild(m_VoiceSizeYinxiao3,3);
        }
    }
    
}


void GameVolumeSet::CanceBtn()
{
    doAction(this, false);
    
    //窗体开闭音乐
    CLSoundManager::sharedSoundManager()->PlayEffectSound("Sound/Sl_WInClose.wav");
    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);
}

void GameVolumeSet::AffirmBtn()
{
    doAction(this, false);
    
    //游戏音量
    this->getfMusic();
    //游戏音效
    this->getfSoundEffect();
    
    CCLog("游戏音量：%d 游戏音效：%d",this->getfMusic(),this->getfSoundEffect());
    
    //确定按钮按钮下的时 把音乐音效的值保存到 本地json中
    Json::Value value;
    value["bgMusic"] = this->getfMusic();
    value["bgEfect"] =  this->getfSoundEffect();
    JsonDataManager::shardJDManager()->SetMusicInfo(value);
  
    
    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);
    
}

void GameVolumeSet::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);
}

void GameVolumeSet::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool GameVolumeSet::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    return true;
}


void GameVolumeSet::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void GameVolumeSet::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
