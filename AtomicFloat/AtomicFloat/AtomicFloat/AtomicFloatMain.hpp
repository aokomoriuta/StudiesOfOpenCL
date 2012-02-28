#ifndef ATOMICFLOAT_ATOMICFLOATMAIN_INCLUDED
#define ATOMICFLOAT_ATOMICFLOATMAIN_INCLUDED

// OpenCL関連の例外を有効化
#define __CL_ENABLE_EXCEPTIONS

//! AtomicFloatに関する名前空間
namespace AtomicFloat
{
	//! アプリケーション本体
	class AtomicFloatMain
	{
	public:
		//! アプリケーションを実行する
		static int Main();

		//! 計算する
		static void Compute();
	};
}
#endif