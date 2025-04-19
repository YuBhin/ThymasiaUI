#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D		g_Texture;
Texture2D       g_TexIcon;
Texture2D       g_TexEdge;
Texture2D       g_TexEffect;

bool g_bIconOn; // ������ ���� �̹��� ���� �ݱ�
bool g_bSkillOn; // ������ ���� �̹��� ���� �ݱ�

float g_fTimeDelta; // �ΰ��� ������Ʈ �ð���
float g_fTImeAlpha; // �ð��� ���� ���� ��

bool g_bImageOn; // True ���� �� �̹��� ����� ����
bool g_bImageLoopOn; // true ���� �� �̹��� ����� ���ڱ��� ���� 

bool g_bTexIconOff; // true ���� �� �̹��� ����� ���ڱ��� ���� 
bool g_bTexEdgeOff; // true ���� �� �̹��� ����� ���ڱ��� ���� 
bool g_bTexEffectOff; // true ���� �� �̹��� ����� ���ڱ��� ���� 

uint g_SlotNum;// �÷��̾� ȭ�� ��ų ���� �ΰ��� ����� �޶� ��ų �����ܰ��� ������ ���߱� ���� �޾ƿ�  

float2 g_TexScale;

struct VS_IN
{
	float3			vPosition : POSITION;	
	float2			vTexcoord : TEXCOORD0;	
};

struct VS_OUT
{
	float4			vPosition : SV_POSITION;
	float2			vTexcoord : TEXCOORD0;
	float4			vWorldPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{	
	VS_OUT			Out = (VS_OUT)0;

	Out.vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	Out.vWorldPos = Out.vPosition;

	Out.vPosition = mul(Out.vPosition, g_ViewMatrix);
	Out.vPosition = mul(Out.vPosition, g_ProjMatrix);	
	Out.vTexcoord = In.vTexcoord;

	return Out;
}

struct PS_IN
{
	float4			vPosition : SV_POSITION;
	float2			vTexcoord : TEXCOORD0;
	float4			vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
	float4			vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	/* Sample : g_Texture�� ���� ������ ��ǥ�� ���� ������ ������� ���´�.*/
	// g_Texture.Sample(��� ���ð���(Sampler_State), ��� ���� �����ð���)
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord); 

    
    //if (0.1f >= Out.vColor.a)
    //    discard;


    return Out;
}

PS_OUT PS_Thymesia_UI(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	/* Sample : g_Texture�� ���� ������ ��ǥ�� ���� ������ ������� ���´�.*/
	// g_Texture.Sample(��� ���ð���(Sampler_State), ��� ���� �����ð���)
    
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    //Out.vColor = float4(1, 0, 0, 1);
    return Out;
}

PS_OUT PS_Thymesia_UI_ImageOnOff(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	/* Sample : g_Texture�� ���� ������ ��ǥ�� ���� ������ ������� ���´�.*/
	// g_Texture.Sample(��� ���ð���(Sampler_State), ��� ���� �����ð���)
    
    if (true == g_bImageOn )
    {
     Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    }
    else
    {
        Out.vColor.a = 0.0f;

    }
    

    return Out;
}

PS_OUT PS_Thymesia_UI_ImageOnOffLoop(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

     Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    if (true == g_bImageLoopOn ) // ���� ���� ������ ��
    {
        Out.vColor.rgb *= abs(g_fTImeAlpha);

    }
    return Out;
}

PS_OUT PS_Thymesia_UI_Image_ItemSlot(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    float4 vBackColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    float4 vFrontColor = g_TexIcon.Sample(LinearSampler, In.vTexcoord);
    
    if (!g_bIconOn)
        vFrontColor.a = 0.0f;
    
    Out.vColor = lerp(vBackColor, vFrontColor, vFrontColor.a);
    
    return Out;
}

PS_OUT PS_Thymesia_UI_Image_Pause(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
     Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    Out.vColor.a -= 0.6f;
    return Out;
}

