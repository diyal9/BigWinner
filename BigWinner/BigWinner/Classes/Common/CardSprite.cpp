//
//  CardSprite.cpp
//  BigWinner
//
//  Created by LunaBox on 13-11-29.
//
//

#include "CardSprite.h"
#include "LGameCommonDefine.h"
#include "JsonDataManager.h"

CardSprite* CardSprite::create(int No)
{
    CardSprite *pRet = new CardSprite();
	if (pRet && pRet->init(No))
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

bool CardSprite::init(int No)
{
    //题卡类型  1 题卡  2 道具 3 角色
    int nArtType = 0;
    
    nArtType = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artType;
    
    //题卡Sprite生成 No 1~45
//    if(No>=1 && No<=45){
    if(nArtType == 1){
        //题卡背景图片
        CCSprite *spriteQuestionCard = CCSprite::create("Image/BackPack/CardBackground.png");
        this->addChild(spriteQuestionCard,0,10);
        
        //道具IconId
        int ArtIconId = 0;
        ArtIconId = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artIconId;
        
//        //商品名称路径
//        std::string RCardName_path = CCString::createWithFormat("Image/BackPack/CardName%d.png", ArtIconId)->getCString();
        
        //商品图标路径
        std::string RCardNameIcon_path = CCString::createWithFormat("Image/BackPack/CardIcon%d.png", ArtIconId)->getCString();
        
//        //题卡文字
//        CCSprite *pRightTitle = CCSprite::create(RCardName_path.c_str());
//        pRightTitle->setPosition(ccp(spriteQuestionCard->getContentSize().width - pRightTitle->getContentSize().width*0.5 -G_SF(10), spriteQuestionCard->getContentSize().height - pRightTitle->getContentSize().height*0.5 -G_SF(15)));
//        spriteQuestionCard->addChild(pRightTitle);
        
        //Icon
        CCSprite *pRightIcon = CCSprite::create(RCardNameIcon_path.c_str());
        pRightIcon->setPosition(ccp(spriteQuestionCard->getContentSize().width*0.5, spriteQuestionCard->getContentSize().height*0.5));
        spriteQuestionCard->addChild(pRightIcon);
        
        int Star = 0;
        Star = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artQStarLevel;
        
        //左边题卡星数
        for(int i = 1 ; i <= Star;i++)
        {
            //卡牌星星之间的距离
//            CCSprite *spriteStartLeft = CCSprite::create("Image/BackPack/C_card_star.png");
//            spriteStartLeft->setPosition( ccp(G_SF(20) + spriteStartLeft->getContentSize().width * 0.5 * ((i-1)*2 + 1), spriteQuestionCard->getContentSize().height - G_SF(25)));
//            spriteQuestionCard->addChild(spriteStartLeft);
            CCSprite *spriteStartLeft = CCSprite::create("Image/BackPack/C_card_star.png");
            spriteStartLeft->setPosition( ccp(G_SF(50) + spriteStartLeft->getContentSize().width * 0.5 * ((i-1)*2 + 1), pRightIcon->getContentSize().height - G_SF(55)));
            pRightIcon->addChild(spriteStartLeft);
        }
    }
    //道具Sprite生成 No 1~45
//    else if( No >= 46 && No <=51)
    else if( nArtType == 2)
    {
        //左边道具图片
        CCSprite *spritePropsUI = CCSprite::create("Image/BackPack/PropsBackground.png");
        this->addChild(spritePropsUI,0,10);
        
        //道具IconId
        int ArtIconId = 0;
        ArtIconId = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artIconId;
        
        //道具Icon路径
        std::string LCardName_path = CCString::createWithFormat("Image/BackPack/Props%d.png", ArtIconId)->getCString();
        
        //道具Icon
        CCSprite *PropsIcon = CCSprite::create(LCardName_path.c_str());
        PropsIcon->setPosition(ccp(spritePropsUI->getContentSize().width*0.5, spritePropsUI->getContentSize().height*0.5));
        spritePropsUI->addChild(PropsIcon);
        
    }
    //角色Sprite生成 No 1~45
//    else if( No >= 52 && No <=59)
    else if( nArtType == 3)
    {
        //左边角色图片
        CCSprite *spriteRole = CCSprite::create("Image/BackPack/RoleBackground.png");
        this->addChild(spriteRole, 0, 10);
        
        //人物IconId
        int ArtIconId = 0;
        ArtIconId = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artIconId;
        
        //人物名称路径
        std::string RoleName_path = CCString::createWithFormat("Image/BackPack/RoleName%d.png", ArtIconId)->getCString();
        
        CCSprite *PropsIcon = CCSprite::create(RoleName_path.c_str());
        PropsIcon->setPosition(ccp(G_SF(110), G_SF(105)));
        spriteRole->addChild(PropsIcon);
        
    }
    //货币Sprite生成 No 1~45
    else if( No >= 60 && No <=67)
    {
        //左边货币图片
        CCSprite *spriteCurrency = CCSprite::create("Image/BackPack/CurrencyBackground.png");
        this->addChild(spriteCurrency,0,10);
        
        //道具IconId
        int ArtIconId = 0;
        ArtIconId = JsonDataManager::shardJDManager()->m_vCardJueseDaoju[No]->m_artNo;
        
        //道具名称路径
        std::string CurrencyName_path = CCString::createWithFormat("Image/BackPack/CurrencyIcon%d.png", ArtIconId)->getCString();
        
        //道具Icon
        CCSprite *PropsIcon = CCSprite::create(CurrencyName_path.c_str());
        PropsIcon->setPosition(ccp(spriteCurrency->getContentSize().width*0.5, spriteCurrency->getContentSize().height*0.5));
        spriteCurrency->addChild(PropsIcon);
        
    }
    else
    {
        return false;
    }
    
    
    return true;
}