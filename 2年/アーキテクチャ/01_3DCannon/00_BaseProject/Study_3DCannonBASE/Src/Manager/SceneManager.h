#pragma once
#include <chrono>
#include <DxLib.h>

class SceneBase;
class Fader;
class Camera;
class Grid;

class SceneManager
{

public:

	static constexpr float DEFAULT_FPS = 60.0f;

	// 重力
	static constexpr float GRAVITY = 9.81f;

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME
	};
	
	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SceneManager& GetInstance(void);

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// 状態遷移
	void ChangeScene(SCENE_ID nextId);
	// シーンIDの取得
	SCENE_ID GetSceneID(void);
	// デルタタイムの取得
	float GetDeltaTime(void) const;
	// カメラの取得
	Camera* GetCamera(void);

	// 標準ポイントライト
	VECTOR pointLightPos_;
	// 追加ポイントライト1
	int pointLight1_;
	VECTOR pointLight1Pos_;
	// 追加ポイントライト2
	int pointLight2_;
	VECTOR pointLight2Pos_;
	// 標準スポットライト
	VECTOR spotLightPos_;
	float outAngle_;
	float inAngle_;

private:

	// 静的インスタンス
	static SceneManager* instance_;

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	// フェード
	Fader* fader_;
	// 各種シーン
	SceneBase* scene_;
	// カメラ
	Camera* camera_;
	// グリッド線
	Grid* grid_;

	// シーン遷移中判定
	bool isSceneChanging_;
	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;
	
	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager(void);

	// コピーコンストラクタも同様
	SceneManager(const SceneManager&);

	// デストラクタも同様
	~SceneManager(void);

	// デルタタイムをリセットする
	void ResetDeltaTime(void);
	// シーン遷移
	void DoChangeScene(SCENE_ID sceneId);
	// フェード
	void Fade(void);

};