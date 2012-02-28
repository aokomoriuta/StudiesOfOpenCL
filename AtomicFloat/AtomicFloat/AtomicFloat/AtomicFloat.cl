/*! float版のatomic_add */
/*!
	\param address 加算先
	\param value 加算値
	\return 加算後の値
*/
float atomic_add_global_float(
	__global float* const address,
	const float value)
{
	/* 加算前、後の値 */
	uint oldValue;
	uint newValue;

	/* 加算前後の値を設定 */
	*(float*)&oldValue = *address;
	*(float*)&newValue = (*(float*)&oldValue + value);


	/* 新旧の値を入れ替えてみる */
	uint thisValue = atom_cmpxchg((__global uint*)address, oldValue, newValue);

	/* 入れ替えてみた値が古い値でない場合 */
	while(thisValue != oldValue)
	{
		/* 新旧の値を更新 */
		oldValue = thisValue;
		*(float*)&newValue = (*(float*)&oldValue + value);
		
		/* もう一度、新旧の値を入れ替えてみる */
		thisValue = atom_cmpxchg((__global uint*)address, oldValue, newValue);
	}

	/* 古い値を返す */
	return *(float*)&oldValue;
}

/* atomic操作で総和を計算する */
/*!
	\param elementCount 入力配列の要素数
	\param values 入力配列
*/
__kernel void SumWithAtomicFloat(
	const uint elementCount,
	__global float* values)
{	
	/* グローバル番号を取得 */
	size_t globalIndex = get_global_id(0);

	/* 先頭以外は */
	if(globalIndex != 0)
	{
		/* 先頭に値を追加 */
		atomic_add_global_float(values, values[globalIndex]);
	}
}