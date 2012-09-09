//translated to cocos2d-x by Deepscorn, 2012, vnms11@gmail.com
#include "Ch1_AnimatingSprites.h"

//implementation Ch1_AnimatingSprites

//this implementation till I find (or create) better
int arc4random() {
	return rand();
}

CCLayer* Ch1_AnimatingSprites::runRecipe() {
	//Add our PLIST to the SpriteFrameCache
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("simple_bat.plist");
	
	//Add castle background
	CCSprite *castle = CCSprite::spriteWithFile("dracula_castle.jpg");
	castle->setPosition(ccp(240,160));
	this->addChild(castle,0,TAG_CASTLE);
	
	//Add a lightning bolt
	lightningBolt = CCSprite::spriteWithFile("lightning_bolt.png");
	lightningBolt->setPosition(ccp(240,160));
	lightningBolt->setOpacity(64);
	lightningBolt->retain();

	//Add a sprite to make it light up other areas.
	lightningGlow = CCSprite::spriteWithFile("lightning_glow.png");
	lightningGlow->setColor(ccc3(255,255,0));
	lightningGlow->setPosition(ccp(240,160));
	lightningGlow->setOpacity(100);
	//lightningGlow->setBlendFunc( (ccBlendFunc) { GL_ONE, GL_ONE });
	lightningBolt->addChild(lightningGlow);
	
	//Set a counter for lightning duration randomization
	lightningRemoveCount = 0;

	//Bats Array Initialization
	bats = new CCArray();
	
	//Add bats using a batch node.
	CCSpriteBatchNode *batch1 = CCSpriteBatchNode::batchNodeWithFile("simple_bat.png",10);
	this->addChild(batch1,2,TAG_BATS);
	
	//Make them start flying up.
	for(int x=0; x<10; x++){
		//Create SimpleAnimObject of bat - method spriteWithBatchNode is declared in SimpleAnimObject! Not coco2d method!
		SimpleAnimObject *bat = SimpleAnimObject::spriteWithBatchNode(batch1,CCRectMake(0,0,48,48));
		batch1->addChild(bat);
		
		CCPoint ptRand = ccp(arc4random()%400+40, arc4random()%150+150);
		bat->setPosition(ptRand);
		
		//Make the bat fly up. Get the animation delay (flappingSpeed).
		float flappingSpeed = this->makeBatFlyUp(bat);
		
		//Base y velocity on flappingSpeed.
		bat->setVelocity(ccp((arc4random()%1000)/500 + 0.2f, 0.1f/flappingSpeed));
		
		//Add a pointer to this bat object to the NSMutableArray
		bats->addObject(bat);
		//bat->retain();
		
		//Set the bat's direction based on x velocity.
		if(bat->getVelocity().x > 0){
			bat->setFlipX(true);
		}
	}

	//Schedule physics updates
	this->schedule(schedule_selector(Ch1_AnimatingSprites::step));
	
	return this;
}

float Ch1_AnimatingSprites::makeBatFlyUp(SimpleAnimObject* bat) {
	CCSpriteFrameCache * cache = CCSpriteFrameCache::sharedSpriteFrameCache();

	//Randomize animation speed.
	float delay = (float)(arc4random()%5+5)/80;
	CCAnimation *animation = CCAnimation::animation(); 
	//initWithName("simply_bat_fly" delay(delay);
	animation->setDelayPerUnit(delay);
	//Randomize animation frame order.
	int num = arc4random()%4+1;
	for(int i=1; i<=4; i+=1){
		animation->addSpriteFrame(cache->spriteFrameByName(CCString::stringWithFormat("simple_bat_0%i.png",num)->getCString()));
		num++;
		if(num > 4){ num = 1; }
	}
	
	//Stop any running animations and apply this one.
	bat->stopAllActions();
	bat->runAction(CCRepeatForever::actionWithAction(CCAnimate::actionWithAnimation(animation)));
	
	//Keep track of which animation is running.
	bat->setAnimationType(BAT_FLYING_UP);

	return delay;	//We return how fast the bat is flapping.*/
}

 void Ch1_AnimatingSprites::makeBatGlideDown(SimpleAnimObject* bat) {
	CCSpriteFrameCache * cache = CCSpriteFrameCache::sharedSpriteFrameCache();

	//Apply a simple single frame gliding animation.
	CCAnimation *animation = CCAnimation::animation();
	//initWithName("simple_bat_glide" delay(100.0f);
	animation->setDelayPerUnit(100.f);
	animation->addSpriteFrame(cache->spriteFrameByName("simple_bat_01.png"));
	
	//Stop any running animations and apply this one.
	bat->stopAllActions();
	bat->runAction(CCRepeatForever::actionWithAction(CCAnimate::actionWithAnimation(animation)));
	
	//Keep track of which animation is running.
	bat->setAnimationType(BAT_GLIDING_DOWN);
}

