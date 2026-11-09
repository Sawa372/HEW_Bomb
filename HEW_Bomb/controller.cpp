#include "controller.h"
#include <SDL3/SDL.h>
#include <cmath>

// グローバル変数

// コントローラー本体
static SDL_Gamepad* gamepad = nullptr;

// コントローラーの接続・切断イベント
static bool connectedEvent = false;
static bool disconnectedEvent = false;

// ボタンの状態
static bool currentButtons[ButtonCount] = {};
static bool previousButtons[ButtonCount] = {};

// イベントで検出した入力を一時保存
static bool pendingPressedButtons[ButtonCount] = {};
static bool pendingReleasedButtons[ButtonCount] = {};

// ゲーム更新1回分の入力記録
static bool framePressedButtons[ButtonCount] = {};
static bool frameReleasedButtons[ButtonCount] = {};

// トリガーのイベント上の押下状態
static bool triggerEventState[2] = {};

// スティックのデッドゾーン
static float stickDeadZone = 0.15f;

// スティック方向判定のしきい値
static float stickDirectionThreshold = 0.15f;

// トリガーの押し込み判定しきい値
static const Sint16 triggerThreshold = 16000;
//-------------------------------------------------------------------
// 以下　コントローラーの入力処理

// トリガーのイベント状態を現在の入力状態に同期
static void SyncTriggerEventState()
{
	if (gamepad == nullptr)
	{
		triggerEventState[0] = false;
		triggerEventState[1] = false;
		return;
	}

	triggerEventState[0] =
		SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER)
		> triggerThreshold;

		triggerEventState[1] =
			SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER)
		> triggerThreshold;
}

// SDLからボタンの状態を取得
static bool ReadButton(ControllerButton Button)
{
	if (gamepad == nullptr)
	{
		return false;
	}

	switch (Button)
	{
	case AButton:
		return SDL_GetGamepadButton(
			gamepad, SDL_GAMEPAD_BUTTON_EAST);

	case BButton:
		return SDL_GetGamepadButton(
			gamepad, SDL_GAMEPAD_BUTTON_SOUTH);

	case XButton:
		return SDL_GetGamepadButton(
			gamepad, SDL_GAMEPAD_BUTTON_NORTH);

	case YButton:
		return SDL_GetGamepadButton(
			gamepad, SDL_GAMEPAD_BUTTON_WEST);

	case LeftButton:
		return SDL_GetGamepadButton(
			gamepad, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER);

	case RightButton:
		return SDL_GetGamepadButton(
			gamepad, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER);

	case StartButton:
		return SDL_GetGamepadButton(
			gamepad, SDL_GAMEPAD_BUTTON_START);

	case SelectButton:
		return SDL_GetGamepadButton(
			gamepad, SDL_GAMEPAD_BUTTON_BACK);
	
	case LeftStickButton:
		return SDL_GetGamepadButton(
			gamepad, SDL_GAMEPAD_BUTTON_LEFT_STICK);
	
	case RightStickButton:
		return SDL_GetGamepadButton(
			gamepad, SDL_GAMEPAD_BUTTON_RIGHT_STICK);

	case DPadUp:
		return SDL_GetGamepadButton(
			gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP);

	case DPadDown:
		return SDL_GetGamepadButton(
			gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN);

	case DPadLeft:
		return SDL_GetGamepadButton(
			gamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT);

	case DPadRight:
		return SDL_GetGamepadButton(
			gamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT);

	case LeftTrigger:
		return SDL_GetGamepadAxis(
			gamepad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) > 16000; 

	case RightTrigger:
		return SDL_GetGamepadAxis(
			gamepad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) > 16000;
	}

	return false;
}

// SDLのボタンをゲーム内のボタンに変換
static bool ConvertButton(
	Uint8 sdlButton,
	ControllerButton& button)
{
	switch (sdlButton)
	{
	case SDL_GAMEPAD_BUTTON_EAST:
		button = AButton;
		return true;

	case SDL_GAMEPAD_BUTTON_SOUTH:
		button = BButton;
		return true;

	case SDL_GAMEPAD_BUTTON_NORTH:
		button = XButton;
		return true;

	case SDL_GAMEPAD_BUTTON_WEST:
		button = YButton;
		return true;

	case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER:
		button = LeftButton;
		return true;

	case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER:
		button = RightButton;
		return true;

	case SDL_GAMEPAD_BUTTON_START:
		button = StartButton;
		return true;

	case SDL_GAMEPAD_BUTTON_BACK:
		button = SelectButton;
		return true;

	case SDL_GAMEPAD_BUTTON_LEFT_STICK:
		button = LeftStickButton;
		return true;

	case SDL_GAMEPAD_BUTTON_RIGHT_STICK:
		button = RightStickButton;
		return true;

	case SDL_GAMEPAD_BUTTON_DPAD_UP:
		button = DPadUp;
		return true;

	case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
		button = DPadDown;
		return true;

	case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
		button = DPadLeft;
		return true;

	case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
		button = DPadRight;
		return true;
	}

	return false;
}


