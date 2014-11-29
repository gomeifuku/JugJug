//
//  StageScene.cpp
//  Hanauranai
//
//  Created by 前原謙一 on 2014/10/27.
//
//
#include "StageScene.h"
#include "HelloWorldScene.h"
#include "BallSprite.h"
#include "math.h"
#include "SimpleAudioEngine.h"
#include "ResultScene.h"
//using namespace cocos2d;の略。cocos2dの名前空間を利用
USING_NS_CC;

Stage::~Stage()
{}

Stage::Stage()
:_balls(NULL)
{
}

Scene *Stage::createScene(){
    
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Stage::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
    
    
}



//cocos2d::Sprite* th_ball[4];
bool Stage::init(){
    
    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    //画面の座標関係の詳しい説明はここ http://www.cocos2d-x.org/wiki/Coordinate_System
    Size visibleSize = Director::getInstance()->getVisibleSize(); //画面のサイズを取得
    Point origin = Director::getInstance()->getVisibleOrigin();  //マルチレゾリューション対応がどうとか
    //layout構築
    InitLay();
    //seリロード
    InitSound();
    //Eventセット
    InitEvent();
    //タッチイベントの監視を開始

    
    
    jugScore=0;

  

    return true;

}

void Stage::pushBack(Object *pSender)
{
    // 遷移先の画面のインスタンス
    Scene *pScene = HelloWorld::createScene();
    
    // 0.5秒かけてフェードアウトしながら次の画面に遷移します
    //    引数１:フィードの時間
    //    引数２：移動先のシーン
    //    引数３：フィードの色（オプション）
    TransitionFade* transition = TransitionFade::create(0.5f, pScene);
    
    //遷移実行  遷移時のアニメーション　http://study-cocos2d-x.info/scenelayer/55/
    Director::getInstance()->replaceScene(transition);
    
    
}



