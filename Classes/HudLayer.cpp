/*
 * HudLayer.cpp
 *
 *  Created on: Jan 5, 2013
 *      Author: jaunesarmiento
 */

#include "Globals.h"
#include "SimpleAudioEngine.h"
#include "HudLayer.h"
#include "MainMenuScene.h"


USING_NS_CC;

using namespace std;
using namespace CocosDenshion;

bool HudLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    /*
     * HUD Labels
     */

    _labelLives = CCLabelTTF::create("x10", "Fonts/visitor1.ttf", 72, CCSizeMake(200, 100), kCCTextAlignmentLeft);
    _labelCakes = CCLabelTTF::create("x0000", "Fonts/visitor1.ttf", 72, CCSizeMake(300, 100), kCCTextAlignmentLeft);
    _labelCoins = CCLabelTTF::create("x0000", "Fonts/visitor1.ttf", 72, CCSizeMake(300, 100), kCCTextAlignmentLeft);

	CCSprite* gameHud = CCSprite::create("Images/Hud.png");
	gameHud->setPosition(ccp(SCREEN.width / 2, SCREEN.height - gameHud->getContentSize().height / 2));
	gameHud->addChild(_labelLives);
	gameHud->addChild(_labelCakes);
	gameHud->addChild(_labelCoins);

	_labelLives->setPosition(ccp(220, (gameHud->getContentSize().height / 2) - 10));
	_labelCakes->setPosition(ccp(570, (gameHud->getContentSize().height / 2) - 10));
	_labelCoins->setPosition(ccp(960, (gameHud->getContentSize().height / 2) - 10));


	/*
	 * Pause Overlay
	 */
	// Pause Overlay Layer
	_pauseOverlay = CCLayerColor::create();
	_pauseOverlay->setPosition(CCPointZero);
	_pauseOverlay->setColor(ccc3(0, 0, 0));
	_pauseOverlay->setOpacity((GLubyte)lroundf(200));

	CCLabelTTF* labelPaused = CCLabelTTF::create("Game Paused", "Fonts/visitor1.ttf", 36);
	labelPaused->setPosition(ccp(SCREEN.width / 2, SCREEN.height - 50));

	CCMenuItemFont::setFontName("Fonts/visitor1.ttf");
	CCMenuItemFont::setFontSize(72);
	CCMenuItemFont* menuItemResume = CCMenuItemFont::create("Resume", this, menu_selector(HudLayer::onResume));
	CCMenuItemFont* menuItemQuit = CCMenuItemFont::create("Quit Game", this, menu_selector(HudLayer::onQuit));

	menuItemResume->setPosition(ccp(SCREEN.width / 2, 300));
	menuItemQuit->setPosition(ccp(SCREEN.width / 2, 150));

	CCMenu* menuPaused = CCMenu::create(menuItemResume, menuItemQuit, NULL);
	menuPaused->setPosition(CCPointZero);

	_pauseOverlay->addChild(labelPaused);
	_pauseOverlay->addChild(menuPaused, 1);
	_pauseOverlay->setVisible(false);

	// Attach the pause overlay to the HudLayer
	this->addChild(_pauseOverlay, 2);

	// Pause Button
	CCMenuItemImage* menuItemPause = CCMenuItemImage::create(
										"Images/ButtonPause.png",
										"Images/ButtonPausePressed.png",
										this,
										menu_selector(HudLayer::onPause));


	menuItemPause->setPosition(ccp(SCREEN.width - 48, SCREEN.height - 42));

	menuPause = CCMenu::create(menuItemPause, NULL);
	menuPause->setPosition(CCPointZero);

	// Attach the pause button to the HudLayer
	this->addChild(menuPause, 1);



	/*
	 * Tool Pane Overlay
	 */

	CCSprite* toolPane = CCSprite::create("Images/HudToolPane.png");
	toolPane->setPosition(ccp(SCREEN.width - toolPane->getContentSize().width / 2, SCREEN.height / 2));

	CCMenuItemImage *menuItemCloseToolPane = CCMenuItemImage::create(
											"Images/ButtonCloseToolPane.png",
											"Images/ButtonCloseToolPanePressed.png",
											this,
											menu_selector(HudLayer::onCloseToolPane));


	menuItemCloseToolPane->setPosition(ccp(toolPane->getPositionX(), SCREEN.height - 42));

	// Todo: Add other tools here


	CCMenu* menuToolPane = CCMenu::create(menuItemCloseToolPane, NULL);
	menuToolPane->setPosition(CCPointZero);


	_toolPaneOverlay = CCLayerColor::create();
	_toolPaneOverlay->setPosition(CCPointZero);
	_toolPaneOverlay->setColor(ccc3(0, 0, 0));
	_toolPaneOverlay->setOpacity((GLubyte)lroundf(80));

	_toolPaneOverlay->addChild(toolPane, 0);
	_toolPaneOverlay->addChild(menuToolPane, 1);

	this->addChild(_toolPaneOverlay, 3);


	// Attach gameHud to the HudLayer
	this->addChild(gameHud, 0);

    return true;
}

