#include "ComputerGpu.hpp"

namespace ViennaCLAdd
{
#pragma unmanaged
	ComputerGpuNative::ComputerGpuNative(int n)
	{
		// 要素数を設定
		count = n;

		// GPU側ベクトル生成
		left = viennacl::vector<double>(count);
		right = viennacl::vector<double>(count);
		result = viennacl::vector<double>(count);
	}

	void ComputerGpuNative::Write(double leftPtr[], double rightPtr[])
	{
		// 足される・足すベクトルを取得
		auto leftCpu = std::vector<double>(leftPtr, leftPtr + count);
		auto rightCpu = std::vector<double>(rightPtr, rightPtr + count);

		// GPUにデータを書き込み
		viennacl::fast_copy(leftCpu, left);
		viennacl::fast_copy(rightCpu, right);

		// 終了まで待機
		viennacl::ocl::get_queue().finish();
	}

	void ComputerGpuNative::Add()
	{
		// 和を実行
		result = left + right;

		// 終了まで待機
		viennacl::ocl::get_queue().finish();
	}

	void ComputerGpuNative::Read(double resultPtr[])
	{
		// 結果ベクトルにデータを転送
		auto resultCpu = std::vector<double>(count);
		viennacl::fast_copy(result, resultCpu);
		std::copy(resultCpu.begin(), resultCpu.end(), resultPtr);

		// 終了まで待機
		viennacl::ocl::get_queue().finish();
	}
#pragma managed
	ComputerGpu::ComputerGpu(int n)
	{
		// 計算クラスを作成
		computer = new ComputerGpuNative(n);
	}
	
	ComputerGpu::~ComputerGpu()
	{
		// 計算クラスを廃棄
		delete(computer);
	}
	
	void ComputerGpu::Write(array<double>^ left, array<double>^ right)
	{
		// 先頭ポインタ取得
		pin_ptr<double> leftPtr = &left[0];
		pin_ptr<double> rightPtr = &right[0];

		// 書き込み
		computer->Write(leftPtr, rightPtr);
	}

	void ComputerGpu::Add()
	{
		// 演算実行
		computer->Add();
	}

	void ComputerGpu::Read(array<double>^ result)
	{
		// 先頭ポインタ取得
		pin_ptr<double> resultPtr = &result[0];

		// 読み込み
		computer->Read(resultPtr);
	}

}