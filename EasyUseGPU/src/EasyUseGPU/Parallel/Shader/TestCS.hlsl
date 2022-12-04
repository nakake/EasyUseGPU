

RWStructuredBuffer<float> Outputs:register(u0);
StructuredBuffer<float>   Inputs1:register(t0);
StructuredBuffer<float>   Inputs2:register(t1);

[numthreads(4, 1, 1)]
void MulMatrix( uint3 dtid : SV_DispatchThreadID )
{
	Outputs[dtid.x] = Inputs1[dtid.x] + Inputs2[dtid.x];
}