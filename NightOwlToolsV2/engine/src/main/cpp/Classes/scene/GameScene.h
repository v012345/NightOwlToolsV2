#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "platform/AEStdC.h"
#include "cocos2d.h"

USING_NS_AE;

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void onEnter();
	virtual void onExit();
    
    // implement the "static create()" method manually
	CREATE_FUNC(GameScene);

private:

};

#endif // __GAME_SCENE_H__