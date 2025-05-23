#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D		g_Texture;
Texture2D       g_TexIcon;
Texture2D       g_TexEdge;
Texture2D       g_TexEffect;

bool g_bIconOn; // 아이템 슬롯 이미지 열고 닫기
bool g_bSkillOn; // 아이템 슬롯 이미지 열고 닫기

float g_fTimeDelta; // 인게임 업데이트 시간값
float g_fTImeAlpha; // 시간에 따른 알파 값

bool g_bImageOn; // True 값일 때 이미지 모습이 보임
bool g_bImageLoopOn; // true 값일 때 이미지 모습이 깜박깜박 보임 

bool g_bTexIconOff; // true 값일 때 이미지 모습이 깜박깜박 보임 
bool g_bTexEdgeOff; // true 값일 때 이미지 모습이 깜박깜박 보임 
bool g_bTexEffectOff; // true 값일 때 이미지 모습이 깜박깜박 보임 

uint g_SlotNum;// 플레이어 화면 스킬 슬롯 두개의 사이즈가 달라 스킬 아이콘과의 비율을 맞추기 위해 받아옴  

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

	/* Sample : g_Texture로 부터 지정된 좌표의 색을 지정한 방식으로 얻어온다.*/
	// g_Texture.Sample(어떻게 얻어올건지(Sampler_State), 어디 색을 가져올건지)
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord); 

    
    //if (0.1f >= Out.vColor.a)
    //    discard;


    return Out;
}

PS_OUT PS_Thymesia_UI(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	/* Sample : g_Texture로 부터 지정된 좌표의 색을 지정한 방식으로 얻어온다.*/
	// g_Texture.Sample(어떻게 얻어올건지(Sampler_State), 어디 색을 가져올건지)
    
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    //Out.vColor = float4(1, 0, 0, 1);
    return Out;
}

PS_OUT PS_Thymesia_UI_ImageOnOff(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	/* Sample : g_Texture로 부터 지정된 좌표의 색을 지정한 방식으로 얻어온다.*/
	// g_Texture.Sample(어떻게 얻어올건지(Sampler_State), 어디 색을 가져올건지)
    
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
    
    if (true == g_bImageLoopOn ) // 루프 값이 켜졌을 때
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
    ChangeUV = saturate(ChangeUV); // UV 범위를 [0, 1]로 제한
    
    float4 vBackColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    float4 vFrontColor = g_TexIcon.Sample(LinearSampler, ChangeUV);
    
    Out.vColor = lerp(vBackColor, vFrontColor, vFrontColor.a);
    
    return Out;
}

PS_OUT PS_Thymesia_UI_Image_TalentSlot2(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float2 ChangeUV = ((In.vTexcoord - 0.5) * 2.0 + 0.5f);
    ChangeUV = saturate(ChangeUV); // UV 범위를 [0, 1]로 제한

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
    ChangeUV = saturate(ChangeUV); // UV 범위를 [0, 1]로 제한

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
    ChangeUV = saturate(ChangeUV); // UV 범위를 [0, 1]로 제한
    
    if (0 == g_SlotNum)
        vFrontColor = g_TexIcon.Sample(LinearSampler, ChangeUV);
    else
        vFrontColor = g_TexIcon.Sample(LinearSampler, In.vTexcoord);
    
    if (!g_bIconOn)
        vFrontColor.a = 0.0f;
    
    
    
    
    Out.vColor = lerp(vBackColor, vFrontColor, vFrontColor.a);
    
    Out.vColor.rgb = ((Out.vColor.rgb - 0.5) * 0.5)+0.5; // 대조하는거?
    //Out.vColor.rgb += 0.1; // 밝기가 조정됨 
    
    return Out;
}

PS_OUT PS_Thymesia_UI_Image_Skill_CoolTime(PS_IN In) // 스킬 사용 시 쿨타임 연출용
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    
    if (!g_bImageOn)
        Out.vColor.a = 0.0f;
    
    
    
    return Out;
}

