#ifndef LWISTERIA_OPENCL_EXCEPTION_EXCEPTION_INCLUDED
#define LWISTERIA_OPENCL_EXCEPTION_EXCEPTION_INCLUDED

// OpenCL関連の例外を有効化
#define __CL_ENABLE_EXCEPTIONS

#include <string>

#include <CL/cl.hpp>

namespace LWisteria{ namespace CL{ namespace Exception
{
	//! LWisteriaCLで発生する例外
	/*!
		cl::Errorのラッパ
	*/
	class Exception : private cl::Error
	{
	private:

	public:
		//! 例外を作成する
		/*!
			\param error 元のエラーオブジェクト
		*/
		Exception(const cl::Error& error);

		//! 発生場所を取得する
		/*!
			\return 発生場所
		*/
		std::string Where() const;

		//! エラー内容を取得する
		/*!
			\return エラー内容
		*/
		std::string Message() const;

		//! エラー番号を取得する
		/*!
			\return エラー番号
		*/
		cl_int ErrorCode() const;
	};
}}}
#endif