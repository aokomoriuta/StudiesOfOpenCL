/* しきい値以上の番号の配列を返す */
/*!
	\param elementCount 入力配列の要素数
	\param maxNearCount 近傍要素数の最大値
	\param input 入力配列
	\param output 出力配列
	\param threshold しきい値
*/
__kernel void ArrayNear(
	const uint elementCount,
	const uint maxNearCount,
	__global const float* input,
	__global int* output,
	const float threshold)
{
	/* グローバルIDを取得 */
	uint thisIndex = get_global_id(0);
	uint otherIndex = get_global_id(1);

	/* 入力要素数より大きいワークアイテム番号なら何もしない */
	if((thisIndex > elementCount) || (otherIndex > elementCount))
	{
		return;
	}

	/* 最大近傍要素数以下なら */
	if(otherIndex < maxNearCount)
	{
		/* 出力配列を-1で初期化 */
		output[thisIndex*maxNearCount + otherIndex] = -1;
	}

	/* ここまで同期 */
	barrier(CLK_GLOBAL_MEM_FENCE);

	/* 相手が自分自身でなければ */
	if(thisIndex != otherIndex)
	{
		/* しきい値以上だったら */
		if(fabs(input[thisIndex] - input[otherIndex]) < threshold)
		{
			/* 出力配列へ格納できたかどうか */
			bool isCompleted = false;

			/* 格納できるまで繰り返す */
			for(int i = 0; !isCompleted; i++)
			{
				/* 格納して、成功してたかどうか */
				isCompleted = (atomic_cmpxchg(output + thisIndex*maxNearCount + i, -1, otherIndex) == -1);

				/* 成功したら */
				if(isCompleted)
				{
					/* ループ抜け */
					break;
				}
			}
		}
	}

}