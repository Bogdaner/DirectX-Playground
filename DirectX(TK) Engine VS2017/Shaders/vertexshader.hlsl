struct VS_Input
{
    float3 inPos : POSITION;
    float2 inTexCoord : TEXCOORD;
};

struct VS_Output
{
    float4 outPos : SV_POSITION;
    float2 outTexCoord : TEXCOORD;
};

VS_Output main(VS_Input input)
{
    VS_Output output;
    output.outPos = float4(input.inPos, 1.0f);
    output.outTexCoord = input.inTexCoord;

    return output;
}
