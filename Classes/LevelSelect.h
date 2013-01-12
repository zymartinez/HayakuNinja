/*
 * LevelSelect.h
 *
 *  Created on: Jan 13, 2013
 *      Author: jaunesarmiento
 */

#ifndef LEVELSELECT_H_
#define LEVELSELECT_H_

#include "cocos2d.h"
USING_NS_CC;

using namespace std;

class LevelSelect : public CCLayerColor {

public:

	virtual bool init();
	static CCScene* scene();

	void menuCloseCallback(CCObject* pSender);
	void goToLevel1();
	void menuLevel1Callback();
	void menuLevel2Callback();
	void menuLevel3Callback();
	void menuLevel4Callback();

	CCString zyzy;

	CREATE_FUNC(LevelSelect);

};

#endif /* LEVELSELECT_H_ */
