
//CPU側のデータ型と合わせる必要がある。
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
	float dspThrdId;//ひとまずこれだけ使用する
	uint groupIndex;
};
RWStructuredBuffer<Buffer_t> outBuff:register(u0);
//idを書き込むだけのCS
[numthreads(4, 4, 4)]
void main(uint3 dtid : SV_DispatchThreadID) {
	//ただただIDを代入
	outBuff[dtid.x * 8 * 8 + dtid.y * 8 + dtid.z].dspThrdId = dtid.x * 8 * 8 + dtid.y * 8 + dtid.z;
}