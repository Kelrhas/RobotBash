#pragma once

#include <windows.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <memory>
#include <string>
#include <vector>
#include <Xinput.h>
#include <imgui.h>
#include <functional>
#include <cwchar>

#include "../tinyxml/tinyxml2.h"

#include "../glm/glm.hpp"
#include "../glm/gtc/constants.hpp"
#include "../glm/gtc/matrix_transform.hpp"

#include <Constants.h>
#include "UTILS/Debug.h"

#define RAND_0_1 ((float)rand() / RAND_MAX)
#define RAND_0_X(x) (rand() % x)

const glm::vec3 VECTOR_0( 0, 0, 0 );
const glm::vec3 VECTOR_1( 1, 1, 1 );
const glm::vec3 VECTOR_X( 1, 0, 0 );
const glm::vec3 VECTOR_Y( 0, 1, 0 );
const glm::vec3 VECTOR_Z( 0, 0, 1 );

const glm::vec3 COLOR_WHITE( 1.f, 1.f, 1.f );
const glm::vec3 COLOR_RED( 1.f, 0.f, 0.f );
const glm::vec3 COLOR_GREEN( 0.f, 1.f, 0.f );
const glm::vec3 COLOR_BLUE( 0.f, 0.f, 1.f );
const glm::vec3 COLOR_BLACK( 0.f, 0.f, 0.f );

#define VEC3_RIGHT VECTOR_X
#define VEC3_UP VECTOR_Y
#define VEC3_FORWARD -VECTOR_Z



typedef std::function<void()>			VoidFunc;
typedef std::function<void( bool )>		BoolFunc;
typedef std::function<void( int )>		IntFunc;
typedef std::function<void( char )>		CharFunc;
typedef std::function<void( float )>	FloatFunc;
typedef std::function<void( glm::vec2 )>Vec2Func;
typedef std::function<void( glm::vec3 )>Vec3Func;

typedef std::vector<VoidFunc>			VoidFuncVec;
typedef std::vector<BoolFunc>			BoolFuncVec;
typedef std::vector<IntFunc>			IntFuncVec;
typedef std::vector<CharFunc>			CharFuncVec;
typedef std::vector<FloatFunc>			FloatFuncVec;
typedef std::vector<Vec2Func>			Vec2FuncVec;
typedef std::vector<Vec3Func>			Vec3FuncVec;
