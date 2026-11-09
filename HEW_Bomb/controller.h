#pragma once
union SDL_Event;

// ベクトル値をまとめる
struct StickVector
{
	float x;
	float y;
};

// スティックの方向を分ける（八方向）
enum StickDirection
{
	StickNone,

	StickUp,
	StickUpRight,

	StickRight,
	StickDownRight,

	StickDown,
	StickDownLeft,

	StickLeft,
	StickUpLeft
};

// ゲーム内で使用するボタン
enum ControllerButton
{
	// 右ボタン群
	AButton,
	BButton,
	XButton,
	YButton,

	LeftButton,  // L1
	RightButton, // R1

	StartButton,  // スタート
	SelectButton, // セレクト

	LeftStickButton,  // Lスティック押し込み
	RightStickButton,  // Rスティック押し込み

	// 十字ボタン
	DPadUp,
	DPadDown,
	DPadLeft,
	DPadRight,
	
	LeftTrigger,  // L2
	RightTrigger,  // R2

	ButtonCount
};


class Controller
{
public:

	// 初期化
	static bool Initialize();

	// 更新
	static void Update();

	// SDLイベントを受け取る
	static void ProcessEvent(const SDL_Event& event);

	// 終了
	static void Finalize();

	// 接続確認
	static bool IsConnected();
	// 接続確認(再接続されたか)
	static bool WasConnected();
	// 接続確認(切断されたか)
	static bool WasDisconnected();


	// ボタン入力
	static bool Press(ControllerButton button);
	static bool Hold(ControllerButton button);
	static bool Release(ControllerButton button);

	// スティック入力
	static float GetLeftStickX();
	static float GetLeftStickY();

	static float GetRightStickX();
	static float GetRightStickY();

	// スティックの入力ベクトルをまとめる
	static StickVector GetLeftStick();
	static StickVector GetRightStick();

	// スティックの入力強度を取得
	static float GetLeftStickMagnitude();
	static float GetRightStickMagnitude();

	// スティックの入力方向を取得（八方向）
	static StickDirection GetLeftStickDirection();
	static StickDirection GetRightStickDirection();

	// スティックのデッドゾーン値変更
	static void SetStickDeadZone(float deadZone);
	// スティック方向判定のしきい値変更
	static void SetStickDirectionThreshold(float threshold = 0.15);


	// コントローラーの振動
	static bool Rumble(
		float lowFrequency,		// 低周波モーター（左）の強さ
		float highFrequency,	// 高周波モーター（右）の強さ
		unsigned int durationMs	// 振動時間（ミリ秒
	);
	// コントローラーの振動を停止
	static bool StopRumble();


	// コントローラーの名前
	static const char* GetName();


	const char* GetDirectionName(StickDirection direction);
};