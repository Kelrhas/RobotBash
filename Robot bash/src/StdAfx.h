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
#include <functional>
#include <cwchar>

#include "imgui/imgui.h"
#include "tinyxml/tinyxml2.h"

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "UTILS/Debug.h"
#include "UTILS/Math.h"

#define RAND_0_1 ((float)rand() / RAND_MAX)
#define RAND_0_X(x) (rand() % x)

const glm::vec3 VEC3_0( 0, 0, 0 );
const glm::vec3 VEC3_1( 1, 1, 1 );
const glm::vec3 VEC3_X( 1, 0, 0 );
const glm::vec3 VEC3_Y( 0, 1, 0 );
const glm::vec3 VEC3_Z( 0, 0, 1 );

const glm::vec3 COLOR_WHITE( 1.f, 1.f, 1.f );
const glm::vec3 COLOR_RED( 1.f, 0.f, 0.f );
const glm::vec3 COLOR_GREEN( 0.f, 1.f, 0.f );
const glm::vec3 COLOR_BLUE( 0.f, 0.f, 1.f );
const glm::vec3 COLOR_BLACK( 0.f, 0.f, 0.f );

#define VEC3_RIGHT VEC3_X
#define VEC3_UP VEC3_Y
#define VEC3_FORWARD -VEC3_Z



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
