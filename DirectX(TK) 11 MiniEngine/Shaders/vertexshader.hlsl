cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float xOffset;
    float yOffset;
}

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
    input.inPos.x += xOffset;
    input.inPos.y += yOffset;
    
    output.outPos = float4(input.inPos, 1.0f);
    output.outTexCoord = input.inTexCoord;

    return output;
}
