#pragma once

WZZ_D3D12_BEGIN


class SingleShaderResourceBindingsLayout
{
public:

};

// necessary?
class ReadOnlySingleShaderReosurceBindings : public SingleShaderResourceBindingsLayout;


class SingleShaderResourceBindings : public SingleShaderResourceBindingsLayout
{
public:

};

class ShaderResourceBindingGroup 
{
public:
	void setOnCommandList();


};

WZZ_D3D12_END