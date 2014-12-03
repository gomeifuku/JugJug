//
//  ScoreCenter.h
//  JugJug
//
//  Created by 前原謙一 on 2014/12/02.
//
//

#ifndef __JugJug__ScoreCenter__
#define __JugJug__ScoreCenter__

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class ScoreCenter : public CCObject {
public:
    static bool isAvailable();
    static void login();
    static void postScore(const char* key, const char* score);
    static void showRanking(const char* key);
};

#endif /* defined(__JugJug__ScoreCenter__) */
