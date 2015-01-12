#ifndef BOX2_H
#define BOX2_H

template <typename T>
struct Box2
{
    T l,t,r,b;

    Box2()
    {
        reset();
    }

    void reset()
    {
        T max = std::numeric_limits<T>::max();
        T min = -1*max;

        l = max;
        t = min;
        r = min;
        b = max;
    }

    void update(T x, T y)
    {
        if (x < l) l = x;
        if (x > r) r = x;
        if (y > t) t = y;
        if (y < b) b = y;
    }

    void update(const Box2 &box)
    {
        update(box.l, box.t);
        update(box.r, box.b);
    }

    T width() const
    {
        return r - l;
    }

    T height() const
    {
        return t - b;
    }

    void getCenter(T *x, T *y) const
    {
        *x = l + width()/2;
        *y = b + height()/2;
    }

};

typedef Box2<int>   box2i;
typedef Box2<double>   box2d;


#endif
