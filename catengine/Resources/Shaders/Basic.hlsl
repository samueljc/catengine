// constant buffer
cbuffer MatrixBuffer
{
  float4x4 wvp;
};

// input layouts
struct VSInput
{
    float3 position : POSITION0;
    float4 color    : COLOR0;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color    : COLOR;
};

// vertex shader
PSInput VSBasic(VSInput input)
{
  PSInput output;

  output.position = mul(float4(input.position, 1.0f), wvp);
  output.color = input.color;

  return output;
}

// pixel shader
float4 PSBasic(PSInput input) : SV_TARGET
{
  return input.color;
}