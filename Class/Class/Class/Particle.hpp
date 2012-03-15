#ifndef CLASS_PARTICLE_INCLUDED
#define CLASS_PARTICLE_INCLUDED

// OpenCL関連の例外を有効化
#define __CL_ENABLE_EXCEPTIONS

#include <string>

#include <CL/cl.hpp>

namespace Class
{
	//! 粒子クラス
	class Particle
	{
	public:
		//! 位置
		cl_float4 x;

		//! 速度
		cl_float4 u;
	};
}
#endif