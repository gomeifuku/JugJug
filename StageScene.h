//
//  StageScene.h
//  Hanauranai
//
//  Created by 前原謙一 on 2014/10/27.
//
//

#ifndef __Hanauranai__StageScene__
#define __Hanauranai__StageScene__
#include "cocos2d.h"
#include "XTLayer.h"
#include "BallSprite.h"

class Stage : public cocos2d::Layer
{
public:
    Stage();
    ~Stage();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    //初期化のメソッド
    virtual bool init();
    cocos2d::Point p;
    int count;
    //create()を使えるようにしている。
    CREATE_FUNC(Stage);
    
  bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
  void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
  void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    cocos2d::Point Last_pt;
    
    void pushBack(cocos2d::Object *pSender); //戻る
    void update(float delta);
    void ballUpdate(float dt);
    void myCallback();
    enum SPRITE_TAG{
        NANKA_NO_TAG,
        RIGHT_HAND_TAG,
        LEFT_HAND_TAG,
        C_BALL_TAG,
        AC_BALL_TAG,
        WAVE_TAG,
        RIGHT_DIRECTION_TAG,
        LEFT_DIRECTION_TAG
    };
    enum SWIPE_FLAG{
        RIGHT_SWIPE,
        LEFT_SWIPE,
        UP_SWIPE,
        DOWN_SWIPE,
        NONE_SWIPE
    };
    bool startFlag;
    bool freeMode;
    int jugScore;
    void InitLay();
    void InitEvent();
    void InitSound();
    void BallStart();
    bool swipeDirectionFlag;
    void HandEffect(SPRITE_TAG hand);
    void StageEffect(float dt);
    SWIPE_FLAG s_flag=NONE_SWIPE;
    double hitDelay;
    double getSec(){
        timeval tv;
        gettimeofday(&tv, nullptr);
        return (tv.tv_sec) + (tv.tv_usec) * 1e-6;
    }
protected:
    cocos2d::Vector<Node*> _balls;
    std::vector<float> _hitTimes;
};


#endif /* defined(__Hanauranai__StageScene__) */
