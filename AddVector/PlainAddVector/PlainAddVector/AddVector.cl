#pragma OPENCL EXTENSION cl_khr_fp64: enable

__kernel void AddVector(
	__global const double* left,
	__global const double* right,
	__global double* output,
	int count)
{
	size_t i = get_global_id(0);

	if(i >= count)
	{
		return;
	}

	output[i] = left[i] + right[i];
}