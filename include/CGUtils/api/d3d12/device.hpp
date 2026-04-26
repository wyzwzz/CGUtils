#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>    // 用于 ComPtr
#include <memory>
#include <vector>

#include "common.hpp"

WZZ_D3D12_BEGIN

class Device {
public:
	// 设备创建配置
	struct Desc {
		bool debug_layer{true};		// 是否启用调试层
		bool gpu_validation{false};	// 是否启用 GPU 验证
		uint32_t back_buffer_count{2};	// 后缓冲数量
	};

	Device(const Desc& desc = {});
	~Device();

	// 禁用拷贝
	Device(const Device&) = delete;
	Device& operator=(const Device&) = delete;

	// 设备信息查询
	bool SupportRayTracing() const;
	bool SupportMeshShader() const;
	D3D_FEATURE_LEVEL GetFeatureLevel() const;
	
	// 获取原生设备接口
	ID3D12Device* GetNative() const { return m_device.Get(); }
	IDXGIFactory6* GetFactory() const { return m_factory.Get(); }
	
	// 适配器相关
	void EnumerateAdapters();
	void SelectAdapter(uint32_t index);
	const std::vector<ComPtr<IDXGIAdapter4>>& GetAdapters() const { return m_adapters; }

private:
	void CreateDevice();
	void SetupDebugLayer();
	
	ComPtr<ID3D12Device> m_device;
	ComPtr<IDXGIFactory6> m_factory;
	ComPtr<IDXGIAdapter4> m_adapter;
	std::vector<ComPtr<IDXGIAdapter4>> m_adapters;
	
	D3D_FEATURE_LEVEL m_featureLevel{D3D_FEATURE_LEVEL_12_0};
	Desc m_desc;
};

WZZ_D3D12_END