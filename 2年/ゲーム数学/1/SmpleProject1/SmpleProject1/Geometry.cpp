#include "Geometry.h"
#include <cassert>
#include <DxLib.h>

Vector2 Vector2::operator+(const Vector2& rval)const
{
    return Vector2{ x + rval.x,y + rval.y };

}

Vector2 Vector2::operator-(const Vector2& rval)const
{
    return Vector2{ x - rval.x,y - rval.y };

}

Vector2 Vector2::operator*(float scale)const
{
    return { x * scale,y * scale };

}

Vector2 Vector2::operator/(float div)const
{
    assert(div != 0.0f);
    return { x / div,y / div };

}

void Vector2::operator+=(const Vector2& rval)
{
    x += rval.x;
    y += rval.y;

}

void Vector2::operator-=(const Vector2& rval)
{
    x -= rval.x;
    y -= rval.y;

}

void Vector2::operator*=(float scale)
{
    x *= scale;
    y *= scale;

}

void Vector2::operator/=(float div)
{
    assert(div != 0.0f);
    x /= div;
    y /= div;

}



Rect Rect::CreateRect(int left, int top, int w, int h)
{
    return { left + static_cast<float>(w / 2),top + static_cast<float>(h / 2),w / 2,h / 2 };

}

int Rect::Left() const
{
    return static_cast<int>(pos.x) - size.w;

}

int Rect::Right() const
{
    return static_cast<int>(pos.x) + size.w;

}

int Rect::Top() const
{
    return static_cast<int>(pos.y) - size.h;

}

int Rect::Bottom() const
{
    return  static_cast<int>(pos.y) + size.h;

}

int Rect::Width() const
{
    return size.w * 2;

}

int Rect::Heigh() const
{
    return size.h * 2;

}

void Rect::Draw(unsigned int color)
{
    DrawBox(Left(),
        Top(),
        Right(),
        Bottom(),
        color, true);

}
