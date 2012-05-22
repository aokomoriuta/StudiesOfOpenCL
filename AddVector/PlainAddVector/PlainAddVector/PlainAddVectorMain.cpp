#include "PlainAddVectorMain.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <CL/cl.hpp>
#include <boost/random.hpp>

#include "Exception.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::string;

namespace PlainAddVector
{
	//! アプリケーションを実行する
	int PlainAddVectorMain::Main()
	{
		try
		{
			// 計算を実行
			PlainAddVectorMain::Compute();
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
	void PlainAddVectorMain::Compute()
	{
		// 実行開始
		cout << "= ベクトル加算テスト =" << endl;

		// OpenCL Cソースファイル名
		const string filepath = "AddVector.cl";

		// エントリポイント名
		const string entryPoint = "AddVector";

		// ベクトルの要素数
		size_t elementCount = 544;

/*****************/
		// ワークグループ内のワークアイテム数は512
		size_t localWorkitemCount = 256;

		// 全ワークアイテム数を計算
		size_t globalWorkitemCount = (elementCount % localWorkitemCount == 0) ?
			elementCount : elementCount + localWorkitemCount - elementCount % localWorkitemCount;

		// 要素数などを表示
		cout << ";要素数" << endl
			 << ": " << elementCount << endl
 			 << ";ワークグループ数" << endl
			 << ": " << (globalWorkitemCount % localWorkitemCount + globalWorkitemCount/localWorkitemCount) << endl
			 << ";全ワークアイテム数" << endl
			 << ": " << globalWorkitemCount << endl
			 << ";ワークグループ内ワークアイテム数" << endl
			 << ": " << localWorkitemCount  << endl; 
		

/*****************/
		cout << endl
			 << "== 初期化処理 == "<< endl
			 << "# ベクトルの初期化" << endl;

		// 入力値を初期化
		cl_double* inputA = new cl_double[globalWorkitemCount]; 
		cl_double* inputB = new cl_double[globalWorkitemCount]; 

		// 乱数生成器の作成
		boost::variate_generator<boost::minstd_rand&, boost::uniform_real<cl_double>> random(
			boost::minstd_rand(42),
			boost::uniform_real<cl_double>(-0, 10));

		// 全要素について
		for(unsigned long i = 0; i < elementCount; i++)
		{
			// 入力値をランダムで与える
			inputA[i] = random();
			inputB[i] = random();
		}

		// OpenCL用出力
		cl_double* outputCL = new cl_double[globalWorkitemCount];

		// CPU用出力
		cl_double* outputCPU = new cl_double[elementCount];

	
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

		size_t maxSize = device.getInfo<CL_DEVICE_MAX_PARAMETER_SIZE>();


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
		cl::Buffer bufferInputA = cl::Buffer(context, CL_MEM_READ_ONLY, sizeof(cl_double) * globalWorkitemCount);
		cl::Buffer bufferInputB = cl::Buffer(context, CL_MEM_READ_ONLY, sizeof(cl_double) * globalWorkitemCount);

		// 出力のバッファーを書きこみ専用で作成
		cl::Buffer bufferOutput = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_double) * globalWorkitemCount);


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

		// 入力値の引数を設定
		kernel.setArg(0, bufferInputA);
		kernel.setArg(1, bufferInputB);

		// 出力値の引数を設定
		kernel.setArg(2, bufferOutput);

		// 要素数の引数を設定
		kernel.setArg(3, elementCount);

/*****************/
		cout << endl
			 << "== 計算の実行 ==" << endl
		     << "# デバイスにデータを書き込み" << endl;

		// 非同期で入力値を書き込み
		queue.enqueueWriteBuffer(bufferInputA, CL_FALSE, 0, sizeof(cl_double) * globalWorkitemCount, inputA);
		queue.enqueueWriteBuffer(bufferInputB, CL_FALSE, 0, sizeof(cl_double) * globalWorkitemCount, inputB);

/*****************/
		cout << "# カーネルの実行" << endl;

		// カーネル実行のイベント
		cl::Event kernelEvent;

		// カーネルを実行
		queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(globalWorkitemCount), cl::NDRange(localWorkitemCount), NULL, &kernelEvent);

		// 実行終了まで待機
		kernelEvent.wait();

/*****************/
		cout << "# デバイスからデータを読み込み" << endl;

		// 同期で出力値に読み込み
		queue.enqueueReadBuffer(bufferOutput, CL_TRUE, 0, sizeof(cl_double) * globalWorkitemCount, outputCL);

/*****************/
		cout << endl
			 << "== 結果の比較 == " << endl
			 << "# CPUで計算" << endl;

		// 全要素について
		for(unsigned long i = 0; i < elementCount; i++)
		{
			// CPUで足し算を計算
			outputCPU[i] = inputA[i] + inputB[i];
		}


		cout << "# 誤差を評価" << endl;

		// 誤差
		double error = 0;

		// 全要素について
		for(unsigned long i = 0; i < elementCount; i++)
		{
			// 誤差を加える
			error += abs(outputCPU[i] - outputCL[i]);
		}

		// 誤差を表示
		cout << ";誤差" << endl
			 << ": " << error / elementCount << endl;

/*****************/
		delete[] inputA;
		delete[] inputB;
		delete[] outputCPU;
		delete[] outputCL;
	}
}
#undef foreach