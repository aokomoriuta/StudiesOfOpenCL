/* 最小値を探索する */
/*!
	\param elementCount 入力配列の要素数
	\param values 入力配列
	\param localValues ローカルメモリ
	\param outGroupCount グループ数
*/
__kernel void ArrayMinimum(
	const uint elementCount,
	global float* values,
	local float* localValues,
	global uint* outGroupCount)
{	
	/* 番号と数を取得
	  * グローバル番号
	  * ローカル番号
	  * グループ番号
	  * ローカルアイテム数
	*/
	size_t globalIndex = get_global_id(0);
	size_t localIndex = get_local_id(0);
	size_t groupIndex = get_group_id(0);
	size_t localSize = get_local_size(0);

	/* グループ数を返す */
	if(globalIndex == 0)
	{
		*outGroupCount = get_num_groups(0);
	}

	/* ローカルメモリに格納（要素数以上は0番目の値を格納しておく） */
	localValues[localIndex] = (globalIndex < elementCount) ?
		values[globalIndex] :
		values[0];

	/* ローカルメモリへの格納を同期 */
	barrier(CLK_LOCAL_MEM_FENCE);

	/* 割る数を2倍ずつ、ローカルサイズまで繰り返す */
	for(int n = 2; n < 2*localSize; n *= 2)
	{
		/* 要素数以下なら */
		if(globalIndex < elementCount)
		{
			/* 計算する要素番号で、隣が存在すれば */
			if((localIndex%n == 0) && (localIndex + n/2 < localSize)) 
			{
				/* 隣の要素と比較して小さい方を設定する */
				localValues[localIndex] = min(localValues[localIndex], localValues[localIndex + n/2]);
			}
		}

		/* ここまで同期 */
		barrier(CLK_LOCAL_MEM_FENCE);
	}

	/* グループ内で先頭の要素なら */
	if(localIndex == 0)
	{
		/* グループ内での最小値をグローバルメモリに格納 */
		values[groupIndex] = localValues[0];
	}
}