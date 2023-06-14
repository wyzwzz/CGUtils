#pragma once

#include "common.hpp"

#include <stack>

WZZ_D3D11_BEGIN


class Device{
public:
    ComPtr<ID3D11Device1> d3d11_device;

    auto operator->(){
        return d3d11_device.Get();
    }

    auto operator->() const{
        return d3d11_device.Get();
    }

    ComPtr<ID3D11Texture1D> createTexture1D(
        const D3D11_TEXTURE1D_DESC& desc,
        const D3D11_SUBRESOURCE_DATA* subrscData = nullptr
    );

    ComPtr<ID3D11Texture2D> createTexture2D(
        const D3D11_TEXTURE2D_DESC& desc,
        const D3D11_SUBRESOURCE_DATA* subrscData = nullptr
    );

    ComPtr<ID3D11Texture3D> createTexture3D(
        const D3D11_TEXTURE3D_DESC& desc,
        const D3D11_SUBRESOURCE_DATA* subrscData = nullptr
    );

    ComPtr<ID3D11Buffer> createBuffer(
        const D3D11_BUFFER_DESC& desc,
        const D3D11_SUBRESOURCE_DATA* subrscData = nullptr
    );

    ComPtr<ID3D11ShaderResourceView> createSRV(
        ID3D11Resource* rsc,
        const D3D11_SHADER_RESOURCE_VIEW_DESC& desc
    );

    ComPtr<ID3D11UnorderedAccessView> createUVA(
        ID3D11Resource* rsc,
        const D3D11_UNORDERED_ACCESS_VIEW_DESC& desc
    );

    ComPtr<ID3D11RenderTargetView> createRTV(
        ID3D11Resource* rsc,
        const D3D11_RENDER_TARGET_VIEW_DESC& desc
    );

    ComPtr<ID3D11DepthStencilView> createDSV(
        ID3D11Resource* rsc,
        const D3D11_DEPTH_STENCIL_VIEW_DESC& desc
    );

    ComPtr<ID3D11BlendState> createBS(
        const D3D11_BLEND_DESC& desc
    );

    ComPtr<ID3D11DepthStencilState> createDSS(
        const D3D11_DEPTH_STENCIL_DESC& desc
    );

    ComPtr<ID3D11RasterizerState> createRS(
        const D3D11_RASTERIZER_DESC& desc
    );

    ComPtr<ID3D11SamplerState> createSampler(
        const D3D11_SAMPLER_DESC&
    );

    ComPtr<ID3D11SamplerState> createSampler(
        D3D11_FILTER filter,
        D3D11_TEXTURE_ADDRESS_MODE addressU,
        D3D11_TEXTURE_ADDRESS_MODE addressV,
        D3D11_TEXTURE_ADDRESS_MODE addressW,
        float mipLODBias = 0.f,
        uint32_t maxAnisotropy = 1,
        D3D11_COMPARISON_FUNC comparisonFunc = D3D11_COMPARISON_NEVER,
        const Color4& boardColor = {0.f, 0.f, 0.f, 0.f},
        float minLOD = -FLT_MAX,
        float maxLOD =  FLT_MAX
    );


    void setCurrent();

    void pushCurrent();

    void popCurrent();

    auto tempSet() {
        pushCurrent();
        return misc::scope_bomb_t([&](){
            return popCurrent();
        });
    }
};

inline std::stack<Device> d3d_device_st;

inline Device d3d_device = {nullptr};


void Device::setCurrent(){
    d3d_device = *this;
    if(d3d_device_st.empty()){
        d3d_device_st.push(*this);
    }
    else{
        d3d_device_st.top() = *this;
    }
}

void Device::pushCurrent(){
    d3d_device = *this;
    d3d_device_st.push(*this);
}

void Device::popCurrent(){
    if(!d3d_device_st.empty()){
        d3d_device_st.pop();
        if(!d3d_device_st.empty()){
            d3d_device = d3d_device_st.top();
        }
    }
}


WZZ_D3D11_END