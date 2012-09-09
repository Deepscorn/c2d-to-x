//translated to cocos2d-x by Deepscorn, 2012, vnms11@gmail.com
#include "SimpleAnimObject.h"
#include "cocos2d.h"

USING_NS_CC;

//implementation SimpleAnimObject

void SimpleAnimObject::update(ccTime t) {	
	this->setPosition(ccp(this->getPosition().x + 
		velocity.x, this->getPosition().y + velocity.y));
}
CCRect SimpleAnimObject::rect() {
	float scaleMod = 0.5f;
	float w = this->getContentSize().width * this->getScale() * scaleMod;
	float h = this->getContentSize().height * this->getScale() * scaleMod;
	CCPoint point = CCPointMake(this->getPosition().x - (w/2), this->getPosition().y - (h/2));
	return CCRectMake(point.x, point.y, w, h); 
}

SimpleAnimObject* SimpleAnimObject::spriteWithBatchNode(CCSpriteBatchNode* batch, CCRect rect) {
		SimpleAnimObject *pobSprite = new SimpleAnimObject();
		CCTexture2D *pTexture = batch->getTexture();
		if (pobSprite && pTexture && pobSprite->initWithTexture(pTexture, rect))
		{
			pobSprite->autorelease();
			return pobSprite;
		}
		CC_SAFE_DELETE(pobSprite);
		return NULL;
	}