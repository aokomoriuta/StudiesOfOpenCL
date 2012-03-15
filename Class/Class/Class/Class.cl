#pragma OPENCL EXTENSION cl_amd_printf: enable

/*! 粒子クラス */
typedef struct __attribute__ ((packed))
{
	/*! 位置 */
	float X[4];

	/*! 速度 */
	float U[4];

	/*! 質量 */
	float M;
} Particle;


/* 粒子を移動させる */
/*!
	\param count 粒子数
	\param particles 入力粒子
	\param force 力
	\param dt 時間刻み
*/
__kernel void MoveParticles(
	const uint count,
	__global Particle* particles,
	const float4 force,
	const float dt)
{
	/* グローバル番号を取得 */
	size_t index = get_global_id(0);

	/* 粒子を取得 */
	Particle particle = particles[index];

	/* 位置と速度を取得 */
	float4 x = vload4(0, particle.X);
	float4 u = vload4(0, particle.U);

	/* 位置と速度を更新 */
	float4 a = force/particle.M;
	x += u * dt + a * dt * dt/2;
	u += a * dt;

	/* 計算した位置と速度を粒子に格納 */
	vstore4(x, 0, particle.X);
	vstore4(u, 0, particle.U);

	/* 更新した結果を戻す */
	particles[index] = particle;
}