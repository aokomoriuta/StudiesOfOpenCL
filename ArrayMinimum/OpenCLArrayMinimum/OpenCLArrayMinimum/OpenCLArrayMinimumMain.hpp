#ifndef ARRAYNEAR_ARRAYMINIMUMMAIN_INCLUDED
#define ARRAYNEAR_ARRAYMINIMUMMAIN_INCLUDED

// OpenCL関連の例外を有効化
#define __CL_ENABLE_EXCEPTIONS

//! ArrayMinimumに関する名前空間
namespace ArrayMinimum
{
	//! アプリケーション本体
	class ArrayMinimumMain
	{
	public:
		//! アプリケーションを実行する
		static int Main();

		//! 計算する
		static void Compute();
	};
}
#endif