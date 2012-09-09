//translated to cocos2d-x by Deepscorn, 2012, vnms11@gmail.com
#ifndef CH1_ANIMATING_SPRITES
#define CH1_ANIMATING_SPRITES

#include "cocos2d.h"
//#include "Recipe.h"
#include "SimpleAnimObject.h"

USING_NS_CC;

enum {
	TAG_CASTLE = 0,
	TAG_LIGHTNING_BOLT = 1,
	TAG_BATS = 2
};

//Bat animation types
enum {
	BAT_FLYING_UP = 0,
	BAT_GLIDING_DOWN = 1
};

//inherit from CCLayer (not Receipt in Receipt.h file) for less code
class Ch1_AnimatingSprites : public CCLayer {
private:
	CCArray *bats;
	CCAnimation *batFlyUp;
	CCSprite *lightningBolt;
	CCSprite *lightningGlow;
	int lightningRemoveCount;
private:
	CCLayer* runRecipe();
	float makeBatFlyUp(SimpleAnimObject* bat);
	void makeBatGlideDown(SimpleAnimObject* bat);
	void step(ccTime delta);
	void cleanRecipe();
public:
	//added to original receipt
	LAYER_NODE_FUNC(Ch1_AnimatingSprites);
	static CCScene* scene();
	Ch1_AnimatingSprites(): bats(NULL), batFlyUp(NULL), lightningBolt(NULL), lightningGlow(NULL),
		lightningRemoveCount(0) {}
	~Ch1_AnimatingSprites() { cleanRecipe(); }
};

#endif