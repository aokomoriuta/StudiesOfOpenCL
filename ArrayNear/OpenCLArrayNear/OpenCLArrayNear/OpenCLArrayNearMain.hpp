#ifndef ARRAYNEAR_ARRAYNEARMAIN_INCLUDED
#define ARRAYNEAR_ARRAYNEARMAIN_INCLUDED

// OpenCL関連の例外を有効化
#define __CL_ENABLE_EXCEPTIONS

//! ArrayNearに関する名前空間
namespace ArrayNear
{
	//! アプリケーション本体
	class ArrayNearMain
	{
	public:
		//! アプリケーションを実行する
		static int Main();

		//! 計算する
		static void Compute();
	};
}
#endif