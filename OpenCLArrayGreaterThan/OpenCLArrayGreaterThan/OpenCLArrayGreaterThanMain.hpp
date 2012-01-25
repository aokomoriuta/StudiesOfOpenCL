#ifndef PLAINADDVECTOR_PLAINADDVECTORMAIN_INCLUDED
#define PLAINADDVECTOR_PLAINADDVECTORMAIN_INCLUDED

// OpenCL関連の例外を有効化
#define __CL_ENABLE_EXCEPTIONS

//! ArrayGreaterThanに関する名前空間
namespace ArrayGreaterThan
{
	//! アプリケーション本体
	class ArrayGreaterThanMain
	{
	public:
		//! アプリケーションを実行する
		static int Main();

		//! 計算する
		static void Compute();
	};
}
#endif