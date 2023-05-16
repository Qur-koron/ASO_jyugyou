#pragma once
#include <vector>
#include "SceneBase.h"
class Stage;
class Cannon;
class EnemyBase;

class GameScene : public SceneBase
{

public:

	// �Q�[���I�[�o�[�n�_�Փ˔���p���̔��a
	static constexpr float OVER_COL_RADIUS = 35.0f;

	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// �Q�[���I�[�o�[����
	bool isGameover_;
	// �Q�[���I�[�o�[�摜
	int imgGameover_;

	// �Q�[���I�[�o�[�n�_
	VECTOR gameoverPoint_;

	// �|�C���^�[
	Stage* stage_;
	Cannon* cannon_;

	// �G�̉摜
	int enemyModelId_;
	std::vector<EnemyBase*> enemys_;

};