bool Stage::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    Director* pDirector = Director::getInstance();
    Last_pt = pDirector -> convertToGL(pTouch -> getLocationInView());
  
    CCLOG("TouchBegan");
    return true;
}
void Stage::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    Size visibleSize = Director::getInstance()->getVisibleSize(); //画面

    //タッチポイントを取得（convertToGLにてOpenGLの座標に変換）
    Director* pDirector = Director::getInstance();
    Point touchPoint = pDirector -> convertToGL(pTouch -> getLocationInView());
    
    double hitStart=getSec();
    float dl=hitStart-hitDelay;
    CCLOG("%f",dl);
    
    const int MIN_DIST=30;
    
    Vec2 current=pTouch->getLocation();
    float rad=atan2(-touchPoint.y+Last_pt.y,-touchPoint.x+Last_pt.x);
    CCLOG("%f",rad);
    float dist=Last_pt.distance(pTouch->getLocation());
    CCLOG("distance=%f",dist);
    float deg=MATH_RAD_TO_DEG(rad);
    if(dist>MIN_DIST){
        CCLOG("Rad2Deg=%f",MATH_RAD_TO_DEG(rad));
        if((180>=deg&&deg>135)||(-180<deg&&deg<=-135)){
            s_flag=LEFT_SWIPE;
            CCLOG("LEFTSWIPE");
        }else if(-135<deg&&deg<=-45){
            s_flag=UP_SWIPE;
            CCLOG("UPSWIPE");
        }else if((-45<deg&&deg<=0)||(0<deg&&deg<=45)){
            s_flag=RIGHT_SWIPE;
            CCLOG("RIGHTSWIPE");
        }else if(45<deg&&deg<=135){
            s_flag=DOWN_SWIPE;
            CCLOG("DOWNSWIPE");
        }
    }else{
        CCLOG("NONESWIPE");
        s_flag=NONE_SWIPE;
    }
    //タグのインスタンスを取
   // float f=Last_pt.getAngle(current);
  //  CCLOG("%f",f);
    Sprite* r_hand= (Sprite*)this->getChildByTag(RIGHT_HAND_TAG);
    Sprite* l_hand= (Sprite*)this->getChildByTag(LEFT_HAND_TAG);
   // Sprite* r_hand = (Sprite*)this->getChildByTag(hogeTagNumber);
    
    Sprite* r_range = Sprite::create();
    Sprite* l_range = Sprite::create();
    r_range->setTextureRect(Rect(0,0 ,visibleSize.width/2, visibleSize.height));
    r_range->setPosition(Point((visibleSize.width*3)/4,visibleSize.height/2));
    l_range->setTextureRect(Rect(0, 0, visibleSize.width/2, visibleSize.height));
    l_range->setPosition(visibleSize.width/4,visibleSize.height/2);
    if (!r_hand) {
        //pHogeインスタンスが取得できなかった場合は処理を抜ける
        return;
    }
    if (!l_hand) {
        //pHogeインスタンスが取得できなかった場合は処理を抜ける
        return;
    }

    
    float animationTime=0.2,jumpHeight=20,jumpNum=1;
    Spawn* RUThrow = Spawn::create(
                                 JumpTo::create( animationTime*2,r_hand->getPosition(),jumpHeight,jumpNum),
                                 NULL);
    
    Spawn* LUThrow = Spawn::create(
                                 JumpTo::create( animationTime*2,l_hand->getPosition(),jumpHeight,jumpNum),
                                 NULL);
    ActionInterval* rotateAction = CCRotateBy::create(1, 360);
    float throwDistance=20;
    Sequence* LLThrow= Sequence::create(
                                    EaseOut::create(MoveBy::create(animationTime, Point(throwDistance,0)), 2),
                                    EaseIn::create(MoveBy::create(animationTime, Point(-throwDistance,0)), 2),NULL);
    Sequence* RRThrow= Sequence::create(
                                        EaseOut::create(MoveBy::create(animationTime, Point(-throwDistance,0)), 2),
                                        EaseIn::create(MoveBy::create(animationTime, Point(throwDistance,0)), 2),NULL);
    float missDistance=5;
    Sequence* Miss= Sequence::create(
                                        EaseOut::create(MoveBy::create(animationTime/3, Point(missDistance,0)), 2),
                                        EaseIn::create(MoveBy::create(animationTime/3, Point(-missDistance*2,0)), 2),
                                        EaseOut::create(MoveBy::create(animationTime/3, Point(missDistance,0)), 2)
                                     ,NULL);
    RepeatForever* repeatForever = CCRepeatForever::create(rotateAction);
    
    Spawn* jug;
    Spawn* r_jug;
    
    
    cocos2d::CallFunc *gameOver = CallFunc::create([this]()
                                                    {
                                                        if(!freeMode){
                                                            UserDefault *userDef = UserDefault::getInstance();
                                                            
                                                            
                                                            //                                                        int highScore = userDef->getIntegerForKey("highScore", 0);
                                                            //int型をセット
                                                            userDef->setIntegerForKey("temp", jugScore);
                                                            jugScore=0;
                                                            
                                                            // 遷移先の画面のインスタンス
                                                            Scene *pScene = Result::createScene();
                                                            
                                                            // 0.5秒かけてフェードアウトしながら次の画面に遷移します
                                                            //    引数１:フィードの時間
                                                            //    引数２：移動先のシーン
                                                            //    引数３：フィードの色（オプション）
                                                            TransitionFade* transition = TransitionFade::create(0.5f, pScene);
                                                            //遷移実行  遷移時のアニメーション　http://study-cocos2d-x.info/scenelayer/55/
                                                            Director::getInstance()->replaceScene(transition);
                                                        }else{
                                                            jugScore=0;
                                                        }
                                                    });
    
    auto remove = RemoveSelf::create(true);
    
    Rect R_Rect = r_hand -> boundingBox();
    
    //pHogeのサイズを取得する
    
    Rect L_Rect = l_hand -> boundingBox();
    
    
    Rect R_Range_Rect = r_range ->boundingBox();
    Rect L_Range_Rect = l_range ->boundingBox();
    
    const float jHigh=500;
    const float jLow=100;
    const float _jHighTime=4.0;
    const float _jLowTime=3.0;
    bool missSoundFlag=true;
    //RightHandアクション
    if (R_Range_Rect.containsPoint(touchPoint)) {
        r_hand->runAction(Miss);
        for(auto it = _balls.begin();it != _balls.end(); ++it){
            Rect ballRect = (*it) -> boundingBox();
            log("tag %d",(*it)->getTag());
            if(R_Rect.intersectsRect(ballRect)){
                if(s_flag==RIGHT_SWIPE&&(*it)->getName()=="CinRight") {
                        jug=Spawn::create(
                                          EaseOut::create(JumpTo::create(_jLowTime, Point(-50,0), jLow, 1),1.5),NULL);
                        r_hand->stopAction(Miss);
                        r_hand->runAction(RRThrow);
                     CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("se_maoudamashii_system46.mp3", false, 1.0f, 0.0f, 1.0f);                                        jugScore++;
                        HandEffect(RIGHT_HAND_TAG);
                    _hitTimes.push_back(dl);
                    missSoundFlag=false;
                }else if(s_flag==UP_SWIPE&&(*it)->getName()=="ACinRight") {
                        jug=Spawn::create(
                                          EaseOut::create(JumpTo::create(_jHighTime, Point(l_hand->getPositionX()-80,0), jHigh, 1),1.5),NULL);
                        r_hand->stopAction(Miss);
                        r_hand->runAction(RUThrow);
                     CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("se_maoudamashii_system46.mp3", false, 1.0f, 0.0f, 1.0f);
                    jugScore++;
                    HandEffect(RIGHT_HAND_TAG);
                    _hitTimes.push_back(dl);
                    missSoundFlag=false;
                }else{
                    continue;
                }
                (*it)->stopAllActions();
                (*it)->runAction(repeatForever);
                cocos2d::CallFunc *callback = CallFunc::create([this](){
                   //  _balls.erase(it);
                   // jugScore=0;
                    CCLOG("RIGHTHand::MISS");
                });//:CallFunc
                
                (*it)->runAction(Sequence::create(jug,remove,callback,gameOver,NULL));
                return;
            }
        }
        CCLOG("TouchRightHand");
    }
    //LeftHandアクション
    if(L_Range_Rect.containsPoint(touchPoint)){
        l_hand->runAction(Miss);
        for(auto it = _balls.begin();it != _balls.end(); ++it){
            Rect ballRect = (*it) -> boundingBox();
            log("tag %d",(*it)->getTag());
            
            if(L_Rect.intersectsRect(ballRect)){

                if(s_flag==UP_SWIPE&&(*it)->getName()=="CinLeft") {
                    r_jug=Spawn::create(
                                        
                                        EaseOut::create(JumpTo::create( _jHighTime,Point(r_hand->getPositionX()+80,0),jHigh,1),1.5),NULL);
                    l_hand->stopAction(Miss);
                    l_hand->runAction(LUThrow);
                     CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("se_maoudamashii_system47.mp3", false, 1.0f, 0.0f, 1.0f);
                    jugScore++;
                    _hitTimes.push_back(dl);
                    HandEffect(LEFT_HAND_TAG);
                    missSoundFlag=false;
                }else if(s_flag==LEFT_SWIPE&&(*it)->getName()=="ACinLeft") {
                    r_jug=Spawn::create(
                                        EaseOut::create(JumpTo::create(_jLowTime, Point(visibleSize.width+50,0), jLow, 1),1.5),NULL);
                    l_hand->stopAction(Miss);
                    l_hand->runAction(LLThrow);
                     CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("se_maoudamashii_system47.mp3", false, 1.0f, 0.0f, 1.0f);
                    jugScore++;
                    _hitTimes.push_back(dl);
                    HandEffect(LEFT_HAND_TAG);
                    missSoundFlag=false;
                }else{
                    continue;
                }
                (*it)->stopAllActions();
                (*it)->runAction(repeatForever);
                cocos2d::CallFunc *callback = CallFunc::create([this](){
                    //  _balls.erase(it);;
                    CCLOG("LeftHand::MISS");
                });//:CallFunc
                (*it)->runAction(Sequence::create(r_jug,remove,callback,gameOver,NULL));
                return;
            }
        }
        CCLOG("TouchRightHand");
    }
    if(missSoundFlag)
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("se_maoudamashii_system19.mp3", false, 1.0f, 0.0f, 1.0f);
}

