//
//  HowToScene.h
//  Hanauranai
//
//  Created by 前原謙一 on 2014/11/05.
//
//

#ifndef __Hanauranai__HowToScene__
#define __Hanauranai__HowToScene__
#include "cocos2d.h"

class HowtoPlay : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    // implement the "static creat*e()" method ma***nually
    CREATE_FUNC(HowtoPlay);
    void pushStart(cocos2d::Ref *pSender);
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    enum PLAY_FLOW{
        START,
        DISPLAYHANDS,
        DISPLAYCLOCKBALL,
        MOVECLOCKBALL,
        LEFTSWIPECLOCKBALLINIT,
        LEFTSWIPECLOCKBALL,
        UPSWIPECLOCKBALLINIT,
        UPSWIPECLOCKBALL,
        DISPLAYANTICLOCKBALL,
        MOVEANTICLOCKBALL,
        RIGHTSWIPEANTICLOCKBALLINIT,
        RIGHTSWIPEANTICLOCKBALL,
        UPSWIPEANTICLOCKBALLINIT,
        UPSWIPEANTICLOCKBALL,
        END,
        BACK
        
    };
    enum SPRITE_TAG{
        RIGHT_HAND_TAG,
        LEFT_HAND_TAG,
        C_BALL_TAG,
        AC_BALL_TAG,
        WAVE_TAG,
        R_RANGE_TAG,
        L_RANGE_TAG,
        ORDER_TAG
    };
    enum SWIPE_FLAG{
        RIGHT_SWIPE,
        LEFT_SWIPE,
        UP_SWIPE,
        DOWN_SWIPE,
        NONE_SWIPE
    };
    SWIPE_FLAG s_flag=NONE_SWIPE;
    PLAY_FLOW playing=START;
    void update(float delta);
    cocos2d::Point Last_pt;
    cocos2d::Point p;
    void HandEffect(SPRITE_TAG hand);
    void pushBack(cocos2d::Ref *pSender); //戻る
protected:
    bool nextFlag;
};

#endif /* defined(__Hanauranai__HowToScene__) */
