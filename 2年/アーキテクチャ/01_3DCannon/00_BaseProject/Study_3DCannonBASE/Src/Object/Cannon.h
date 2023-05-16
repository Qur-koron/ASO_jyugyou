#pragma once
#include <vector>
#include <DxLib.h>
class ShotBase;

class Cannon
{

public:

	// 砲身のX回転制限(最大)
	static constexpr float MAX_ANGLE_X = 15.0f * DX_PI_F / 180.0f;
	// 砲身のX回転制限(最小)
	static constexpr float MIN_ANGLE_X = -30.0f * DX_PI_F / 180.0f;
	// 弾発射後の硬直時間
	static constexpr float SHOT_DELAY = 1.0f;
	// 爆発のサイズ
	static constexpr int BLAST_SIZE_X = 32;
	static constexpr int BLAST_SIZE_Y = 32;
	// 爆発のアニメーション数
	static constexpr int BLAST_ANIM_NUM = 16;

	// コンストラクタ
	Cannon(void);

	// デストラクタ
	~Cannon(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// 弾の取得
	std::vector<ShotBase*> GetShots(void);

private:

	// 土台
	VECTOR standScl_;
	VECTOR standPos_;
	VECTOR standRot_;
	// 砲台
	VECTOR barrelScl_;
	VECTOR barrelPos_;
	VECTOR barrelRot_;
	// 砲台からの相対座標
	VECTOR barrelLocalPos_;

	// ３ＤモデルのハンドルID
	int standModelId_;
	int barrelModelId_;
	// 弾のモデルID
	int shotModelId_;
	// 弾発射後の硬直時間計算用
	float stepShotDelay_;
	// 爆発の画像(本来は外部リソース用の管理クラスを作るべき。弾モデルも。)
	int blastImgs_[BLAST_ANIM_NUM];

	// ショット(ポインタ)
	std::vector<ShotBase*> shots_;

	// 回転操作
	void ProcessPot(void);
	// 発射操作
	void ProcessShot(void);
	// 有効な弾を取得する
	ShotBase* GetValidShot(void);

};
