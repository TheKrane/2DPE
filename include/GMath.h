#ifndef GMATH_H
#define GMATH_H

class Vector2
{
public:
	Vector2(float xcoord, float ycoord) { x = xcoord; y = ycoord; }
	Vector2() {}
	~Vector2() {}

	void setX(float xcoord) { x = xcoord; }
	float getX() { return x; }

	void setY(float ycoord) { y = ycoord; }
	float getY() { return y; }

	float x, y;
        
        Vector2& operator+=(const Vector2& rhs)
        {
                x += rhs.x;
                y += rhs.y;
                return *this;
        }
};

inline Vector2 operator+ (Vector2 lhs, const Vector2& rhs)
{
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        return lhs;
}

inline Vector2 operator- (Vector2 lhs, const Vector2& rhs)
{
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        return lhs;
}

inline Vector2 operator/ (Vector2 lhs, const float& rhs)
{
        lhs.x /= rhs;
        lhs.y /= rhs;
        return lhs;
}

class Rect
{
public:
	Rect(float _x, float _y, float _w, float _h) { x = _x; y = _y; w = _w; h = _h; }
	Rect() { x = 0.0f; y = 0.0f; w = 0.0f; h = 0.0f; }
	~Rect() {}

	float x, y, w, h;
};

#endif