PS_OUT PS_Thymesia_UI_9Slice_Scaling(PS_IN In) // 테두리 이미지
{
    PS_OUT Out = (PS_OUT) 0;
    
    float2 TextureSize = { 156.f, 150.f }; // 리소스 자체 크기(픽셀)
    float4 EdgeSizes = { 50.f, 50.f, 50.f, 50.f }; // 슬라이스 할 꼭짓점 영역 범위 
    float2 fOutSize = { 312.f, 300.f }; // 최종적으로 출력하고 싶은 사이즈
    
    float2 uv = In.vTexcoord;
    float2 edgeScale = EdgeSizes.xy / TextureSize.xy; // 좌상단 영역 크기
    float2 centralScale = float2(1.0, 1.0) - (EdgeSizes.zw + EdgeSizes.xy) / TextureSize.xy; // 중앙 크기

    if (uv.x < edgeScale.x && uv.y < edgeScale.y)
    {
    // 좌상단 고정
        Out.vColor = g_Texture.Sample(LinearSampler_Clamp, uv);
    }
    else if (uv.x > 1.0 - edgeScale.x && uv.y < edgeScale.y)
    {
    // 우상단 고정
        Out.vColor = g_Texture.Sample(LinearSampler_Clamp, uv);
    }
    else if (uv.x < edgeScale.x && uv.y > 1.0 - edgeScale.y)
    {
    // 좌하단 고정
        Out.vColor = g_Texture.Sample(LinearSampler_Clamp, uv);
    }
    else if (uv.x > 1.0 - edgeScale.x && uv.y > 1.0 - edgeScale.y)
    {
    // 우하단 고정
        Out.vColor = g_Texture.Sample(LinearSampler_Clamp, uv);
    }
    else if (uv.y < edgeScale.y)
    {
    // 상단 테두리 (가로로 스케일링)
        float2 localUV = float2((uv.x - edgeScale.x) / centralScale.x, uv.y);
        Out.vColor = g_Texture.Sample(LinearSampler_Clamp, localUV);
    }
    else if (uv.y > 1.0 - edgeScale.y)
    {
    // 하단 테두리 (가로로 스케일링)
        float2 localUV = float2((uv.x - edgeScale.x) / centralScale.x, uv.y);
        Out.vColor = g_Texture.Sample(LinearSampler_Clamp, localUV);
    }
    else if (uv.x < edgeScale.x)
    {
    // 좌측 테두리 (세로로 스케일링)
        float2 localUV = float2(uv.x, (uv.y - edgeScale.y) / centralScale.y);
        Out.vColor = g_Texture.Sample(LinearSampler_Clamp, localUV);
    }
    else if (uv.x > 1.0 - edgeScale.x)
    {
    // 우측 테두리 (세로로 스케일링)
        float2 localUV = float2(uv.x, (uv.y - edgeScale.y) / centralScale.y);
        Out.vColor = g_Texture.Sample(LinearSampler_Clamp, localUV);
    }
    else
    {
    // 중앙 영역 (가로+세로로 스케일링)
        float2 localUV = (uv - edgeScale) / centralScale;
        Out.vColor = g_Texture.Sample(LinearSampler_Clamp, localUV);
    }
    
    return Out;
}


technique11 DefaultTechnique
{
  // 0번
pass DefaultPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

pass Thymesia_UI // 1번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI();
    }

pass Thymesia_UI_ImageOnOff // 2번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_ImageOnOff();
    }

pass Thymesia_UI_ImageOnOffLoop // 3번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_ImageOnOffLoop();
    }

pass Thymesia_UI_Image_ItemSlot // 4번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Image_ItemSlot();
    }
    
pass Thymesia_UI_Image_Pause // 5번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Image_Pause();
    }
    
pass Thymesia_UI_Image_TalentSlot // 6번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Image_TalentSlot3();
    }
    

pass Thymesia_UI_Image_PlunderSlot // 7번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Image_PlunderSlot();
    }
    
pass Thymesia_UI_Image_SkillIcon // 8번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_Image_Skill_CoolTime();
    }

pass Thymesia_UI_Image_Frame // 9번
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Thymasia_UI, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Thymesia_UI_9Slice_Scaling();
    }

}
