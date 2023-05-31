#pragma once
#include <DxLib.h>

class Camera
{

public:

	// �J�������[�h
	enum class MODE
	{
	NONE, 
	FIXED_POINT,// ��_�J����
	FREE,// �t���[���[�h
	};

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);
	void Draw(void);
	void Release(void);

	VECTOR GetPos(void) const;
	VECTOR GetAngles(void) const;

	// �J�������[�h�̕ύX
	void ChangeMode(MODE mode);

private:

	// �J�������[�h
	MODE mode_;
	// �J�����̈ʒu
	VECTOR pos_;
	// �J�����̊p�x(rad)
	VECTOR angles_;

	void MoveXYZDirection(void);

	void MoveXZAngle(void);

};

