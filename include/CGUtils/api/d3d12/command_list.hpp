#pragma once

#include "common.hpp"

WZZ_D3D12_BEGIN

class CommandList
{
public:

	ComPtr<ID3D12CommandList> m_command_list = nullptr;
	
	CommandList();
	~CommandList();

	auto operator->() -> ID3D12CommandList*
	{
		return m_command_list.Get();
	}

	auto operator->() const -> ID3D12CommandList*
	{
		return m_command_list.Get();
	}
};

WZZ_D3D12_END