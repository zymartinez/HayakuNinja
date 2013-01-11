/*
 * GameScene.h
 *
 *  Created on: Jan 5, 2013
 *      Author: jaunesarmiento
 */

#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include "cocos2d.h"
#include <stack>

#include "Player.h"

USING_NS_CC;

using namespace std;

class GameScene : public CCLayer {

private:

	int _numOfTouches;

	Player* _player;
	stack<int> _directionStack;

public:

	virtual bool init();

	static CCScene* scene();

	void registerWithTouchDispatcher();
	void ccTouchesBegan(CCSet *touches, CCEvent *event);
	void ccTouchesEnded(CCSet *touches, CCEvent *event);

	//void menuCloseCallback(CCObject* pSender);

	CREATE_FUNC(GameScene);

};

#endif /* GAMESCENE_H_ */
