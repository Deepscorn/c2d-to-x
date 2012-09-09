//translated to cocos2d-x by Deepscorn, 2012, vnms11@gmail.com
#ifndef SIMPLE_ANIM_OBJECT
#define SIMPLE_ANIM_OBJECT

#include "cocos2d.h"
USING_NS_CC;

class SimpleAnimObject : public CCSprite {
	void update(ccTime t);
	CCRect rect();
	CC_SYNTHESIZE(int,animationType,AnimationType);
	CC_SYNTHESIZE(CCPoint,velocity,Velocity);
public:
	//added to receipt for easy-reading code
	static SimpleAnimObject* spriteWithBatchNode(CCSpriteBatchNode* batch, CCRect rect);
};

#endif