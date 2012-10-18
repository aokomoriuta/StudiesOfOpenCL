#include "ComputerCpu.hpp"

namespace ViennaCLAdd
{
#pragma unmanaged
	ComputerCpuNative::ComputerCpuNative(int n)
	{
		// 要素数を設定
		count = n;

		// 結果ベクトル生成
		result = std::vector<double>(count);
	}

	void ComputerCpuNative::Write(double leftPtr[], double rightPtr[])
	{
		// 足される・足すベクトルにデータを転送
		left = std::vector<double>(leftPtr, leftPtr + count);
		right = std::vector<double>(rightPtr, rightPtr + count);
	}

	void ComputerCpuNative::Add()
	{
		// 全要素で
		for(int i = 0; i < count; i++)
		{
			// それぞれ足す
			result[i] += left[i] + right[i];
		}
	}

	void ComputerCpuNative::Read(double resultPtr[])
	{
		// 結果ベクトルにデータを転送
		std::copy(result.begin(), result.end(), resultPtr);
	}
#pragma managed
	ComputerCpu::ComputerCpu(int n)
	{
		// 計算クラスを作成
		computer = new ComputerCpuNative(n);
	}
	
	ComputerCpu::~ComputerCpu()
	{
		// 計算クラスを廃棄
		delete(computer);
	}
	
	void ComputerCpu::Write(array<double>^ left, array<double>^ right)
	{
		// 先頭ポインタ取得
		pin_ptr<double> leftPtr = &left[0];
		pin_ptr<double> rightPtr = &right[0];

		// 書き込み
		computer->Write(leftPtr, rightPtr);
	}

	void ComputerCpu::Add()
	{
		// 演算実行
		computer->Add();
	}

	void ComputerCpu::Read(array<double>^ result)
	{
		// 先頭ポインタ取得
		pin_ptr<double> resultPtr = &result[0];

		// 読み込み
		computer->Read(resultPtr);
	}

}