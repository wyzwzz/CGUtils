#pragma once

#include <d3dcompiler.h>
#include <map>
#include "device.hpp"
#include "device_context.hpp"

#include <CGUtils/file/raw_file_io.hpp>

WZZ_D3D11_BEGIN

enum class ShaderStage{
    VS = 0,
    HS,
    DS,
    GS,
    PS,
    CS
};

constexpr ShaderStage VS = ShaderStage::VS;
constexpr ShaderStage HS = ShaderStage::HS;
constexpr ShaderStage DS = ShaderStage::DS;
constexpr ShaderStage GS = ShaderStage::GS;
constexpr ShaderStage PS = ShaderStage::PS;
constexpr ShaderStage CS = ShaderStage::CS;


namespace detail{
    template<ShaderStage STAGE>
    void BindConstantBuffer(uint32_t slot, ID3D11Buffer* buffer);

    #define D3D11_BIND_CONSTANT_BUFFER(STAGE)\
    template<>\
    inline void BindConstantBuffer<ShaderStage::STAGE>\
    (uint32_t slot, ID3D11Buffer* buffer)\
    {                                                                 \
        d3d_dev_ctx->STAGE##SetConstantBuffers(slot, 1, &buffer);          \
    }

    D3D11_BIND_CONSTANT_BUFFER(VS)
    D3D11_BIND_CONSTANT_BUFFER(HS)
    D3D11_BIND_CONSTANT_BUFFER(DS)
    D3D11_BIND_CONSTANT_BUFFER(GS)
    D3D11_BIND_CONSTANT_BUFFER(PS)
    D3D11_BIND_CONSTANT_BUFFER(CS)

    #undef D3D11_BIND_CONSTANT_BUFFER

//--------------------------------------------------------------------

    template<ShaderStage STAGE>
    void BindSampler(uint32_t slot, ID3D11SamplerState* sampler);

    #define D3D11_BIND_SAMPLER(STAGE)\
    template<>\
    inline void BindSampler<STAGE>(uint32_t slot, ID3D11SamplerState* sampler){\
        d3d_dev_ctx->STAGE##SetSamplers(slot, 1, &sampler);\
    } 

    D3D11_BIND_SAMPLER(VS)
    D3D11_BIND_SAMPLER(HS)
    D3D11_BIND_SAMPLER(DS)
    D3D11_BIND_SAMPLER(GS)
    D3D11_BIND_SAMPLER(PS)
    D3D11_BIND_SAMPLER(CS)

    #undef D3D11_BIND_SAMPLER

//--------------------------------------------------------------------

    template<ShaderStage STAGE>
    void BindUnorderedAccessView(uint32_t slot, ID3D11UnorderedAccessView* uav, uint32_t initialCounter);

    template<>
    void BindUnorderedAccessView<CS>(uint32_t slot, ID3D11UnorderedAccessView* uav, uint32_t initialCounter){
        d3d_dev_ctx->CSSetUnorderedAccessViews(slot, 1, &uav, &initialCounter);
    }

//--------------------------------------------------------------------

    template<ShaderStage STAGE>
    void BindShaderResourceViewArray(uint32_t slot, uint32_t count, ID3D11ShaderResourceView** srvs);

    #define D3D11_BIND_SHADER_RESOURCE_VIEW_ARRAY(STAGE) \
    template<> \
    inline void BindShaderResourceViewArray<STAGE>(uint32_t slot, uint32_t count, ID3D11ShaderResourceView** srvs){ \
        d3d_dev_ctx->STAGE##SetShaderResources(slot, count, srvs); \
    }

    D3D11_BIND_SHADER_RESOURCE_VIEW_ARRAY(VS)
    D3D11_BIND_SHADER_RESOURCE_VIEW_ARRAY(HS)
    D3D11_BIND_SHADER_RESOURCE_VIEW_ARRAY(DS)
    D3D11_BIND_SHADER_RESOURCE_VIEW_ARRAY(GS)
    D3D11_BIND_SHADER_RESOURCE_VIEW_ARRAY(PS)
    D3D11_BIND_SHADER_RESOURCE_VIEW_ARRAY(CS)

//--------------------------------------------------------------------

