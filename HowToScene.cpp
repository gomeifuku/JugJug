//
//  HowToScene.cpp
//  Hanauranai
//
//  Created by 前原謙一 on 2014/11/05.
//
//

#include "HowToScene.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

Scene* HowtoPlay::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HowtoPlay::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HowtoPlay::init()
{
    //////////////////////////////
    //  super init first
    if ( !Layer::init() )
    {
        return false;
    }
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    


    //タッチイベントの監視を開始
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HowtoPlay::onTouchBegan, this);
    listener->onTouchCancelled= CC_CALLBACK_2(HowtoPlay::onTouchCancelled,this);
    listener->onTouchMoved= CC_CALLBACK_2(HowtoPlay::onTouchMoved,this);
    listener->onTouchEnded = CC_CALLBACK_2(HowtoPlay::onTouchEnded, this);
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
//    ui::Layout*	layout = ui::Layout::create();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    
//    //デバイスがiOSの時
//    auto *widget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("NewUi_1.json");
//#else
//    
//    //デバイスがAndroidの時
//    auto *widget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(FileUtils::getInstance()->fullPathForFilename("NewUi_1/NewUi_1.json").c_str());
//#endif
//    
//    
//    this->addChild(widget);
//    
//    
//    //this->addChild(layout);
//    
//    
//    if(widget){
//        cocostudio::ActionManagerEx::getInstance()->playActionByName("NewUi_1.json", "Animation0");
//    }
    
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    float x_r = visibleSize.width / 2;
    float y_r = visibleSize.height / 2;
    
    float dx=350;
    float dy=200;
    
    Sprite* r_hand= Sprite::create();
    
    Sprite* l_hand= Sprite::create();
    r_hand->setTextureRect(Rect(0, 0, 180, 180));
    r_hand->setColor(Color3B::RED);
    r_hand->setPosition(Point(x_r+dx, y_r-dy));
    
    l_hand->setTextureRect(Rect(0, 0, 180, 180));
    l_hand->setColor(Color3B::BLUE);
    l_hand->setPosition(Point(x_r-dx, y_r-dy));
    
    this->addChild(r_hand,-1,RIGHT_HAND_TAG);
    this->addChild(l_hand,-1,LEFT_HAND_TAG);
    
    Sprite* r_range= Sprite::create();
    r_range->setTextureRect(Rect(0, 0, visibleSize.width/2, visibleSize.height));
    r_range->setColor(Color3B(0,80,80));
    r_range->setPosition(Point((visibleSize.width*3)/4,visibleSize.height/2));
    r_range->setOpacity(0);
    r_range->setPositionZ(-2);
    
    Sprite* l_range= Sprite::create();
    l_range->setTextureRect(Rect(0, 0, visibleSize.width/2, visibleSize.height));
    l_range->setColor(Color3B(80,80,0));
    l_range->setPosition(Point(visibleSize.width/4,visibleSize.height/2));
    l_range->setOpacity(0);
    l_range->setPositionZ(-2);
    this->addChild(r_range,-2,R_RANGE_TAG);
    
    this->addChild(l_range,-2,L_RANGE_TAG);
    
    auto text = Label::createWithSystemFont("HowtoPlay", "HiraKakuProN-W6", 48);
    text->setPosition(Point(visibleSize/2));
    this->addChild(text);
    
    auto nextPhase = CallFunc::create([this](){
        playing=(PLAY_FLOW)((int)playing+1);
        nextFlag=true;
        CCLOG("%d",playing);
    });
    
    auto textAnimation=Sequence::create(FadeIn::create(1.0)
                                        ,DelayTime::create(1.0)
                                        ,FadeTo::create(0.3f, 0)
                                        ,RemoveSelf::create(true)
                                        ,nextPhase,NULL);
    
    text->runAction(textAnimation);
    

    this->scheduleUpdate();
    
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.5);
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("se_maoudamashii_system46.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("se_maoudamashii_system47.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("se_maoudamashii_system19.mp3");
    
    auto backButton = MenuItemImage::create(
                                            "back.png",  //表示
                                            "back.png",  //タップ時の画像
                                            CC_CALLBACK_1(HowtoPlay::pushBack, this));
    
    backButton->setPosition(Point(origin.x + visibleSize.width - backButton->getContentSize().width/2 ,
                                  origin.y + backButton->getContentSize().height/2));
    
    //create menu, it's an autorelease object
    auto menu = Menu::create(backButton, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
    
    return true;
}


bool HowtoPlay::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    Director* pDirector = Director::getInstance();
    Last_pt = pDirector -> convertToGL(pTouch -> getLocationInView());

    CCLOG("TouchBegan");
    return true;
}