void Ch1_AnimatingSprites::step(ccTime delta) {
	CCSize s = CCDirector::sharedDirector()->getWinSize();

	for(int i=0;i<bats->count();i++){
		//Get SimpleAnimObject out of NSArray of NSValue objects.
		SimpleAnimObject *bat = (SimpleAnimObject*)bats->objectAtIndex(i);//(key pointerValue);
	
		//Make sure bats don't fly off the screen
		if(bat->getPosition().x > s.width){
			bat->setVelocity(ccp(-bat->getVelocity().x, bat->getVelocity().y));
			bat->setFlipX(false);
		}else if(bat->getPosition().x < 0){
			bat->setVelocity(ccp(-bat->getVelocity().x, bat->getVelocity().y));
			bat->setFlipX(true);
		}else if(bat->getPosition().y > s.height){
			bat->setVelocity(ccp(bat->getVelocity().x, -bat->getVelocity().y));
			this->makeBatGlideDown(bat);
		}else if(bat->getPosition().y < 0){
			bat->setVelocity(ccp(bat->getVelocity().x, -bat->getVelocity().y));
			this->makeBatFlyUp(bat);
		}
		
		//Randomly make them fly back up
		if(arc4random()%100 == 7){
			if(bat->getAnimationType() == BAT_GLIDING_DOWN){ this->makeBatFlyUp(bat); bat->setVelocity(ccp(bat->getVelocity().x, -bat->getVelocity().y)); }
			else if(bat->getAnimationType() == BAT_FLYING_UP){ this->makeBatGlideDown(bat); bat->setVelocity(ccp(bat->getVelocity().x, -bat->getVelocity().y)); }
		}
		
		//Update bat position based on direction
		bat->setPosition(ccp(bat->getPosition().x + bat->getVelocity().x, bat->getPosition().y + bat->getVelocity().y));
	}
	
	//Randomly make lightning strike
	if(arc4random()%70 == 7){
		if(lightningRemoveCount < 0){
			this->addChild(lightningBolt,1,TAG_LIGHTNING_BOLT);
			lightningRemoveCount = arc4random()%5+5;
		}
	}
	
	//Count down
	lightningRemoveCount -= 1;
	
	//Clean up any old lightning bolts
	if(lightningRemoveCount == 0){
		this->removeChildByTag(TAG_LIGHTNING_BOLT,false);
	}
}

void Ch1_AnimatingSprites::cleanRecipe() {
	bats->release();
	//(super cleanRecipe); - no need to call, because virtual destructor will do that implicitly
}

CCScene* Ch1_AnimatingSprites::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::node();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        Ch1_AnimatingSprites *layer = Ch1_AnimatingSprites::node();
        CC_BREAK_IF(! layer);
		//manually run recipe by calling layer->runRecipe();
		layer->runRecipe();
        // add layer as a child to scene
        scene->addChild(layer);
		
    } while (0);

    // return the scene
    return scene;
}