    template<ShaderStage STAGE>
    void BindShaderResourceView(uint32_t slot, ID3D11ShaderResourceView* srv){
        BindShaderResourceViewArray<STAGE>(slot, 1, &srv);
    }


}

template<ShaderStage STAGE>
class ConstantBufferSlot{
public:
    explicit ConstantBufferSlot(uint32_t slot = 0) noexcept
    :_slot(slot)
    {

    }


    void setBuffer(ComPtr<ID3D11Buffer> buffer) noexcept{
        _buffer.Swap(buffer);
    }

    uint32_t getSlot() const noexcept{
        return _slot;
    }

    void bind() const{
        detail::BindConstantBuffer(_slot, _buffer.Get());    
    }

    void unbind() const{
        detail::BindConstantBuffer(_slot, nullptr);
    }


private:
    uint32_t _slot;
    ComPtr<ID3D11Buffer> _buffer;
};


template<ShaderStage STAGE>
class SamplerSlot{
public:
    explicit SamplerSlot(uint32_t slot = 0) noexcept
    :_slot(slot)
    {

    }

    void setSampler(ComPtr<ID3D11SamplerState> sampler){
        _sampler.Swap(sampler);
    }

    auto getSlot() const noexcept {
        return _slot;
    }

    void bind() const {
        detail::BindSampler<STAGE>(_slot, _sampler.Get());
    }

    void unbind() const {
        detail::BindSampler<STAGE>(_slot, nullptr);
    }

private:
    uint32_t _slot;
    ComPtr<ID3D11SamplerState> _sampler;
};

template<ShaderStage STAGE>
class ShaderResourceViewSlot{
public:
    explicit ShaderResourceViewSlot(uint32_t slot = 0) noexcept
    :_slot(slot)
    {

    }

    void setShaderResourceView(ComPtr<ID3D11ShaderResourceView> srv){
        _srv.Swap(srv);
    }

    auto getSlot() const {
        return _slot;
    }

    void bind() const {
        detail::BindShaderResourceView<STAGE>(_slot, _srv.Get());
    }

    void unbind() const {
        detail::BindShaderResourceView<STAGE>(_slot, nullptr);
    }

private:
    uint32_t _slot;
    ComPtr<ID3D11ShaderResourceView> _srv;
};

template<ShaderStage STAGE>
class ShaderResourceViewArraySlot{
public:
    using Self = ShaderResourceViewArraySlot<STAGE>;

    explicit ShaderResourceViewArraySlot(uint32_t slot = 0, uint32_t count = 1)
    :_slot(slot), _srvs(count)
    {

    }

    ShaderResourceViewArraySlot(const Self& other){
        _slot = other._slot;
        _srvs = other._srvs;
        addRef();    
    }

    Self& operator=(const Self& other){
        releaseRef();
        _slot = other._slot;
        _srvs = other._srvs;
        addRef();
        return *this;
    }

    ShaderResourceViewArraySlot(Self&& other) noexcept {
        _slot = other._slot;
        _srvs = std::move(other._srvs);
        other._slot = 0;
    }

    Self& operator=(Self&& other) noexcept {
        releaseRef();
        _slot = other._slot;
        _srvs = std::move(other._srvs);
        other._slot = 0;
        return *this;
    }

    ~ShaderResourceViewArraySlot(){
        releaseRef();
    }

    int getSRVCount() const noexcept {
        return _srvs.size();
    }

    void setShaderResourceView(int index, ID3D11ShaderResourceView* srv){
        assert(index < _srvs.size() && index >= 0);
        auto& dst = _srvs[index];
        if(dst) dst->Release();
        dst = srv;
        if(dst) dst->AddRef();
    }

    void setShaderResourceView(int index, const ComPtr<ID3D11ShaderResourceView>& srv){
        setShaderResourceView(index, srv.Get());
    }

    void setShaderResourceView(int begin, int count, ID3D11ShaderResourceView** srvs){
        for(auto index = begin, i = 0; i < count; i++, index++)
            setShaderResourceView(index, srvs[i]);
    }

    void bind() const {
        detail::BindShaderResourceViewArray<STAGE>(_slot, _srvs.size(), _srvs.data());
    }

