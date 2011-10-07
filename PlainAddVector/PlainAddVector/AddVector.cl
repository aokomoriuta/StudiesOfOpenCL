__kernel void AddVector(
	__constant float* left,
	__constant float* right,
	__global float* output,
	__constant ulong* count)
{
	size_t i = get_global_id(0);

	if(i > *count)
	{
		return;
	}

	output[i] = left[i] + right[i];
}