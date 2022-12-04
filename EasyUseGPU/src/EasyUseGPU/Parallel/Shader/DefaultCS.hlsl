
RWStructuredBuffer<float> FloatOutputs:register(u0);
StructuredBuffer<float>   FloatInputs1:register(t0);
StructuredBuffer<float>   FloatInputs2:register(t1);

//行列(Float)計算用エントリーポイント
[numthreads(threadX, threadY, threadZ)]
void MulFloatMatrix(uint3 dtid : SV_DispatchThreadID)
{
	if (dtid.x < threadX && dtid.y < threadY)
	{
		FloatOutputs[dtid.x + dtid.y * threadY] = 0;
		for (int i = 0; i < threadX; i++)
		{
			for (int j = i % threadX; j < threadY; j += threadY)
			{
				FloatOutputs[dtid.x + dtid.y * threadY] += FloatInputs1[i] * FloatInputs2[j];
			}
		}
	}
}

RWStructuredBuffer<int> IntOutputs:register(u0);
StructuredBuffer<int>   IntInputs1:register(t0);
StructuredBuffer<int>   IntInputs2:register(t1);

//行列(int)計算用エントリーポイント
[numthreads(threadX, threadY, threadZ)]
void MulIntMatrix( uint3 dtid : SV_DispatchThreadID )
{
	int OutputIndex = dtid.x + dtid.y * threadY;

	if (dtid.x < threadX && dtid.y < threadY)
	{
		IntOutputs[OutputIndex] = 0;
		for (int i = 0; i < threadX; i++)
		{
			for (int j = i % threadX; j < threadY; j += threadY)
			{
				IntOutputs[OutputIndex] += IntInputs1[i] * IntInputs2[j];
			}
		}
	}
	
}