    void unbind() const {
        static std::vector<ID3D11ShaderResourceView> _null;
        _null.clear();
        _null.resize(_srvs.size(), nullptr);
        detail::BindShaderResourceViewArray<STAGE>(_slot, _srvs.size(), _null.data());
    }


private:
    void addRef() noexcept {
        for(auto srv : _srvs) if(srv) srv->AddRef();
    }

    void releaseRef() noexcept {
        for(auto srv : _srvs) if(srv) srv->Release();
    }
private:
    uint32_t _slot;
    mutable std::vector<ID3D11ShaderResourceView*> _srvs;
};

template<ShaderStage STAGE>
class UnorderedAccessViewSlot{
public:
    explicit UnorderedAccessViewSlot(uint32_t slot = 0) noexcept
    :_slot(slot), _initial_counter(-1)
    {

    }

    void setUnorderedAccessView(ComPtr<ID3D11UnorderedAccessView> uav, uint32_t initialCounter = -1){
        _uav.Swap(uav);
        _initial_counter = initialCounter;
    }

    auto getSlot() const noexcept {
        return _slot;
    }

    void bind() const{
        detail::BindUnorderedAccessView<STAGE>(_slot, _uav.Get(), _initial_counter);
    }
 
    void unbind() const{
        detail::BindUnorderedAccessView<STAGE>(_slot, nullptr, -1);
    }

private:
    uint32_t _slot;
    ComPtr<ID3D11UnorderedAccessView> _uav;
    uint32_t _initial_counter;
};


namespace detail{
    template<typename RESOURCE>
    class ResourceManager{
    public:
        using ResourceMap = std::map<std::string, RESOURCE>;

        ResourceManager() = default;

        explicit ResourceManager(std::map<std::string, RESOURCE> mp) noexcept
        : resc_mp(std::move(mp))
        {

        }

        template<typename... Args>
        void emplace(std::string name, Args&&... args){
            assert(resc_mp.count(name) == 0);
            resc_mp.insert(
                std::make_pair(std::move(name,
                RESOURCE(std::forward<Args>(args)...))));
        }
        
        void push(std::string name, const RESOURCE& resc){
            assert(resc_mp.count(name) == 0);
            resc_mp.insert(std::make_pair(std::move(name), resc));
        }

        RESOURCE* get(const std::string& name){
            auto it = resc_mp.find(name);
            return it == resc_mp.end() ? nullptr : &it->second;
        }

        const RESOURCE* get(const std::string& name) const {
            auto it = resc_mp.find(name);
            return it == resc_mp.end() ? nullptr : &it->second;
        }

        void bind() const {
            for(auto& [name, resc] : resc_mp)
                resc.bind();
        }

        void unbind() const {
            for(auto& [name, resc] : resc_mp)
                resc.unbind();
        }
    private:
        ResourceMap resc_mp;
    };
}



template<ShaderStage STAGE>
using ConstantBufferSlotMgr = detail::ResourceManager<ConstantBufferSlot<STAGE>>;

template<ShaderStage STAGE>
using SamplerSlotMgr = detail::ResourceManager<SamplerSlot<STAGE>>;

template<ShaderStage STAGE>
using ShaderResourceViewSlotMgr = detail::ResourceManager<ShaderResourceViewSlot<STAGE>>;

template<ShaderStage STAGE>
using ShaderResourceViewArraySlotMgr = detail::ResourceManager<ShaderResourceViewArraySlot<STAGE>>;

template<ShaderStage STAGE>
using UnorderedAccessViewSlotMgr = detail::ResourceManager<UnorderedAccessViewSlot<STAGE>>;

namespace detail{


template<ShaderStage STAGE>
struct StageToD3D11ShaderType;
template<> struct StageToD3D11ShaderType<VS> { using Type = ID3D11VertexShader; };
template<> struct StageToD3D11ShaderType<HS> { using Type = ID3D11HullShader; };
template<> struct StageToD3D11ShaderType<DS> { using Type = ID3D11DomainShader; };
template<> struct StageToD3D11ShaderType<GS> { using Type = ID3D11GeometryShader; };
template<> struct StageToD3D11ShaderType<PS> { using Type = ID3D11PixelShader; };
template<> struct StageToD3D11ShaderType<CS> { using Type = ID3D11ComputeShader; };

template<ShaderStage STAGE>
class StageShaderHelper{
    public:
    using D3D11ShaderType = typename StageToD3D11ShaderType<STAGE>::Type;

