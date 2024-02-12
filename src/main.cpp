#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/LevelSelectLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>

#include <cmath>

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

CCSprite* m_flashlight;

    bool init (GJGameLevel* level, bool first, bool second) {
        bool results = PlayLayer::init(level, first, second);

		m_fields->m_flashlight = CCSprite::create("flashlight.png"_spr);

		if (m_player1->m_isPlatformer){
			toggle = false; // checks if its a platformer and if it is to toggle off || will rework later
		}
        if (toggle) {
            m_fields->m_flashlight->setOpacity(0);
			m_fields->m_flashlight->setScale(2);

            CCNode* node = reinterpret_cast<CCNode*>(getChildren()->objectAtIndex(1));
			CCLayer* layer = reinterpret_cast<CCLayer*>(node->getChildren()->objectAtIndex(1));

			std::string flashlight_string = "Flashlight Max %: " + std::to_string(Mod::get()->getSettingValue<int64_t>("max-percent")) + "%";

			CCLabelBMFont * flashlight_max = CCLabelBMFont::create(flashlight_string.c_str(), "bigFont.fnt");
			flashlight_max->setScale(0.4);
			flashlight_max->setPosition({83,10});
			flashlight_max->setOpacity(45);


			layer->setZOrder(41);
			layer->addChild(m_fields->m_flashlight,1000);

			log::info("count: {}", node->getChildren()->count());

			if (Mod::get()->getSettingValue<bool>("display-max")) {
				if (!Mod::get()->getSettingValue<bool>("always-dark")) {
					addChild(flashlight_max);
				}
			}
        } 
		return results;
    }

    void postUpdate(float first) {
        PlayLayer::postUpdate(first);
		auto director = CCDirector::sharedDirector();
		auto size = director->getWinSize();
		int flashlight_opacity = 0;
        if (toggle) {
			m_fields->m_flashlight->setPosition({m_player1->getPositionX() + 31, m_player1->getPositionY() - 10});
			if (Mod::get()->getSettingValue<bool>("always-dark")) {
				int flashlight_opacity = PlayLayer::getCurrentPercentInt() * 84.3;
				if (flashlight_opacity < 253) {
					m_fields->m_flashlight->setOpacity(flashlight_opacity);
				}
			}
			else {
				flashlight_opacity += PlayLayer::getCurrentPercentInt() * ((2.55f * Mod::get()->getSettingValue<int64_t>("max-opacity")) / Mod::get()->getSettingValue<int64_t>("max-percent"));
				if (flashlight_opacity <= (2.55 * Mod::get()->getSettingValue<int64_t>("max-opacity")) - 2) {
					m_fields->m_flashlight->setOpacity(flashlight_opacity);
				}
				else { //
					m_fields->m_flashlight->setOpacity((2.55 * Mod::get()->getSettingValue<int64_t>("max-opacity")) - 3);
				}
			}
        } 
    }
};

// 1.0.3 Change log
// Added Opacity slider
// Added Android64 & Android32 Support