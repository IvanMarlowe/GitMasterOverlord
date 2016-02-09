#include "Driver.h"
#include "SnakeCoords.h"
#include "Coordinates.h"
#include "GameOver.h"
#include <vector>  //for std::vector
#include <thread>
#include <string>
#include <iostream>
#include "SimpleAudioEngine.h";
#define COCOS2D_DEBUG 1
USING_NS_CC;
boolean gameOver = false;
std::vector<SnakeCoords> coordsSnake;
std::vector<SnakeCoords> coordsSnakePreviousValues;
std::vector<Sprite*> wallSprites;
std::vector<Sprite*> snakeBodies;
Sprite* burger;
Sprite* slowPotion;
Sprite* speedPotion;
Sprite* spriteHeadBoundingBox;
int burgerTag = 200;
float gx = 0;
float gy = 2;
boolean globalCount = 0;
int count = 0;
int yDeductable = 12;
int GameOverplus = 0;
float speedCap = 8;
boolean hasEaten = false;
boolean isReady = false;
boolean isHit = false;
int score = 0;
Label* scoreText;

Scene* Driver::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Driver::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}


wchar_t *convertCharArrayToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}


void Output(const char* szFormat, ...)
{
	char szBuff[1024];
	va_list arg;
	va_start(arg, szFormat);
	_vsnprintf(szBuff, sizeof(szBuff), szFormat, arg);
	va_end(arg);

	OutputDebugString(convertCharArrayToLPCWSTR(szBuff));
}



//std::thread threadMovement(movementSnake);


// on "init" you need to initialize your instance
bool Driver::init()
{
    //////////////////////////////
    // 1. super init first
	srand(time(NULL));
    if ( !Layer::init() )
    {
        return false;
    }
    
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto spriteHead = Sprite::create("snake-head-up.png");
	auto eventListener = EventListenerKeyboard::create();
	auto backgroundSprite = Sprite::create("background.jpg");
	
	
	spriteHeadBoundingBox = Sprite::create("hitboxSnake.jpg");
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	spriteHead->setPosition(Point((visibleSize.width / 2) + origin.x, (visibleSize.height / 2) + origin.y));
	spriteHead->setTag(0);

	speedPotion = Sprite::create("speedPotion.png");//Point(rand() % 300 + 30, rand() % 300 + 30)
	speedPotion->setPositionX(rand() % (429 - 61 + 1) + 61);
	speedPotion->setPositionY(rand() % (269 - 61 + 1) + 61);
	speedPotion->setTag(burgerTag + 1);

	slowPotion = Sprite::create("slowPotion.png");//Point(rand() % 300 + 30, rand() % 300 + 30)
	slowPotion->setPositionX(rand() % (429 - 61 + 1) + 61);
	slowPotion->setPositionY(rand() % (269 - 61 + 1) + 61);
	slowPotion->setTag(burgerTag + 2);

	burger = Sprite::create("water.png");
	burger->setPositionX(rand() % (429 - 61 + 1) + 61);
	burger->setPositionY(rand() % (269 - 61 + 1) + 61);
	burger->setTag(burgerTag);

	this->addChild(backgroundSprite);
	
	

	//add walls
	for (int x = 0; x < 20; x++) {
		Sprite* wall = Sprite::create("wall.png");
		wall->setPosition(x * 25, 20);
		this->addChild(wall);
		wallSprites.push_back(wall);
	}

	for (int x = 0; x < 15; x++) {
		Sprite* wall = Sprite::create("wall.png");
		wall->setPosition(10, x * 25);
		this->addChild(wall);
		wallSprites.push_back(wall);
	}

	for (int x = 0; x < 15; x++) {
		Sprite* wall = Sprite::create("wall.png");
		wall->setPosition(visibleSize.width - 10, x * 25);
		this->addChild(wall);
		wallSprites.push_back(wall);
	}

	for (int x = 0; x < 20; x++) {
		Sprite* wall = Sprite::create("wall.png");
		wall->setPosition(x * 25, visibleSize.height + 10);
		this->addChild(wall);
		wallSprites.push_back(wall);
	}

	this->addChild(speedPotion);
	this->addChild(burger);
	this->addChild(slowPotion);
	//push to arraylist of coords
	coordsSnake.push_back(SnakeCoords((visibleSize.width / 2) + origin.x,  (visibleSize.height / 2) + origin.y, NULL));
	coordsSnakePreviousValues.push_back(SnakeCoords((visibleSize.width / 2) + origin.x, (visibleSize.height / 2) + origin.y, NULL));
	snakeBodies.push_back(spriteHead);
	
	for (int x = 1; x < 3; x++) {
		snakeBodies.push_back(Sprite::create("snake-body.png"));
		snakeBodies.at(x)->setTag(x);
		this->addChild(snakeBodies.at(x));
		((Sprite*)getChildByTag(x))->setPosition(Point(coordsSnake.at(x - 1).getX(), coordsSnake.at(x - 1).getY()));
		coordsSnake.push_back(SnakeCoords(((Sprite*)getChildByTag(x))->getPositionX(), ((Sprite*)getChildByTag(x))->getPositionY(), 0));
		coordsSnakePreviousValues.push_back(SnakeCoords(coordsSnake.at(x).getX(), coordsSnake.at(x).getY() - yDeductable, 0));
	}

	this->addChild(spriteHeadBoundingBox);
	this->addChild(spriteHead);
	

	std::string stringScore;
	stringScore.append("Score: ");
	stringScore.append(std::to_string(score));
	//CCLabelBMFont *label = CCLabelBMFont::create(stringScore, "C:\\Users\\Ivan\\Desktop\\shablagoo.fnt", 100 / 1.5, kCCTextAlignmentCenter);
	scoreText = Label::createWithTTF(stringScore, "shablagoo.ttf", 15);
	scoreText->setColor(ccc3(141, 35, 35));
	scoreText->setPosition(Vec2(visibleSize.width - 100, visibleSize.height - 30));
	this->addChild(scoreText);

	//this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, spriteHead);
	//start thread
	Director::getInstance()->getOpenGLView()->setIMEKeyboardState(true);
	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		// If a key already exists, do nothing as it will already have a time stamp
		// Otherwise, set's the timestamp to now
		if (keys.find(keyCode) == keys.end()) {
			keys[keyCode] = std::chrono::high_resolution_clock::now();
		}
	};
	eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		// remove the key.  std::map.erase() doesn't care if the key doesnt exist
		keys.erase(keyCode);
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
	// Let cocos know we have an update function to be called.
	// No worries, ill cover this in more detail later on
	this->scheduleUpdate();

    return true;
}





