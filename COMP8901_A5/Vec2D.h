#pragma once
struct Vec2D {
    Vec2D() : x(0), y(0) {};
    Vec2D(int x, int y) : x(x), y(y) {};
    
    int x, y;

    Vec2D operator +(const Vec2D &v) { return Vec2D(x + v.x, y + v.y); }
    const Vec2D operator +(const Vec2D &v) const { return Vec2D(x + v.x, y + v.y); }
    Vec2D operator -(const Vec2D &v) { return Vec2D(x - v.x, y - v.y); }
    const Vec2D operator -(const Vec2D &v) const { return Vec2D(x - v.x, y - v.y); }
    Vec2D operator *(const int n) { return Vec2D(x * n, y * n); }
    const Vec2D operator *(const int n) const { return Vec2D(x * n, y * n); }
    Vec2D operator /(const int n) { return Vec2D(x / n, y / n); }
    const Vec2D operator /(const int n) const { return Vec2D(x / n, y / n); }
    void operator +=(const Vec2D &v) { x += v.x; y += v.y; }
    void operator -=(const Vec2D &v) { x -= v.x; y -= v.y; }
};

