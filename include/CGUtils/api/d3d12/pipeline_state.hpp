#pragma once

#include "CGUtils/api/d3d12/d3d12_api.hpp"

WZZ_D3D12_BEGIN

class PipelineState
{
public:
	PipelineState();

	~PipelineState();

private:
	ComPtr<ID3D12PipelineState> m_dx_pipelineState;
	ComPtr<ID3D12RootSignature> m_dx_rootSignature; 
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_dx_psoDesc;
};


WZZ_D3D12_END