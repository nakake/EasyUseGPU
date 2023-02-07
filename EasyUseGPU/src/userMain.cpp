#include <EasyUseGPU.hpp>
#include <fstream>
#include <string>

void Main() {
	vector<int> input1{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	vector<int> input2{10,11,12,13,14,15,16,17,18 };
	vector<int> output(9);

	Resource::SetUploadBufferData(input1);
	Resource::SetUploadBufferData(input2);
	Resource::SetOutputBufferData(output);

	EUG_PARALLEL_STATE state = {};
	state.FilePath = L"ComputeShader.hlsl";
	state.pDefines.push_back({ "DATA_X", "3" });
	state.pDefines.push_back({ "DATA_Y", "3" });

	Parallel::SetParaState(&state);
	Parallel::ExecutionGPU(1, 1, 1);
	Resource::GetBufferDataFromGPU(output, 0);

	std::ofstream file;
	std::string fileName = "../../EasyUseGPU/src/result.txt";
	file.open(fileName, std::ios::out);
	int index = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++) {
			file << output[index] << " ";
			index++;
		}
		file << endl;
	}
	file << endl;
	file.close();
}