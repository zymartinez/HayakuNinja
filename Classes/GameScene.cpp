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
#include "HudLayer.h"

#include "Player.h"

USING_NS_CC;

using namespace std;
using namespace CocosDenshion;


static std::string cakeList[] =
{
    "CakeChoco.png",
    "CakeStrawberry.png",
};

static std::string powerUpList[] =
{
    "PrayerSeal.png"
};

static std::string obstacleList[] =
{
    "ObstacleBomb.png",
    "ObstacleTrap.png",
};

static int cakeCount = sizeof(cakeList) / sizeof(*cakeList);
static int powerUpCount = sizeof(powerUpList) / sizeof(*powerUpList);
static int obstacleCount = sizeof(obstacleList) / sizeof(*obstacleList);


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

    /*
     * Initialize game variables
     */
    this->_comboCount = 0;
    this->_gameSpeed = 1.0f;
    this->_numOfTouches = 0;

    // Must load these from file
    this->_totalLife = 3;
    this->_totalCakes = 0;
    this->_totalCoins = 0;


    setTouchEnabled(true);


    CCSprite* bg = CCSprite::create("Images/DummyBG.png");
    bg->setPosition(CENTER);
    this->addChild(bg, -1);

    /*
     * Initialize Player
     */
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Images/Player_default.plist");
	CCSpriteBatchNode* spriteBatchNode = CCSpriteBatchNode::create("Images/Player_default.png", 15);
	this->addChild(spriteBatchNode);

    _player = Player::player();
    _player->idle();
    this->addChild(_player);

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Images/AnimatedObjects_default.plist");
    CCSpriteBatchNode* animatedSpriteBatchNode = CCSpriteBatchNode::create("Images/AnimatedObjects_default.png", 6);
    this->addChild(animatedSpriteBatchNode);


    /*
     * Initialize Poof animation of player
     */
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Images/Smoke_s.plist");
	CCSpriteBatchNode* smokeSpriteBatchNode = CCSpriteBatchNode::create("Images/Smoke_s.png", 12);
	this->addChild(smokeSpriteBatchNode);

	CCAnimation* playerPoof = CCAnimation::create();

	for (int i = 0 ; i < 12 ; ++i )
	{
		std::stringstream cc;
		if (i < 9) {
			cc << "0" << i + 1;
		}
		else {
			cc << i + 1;
		}

		CCLog(cc.str().c_str());

		std::stringstream ss;
		ss << "Smoke" << cc.str().c_str()  << ".png";

		std::string name = ss.str();
		CCSpriteFrame* sprite =  CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name.c_str());
		playerPoof->addSpriteFrame(sprite);
	}

	playerPoof->setDelayPerUnit(0.075f);

	_poofAnimAction = CCAnimate::create(playerPoof);
	_poofAnimAction->retain();



	/*
	 * Initialize notices
	 */
	_noticeGameSpeedDouble = CCSprite::create("Images/GameSpeedDouble.png");
	_noticeHasteMode = CCSprite::create("Images/GameModeHaste.png");

	_noticeGameSpeedDouble->setOpacity((GLubyte)lroundf(0));
	_noticeHasteMode->setOpacity((GLubyte)lroundf(0));

	_noticeGameSpeedDouble->setPosition(ccp(SCREEN.width / 2, SCREEN.height - _noticeGameSpeedDouble->getContentSize().height / 2));
	_noticeHasteMode->setPosition(ccp(SCREEN.width / 2, SCREEN.height - _noticeHasteMode->getContentSize().height / 2));

	this->addChild(_noticeGameSpeedDouble, 3);
	this->addChild(_noticeHasteMode, 3);



    /*
     * Initialize HudLayer
     */
    _hudLayer = HudLayer::create();
    this->addChild(_hudLayer, 20);

    _hudLayer->updateLives(this->_totalLife);






    /*
     * Initialize arrays
     */
    _coins = CCArray::create();
	_coins->retain();

	_cakes = CCArray::create();
	_cakes->retain();

	_powerUps = CCArray::create();
	_powerUps->retain();

	_obstacles = CCArray::create();
	_obstacles->retain();



	// Play BGM
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("Sounds/foogies.wav")).c_str(), true);


    this->schedule( schedule_selector(GameScene::updateGameSpeed), 30 );
    this->schedule( schedule_selector(GameScene::gameLogic), 1 );
    this->schedule( schedule_selector(GameScene::update) );

    return true;
}


void GameScene::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
}

void GameScene::ccTouchesBegan(CCSet *touches, CCEvent *event)
{
	_numOfTouches++;

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

	_player->_direction = direction;
	_player->movePlayerWithDirection(direction);
}