// 初期化
bool Controller::Initialize()
{
	if (!SDL_Init(SDL_INIT_GAMEPAD))
	{
		return false;
	}

	// 接続されているコントローラーを取得
	int count = 0;
	SDL_JoystickID* joysticks =
		SDL_GetGamepads(&count);

	if (count > 0)
	{
		gamepad = SDL_OpenGamepad(joysticks[0]);
	}

	SDL_free(joysticks);

	// トリガー状態を同期
	SyncTriggerEventState();

	return true;
}

// 更新
void Controller::Update()
{
	// 前回の状態を保存
	for (int i = 0;i < ButtonCount; i++)
	{
		previousButtons[i] = currentButtons[i];
	}

	// 接続が切れていたらポインタリセット
	if (gamepad != nullptr &&
		!SDL_GamepadConnected(gamepad))
	{
		SDL_CloseGamepad(gamepad);
		gamepad = nullptr;
	}

	// 未接続であれば新しい接続を探す
	if (gamepad == nullptr)
	{
		int count = 0;
		SDL_JoystickID* joysticks = 
			SDL_GetGamepads(&count);

		if (count > 0)
		{
			gamepad = SDL_OpenGamepad(joysticks[0]);

			// 再接続時にトリガー状態を同期
			SyncTriggerEventState();
		}

		SDL_free(joysticks);
	}

	// イベントで記録した入力を今回のフレームに反映
	for (int i = 0; i < ButtonCount; i++)
	{
		framePressedButtons[i] = pendingPressedButtons[i];
		frameReleasedButtons[i] = pendingReleasedButtons[i];

		// 次のフレームに備えてリセット
		pendingPressedButtons[i] = false;
		pendingReleasedButtons[i] = false;
	}

	// 現在のボタン状態を取得
	for (int i = 0; i < ButtonCount; i++)
	{
		currentButtons[i] =
			ReadButton(static_cast<ControllerButton>(i));
	}
}

// SDLイベントを受け取る
void Controller::ProcessEvent(const SDL_Event& event)
{
	// コントローラーが接続された
	if (event.type == SDL_EVENT_GAMEPAD_ADDED)
	{
		// まだ管理しているコントローラーがいない場合
		if (gamepad == nullptr)
		{
			gamepad = SDL_OpenGamepad(event.gdevice.which);

			if (gamepad != nullptr)
			{
				connectedEvent = true;
				SyncTriggerEventState();
			}
		}

		return;
	}

	// コントローラーが切断された
	if (event.type == SDL_EVENT_GAMEPAD_REMOVED)
	{
		if (gamepad != nullptr &&
			event.gdevice.which == SDL_GetGamepadID(gamepad))
		{
			SDL_CloseGamepad(gamepad);
			gamepad = nullptr;

			disconnectedEvent = true;
		}

		return;
	}

	// ボタンイベントの処理
	if (gamepad == nullptr)
	{
		return;
	}

	// ボタンイベント・軸イベント以外は無視
	if (event.type != SDL_EVENT_GAMEPAD_BUTTON_DOWN &&
		event.type != SDL_EVENT_GAMEPAD_BUTTON_UP &&
		event.type != SDL_EVENT_GAMEPAD_AXIS_MOTION)
	{
		return;
	}

	// イベントを発生させたコントローラーのIDを取得
	Sint32 eventGamepadID;

	if (event.type == SDL_EVENT_GAMEPAD_AXIS_MOTION)
	{
		eventGamepadID = event.gaxis.which;
	}
	else
	{
		eventGamepadID = event.gbutton.which;
	}

	// 現在管理しているコントローラーか確認
	if (eventGamepadID != SDL_GetGamepadID(gamepad))
	{
		return;
	}

	// トリガーの軸イベント
	if (event.type == SDL_EVENT_GAMEPAD_AXIS_MOTION)
	{
		int triggerIndex = -1;
		ControllerButton triggerButton;

		// 左トリガー
		if (event.gaxis.axis == SDL_GAMEPAD_AXIS_LEFT_TRIGGER)
		{
			triggerIndex = 0;
			triggerButton = LeftTrigger;
		}
		// 右トリガー
		else if (event.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHT_TRIGGER)
		{
			triggerIndex = 1;
			triggerButton = RightTrigger;
		}
		else
		{
			return;
		}

		// 押し込み量を取得
		bool isPressed = event.gaxis.value > triggerThreshold;

		// 前回の状態と比較
		if (isPressed && !triggerEventState[triggerIndex])
		{
			pendingPressedButtons[triggerButton] = true;
		}
		else if (!isPressed && triggerEventState[triggerIndex])
		{
			pendingReleasedButtons[triggerButton] = true;
		}

		// 現在の状態を保存
		triggerEventState[triggerIndex] = isPressed;

		return;
	}


	ControllerButton button;

	if (!ConvertButton(event.gbutton.button, button))
	{
		return;
	}

	int index = static_cast<int>(button);

	// 押した瞬間
	if (event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN)
	{
		pendingPressedButtons[index] = true;
	}
	// 離した瞬間
	else if (event.type == SDL_EVENT_GAMEPAD_BUTTON_UP)
	{
		pendingReleasedButtons[index] = true;
	}
}
// 終了
void Controller::Finalize()
{
	if (gamepad != nullptr)
	{
		SDL_CloseGamepad(gamepad);
		gamepad = nullptr;
	}

	SDL_QuitSubSystem(SDL_INIT_GAMEPAD);
}