bool Driver::isKeyPressed(EventKeyboard::KeyCode code) {
	// Check if the key is currently pressed by seeing it it's in the std::map keys
	// In retrospect, keys is a terrible name for a key/value paried datatype isnt it?
	if (keys.find(code) != keys.end())
		return true;
	return false;
}

Coordinates getPointHead(int direction) {
	float posX = 0;
	float posY = 0;
	switch (direction) {
	case 0:
		posX = coordsSnake.at(0).getX();
		posY = coordsSnake.at(0).getY() + 11.5;
		break;
	case 1:
		posX = coordsSnake.at(0).getX();
		posY = coordsSnake.at(0).getY() - 11.5;
		break;
	case 2:
		posX = coordsSnake.at(0).getX() - 11.5;
		posY = coordsSnake.at(0).getY();
		break;
	case 3:
		posX = coordsSnake.at(0).getX() + 11.5;
		posY = coordsSnake.at(0).getY();
		break;

	}
	coordsSnake.at(0).setX(posX);
	coordsSnake.at(0).setY(posY);
	return Coordinates(posX, posY);
}

Coordinates getPointBody(int direction, int index, float multi) {
	float posX = 0;
	float posY = 0;
	switch (direction) {
	case 0:
		posX = coordsSnake.at(index).getX();
		posY = coordsSnake.at(index).getY();
		break;
	case 1:
		posX = coordsSnake.at(index).getX();
		posY = coordsSnake.at(index).getY();
		break;
	case 2:
		posX = coordsSnake.at(index).getX();
		posY = coordsSnake.at(index).getY();
		break;
	case 3:
		posX = coordsSnake.at(index).getX();
		posY = coordsSnake.at(index).getY();
		break;

	}
	return Coordinates(posX, posY);
}

