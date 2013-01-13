/*
 * GameScene.h
 *
 *  Created on: Jan 5, 2013
 *      Author: jaunesarmiento
 */

#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include "cocos2d.h"
#include "Player.h"
#include "HudLayer.h"

USING_NS_CC;

using namespace std;

class GameScene : public CCLayer {


protected:
	HudLayer* _hudLayer;

	CCArray* _coins;
	CCArray* _cakes;
	CCArray* _powerUps;
	CCArray* _obstacles;

	CCAnimate * _poofAnimAction;

	int _totalLife;
	int _totalCakes;
	int _totalCoins;

	CCSprite* _noticeGameSpeedDouble;
	CCSprite* _noticeHasteMode;

private:

	int _numOfTouches;
	int _comboCount;
	int _countdown;

	float _gameSpeed; // the lower the number, the faster the game is

	int direction; // player's direction (left or right)

	Player* _player;

	CCParallaxNode* _background;

public:

	virtual bool init();

	static CCScene* scene();

	void registerWithTouchDispatcher();
	void ccTouchesBegan(CCSet *touches, CCEvent *event);
	void ccTouchesEnded(CCSet *touches, CCEvent *event);

	void update(float dt);
	void updateGameSpeed(float dt);

	void gameLogic(float dt);

	void dropItem(int itemType);
	void spriteMoveFinished(CCNode* sender);
	void labelMoveFinished(CCNode* sender);

	void playPoof();
	void hasteModeTick(float dt);
	void activateHasteMode();
	void deactivateHasteMode();
	void hasteModeAfterBurn(float dt);
	void moveBackground(float dt);

	CREATE_FUNC(GameScene);

};

#endif /* GAMESCENE_H_ */
