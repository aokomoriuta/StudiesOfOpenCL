__kernel void AddVector(
	__global const float* left,
	__global const float* right,
	__global float* output,
	int count)
{
	size_t i = get_global_id(0);

	if(i > count)
	{
		return;
	}

	output[i] = left[i] + right[i];
}