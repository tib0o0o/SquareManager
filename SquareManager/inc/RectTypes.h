#pragma once
typedef struct Rect_s
{
    int w;
    int h;
    int x;
    int y;
    int id;

    bool operator > (const Rect_s& str) const
    {
        return (w < str.w);
    }
}
Rect;

typedef struct Space_s
{
    int w;
    int h;
    int x;
    int y;

    bool operator > (const Space_s& str) const
    {
        return (w > str.w);
    }
}
Space;