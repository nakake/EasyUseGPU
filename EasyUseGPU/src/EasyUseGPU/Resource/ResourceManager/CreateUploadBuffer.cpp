#include "ResourceManager.hpp"

namespace eug
{
	namespace Resource
	{
		ComPtr<ID3D12Resource> CreateUploadBuffer(
			ID3D12Device* pDevice,
			ID3D12GraphicsCommandList* pCmdList,
			const void* initData,
			uint64_t byteSize,
			ComPtr<ID3D12Resource>& pUploadBuffer
		)
		{
			ComPtr<ID3D12Resource> ResourceBuffer;

			auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
			auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);

			pDevice->CreateCommittedResource(
				&heapProp,
				D3D12_HEAP_FLAG_NONE,
				&resourceDesc,
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(ResourceBuffer.GetAddressOf()));

			heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
			resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);

			pDevice->CreateCommittedResource(
				&heapProp,
				D3D12_HEAP_FLAG_NONE,
				&resourceDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(pUploadBuffer.GetAddressOf()));

			D3D12_SUBRESOURCE_DATA subResourceData = {};
			subResourceData.pData = initData;
			subResourceData.RowPitch = byteSize;
			subResourceData.SlicePitch = subResourceData.RowPitch;

			auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(ResourceBuffer.Get(),
				D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
			pCmdList->ResourceBarrier(1, &resourceBarrier);

			UpdateSubresources<1>(pCmdList, ResourceBuffer.Get(), pUploadBuffer.Get(), 0, 0, 1, &subResourceData);

			resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(ResourceBuffer.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
			pCmdList->ResourceBarrier(1, &resourceBarrier);


			/*
			*
			* 注意点
			* コピーバッファーはコピー後も削除してはいけない
			* 実際にコマンドリストの実行(Dispach)後にコピーが行われるため
			*
			*/

			return ResourceBuffer;
		}

	}
}