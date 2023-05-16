#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "ShotBase.h"
#include "Cannon.h"

Cannon::Cannon(void)
{
}

Cannon::~Cannon(void)
{
}

void Cannon::Init(void)
{
	// Standモデル読み込み
	// 外部ファイルの３Ｄモデルをロード
	standModelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Cannon/Stand.mv1").c_str());
	standScl_ = { 0.8f, 0.8f, 0.8f };
	standPos_ = { 0.0f, 10.0f, -200.0f };
	standRot_ = { 0.0f, 0.0f, 0.0f };

	// 土台からの相対座標とする
	barrelLocalPos_ = { 0.0f, 100.0f, 0.0f };

	// Barrelモデル読み込み
	// 外部ファイルの３Ｄモデルをロード
	barrelModelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Cannon/Barrel.mv1").c_str());
	barrelScl_ = { 0.8f, 0.8f, 0.8f };
	//barrelPos_ = { 0.0f, 110.0f, -200.0f };
	// 普通
	barrelPos_ = VAdd(standPos_, barrelLocalPos_);
	barrelRot_ = { 0.0f, 0.0f, 0.0f };

	// 弾のモデル
	shotModelId_ =MV1LoadModel(
		(Application::PATH_MODEL + "Cannon/Shot.mv1").c_str());
	// 弾発射の硬直時間
	stepShotDelay_ = 0.0f;

	// 爆発エフェクト読み込み
	LoadDivGraph((Application::PATH_IMAGE + "Blast.png").c_str(),
		BLAST_ANIM_NUM, 4, 4, BLAST_SIZE_X, BLAST_SIZE_Y, blastImgs_, true);

	// 3Dモデルに制御情報を反映させる
	Update();

}

void Cannon::Update(void)
{
	// 回転操作
	ProcessPot();

	// 発射操作
	ProcessShot();

	// ３Ｄモデルの大きさを設定(引数は、x, y, zの倍率)
	MV1SetScale(standModelId_, standScl_);
	// ３Ｄモデルの位置(引数は、３Ｄ座標)
	MV1SetPosition(standModelId_, standPos_);
	// ３Ｄモデルの向き(引数は、x, y, zの回転量。単位はラジアン。)
	MV1SetRotationXYZ(standModelId_, standRot_);

	// ３Ｄモデルの大きさを設定(引数は、x, y, zの倍率)
	MV1SetScale(barrelModelId_, barrelScl_);
	// ３Ｄモデルの位置(引数は、３Ｄ座標)
	MV1SetPosition(barrelModelId_, barrelPos_);
	// ３Ｄモデルの向き(引数は、x, y, zの回転量。単位はラジアン。)
	MV1SetRotationXYZ(barrelModelId_, barrelRot_);
}

void Cannon::Draw(void)
{
	// 弾の発射位置
	//VECTOR pos = barrelPos_;
	// 砲身からの相対座標
	//VECTOR localPos = { 0.0f, 25.0f, 30.0f };
	// ローカル座標からワールド座標へ変換
	//pos = VAdd(pos, localPos);
	// 弾の発射位置目安
	//DrawSphere3D(pos, 10.0f, 10, 0x00ff00, 0x00ff00, true);
	// 砲台のモデル描画
	MV1DrawModel(standModelId_);
	// 砲身のモデル描画(一時的に半透明にする)
	//MV1SetMaterialDifColor(barrelModelId_, 0, GetColorF(1.0f, 1.0f, 1.0f, 0.5f));
	MV1DrawModel(barrelModelId_);

}

void Cannon::Release(void)
{
	// ロードされた３Ｄモデルをメモリから解放
	MV1DeleteModel(standModelId_);

	MV1DeleteModel(barrelModelId_);

	MV1DeleteModel(shotModelId_);
	for (auto shot : shots_)
	{
		shot->Release();
		delete shot;
	}

	// 読み込んだ画像の解放
	for (int i = 0; i < BLAST_ANIM_NUM; i++)
	{
		DeleteGraph(blastImgs_[i]);
	}

}

std::vector<ShotBase*> Cannon::GetShots(void)
{
	return shots_;
}

void Cannon::ProcessShot(void)
{
	auto& ins = InputManager::GetInstance();

	// 攻撃キーを押すと、弾を生成
	if (ins.IsNew(KEY_INPUT_Z) && stepShotDelay_ <= 0.0f)
	{
		// 有効な弾を取得する
		ShotBase* shot = GetValidShot();

		// 単位行列(無回転の状態)
		MATRIX matRot = MGetIdent();

		// それぞれの軸の行列を作り、更に行列を合成していく
		matRot = MMult(matRot, MGetRotX(barrelRot_.x));
		matRot = MMult(matRot, MGetRotY(barrelRot_.y));
		matRot = MMult(matRot, MGetRotZ(barrelRot_.z));

		//MATRIX matrRotY90 = MMult(matrRotY90, matrRotY90);
		//
		// 弾を生成(方向は仮で正面方向)
		//shot->CreateShot(barrelPos_, { 0.0f, 0.0f, 1.0f });
		
		// 回転行列を使用して、ベクトルを回転させる
		VECTOR dir = VTransform({ 0.0f, 0.0f, 1.0f }, matRot);

		// 弾の発射位置
		VECTOR pos = barrelPos_;

		// 砲身からの相対座標
		VECTOR localPos = { 0.0f, 25.0f, 30.0f };

		// 方向と同じ要領で、相対座標を回転
		VECTOR localPosRot = VTransform(localPos, matRot);

		// ローカル座標からワールド座標へ変換
		pos = VAdd(pos, localPosRot);

		// 弾を生成(方向は仮で正面方向)
		shot->CreateShot(pos, dir);

		// 弾発射後の硬直時間セット
		stepShotDelay_ = SHOT_DELAY;

	}

	// 弾発射後の硬直時間を減らしていく
	if (stepShotDelay_ > 0.0f)
	{
		stepShotDelay_ -= 1.0f / SceneManager::DEFAULT_FPS;
	}

}

ShotBase* Cannon::GetValidShot(void)
{
	size_t size = shots_.size();
	for (int i = 0; i < size; i++)
	{
		if (!shots_[i]->IsShot())
		{
			return shots_[i];
		}
	}

	ShotBase* shot = new ShotBase(shotModelId_, blastImgs_, BLAST_ANIM_NUM);
	shots_.push_back(shot);

	return shot;

}

void Cannon::ProcessPot(void)
{
	// InputManager& ins = InputManager::GetInstance();
	auto& ins = InputManager::GetInstance();

	// 回転量
	float rotPow = 2.0f * DX_PI_F / 180.0f;

	//横回転
	if (ins.IsNew(KEY_INPUT_RIGHT))
	{
		standRot_.y += rotPow;
		barrelRot_.y = standRot_.y;
	}

	if (ins.IsNew(KEY_INPUT_LEFT))
	{
		standRot_.y -= rotPow;
		barrelRot_.y = standRot_.y;
	}

	// 縦回転
	if (ins.IsNew(KEY_INPUT_UP))
	{
		barrelRot_.x -= rotPow;
		if (barrelRot_.x < -MAX_ANGLE_X)
		{
			barrelRot_.x = -MAX_ANGLE_X;
		}
	}

	if (ins.IsNew(KEY_INPUT_DOWN))
	{
		barrelRot_.x += rotPow;
		if (barrelRot_.x > MAX_ANGLE_X)
		{
			barrelRot_.x = MAX_ANGLE_X;
		}
	}

}
