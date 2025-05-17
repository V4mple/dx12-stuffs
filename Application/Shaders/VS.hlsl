struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

struct PassData
{
    float4x4 world;
    float4x4 viewproj;
};

ConstantBuffer<PassData> globalPassData : register(b0);

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    float4 worldPos = mul(globalPassData.world, float4(input.position, 1.0f));
    output.position = mul(globalPassData.viewproj, worldPos);
    output.color = input.color;
    return output;
}