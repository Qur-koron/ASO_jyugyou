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
    // �g�p�������e�ʂƓǂݍ��ݎ��Ԃ̍팸�̂���
    // ���f���f�[�^����������������ɑ��݂����Ȃ�
    modelId_ = MV1DuplicateModel(baseModelId_);

    // �e�̑傫����ݒ�
    scl_ = { 0.8f, 0.8f, 0.8f };
    // �e�̊p�x��ݒ�
    rot_ = { 0.0f, 0.0f, 0.0f };
    // �e�̔��ˈʒu��ݒ�
    pos_ = pos;
    // �e�̔��˕����̐ݒ�
    dir_ = dir;
    // �e�̑��x
    speed_ = 8.0f;
    // �e�̐�������
    isAlive_ = true;
    // �d��
    gravityPow_ = 0.0f;
    // �����̃A�j���[�V�����p�J�E���^
    blastCntAnim_ = 0;
    // �����̃A�j���[�V�������x
    blastSpeedAnim_ = 0.3f;
    // ��ԑJ��
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
    // �e���ړ�������
    // �ړ��ʂ̌v�Z(�����~�X�s�[�h)
    VECTOR movePow;
    //movePow.x = dir_.x * speed_;
    //movePow.y = dir_.y * speed_;
    //movePow.z = dir_.z * speed_;
    
    // �ړ�����(���W+�ړ���)
    //pos_.x += movePow.x;
    //pos_.y += movePow.y;
    //pos_.z += movePow.z;
    
    //movePow = VScale(dir_, speed_);
    //pos_ = VAdd(pos_, movePow);
    
    pos_ = VAdd(pos_, VScale(dir_, speed_));

    // �X�ɉ����x�I�ɏd�͂�������
    gravityPow_ += SceneManager::GRAVITY / SceneManager::DEFAULT_FPS;
    pos_.y -= gravityPow_;
    
    // �傫���̐ݒ�
    MV1SetScale(modelId_, scl_);
    // �p�x�̐ݒ�
    MV1SetRotationXYZ(modelId_, rot_);
    // �ʒu�̐ݒ�
    MV1SetPosition(modelId_, pos_);

}

void ShotBase::UpdateBlast(void)
{
    // �����̃A�j���[�V����
    blastCntAnim_++;

    // �����̃A�j���[�V�����ԍ�(0�`15�j
    blastIdxAnim_ = static_cast<int>(static_cast<float>(blastCntAnim_) * blastSpeedAnim_) % blastAnimNum_;

    // �����A�j���[�V�����̏I������
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
    // 2D�A�j���[�V�����̕`��
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
    // BLAST��Ԃ֑J�ڂ�����
    ChangeState(STATE::BLAST);
}

void ShotBase::ChangeState(STATE state)
{
    // ��Ԃ̍X�V
    state_ = state;

    // ��ԑJ�ڎ��̏�����
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
