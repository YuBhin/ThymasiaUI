

sampler LinearSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = wrap;
    AddressV = wrap;
}; 

sampler LinearSampler_Clamp = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = clamp;
    AddressV = clamp;
};
SamplerState PointSampler = sampler_state
{
    Filter = MIN_MAG_MIP_POINT; // 포인트 필터링 (보간 없음)
    AddressU = CLAMP; // 텍스처 경계에서 반복 안함
    AddressV = CLAMP;
};
SamplerState BorderSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = BORDER;
    AddressV = BORDER;
    BorderColor = float4(0, 0, 0, 0);
};

RasterizerState RS_Default
{
    CullMode = BACK;
};

RasterizerState RS_Wireframe
{
    FillMode = WIREFRAME;
};

RasterizerState Rs_Cull_CW
{
    FillMode = SOLID;
    CullMode = FRONT;
};

RasterizerState Rs_Cull_NONE    
{
    FillMode = SOLID;
    CullMode = NONE;    
};


DepthStencilState DSS_Default
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
    DepthFunc = LESS_EQUAL;
};

DepthStencilState DSS_SKip_Z
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};

DepthStencilState DSS_Thymasia_UI
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;

};

DepthStencilState DSS_Thymasia_Test
{
  

};

BlendState BS_Default
{
    BlendEnable[0] = FALSE; /* 여기서 0은 렌더타겟의 넘버 */
};

BlendState BS_AlphaBlend
{
    BlendEnable[0] = TRUE;

    BlendOp = Add;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;

	/*
	SrcColor(rgba), DestColor(rgba)

	SrcColor.rgb * SrcColor.a + DestColor.rgb * (1.f - SrcColor.a)
	*/		
};

BlendState BS_Blend
{
    BlendEnable[0] = TRUE;
    BlendOp[0] = Add;
    SrcBlend[0] = ONE;
    DestBlend[0] = ONE;

	/*
	SrcColor(rgba), DestColor(rgba)

	SrcColor.rgb * 1 + DestColor.rgb * 1


	*/
};