void Stage::onTouchMoved(Touch *pTouch, Event *pEvent)
{
//    Director* pDirector = Director::getInstance();
//    Point touchPoint = pDirector -> convertToGL(pTouch -> getLocationInView());
//    const int MIN_DIST=30;
//    
//    Vec2 current=pTouch->getLocation();
//    float rad=atan2(-touchPoint.y+Last_pt.y,-touchPoint.x+Last_pt.x);
//    CCLOG("%f",rad);
//    float dist=Last_pt.distance(pTouch->getLocation());
//    CCLOG("distance=%f",dist);
//    float deg=MATH_RAD_TO_DEG(rad);
//    if(dist>MIN_DIST){
//        CCLOG("Rad2Deg=%f",MATH_RAD_TO_DEG(rad));
//        if((180>=deg&&deg>135)||(-180<deg&&deg<=-135)){
//            s_flag=LEFT_SWIPE;
//            CCLOG("LEFTSWIPE");
//        }else if(-135<deg&&deg<=-45){
//            s_flag=UP_SWIPE;
//            CCLOG("UPSWIPE");
//        }else if((-45<deg&&deg<=0)||(0<deg&&deg<=45)){
//            s_flag=RIGHT_SWIPE;
//            CCLOG("RIGHTSWIPE");
//        }else if(45<deg&&deg<=135){
//            s_flag=DOWN_SWIPE;
//            CCLOG("DOWNSWIPE");
//        }
//    }else{
//        CCLOG("NONESWIPE");
//        s_flag=NONE_SWIPE;
//    }
    CCLOG("TouchMoved");
}