void GameScene::ccTouchesEnded(CCSet *touches, CCEvent *event)
{
	_numOfTouches--;

	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);

	if (_numOfTouches == 0) {
		_player->idle();
		_player->_direction = 0;
	}
}

void GameScene::gameLogic(float dt)
{
	int min = 1;
	int max = 1000;
	int range = max - min;
	int algo = (rand() % range) + min;

	if (algo <= 5) {
		this->dropItem(0); // Drop coin
	}
	else if (algo > 5 && algo <= 300){
		this->dropItem(1); // Drop obstacle
	}
	else if (algo > 300 && algo <= 950) {
		this->dropItem(2); // Drop cake
	}
	else if (algo > 950){
		this->dropItem(3); // Drop power up
	}
}


void GameScene::dropItem(int itemType)
{
	CCString frameName;

	switch (itemType) {
		case 0 : frameName = "Noin.png"; break;
		case 1 : frameName = obstacleList[ (rand() % obstacleCount) ].c_str(); break;
		case 2 : frameName = cakeList[ (rand() % cakeCount) ].c_str(); break;
		case 3 : frameName = "PrayerSeal.png"; break;
		default: frameName = "CakeChoco.png"; break;
	}

	CCSprite* item = CCSprite::createWithSpriteFrameName(frameName.getCString());
	item->setRotation((rand() % 360));
	item->setTag(itemType);

	if (itemType == 1 || itemType == 2) {
		// Randomly flip item on X axis
		if ( rand() % 2 == 0 ) {
			item->setFlipX(true);
		}
		else if ( rand() % 2 == 1) {
			item->setFlipY(true);
		}
		else {
			// no flip
		}
	}

	if (itemType == 0) this->_coins->addObject(item);
	if (itemType == 1) this->_obstacles->addObject(item);
	if (itemType == 2) this->_cakes->addObject(item);
	if (itemType == 3) this->_powerUps->addObject(item);


	int minX = item->getContentSize().width / 2;
	int maxX = SCREEN.width - item->getContentSize().width / 2;
	int rangeX = maxX - minX;
	int actualX = ( rand() % rangeX ) + minX;

	// Create the item slightly off-screen along the right edge,
	// and along a random position along the X axis as calculated
	item->setPosition( ccp(actualX, SCREEN.height + (item->getContentSize().height/2)) );

	this->addChild(item, 4);

	// Determine speed of the item
	int minDuration = 3;
	int maxDuration = 4;
	int rangeDuration = maxDuration - minDuration;
	float actualDuration = (( rand() % rangeDuration ) + minDuration) * this->_gameSpeed;

	stringstream buffer;
	buffer << "actualDuration: " << actualDuration;
	CCLog( buffer.str().c_str() );

	// Create the actions
	CCActionInterval* actionRotate = CCRotateBy::create((float)actualDuration, 360);
	CCActionInterval* actionMove = CCMoveTo::create( (float)actualDuration, ccp(actualX, item->getContentSize().height / 2) );

	CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create( this, callfuncN_selector(GameScene::spriteMoveFinished));

	item->runAction(actionRotate);
	item->runAction(CCSequence::actions(actionMove, actionMoveDone, NULL));

}

void GameScene::spriteMoveFinished(CCNode* sender)
{
	CCSprite *sprite = (CCSprite *)sender;
	this->removeChild(sprite, true);

	if (sprite->getTag() == 0) // coins
	{
		this->_coins->removeObject(sprite);
	}
	if (sprite->getTag() == 1)  // obstacles
	{
		this->_obstacles->removeObject(sprite);
	}
	else if (sprite->getTag() == 2) // cakes
	{
		if (this->_comboCount > 1) {
			CCLabelTTF* label = CCLabelTTF::create("C-C-C-Combo Breaker", "Fonts/visitor1.ttf", 36, CCSizeMake(500, 30), kCCTextAlignmentLeft);
			label->setPosition(ccpAdd(sprite->getPosition(), ccp(0, -30)));
			this->addChild(label, 4);

			CCActionInterval* actionMove = CCMoveTo::create( 2.1f, ccp(label->getPositionX(), label->getPositionY() + 100) );
			CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create( this, callfuncN_selector(GameScene::labelMoveFinished));

			label->runAction(CCFadeOut::actionWithDuration(2.f));
			label->runAction(CCSequence::actions(actionMove, actionMoveDone, NULL));
		}

		// Reset combo
		this->_comboCount = 0;

		this->_cakes->removeObject(sprite);
	}
	else if (sprite->getTag() == 3) // power ups
	{
		this->_powerUps->removeObject(sprite);
	}
}

