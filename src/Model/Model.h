#pragma once

#include <memory>

#include <glm/vec3.hpp>

class Model
{
public:
	

};

std::shared_ptr<Model> load_model_from_obj(const char* filename);
