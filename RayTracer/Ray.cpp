#include "Ray.h"

#include <random>

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void Ray::CreateRay(const Vector3& eye, const RenderOptions& options, size_t x, size_t y)
{
    m_origin = eye;
 
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
 
    auto randomX = options.m_usePathTracing ? dis(gen) : 0.5;
    auto randomY = options.m_usePathTracing ? dis(gen) : 0.5;

    m_direction = Vector3((2 * ((x + randomX) / options.m_outputWidth) - 1) * options.m_aspectRatio * options.m_tanFov, (1 - 2 * ((y + randomY) / options.m_outputHeight)) * options.m_tanFov, 1) - m_origin;
    m_direction.normalize();
}
