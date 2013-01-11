/*
 * Player.cpp
 *
 *  Created on: Jan 5, 2013
 *      Author: jaunesarmiento
 */

#include "Globals.h"
#include "Player.h"

#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace CocosDenshion;

Player* Player::player()
{
	Player *player = Player::create();
	return player;
}

bool Player::init()
{
	if(!CCSprite::initWithSpriteFrameName("PlayerIdle1.png"))
	{
		return false;
	}

	CCAnimation* playerIdleAnimation = CCAnimation::create();
	CCAnimation* playerRunAnimation = CCAnimation::create();
	CCAnimation* playerRunSpeedAnimation = CCAnimation::create();

	for (int i = 0 ; i < 3 ; ++i )
	{
		std::stringstream ss;
		ss << "PlayerIdle" << i + 1  << ".png";
		std::string name = ss.str();
		CCSpriteFrame* sprite =  CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name.c_str());
		playerIdleAnimation->addSpriteFrame(sprite);
	}

	for (int i = 0; i < 4; ++i)
	{
		std::stringstream ss;
		ss << "PlayerRun" << i + 1  << ".png";
		std::string name = ss.str();
		CCSpriteFrame* sprite =  CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name.c_str());
		playerRunAnimation->addSpriteFrame(sprite);
	}

	for (int i = 0; i < 4; ++i)
	{
		std::stringstream ss;
		ss << "PlayerRunSpeed" << i + 1  << ".png";
		std::string name = ss.str();
		CCSpriteFrame* sprite =  CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name.c_str());
		playerRunSpeedAnimation->addSpriteFrame(sprite);
	}

	playerIdleAnimation->setDelayPerUnit(0.5f);
	playerRunAnimation->setDelayPerUnit(0.1f);
	playerRunSpeedAnimation->setDelayPerUnit(0.1f);

	CCAnimate *playerIdleAnimate = CCAnimate::create(playerIdleAnimation);
	CCAnimate *playerRunAnimate = CCAnimate::create(playerRunAnimation);
	CCAnimate *playerRunSpeedAnimate = CCAnimate::create(playerRunSpeedAnimation);

	_idleAnimation = CCRepeatForever::create(playerIdleAnimate);
	_runAnimation = CCRepeatForever::create(playerRunAnimate);
	_hasteRunAnimation = CCRepeatForever::create(playerRunSpeedAnimate);

	_idleAnimation->retain();
	_runAnimation->retain();
	_hasteRunAnimation->retain();

	this->setPosition(ccp(SCREEN.width / 2, 120));
	this->schedule(schedule_selector(Player::update));

	return true;
}

void Player::update(float dt)
{
	if (this->_playerState == kPlayerStateMove) {

		CCPoint playerMove = ccp(this->_speed * this->_direction, 0);
		CCPoint moveTo;
		moveTo = ccpAdd(this->getPosition(), ccpMult(playerMove, dt));

		if (moveTo.x >= 100 && moveTo.x <= 1180){
			this->setPosition( moveTo );
		}

	}
}

void Player::idle()
{
	if (this->_playerState != kPlayerStateIdle) {

		// Stop sound effects
		SimpleAudioEngine::sharedEngine()->stopEffect(this->_walkSoundId);

		this->stopAllActions();

		if (_hasteModeActive) {
			//this->runAction(_hasteIdleAnimation); // Switch to this when we get the new assets
			this->runAction(_idleAnimation);
		}
		else {
			this->runAction(_idleAnimation);
		}

		this->_playerState = kPlayerStateIdle;
	}

	this->_speed = 0;
	this->_direction = 0;
}

void Player::movePlayerWithDirection(int direction)
{
	this->unschedule(schedule_selector(Player::update));
	this->schedule(schedule_selector(Player::update));

	if (direction == -1) {
		this->setFlipX(true);
	}
	else {
		this->setFlipX(false);
	}

	if (this->_playerState == kPlayerStateIdle) {

		this->stopAllActions();

		this->_playerState = kPlayerStateMove;
		this->_direction = direction;

		if (_hasteModeActive) {
			this->runAction(_hasteRunAnimation);
			this->_speed = PLAYER_SPEED_NORMAL;
			//this->_dx = PLAYER_SPEED_HASTED * direction;
		}
		else {
			this->runAction(_runAnimation);
			this->_speed = PLAYER_SPEED_NORMAL;
			//this->_dx = PLAYER_SPEED_NORMAL * direction;
		}

		// Play the walking sound effect
		this->_walkSoundId = SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("Sounds/walk.wav")).c_str(), true);
	}
}
