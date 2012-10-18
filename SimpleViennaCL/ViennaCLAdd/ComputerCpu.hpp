#pragma once
#include <vector>

namespace ViennaCLAdd
{
	//! CPUで計算するクラス
	class ComputerCpuNative
	{
	private:
		//! 要素数
		int count;

		//! 足されるベクトル
		std::vector<double> left;

		//! 足すベクトル
		std::vector<double> right;

		//! 結果を格納するベクトル
		std::vector<double> result;

	public:
		/* CPUで計算するクラスを生成する
		* @param n 要素数
		*/
		ComputerCpuNative(int n);

		/* 入力値を書き込む
		* @param leftPtr 足されるベクトルのポインタ
		* @param rightPtr 足すベクトルのポインタ
		*/
		void Write(double leftPtr[], double rightPtr[]);

		//! 演算を実行する
		void Add();

		/* 出力値を読み込む
		* @param resultPtr 結果を格納するベクトル
		*/
		void Read(double resultPtr[]);
	};

	//! CPUで計算するクラスのラッパー
	public ref class ComputerCpu
	{
	private:
		//! ネイティブ側の計算クラス
		ComputerCpuNative* computer;

	public:
		/* CPUで計算するクラスを生成する
		* @param n 要素数
		*/
		ComputerCpu(int n);

		//! CPUで計算するクラスを破棄する
		~ComputerCpu();

		/* 入力値を書き込む
		* @param left 足されるベクトルの配列
		* @param right 足すベクトルの配列
		*/
		void Write(array<double>^ left, array<double>^ right);
		
		//! 演算を実行する
		void Add();

		/* 出力値を読み込む
		* @param result 結果を格納する配列
		*/
		void Read(array<double>^ result);
	};
}