void GameScene::updateGameSpeed(float dt)
{
	CCLog("Updating Game Speed");

	if (this->_gameSpeed > MAX_GAME_SPEED) {
		this->_gameSpeed -= 0.05f;

		stringstream buffer;
		buffer << this->_gameSpeed;
		CCLog( buffer.str().c_str() );
	}
}

void GameScene::update(float dt)
{
	CCArray* itemsToDelete = CCArray::create();
	CCObject* arrayItem;

	CCARRAY_FOREACH(_cakes, arrayItem)
	{
		CCSprite* cake = (CCSprite*)(arrayItem);
		CCRect cakeRect = CCRectMake(cake->getPosition().x - (cake->getContentSize().width/2), cake->getPosition().y - (cake->getContentSize().height/2), cake->getContentSize().width, cake->getContentSize().height);

		if ( CCRect::CCRectIntersectsRect(_player->getRect(), cakeRect) ) {
			this->_comboCount++;

			if (this->_comboCount > 1) {
				stringstream buffer;
				buffer << this->_comboCount << " COMBO";
				CCLabelTTF* combo = CCLabelTTF::create(buffer.str().c_str(), "Fonts/visitor1.ttf", 36, CCSizeMake(200, 30), kCCTextAlignmentLeft);

				combo->setPosition(cake->getPosition());
				this->addChild(combo, 4);

				CCActionInterval* actionMove = CCMoveTo::create( 2.1f, ccp(combo->getPositionX(), combo->getPositionY() + 100) );
				CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create( this, callfuncN_selector(GameScene::labelMoveFinished));

				combo->runAction(CCFadeOut::actionWithDuration(2.f));
				combo->runAction(CCSequence::actions(actionMove, actionMoveDone, NULL));
			}

			itemsToDelete->addObject(cake);
		}
	}

	CCARRAY_FOREACH(_obstacles, arrayItem)
	{
		CCSprite* obstacle = (CCSprite*)(arrayItem);
		CCRect obstacleRect = CCRectMake(obstacle->getPosition().x - (obstacle->getContentSize().width / 2), obstacle->getPosition().y - (obstacle->getContentSize().height/2), obstacle->getContentSize().width * 0.8, obstacle->getContentSize().height * 0.8);

		if (CCRect::CCRectIntersectsRect(_player->getRect(), obstacleRect))
		{

			CCLabelTTF* label = CCLabelTTF::create("-1 Up", "Fonts/visitor1.ttf", 36, CCSizeMake(200, 30), kCCTextAlignmentLeft);
			label->setPosition(obstacle->getPosition());
			this->addChild(label, 4);

			CCActionInterval* actionMove = CCMoveTo::create( 2.1f, ccp(label->getPositionX(), label->getPositionY() + 100) );
			CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create( this, callfuncN_selector(GameScene::labelMoveFinished));

			label->runAction(CCFadeOut::actionWithDuration(2.f));
			label->runAction(CCSequence::actions(actionMove, actionMoveDone, NULL));

			if (this->_comboCount > 1) {
				CCLabelTTF* label = CCLabelTTF::create("C-C-C-Combo Breaker", "Fonts/visitor1.ttf", 36, CCSizeMake(500, 30), kCCTextAlignmentLeft);
				label->setPosition(ccpAdd(obstacle->getPosition(), ccp(0, -30)));
				this->addChild(label, 4);

				CCActionInterval* actionMove = CCMoveTo::create( 2.1f, ccp(label->getPositionX(), label->getPositionY() + 100) );
				CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create( this, callfuncN_selector(GameScene::labelMoveFinished));

				label->runAction(CCFadeOut::actionWithDuration(2.f));
				label->runAction(CCSequence::actions(actionMove, actionMoveDone, NULL));
			}

			// Reset combo
			this->_comboCount = 0;
			itemsToDelete->addObject(obstacle);
		}
	}

	CCARRAY_FOREACH(_coins, arrayItem)
	{
		CCSprite* coin = (CCSprite*)(arrayItem);
		CCRect coinRect = CCRectMake(coin->getPosition().x - (coin->getContentSize().width/2), coin->getPosition().y - (coin->getContentSize().height/2), coin->getContentSize().width, coin->getContentSize().height);

		if (CCRect::CCRectIntersectsRect(_player->getRect(), coinRect))
		{
			CCLabelTTF* label = CCLabelTTF::create("+1 Coin", "Fonts/visitor1.ttf", 36, CCSizeMake(200, 30), kCCTextAlignmentLeft);
			label->setPosition(coin->getPosition());
			this->addChild(label, 4);

			CCActionInterval* actionMove = CCMoveTo::create( 2.1f, ccp(label->getPositionX(), label->getPositionY() + 100) );
			CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create( this, callfuncN_selector(GameScene::labelMoveFinished));

			label->runAction(CCFadeOut::actionWithDuration(2.f));
			label->runAction(CCSequence::actions(actionMove, actionMoveDone, NULL));

			itemsToDelete->addObject(coin);
		}
	}

	CCARRAY_FOREACH(_powerUps, arrayItem)
	{
		CCSprite* powerUp = (CCSprite*)(arrayItem);
		CCRect powerUpRect = CCRectMake(powerUp->getPosition().x - (powerUp->getContentSize().width/2), powerUp->getPosition().y - (powerUp->getContentSize().height/2), powerUp->getContentSize().width, powerUp->getContentSize().height);

		if (CCRect::CCRectIntersectsRect(_player->getRect(), powerUpRect))
		{
			CCString labelPowUp;
			if (_player->getHasteMode() == true) {
				labelPowUp = "Extended";
			}
			else {
				labelPowUp = "Haste Mode";
			}

			CCLabelTTF* label = CCLabelTTF::create(labelPowUp.getCString(), "Fonts/visitor1.ttf", 36, CCSizeMake(300, 30), kCCTextAlignmentLeft);
			label->setPosition(powerUp->getPosition());
			this->addChild(label, 4);

			CCActionInterval* actionMove = CCMoveTo::create( 2.1f, ccp(label->getPositionX(), label->getPositionY() + 100) );
			CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create( this, callfuncN_selector(GameScene::labelMoveFinished));

			label->runAction(CCFadeOut::actionWithDuration(2.f));
			label->runAction(CCSequence::actions(actionMove, actionMoveDone, NULL));

			itemsToDelete->addObject(powerUp);
		}
	}

	CCARRAY_FOREACH(itemsToDelete, arrayItem)
	{
		CCSprite* item = (CCSprite*)(arrayItem);
		if (item->getTag() == 0) {
			SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("Sounds/coin.wav")).c_str());
			_coins->removeObject(item);

			this->_totalCoins++;
			_hudLayer->updateCoins(this->_totalCoins);
		}
		if (item->getTag() == 1){
			SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("Sounds/explosion.wav")).c_str());
			_obstacles->removeObject(item);

			this->_totalLife--;
			_hudLayer->updateLives(this->_totalLife);
		}
		else if (item->getTag() == 2){
			SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("Sounds/coin.wav")).c_str());
			_cakes->removeObject(item);

			this->_totalCakes++;
			_hudLayer->updateCakes(this->_totalCakes);
		}
		else if (item->getTag() >= 3 && item->getTag() <= 5) {
			this->activateHasteMode();
			_powerUps->removeObject(item);

		}
		this->removeChild(item, true);
	}

	itemsToDelete->release();
}

