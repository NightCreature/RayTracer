#include "Ray.h"

#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0, 1.0);

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void Ray::CreateRay(const RenderOptions& options, size_t x, size_t y)
{

    auto WindowRight = options.m_tanFov * options.m_aspectRatio;
    auto WindowTop = options.m_tanFov;

    auto randomX = options.m_usePathTracing ? dis(gen) : 0.5;
    auto randomY = options.m_usePathTracing ? dis(gen) : 0.5;
    
    auto xPixel = (x + randomX) / options.m_outputWidth;
    auto yPixel = (y + randomY) / options.m_outputHeight;

    auto u = xPixel * 2.0f - 1.0f;
    auto v = yPixel * 2.0f - 1.0f;

    // find where the ray hits the near plane, and normalize that vector to get the ray direction.
    m_origin = options.m_cam.getEye() + options.m_cam.getLookAt() * options.m_nearPlaneDistance;
    m_origin += options.m_cam.getRight() * WindowRight * u;
    m_origin += options.m_cam.getUp() * WindowTop * v;
    m_direction = m_origin - options.m_cam.getEye();
    m_direction.normalize();
}
