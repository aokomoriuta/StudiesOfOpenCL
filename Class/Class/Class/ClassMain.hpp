#ifndef CLASS_CLASSMAIN_INCLUDED
#define CLASS_CLASSMAIN_INCLUDED

// OpenCL関連の例外を有効化
#define __CL_ENABLE_EXCEPTIONS

//! Classに関する名前空間
namespace Class
{
	//! アプリケーション本体
	class ClassMain
	{
	public:
		//! アプリケーションを実行する
		static int Main();

		//! 計算する
		static void Compute();
	};
}
#endif