void Stage::BallStart(){
    Sprite* r_hand=(Sprite*)this->getChildByTag(RIGHT_HAND_TAG);
    Sprite* l_hand=(Sprite*)this->getChildByTag(LEFT_HAND_TAG);
    Sprite* ball=Sprite::create();

    Point ballPos;
    float ball_size=30;
    ball->setTextureRect(Rect(0, 0, ball_size, ball_size));
    
    count=arc4random()%2;
    int randomNum = count%2+1;
    int jumpHeight=100;
    if(randomNum==(int)RIGHT_HAND_TAG){
        ball->setTag(C_BALL_TAG);
        ball->setColor(Color3B::YELLOW);
        
        ballPos=r_hand->getPosition();
    }else{
        
        ball->setTag(AC_BALL_TAG);
        ball->setColor(Color3B::GREEN);
        ballPos=l_hand->getPosition();
        
    }
    ball->setPosition(ballPos);
    this->addChild(ball);
    
    ActionInterval* rotateAction = CCRotateBy::create(1, 360);
    
    //リピート
    RepeatForever* repeatForever = CCRepeatForever::create(rotateAction);
    ball->runAction(repeatForever);
    
    ball->setPositionZ(0);
    
    count++;
    _balls.pushBack(ball);
    
    startFlag=false;

}
void Stage::InitEvent(){
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    // onTouchBeganイベントコールバック関数実装のラムダ式の例
    
    listener->onTouchBegan=CC_CALLBACK_2(Stage::onTouchBegan ,this);
    listener->onTouchCancelled= CC_CALLBACK_2(Stage::onTouchCancelled,this);
    listener->onTouchMoved= CC_CALLBACK_2(Stage::onTouchMoved,this);
    listener->onTouchEnded = CC_CALLBACK_2(Stage::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto listener2 = EventListenerKeyboard::create();
    listener2->onKeyReleased = [](EventKeyboard::KeyCode keyCode, Event* unused_event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
            CCLOG("Back!");
            // 遷移先の画面のインスタンス
            Scene *pScene = HelloWorld::createScene();
            
            // 0.5秒かけてフェードアウトしながら次の画面に遷移します
            //    引数１:フィードの時間
            //    引数２：移動先のシーン
            //    引数３：フィードの色（オプション）
            TransitionFade* transition = TransitionFade::create(0.5f, pScene);
            
            //遷移実行  遷移時のアニメーション　http://study-cocos2d-x.info/scenelayer/55/
            Director::getInstance()->replaceScene(transition);
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener2, this);
    
    this->scheduleUpdate();
}
void Stage::InitSound(){
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.5);
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("se_maoudamashii_system46.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("se_maoudamashii_system47.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("se_maoudamashii_system19.mp3");
    
}
void Stage::InitLay(){
    Size visibleSize = Director::getInstance()->getVisibleSize(); //画面のサイズを取得
    
    Point origin = Director::getInstance()->getVisibleOrigin();  //マルチレ対応がどうとか
    //
    
    Sprite* r_hand= Sprite::create();
    
    Sprite* l_hand= Sprite::create();
    r_hand->setTextureRect(Rect(0, 0, 180, 180));
    r_hand->setColor(Color3B::RED);
    r_hand->setPositionZ(-1);
    
    l_hand->setTextureRect(Rect(0, 0, 180, 180));
    l_hand->setColor(Color3B::BLUE);
    l_hand->setPositionZ(-1);
    
    //    for(int i;i<4;i++){
    //        th_ball[i]=Sprite::create();
    //        th_ball[i]->create();
    //        th_ball[i]->setTextureRect(Rect(0, 0,8, 8));
    //        th_ball[i]->setColor(Color3B::BLACK);
    //        th_ball[i]->setPosition(0, 0);
    //        th_ball[i]->setPositionZ(1);
    ////        th_ball[i]->setOpacity(128);
    //        this->addChild(th_ball[i]);
    //    }
    
    float x_r = visibleSize.width / 2;
    float y_r = visibleSize.height / 2;
    
    float dx=350;
    float dy=200;
    r_hand->setPosition(Point(x_r+dx, y_r-dy));
    r_hand->setTag(1);
    this->addChild(r_hand);
    
    
    l_hand->setPosition(Point(x_r-dx, y_r-dy));
    l_hand->setTag(2);
    this->addChild(l_hand);
    
    auto draw = DrawNode::create();
    draw->drawDot(r_hand->getPosition(), 10.0f, Color4F(1.0f, .6f, .5f, 1.0f));
    draw->drawDot(l_hand->getPosition(), 10.0f, Color4F(1.0f, .6f, .5f, 1.0f));
    this->addChild(draw,9);
    //戻るボタンを設置
    auto backButton = MenuItemImage::create(
                                            "back.png",  //表示
                                            "back.png",  //タップ時の画像
                                            CC_CALLBACK_1(Stage::pushBack, this));
    
    backButton->setPosition(Point(origin.x + visibleSize.width - backButton->getContentSize().width/2 ,
                                  origin.y + backButton->getContentSize().height/2));
    
    //create menu, it's an autorelease object
    auto menu = Menu::create(backButton, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
    
    auto scoreLabel = Label::createWithSystemFont(cocos2d::StringUtils::toString(jugScore), "HiraKakuProN-W6", 48);
    scoreLabel->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    scoreLabel->setTag(20);
    this->addChild(scoreLabel);
    
    UserDefault *userDef = UserDefault::getInstance();
    
    
    freeMode = userDef->getBoolForKey("freemode");
    hitDelay=getSec();
    
    auto text = Label::createWithSystemFont("GAME START", "HiraKakuProN-W6", 48);
    text->setPosition(Point(visibleSize.width/2,visibleSize.height+100));
    this->addChild(text);
    auto textAnimation=Sequence::create(FadeIn::create(1.0)
                                        ,DelayTime::create(1.0)
                                        ,FadeTo::create(0.3f, 0)
                                        ,RemoveSelf::create(true)
                                        ,NULL);
    
    text->runAction(textAnimation);
    
    
    BallStart();

}
void Stage::ballUpdate(float dt){
    Size visibleSize = Director::getInstance()->getVisibleSize(); //画面のサイズを取得
    
    Sprite* r_hand=(Sprite*)this->getChildByTag(RIGHT_HAND_TAG);
    Sprite* l_hand=(Sprite*)this->getChildByTag(LEFT_HAND_TAG);
    Sprite* ball=Sprite::create();
    Spawn* jump;
    
    float ball_px;
    float ball_py;
    float ball_size=30;
    ball->setTextureRect(Rect(0, 0, ball_size, ball_size));
    int randomNum = count%2+1;
    int jumpHeight=100;
    if(randomNum==(int)RIGHT_HAND_TAG){
        
        ball_px=r_hand->getPositionX()-50;
        ball_py=r_hand->getPositionY()+300;
        jump = Spawn::create(
                             JumpTo::create(4.0f,Point(r_hand->getPositionX()+50,0),jumpHeight,1),
                             NULL);
        ball->setTag(C_BALL_TAG);
        ball->setColor(Color3B::YELLOW);
    }else{
        
        ball_px=l_hand->getPositionX()+50;
        ball_py=l_hand->getPositionY()+300;
        ball->setTag(AC_BALL_TAG);
        ball->setColor(Color3B::GREEN);
        jump = Spawn::create(
                             JumpTo::create(4.0f,Point(l_hand->getPositionX()-50,0),jumpHeight,1),
                             NULL);

    }
    ball->setPosition(Point(ball_px,ball_py));
    this->addChild(ball);

    
    auto remove = RemoveSelf::create(true);
    cocos2d::CallFunc *callback = CallFunc::create([this]()
    {
    //    cocos2d::Vector<BallSprite*>::iterator first =_balls.begin();
        //jugScore=0;
        CCLOG("STARTBALLMISS...");
        //log("erase::%d",(*first)->getTag());
      //  _balls.erase(first);
    //    for(auto it = _balls.begin();it != _balls.end(); ++it){
       //     log("tag %d",(*it)->getTag());
      //  }
    });//:CallFunc
    
    
    cocos2d::CallFunc *gameOver = CallFunc::create([this]()
                                                   {
                                                       if(!freeMode){
                                                       UserDefault *userDef = UserDefault::getInstance();
                                                       
                                                       
                                                       //                                                        int highScore = userDef->getIntegerForKey("highScore", 0);
                                                       //int型をセット
                                                       userDef->setIntegerForKey("temp", jugScore);
                                                       jugScore=0;
                                                       
                                                       // 遷移先の画面のインスタンス
                                                       Scene *pScene = Result::createScene();
                                                       
                                                       // 0.5秒かけてフェードアウトしながら次の画面に遷移します
                                                       //    引数１:フィードの時間
                                                       //    引数２：移動先のシーン
                                                       //    引数３：フィードの色（オプション）
                                                       TransitionFade* transition = TransitionFade::create(0.5f, pScene);
                                                       //遷移実行  遷移時のアニメーション　http://study-cocos2d-x.info/scenelayer/55/
                                                       Director::getInstance()->replaceScene(transition);
                                                       
                                                       }else{
                                                           jugScore=0;
                                                       }
                                                   });
    float _dTime=3.0;
    float delta=(float)(arc4random()%6)/10.0;
    CCLOG("delta=%f",delta);
    _dTime = _dTime+delta;
    auto delay = DelayTime::create(_dTime);
    ActionInterval* rotateAction = CCRotateBy::create(1, 360);
    
    //リピート
    RepeatForever* repeatForever = CCRepeatForever::create(rotateAction);
    ball->runAction(repeatForever);
    Sequence* sequence = Sequence::create(delay,jump,remove,callback,gameOver,NULL);
    sequence->setTag(1);
    ball->setPositionZ(0);
    ball->runAction(sequence);
    
    count++;
    _balls.pushBack(ball);
}
void Stage::update(float delta){

    auto draw =DrawNode::create();
   
    Size visibleSize = Director::getInstance()->getVisibleSize(); //画面のサイズを取得
   Point origin = Director::getInstance()->getVisibleOrigin();  //マルチレ対応がどうとか
//    
    Sprite* r_hand=(Sprite*)this->getChildByTag(RIGHT_HAND_TAG);
    Sprite* l_hand=(Sprite*)this->getChildByTag(LEFT_HAND_TAG);
    BallSprite::Ball_P ball_p;
//    
//    draw->drawDot(Point(r_hand->getPositionX()+r_hand->getContentSize().width/2,r_hand->getPositionY()-r_hand->getContentSize().height/2), 10.0f, Color4F(1.0f, .6f, .5f, 1.0f));
//    this->addChild(draw);
    for(int i=0;i<4;i++){
        
        ball_p.minDist[i]=300;
        ball_p.ballInHand[i]=false;
      //  th_ball[i]->setPosition(Point(-30,-30));
    
    }
    for(auto it = _balls.begin();it != _balls.end(); ++it){
        if((*it)->getTag()==C_BALL_TAG){
            (*it)->setColor(Color3B::YELLOW);
        }
        else if((*it)->getTag()==AC_BALL_TAG)
            (*it)->setColor(Color3B::GREEN);
        if(r_hand->getBoundingBox().intersectsRect((*it)->getBoundingBox())){
            float dist =(*it)->getPosition().getDistance(Point(r_hand->getPositionX()+r_hand->getContentSize().width/4,r_hand->getPositionY()-r_hand->getContentSize().height/2));
            if((*it)->getTag()==C_BALL_TAG){
                ball_p.ballInHand[(int)RIGHT_HAND_TAG-1]=true;
                if(dist<ball_p.minDist[RIGHT_HAND_TAG-1]){
                    ball_p.minDist[(int)RIGHT_HAND_TAG-1]=dist;
                    ball_p.min[(int)RIGHT_HAND_TAG-1]=it;
                }
            }else if((*it)->getTag()==AC_BALL_TAG){
                ball_p.ballInHand[(int)RIGHT_HAND_TAG+1]=true;
                if(dist<ball_p.minDist[RIGHT_HAND_TAG+1]){
                    ball_p.minDist[(int)RIGHT_HAND_TAG+1]=dist;
                    ball_p.min[(int)RIGHT_HAND_TAG+1]=it;
                }
            }

        }else if(l_hand->getBoundingBox().intersectsRect((*it)->getBoundingBox())){
            float dist =(*it)->getPosition().getDistance(Point(l_hand->getPositionX()-l_hand->getContentSize().width/4,l_hand->getPositionY()-l_hand->getContentSize().height/2));
            if((*it)->getTag()==C_BALL_TAG){
                ball_p.ballInHand[(int)LEFT_HAND_TAG-1]=true;
                if(dist<ball_p.minDist[LEFT_HAND_TAG-1]){
                    ball_p.minDist[(int)LEFT_HAND_TAG-1]=dist;
                    ball_p.min[(int)LEFT_HAND_TAG-1]=it;
                }
            }else if((*it)->getTag()==AC_BALL_TAG){
                ball_p.ballInHand[(int)LEFT_HAND_TAG+1]=true;
                if(dist<ball_p.minDist[LEFT_HAND_TAG+1]){
                    ball_p.minDist[(int)LEFT_HAND_TAG+1]=dist;
                    ball_p.min[(int)LEFT_HAND_TAG+1]=it;
                }
            }
        }
        (*it)->setName("outRange");

    }
    if(ball_p.ballInHand[(int)RIGHT_HAND_TAG-1]){
       // (*ball_p.min[(int)RIGHT_HAND_TAG-1])->setColor(Color3B::WHITE);
        (*ball_p.min[(int)RIGHT_HAND_TAG-1])->setName("CinRight");
     //   th_ball[(int)RIGHT_HAND_TAG-1]->setPosition((*ball_p.min[(int)RIGHT_HAND_TAG-1])->getPosition());
        
    }
    if(ball_p.ballInHand[(int)RIGHT_HAND_TAG+1]){
     //   (*ball_p.min[(int)RIGHT_HAND_TAG+1])->setColor(Color3B::ORANGE);
        (*ball_p.min[(int)RIGHT_HAND_TAG+1])->setName("ACinRight");
         //th_ball[(int)RIGHT_HAND_TAG+1]->setPosition((*ball_p.min[(int)RIGHT_HAND_TAG+1])->getPosition());
        
    }
    if(ball_p.ballInHand[(int)LEFT_HAND_TAG-1]){
      //  (*ball_p.min[(int)LEFT_HAND_TAG-1])->setColor(Color3B::WHITE);
        (*ball_p.min[(int)LEFT_HAND_TAG-1])->setName("CinLeft");
       //  th_ball[(int)LEFT_HAND_TAG-1]->setPosition((*ball_p.min[(int)LEFT_HAND_TAG-1])->getPosition());
    }
    if(ball_p.ballInHand[(int)LEFT_HAND_TAG+1]){
       // (*ball_p.min[(int)LEFT_HAND_TAG+1])->setColor(Color3B::ORANGE);
        (*ball_p.min[(int)LEFT_HAND_TAG+1])->setName("ACinLeft");
      //   th_ball[(int)LEFT_HAND_TAG+1]->setPosition((*ball_p.min[(int)LEFT_HAND_TAG+1])->getPosition());
    }
    
    
//         this->addChild(draw);
//    draw->setTag(5);
    auto scoreLabel=(Label*)getChildByTag(20);
    scoreLabel->setString(cocos2d::StringUtils::toString(jugScore));
    
    if(!startFlag&&jugScore==1){
        this->runAction(Sequence::create(DelayTime::create(2),CallFunc::create([this](){
            Stage::ballUpdate(0);
        }), NULL));
        this->schedule( schedule_selector(Stage::ballUpdate), 9.0 );
        this->schedule( schedule_selector(Stage::StageEffect), 4.0 );
        startFlag=true;
    }

}
void Stage::HandEffect(Stage::SPRITE_TAG hand){
        Size visibleSize = Director::getInstance()->getVisibleSize(); //画面のサイズを取得
    // 輝かせたい画像オブジェクトをタグから取得する
    Sprite*  hSprite = (Sprite*)this->getChildByTag(hand);
    
    Point handPos=hSprite->getPosition();
    // スプライトの右上と左下の座標からランダムな座標を作る

    // 画像スプライトを配置する
    Sprite* waveSprite = Sprite::create("wave.png");
    waveSprite->setPosition(handPos);
    waveSprite->setScale(1.0f);
    waveSprite->setOpacity(200);
    waveSprite->setPositionZ(-2);
    this->addChild(waveSprite, 30);
    
    auto remove = RemoveSelf::create(true);
    
    Spawn* spawn = Spawn::create(
                                 ScaleTo::create(0.3f,5),
                                 FadeTo::create(0.3f, 0),
                                 NULL);
    
    waveSprite->runAction(Sequence::create(spawn,remove,NULL));

    Spawn* spawn2 = Spawn::create(
                                 ScaleTo::create(0.3f,5),
                                 FadeTo::create(0.3f, 0),
                                 NULL);
    
    auto scoreLabelClone = Label::createWithSystemFont(cocos2d::StringUtils::toString(jugScore), "HiraKakuProN-W6", 48);
    scoreLabelClone->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    this->addChild(scoreLabelClone);
    scoreLabelClone->runAction(Sequence::create(spawn2,remove,NULL));
    
    
}
void Stage::StageEffect(float dt){
    Size visibleSize = Director::getInstance()->getVisibleSize(); //画面のサイズを取得
    
    // 画像スプライトを配置するX
    hitDelay=getSec();
//    CCLOG("%f",hitDelay);
//    for(int i=0;i<5;i++){
//        _hitTimes.push_back(i*0.5);
//    }
    std::vector<float> span;
    int i=0;
    int hitCount = (int)_hitTimes.size();
    float hitSpan =0;
    if(hitCount!=0)
        hitSpan = 4.0/(float)hitCount;
    else
        hitSpan=0;
    for(auto it = _hitTimes.begin();it != _hitTimes.end(); ++it){
        if(i%2==0){
            Sprite* waveSprite = Sprite::create("wave.png");
            Sprite* waveSprite2 = Sprite::create("wave.png");
            
            waveSprite->setPosition(Point(visibleSize.width,visibleSize.height));
            waveSprite->setScale(1.0f);
            waveSprite->setOpacity(200);
            waveSprite->setPositionZ(-2);
            waveSprite->setTag((int)WAVE_TAG);
            waveSprite->setOpacity(0);
            
            waveSprite2->setPosition(Point(0,visibleSize.height));
            waveSprite2->setScale(1.0f);
            waveSprite2->setOpacity(200);
            waveSprite2->setPositionZ(-2);
            waveSprite2->setTag((int)WAVE_TAG+1);
            waveSprite2->setOpacity(0);
            
            //float _dTime=(*it);
            this->addChild(waveSprite, 30);
            this->addChild(waveSprite2,31);
            
            auto  delay = DelayTime::create((i*hitSpan)+(hitSpan/2));
            auto remove = RemoveSelf::create(true);
            cocos2d::CallFunc *afterOpacity = CallFunc::create([this](){
                Sprite* waveSprite =(Sprite*)this->getChildByTag((int)WAVE_TAG);
                waveSprite->setOpacity(150);
                CCLOG("エメラルドスプラッシュ");
            });//:CallFunc
            cocos2d::CallFunc *afterOpacity2 = CallFunc::create([this](){
                Sprite* waveSprite2 =(Sprite*)this->getChildByTag((int)WAVE_TAG+1);
                waveSprite2->setOpacity(150);
                CCLOG("エメラルドスプラッシュ2");
            });//:CallFunc
            Spawn* spawn = Spawn::create(
                                         ScaleTo::create(0.3f,10),
                                         FadeTo::create(0.3f, 0),
                                         NULL);
            Spawn* spawn2 = Spawn::create(
                                          ScaleTo::create(0.3f,10),
                                          FadeTo::create(0.3f, 0),
                                          NULL);
            waveSprite->runAction(Sequence::create(delay,afterOpacity,spawn,remove,NULL));
            waveSprite2->runAction(Sequence::create(delay,afterOpacity2,spawn2,remove,NULL));
        }
        i++;
        
    }
    _hitTimes.clear();

    

    
}