void HudLayer::updateLives(int value)
{
	std::stringstream cc;
	if (value < 10) {
		cc << "0" << value;
	}
	else {
		cc << value;
	}

	stringstream buffer;
	buffer << "x" << cc.str().c_str();
	this->_labelLives->setString( buffer.str().c_str() );
}

void HudLayer::updateCakes(int value)
{
	std::stringstream cc;
	if (value < 10) {
		cc << "000" << value;
	}
	else if (value < 100){
		cc << "00" << value;
	}
	else if (value < 1000)
	{
		cc << "0" << value;
	}
	else {
		cc << value;
	}

	stringstream buffer;
	buffer << "x" << cc.str().c_str();
	this->_labelCakes->setString( buffer.str().c_str() );
}

void HudLayer::updateCoins(int value)
{
	std::stringstream cc;
	if (value < 10) {
		cc << "000" << value;
	}
	else if (value < 100){
		cc << "00" << value;
	}
	else if (value < 1000)
	{
		cc << "0" << value;
	}
	else {
		cc << value;
	}

	stringstream buffer;
	buffer << "x" << cc.str().c_str();
	this->_labelCoins->setString( buffer.str().c_str() );
}

void HudLayer::onPause(CCObject* pSender)
{
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.05);
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.1);

	_pauseOverlay->setVisible(true);
	CCDirector::sharedDirector()->pause();
}

void HudLayer::onResume(CCObject* pSender)
{
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.2);
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5);

	_pauseOverlay->setVisible(false);
	CCDirector::sharedDirector()->resume();
}

void HudLayer::onQuit(CCObject* pSender)
{
	_pauseOverlay->setVisible(false);
	CCDirector::sharedDirector()->resume();

	CCScene* mainMenuScene = MainMenuScene::scene();
	CCTransitionScene *pauseToMenu = CCTransitionFade::create(1.0f, mainMenuScene);
	CCDirector::sharedDirector()->replaceScene(pauseToMenu);
}

void HudLayer::onOpenToolPane(CCObject* pSender)
{
	actionOpenToolPane = CCMoveTo::create( 0.25, ccp(_toolPaneOverlay->getPositionX() - 180, 0) );
	_toolPaneOverlay->runAction(actionOpenToolPane);

	menuPause->setEnabled(false);
	CCLog("Called onOpenToolPane");
}

void HudLayer::onCloseToolPane(CCObject* pSender)
{
	actionCloseToolPane = CCMoveTo::create( 0.25, ccp(_toolPaneOverlay->getPositionX() + 180, 0) );
	_toolPaneOverlay->runAction(actionCloseToolPane);

	menuPause->setEnabled(true);
	CCLog("Called onCloseToolPane");
}
