#pragma OPENCL EXTENSION cl_amd_printf: enable

/*! 粒子クラス */
typedef struct __attribute__ ((packed))
{
	/*! 位置 */
	float4 X;
} Particle;


/* 粒子を移動させる */
/*!
	\param count 粒子数
	\param particles 入力粒子
	\param dx 移動量
*/
__kernel void MoveParticles(
	const uint count,
	__global Particle* particles,
	const float4 dx)
{
	/* グローバル番号を取得 */
	size_t index = get_global_id(0);

	/* 粒子を取得 */
	Particle particle = particles[index];

	printf("%d: %v4f + %v4f\n", index, particle.X, dx);

	/* 位置を更新 */
	particle.X += dx;

	/* 更新した結果を戻す */
	particles[index] = particle;
}