#include "OpenCLArrayNearMain.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <CL/cl.hpp>
#include <boost/random.hpp>
#include <boost/timer.hpp>

#include "Exception.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::string;

namespace ArrayNear
{
	//! アプリケーションを実行する
	int ArrayNearMain::Main()
	{
		try
		{
			// 計算を実行
			ArrayNearMain::Compute();
		}
		catch(cl::Error error)
		{
			// エラーを作成
			LWisteria::CL::Exception::Exception exception = LWisteria::CL::Exception::Exception(error);

			cout << "== エラー！ == " << endl
				 << ";場所" << endl
				 << ": " << exception.Where() << endl
				 << ";詳細" << endl
				 << ": " << exception.Message() << endl;
		}

		// 成功で終了
		system("pause");
		return 0;
	}

	//! 計算する
	void ArrayNearMain::Compute()
	{
		// 実行開始
		cout << "= しきい値テスト =" << endl;

		// OpenCL Cソースファイル名
		const string filepath = "ArrayNear.cl";

		// エントリポイント名
		const string entryPoint = "ArrayNear";

		// ベクトルの要素数
		const cl_uint elementCount = 10000;
		const cl_uint maxNearCount = 10;

		// 最大値と最小値
		const cl_float minValue = -10000;
		const cl_float maxValue = +10000;

		// しきい値
		const cl_float threshold = 0.0001f;

/*****************/
		// 要素数を表示
		cout << ";要素数" << endl
			 << ": " << elementCount << endl;
		

/*****************/
		cout << endl
			 << "# 入力配列の初期化" << endl;

		// 入力値を初期化
		cl_float* input = new cl_float[elementCount]; 

		// 乱数生成器の作成
		boost::variate_generator<boost::minstd_rand&, boost::uniform_real<cl_float>> random(
			boost::minstd_rand(42),
			boost::uniform_real<cl_float>(minValue, maxValue));

		// 全要素について
		for(cl_uint i = 0; i < elementCount; i++)
		{
			// 入力値をランダムで与える
			input[i] = random();
		}

		// OpenCL用出力
		cl_int* outputCL = new cl_int[elementCount * maxNearCount];

		// CPU用出力
		cl_int* outputCPU = new cl_int[elementCount * maxNearCount];

		// タイマーで時間測定
		boost::timer timer = boost::timer();


/*****************/
		cout << "# CPUの計算";
		timer.restart();
		
		// 全要素について
		for(cl_uint i = 0; i < elementCount; i++)
		{
			// 全近傍について
			for(cl_uint j = 0; j < maxNearCount; j++)
			{
				// 出力値を-1で初期化
				outputCPU[i*maxNearCount + j] = -1;
			}
		}

		// 全要素について
		for(cl_uint i = 0; i < elementCount; i++)
		{
			// この要素の近傍要素数を初期化
			cl_uint nearCount = 0;

			// 他の要素に対して
			for(cl_uint j = 0; j < elementCount; j++)
			{
				// 自分以外に対して
				if(i != j)
				{
					// 相手との差が小さければ
					if(abs(input[i] - input[j]) < threshold)
					{
						// 出力値に要素番号を追加
						outputCPU[i*maxNearCount + nearCount] = j;

						// 近傍要素数を増やす
						nearCount++;

						// 近傍要素数が最大値に達したら
						if(nearCount > maxNearCount)
						{
							// 例外
							throw "近傍要素数の最大値に達しました";
						}
					}
				}
			}
		}
		cout << " - " << timer.elapsed() << "[s]" << endl;
	
/*****************/
		cout << "# プラットフォームを取得" << endl;

		// プラットフォーム一覧を取得
		vector<cl::Platform> platforms = vector<cl::Platform>();
		cl::Platform::get(&platforms);

		// 先頭のプラットフォームを取得
		cl::Platform platform = platforms[0];


/*****************/
		cout << "# デバイス一覧を取得" << endl;
		
		// デバイス一覧を取得
		vector<cl::Device> devices = vector<cl::Device>();
		platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

		// 先頭のデバイスを取得
		cl::Device device = devices[0];


/*****************/
		cout << "# コンテキストを作成" << endl;

		// コンテキストを作成
		cl::Context context = cl::Context(devices);


/*****************/
		cout << "# コマンドキューを作成" << endl;
		
		// コマンドキューを作成
		cl::CommandQueue queue = cl::CommandQueue(context, device);


/*****************/
		cout << "# バッファーの作成" << endl;

		// 入力のバッファーを読み込み専用で作成
		cl::Buffer bufferInput = cl::Buffer(context, CL_MEM_READ_ONLY, sizeof(cl_float) * elementCount);

		// 出力のバッファーを書きこみ専用で作成
		cl::Buffer bufferOutput = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_int) * elementCount*maxNearCount);


