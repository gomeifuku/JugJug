//
//  BallSprite.h
//  Hanauranai
//
//  Created by 前原謙一 on 2014/11/01.
//
//

#ifndef __Hanauranai__BallSprite__
#define __Hanauranai__BallSprite__
#include "cocos2d.h"
#include <stdio.h>
class BallSprite : public cocos2d::Sprite
{
private:
    //
    bool inHand;
public:
    //範囲を設定する
    void setRadius(float radius);
    //当たり判定
    bool ballInHand(BallSprite* ball,cocos2d::Rect Hand);
    //create
    static BallSprite* createBall();
    bool init();
    void addParent(cocos2d::Node *Parent);
    cocos2d::Sprite* getpSprite();
    void setPosition(const cocos2d::Point& pos);
    typedef struct Ballpara{
        float minDist[4];
        bool ballInHand[4];
        cocos2d::Vector<Node*>::iterator min[4];
    }Ball_P;
private:
    cocos2d::Sprite* pSprite;

};


#endif /* defined(__Hanauranai__BallSprite__) */