#pragma once

#include "common.hpp"

WZZ_D3D11_BEGIN

class RenderTarget : public misc::no_copy_t{

public:
    RenderTarget();



    void swap(RenderTarget& other) noexcept;

    Int2 getSize() const noexcept;

    float getWOverH() const noexcept;

    void addColorBuffer(DXGI_FORMAT format, DXGI_FORMAT rtvFormat);

    void addColorBuffer(
        DXGI_FORMAT format,
        DXGI_FORMAT rtvFormat,
        DXGI_FORMAT srvFormat
    );

    void addColorBuffer(
        ComPtr<ID3D11Texture2D> tex,
        ComPtr<ID3D11RenderTargetView> rtv,
        ComPtr<ID3D11ShaderResourceView> srv
    );

    void addDepthStencil(DXGI_FORMAT format, DXGI_FORMAT rtvFormat);

    void addDepthStencil(
        DXGI_FORMAT format,
        DXGI_FORMAT rtvFormat,
        DXGI_FORMAT srvFormat
    );

    void addDepthStencil(
        ComPtr<ID3D11Texture2D> tex,
        ComPtr<ID3D11RenderTargetView> rtv,
        ComPtr<ID3D11ShaderResourceView> srv
    );

    void clearColorBuffer(int index, const Color4& color);

    void clearDepth(float depth);

    void clearStencil(uint8_t stencil);

    void clearDepthStencil(float depth, uint8_t stencil);

    ComPtr<ID3D11ShaderResourceView> getColorShaderResourceView(int index) const;

    ComPtr<ID3D11ShaderResourceView> getDepthShaderResourceView() const;

    void useDefaultViewport() const;

    void bind() const;
    
    void unbind() const;

private:


};


WZZ_D3D11_END
