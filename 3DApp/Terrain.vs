cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	//float3 tangent : TANGENT;
	//float3 binormal : BINORMAL;
    //float3 color : COLOR;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	//float3 tangent : TANGENT;
	//float3 binormal : BINORMAL;
    //float4 color : COLOR;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType TerrainVertexShader(VertexInputType input)
{
    PixelInputType output;
    //float4 worldPosition;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
    // Calculate the normal vector against the world matrix only and then normalize the final value
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	//Calculate the viewing direction based on the position of the camera and the position of the vertex in the world
	//worldPosition = mul(input.position, worldMatrix);
	//output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	//output.viewDirection = normalize(output.viewDirection);

	//Calculate the tangent vector against the world matrix only and the normalize the final value
	//output.tangent = mul(input.tangent, (float3x3)worldMatrix);
	//output.tangent = normalize(output.tangent);

	//Calculate the binormal vector against the world matrix only and the normalize the final value
	//output.binormal = mul(input.binormal, (float3x3)worldMatrix);
	//output.binormal = normalize(output.binormal);
	
	//output.lightDirection = normalize(-lightDirection.xyz);
    // Store the input color for the pixel shader to use.
    //output.color = float4(input.color, 1.0f);

    return output;
}