// 接続確認
bool Controller::IsConnected()
{
	return gamepad != nullptr &&
		SDL_GamepadConnected(gamepad);
}

// コントローラーが接続されたか
bool Controller::WasConnected()
{
	if (connectedEvent)
	{
		connectedEvent = false;
		return true;
	}

	return false;
}

// コントローラーが切断されたか
bool Controller::WasDisconnected()
{
	if (disconnectedEvent)
	{
		disconnectedEvent = false;
		return true;
	}

	return false;
}

bool Controller::Press(ControllerButton button)
{
	int index = static_cast<int>(button);

	if (index < 0 || index >= ButtonCount)
	{
		return false;
	}


	return framePressedButtons[index];
}

bool Controller::Hold(ControllerButton button)
{
	int index = static_cast<int>(button);

	if (index < 0 || index >= ButtonCount)
	{
		return false;
	}

	return currentButtons[index];;
}

bool Controller::Release(ControllerButton button)
{
	int index = static_cast<int>(button);

	if (index < 0 || index >= ButtonCount)
	{
		return false;
	}

	return frameReleasedButtons[index];
}

// スティック入力を取得する共通処理
static float ReadAxis(SDL_GamepadAxis axis, bool invert = false)
{
	if (gamepad == nullptr)
	{
		return 0.0f;
	}

	// SDLから入力値を取得
	Sint16 value = SDL_GetGamepadAxis(gamepad, axis);

	// -1.0 ～ 1.0 に変換
	float result = static_cast<float>(value) / 32767.0f;

	// Y軸の反転
	if (invert)
	{
		result = -result;
	}

	// デッドゾーン
	const float deadZone = stickDeadZone;

	if (fabsf(result) < deadZone)
	{
		return 0.0f;
	}

	// デッドゾーン分を除去して再調整
	// 入力方向を維持
	float sign = (result < 0.0f) ? -1.0f : 1.0f;

	result = (fabsf(result) - deadZone) / (1.0f - deadZone);

	result *= sign;


	// 範囲制限
	if (result > 1.0f) result = 1.0f;
	if (result < -1.0f) result = -1.0f;

	return result;
}

// 左スティックX
float Controller::GetLeftStickX()
{
	return ReadAxis(SDL_GAMEPAD_AXIS_LEFTX);
}
// 左スティックY
float Controller::GetLeftStickY()
{
	return ReadAxis(SDL_GAMEPAD_AXIS_LEFTY, true);
}
// 右スティックX
float Controller::GetRightStickX()
{
	return ReadAxis(SDL_GAMEPAD_AXIS_RIGHTX);
}
// 右スティックY
float Controller::GetRightStickY()
{
	return ReadAxis(SDL_GAMEPAD_AXIS_RIGHTY, true);
}

