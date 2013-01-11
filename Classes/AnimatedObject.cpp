/*
 * AnimatedObject.cpp
 *
 *  Created on: Jan 11, 2013
 *      Author: jaunesarmiento
 */

#include "AnimatedObject.h"

USING_NS_CC;

bool AnimatedObject::init(const char* frameName, CCSpriteBatchNode* spriteSheet)
{
	_spriteSheet = spriteSheet;
	if(!CCSprite::initWithSpriteFrameName(frameName))
	{
		return false;
	}

	float spriteWidth = this->getContentSize().width;
	this->setPosition(ccp(-spriteWidth, 0.0f));
	_spriteSheet->addChild(this);

	return true;
}

void AnimatedObject::destroy()
{
	this->stopAllActions();
	_spriteSheet->removeChild(this, true);
}

CCRect AnimatedObject::getRect()
{
	return CCRectMake(this->getPosition().x - this->getContentSize().width / 2,
					  this->getPosition().y - this->getContentSize().height / 2,
					  this->getContentSize().width,
					  this->getContentSize().height);
}
