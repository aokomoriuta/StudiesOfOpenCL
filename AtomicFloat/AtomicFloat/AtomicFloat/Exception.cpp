#include "Exception.hpp"

#include <string>
#include <sstream>

#include <CL/cl.hpp>


using namespace LWisteria::CL;
using std::string;
using std::stringstream;

namespace LWisteria{ namespace CL{ namespace Exception
{
	//! 例外を作成する
	Exception::Exception(const cl::Error& error)
		: cl::Error(error)
	{
	}

	//! 発生場所を取得する
	std::string Exception::Where() const
	{
		// 発生場所を取得して返す
		return this->what();
	}

	//! エラー内容を取得する
	std::string Exception::Message() const
	{
		// 未定義の場合は最初にエラー番号を表示
		stringstream message = stringstream();
		message << this->ErrorCode() << ": ";

		// エラー番号によって文字列を変化
		switch(this->ErrorCode())
		{
		case CL_SUCCESS:
			message << "成功(SUCCESS)";
			break;

		case CL_DEVICE_NOT_FOUND:
			message << "デバイスが見つからない(DEVICE_NOT_FOUND)";
			break;

		case CL_DEVICE_NOT_AVAILABLE:
			message << "デバイスが使用不可(DEVICE_NOT_AVAILABLE)";
			break;

		case CL_COMPILER_NOT_AVAILABLE:
			message << "コンパイラーが使用不可(COMPILER_NOT_AVAILABLE)";
			break;

		case CL_MEM_OBJECT_ALLOCATION_FAILURE:
			message << "(MEM_OBJECT_ALLOCATION_FAILURE)";
			break;

		case CL_OUT_OF_RESOURCES:
			message << "(OUT_OF_RESOURCES)";
			break;

		case CL_OUT_OF_HOST_MEMORY:
			message << "(OUT_OF_HOST_MEMORY)";
			break;

		case CL_PROFILING_INFO_NOT_AVAILABLE:
			message << "(PROFILING_INFO_NOT_AVAILABLE)";
			break;

		case CL_MEM_COPY_OVERLAP:
			message << "(MEM_COPY_OVERLAP)";
			break;

		case CL_IMAGE_FORMAT_MISMATCH:
			message << "(IMAGE_FORMAT_MISMATCH)";
			break;

		case CL_IMAGE_FORMAT_NOT_SUPPORTED:
			message << "(IMAGE_FORMAT_NOT_SUPPORTED)";
			break;

		case CL_BUILD_PROGRAM_FAILURE:
			message << "プログラムビルド失敗(BUILD_PROGRAM_FAILURE)";
			break;

		case CL_MAP_FAILURE:
			message << "(MAP_FAILURE)";
			break;

		case CL_MISALIGNED_SUB_BUFFER_OFFSET:
			message << "(MISALIGNED_SUB_BUFFER_OFFSET)";
			break;

		case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
			message << "(EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST)";
			break;

		case CL_INVALID_VALUE:
			message << "不正な値(INVALID_VALUE)";
			break;

		case CL_INVALID_DEVICE_TYPE:
			message << "不正なデバイスタイプ(INVALID_DEVICE_TYPE)";
			break;

		case CL_INVALID_PLATFORM:
			message << "不正なプラットフォーム(INVALID_PLATFORM)";
			break;

		case CL_INVALID_DEVICE:
			message << "不正なデバイス(INVALID_DEVICE)";
			break;

		case CL_INVALID_CONTEXT:
			message << "不正なコンテキスト(INVALID_CONTEXT)";
			break;

		case CL_INVALID_QUEUE_PROPERTIES:
			message << "不正なキュープロパティ(INVALID_QUEUE_PROPERTIES)";
			break;

		case CL_INVALID_COMMAND_QUEUE:
			message << "不正なコマンドキュー(INVALID_COMMAND_QUEUE)";
			break;

		case CL_INVALID_HOST_PTR:
			message << "不正なホストメモリのポインタ(INVALID_HOST_PTR)";
			break;

		case CL_INVALID_MEM_OBJECT:
			message << "(INVALID_MEM_OBJECT)";
			break;

		case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
			message << "(INVALID_IMAGE_FORMAT_DESCRIPTOR)";
			break;

		case CL_INVALID_IMAGE_SIZE:
			message << "(INVALID_IMAGE_SIZE)";
			break;

		case CL_INVALID_SAMPLER:
			message << "(INVALID_SAMPLER)";
			break;

		case CL_INVALID_BINARY:
			message << "(INVALID_BINARY)";
			break;

		case CL_INVALID_BUILD_OPTIONS:
			message << "(INVALID_BUILD_OPTIONS)";
			break;

		case CL_INVALID_PROGRAM:
			message << "不正なプログラム(INVALID_PROGRAM)";
			break;

		case CL_INVALID_PROGRAM_EXECUTABLE:
			message << "(INVALID_PROGRAM_EXECUTABLE)";
			break;

		case CL_INVALID_KERNEL_NAME:
			message << "不正なカーネル名(INVALID_KERNEL_NAME)";
			break;

		case CL_INVALID_KERNEL_DEFINITION:
			message << "不正なカーネル定義(INVALID_KERNEL_DEFINITION)";
			break;

		case CL_INVALID_KERNEL:
			message << "不正なカーネル(INVALID_KERNEL)";
			break;

		case CL_INVALID_ARG_INDEX:
			message << "不正な引数番号(INVALID_ARG_INDEX)";
			break;

		case CL_INVALID_ARG_VALUE:
			message << "不正な引数値(INVALID_ARG_VALUE)";
			break;

		case CL_INVALID_ARG_SIZE:
			message << "不正な引数サイズ(INVALID_ARG_SIZE)";
			break;

		case CL_INVALID_KERNEL_ARGS:
			message << "不正なカーネル引数(INVALID_KERNEL_ARGS)";
			break;

		case CL_INVALID_WORK_DIMENSION:
			message << "不正な次元(INVALID_WORK_DIMENSION)";
			break;

		case CL_INVALID_WORK_GROUP_SIZE:
			message << "不正なワークグループ数(INVALID_WORK_GROUP_SIZE)";
			break;

		case CL_INVALID_WORK_ITEM_SIZE:
			message << "不正なワークアイテム数(INVALID_WORK_ITEM_SIZE)";
			break;

		case CL_INVALID_GLOBAL_OFFSET:
			message << "(INVALID_GLOBAL_OFFSET)";
			break;

		case CL_INVALID_EVENT_WAIT_LIST:
			message << "(INVALID_EVENT_WAIT_LIST)";
			break;

		case CL_INVALID_EVENT:
			message << "不正なイベント(INVALID_EVENT)";
			break;

		case CL_INVALID_OPERATION:
			message << "(INVALID_OPERATION)";
			break;

		case CL_INVALID_GL_OBJECT:
			message << "(INVALID_GL_OBJECT)";
			break;

		case CL_INVALID_BUFFER_SIZE:
			message << "(INVALID_BUFFER_SIZE)";
			break;

		case CL_INVALID_MIP_LEVEL:
			message << "(INVALID_MIP_LEVEL)";
			break;

		case CL_INVALID_GLOBAL_WORK_SIZE:
			message << "(INVALID_GLOBAL_WORK_SIZE)";
			break;

		case CL_INVALID_PROPERTY:
			message << "(INVALID_PROPERTY)";
			break;

		default:
			message << "不明なエラー";
			break;
		}

		// メッセージ文字列を返す
		return message.str();
	}

	//! エラー番号を取得する
	cl_int Exception::ErrorCode() const
	{
		// エラー番号を取得して返す
		return this->err();
	}
}}}