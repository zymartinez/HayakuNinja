/*
 * LevelSelect.cpp
 *
 *  Created on: Jan 13, 2013
 *      Author: jaunesarmiento
 */

#include "Globals.h"
#include "SimpleAudioEngine.h"

#include "LevelSelect.h"
#include "MainMenuScene.h"
#include "GameScene.h"

USING_NS_CC;

using namespace std;
using namespace CocosDenshion;

CCScene* LevelSelect::scene()
{
    CCScene *scene = CCScene::create();

    LevelSelect *layer = LevelSelect::create();

    scene->addChild(layer);

    return scene;
}

bool LevelSelect::init() {
    bool returnbool = false;

    do{

        CC_BREAK_IF( !CCLayerColor::initWithColor( ccc4(255,0,0,255) ) );

        if ( CCLayer::init() ) {
            CCLabelTTF *zy = CCLabelTTF::create("ZYZYZYZYZYZY","Arial",20.0);
            zy->setPosition( ccp(zy->getContentSize().width/2, SCREEN.height/2) );
            this->addChild(zy);

            CCSprite *bgMap = CCSprite::create("Images/BackgroundMap.png");
            bgMap->setPosition(ccp( SCREEN.width / 2, SCREEN.height - (bgMap->getContentSize().height / 2) ));
            this->addChild(bgMap, 0);

            CCMenuItemImage *pCliff = CCMenuItemImage::create(
                                                "Images/Level1Cliff.png",
                                                "Images/Level1CliffPressed.png",
                                                this,
                                                menu_selector(LevelSelect::menuLevel1Callback));

            CCMenuItemImage *pWarehouse = CCMenuItemImage::create(
                                                "Images/Level1Warehouse.png",
                                                "Images/Level1WarehousePressed.png",
                                                this,
                                                menu_selector(LevelSelect::menuLevel2Callback));

            CCMenuItemImage *pTemple = CCMenuItemImage::create(
                                                "Images/Level1Temple.png",
                                                "Images/Level1TemplePressed.png",
                                                this,
                                                menu_selector(LevelSelect::menuLevel3Callback));

            CCMenuItemImage *pCastle = CCMenuItemImage::create(
                                                "Images/Level1Castle.png",
                                                "Images/Level1CastlePressed.png",
                                                this,
                                                menu_selector(LevelSelect::menuLevel4Callback));

            pCliff->setPosition(340,SCREEN.height-220);
            pWarehouse->setPosition(690,SCREEN.height-285);
            pTemple->setPosition(484,SCREEN.height-460);
            pCastle->setPosition(949,SCREEN.height-485);

            CCMenu* lvlMenu = CCMenu::create(pCliff, pWarehouse, pTemple, pCastle, NULL);
            lvlMenu->setPosition(CCPointZero);

            this->addChild(lvlMenu, 1);
        }

        returnbool=true;

    }while (0);


    return returnbool;

}

void LevelSelect::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
    #endif
}

// void LevelSelect::goToLevel1() {
//     CCScene *level1 = GameScene::scene();
//     CCTransitionScene *menuToLevel1 = CCTransitionFade::create(1.0f, level1);
//     CCDirector::sharedDirector()->replaceScene(menuToLevel1);
// }

void LevelSelect::menuLevel1Callback() {
	CCUserDefault::sharedUserDefault()->setIntegerForKey("level",1);
    CCScene *level1 = GameScene::scene();
    CCTransitionScene *menuToLevel1 = CCTransitionFade::create(1.0f, level1);
    CCDirector::sharedDirector()->replaceScene(menuToLevel1);
}

void LevelSelect::menuLevel2Callback() {
	CCUserDefault::sharedUserDefault()->setIntegerForKey("level",2);
    CCScene *level1 = GameScene::scene();
    CCTransitionScene *menuToLevel1 = CCTransitionFade::create(1.0f, level1);
    CCDirector::sharedDirector()->replaceScene(menuToLevel1);
}

void LevelSelect::menuLevel3Callback() {

}

void LevelSelect::menuLevel4Callback() {

}
