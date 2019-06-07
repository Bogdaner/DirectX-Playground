struct VS_Input
{
    float2 inPos : POSITION;
    float3 inColor : COLOR;
};

struct VS_Output
{
    float4 outPos : SV_POSITION;
    float3 outColor : COLOR;
};

VS_Output main(VS_Input input)
{
    VS_Output output;
    output.outPos = float4(input.inPos, 0.0f, 1.0f);
    output.outColor = input.inColor;

    return output;
}