// 左スティックの値を一つの変数にまとめる
StickVector Controller::GetLeftStick()
{
	StickVector stick;

	stick.x = GetLeftStickX();
	stick.y = GetLeftStickY();

	return stick;
}
// 右スティックの値を一つの変数にまとめる
StickVector Controller::GetRightStick()
{
	StickVector stick;

	stick.x = GetRightStickX();
	stick.y = GetRightStickY();

	return stick;
}

// 左入力強度
float Controller::GetLeftStickMagnitude()
{
	StickVector stick = GetLeftStick();

	float magnitude = sqrtf(
		stick.x * stick.x +
		stick.y * stick.y
	);

	if (magnitude > 1.0f)
	{
		magnitude = 1.0f;
	}

	return magnitude;
}

// 右入力強度
float Controller::GetRightStickMagnitude()
{
	StickVector stick = GetRightStick();

	float magnitude = sqrtf(
		stick.x * stick.x +
		stick.y * stick.y
	);

	if (magnitude > 1.0f)
	{
		magnitude = 1.0f;
	}

	return magnitude;
}

// スティックの角度を取得し方向分けする
static StickDirection ReadStickDirection(StickVector stick)
{
	// 入力がしきい値未満なら入力なし
	if (fabsf(stick.x) < stickDirectionThreshold &&
		fabsf(stick.y) < stickDirectionThreshold)
	{
		return StickNone;
	}

	float angle = atan2f(stick.y, stick.x) * 180.0f / 3.14159265f;

	if (angle >= -22.5f && angle < 22.5f)
		return StickRight;

	if (angle >= 22.5f && angle < 67.5f)
		return StickUpRight;

	if (angle >= 67.5f && angle < 112.5f)
		return StickUp;

	if (angle >= 112.5f && angle < 157.5f)
		return StickUpLeft;

	if (angle >= 157.5f || angle < -157.5f)
		return StickLeft;

	if (angle >= -157.5f && angle < -112.5f)
		return StickDownLeft;

	if (angle >= -112.5f && angle < -67.5f)
		return StickDown;

	return StickDownRight;
}

// 左スティックの入力方向を取得（八方向）
StickDirection Controller::GetLeftStickDirection()
{
	return ReadStickDirection(GetLeftStick());
}

// 右スティックの入力方向を取得（八方向）
StickDirection Controller::GetRightStickDirection()
{
	return ReadStickDirection(GetRightStick());
}

// スティックのデッドゾーン値変更
void Controller::SetStickDeadZone(float deadZone)
{
	if (deadZone < 0.0f)
	{
		deadZone = 0.0f;
	}

	if (deadZone > 1.0f)
	{
		deadZone = 1.0f;
	}

	stickDeadZone = deadZone;
}
// スティック方向判定のしきい値変更
void Controller::SetStickDirectionThreshold(float threshold)
{
	if (threshold < 0.0f)
	{
		threshold = 0.0f;
	}

	if (threshold > 1.0f)
	{
		threshold = 1.0f;
	}

	stickDirectionThreshold = threshold;
}

// コントローラーを振動させる
bool Controller::Rumble(
	float lowFrequency,
	float highFrequency,
	unsigned int durationMs)
{
	// コントローラーが接続されていなければ失敗
	if (gamepad == nullptr || !SDL_GamepadConnected(gamepad))
	{
		return false;
	}

	// 振動の強さを 0.0 ～ 1.0 に制限
	if (lowFrequency < 0.0f) lowFrequency = 0.0f;
	if (lowFrequency > 1.0f) lowFrequency = 1.0f;

	if (highFrequency < 0.0f) highFrequency = 0.0f;
	if (highFrequency > 1.0f) highFrequency = 1.0f;

	// SDL用の値（0 ～ 65535）に変換
	Uint16 low = static_cast<Uint16>(lowFrequency * 65535.0f);
	Uint16 high = static_cast<Uint16>(highFrequency * 65535.0f);

	// 振動を実行
	return SDL_RumbleGamepad(
		gamepad,
		low,
		high,
		static_cast<Uint32>(durationMs)
	);
}
// コントローラーの振動を停止
bool Controller::StopRumble()
{
	// コントローラーが接続されていなければ失敗
	if (gamepad == nullptr || !SDL_GamepadConnected(gamepad))
	{
		return false;
	}

	// 振動の強さを両方0にして停止
	return SDL_RumbleGamepad(
		gamepad,
		0,
		0,
		0
	);
}


// コントローラーの名前
const char* Controller::GetName()
{
	if (gamepad == nullptr)
	{
		return "Not Connected";
	}

	return SDL_GetGamepadName(gamepad);
}