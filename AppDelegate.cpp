#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "AppCCloudPlugin.h"
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
        //デザイン解像度の設定
        Size designResolutionSize =Size(1080,1920);
        
//        Size ResourceDpi = glview->getFrameSize();
//        
//        director->setContentScaleFactor(MIN(ResourceDpi.height / designResolutionSize.height ,
//                                            ResourceDpi.width / designResolutionSize.width));
//        log("解像度::%f,%f",ResourceDpi.height,ResourceDpi.width);
        //glview->setDesignResolutionSize(480, 320, ResolutionPolicy::SHOW_ALL);

    }
    
    glview->setDesignResolutionSize(1334,750,ResolutionPolicy::SHOW_ALL);
    // turn on display FPS
    director->setDisplayStats(true);
    
    // appC cloud 開始
    // pDirector->setDisplayStats(true)の直後で一度だけ呼び出してください
    
    AppCCloudPlugin::setMK_iOS("39a479fea3b4c7b51ac2124e736994066d66096a").start();
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}