//
//  ScoreCenter.cpp
//  JugJug
//
//  Created by 前原謙一 on 2014/12/02.
//
//

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
USING_NS_CC;

#include "ScoreCenter.h"
#include <jni.h>
#include  "platform/android/jni/JniHelper.h"

#define CLASS_NAME "hone/SC"

bool ScoreCenter::isAvailable() {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME,
                                       "isAvailable", "()Z")) {
        jboolean result = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
        if (result == JNI_TRUE) {
            return true;
            
        } else {
            return false;
        }
    }
    return false;
}

void ScoreCenter::login() {
    if (!isAvailable()) {
        return;
    }
    
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME,
                                       "login", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

void ScoreCenter::postScore(const char* key, const char* score) {
    if (!isAvailable()) {
        return;
    }
    
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME,
                                       "postScore", "(Ljava/lang/String;Ljava/lang/String;)V")) {
        jstring jkey   = t.env->NewStringUTF(key);
        jstring jscore = t.env->NewStringUTF(score);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jkey, jscore);
        t.env->DeleteLocalRef(jkey);
        t.env->DeleteLocalRef(jscore);
        t.env->DeleteLocalRef(t.classID);
    }
}

void ScoreCenter::showRanking(const char* key) {
    if (!isAvailable()) {
        return;
    }
    
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME,
                                       "showRanking", "(Ljava/lang/String;)V")) {
        jstring jkey = t.env->NewStringUTF(key);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jkey);
        t.env->DeleteLocalRef(jkey);
        t.env->DeleteLocalRef(t.classID);
    }
}

#endif