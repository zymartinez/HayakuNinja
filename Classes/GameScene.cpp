/*
 * GameScene.cpp
 *
 *  Created on: Jan 5, 2013
 *      Author: jaunesarmiento
 */

#include "Globals.h"
#include "SimpleAudioEngine.h"

#include "GameScene.h"
#include "MainMenuScene.h"
#include "Player.h"
#include "HudLayer.h"

#include <stack>

USING_NS_CC;

using namespace std;
using namespace CocosDenshion;

CCScene* GameScene::scene()
{
    CCScene *scene = CCScene::create();

    GameScene *layer = GameScene::create();

    scene->addChild(layer);

    return scene;
}

// This gets called on GameScene::create() or new GameScene
bool GameScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    _numOfTouches = 0;
    setTouchEnabled(true);


    /*
     * Initialize Player
     */
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Images/Player_default.plist");
	CCSpriteBatchNode* spriteBatchNode = CCSpriteBatchNode::create("Images/Player_default.png", 11);
	this->addChild(spriteBatchNode);

    _player = Player::player();
    _player->idle();
    this->addChild(_player);



    /*
     * Initialize HudLayer
     */
    HudLayer* hudLayer = HudLayer::create();
    this->addChild(hudLayer, 20);

    hudLayer->updateLives(3);







	// Play BGM
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("Sounds/foogies.wav")).c_str(), true);

    return true;
}


void GameScene::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
}

void GameScene::ccTouchesBegan(CCSet *touches, CCEvent *event)
{
	_numOfTouches++;

	std::stringstream ss;
	ss << "Touches: " << _numOfTouches;

	CCLog(ss.str().c_str());

	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);

	int direction;

	// -1 is going left, 1 is going right
	if (location.x <= (SCREEN.width / 2)){
		direction = -1;
	}
	else {
		direction = 1;
	}

	_directionStack.push(direction);

	_player->_direction = direction;
	_player->movePlayerWithDirection(direction);
}

void GameScene::ccTouchesEnded(CCSet *touches, CCEvent *event)
{
	_numOfTouches--;

	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);

	std::stringstream ss;
	ss << "Touch ended with " << _numOfTouches << " touches";

	_directionStack.pop();

	CCLog(ss.str().c_str());

	if (_directionStack.size() == 0 || _numOfTouches == 0) {
		_player->idle();
		_player->_direction = 0;
	}

}
