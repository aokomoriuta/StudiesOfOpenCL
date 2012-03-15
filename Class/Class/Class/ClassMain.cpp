#include "ClassMain.hpp"

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
#include "Particle.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::string;

namespace Class
{
	//! アプリケーションを実行する
	int ClassMain::Main()
	{
		try
		{
			// 計算を実行
			ClassMain::Compute();
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
	void ClassMain::Compute()
	{
		// 実行開始
		cout << "= クラス（構造体）を使用したテスト =" << endl;

		// OpenCL Cソースファイル名
		const string filepath = "Class.cl";

		// エントリポイント名
		const string entryPoint = "MoveParticles";

		// 粒子数
		const cl_uint count = 4;

		// 最大値と最小値
		const cl_float minValue = -10;
		const cl_float maxValue = +10;

		// 時間刻み
		cl_float dt = 0.1f;

/*****************/
		// 粒子数を表示
		cout << ";粒子数" << endl
		     << ": " << count << endl;
		

/*****************/
		cout << endl
			 << "# 粒子の初期化" << endl;

		// OpenCL用粒子
		Particle* particlesCL = new Particle[count];

		// CPU用粒子
		Particle* particlesCPU = new Particle[count];

		// 乱数生成器の作成
		boost::minstd_rand gen(42);
		boost::uniform_real<> dst(minValue, maxValue);
		boost::variate_generator<
			boost::minstd_rand&, boost::uniform_real<>
		> random(gen, dst);
		random();

		// 全要素について
		for(cl_uint i = 0; i < count; i++)
		{
			// 位置と速度をランダムで与える
			cl_float4 x;
			x.s[0] = random();
			x.s[1] = random();
			x.s[2] = random();
			x.s[3] = 0;
			
			cl_float4 u;
			u.s[0] = random();
			u.s[1] = random();
			u.s[2] = random();
			u.s[3] = 0;

			// 位置を速度を設定
			particlesCL [i].x = x;
			particlesCPU[i].x = x;
			particlesCL [i].u = u;
			particlesCPU[i].u = u;
		}

		// タイマーで時間測定
		boost::timer timer = boost::timer();


/*****************/
		cout << "# CPUの計算";
		timer.restart();

		// 全要素について
		for(cl_uint i = 0; i < count; i++)
		{
			// xyzそれぞれ加算する
			particlesCPU[i].x.s[0] += particlesCPU[i].u.s[0] * dt;
			particlesCPU[i].x.s[1] += particlesCPU[i].u.s[1] * dt;
			particlesCPU[i].x.s[2] += particlesCPU[i].u.s[2] * dt;
		}
		cout << " - " << timer.elapsed() << "[s]" << endl;
	
/*****************/
		cout << "# プラットフォームを取得" << endl;

		// プラットフォーム一覧を取得
		vector<cl::Platform> platforms = vector<cl::Platform>();
		cl::Platform::get(&platforms);

		// 先頭のプラットフォームを取得
		cl::Platform platform = platforms[0];

		// プラットフォーム情報を表示
		cout << "#* プラットフォーム名：" << platform.getInfo<CL_PLATFORM_NAME>() << endl
		     << "#* OpenCLバージョン：" << platform.getInfo<CL_PLATFORM_VERSION>() << endl;


/*****************/
		cout << "# デバイス一覧を取得" << endl;
		
		// デバイス一覧を取得
		vector<cl::Device> devices = vector<cl::Device>();
		platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

		// 先頭のデバイスを取得
		cl::Device device = devices[0];

		// デバイス情報を表示
		cout << "#* デバイス名：" << device.getInfo<CL_DEVICE_NAME>() << endl
			 << "#* クロック周波数[MHz]：" << device.getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>() << endl
			 << "#* 並列計算コア数：" << device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << endl;


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

		// データ転送用バッファーを読み書き可能にして作成
		cl::Buffer particlesBuffer = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(Particle) * count);

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
		cout << endl
			 << "== 計算の実行 ==" << endl
		     << "# デバイスにデータを書き込み" << endl;
		timer.restart();

		// 非同期で入力値を書き込み
		queue.enqueueWriteBuffer(particlesBuffer, CL_FALSE, 0, sizeof(Particle) * count, particlesCL);

/*****************/
		cout << "# カーネルを実行" << endl;

		// 引数を設定
		// # 粒子数
		// # 粒子群
		// # 移動量
		kernel.setArg(0, count);
		kernel.setArg(1, particlesBuffer);
		kernel.setArg(2, dt);

		// カーネルを実行
		queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(count), cl::NullRange);

		cout << "#: " << timer.elapsed() << "[s]" << endl;
		
		// 結果を読み込み
		queue.enqueueReadBuffer(particlesBuffer, CL_TRUE, 0, sizeof(Particle)*count, particlesCL);

/*****************/
		cout << endl
			 << "== 結果 == " << endl;

		// 全粒子について
		for(int i = 0; i < count; i++)
		{
			// 位置を取得
			cl_float3 xCL  = particlesCL [i].x;
			cl_float3 xCPU = particlesCPU[i].x;

			// 位置が異なっていれば
			if(xCL.s[0] != xCPU.s[0])
			{
				// 出力
				cout << i << "(CPU= " << xCPU.s[0] << ", " << "CL= " << xCL.s[0] << ")" << endl;
			}
		}


/*****************/
		delete[] particlesCL;
		delete[] particlesCPU;
	}
}