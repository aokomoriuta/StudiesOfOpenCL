/* しきい値以上の番号の配列を返す */
/*!
	\param input 入力配列
	\param inputCount 入力配列の要素数
	\param threshold しきい値
	\param output 出力配列
	\param outputCount 出力配列の要素数
*/
__kernel void ArrayGreaterThan(
	__global const float* input,
	const uint inputCount,
	const float threshold,
	__global int* output,
	__global uint* outputCount)
{
	/* グローバルIDを取得 */
	ulong gi = get_global_id(0);

	/* 入力要素数より大きいワークアイテム番号なら何もしない */
	if(gi > inputCount)
	{
		return;
	}

	/* 出力配列を-1で初期化 */
	output[gi] = -1;

	/* 出力要素数をゼロに初期化 */
	if(gi == 0)
	{
		*outputCount = 0;
	}

	/* ここまで同期 */
	barrier(CLK_GLOBAL_MEM_FENCE);


	/* しきい値以上だったら */
	if(input[gi] > threshold)
	{
		/* 出力配列へ格納できたかどうか */
		bool isCompleted = false;

		/* 格納できるまで繰り返す */
		for(int j = *outputCount; !isCompleted; j++)
		{
			/* 入力要素数以上だったら */
			if(j >= inputCount)
			{
				/* ゼロに戻す */
				j = 0;
			}

			/* 格納して、成功してたかどうか */
			isCompleted = (atomic_cmpxchg(output+j, -1, gi) == -1);
		}

		/* 出力要素数を増やす */
		atomic_inc(outputCount);
	}
}