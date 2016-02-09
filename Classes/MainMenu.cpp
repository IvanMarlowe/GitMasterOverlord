#include "MainMenu.h"
#include "Driver.h"
#include "SnakeCoords.h"
#include "Coordinates.h"
#include <vector>  //for std::vector
#include <thread>
#include <string>
#include <iostream>
#include "SimpleAudioEngine.h";
#define COCOS2D_DEBUG 1
USING_NS_CC;
Scene* MainMenu::createScene()
{

	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainMenu::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}



//std::thread threadMovement(movementSnake);


// on "init" you need to initialize your instance
bool MainMenu::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto backgroundSprite = Sprite::create("isnaykersbg.jpg");
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	backgroundSprite->setScaleX((visibleSize.width / backgroundSprite->getContentSize().width) * 1);
	backgroundSprite->setScaleY((visibleSize.height / backgroundSprite->getContentSize().height) * 1);
	this->addChild(backgroundSprite);

	auto TitleName = Label::createWithTTF("Isnaykers", "shablagoo.ttf", 60);
	TitleName->setColor(ccc3(141, 35, 35));
	TitleName->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2 + origin.y + visibleSize.height * 0.2));
	this->addChild(TitleName);


	auto playAgain = MenuItemFont::create("Play", CC_CALLBACK_1(MainMenu::PlayNow, this));
	playAgain->setFontSize(15);
	playAgain->setColor(ccc3(141, 35, 35));
	playAgain->setFontNameObj("shablagoo.ttf");
	playAgain->setPosition(Point(visibleSize.width / 2 + origin.x, (visibleSize.height / 2 + origin.y) - visibleSize.height * 0.1));

	auto exitGame = MenuItemFont::create("Exit", CC_CALLBACK_1(MainMenu::ExitNow, this));
	exitGame->setFontSize(15);
	exitGame->setColor(ccc3(141, 35, 35));
	exitGame->setFontNameObj("shablagoo.ttf");
	exitGame->setPosition(Point(visibleSize.width / 2 + origin.x, (visibleSize.height / 2 + origin.y) - visibleSize.height * 0.20));

	Menu *menu = Menu::create(playAgain, exitGame, NULL);
	menu->setPosition(Point(0, 0));
	this->addChild(menu);


	return true;
}



void MainMenu::PlayNow(cocos2d::Ref *pSender) {
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect("buttonClick.wav", false, 1.0f, 1.0f, 0.005f);
	auto director = Director::getInstance();
	auto newScene = Driver::createScene();
	director->replaceScene(TransitionFade::create(1.0, newScene, Color3B(0, 0, 0)));


}

void MainMenu::ExitNow(cocos2d::Ref *pSender) {
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect("buttonClick.wav", false, 1.0f, 1.0f, 0.005f);
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
		MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
		return;
	#endif

		Director::getInstance()->end();

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
	#endif
}

