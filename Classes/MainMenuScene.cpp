/*
 * MainMenuScene.cpp
 *
 *  Created on: Jan 5, 2013
 *      Author: jaunesarmiento
 */

#include "Globals.h"
#include "SimpleAudioEngine.h"

#include "MainMenuScene.h"
#include "GameScene.h"

USING_NS_CC;

using namespace CocosDenshion;

CCScene* MainMenuScene::scene()
{
	CCScene *scene = CCScene::create();

	MainMenuScene *layer = MainMenuScene::create();

	scene->addChild(layer);

	return scene;
}


bool MainMenuScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    setTouchEnabled(true);

	CCSprite* background = CCSprite::create("Images/BackgroundMainMenu.png");
	background->setPosition(ccp( SCREEN.width / 2, SCREEN.height - (background->getContentSize().height / 2) ));
	this->addChild(background, 0);


	CCMenuItemImage *pPlay = CCMenuItemImage::create(
										"Images/ButtonPlay.png",
										"Images/ButtonPlayPressed.png",
										this,
										menu_selector(MainMenuScene::menuPlayCallback));

	CCMenuItemImage *pToolShop = CCMenuItemImage::create(
											"Images/ButtonToolShop.png",
											"Images/ButtonToolShopPressed.png",
											this,
											menu_selector(MainMenuScene::menuToolShopCallback));

	CCMenuItemImage *pOptions = CCMenuItemImage::create(
											"Images/ButtonOptions.png",
											"Images/ButtonOptionsPressed.png",
											this,
											menu_selector(MainMenuScene::menuOptionsCallback));

	CCMenuItemImage *pExit = CCMenuItemImage::create(
											"Images/ButtonExit.png",
											"Images/ButtonExitPressed.png",
											this,
											menu_selector(MainMenuScene::menuExitCallback));

	pPlay->setPosition(ccp(SCREEN.width - 250, SCREEN.height - 150));
	pToolShop->setPosition(ccp(SCREEN.width - 250, SCREEN.height - 300));
	pOptions->setPosition(ccp(SCREEN.width - 250, SCREEN.height - 450));
	pExit->setPosition(ccp(SCREEN.width - 250, SCREEN.height - 600));

	CCMenu* menu = CCMenu::create(pPlay, pToolShop, pOptions, pExit, NULL);
	menu->setPosition(CCPointZero);

	this->addChild(menu, 1);

	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("Sounds/foogies.wav")).c_str(), true);

    return true;
}

void MainMenuScene::menuPlayCallback(CCObject* pSender)
{
	CCScene *gameScene = GameScene::scene();
	CCTransitionScene *menuToGame = CCTransitionFade::create(1.0f, gameScene);
	CCDirector::sharedDirector()->replaceScene(menuToGame);
}

void MainMenuScene::menuToolShopCallback(CCObject* pSender)
{
	CCLog("Called menuToolShopCallback");
}

void MainMenuScene::menuOptionsCallback(CCObject* pSender)
{
	CCLog("Called menuOptionsCallback");
}

void MainMenuScene::menuExitCallback(CCObject* pSender)
{
	CCLog("Called menuOptionsCallback");

    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
