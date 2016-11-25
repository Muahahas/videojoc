#pragma once
#ifndef _ENGINE_ACTIONMANAGER_CPB_1611201620428_H
#define _ENGINE_ACTIONMANAGER_CPB_1611201620428_H

#include <vector>
#include "Base\Utils\TemplatedMap.h"
#include "InputDefinitions.h"
#include "InputManager.h"
#include "Base/XML/XML.h"

namespace engine {
	namespace input {

		struct ActionTrigger {
			enum ButtonActionType
			{
				IsPressed, IsReleased, BecomesPressed, BecomesReleased
			};
			enum TriggerType {
				KEYBOARD,
				MOUSE,
				MOUSE_BUTTON,
				GAMEPAD,
				GAMEPAD_BUTTON
			} type;
			union
			{
				struct
				{
					ButtonActionType actionType;
					unsigned char keyCode;
					float value;
				} keyboard;
				struct MouseButton
				{
					ButtonActionType actionType;
					InputDefinitions::MouseButton button;
					float value;
				} mouseButton;
				struct
				{
					float threshold;
					InputDefinitions::MouseAxis axis;
					bool geThreshold;
				} mouse;
			};

			static TriggerType GetTriggerTypeFromString(const char* str, TriggerType defaultValue = (TriggerType)-1);
			static ButtonActionType GetButtonActionTypeFromString(const char* str, ButtonActionType defaultValue = (ButtonActionType)-1);
		};

		struct InputAction {
			float value;
			bool active;

			std::vector<ActionTrigger> triggers;
		};

		class CActionManager : public base::utils::CTemplatedMap < InputAction > {

		public:
			
			CActionManager(const CInputManager &inputManager);
			virtual ~CActionManager(){};

			bool LoadActions(const std::string &path);
			void SetDemoActions();

			void Update();

		private:
			const CInputManager& m_InputManager;
		};
	}
}

#endif //_ENGINE_ACTIONMANAGER_CPB_1611201620428_H
