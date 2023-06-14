#include "common.hpp"
#include "device.hpp"
#include "device_context.hpp"




WZZ_D3D11_BEGIN

template<typename Vertex>
class VertexBuffer{
    
    ComPtr<ID3D11Buffer> _buffer;
    uint32_t _vtx_cnt;

    public:
    

        VertexBuffer() = default;

    void initialize(uint32_t vtxCnt, const Vertex* data);

    void initialize(size_t vtxCnt, ComPtr<ID3D11Buffer> buffer);

    bool isAvailable() const noexcept;

    void destroy();

    uint32_t getVertexCount() const noexcept;

    void bind(int inSlot) const;

    void unbind(int inSlot) const;

    ComPtr<ID3D11Buffer> getBuffer() const;

};


template<typename Index>
class IndexBuffer{

};



template<typename Struct>
class ConstantBuffer{

};




WZZ_D3D11_END