PS_OUT PS_Thymesia_UI_Image_TalentSlot(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float2 ChangeUV = ((In.vTexcoord - 0.5) * 0.5 + 0.5f);
    ChangeUV = saturate(ChangeUV); // UV ������ [0, 1]�� ����
    
    float4 vBackColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    float4 vFrontColor = g_TexIcon.Sample(LinearSampler, ChangeUV);
    
    Out.vColor = lerp(vBackColor, vFrontColor, vFrontColor.a);
    
    return Out;
}

PS_OUT PS_Thymesia_UI_Image_TalentSlot2(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float2 ChangeUV = ((In.vTexcoord - 0.5) * 2.0 + 0.5f);
    ChangeUV = saturate(ChangeUV); // UV ������ [0, 1]�� ����

    float4 vTexColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    float4 vTexIconColor = g_TexIcon.Sample(LinearSampler, In.vTexcoord);
    float4 vTexEdgeColor = g_TexEdge.Sample(LinearSampler, In.vTexcoord);
    float4 vTexEffectColor = g_TexEffect.Sample(LinearSampler, In.vTexcoord);

    
    //vTexEffectColor = lerp(vTexEffectColor, vTexColor, vTexColor.a);
    vTexColor = lerp(vTexColor, vTexEdgeColor, vTexEdgeColor.a);
    Out.vColor = lerp(vTexColor, vTexIconColor, vTexIconColor.a);
    
    
    return Out;
}

PS_OUT PS_Thymesia_UI_Image_TalentSlot3(PS_IN In)
{
   PS_OUT Out = (PS_OUT) 0;
    
    float2 ChangeUV = ((In.vTexcoord - 0.5) * 2.0 + 0.5f);
    ChangeUV = saturate(ChangeUV); // UV ������ [0, 1]�� ����

    float4 vTexColor = g_Texture.Sample(LinearSampler, ChangeUV);
    float4 vTexIconColor = g_TexIcon.Sample(LinearSampler, In.vTexcoord);
    float4 vTexEdgeColor = g_TexEdge.Sample(LinearSampler, In.vTexcoord);
    float4 vTexEffectColor = g_TexEffect.Sample(LinearSampler, In.vTexcoord);

    
    vTexEffectColor = lerp(vTexEffectColor, vTexColor, vTexColor.a);
    vTexEffectColor = lerp(vTexEffectColor, vTexEdgeColor, vTexEdgeColor.a);
    Out.vColor = lerp(vTexEffectColor, vTexIconColor, vTexIconColor.a);
    
    
        return Out;
    }

PS_OUT PS_Thymesia_UI_Image_PlunderSlot(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    float4 vBackColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    float4 vFrontColor;
    
    float2 ChangeUV = ((In.vTexcoord - 0.5) * 0.5 + 0.5f);
    ChangeUV = saturate(ChangeUV); // UV ������ [0, 1]�� ����
    
    if (0 == g_SlotNum)
        vFrontColor = g_TexIcon.Sample(LinearSampler, ChangeUV);
    else
        vFrontColor = g_TexIcon.Sample(LinearSampler, In.vTexcoord);
    
    if (!g_bIconOn)
        vFrontColor.a = 0.0f;
    
    
    
    
    Out.vColor = lerp(vBackColor, vFrontColor, vFrontColor.a);
    
    Out.vColor.rgb = ((Out.vColor.rgb - 0.5) * 0.5)+0.5; // �����ϴ°�?
    //Out.vColor.rgb += 0.1; // ��Ⱑ ������ 
    
    return Out;
}

PS_OUT PS_Thymesia_UI_Image_Skill_CoolTime(PS_IN In) // ��ų ��� �� ��Ÿ�� �����
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    
    if (!g_bImageOn)
        Out.vColor.a = 0.0f;
    
    
    
    return Out;
}

