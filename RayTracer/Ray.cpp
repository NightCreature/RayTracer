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

    auto WindowRight = options.m_tanFov;
    auto WindowTop = options.m_tanFov;

    auto randomX = options.m_usePathTracing ? dis(gen) : 0.5;
    auto randomY = options.m_usePathTracing ? dis(gen) : 0.5;
    
    auto xPixel = (x + randomX) / options.m_outputWidth;
    auto yPixel = (y + randomY) / options.m_outputHeight;

    auto u = (xPixel * 2.0f - 1.0f) * options.m_aspectRatio * options.m_tanFov;
    auto v = (1.0f - yPixel * 2.0f) * options.m_tanFov;

    // find where the ray hits the near plane, and normalize that vector to get the ray direction.6
    /*m_origin = options.m_cam.getEye() + options.m_cam.getLookAt() * options.m_nearPlaneDistance;
    m_origin += options.m_cam.getRight() * WindowRight * u;
    m_origin += options.m_cam.getUp() * WindowTop * v;
    m_direction = m_origin - options.m_cam.getEye();
    m_direction.normalize();*/

    m_origin = Vector3(0, 0, -25);
    m_direction = Vector3(u, v, 1);

    //Matrix44 cameraMatrix = options.m_cam.getCamera();
    //auto origen = Vector4(m_origin, 1) * cameraMatrix;
    //auto direction = Vector4(m_direction, 1) * cameraMatrix;
    //m_origin = Vector3(origen.x(), origen.y(), origen.z());
    //m_direction = Vector3(direction.x(), direction.y(), direction.z()) - m_origin;
    m_direction.normalize();

    //Vector3 translation;
    //translation.setX(u / static_cast<double>(options.m_projectionMatrix[0][0]));
    //translation.setY(v / static_cast<double>(options.m_projectionMatrix[1][1]));
    //translation.setZ(-1.0);

    //Matrix44 invView = options.m_cam.getCamera();

    //// Transform the screen space Pick ray into 3D space
    //m_direction.setX(translation.x() * invView[0][0] + translation.y() * invView[1][0] + translation.z() * invView[2][0]);
    //m_direction.setY(translation.x() * invView[0][1] + translation.y() * invView[1][1] + translation.z() * invView[2][1]);
    //m_direction.setZ(translation.x() * invView[0][2] + translation.y() * invView[1][2] + translation.z() * invView[2][2]);
    //m_direction.normalize();
    //m_origin.setX(invView[3][0]);
    //m_origin.setY(invView[3][1]);
    //m_origin.setZ(invView[3][2]);
}
