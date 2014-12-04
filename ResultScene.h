//
//  ResultScene.h
//  Hanauranai
//
//  Created by 前原謙一 on 2014/11/04.
//
//

#ifndef __Hanauranai__ResultScene__
#define __Hanauranai__ResultScene__

#include "cocos2d.h"

class Result : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(Result);
    void pushStart(cocos2d::Object *pSender);
    void pushRetry(cocos2d::Ref *pSender);
    void pushScore(cocos2d::Ref *pSender);
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void StageEffect(float dt);
    void onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

};


#endif /* defined(__Hanauranai__ResultScene__) */
