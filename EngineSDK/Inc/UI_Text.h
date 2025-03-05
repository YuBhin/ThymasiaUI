#pragma once
#include "UIObject.h"

BEGIN(Engine)
class ENGINE_DLL CUI_Text abstract : public CUIObject
{

public:
	enum TextRender
	{
		FONT_DEFALUT, FONT_SHADOW, FONT_OUTLINE, FONT_END
};


protected:
	CUI_Text(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	CUI_Text(const CUI_Text& Prototype);
	virtual ~CUI_Text() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;
	
	void Set_Change_TextColor(_float4 fColor) { m_fTextColor = { fColor.x / 255.f, fColor.y / 255.f,fColor.z / 255.f,fColor.w / 255.f }; }
	void Set_TextRenderType(TextRender eType) { m_eRenderType = eType; }

//public:
//	void Set_FontName(const _tchar* pFont) { strUseFontName = pFont;}
//	void Set_ContentText(const _tchar* pConstent){ strUseText = pConstent; }
//
//	_wstring Get_FontName(){ return strUseFontName; }
//	_wstring Get_ContentText(){ return strUseText; }
	
protected:
	_float4		m_fTextColor = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f };
	TextRender	m_eRenderType = { FONT_DEFALUT };

	/*
	_wstring strUseFontName = { TEXT("Font_NotoSansKR16") };
	_wstring strUseText = { TEXT("기본 입력 상태입니다") };*/

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END