#include "JoyStick.h"
#include "CommonData.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"
#include "PauseScene.h"

USING_NS_CC;
using namespace CommonData;

bool JoyStick::init()
{
	if (!Layer::init()) {
		return false;
	}

	currentTouch = ROCKER;
	m_hp = PLAYER_HP;
	m_totalHp = PLAYER_HP;

	// ��ǩ��������
	TTFConfig ttfConfig;
	ttfConfig.fontFilePath = "Marker Felt.ttf";
	ttfConfig.fontSize = 20;

	//ҡ�˱�������Բ���λ��
	rocker_bg = Sprite::create("joystickBg.png");
	rocker_bg->setPosition(Vec2(110,110));
	addChild(rocker_bg);

	//ҡ�˼���Բ���λ��
	rocker = Sprite::create("joystick.png");
	rocker->setPosition(Vec2(110, 110));
	addChild(rocker);

	//�����ť
	shootButton = Sprite::create("Button_Shoot-untouch.png");
	shootButton->setPosition(Vec2(VISIBLE_SIZE.width - 110, 110));
	addChild(shootButton);

	//��ǹ��ť
	switchButton = Sprite::create("Button_Weapon-Gun-untouch.png");
	switchButton->setPosition(Vec2(VISIBLE_SIZE.width - 50, 225));
	addChild(switchButton);

	//��ͣ��ť
	pauseButton = Sprite::create("Button_Pause-untouch.png");
	pauseButton->setPosition(Vec2(VISIBLE_SIZE.width - 80, VISIBLE_SIZE.height - 40));
	addChild(pauseButton);

	//����ֵͼ��
	auto hpIco = Sprite::create("Module_HP.png");
	hpIco->setPosition(Vec2(50, VISIBLE_SIZE.height - 25));
	addChild(hpIco);

	//������
	auto hpBar = Sprite::create("Module_HP-Bar.png");
	hpBar->setAnchorPoint(Vec2(0, 0.5));
	hpBar->setPosition(Vec2(70, VISIBLE_SIZE.height - 25));
	addChild(hpBar);

	//����ֵ��ʾ
	hpLable = Label::createWithTTF(ttfConfig, StringUtils::format("%d", m_totalHp));
	hpLable->setAnchorPoint(Vec2(0, 0.5));
	hpLable->setPosition(Vec2(100, VISIBLE_SIZE.height - 28));
	addChild(hpLable);

	//�ӵ���ͼ��
	auto mpIco = Sprite::create("Module_MP.png");
	mpIco->setPosition(Vec2(250, VISIBLE_SIZE.height - 25));
	addChild(mpIco);

	//�ӵ���
	auto mpBar = Sprite::create("Module_MP-Bar.png");
	mpBar->setAnchorPoint(Vec2(0, 0.5));
	mpBar->setPosition(Vec2(270, VISIBLE_SIZE.height - 25));
	addChild(mpBar);

	//�ӵ�����ʾ
	mpLable = Label::createWithTTF(ttfConfig, StringUtils::format("10000"));
	mpLable->setAnchorPoint(Vec2(0, 0.5));
	mpLable->setPosition(Vec2(320, VISIBLE_SIZE.height - 28));
	addChild(mpLable);

	message = Label::createWithTTF(ttfConfig, StringUtils::format("Message"));
	message->setPosition(Vec2(VISIBLE_SIZE.width / 2, VISIBLE_SIZE.height / 2 - 30));
	message->setVisible(false);
	addChild(message);

	return true;
}


