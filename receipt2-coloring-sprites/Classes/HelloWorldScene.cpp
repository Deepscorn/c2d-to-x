#include "HelloWorldScene.h"

using namespace cocos2d;

CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::node();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::node();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemImage *pCloseItem = CCMenuItemImage::itemWithNormalImage(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(HelloWorld::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::menuWithItems(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);

        // 2. Add a label shows "Hello World".

        // Create a label and initialize with string "Hello World".
        CCLabelTTF* pLabel = CCLabelTTF::labelWithString("Coloring sprites", "Arial", 24);
        CC_BREAK_IF(! pLabel);

        // Get window size and place the label upper. 
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        pLabel->setPosition(ccp(size.width / 2, size.height - 50));

        // Add the label to HelloWorld layer as a child layer.
        this->addChild(pLabel, 1);

        // 3. Add add a splash screen, show the cocos2d splash image.
        CCSprite* pSprite = CCSprite::spriteWithFile("HelloWorld.png");
        CC_BREAK_IF(! pSprite);

        // Place the sprite on the center of the screen
        pSprite->setPosition(ccp(size.width/2, size.height/2));

        // Add the sprite to HelloWorld layer as a child layer.
        this->addChild(pSprite, 0);

		//YOUR code here...
		runRecipe();

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::runRecipe() {
	//fade scene to...
	CCSprite *fadeSprite = CCSprite::spriteWithFile("blank.png");
	fadeSprite->setOpacity(0);
	fadeSprite->setPosition(ccp(240,160));
	fadeSprite->setTextureRect(CCRectMake(0,0,480,320));
	this->addChild(fadeSprite,3,TAG_FADE_SPRITE);
	//add gradient below the mountains
	//NO CCGradient* found in cocos2d-x v 2.0!
	initButtons();

	//samurais...
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite* goodSamurai = CCSprite::spriteWithFile("samurai_good.png",CCRectMake(0,0,winSize.width*0.75f,winSize.height));
	goodSamurai->setAnchorPoint(ccp(0,0));
	goodSamurai->setPosition(ccp(0,0));
	this->addChild(goodSamurai);
	this->glowAt(ccp(210,220),CCSizeMake(3.0f,11.0f),ccc3(0,230,255),45.0f,goodSamurai);
	CCSprite* evilSamurai = CCSprite::spriteWithFile("samurai_evil.png",CCRectMake(0,0,winSize.width*0.75f,winSize.height));
	evilSamurai->setFlipX(true);
	evilSamurai->setAnchorPoint(ccp(0,0));
	evilSamurai->setPosition(ccp(winSize.width*0.25f,0));
	this->addChild(evilSamurai);
	this->glowAt(ccp(150,220),CCSizeMake(3.0f,11.0f),ccc3(255,200,2),-45.0f,evilSamurai);
}

void HelloWorld::glowAt(CCPoint pos,CCSize size, ccColor3B color, float rotation, CCSprite* sprite) {
	CCSprite *glowSprite = CCSprite::spriteWithFile("fire.png");
	glowSprite->setColor(color);
	glowSprite->setPosition(pos);
	glowSprite->setRotation(rotation);
	//glowSprite->setBlendFunc(new ccBlendFunc(GL_ONE,GL_ONE));
	glowSprite->runAction(CCRepeatForever::actionWithAction((CCActionInterval*)CCSequence::actions(
		CCScaleTo::actionWithDuration(0.9f,size.width,size.height),
		CCScaleTo::actionWithDuration(0.9f,size.width*0.75f,size.height*0.75f),
		NULL)));
	glowSprite->runAction(CCRepeatForever::actionWithAction((CCActionInterval*)CCSequence::actions(
		CCFadeTo::actionWithDuration(0.9f,150),
		CCFadeTo::actionWithDuration(0.9f,255),
		NULL)));
	sprite->addChild(glowSprite);
}

void HelloWorld::initButtons() {
	CCMenuItemFont::setFontSize(16);
	//fade to 'black' button
	CCMenuItemFont *fadeToBlack = CCMenuItemFont::itemWithString("FADE TO BLACK");
	fadeToBlack->setTarget(this,menu_selector(HelloWorld::fadeToBlackCallback));
	CCMenu* fadeToBlackMenu = CCMenu::menuWithItems(fadeToBlack,NULL);
	fadeToBlackMenu->setPosition(ccp(180,20));
	this->addChild(fadeToBlackMenu,4,TAG_FADE_TO_BLACK);
}

/* Fade the scene to black */
void HelloWorld::fadeToBlackCallback(CCObject* sender) {
	CCSprite *fadeSprite = (CCSprite*)this->getChildByTag(TAG_FADE_SPRITE);
	fadeSprite->stopAllActions();
	fadeSprite->setColor(ccc3(0,0,0));
	fadeSprite->setOpacity(0.0f);
	fadeSprite->runAction(CCSequence::actions(
		CCFadeIn::actionWithDuration(2.0f),
		CCFadeOut::actionWithDuration(2.0f),
		NULL));
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

