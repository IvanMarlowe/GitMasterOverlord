#ifndef __DRIVER_SCENE_H__
#define __DRIVER_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class Driver : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
	void movementSnake();
    virtual bool init();
	bool isKeyPressed(cocos2d::EventKeyboard::KeyCode);
    // implement the "static create()" method manually
    CREATE_FUNC(Driver);

private:
	static std::map<cocos2d::EventKeyboard::KeyCode,
		std::chrono::high_resolution_clock::time_point> keys;
	cocos2d::Label * label;
public:
	virtual void update(float delta) override;
};

#endif // __HELLOWORLD_SCENE_H__