void HowtoPlay::pushStart(Ref *pSender)
{
 
}

void HowtoPlay::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    //タッチポイントを取得（convertToGLにてOpenGLの座標に変換）
    Director* pDirector = Director::getInstance();
    Point touchPoint = pDirector -> convertToGL(pTouch -> getLocationInView());
    
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
    Size visibleSize = Director::getInstance()->getVisibleSize(); //画面のサイズを取得
    
    Sprite* r_range = Sprite::create();
    Sprite* l_range = Sprite::create();
    r_range->setTextureRect(Rect(0,0 ,visibleSize.width/2, visibleSize.height));
    r_range->setPosition(Point((visibleSize.width*3)/4,visibleSize.height/2));
    l_range->setTextureRect(Rect(0, 0, visibleSize.width/2, visibleSize.height));
    l_range->setPosition(visibleSize.width/4,visibleSize.height/2);

    auto r_range_rect = r_range->getBoundingBox();
    auto l_range_rect = l_range->getBoundingBox();
    if(nextFlag){
        if(playing==LEFTSWIPECLOCKBALL&&s_flag==RIGHT_SWIPE&&r_range_rect.containsPoint(touchPoint)){
            auto swipeText=this->getChildByName("swipe");
            auto swipeText2=this->getChildByName("area");
            swipeText->runAction(Sequence::create(FadeOut::create(0.5),RemoveSelf::create(), NULL));
            swipeText2->runAction(Sequence::create(FadeOut::create(0.5),RemoveSelf::create(), NULL));
            Sprite* r_range=(Sprite*)this->getChildByTag(R_RANGE_TAG);
            r_range->stopAllActions();
            
            
            r_range->runAction(FadeOut::create(0.1));
            auto ball = (Sprite*)this->getChildByTag(C_BALL_TAG);
            auto r_hand = (Sprite*)this->getChildByTag(RIGHT_HAND_TAG);
            auto l_hand = (Sprite*)this->getChildByTag(LEFT_HAND_TAG);
            Spawn* jug;
            
            const float jLow=100;
            const float _jLowTime=3.0;
            
            
            r_hand->stopAllActions();
            
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("se_maoudamashii_system46.mp3", false, 1.0f, 0.0f, 1.0f);
            HandEffect(RIGHT_HAND_TAG);
            
            jug=Spawn::create(
                              EaseOut::create(JumpTo::create(_jLowTime, l_hand->getPosition(), jLow, 1),1.5),NULL);
            
            
            ActionInterval* rotateAction = CCRotateBy::create(1, 360);
            
            //リピート
            RepeatForever* repeatForever = CCRepeatForever::create(rotateAction);
            ball->runAction(repeatForever);
            
            auto nextPhase = CallFunc::create([this](){
                playing=(PLAY_FLOW)((int)playing+1);
                nextFlag=true;
                CCLOG("%d",playing);
                
            });
            Sequence* sequence = Sequence::create(jug,nextPhase,NULL);
            sequence->setTag(1);
            ball->setPositionZ(0);
            ball->runAction(sequence);
            
            nextFlag=false;
        }
        if(playing==UPSWIPECLOCKBALL&&s_flag==UP_SWIPE&&l_range_rect.containsPoint(touchPoint)){
            Size visibleSize = Director::getInstance()->getVisibleSize(); //画面のサイズを取得
            
            auto swipeText=this->getChildByName("swipe");
            auto swipeText2=this->getChildByName("area");
            swipeText->runAction(Sequence::create(FadeOut::create(0.5),RemoveSelf::create(), NULL));
            swipeText2->runAction(Sequence::create(FadeOut::create(0.5),RemoveSelf::create(), NULL));
            Sprite* l_range=(Sprite*)this->getChildByTag(L_RANGE_TAG);
            l_range->stopAllActions();
            
            
            l_range->runAction(FadeOut::create(0.1));
            auto ball = (Sprite*)this->getChildByTag(C_BALL_TAG);
            auto l_hand = (Sprite*)this->getChildByTag(LEFT_HAND_TAG);
            Spawn* jug;
            
            
            const float _jLowTime=3.0;
            
            
            
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("se_maoudamashii_system46.mp3", false, 1.0f, 0.0f, 1.0f);
            HandEffect(LEFT_HAND_TAG);
            
            jug=Spawn::create(
                              EaseOut::create(JumpTo::create(_jLowTime,Point(visibleSize.width/2,visibleSize.height+50) , 100, 1),1.5),NULL);
            
            
            ActionInterval* rotateAction = CCRotateBy::create(1, 360);
            
            //リピート
            RepeatForever* repeatForever = CCRepeatForever::create(rotateAction);
            ball->runAction(repeatForever);
            
            auto nextPhase = CallFunc::create([this](){
                playing=(PLAY_FLOW)((int)playing+1);
                nextFlag=true;
                CCLOG("%d",playing);
                
            });
            auto remove=RemoveSelf::create();
            Sequence* sequence = Sequence::create(jug,remove,nextPhase,NULL);
            sequence->setTag(1);
            ball->setPositionZ(0);
            ball->runAction(sequence);
            
            nextFlag=false;
        }
        if(playing==RIGHTSWIPEANTICLOCKBALL&&s_flag==LEFT_SWIPE&&l_range_rect.containsPoint(touchPoint)){
            auto swipeText=this->getChildByName("swipe");
            auto swipeText2=this->getChildByName("area");
            swipeText->runAction(Sequence::create(FadeOut::create(0.5),RemoveSelf::create(), NULL));
            swipeText2->runAction(Sequence::create(FadeOut::create(0.5),RemoveSelf::create(), NULL));
            Sprite* l_range=(Sprite*)this->getChildByTag(L_RANGE_TAG);
            l_range->stopAllActions();
            
            l_range->runAction(FadeOut::create(0.1));
            auto ball = (Sprite*)this->getChildByTag(AC_BALL_TAG);
            auto r_hand = (Sprite*)this->getChildByTag(RIGHT_HAND_TAG);
            auto l_hand = (Sprite*)this->getChildByTag(LEFT_HAND_TAG);
            Spawn* jug;
            
            const float jLow=100;
            const float _jLowTime=3.0;
            
            
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("se_maoudamashii_system46.mp3", false, 1.0f, 0.0f, 1.0f);
            HandEffect(LEFT_HAND_TAG);
            
            jug=Spawn::create(
                              EaseOut::create(JumpTo::create(_jLowTime, r_hand->getPosition(), jLow, 1),1.5),NULL);
            
            
            ActionInterval* rotateAction = CCRotateBy::create(1, 360);
            
            //リピート
            RepeatForever* repeatForever = CCRepeatForever::create(rotateAction);
            ball->runAction(repeatForever);
            
            auto nextPhase = CallFunc::create([this](){
                playing=(PLAY_FLOW)((int)playing+1);
                nextFlag=true;
                CCLOG("%d",playing);
                
            });
            Sequence* sequence = Sequence::create(jug,nextPhase,NULL);
            sequence->setTag(1);
            ball->setPositionZ(0);
            ball->runAction(sequence);
            
            nextFlag=false;
        }if(playing==UPSWIPEANTICLOCKBALL&&s_flag==UP_SWIPE&&r_range_rect.containsPoint(touchPoint)){
            Size visibleSize = Director::getInstance()->getVisibleSize(); //画面のサイズを取得
            
            auto swipeText=this->getChildByName("swipe");
            auto swipeText2=this->getChildByName("area");
            swipeText->runAction(Sequence::create(FadeOut::create(0.5),RemoveSelf::create(), NULL));
            swipeText2->runAction(Sequence::create(FadeOut::create(0.5),RemoveSelf::create(), NULL));
            Sprite* r_range=(Sprite*)this->getChildByTag(R_RANGE_TAG);
            r_range->stopAllActions();
            
            
            r_range->runAction(FadeOut::create(0.1));
            auto ball = (Sprite*)this->getChildByTag(AC_BALL_TAG);
            auto r_hand = (Sprite*)this->getChildByTag(RIGHT_HAND_TAG);
            Spawn* jug;
            
            
            const float _jLowTime=3.0;
            
            
            
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("se_maoudamashii_system46.mp3", false, 1.0f, 0.0f, 1.0f);
            HandEffect(RIGHT_HAND_TAG);
            
            jug=Spawn::create(
                              EaseOut::create(JumpTo::create(_jLowTime,Point(visibleSize.width/2,visibleSize.height+50) , 100, 1),1.5),NULL);
            
            
            ActionInterval* rotateAction = CCRotateBy::create(1, 360);
            
            //リピート
            RepeatForever* repeatForever = CCRepeatForever::create(rotateAction);
            ball->runAction(repeatForever);
            
            auto nextPhase = CallFunc::create([this](){
                playing=(PLAY_FLOW)((int)playing+1);
                nextFlag=true;
                CCLOG("%d",playing);
                
            });
            auto remove=RemoveSelf::create();
            Sequence* sequence = Sequence::create(jug,remove,nextPhase,NULL);
            sequence->setTag(1);
            ball->setPositionZ(0);
            ball->runAction(sequence);
            
            nextFlag=false;
        }

    
    }
        CCLOG("TouchEnded");
}

