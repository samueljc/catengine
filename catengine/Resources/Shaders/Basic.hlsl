// constant buffer
cbuffer MatrixBuffer
{
	float4x4 world_matrix;
	float4x4 view_matrix;
	float4x4 proj_matrix;
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
    float4 color    : COLOR0;
};

// vertex shader
PSInput VSBasic(VSInput input)
{
  PSInput output;
  output.position = float4(input.position, 1.0f);

  // Calculate the position of the vertex against the world, view, and projection matrices.
  output.position = mul(output.position, world_matrix);
  output.position = mul(output.position, view_matrix);
  output.position = mul(output.position, proj_matrix);

  // Store the input color for the pixel shader to use.
  output.color = input.color;

  return output;
}

// pixel shader
float4 PSBasic(PSInput input) : SV_TARGET
{
  return input.color;
}