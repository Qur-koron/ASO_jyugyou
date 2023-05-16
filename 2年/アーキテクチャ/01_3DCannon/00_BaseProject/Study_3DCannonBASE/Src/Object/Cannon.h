#pragma once
#include <vector>
#include <DxLib.h>
class ShotBase;

class Cannon
{

public:

	// �C�g��X��]����(�ő�)
	static constexpr float MAX_ANGLE_X = 15.0f * DX_PI_F / 180.0f;
	// �C�g��X��]����(�ŏ�)
	static constexpr float MIN_ANGLE_X = -30.0f * DX_PI_F / 180.0f;
	// �e���ˌ�̍d������
	static constexpr float SHOT_DELAY = 1.0f;
	// �����̃T�C�Y
	static constexpr int BLAST_SIZE_X = 32;
	static constexpr int BLAST_SIZE_Y = 32;
	// �����̃A�j���[�V������
	static constexpr int BLAST_ANIM_NUM = 16;

	// �R���X�g���N�^
	Cannon(void);

	// �f�X�g���N�^
	~Cannon(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// �e�̎擾
	std::vector<ShotBase*> GetShots(void);

private:

	// �y��
	VECTOR standScl_;
	VECTOR standPos_;
	VECTOR standRot_;
	// �C��
	VECTOR barrelScl_;
	VECTOR barrelPos_;
	VECTOR barrelRot_;
	// �C�䂩��̑��΍��W
	VECTOR barrelLocalPos_;

	// �R�c���f���̃n���h��ID
	int standModelId_;
	int barrelModelId_;
	// �e�̃��f��ID
	int shotModelId_;
	// �e���ˌ�̍d�����Ԍv�Z�p
	float stepShotDelay_;
	// �����̉摜(�{���͊O�����\�[�X�p�̊Ǘ��N���X�����ׂ��B�e���f�����B)
	int blastImgs_[BLAST_ANIM_NUM];

	// �V���b�g(�|�C���^)
	std::vector<ShotBase*> shots_;

	// ��]����
	void ProcessPot(void);
	// ���ˑ���
	void ProcessShot(void);
	// �L���Ȓe���擾����
	ShotBase* GetValidShot(void);

};
