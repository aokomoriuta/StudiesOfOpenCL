#pragma OPENCL EXTENSION cl_amd_printf: enable

/*! 粒子クラス */
typedef struct __attribute__ ((packed))
{
	/*! 位置 */
	float4 X;

	/*! 速度 */
	float4 U;
} Particle;


/* 粒子を移動させる */
/*!
	\param count 粒子数
	\param particles 入力粒子
	\param dt 時間刻み
*/
__kernel void MoveParticles(
	const uint count,
	__global Particle* particles,
	const float dt)
{
	/* グローバル番号を取得 */
	size_t index = get_global_id(0);

	/* 粒子を取得 */
	Particle particle = particles[index];

	/* 位置を更新 */
	particle.X += particle.U * dt;

	/* 更新した結果を戻す */
	particles[index] = particle;
}