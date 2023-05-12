#include "../Manager/SceneManager.h"
#include "Shotbase.h"

ShotBase::ShotBase(int baseModelId, int* blastImags, int blastAnimNum)
{
    baseModelId_ = baseModelId;
    blastImgs_ = blastImags;
    blastAnimNum_ = blastAnimNum;

}

ShotBase::~ShotBase(void)
{
}

void ShotBase::CreateShot(VECTOR pos, VECTOR dir)
{
    // 使用メモリ容量と読み込み時間の削減のため
    // モデルデータをいくつもメモリ上に存在させない
    modelId_ = MV1DuplicateModel(baseModelId_);

    // 弾の大きさを設定
    scl_ = { 0.8f, 0.8f, 0.8f };
    // 弾の角度を設定
    rot_ = { 0.0f, 0.0f, 0.0f };
    // 弾の発射位置を設定
    pos_ = pos;
    // 弾の発射方向の設定
    dir_ = dir;
    // 弾の速度
    speed_ = 8.0f;
    // 弾の生存判定
    isAlive_ = true;
    // 重力
    gravityPow_ = 0.0f;
    // 爆発のアニメーション用カウンタ
    blastCntAnim_ = 0;
    // 爆発のアニメーション速度
    blastSpeedAnim_ = 0.3f;
    // 状態遷移
    ChangeState(STATE::SHOT);

}

void ShotBase::Update(void)
{

    switch (state_)
    {
    case ShotBase::STATE::SHOT:
        UpdateShot();
        break;
    case ShotBase::STATE::BLAST:
        UpdateBlast();
        break;
    case ShotBase::STATE::END:
        UpdateEnd();
        break;
    }

}

void ShotBase::UpdateShot(void)
{
    // 弾を移動させる
    // 移動量の計算(方向×スピード)
    VECTOR movePow;
    //movePow.x = dir_.x * speed_;
    //movePow.y = dir_.y * speed_;
    //movePow.z = dir_.z * speed_;
    
    // 移動処理(座標+移動量)
    //pos_.x += movePow.x;
    //pos_.y += movePow.y;
    //pos_.z += movePow.z;
    
    //movePow = VScale(dir_, speed_);
    //pos_ = VAdd(pos_, movePow);
    
    pos_ = VAdd(pos_, VScale(dir_, speed_));

    // 更に加速度的に重力を加える
    gravityPow_ += SceneManager::GRAVITY / SceneManager::DEFAULT_FPS;
    pos_.y -= gravityPow_;
    
    // 大きさの設定
    MV1SetScale(modelId_, scl_);
    // 角度の設定
    MV1SetRotationXYZ(modelId_, rot_);
    // 位置の設定
    MV1SetPosition(modelId_, pos_);

}

void ShotBase::UpdateBlast(void)
{
    // 爆発のアニメーション
    blastCntAnim_++;

    // 爆発のアニメーション番号(0～15）
    blastIdxAnim_ = static_cast<int>(static_cast<float>(blastCntAnim_) * blastSpeedAnim_) % blastAnimNum_;

    // 爆発アニメーションの終了判定
    if (blastIdxAnim_ + 1 >= blastAnimNum_)
    {
        ChangeState(STATE::END);
    }

}

void ShotBase::UpdateEnd(void)
{
}

void ShotBase::Draw()
{
    switch (state_)
    {
    case ShotBase::STATE::SHOT:
        DrawShot();
        break;
    case ShotBase::STATE::BLAST:
        DrawBlast();
        break;
    case ShotBase::STATE::END:
        DrawEnd();
        break;
    }

}

void ShotBase::DrawShot(void)
{
    MV1DrawModel(modelId_);

}

void ShotBase::DrawBlast(void)
{
    // 2Dアニメーションの描画
    DrawBillboard3D(pos_, 0.5f, 0.5f, 80.0f, 0.0f, blastImgs_[blastIdxAnim_], true);

}

void ShotBase::DrawEnd(void)
{
}

void ShotBase::Release(void)
{
    MV1DeleteModel(modelId_);

}

bool ShotBase::IsShot(void)
{
    return state_ == STATE::SHOT;
}

bool ShotBase::IsAlive(void)
{
    return isAlive_;
}

VECTOR ShotBase::GetPos(void)
{
    return pos_;
}

void ShotBase::Blast(void)
{
    // BLAST状態へ遷移させる
    ChangeState(STATE::BLAST);
}

void ShotBase::ChangeState(STATE state)
{
    // 状態の更新
    state_ = state;

    // 状態遷移時の初期化
    switch (state_)
    {
    case ShotBase::STATE::SHOT:
        break;
    case ShotBase::STATE::BLAST:
        blastCntAnim_ = 0;
        break;
    case ShotBase::STATE::END:
        break;
    }

}
