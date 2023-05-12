#pragma once

// 「ベクトル」ってのは、結局複数の次元の
// 要素を一つにまとめたまの。今回は
// ｘとｙを一つにまとめたものをVector2とします。
// ただし、ベクトルとして取り扱うには
//「演算」を定義する必要があります。
// そのために演算子オーバーロードを作ります

/// <summary>
/// ベクトル構造体
/// </summary>

struct Vector2
{
	float x;
	float y;

	// 演算子定義
	// 実際に使うときに、自分自身の右に何が来るのかが
	// 引数になる
	Vector2 operator+(const Vector2& rval)const;
	Vector2 operator-(const Vector2& rval)const;
	Vector2 operator*(float scale)const;
	Vector2 operator/(float div)const;

	void operator+=(const Vector2& rval);
	void operator-=(const Vector2& rval);
	void operator*=(float rval);
	void operator/=(float rval);

	// ベクトルの長さ（ノルム）を返す
	float Length()const;

};

using Position2 = Vector2;

struct Size 
{
	int w, h;
};

// 矩形構造体
struct Rect
{
	Position2 pos; // 中心座標
	Size size;     // 中心から端までの距離

	static Rect CreateRect(int x, int y, int w, int h);
	int Left()const;	// 左
	int Right()const;	// 右
	int Top()const;		// 上
	int Bottom()const;	// 下
	int Width()const;   // 実際の幅
	int Heigh()const;   // 実際の高さ
	void Draw(unsigned int color = 0xffffffff);

};


