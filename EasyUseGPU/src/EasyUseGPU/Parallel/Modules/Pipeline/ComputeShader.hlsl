
//CPU���̃f�[�^�^�ƍ��킹��K�v������B
struct SimpleBuffer_t
{
	int		i;
	float	f;
};

StructuredBuffer<SimpleBuffer_t>	inBuff0 : register(t0);
StructuredBuffer<SimpleBuffer_t>	inBuff1 : register(t1);

struct Buffer_t {
	float grpId;
	float grpThrdId;
	float dspThrdId;//�ЂƂ܂����ꂾ���g�p����
	uint groupIndex;
};
RWStructuredBuffer<Buffer_t> outBuff:register(u0);
//id���������ނ�����CS
[numthreads(4, 4, 4)]
void main(uint3 dtid : SV_DispatchThreadID) {
	//��������ID����
	outBuff[dtid.x * 8 * 8 + dtid.y * 8 + dtid.z].dspThrdId = dtid.x * 8 * 8 + dtid.y * 8 + dtid.z;
}