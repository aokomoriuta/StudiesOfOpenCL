using System;

namespace LWisteria.StudiesOfOpenCL.SimpleViennaCL
{
	/// <summary>
	/// ViennaCLを使う試験を兼ねた簡易プログラム
	/// </summary>
	static class SimpleViennaCLMain
	{
		/// <summary>
		/// 要素数
		/// </summary>
		const int N = 345678;

		/// <summary>
		/// エントリポイント
		/// </summary>
		/// <returns></returns>
		static int Main()
		{
			// 配列を初期化
			var result = new double[N];
			var left = new double[N];
			var right = new double[N];


			// ストップウォッチ作成
			var stopwatch = new System.Diagnostics.Stopwatch();

			Console.WriteLine("CPU => ");
			{
				// 各要素の値を
				for(int i = 0; i < N; i++)
				{
					// それぞれ設定
					result[i] = 0;
					left[i] = i;
					right[i] = (double)i / N;
				}

				// CPUで加算を作成
				var computer = new ViennaCLAdd.ComputerCpu(N);

				// 入力
				stopwatch.Restart();
				computer.Write(left, right);
				var inputTime = stopwatch.ElapsedMilliseconds;

				// 演算実行
				stopwatch.Restart();
				computer.Add();
				var executionTime = stopwatch.ElapsedMilliseconds;

				// 出力
				stopwatch.Restart();
				computer.Read(result);
				var outputTime = stopwatch.ElapsedMilliseconds;

				// 実行時間表示
				Console.WriteLine("{0, 5} + {1, 5} + {2, 5} = {3, 5}", inputTime, executionTime, outputTime, inputTime + executionTime + outputTime);

				// 結果の表示
				for(int i = 0; i < N; i++)
				{
					if(left[i] + right[i] != result[i])
					{
						Console.WriteLine("{0} + {1} = {2}", left[i], right[i], result[i]);
					}
				}
			} 
			
			Console.WriteLine("GPU => ");
			{
				// 各要素の値を
				for(int i = 0; i < N; i++)
				{
					// それぞれ設定
					result[i] = 0;
					left[i] = i;
					right[i] = (double)i / N;
				}

				// GPUで加算を作成
				var computer = new ViennaCLAdd.ComputerGpu(N);

				// 入力
				stopwatch.Restart();
				computer.Write(left, right);
				var inputTime = stopwatch.ElapsedMilliseconds;

				// 演算実行
				stopwatch.Restart();
				computer.Add();
				var executionTime = stopwatch.ElapsedMilliseconds;

				// 出力
				stopwatch.Restart();
				computer.Read(result);
				var outputTime = stopwatch.ElapsedMilliseconds;

				// 実行時間表示
				Console.WriteLine("{0, 5} + {1, 5} + {2, 5} = {3, 5}", inputTime, executionTime, outputTime, inputTime + executionTime + outputTime);

				// 結果の表示
				for(int i = 0; i < N; i++)
				{
					if(left[i] + right[i] != result[i])
					{
						Console.WriteLine("{0} + {1} = {2}", left[i], right[i], result[i]);
					}
				}
			}

			return Environment.ExitCode;
		}
	}
}