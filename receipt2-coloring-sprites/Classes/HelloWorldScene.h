#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

USING_NS_CC;

class HelloWorld : public cocos2d::CCLayer
{
public:
	//YOUR code here...
	void runRecipe();
	void glowAt(CCPoint pos,CCSize size, ccColor3B color, float rotation, CCSprite* sprite);
	void initButtons();
	void fadeToBlackCallback(CCObject* sender);
	enum {TAG_FADE_SPRITE, TAG_FADE_TO_BLACK};
	//Standart stuff below...
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    virtual void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    LAYER_NODE_FUNC(HelloWorld);
};

#endif  // __HELLOWORLD_SCENE_H__