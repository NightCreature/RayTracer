#include "RenderOptions.h"

#include "Math/matrixmath.h"
#include "tinyxml2.h"


///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void RenderOptions::Deserialise(const std::filesystem::path& settingsFilePath)
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(settingsFilePath.string().c_str()) == tinyxml2::XML_NO_ERROR)
    {
        auto xmlNode = doc.FirstChild(); //THis should be the <xml element>
        for (auto xmlElement = xmlNode->FirstChildElement(); xmlElement != nullptr; xmlElement = xmlElement->NextSiblingElement())
        {
            if (strcmp(xmlElement->Value(), "RenderOptions") == 0)
            {
                m_outputWidth = xmlElement->UnsignedAttribute("ouputWidth");
                m_outputHeight = xmlElement->UnsignedAttribute("ouputHeight");
                m_nearPlaneDistance = xmlElement->DoubleAttribute("nearDistance");
                m_numberOfSamples = xmlElement->UnsignedAttribute("noSamples");
                m_numberOfBounces = xmlElement->UnsignedAttribute("noBounces");
                m_numberOfWorkerThreads = xmlElement->UnsignedAttribute("numberOfWorkerThreads");
                m_numberOfTasks = xmlElement->UnsignedAttribute("numberOfTasks");
                m_outputFileName = xmlElement->Attribute("outputFile");
            }
        }
    }

    m_aspectRatio = static_cast<double>(m_outputWidth) / static_cast<double>(m_outputHeight);

    m_projectionMatrix = math::createLeftHandedFOVPerspectiveMatrix(m_fov, m_aspectRatio, 0.1, 10000.0);
    m_invProjection = m_projectionMatrix.inverted();
}
