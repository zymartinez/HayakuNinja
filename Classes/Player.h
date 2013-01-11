/*
 * Player.h
 *
 *  Created on: Jan 5, 2013
 *      Author: jaunesarmiento
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "cocos2d.h"

#include "Globals.h"

USING_NS_CC;

class Player : public CCSprite {

private:
	float _speed;
	float _dx; // displacement

	int _walkSoundId;

	bool _hasteModeActive;

	PlayerState _playerState;

	CCRepeatForever* _idleAnimation;
	CCRepeatForever* _runAnimation;
	CCRepeatForever* _hasteRunAnimation;

public:

	int _direction;

	virtual bool init();
	static Player* player();

	void update(float dt);
	void movePlayerWithDirection(int direction);

	// Setter for player states
	void idle();

	CREATE_FUNC(Player);

};

#endif /* PLAYER_H_ */
