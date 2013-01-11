/*
 * AnimatedObject.h
 *
 *  Created on: Jan 11, 2013
 *      Author: jaunesarmiento
 */

#ifndef ANIMATEDOBJECT_H_
#define ANIMATEDOBJECT_H_

#include "cocos2d.h"

USING_NS_CC;

class AnimatedObject : public CCSprite {

private:
	CCSpriteBatchNode* _spriteSheet;

public:
	bool init(const char* frameName, CCSpriteBatchNode* spriteSheet);

	CCRect getRect();

	void destroy();

};

#endif /* ANIMATEDOBJECT_H_ */