void Driver::movementSnake() {
	
	spriteHeadBoundingBox->setPosition(((Sprite*)getChildByTag(0))->getPosition());
	for (int x = 1; x < coordsSnake.size(); x++) {
		Coordinates bodySnakes = getPointBody(coordsSnake.at(x - 1).getDirections(), x, x);
		((Sprite*)getChildByTag(x))->setPosition(bodySnakes.getX(), bodySnakes.getY());
	}
	
}

void Driver::update(float delta) {
	Node::update(delta);
	std::string stringScore;

	if (count >= speedCap){
		for (int x = 0; x < coordsSnake.size(); x++) {
			coordsSnakePreviousValues.at(x).setX(coordsSnake.at(x).getX());
			coordsSnakePreviousValues.at(x).setY(coordsSnake.at(x).getY());
			coordsSnakePreviousValues.at(x).setDirection(coordsSnake.at(x).getDirections());
		}
	}

	if (isKeyPressed(EventKeyboard::KeyCode::KEY_UP_ARROW) || isKeyPressed(EventKeyboard::KeyCode::KEY_W)) {
		isReady = true;
		if (coordsSnake.at(0).getDirections() != 1) {
			if (coordsSnake.at(0).getDirections() != 0) {
				((Sprite*)getChildByTag(0))->setTexture(CCTextureCache::sharedTextureCache()->addImage("snake-head-up.png"));
			}//

			coordsSnake.at(0).setDirection(0);
		}
	}
	else if (isKeyPressed(EventKeyboard::KeyCode::KEY_DOWN_ARROW) || isKeyPressed(EventKeyboard::KeyCode::KEY_S)) {
		isReady = true;
		if (coordsSnake.at(0).getDirections() != 0) {
			if (coordsSnake.at(0).getDirections() != 1) {
				((Sprite*)getChildByTag(0))->setTexture(CCTextureCache::sharedTextureCache()->addImage("snake-head-down.png"));
			}
			coordsSnake.at(0).setDirection(1);
		}
	}
	else if (isKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW) || isKeyPressed(EventKeyboard::KeyCode::KEY_A)) {
		isReady = true;
		if (coordsSnake.at(0).getDirections() != 3) {
			if (coordsSnake.at(0).getDirections() != 2) {
				((Sprite*)getChildByTag(0))->setTexture(CCTextureCache::sharedTextureCache()->addImage("snake-head-left.png"));
			}
			coordsSnake.at(0).setDirection(2);
		}
	}
	else if (isKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW) || isKeyPressed(EventKeyboard::KeyCode::KEY_D)) {
		isReady = true;
		if (coordsSnake.at(0).getDirections() != 2) {
			if (coordsSnake.at(0).getDirections() != 3) {
				((Sprite*)getChildByTag(0))->setTexture(CCTextureCache::sharedTextureCache()->addImage("snake-head-right.png"));
			}
			coordsSnake.at(0).setDirection(3);
		}
	}
	else if (isKeyPressed(EventKeyboard::KeyCode::KEY_P)) {
		isReady = false;
	}
	if (count >= speedCap) {
		if (isReady) {
			Coordinates coordinates = getPointHead(coordsSnake.at(0).getDirections());
			((Sprite*)getChildByTag(0))->setPosition(coordinates.getX(), coordinates.getY());
			//set previous
			for (int x = 1; x < coordsSnake.size(); x++) {
				
				coordsSnake.at(x).setX(coordsSnakePreviousValues.at(x - 1).getX());
				coordsSnake.at(x).setY(coordsSnakePreviousValues.at(x - 1).getY());
				coordsSnake.at(x).setDirection(coordsSnakePreviousValues.at(x - 1).getDirections());

				movementSnake();
			}
			count = 0;
		}
		
	}
	
	count++;
	Rect eaterRect = ((Sprite*)getChildByTag(0))->boundingBox();
	Rect hitBoxRect = (spriteHeadBoundingBox)->boundingBox();
	if (burger->boundingBox().intersectsRect(eaterRect))
	{

		auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
		audio->playEffect("waterConsume.wav", false, 1.0f, 1.0f, 0.005f);
		score += (21 - speedCap);
		stringScore.clear();
		stringScore.append("Score: ");
		stringScore.append(std::to_string(score));
		scoreText->setString(stringScore);


		int index = snakeBodies.size();
		snakeBodies.push_back(Sprite::create("snake-body.png"));
		snakeBodies.at(index)->setTag(index);
		this->addChild(snakeBodies.at(index));
		((Sprite*)getChildByTag(index))->setPosition(Point(coordsSnake.at(index - 1).getX(), coordsSnake.at(index - 1).getY()));
		coordsSnake.push_back(SnakeCoords(((Sprite*)getChildByTag(index))->getPositionX(), ((Sprite*)getChildByTag(index))->getPositionY(), 0));
		coordsSnakePreviousValues.push_back(SnakeCoords(coordsSnake.at(index).getX(), coordsSnake.at(index).getY() - yDeductable, 0));
	
		float randx = rand() % (429 - 61 + 1) + 61;
		float randy = rand() % (269 - 61 + 1) + 61;
		CCRect targetRect = burger->getBoundingBox();
		CCRect speedPotionRect = speedPotion->getBoundingBox();
		CCRect slowPotionRect = slowPotion->getBoundingBox();

		this->removeChild(burger);
		for (int x = 0; x < snakeBodies.size(); x++) {
			CCRect snakeBodyRect = (snakeBodies.at(x))->getBoundingBox();

			while (snakeBodyRect.intersectsRect(targetRect)) {
				randx = rand() % (429 - 61 + 1) + 61;
				randy = rand() % (269 - 61 + 1) + 61;
				targetRect = CCRectMake(randx - (targetRect.size.height / 2), randy - (targetRect.size.width / 2), targetRect.size.height, targetRect.size.width);
			}
		}

		while (speedPotionRect.intersectsRect(targetRect)) {
			randx = rand() % (429 - 61 + 1) + 61;
			randy = rand() % (269 - 61 + 1) + 61;
			targetRect = CCRectMake(randx - (targetRect.size.height / 2), randy - (targetRect.size.width / 2), targetRect.size.height, targetRect.size.width);
		}

		while (slowPotionRect.intersectsRect(targetRect)) {
			randx = rand() % (429 - 61 + 1) + 61;
			randy = rand() % (269 - 61 + 1) + 61;
			targetRect = CCRectMake(randx - (targetRect.size.height / 2), randy - (targetRect.size.width / 2), targetRect.size.height, targetRect.size.width);
		}

		//rand() % 400 + 120,rand() %269 + 61// (rand() % 269 + 61 ) % (269 + 61)//rand() % 429 + 61
		burger = Sprite::create("water.png");
		burger->setPosition(randx, randy);
		this->addChild(burger);
	}

	if(speedPotion->boundingBox().intersectsRect(eaterRect))
	{

		auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
		audio->playEffect("potionConsume.wav", false, 1.0f, 1.0f, 0.005f);
		score += 3;
		stringScore.clear();
		stringScore.append("Score: ");
		stringScore.append(std::to_string(score));
		scoreText->setString(stringScore);

		float randx = rand() % (429 - 61 + 1) + 61;
		float randy = rand() % (269 - 61 + 1) + 61;
		CCRect targetRect = speedPotion->getBoundingBox();
		CCRect burgerRect = burger->getBoundingBox();
		CCRect slowPotionRect = slowPotion->getBoundingBox();

		this->removeChild(speedPotion);
		for (int x = 0; x < snakeBodies.size(); x++) {
			CCRect snakeBodyRect = (snakeBodies.at(x))->getBoundingBox();

			while (snakeBodyRect.intersectsRect(targetRect)) {
				randx = rand() % (429 - 61 + 1) + 61;
				randy = rand() % (269 - 61 + 1) + 61;
				targetRect = CCRectMake(randx - (targetRect.size.height / 2), randy - (targetRect.size.width / 2), targetRect.size.height, targetRect.size.width);
			}
		}

		while (burgerRect.intersectsRect(targetRect)) {
			randx = rand() % (429 - 61 + 1) + 61;
			randy = rand() % (269 - 61 + 1) + 61;
			targetRect = CCRectMake(randx - (targetRect.size.height / 2), randy - (targetRect.size.width / 2), targetRect.size.height, targetRect.size.width);
		}

		while (slowPotionRect.intersectsRect(targetRect)) {
			randx = rand() % (429 - 61 + 1) + 61;
			randy = rand() % (269 - 61 + 1) + 61;
			targetRect = CCRectMake(randx - (targetRect.size.height / 2), randy - (targetRect.size.width / 2), targetRect.size.height, targetRect.size.width);
		}

		//rand() % 400 + 120,rand() %269 + 61// (rand() % 269 + 61 ) % (269 + 61)//rand() % 429 + 61
		speedPotion = Sprite::create("speedPotion.png");
		speedPotion->setPosition(randx, randy);
		this->addChild(speedPotion);
	}

	if (slowPotion->boundingBox().intersectsRect(eaterRect))
	{
		score += 2;
		stringScore.clear();
		stringScore.append("Score: ");
		stringScore.append(std::to_string(score));
		scoreText->setString(stringScore);

		auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
		audio->playEffect("potionConsume.wav", false, 1.0f, 1.0f, 0.005f);

		float randx = rand() % (429 - 61 + 1) + 61;
		float randy = rand() % (269 - 61 + 1) + 61;
		CCRect targetRect = slowPotion->getBoundingBox();
		CCRect burgerRect = burger->getBoundingBox();
		CCRect speedPotionRect = speedPotion->getBoundingBox();

		this->removeChild(slowPotion);
		for (int x = 0; x < snakeBodies.size(); x++) {
			CCRect snakeBodyRect = (snakeBodies.at(x))->getBoundingBox();

			while (snakeBodyRect.intersectsRect(targetRect)) {
				randx = rand() % (429 - 61 + 1) + 61;
				randy = rand() % (269 - 61 + 1) + 61;
				targetRect = CCRectMake(randx - (targetRect.size.height / 2), randy - (targetRect.size.width / 2), targetRect.size.height, targetRect.size.width);
			}
		}

		while (burgerRect.intersectsRect(targetRect)) {
			randx = rand() % (429 - 61 + 1) + 61;
			randy = rand() % (269 - 61 + 1) + 61;
			targetRect = CCRectMake(randx - (targetRect.size.height / 2), randy - (targetRect.size.width / 2), targetRect.size.height, targetRect.size.width);
		}

		while (speedPotionRect.intersectsRect(targetRect)) {
			randx = rand() % (429 - 61 + 1) + 61;
			randy = rand() % (269 - 61 + 1) + 61;
			targetRect = CCRectMake(randx - (targetRect.size.height / 2), randy - (targetRect.size.width / 2), targetRect.size.height, targetRect.size.width);
		}

		//rand() % 400 + 120,rand() %269 + 61// (rand() % 269 + 61 ) % (269 + 61)//rand() % 429 + 61
		slowPotion = Sprite::create("slowPotion.png");
		slowPotion->setPosition(randx, randy);
		this->addChild(slowPotion);
	}


	if (isReady) {
		
		for (int x = 3; x < snakeBodies.size(); x++) {

			if (((Sprite*)getChildByTag(x))->boundingBox().intersectsRect(hitBoxRect)) {
				auto nextScene = GameOver::createScene(std::to_string(score));
				auto director = Director::getInstance();

				if (!isHit) {
					isHit = true;
					auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
					audio->playEffect("snakeBump.wav", false, 1.0f, 1.0f, 0.005f);
					CCLOG("wut");
				}
				director->replaceScene(TransitionFade::create(1.0, nextScene, Color3B(255, 255, 255)));
				
			}

		}

		
		for (int x = 0; x < wallSprites.size(); x++) {
			if (wallSprites.at(x)->boundingBox().intersectsRect(hitBoxRect)) {
				auto nextScene = GameOver::createScene(std::to_string(score));
				auto director = Director::getInstance();
				
				if (!isHit) {
					isHit = true;
					auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
					audio->playEffect("snakeBump.wav", false, 1.0f, 1.0f, 0.005f);
				}
				director->replaceScene(TransitionFade::create(1.0, nextScene, Color3B(255, 255, 255)));
			}
		}
	}
}



std::map<cocos2d::EventKeyboard::KeyCode,
	std::chrono::high_resolution_clock::time_point> Driver::keys;