#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CUIObject abstract : public CGameObject
{
public:
	struct UIOBJECT_DESC : public CGameObject::GAMEOBJECT_DESC	
	{
		_float			fX, fY,fZ;
		_float			fSizeX, fSizeY;
		_float			fNear, fFar;
		_wstring		strProtoName;
		_uint			iShaderPassNum;
		_int			iTexNumber;
		_uint			iGroupID;
		_float3			fRotation;
		//////////////////////////////////
		_wstring		strFontName;
		_wstring		strContent;

	};

protected:
	CUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIObject(const CUIObject& Prototype);
	virtual ~CUIObject() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	
	virtual _float3 Get_UI_Position() { return m_fPos; }
	virtual _float2 Get_UI_Size() { return m_fSize; }
	virtual _uint Get_UI_Type() { return m_iUIType; }

	virtual _uint Get_UI_ShaderPassNum() { return m_iShaderPassNum; }
	virtual _uint Get_UI_GroupID() { return m_iGroupID; }
	virtual _uint Get_UI_TexNumber() { return m_iTexNumber; }

	virtual _uint Set_UI_ShaderPassNum(_uint iShadernum) { return m_iShaderPassNum = iShadernum; }
	virtual _uint Set_UI_GroupID(_uint iGroupID) { return m_iGroupID = iGroupID; }
	void Set_TexNumber(_int iNumber) { m_iTexNumber = iNumber; }

	virtual _wstring& Get_UI_Name() { return m_strProtoName; }

	virtual void Set_FontName(const _tchar* pFont) { m_strFontName = pFont; }
	virtual void Set_Content(const _tchar*  pText) { m_strContentText = pText; }
	virtual _wstring& Get_Font_Name() { return m_strFontName; }
	virtual _wstring& Get_Content() { return m_strContentText; }


public:
	virtual void Set_Render_OnOff(_bool bOpen) { m_bRenderOpen = bOpen; }
	virtual _bool Get_Render_State() {return  m_bRenderOpen; }
	void Set_OnOff(_bool bOpen) { m_bOpen = bOpen; }
protected:
	_bool m_bOpen = { true }; // 켜져라는 호출 없으면 안 보인다. // 기본이 키는 것 => 코드를 통해 끄자

protected:
	HRESULT Add_UI_Object(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, CGameObject** ppOut, void* pArg);
protected:
	_float4x4			m_ViewMatrix{}, m_ProjMatrix{};	

	_float3				m_fPos = {}; // 저장용
	_float2				m_fSize = {}; // 저장용

	_wstring			m_strFontName;
	_wstring			m_strContentText;


	_wstring			m_strProtoName = {};
	_uint				m_iShaderPassNum = { 0 };		// 쉐이더 패스
	_uint				m_iGroupID = { 0 };		// 그룹아이디
	_uint				m_iUIType = { 0 };		// UI 생성 타입
protected:
	_bool				m_bRenderOpen = { true }; // 기본은 false 이고 특정 이벤트를 통해 on되는 형식
	_uint				m_iTexNumber = {0}; // 텍스처 넘버

	_vector             m_vTextClolor = {1.0f,1.0f,1.0f,1.0f};





public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END