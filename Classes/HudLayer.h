/*
 * HudLayer.h
 *
 *  Created on: Jan 5, 2013
 *      Author: jaunesarmiento
 */

#ifndef HUDLAYER_H_
#define HUDLAYER_H_

#include "cocos2d.h"

USING_NS_CC;

class HudLayer : public CCLayer {

protected:
	CCLayerColor* _pauseOverlay;
	CCLayerColor* _toolPaneOverlay;

	CCMenu* menuPause;

	CCActionInterval* actionOpenToolPane;
	CCActionInterval* actionCloseToolPane;

private:
	CCSprite* noticeGameSpeedDouble;
	CCSprite* noticeGameSpeedNormal;
	CCSprite* noticeHasteMode;

	CCLabelTTF* _labelLives;
	CCLabelTTF* _labelCakes;
	CCLabelTTF* _labelCoins;

	void onPause(CCObject* pSender);
	void onResume(CCObject* pSender);
	void onQuit(CCObject* pSender);

	void onOpenToolPane(CCObject* pSender);
	void onCloseToolPane(CCObject* pSender);

public:
	virtual bool init();

	void updateLives(int value);
	void updateCakes(int value);
	void updateCoins(int value);

	CREATE_FUNC(HudLayer);

};

#endif /* HUDLAYER_H_ */
