#include "pch.h"
#include "HLSLShader.h"

namespace Psycho
{
	HLSLShader::~HLSLShader()
	{
		Release();
	}

	void HLSLShader::Initialize(const LPCWSTR fileName, const ShaderType shaderType)
	{
		UINT flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_ALL_RESOURCES_BOUND;

		const char* target = "";
		switch (shaderType)
		{
			case ShaderType::VERTEX:
			{
				target = "vs_5_1"; break;
			}

			case ShaderType::PIXEL:
			{
				target = "ps_5_1"; break;
			}

			default:
			{
				PRINT_N("Error. Unsupported Shader Compilation Type")
				return;
			}

		}
		ID3DBlob* errorMessage = nullptr;

		HRESULT result = D3DCompileFromFile(fileName, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", target, flags, 0, &byteCode, &errorMessage);
		{
			if (result != S_OK)
			{
				PRINT_N("Error Loading Shader File: " << result);
			}
			
			if (errorMessage)
			{
				PRINT_N("Shader Compilation Error: " << (const char*)errorMessage->GetBufferPointer());
				return;
			}

			PRINT_W_N("Loaded Shader: " << fileName);
		}
	}
	void HLSLShader::Release()
	{
		if (byteCode)
		{
			byteCode->Release();
		}
	}
}