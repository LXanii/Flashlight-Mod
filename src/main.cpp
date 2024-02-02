#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/LevelSelectLayer.hpp>

using namespace geode::prelude;

bool toggle = false;

CCSprite* box;

class Flashlight_Btn {
	public:
		void flashlight_toggle(CCObject*);
};

void Flashlight_Btn::flashlight_toggle(CCObject*) {
	if (toggle){
		toggle = false;
		box->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("GJ_checkOff_001.png"));
	}
	else {
		toggle = true;
		box->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("GJ_checkOn_001.png"));
	}
}

class $modify(LevelInfoLayer) {
	bool init(GJGameLevel* level, bool first) {
		//toggle = false;
		bool result = LevelInfoLayer::init(level,first); // adds code AFTER execution
		auto director = CCDirector::sharedDirector();
		auto size = director->getWinSize(); // -> cuz its a pointer
		CCLabelBMFont* fl_text = CCLabelBMFont::create("FL", "goldFont.fnt");
		if (toggle){
			box = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
		}
		else {
			box = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
		}
		auto button = CCMenuItemSpriteExtra::create(box, this, menu_selector(Flashlight_Btn::flashlight_toggle)); // this shows its in the garagelayer, (current layer)
		auto menu = CCMenu::create();
		menu->setPosition({size.width/2 + 96.5f,size.height/2 - 76});
		fl_text->setPosition({size.width/2 + 211,size.height/2 + 13});
		menu->setScale(0.6f);
		menu->addChild(button);

		fl_text->setScale(0.5f);
		addChild(fl_text);
		addChild(menu);
		return result;
	}
};

class $modify(EditLevelLayer) {
	bool init(GJGameLevel* level) {
		toggle = false;
		bool result = EditLevelLayer::init(level); // adds code AFTER execution
		auto director = CCDirector::sharedDirector();
		auto size = director->getWinSize(); // -> cuz its a pointer
		CCLabelBMFont* fl_text = CCLabelBMFont::create("FL", "goldFont.fnt");
		if (toggle){
			box = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
		}
		else {
			box = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
		}
		auto button = CCMenuItemSpriteExtra::create(box, this, menu_selector(Flashlight_Btn::flashlight_toggle)); // this shows its in the garagelayer, (current layer)
		auto menu = CCMenu::create();
		menu->setPosition({size.width/2 + 96.5f,size.height/2 - 76});
		menu->setScale(0.6f);
		menu->addChild(button);
		fl_text->setScale(0.5f);
		fl_text->setPosition({size.width/2 + 211,size.height/2 - 32});

		addChild(fl_text);
		addChild(menu);
		return result;
	}
};

class $modify(LevelSelectLayer) {
	bool init(int first) {
		toggle = false;
		bool result = LevelSelectLayer::init(first); // adds code AFTER execution
		auto director = CCDirector::sharedDirector();
		auto size = director->getWinSize(); // -> cuz its a pointer
		CCLabelBMFont* fl_text = CCLabelBMFont::create("FL", "goldFont.fnt");
		if (toggle){
			box = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
		}
		else {
			box = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
		}
		auto button = CCMenuItemSpriteExtra::create(box, this, menu_selector(Flashlight_Btn::flashlight_toggle)); // this shows its in the garagelayer, (current layer)
		auto menu = CCMenu::create();
		menu->setPosition({size.width/2 + 96.5f,size.height/2 - 136});
		menu->setScale(0.6f);
		menu->addChild(button);
		fl_text->setScale(0.5f);
		fl_text->setPosition({size.width/2 + 211,size.height/2 - 92});

		addChild(fl_text);
		addChild(menu);
		return result;
	}
};

class $modify(PlayLayer) {

CCSprite* flashlight;

    bool init (GJGameLevel* level, bool first, bool second) {
        bool results = PlayLayer::init(level, first, second);
        int flashlight_opacity = PlayLayer::getCurrentPercentInt() * 2.55;
		flashlight = CCSprite::create("flashlight.png"_spr);
        if (toggle) {
            flashlight->setOpacity(flashlight_opacity);
			flashlight->setScale(2);
            CCNode* node = reinterpret_cast<CCNode*>(getChildren()->objectAtIndex(1));
			CCLayer* layer = reinterpret_cast<CCLayer*>(node->getChildren()->objectAtIndex(1));
			layer->setZOrder(41);
			layer->addChild(flashlight,1000);
			log::info("count: {}", node->getChildren()->count());
        } 
		return results;
    }

    void postUpdate(float first) {
        PlayLayer::postUpdate(first);
		auto director = CCDirector::sharedDirector();
		auto size = director->getWinSize();
        int flashlight_opacity = PlayLayer::getCurrentPercentInt() * 2.83;
        if (toggle) {
			flashlight->setPosition({m_player1->getPositionX() + 31, m_player1->getPositionY() - 10});
			if (flashlight_opacity < 253) {
				flashlight->setOpacity(flashlight_opacity);
			}
			else {
				if (PlayLayer::getCurrentPercentInt() == 100) {
					toggle = false;
				}
			}
        } 
    }
};