PS_OUT PS_Thymesia_UI_9Slice_Scaling(PS_IN In) // �׵θ� �̹���
{
    PS_OUT Out = (PS_OUT) 0;
    
    float2 TextureSize = { 156.f, 150.f }; // ���ҽ� ��ü ũ��(�ȼ�)
    float4 EdgeSizes = { 50.f, 50.f, 50.f, 50.f }; // �����̽� �� ������ ���� ���� 
    float2 fOutSize = { 312.f, 300.f }; // ���������� ����ϰ� ���� ������
    
    float2 uv = In.vTexcoord;
    float2 edgeScale = EdgeSizes.xy / TextureSize.xy; // �»�� ���� ũ��
    float2 centralScale = float2(1.0, 1.0) - (EdgeSizes.zw + EdgeSizes.xy) / TextureSize.xy; // �߾� ũ��

    if (uv.x < edgeScale.x && uv.y < edgeScale.y)
    {
    // �»�� ����
        Out.vColor = g_Texture.Sample(LinearSampler_Clamp, uv);
    }
    else if (uv.x > 1.0 - edgeScale.x && uv.y < edgeScale.y)
    {
    // ���� ����
        Out.vColor = g_Texture.Sample(LinearSampler_Clamp, uv);
    }
    else if (uv.x < edgeScale.x && uv.y > 1.0 - edgeScale.y)
    {
    // ���ϴ� ����
        Out.vColor = g_Texture.Sample(LinearSampler_Clamp, uv);
    }
    else if (uv.x > 1.0 - edgeScale.x && uv.y > 1.0 - edgeScale.y)
    {
    // ���ϴ� ����
        Out.vColor = g_Texture.Sample(LinearSampler_Clamp, uv);
    }
    else if (uv.y < edgeScale.y)
    {
    // ��� �׵θ� (���η� �����ϸ�)
        float2 localUV = float2((uv.x - edgeScale.x) / centralScale.x, uv.y);
        Out.vColor = g_Texture.Sample(LinearSampler_Clamp, localUV);
    }
    else if (uv.y > 1.0 - edgeScale.y)
    {
    // �ϴ� �׵θ� (���η� �����ϸ�)
        float2 localUV = float2((uv.x - edgeScale.x) / centralScale.x, uv.y);
        Out.vColor = g_Texture.Sample(LinearSampler_Clamp, localUV);
    }
    else if (uv.x < edgeScale.x)
    {
    // ���� �׵θ� (���η� �����ϸ�)
        float2 localUV = float2(uv.x, (uv.y - edgeScale.y) / centralScale.y);
        Out.vColor = g_Texture.Sample(LinearSampler_Clamp, localUV);
    }
    else if (uv.x > 1.0 - edgeScale.x)
    {
    // ���� �׵θ� (���η� �����ϸ�)
        float2 localUV = float2(uv.x, (uv.y - edgeScale.y) / centralScale.y);
        Out.vColor = g_Texture.Sample(LinearSampler_Clamp, localUV);
    }
    else
    {
    // �߾� ���� (����+���η� �����ϸ�)
        float2 localUV = (uv - edgeScale) / centralScale;
        Out.vColor = g_Texture.Sample(LinearSampler_Clamp, localUV);
    }
    
    return Out;
}


technique11 DefaultTechnique
{
  // 0��
pass DefaultPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

pass Thymesia_UI // 1��
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI();
    }

pass Thymesia_UI_ImageOnOff // 2��
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_ImageOnOff();
    }

pass Thymesia_UI_ImageOnOffLoop // 3��
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_ImageOnOffLoop();
    }

pass Thymesia_UI_Image_ItemSlot // 4��
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Image_ItemSlot();
    }
    
pass Thymesia_UI_Image_Pause // 5��
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Image_Pause();
    }
    
pass Thymesia_UI_Image_TalentSlot // 6��
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Image_TalentSlot3();
    }
    

pass Thymesia_UI_Image_PlunderSlot // 7��
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Image_PlunderSlot();
    }
    
pass Thymesia_UI_Image_SkillIcon // 8��
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Image_Skill_CoolTime();
    }

pass Thymesia_UI_Image_Frame // 9��
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_9Slice_Scaling();
    }

}
