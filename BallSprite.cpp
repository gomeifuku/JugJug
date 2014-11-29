//
//  BallSprite.cpp
//  Hanauranai
//
//  Created by 前原謙一 on 2014/11/01.
//
//

#include "BallSprite.h"

BallSprite* BallSprite::createBall(){
    BallSprite* ball = new BallSprite();
    if (ball && ball->init()) {
        ball->autorelease();
        // 他に必要な初期化処理
        ball->setTextureRect(cocos2d::Rect(0,0,30,30));
        ball->inHand=false;
        ball->init();
        return ball;
    }
    else {
        CC_SAFE_DELETE(ball);
//        delete ball;
//        ball = NULL;
        return NULL;
    }
   
}
bool BallSprite::init(){
    pSprite= Sprite::create();
    return true;
}

bool BallSprite::ballInHand(BallSprite* ball,cocos2d::Rect Hand){
    cocos2d::Rect ballRect = ball->boundingBox();
    if(Hand.intersectsRect(ballRect)){
        ball->inHand=true;
        CCLOG("ballInHand::%i",inHand);
        return true;
    }
    ball->inHand=false;
    return false;
}
void BallSprite::addParent(cocos2d::Node *Parent)
{
 
    // add the sprite as a child to this layer
    Parent->addChild(pSprite);
    

}
cocos2d::Sprite* BallSprite::getpSprite()
{
    return this->pSprite;
}
void BallSprite::setPosition(const cocos2d::Point& pos)
{
    pSprite->setPosition(pos);
}