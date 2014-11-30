//
//  ResultScene.cpp
//  Hanauranai
//
//  Created by 前原謙一 on 2014/11/04.
//
//

#include "ResultScene.h"
#include "StageScene.h"
#include "HelloWorldScene.h"
#include "AppCCloudPlugin.h"
USING_NS_CC;

Scene* Result::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Result::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Result::init()
{
    //////////////////////////////
    //  super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    
    
    
    
    
    //タッチイベントの監視を開始
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(Result::onTouchBegan, this);
    listener->onTouchCancelled= CC_CALLBACK_2(Result::onTouchCancelled,this);
    listener->onTouchMoved= CC_CALLBACK_2(Result::onTouchMoved,this);
    listener->onTouchEnded = CC_CALLBACK_2(Result::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    int dby=100;
    
    //スタートボタンを設置
    auto startButton = MenuItemImage::create(
                                             "BACKbutton.png",  // 通常状態の画像
                                             "BACKbutton.png",  // 押下状態の画像
                                             CC_CALLBACK_1(Result::pushStart, this)); // 押下時のアクション
    
    startButton->setPosition(Point(visibleSize.width/4,
                                 visibleSize.height/2-dby));
    
    //create menu, it's an autorelease object
    auto s_menu = Menu::create(startButton, NULL);
    s_menu->setPosition(Point::ZERO);
    this->addChild(s_menu, 2);
    
    //リトライボタンを設置
    auto retryButton = MenuItemImage::create(
                                             "RETRYbutton.png",  // 通常状態の画像
                                             "RETRYbutton.png",  // 押下状態の画像
                                             CC_CALLBACK_1(Result::pushRetry, this)); // 押下時のアクション
    
    retryButton->setPosition(Point(visibleSize.width*3/4 ,visibleSize.height/2-dby));
    //create menu, it's an autorelease object
    auto r_menu = Menu::create(retryButton, NULL);
    r_menu->setPosition(Point::ZERO);
    this->addChild(r_menu, 3);
    
    //this->particle = particle;
    
    
//    auto scoreLabel = Label::createWithSystemFont(cocos2d::StringUtils::toString(Stage:jugScore), "HiraKakuProN-W6", 48);
//    scoreLabel->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
//    scoreLabel->setTag(20);
//    this->addChild(scoreLabel)
    

    UserDefault *userDef = UserDefault::getInstance();
    
    
    int highScore = userDef->getIntegerForKey("highScore", 0);
    //int型をセット
    int tempScore = userDef->getIntegerForKey("temp");
    
    //点数評価
    std::string honor;
    if(tempScore<=10)
        honor="GO How to Play";
    else if(10<tempScore&&tempScore<=30)
        honor="Do you play with a blindfold?";
    else if(30<tempScore&&tempScore<=50)
        honor="My grandma level";
    else if(50<tempScore&&tempScore<=80)
        honor="Otedamer";
    else if(80<tempScore&&tempScore<=100)
        honor="Pro otedamer";
    else if(100<tempScore&&tempScore<=130)
        honor="Cool Juggler";
    else if(130<tempScore&&tempScore<=150)
        honor="God juggler!!";
    else
        honor="God";
    std::string honor2="Your Rank : ";
    honor2.append(honor);
    auto honorLabel = Label::createWithSystemFont(honor2, "HiraKakuProN-W6", 40);
    honorLabel->setPosition(Point(visibleSize.width/2, visibleSize.height-70));
    this->addChild(honorLabel);

    
    CCLOG("highscore=%dtempScore=%d",highScore,tempScore);
    if(tempScore>highScore){
        userDef->setIntegerForKey("highScore", tempScore);
        auto scoreLabel = Label::createWithSystemFont(cocos2d::StringUtils::toString(tempScore), "HiraKakuProN-W6", 60);
        scoreLabel->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
        auto high = Label::createWithSystemFont("HIGHSCORE!!!!!", "HiraKakuProN-W6", 48);
        high->setPosition(Point(visibleSize.width/2, visibleSize.height/2+250));
        this->addChild(high);
        this->addChild(scoreLabel);
    }else{
        auto scoreLabel = Label::createWithSystemFont(cocos2d::StringUtils::toString(tempScore), "HiraKakuProN-W6", 48);
        scoreLabel->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
        this->addChild(scoreLabel);
        auto highScoreLabel = Label::createWithSystemFont(cocos2d::StringUtils::toString(highScore), "HiraKakuProN-W6", 24);
        highScoreLabel->setPosition(Point(visibleSize.width-100, visibleSize.height-100));
        this->addChild(highScoreLabel);
    }
    return true;
}


bool Result::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    Point location =pTouch->getLocation();
    
     
    CCLOG("TouchBegan");
    return true;
}

void Result::pushStart(Ref *pSender)
{
    AppCCloudPlugin::Ad::showCutin();
    
    
    
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
void Result::pushRetry(Ref *pSender)
{
    // 遷移先の画面のインスタンス
    Scene *pScene = Stage::createScene();
    
    // 0.5秒かけてフェードアウトしながら次の画面に遷移します
    //    引数１:フィードの時間
    //    引数２：移動先のシーン
    //    引数３：フィードの色（オプション）
    TransitionFade* transition = TransitionFade::create(0.5f, pScene);
    
    //遷移実行  遷移時のアニメーション　http://study-cocos2d-x.info/scenelayer/55/
    Director::getInstance()->replaceScene(transition);
}
void Result::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    CCLOG("TouchEnded");
}

void Result::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    CCLOG("TouchMoved");
}

void Result::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    CCLOG("TouchCancelled");
}
