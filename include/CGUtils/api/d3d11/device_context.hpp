#pragma once

#include "common.hpp"
#include <stack>

WZZ_D3D11_BEGIN

class DeviceContext{
public:
    ComPtr<ID3D11DeviceContext> d3d11_dev_ctx;

    void setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology);

    void setInputLayout(ID3D11InputLayout* inputLayout);

    void draw(uint32_t vtxCnt, uint32_t vtxOffset = 0);

    void drawIndexed(uint32_t idxCnt, uint32_t idxOffset = 0, uint32_t vtxOffset = 0);

    void dispatch(uint32_t x, uint32_t y = 1, uint32_t z = 1);

    auto operator->() {
        return d3d11_dev_ctx.Get();
    }

    auto operator->() const {
        return d3d11_dev_ctx.Get();
    }

    void setCurrent();

    void pushCurrent();

    void popCurrent();

    auto tempSet() {
        pushCurrent();
        return wzz::misc::scope_bomb_t([&](){
            return popCurrent();
        });
    }
};

inline std::stack<DeviceContext> d3d_dev_ctx_st;

inline DeviceContext d3d_dev_ctx = {nullptr};

void DeviceContext::setCurrent(){
    d3d_dev_ctx = *this;
    if(d3d_dev_ctx_st.empty()){
        d3d_dev_ctx_st.push(*this);
    }
    else{
        d3d_dev_ctx_st.top() = *this;
    }
}

void DeviceContext::pushCurrent(){
    d3d_dev_ctx = *this;
    d3d_dev_ctx_st.push(*this);
}

void DeviceContext::popCurrent(){
    if(!d3d_dev_ctx_st.empty()){
        d3d_dev_ctx_st.pop();
        if(!d3d_dev_ctx_st.empty()){
            d3d_dev_ctx = d3d_dev_ctx_st.top();
        }
    }
}


WZZ_D3D11_END