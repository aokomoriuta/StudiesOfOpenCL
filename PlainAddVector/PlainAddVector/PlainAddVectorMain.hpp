#ifndef PLAINADDVECTOR_PLAINADDVECTORMAIN_INCLUDED
#define PLAINADDVECTOR_PLAINADDVECTORMAIN_INCLUDED

// OpenCL関連の例外を有効化
#define __CL_ENABLE_EXCEPTIONS

//! PlainAddVectorに関する名前空間
namespace PlainAddVector
{
	//! アプリケーション本体
	class PlainAddVectorMain
	{
	public:
		//! アプリケーションを実行する
		static int Main();

		//! 計算する
		static void Compute();
	};
}
#endif