    static const char* DefaultCompileTarget() noexcept{
        if constexpr(STAGE == VS) return "vs_5_0";
        if constexpr(STAGE == HS) return "hs_5_0";
        if constexpr(STAGE == DS) return "ds_5_0";
        if constexpr(STAGE == GS) return "gs_5_0";
        if constexpr(STAGE == PS) return "ps_5_0";
        if constexpr(STAGE == CS) return "cs_5_0";
        assert(false); 
    }

#ifdef WZZ_DEBUG
    static constexpr uint32_t _compile_flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    static constexpr uint32_t _compile_flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif


    static ComPtr<ID3D10Blob> Compile(
        std::string_view source,
        const char* sourceName,
        const char* target,
        const char* entry,
        const D3D_SHADER_MACRO* macros
    ){
        ComPtr<ID3D10Blob> ret, err;
        auto hr = D3DCompile(
            source.data(), source.size(), sourceName, macros,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entry, target,
            _compile_flags, 0,
            ret.GetAddressOf(), err.GetAddressOf()
        );
        if(FAILED(hr)){
            throw D3D11Exception(reinterpret_cast<char*>(err->GetBufferPointer()));
        }
        return ret;
    }

    static ComPtr<D3D11ShaderType> CreateShader(void* compiled, size_t len){
        ComPtr<D3D11ShaderType> shader;
        HRESULT hr;
        if constexpr(STAGE == VS){
            hr = d3d_device->CreateVertexShader(compiled, len, nullptr, shader.GetAddressOf());
        }
        else if constexpr(STAGE == HS){
            hr = d3d_device->CreateHullShader(compiled, len, nullptr, shader.GetAddressOf());
        }
        else if constexpr(STAGE == DS){
            hr = d3d_device->CreateDomainShader(compiled, len, nullptr, shader.GetAddressOf());
        }
        else if constexpr(STAGE == GS){
            hr = d3d_device->CreateGeometryShader(compiled, len, nullptr, shader.GetAddressOf());
        }
        else if constexpr(STAGE == PS){
            hr = d3d_device->CreatePixelShader(compiled, len, nullptr, shader.GetAddressOf());
        }
        else if constexpr(STAGE == CS){
            hr = d3d_device->CreateComputeShader(compiled, len, nullptr, shader.GetAddressOf());
        }
        else{
            assert(false);
        }
    }

