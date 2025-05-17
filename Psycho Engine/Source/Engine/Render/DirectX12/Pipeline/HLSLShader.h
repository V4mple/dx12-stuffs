#pragma once

namespace Psycho
{
	class HLSLShader
	{
	public:
		enum ShaderType
		{
			VERTEX,
			PIXEL,

		};

		HLSLShader() = default;
		~HLSLShader();

		void Initialize(const LPCWSTR fileName, const ShaderType shaderType);
		void Release();

		inline ID3DBlob* GetByteBlob() { return byteCode; }

	private:
		ID3DBlob* byteCode = 0;
	};
}