/*��ǰ���󱻼��뵽����ڵ��ϻ���ø÷���*/
void JoyStick::onEnter()
{
	/*���ȵ��ø����onEnter����*/
	Layer::onEnter();

	/*�������㴥���ļ���, ������auto*/
	listener = EventListenerTouchOneByOne::create();

	/*û��ʹ��lambda��ʹ�ûص������ڴ����Ķ�*/
	listener->onTouchBegan = CC_CALLBACK_2(JoyStick::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(JoyStick::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(JoyStick::onTouchEnded, this);

	/*ע�����*/
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

}

/*��ǰ����Ӹ������Ƴ�ʱ����ø÷���*/
void JoyStick::onExit()
{
	/*�ӷַ������Ƴ�ע��ļ���*/
	Director::getInstance()->getEventDispatcher()->removeEventListener(listener);

	/*�����ø����onExit()����*/
	Layer::onExit();
}

bool JoyStick::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	Vec2 touch_pos = touch->getLocation();
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	if (this->shootButton->getBoundingBox().containsPoint(touch_pos)) {
		shootButton->setTexture("Button_Shoot-touched.png");
		currentTouch = SHOOT_BUTTON;
		return true;
	}

	if (this->switchButton->getBoundingBox().containsPoint(touch_pos)) {
		currentTouch = SWITCH_BUTTON;
		return true;
	}

	if (this->pauseButton->getBoundingBox().containsPoint(touch_pos)) {
		currentTouch = PAUSE_BUTTON;
		return true;
	}
	

	currentTouch = ROCKER;
	/*��������ʼ��ʱ�� ����������λ�ú��������ĵ�λ�õľ��� < Բ�İ뾶 ���ǲ���Move*/

	/*��ȡԲ�ĵ�Ͱ뾶*/
	float radius = rocker_bg->getContentSize().width / 2;
	Vec2 center = rocker_bg->getPosition();

	if (touch->getLocation().distance(center) > radius) {
		return false;
	}
	else {
		rocker->setPosition(Vec2(touch->getLocation()));
		return true;
	}

}

void JoyStick::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

	if (currentTouch != ROCKER)return;
	/*�������ƶ���ʱ�� ����������λ�ú��������ĵ�λ�õľ��� < Բ�İ뾶 */

	/*��ȡԲ�ĵ�Ͱ뾶*/
	float radius = rocker_bg->getContentSize().width / 2;
	Vec2 center = rocker_bg->getPosition();
	/*��ȡ������λ��*/
	Vec2 touch_pos = touch->getLocation();
	float dis = touch_pos.distance(center);
	if (dis == 0) { return; }
	angle = acos((touch_pos.x - center.x) / dis);
	if (touch_pos.y > center.y) {
		checkDirection(angle);
	}
	else {
		checkDirection(-angle);
	}

	if (dis <= radius) {
		rocker->setPosition(Vec2(touch_pos));
	}
	else {
		/*������ϰ�Բ*/
		if (touch_pos.y > center.y) {
			rocker->setPosition(Vec2(center.x + radius * cos(angle), center.y + radius * sin(angle)));
		}
		else {
			rocker->setPosition(Vec2(center.x + radius * cos(angle), center.y - radius * sin(angle)));
		}
	}
}

void JoyStick::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	switch (currentTouch)
	{
	case SHOOT_BUTTON:
		switch (m_player->m_currentWeaponType)
		{
		case HAND_GUN:
			if (m_player->fire()) {
				audio->playEffect("hand_gun.wav", false, 1.0f, 1.0f, 1.0f);
			}
			break;
		case RIFLE_GUN:
			if (m_player->fire()) {
				audio->playEffect("rifle_gun.wav", false, 1.0f, 1.0f, 1.0f);
			}
			break;
		case SHOT_GUN:
			if (m_player->fire()) {
				audio->playEffect("shot_gun.wav", false, 1.0f, 1.0f, 1.0f);
			}
			break;
		case SNIPER_GUN:
			if (m_player->fire()) {
				audio->playEffect("sniper_gun.wav", false, 1.0f, 1.0f, 1.0f);
			}
			break;
		case RPG:
			if (m_player->fire()) {
				audio->playEffect("RPG.wav", false, 1.0f, 1.0f, 1.0f);
			}
			break;
		default:
			break;
		}
		shootButton->setTexture("Button_Shoot-untouch.png");
		
		this->setMp(m_player->getCurWeapon()->getBulletCount());
		currentTouch = ROCKER;
		break;
	case SWITCH_BUTTON:
		audio->playEffect("click2.wav", false, 1.0f, 1.0f, 1.0f);
		log("%s", "Weapon Changed");
		m_player->switchToNextWeapon();
		switch (m_player->m_currentWeaponType)
		{
		case HAND_GUN:
			switchButton->setTexture("Button_Weapon-Gun-untouch.png");
			break;
		case RIFLE_GUN:
			switchButton->setTexture("Button_Weapon-AK-untouch.png");
			break;
		case SHOT_GUN:
			switchButton->setTexture("Button_Weapon-SG-untouch.png");
			break;
		case SNIPER_GUN:
			switchButton->setTexture("Button_Weapon-AWP-untouch.png");
			break;
		case RPG:
			switchButton->setTexture("Button_Weapon-RPG-untouch.png");
			break;
		default:
			break;
		}
		currentTouch = ROCKER;
		this->setMp(m_player->getCurWeapon()->getBulletCount());
		break;
	case PAUSE_BUTTON:
		pauseScene = PauseScene::createScene();
		Director::getInstance()->pushScene(pauseScene);
		break;
	case ROCKER:
		rocker->setPosition(rocker_bg->getPosition());
		dir = STAY;
		break;
	default:
		break;
	}
	
}

