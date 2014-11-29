#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    int i=0;
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    void pushStart(cocos2d::Ref *pSender);
    void pushFree(cocos2d::Ref *pSender);
    void pushHowto(cocos2d::Ref *pSender);
    
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void showCutinCallback(cocos2d::Ref* pSender);
    
};

#endif // __HELLOWORLD_SCENE_H__
