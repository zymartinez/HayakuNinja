/*
 * Globals.h
 *
 *  Created on: Jan 5, 2013
 *      Author: jaunesarmiento
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "cocos2d.h"

USING_NS_CC;

#define SCREEN                       CCDirector::sharedDirector()->getWinSize()
#define CENTER                       ccp(SCREEN.width/2, SCREEN.height/2)
#define PLAYER_SPEED_NORMAL          360
#define PLAYER_SPEED_HASTED          640
#define MAX_GAME_SPEED               0.5f

#define HASTE_COUNTDOWN              10
#define FADE_OUT                     CCSequence::actions(CCFadeIn::create(0.1f), CCFadeOut::actionWithDuration(5.0f), NULL)

typedef enum _PlayerState {
    kPlayerStateNone = 0,
    kPlayerStateIdle,
    kPlayerStateMove,
    kPlayerStateHurt,
    kPlayerStateKnockedOut
} PlayerState;

#endif /* GLOBALS_H_ */
