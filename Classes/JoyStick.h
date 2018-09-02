#ifndef _JOY_STICK_H_
#define _JOY_STICK_H_

#include "cocos2d.h"
#include "CommonData.h"
#include "Player.h"
#include "PauseScene.h"

using namespace CommonData;

typedef enum {
	ROCKER,
	SHOOT_BUTTON,
	SWITCH_BUTTON,
	PAUSE_BUTTON
}TOUCH_TYPE;

class JoyStick : public cocos2d::Layer   //JoyStick��Ӳ�̳�
{
public:
	virtual bool init() override;
	
	CREATE_FUNC(JoyStick);

	virtual void onEnter() override;
	virtual void onExit() override;

	// �������¼�
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;

	/*��鵱ǰ�ǶȲ����÷���*/
	void checkDirection(float angle);

	cocos2d::Sprite* getRocker();
	cocos2d::Sprite* getRockerBg();
	float getAngle();
	Direction getDir();
	PLAYER *m_player;
	Scene* pauseScene;
	Label* hpLable;
	Label* mpLable;

	void setTotalHp(int totalHp);
	void setHp(int hp);

	void setTotalMp(int totalMp);
	void setMp(int mp);

	void sendMessage(cocos2d::String m);

private:
	cocos2d::EventListenerTouchOneByOne* listener;  /*����Ϊ��Ա�����������Ƴ�����*/
	cocos2d::Sprite* rocker; 
	cocos2d::Sprite* rocker_bg;

	//��ť
	cocos2d::Sprite* shootButton;
	cocos2d::Sprite* switchButton;
	cocos2d::Sprite* pauseButton;
	Direction dir;
	float angle;
	TOUCH_TYPE currentTouch;

	//��ֵ
	int m_totalHp;
	int m_hp;
	int m_totalMp;
	int m_mp;

	//Message
	cocos2d::Label* message;
};

#endif // !_JOY_STICK_H_