// �Է�������ж�
void JoyStick::checkDirection(float angle)
{
	/*�ҷ���*/
	if (angle >= -M_PI / 8.0 && angle <= M_PI / 8.0) {
		dir = RIGHT;
	}
	/*�ұ��� -�˷�֮�� �� �˷�֮��*/
	if (angle >= -(M_PI / 8.0) && angle <= M_PI / 8.0) {
		dir = RIGHT;
	}
	/*���Ϸ��� �˷�֮�� �� �˷�֮����*/
	else if (angle >= M_PI / 8.0 && angle < 3 * M_PI / 8.0) {
		dir = R_UP;
	}
	/*�Ϸ��� �˷�֮���� �� �˷�֮����*/
	else if (angle >= 3 * M_PI / 8.0 && angle <= 5 * M_PI / 8.0) {
		dir = UP;
	}
	/*���Ϸ��� �˷�֮5�� �� �˷�֮����*/
	else if (angle > 5 * M_PI / 8.0 && angle < 7 * M_PI / 8.0) {
		dir = L_UP;
	}
	/*����*/
	else if ((angle >= 7 * M_PI / 8.0 && angle <= M_PI) || (angle <= -7 * M_PI / 8.0 && angle >= -M_PI)) {
		dir = LEFT;
	}
	/*���·���*/
	else if (angle > -7 * M_PI / 8.0 && angle < -5 * M_PI / 8.0) {
		dir = L_DOWN;
	}
	/*�·���*/
	else if (angle >= -5 * M_PI / 8.0 && angle <= -3 * M_PI / 8.0) {
		dir = DOWN;
	}
	/*���·���*/
	else if (angle > -3 * M_PI / 8.0 && angle < -M_PI / 8.0) {
		dir = R_DOWN;
	}

}
Sprite* JoyStick::getRocker()
{
	return rocker;
}
Sprite* JoyStick::getRockerBg()
{
	return rocker_bg;
}

float JoyStick::getAngle()
{
	return angle;
}

Direction JoyStick::getDir()
{
	return dir;
}

void JoyStick::setTotalHp(int totalHp)
{
	this->m_totalHp = totalHp;
}

void JoyStick::setHp(int hp)
{
	this->m_hp = hp;
	this->hpLable->setString(StringUtils::format("%d", m_hp));
}

void JoyStick::setTotalMp(int totalMp)
{
	this->m_totalMp = totalMp;
}

void JoyStick::setMp(int mp)
{
	this->m_mp = mp;
	this->mpLable->setString(StringUtils::format("%d", m_mp));
}

void JoyStick::sendMessage(String m)
{
	this->message->setVisible(true);
	this->message->setString(StringUtils::format("%s",m.getCString()));
	if (isScheduled("MessageDelay")) {
		unschedule("MessageDelay");
	}
	scheduleOnce([this](float dt) {
		this->message->setVisible(false);
	}, 2.0f, "MessageDelay");

}