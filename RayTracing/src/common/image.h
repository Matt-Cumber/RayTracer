#ifndef IMAGE_H
#define IMAGE_H

class Image {
public:
    Image() : m_ar(0.0f), m_w(0.0f), m_h(0.0f), m_nos(0) {};
    Image(float ar, float w, float h, int nos) : m_ar(ar), m_w(w), m_h(h), m_nos(nos) {};

    float AspectRatio() const { return m_ar; };
    float Width() const { return m_w; };
    float Height() const { return m_h; };
    int NumberOfSamples() const { return m_nos; };

private:
    float m_ar;
    float m_w;
    float m_h;
    int m_nos;
};

#endif
