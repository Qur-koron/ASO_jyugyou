#pragma once

// �u�x�N�g���v���Ă̂́A���Ǖ����̎�����
// �v�f����ɂ܂Ƃ߂��܂́B�����
// ���Ƃ�����ɂ܂Ƃ߂����̂�Vector2�Ƃ��܂��B
// �������A�x�N�g���Ƃ��Ď�舵���ɂ�
//�u���Z�v���`����K�v������܂��B
// ���̂��߂ɉ��Z�q�I�[�o�[���[�h�����܂�

/// <summary>
/// �x�N�g���\����
/// </summary>

struct Vector2
{
	float x;
	float y;

	// ���Z�q��`
	// ���ۂɎg���Ƃ��ɁA�������g�̉E�ɉ�������̂���
	// �����ɂȂ�
	Vector2 operator+(const Vector2& rval)const;
	Vector2 operator-(const Vector2& rval)const;
	Vector2 operator*(float scale)const;
	Vector2 operator/(float div)const;

	void operator+=(const Vector2& rval);
	void operator-=(const Vector2& rval);
	void operator*=(float rval);
	void operator/=(float rval);

	// �x�N�g���̒����i�m�����j��Ԃ�
	float Length()const;

};

using Position2 = Vector2;

struct Size 
{
	int w, h;
};

// ��`�\����
struct Rect
{
	Position2 pos; // ���S���W
	Size size;     // ���S����[�܂ł̋���

	static Rect CreateRect(int x, int y, int w, int h);
	int Left()const;	// ��
	int Right()const;	// �E
	int Top()const;		// ��
	int Bottom()const;	// ��
	int Width()const;   // ���ۂ̕�
	int Heigh()const;   // ���ۂ̍���
	void Draw(unsigned int color = 0xffffffff);

};