void HowtoPlay::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    CCLOG("TouchMoved");
}

void HowtoPlay::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    CCLOG("TouchCancelled");
}

void HowtoPlay::update(float delta){
//手の説明

    if(nextFlag){
        if(playing==DISPLAYHANDS){
            auto r_hand = (Sprite*)this->getChildByTag(RIGHT_HAND_TAG);
            auto l_hand = (Sprite*)this->getChildByTag(LEFT_HAND_TAG);
            
            auto r_text = Label::createWithSystemFont("RIGHTHAND", "HiraKakuProN-W6", 24);
            r_text->setPosition(Point(r_hand->getPositionX(),r_hand->getPositionY()+150));
            this->addChild(r_text);
            
            
            auto l_text = Label::createWithSystemFont("LEFTHAND", "HiraKakuProN-W6", 24);
            l_text->setPosition(Point(l_hand->getPositionX(),l_hand->getPositionY()+150));
            this->addChild(l_text);
            
            auto nextPhase = CallFunc::create([this](){
                playing=(PLAY_FLOW)((int)playing+1);
                nextFlag=true;
                CCLOG("%d",playing);
            });
            
            auto r_textAnimation=Sequence::create(FadeIn::create(1.0)
                                                  ,DelayTime::create(1.0)
                                                  ,FadeTo::create(0.3f, 0)
                                                  ,RemoveSelf::create(true)
                                                  ,nextPhase,NULL);
            
            auto l_textAnimation=Sequence::create(FadeIn::create(1.0)
                                                  ,DelayTime::create(1.0)
                                                  ,FadeTo::create(0.3f, 0)
                                                  ,RemoveSelf::create(true)
                                                  ,NULL);
            r_text->runAction(r_textAnimation);
            l_text->runAction(l_textAnimation);
            nextFlag=false;
        }
        if(playing==DISPLAYCLOCKBALL){
            Size visibleSize = Director::getInstance()->getVisibleSize(); //画面のサイズを取得
            
            
            Sprite* r_hand=(Sprite*)this->getChildByTag(RIGHT_HAND_TAG);
 
            
        
            Sprite* ball=Sprite::create();
            
            float ball_px;
            float ball_py;
            float ball_size=30;
            ball->setTextureRect(Rect(0, 0, ball_size, ball_size));
            
            ball_px=r_hand->getPositionX()-50;
            ball_py=r_hand->getPositionY()+300;
            ball->setTag(C_BALL_TAG);
            ball->setColor(Color3B::YELLOW);
            ball->setPosition(Point(ball_px,ball_py));
            this->addChild(ball);
            
            
            auto ball_text = Label::createWithSystemFont("YELLOWBALL", "HiraKakuProN-W6", 24);
            ball_text->setPosition(Point(ball->getPositionX(),ball->getPositionY()+100));
            this->addChild(ball_text);
            
            auto nextPhase = CallFunc::create([this](){
                playing=(PLAY_FLOW)((int)playing+1);
                nextFlag=true;
                CCLOG("%d",playing);
            });
            
            auto ball_textAnimation=Sequence::create(FadeIn::create(1.0)
                                                  ,DelayTime::create(1.0)
                                                  ,FadeTo::create(0.3f, 0)
                                                  ,RemoveSelf::create(true)
                                                  ,nextPhase,NULL);
            
            ball_text->runAction(ball_textAnimation);
            
            nextFlag=false;
        }
        if(playing==MOVECLOCKBALL){
            
            auto ball = (Sprite*)this->getChildByTag(C_BALL_TAG);
            auto r_hand = (Sprite*)this->getChildByTag(RIGHT_HAND_TAG);
            Spawn* jump;
            
            int jumpHeight=100;
            jump = Spawn::create(
                                    JumpTo::create(4.0f,r_hand->getPosition(),jumpHeight,1),
                                     NULL);
            
            ActionInterval* rotateAction = CCRotateBy::create(1, 360);
            
            //リピート
            RepeatForever* repeatForever = CCRepeatForever::create(rotateAction);
            ball->runAction(repeatForever);
            auto nextPhase = CallFunc::create([this](){
                playing=(PLAY_FLOW)((int)playing+1);
                nextFlag=true;
                CCLOG("%d",playing);
                
            });
            
            Sequence* sequence = Sequence::create(jump,nextPhase,NULL);
            sequence->setTag(1);
            ball->setPositionZ(0);
            ball->runAction(sequence);

            nextFlag=false;
        }
        if(playing==LEFTSWIPECLOCKBALLINIT){
            Size visibleSize = Director::getInstance()->getVisibleSize(); //画面のサイズを取得
            auto swipeText =Label::createWithSystemFont("LEFTSWIPE", "HiraKakuProN-W6", 48);
            swipeText->setPosition(Point((visibleSize.width*3)/4,visibleSize.height/2));
            swipeText->setName("swipe");
            this->addChild(swipeText,1,ORDER_TAG);
            
            auto swipeText2 =Label::createWithSystemFont("IN  AREA!", "HiraKakuProN-W6", 48);
            swipeText2->setPosition(Point((visibleSize.width*3)/4,visibleSize.height/2-80));
            swipeText2->setName("area");
            this->addChild(swipeText2,1,ORDER_TAG);
            
            Sprite* r_range=(Sprite*)this->getChildByTag(R_RANGE_TAG);
            auto madeBlink=Sequence::create(FadeIn::create(0.3),
                                         FadeOut::create(0.3), NULL);
            auto repeat = RepeatForever::create(madeBlink);
            r_range->runAction(repeat);
            
            playing=(PLAY_FLOW)((int)playing+1);
            nextFlag=true;

        }
        if(playing==UPSWIPECLOCKBALLINIT){
            Size visibleSize = Director::getInstance()->getVisibleSize(); //画面のサイズを取得
            auto swipeText =Label::createWithSystemFont("UPSWIPE", "HiraKakuProN-W6", 48);
            swipeText->setPosition(Point((visibleSize.width)/4,visibleSize.height/2));
            swipeText->setName("swipe");
            this->addChild(swipeText,1,ORDER_TAG);
            
            auto swipeText2 =Label::createWithSystemFont("IN  AREA!", "HiraKakuProN-W6", 48);
            swipeText2->setPosition(Point((visibleSize.width)/4,visibleSize.height/2-80));
            swipeText2->setName("area");
            this->addChild(swipeText2,1,ORDER_TAG);
            
            Sprite* l_range=(Sprite*)this->getChildByTag(L_RANGE_TAG);
            auto madeBlink=Sequence::create(FadeIn::create(0.3),
                                            FadeOut::create(0.3), NULL);
            auto repeat = RepeatForever::create(madeBlink);
            l_range->runAction(repeat);
            
            playing=(PLAY_FLOW)((int)playing+1);
            nextFlag=true;
        }
        if (playing==DISPLAYANTICLOCKBALL) {
            Size visibleSize = Director::getInstance()->getVisibleSize(); //画面のサイズを取得
            
            
            Sprite* l_hand=(Sprite*)this->getChildByTag(LEFT_HAND_TAG);
            
            
            
            Sprite* ball=Sprite::create();
            
            float ball_px;
            float ball_py;
            float ball_size=30;
            ball->setTextureRect(Rect(0, 0, ball_size, ball_size));
            
            ball_px=l_hand->getPositionX()-50;
            ball_py=l_hand->getPositionY()+300;
            ball->setTag(AC_BALL_TAG);
            ball->setColor(Color3B::GREEN);
            ball->setPosition(Point(ball_px,ball_py));
            this->addChild(ball);
            
            
            auto ball_text = Label::createWithSystemFont("GREENBALL", "HiraKakuProN-W6", 24);
            ball_text->setPosition(Point(ball->getPositionX(),ball->getPositionY()+100));
            this->addChild(ball_text);
            
            auto nextPhase = CallFunc::create([this](){
                playing=(PLAY_FLOW)((int)playing+1);
                nextFlag=true;
                CCLOG("%d",playing);
            });
            
            auto ball_textAnimation=Sequence::create(FadeIn::create(1.0)
                                                     ,DelayTime::create(1.0)
                                                     ,FadeTo::create(0.3f, 0)
                                                     ,RemoveSelf::create(true)
                                                     ,nextPhase,NULL);
            
            ball_text->runAction(ball_textAnimation);
            
            nextFlag=false;
        }
        if(playing==MOVEANTICLOCKBALL){
            auto ball = (Sprite*)this->getChildByTag(AC_BALL_TAG);
            auto l_hand = (Sprite*)this->getChildByTag(LEFT_HAND_TAG);
            Spawn* jump;
            
            int jumpHeight=100;
            jump = Spawn::create(
                                 JumpTo::create(4.0f,l_hand->getPosition(),jumpHeight,1),
                                 NULL);
            
            ActionInterval* rotateAction = CCRotateBy::create(1, 360);
            
            //リピート
            RepeatForever* repeatForever = CCRepeatForever::create(rotateAction);
            ball->runAction(repeatForever);
            auto nextPhase = CallFunc::create([this](){
                playing=(PLAY_FLOW)((int)playing+1);
                nextFlag=true;
                CCLOG("%d",playing);
                
            });
            
            Sequence* sequence = Sequence::create(jump,nextPhase,NULL);
            sequence->setTag(1);
            ball->setPositionZ(0);
            ball->runAction(sequence);
            
            nextFlag=false;

        }if(playing==RIGHTSWIPEANTICLOCKBALLINIT){
            Size visibleSize = Director::getInstance()->getVisibleSize(); //画面のサイズを取得
            auto swipeText =Label::createWithSystemFont("RIGHTSWIPE", "HiraKakuProN-W6", 48);
            swipeText->setPosition(Point((visibleSize.width)/4,visibleSize.height/2));
            swipeText->setName("swipe");
            this->addChild(swipeText,1,ORDER_TAG);
            
            auto swipeText2 =Label::createWithSystemFont("IN  AREA!", "HiraKakuProN-W6", 48);
            swipeText2->setPosition(Point((visibleSize.width)/4,visibleSize.height/2-80));
            swipeText2->setName("area");
            this->addChild(swipeText2,1,ORDER_TAG);
            
            Sprite* l_range=(Sprite*)this->getChildByTag(L_RANGE_TAG);
            auto madeBlink=Sequence::create(FadeIn::create(0.3),
                                            FadeOut::create(0.3), NULL);
            auto repeat = RepeatForever::create(madeBlink);
            l_range->runAction(repeat);
            
            playing=(PLAY_FLOW)((int)playing+1);
            nextFlag=true;
        }if(playing==UPSWIPEANTICLOCKBALLINIT){
            Size visibleSize = Director::getInstance()->getVisibleSize(); //画面のサイズを取得
            auto swipeText =Label::createWithSystemFont("UPSWIPE", "HiraKakuProN-W6", 48);
            swipeText->setPosition(Point((visibleSize.width*3)/4,visibleSize.height/2));
            swipeText->setName("swipe");
            this->addChild(swipeText,1,ORDER_TAG);
            
            auto swipeText2 =Label::createWithSystemFont("IN  AREA!", "HiraKakuProN-W6", 48);
            swipeText2->setPosition(Point((visibleSize.width*3)/4,visibleSize.height/2-80));
            swipeText2->setName("area");
            this->addChild(swipeText2,1,ORDER_TAG);
            
            Sprite* r_range=(Sprite*)this->getChildByTag(R_RANGE_TAG);
            auto madeBlink=Sequence::create(FadeIn::create(0.3),
                                            FadeOut::create(0.3), NULL);
            auto repeat = RepeatForever::create(madeBlink);
            r_range->runAction(repeat);
            
            playing=(PLAY_FLOW)((int)playing+1);
            nextFlag=true;
        }if(playing==END){
            Size visibleSize = Director::getInstance()->getVisibleSize(); //画面のサイズを取得
            auto text = Label::createWithSystemFont("Play This Game!", "HiraKakuProN-W6", 48);
            text->setPosition(Point(visibleSize/2));
            this->addChild(text);
            
            auto nextPhase = CallFunc::create([this](){
                playing=(PLAY_FLOW)((int)playing+1);
                nextFlag=true;
                CCLOG("%d",playing);
            });
            text->runAction(Sequence::create(FadeIn::create(1.0)                                          ,DelayTime::create(4.0)
                                             ,FadeTo::create(0.3f, 0)
                                             ,RemoveSelf::create(true)
                                             ,nextPhase,NULL));
            nextFlag=false;
        }if(playing==BACK){
            Scene *pScene = HelloWorld::createScene();
            
            // 0.5秒かけてフェードアウトしながら次の画面に遷移します
            //    引数１:フィードの時間
            //    引数２：移動先のシーン
            //    引数３：フィードの色（オプション）
            TransitionFade* transition = TransitionFade::create(0.5f, pScene);
            //遷移実行  遷移時のアニメーション　http://study-cocos2d-x.info/scenelayer/55/
            Director::getInstance()->replaceScene(transition);
        }
    }

    s_flag=NONE_SWIPE;
}


void HowtoPlay::HandEffect(HowtoPlay::SPRITE_TAG hand){
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
}
void HowtoPlay::pushBack(Ref *pSender)
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
