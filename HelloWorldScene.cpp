#include "HelloWorldScene.h"
#include "StageScene.h"
#include "HowToScene.h"
#include <ui/CocosGUI.h>
#include "cocostudio/CCSGUIReader.h"
#include "ScoreCenter.h"
#include "AppCCloudPlugin.h"
#include <algorithm>
#include "editor-support/cocostudio/CocoStudio.h"
USING_NS_CC;
using namespace cocostudio;
using namespace ui;
Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
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
    

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("Title.png");

    
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));


    // add the sprite as a child to this layer
    this->addChild(sprite, 0,0);
    
    
   
    
    //タッチイベントの監視を開始
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchCancelled= CC_CALLBACK_2(HelloWorld::onTouchCancelled,this);
    listener->onTouchMoved= CC_CALLBACK_2(HelloWorld::onTouchMoved,this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto listener2 = EventListenerKeyboard::create();
    listener2->onKeyReleased = [](EventKeyboard::KeyCode keyCode, Event* unused_event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
        
            Director::getInstance()->end();
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener2, this);
    //スタートボタンを設置
    auto startButton = MenuItemImage::create(
                                             "START.png",  // 通常状態の画像
                                             "START.png",  // 押下状態の画像
                                             CC_CALLBACK_1(HelloWorld::pushStart, this)); // 押下時のアクション
    
    startButton->setPosition(Point(visibleSize.width/2 ,
                                   origin.y + startButton->getContentSize().height/2+200));
    //create menu, it's an autorelease object
    auto s_menu = Menu::create(startButton, NULL);
    s_menu->setPosition(Point::ZERO);
    this->addChild(s_menu, 2);
    
    auto freeButton = MenuItemImage::create(
                                             "Free.png",  // 通常状態の画像
                                             "Free.png",  // 押下状態の画像
                                             CC_CALLBACK_1(HelloWorld::pushFree, this)); // 押下時のアクション
    
    freeButton->setPosition(Point(visibleSize.width/2 ,
                                   origin.y + freeButton->getContentSize().height/2+100));
    
    //create menu, it's an autorelease object
    auto f_menu = Menu::create(freeButton, NULL);
    f_menu->setPosition(Point::ZERO);
    this->addChild(f_menu, 3);
    
    //遊び方ボタン設置
    auto howtoButton = MenuItemImage::create(
                                             "HOWTOUSE.png",  // 通常状態の画像
                                             "HOWTOUSE.png",  // 押下状態の画像
                                             CC_CALLBACK_1(HelloWorld::pushHowto, this)); // 押下時のアクション
    howtoButton->setPosition(Point(visibleSize.width/2  ,
                                   origin.y + howtoButton->getContentSize().height/2));
    
    //create menu, it's an autorelease object
    auto h_menu = Menu::create(howtoButton, NULL);
    h_menu->setPosition(Point::ZERO);
    this->addChild(h_menu, 4);
    
    UserDefault *userDef = UserDefault::getInstance();
    
    int highScore = userDef->getIntegerForKey("highScore", 0);
    std::string hi="highScore : ";
    auto highScoreLabel = Label::createWithSystemFont(hi.append(cocos2d::StringUtils::toString(highScore)), "HiraKakuProN-W6", 24);
    highScoreLabel->setPosition(Point(visibleSize.width-130, visibleSize.height-100));
    this->addChild(highScoreLabel);

    
    
    //Spriteを用意
    auto image1 = Sprite::create("offButton.png");
    auto image2 = Sprite::create("onButton.png");
    
    
    //MenuItemSpriteクラスを作成
    auto item1 = MenuItemSprite::create(image1, image1);
    auto item2 = MenuItemSprite::create(image2, image2);
    
    
    
    bool assistStates = userDef->getBoolForKey("assistStates", true);
    
    auto menuToggle1 = MenuItemToggle::createWithCallback([userDef,assistStates](Ref *sender){
        if(assistStates){
            userDef->setBoolForKey("assistStates", false);
        }else if(!assistStates){
            userDef->setBoolForKey("assistStates", true);
        }
    }, item1,item2,NULL);
    
    if(assistStates){
        menuToggle1->setSelectedIndex(1);
    }else{
        menuToggle1->setSelectedIndex(0);
    }
    
    auto _menu = Menu::create(menuToggle1,NULL);
    _menu->setPosition(Point(150,visibleSize.height-100));
    
    addChild(_menu);
    
    
    auto assistLabel = Label::createWithSystemFont("Assist", "HiraKakuProN-W6", 24);
    assistLabel->setPosition(Point(50, visibleSize.height-100));
    this->addChild(assistLabel);

    auto rankButton = MenuItemImage::create(
                                             "RANKING.png",  // 通常状態の画像
                                             "RANKING.png",  // 押下状態の画像
                                             CC_CALLBACK_1(HelloWorld::pushRank, this)); // 押下時のアクション
    
    rankButton->setPosition(Point(visibleSize.width*3/4+100 ,visibleSize.height/2+150));
    //create menu, it's an autorelease object
    auto rk_menu = Menu::create(rankButton, NULL);
    rk_menu->setPosition(Point::ZERO);
    this->addChild(rk_menu, 5);
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ScoreCenter::login();
#endif
    
    return true;
}


bool HelloWorld::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    Point location =pTouch->getLocation();
    // 作成したパーティクルのプロパティリストを読み込み
   
    
  ;
    CCLOG("TouchBegan");
    return true;
}

void HelloWorld::pushStart(Ref *pSender)
{
    UserDefault *userDef = UserDefault::getInstance();
    
    
    //                                                        int highScore = userDef->getIntegerForKey("highScore", 0);
    //int型をセット
    userDef->setBoolForKey("freemode", false);
    
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

void HelloWorld::pushFree(Ref *pSender)
{
    UserDefault *userDef = UserDefault::getInstance();
    
    
    //                                                        int highScore = userDef->getIntegerForKey("highScore", 0);
    //int型をセット
    userDef->setBoolForKey("freemode", true);
    
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
void HelloWorld::pushRank(Ref *pSender){
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ScoreCenter::showRanking("com.com.jugjug_scoreboard1");
#endif
}
void HelloWorld::pushHowto(Ref *pSender)
{
    
    
    // 遷移先の画面のインスタンス
    Scene *pScene = HowtoPlay::createScene();
    
    // 0.5秒かけてフェードアウトしながら次の画面に遷移します
    //    引数１:フィードの時間
    //    引数２：移動先のシーン
    //    引数３：フィードの色（オプション）
    TransitionFade* transition = TransitionFade::create(0.5f, pScene);
    
    //遷移実行  遷移時のアニメーション　http://study-cocos2d-x.info/scenelayer/55/
    Director::getInstance()->replaceScene(transition);
}

void HelloWorld::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    CCLOG("TouchEnded");
}

void HelloWorld::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    CCLOG("TouchMoved");
}

void HelloWorld::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    CCLOG("TouchCancelled");
}

