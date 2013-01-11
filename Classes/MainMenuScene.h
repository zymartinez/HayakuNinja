/*
 * MainMenuScene.h
 *
 *  Created on: Jan 5, 2013
 *      Author: jaunesarmiento
 */

#ifndef MAINMENUSCENE_H_
#define MAINMENUSCENE_H_

#include "cocos2d.h"

USING_NS_CC;

class MainMenuScene : public CCLayer {

public:

	virtual bool init();

	static CCScene* scene();

	void menuPlayCallback(CCObject* pSender);
	void menuToolShopCallback(CCObject* pSender);
	void menuOptionsCallback(CCObject* pSender);
	void menuExitCallback(CCObject* pSender);

	CREATE_FUNC(MainMenuScene);

};

#endif /* MAINMENUSCENE_H_ */
