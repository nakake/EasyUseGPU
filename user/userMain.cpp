#include <EasyUseGPU.hpp>
#include <fstream>
#include <string>

void Main() {
	vector<int> input1(1024);
	vector<int> input2(1024);
	vector<int> output(1024);

	for (auto& num : input1) {
		num = 5;
	}

	int i = 0;
	for (auto& num : input2) {
		num = i;
		i++;
	}

	Resource::SetUploadBufferData(input1);
	Resource::SetUploadBufferData(input2);
	Resource::SetOutputBufferData(output);

	EUG_PARALLEL_STATE state = {};
	state.FilePath = L"ComputeShader.hlsl";
	state.pDefines.push_back({ "DATA_X", "32" });
	state.pDefines.push_back({ "DATA_Y", "32" });

	Parallel::SetParaState(&state);
	Parallel::ExecutionGPU(1, 1, 1);
	Resource::GetBufferDataFromGPU(output, 0);


	// note
	//
	// Visual studioでのカレントディレクトリ(ファイルが出力される場所)は
	// projectファイルのある場所/x64/Debug or Relese/カレントディレクトリ
	// となっている
	// なのでメインファイルのあるディレクトリにファイル出力をしたい場合は
	// ../../user(本フレームワークのユーザーメインフォルダ)/ファイル名
	// とする必要がある
	// 
	// HLSLファイルの指定ではフレームワーク内でファイル指定用の関数を
	// かましているのでファイル名指定のみで動作する
	//
	std::ofstream file;
	std::string fileName = "../../user/result2.txt";
	file.open(fileName, std::ios::out);
	int index = 0;
	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 32; j++) {
			file << output[index] << " ";
			index++;
		}
		file << endl;
	}
	file << endl;
	file.close();
}