    static void Bind(D3D11ShaderType* shader){
        if constexpr(STAGE == VS){
            d3d_dev_ctx->VSSetShader(shader, nullptr, 0);
        }
        else if constexpr(STAGE == HS){
            d3d_dev_ctx->HSSetShader(shader, nullptr, 0);
        }
        else if constexpr(STAGE == DS){
            d3d_dev_ctx->DSSetShader(shader, nullptr, 0);
        }
        else if constexpr(STAGE == GS){
            d3d_dev_ctx->GSSetShader(shader, nullptr, 0);
        }
        else if constexpr(STAGE == PS){
            d3d_dev_ctx->PSSetShader(shader, nullptr, 0);
        }
        else if constexpr(STAGE == CS){
            d3d_dev_ctx->CSSetShader(shader, nullptr, 0);
        }
        else{
            assert(false);
        }
    }

};

inline ComPtr<ID3D11ShaderReflection> GetShaderReflection(ID3D10Blob* byteCode){
    assert(byteCode);
    ComPtr<ID3D11ShaderReflection> refl;
    auto hr = D3DReflect(byteCode->GetBufferPointer(), byteCode->GetBufferSize(),
    IID_ID3D11ShaderReflection, (void**)refl.GetAddressOf());
    assert(hr && refl.Get());
    return refl;
}

struct ConstantBufferReflInfo{
    uint32_t slot;  
};

struct ShaderResourceReflInfo{
    uint32_t slot;
    uint32_t count;
};

struct UnorderedAccessReflInfo{
    uint32_t slot;
};

struct ShaderSamplerReflInfo{
    uint32_t slot;
};

inline void GetShaderReflectionInfo(
    ID3D11ShaderReflection* refl,
    std::map<std::string, ConstantBufferReflInfo>&  constantBufferReflInfo,
    std::map<std::string, ShaderResourceReflInfo>&  shaderResourceReflInfo,
    std::map<std::string, UnorderedAccessReflInfo>& unorderedAccessReflInfo,
    std::map<std::string, ShaderSamplerReflInfo>&   shaderSamplerReflInfo){

    assert(refl);

    D3D11_SHADER_DESC shader_desc;
    refl->GetDesc(&shader_desc);

    for(auto resc = 0u; resc < shader_desc.BoundResources; ++resc){
        D3D11_SHADER_INPUT_BIND_DESC bind_desc;
        refl->GetResourceBindingDesc(resc, &bind_desc);
        if(bind_desc.Type == D3D_SIT_CBUFFER){
            constantBufferReflInfo.insert(
                std::make_pair(std::string(bind_desc.Name),
                ConstantBufferReflInfo{bind_desc.BindPoint})
            );
        }
        else if(bind_desc.Type == D3D_SIT_TEXTURE ||
                bind_desc.Type == D3D_SIT_STRUCTURED ||
                bind_desc.Type == D3D_SIT_BYTEADDRESS){
            shaderResourceReflInfo.insert(std::make_pair(
                std::string(bind_desc.Name),
                ShaderResourceReflInfo{bind_desc.BindPoint, bind_desc.BindCount}
            ));
        }
        else if(bind_desc.Type == D3D_SIT_SAMPLER){
            shaderSamplerReflInfo.insert(std::make_pair(
                std::string(bind_desc.Name),
                ShaderSamplerReflInfo{bind_desc.BindPoint}
            ));
        }
        else if(bind_desc.Type == D3D_SIT_UAV_RWTYPED ||
                bind_desc.Type == D3D_SIT_UAV_RWSTRUCTURED ||
                bind_desc.Type == D3D_SIT_UAV_RWBYTEADDRESS ||
                bind_desc.Type == D3D_SIT_UAV_APPEND_STRUCTURED ||
                bind_desc.Type == D3D_SIT_UAV_CONSUME_STRUCTURED ||
                bind_desc.Type == D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER){
            assert(bind_desc.BindCount == 1);

            unorderedAccessReflInfo.insert(std::make_pair(
                std::string(bind_desc.Name),
                UnorderedAccessReflInfo{bind_desc.BindPoint}
            ));
        }
    }

}


template<ShaderStage STAGE>
class StageShader : misc::no_copy_t{
    ComPtr<typename StageShaderHelper<STAGE>::D3D11ShaderType> _stage_shader;
    ComPtr<ID3D10Blob> _byte_code_blob;


    using ConstantBufferSlotMgrMp = ConstantBufferSlotMgr<STAGE>::ResourceMap;
    using SamplerSlotMgrMp = SamplerSlotMgr<STAGE>::ResourceMap;
    using ShaderResourceViewSlotMgrMp = ShaderResourceViewSlotMgr<STAGE>::ResourceMap;
    using ShaderResourceViewSlotArrayMgrMp = ShaderResourceViewArraySlotMgr<STAGE>::ResourceMap;
    using UnorderedAccessViewSlotMgrMp = UnorderedAccessViewSlotMgr<STAGE>::ResourceMap;

    ConstantBufferSlotMgrMp _cb_slot_mp;
    SamplerSlotMgrMp _sam_slot_mp;
    ShaderResourceViewSlotMgrMp _srv_slot_mp;
    ShaderResourceViewSlotArrayMgrMp _srvs_slot_mp;
    UnorderedAccessViewSlotMgrMp _uav_slot_mp;

