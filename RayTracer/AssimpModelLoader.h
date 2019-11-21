#pragma once

#include "Model.h"

#include <string>

class Material;

namespace AssimpModelLoader 
{

    CreatedModel LoadModel(const Material& material, const std::string& fileName);

};