void GameScene::labelMoveFinished(CCNode* sender)
{
	CCLabelTTF *label = (CCLabelTTF *)sender;
	this->removeChild(label, true);
}

void GameScene::playPoof()
{
	SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("Sounds/explosion.wav")).c_str());

	CCSprite* poof = CCSprite::createWithSpriteFrameName("Smoke01.png");
	poof->setPosition(_player->getPosition());
	this->addChild(poof, 6);

	CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create( this, callfuncN_selector(GameScene::spriteMoveFinished));
	poof->runAction(CCSequence::actions(_poofAnimAction, actionMoveDone, NULL));
}

void GameScene::hasteModeTick(float dt)
{
	this->_countdown--;

	if (this->_countdown == 0) {
		this->playPoof();
		this->deactivateHasteMode();
	}
}

void GameScene::activateHasteMode()
{
	_player->idle();
	if (_player->getHasteMode() == true) {
		this->deactivateHasteMode();
	}
	else {
		this->playPoof();
	}

	this->_countdown = HASTE_COUNTDOWN;

	_noticeHasteMode->runAction( FADE_OUT );
	_player->setHasteModeActive(true);

	this->schedule( schedule_selector(GameScene::hasteModeTick), 1 );
	this->schedule( schedule_selector(GameScene::hasteModeAfterBurn), 0.15 );
}

void GameScene::deactivateHasteMode()
{
	_player->idle();
	_player->setHasteModeActive(false);
	this->unschedule( schedule_selector(GameScene::hasteModeTick) );
	this->unschedule( schedule_selector(GameScene::hasteModeAfterBurn) );
}

void GameScene::hasteModeAfterBurn(float dt)
{
	CCParticleFire* f_emitter = CCParticleFire::create();
	f_emitter->setPositionType( kCCPositionTypeRelative );
	f_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("Images/TextureHaste.png") );//.pvr");
	f_emitter->setDuration(0.5f);
	f_emitter->setPosition(ccp(_player->getPosition().x, 30));
	f_emitter->setAutoRemoveOnFinish(true);

	this->addChild(f_emitter, 10);
}