/*****************/
		cout << "# ソースの読み込み" << endl;

		// ソースを文字列に格納
		std::string sourceString = std::string(
			std::istreambuf_iterator<char>(std::ifstream(filepath)),
			(std::istreambuf_iterator<char>())).substr(3);

		// ソースの作成
		cl::Program::Sources sources = cl::Program::Sources();
		sources.push_back(std::make_pair(sourceString.c_str(), sourceString.size()));


/*****************/
		cout << "# プログラムの作成" << endl;

		// プログラムを作成
		cl::Program program = cl::Program(context, sources);


/*****************/
		cout << "# プログラムをビルド" << endl;

		try
		{
			// コンテキスト内の全てのデバイスを対象にしてプログラムをビルド
			program.build(devices);
		}
		// OpenCL例外があった場合
		catch(cl::Error error)
		{
			// ビルドエラーなら
			if(error.err() == CL_BUILD_PROGRAM_FAILURE)
			{
				// ビルドログを表示
				cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
			}

			// エラーをそのまま投げる
			throw error;
		}

/*****************/
		cout << "# カーネルを作成" << endl;

		// カーネルを作成
		cl::Kernel kernel = cl::Kernel(program, entryPoint.c_str());


/*****************/
		cout << "# 引数を設定" << endl;

		// 引数を設定
		// # 入力配列の要素数
		// # 近傍要素数の最大値
		// # 入力配列
		// # 出力配列
		// # しきい値
		kernel.setArg(0, elementCount);
		kernel.setArg(1, maxNearCount);
		kernel.setArg(2, bufferInput);
		kernel.setArg(3, bufferOutput);
		kernel.setArg(4, threshold);

/*****************/
		cout << endl
			 << "== 計算の実行 ==" << endl
		     << "# デバイスにデータを書き込み" << endl;
		timer.restart();

		// 非同期で入力値を書き込み
		queue.enqueueWriteBuffer(bufferInput, CL_FALSE, 0, sizeof(cl_float) * elementCount, input);

/*****************/
		cout << "# カーネルの実行" << endl;

		// カーネル実行のイベント
		cl::Event kernelEvent;

		// カーネルを実行
		queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(elementCount, elementCount), cl::NullRange, NULL, &kernelEvent);

		// 実行終了まで待機
		kernelEvent.wait();

/*****************/
		cout << "# デバイスからデータを読み込み" << endl;

		// 出力待機イベント
		cl::Event readEvent;

		// 同期で出力値に読み込み
		queue.enqueueReadBuffer(bufferOutput, CL_FALSE, 0, sizeof(cl_int) * elementCount*maxNearCount, outputCL, NULL, &readEvent);

		// 読み込み完了まで待機
		readEvent.wait();

		cout << "#: " << timer.elapsed() << "[s]" << endl;

/*****************/
		cout << endl
			 << "== 結果 == " << endl
			 << "# CLでの計算" << endl;

		// 全出力値について
		for(cl_uint i = 0; i < elementCount; i++)
		{
			for(cl_uint j = 0; j < maxNearCount; j++)
			{
				// 近傍が存在すれば
				if(outputCL[i*maxNearCount + j] != -1)
				{
					// 計算結果を表示
					cout << "* " << i << " : " << outputCL[i*maxNearCount + j] << " (" << input[i] << " : " << input[outputCL[i*maxNearCount + j]] << ")" << endl;
				}
			}
		}



		cout << "# CPUでの計算" << endl;

		// 全出力値について
		for(cl_uint i = 0; i < elementCount; i++)
		{
			for(cl_uint j = 0; j < maxNearCount; j++)
			{
				// 近傍が存在すれば
				if(outputCPU[i*maxNearCount + j] != -1)
				{
					// 計算結果を表示
					cout << "* " << i << " : " << outputCPU[i*maxNearCount + j] << " (" << input[i] << " : " << input[outputCPU[i*maxNearCount + j]] << ")" << endl;
				}
			}
		}


/*****************/
		delete[] input;
		delete[] outputCPU;
		delete[] outputCL;
	}
}
#undef foreach