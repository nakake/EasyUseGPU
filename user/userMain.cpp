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
	// Visual studio�ł̃J�����g�f�B���N�g��(�t�@�C�����o�͂����ꏊ)��
	// project�t�@�C���̂���ꏊ/x64/Debug or Relese/�J�����g�f�B���N�g��
	// �ƂȂ��Ă���
	// �Ȃ̂Ń��C���t�@�C���̂���f�B���N�g���Ƀt�@�C���o�͂��������ꍇ��
	// ../../user(�{�t���[�����[�N�̃��[�U�[���C���t�H���_)/�t�@�C����
	// �Ƃ���K�v������
	// 
	// HLSL�t�@�C���̎w��ł̓t���[�����[�N���Ńt�@�C���w��p�̊֐���
	// ���܂��Ă���̂Ńt�@�C�����w��݂̂œ��삷��
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