#pragma once

#include "Camera.h"
#include <cmath>


struct RenderOptions
{
    RenderOptions() = default;
    RenderOptions(const Camera& cam, size_t width, size_t height, double fov)
        : m_cam(cam)
        , m_outputWidth(width)
        , m_outputHeight(height)
        , m_aspectRatio(width/height)
        , m_fov(fov)
        , m_tanFov(tan(fov))
    {}

    Camera m_cam;

    size_t m_outputWidth = 255;
    size_t m_outputHeight= 255;
    size_t m_aspectRatio;
    double m_fov;
    double m_tanFov;

    bool m_usePathTracing = false;
};
