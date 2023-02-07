StructuredBuffer<int> Input1:register(t0);
StructuredBuffer<int> Input2:register(t1);
RWStructuredBuffer<int> Output:register(u0);

#define THREAD_X 9
#define THREAD_Y 1
#define THREAD_Z 1

[numthreads(THREAD_X, THREAD_Y, THREAD_Z)]
void main(uint3 dtid : SV_DispatchThreadID)
{
	int index1, index2;
	index1 = (dtid.x / DATA_X) * (THREAD_X / DATA_X);
	index2 = dtid.x % DATA_X;
	Output[dtid.x] = 0;
	for (int i = 0; i < THREAD_X / DATA_X; ++i)
	{
		Output[dtid.x] += Input1[index1] * Input2[index2];
		++index1;
		index2 += THREAD_X / DATA_X;
	}
}