    // get reflection infos from shader code
    void initRescMp(){
        auto refl = GetShaderReflection(_byte_code_blob.Get());
        if(!refl){
            throw D3D11Exception("Failed to get shader reflection");
        }

        std::map<std::string, ConstantBufferReflInfo> constant_buffers_mp;
        std::map<std::string, ShaderResourceReflInfo> shader_resource_mp;
        std::map<std::string, UnorderedAccessReflInfo> unordered_access_mp;
        std::map<std::string, ShaderSamplerReflInfo> shader_sampler_mp;

        GetShaderReflectionInfo(refl.Get(),
                                constant_buffers_mp,
                                shader_resource_mp,
                                unordered_access_mp,
                                shader_sampler_mp);


        for(auto& [name, info] : constant_buffers_mp){
            _cb_slot_mp[name] = ConstantBufferSlot<STAGE>(info.slot);
        }

        for(auto& [name, info] : unordered_access_mp){
            _uav_slot_mp[name] = UnorderedAccessViewSlot<STAGE>(info.slot);
        }

        for(auto& [name, info] : shader_sampler_mp){
            _sam_slot_mp[name] = SamplerSlot<STAGE>(info.slot);
        }

        for(auto& [name, info] : shader_resource_mp){
            auto p = name.find('[');
            if(p == std::string::npos){
                assert(info.count == 1);
                _srv_slot_mp[name] = ShaderResourceViewSlot<STAGE>(info.slot);
            }
            else{
                _srvs_slot_mp[name] = ShaderResourceViewArraySlot<STAGE>(info.slot, info.count);
            }
        }


    }

public:
    using StageShaderHelper = StageShaderHelper<STAGE>;

    void initialize(
        std::string_view src,
        const char* srcName,
        const char* entry,
        const char* target,
        const D3D_SHADER_MACRO* macros)
    {
        _byte_code_blob = StageShaderHelper::Compile(src, srcName, target, entry, macros);

        _stage_shader = StageShaderHelper::CreateShader(
            _byte_code_blob->GetBufferPoint(), _byte_code_blob->GetBufferSize());

        initRescMp(); 
    }

    void bind() const{
        StageShaderHelper::Bind(_stage_shader.Get());
    }

    void unbind() const{
        StageShaderHelper::Bind(nullptr);
    }

    auto& getConstantBufferMap() const noexcept {

    }

    auto& getShaderResourceViewMap() const noexcept {

    }

    auto& getShaderResourceViewArrayMap() const noexcept {

    }

    auto& getUnorderedAccessViewMap() const noexcept {

    }

    auto& getSamplerMap() const noexcept {

    }

    auto getShaderByteCode() const noexcept {
        return _byte_code_blob.Get();
    }

};

template<ShaderStage STAGE>
class StageResourceMgr{

};

}// end of namespace detail


template<ShaderStage... STAGES>
class ShaderResourceMgr{

};

template<ShaderStage... STAGES>
class Shader : misc::no_copy_t{
public:
    using Self = Shader<STAGES...>;

    template<ShaderStage STAGE>
    static const char* DefaultTarget() noexcept{
        return detail::StageShader<STAGE>::Traits::DefaultTarget();
    }


    template<ShaderStage STAGE>
    Self& initializeStage(
        std::string_view source,
        const char* sourceName,
        const D3D_SHADER_MACRO* macros = nullptr,
        const char* entry = "main",
        const char* target = DefaultTarget<STAGE>()
    ){
        auto& stage_shader = std::get<detail::StageShader<STAGE>>(_stage_shaders);
        //todo

        return *this;
    }

    template<ShaderStage STAGE>
    Self& initializeStageFromFile(
        const std::string& filename,
        const D3D_SHADER_MACRO* macros = nullptr,
        const char* entry = "main",
        const char* target = DefaultTarget<STAGE>()
    ){
        auto src = file::read_txt_file(filename);
        initializeStage<STAGE>(src, filename,macros, entry, target);
        return *this;
    }


 
    void bind() const {
        // https://www.cppstories.com/2022/tuple-iteration-apply/
        std::apply([](const auto&... stage){
            (stage.bind(), ...);
        }, _stage_shaders);
    }

    void unbind() const {
        std::apply([](const auto&... stage){
            (stage.unbind(), ...);
        }, _stage_shaders);
    }

private:
    std::tuple<detail::StageShader<STAGES>...> _stage_shaders;
};

WZZ_D3D11_END