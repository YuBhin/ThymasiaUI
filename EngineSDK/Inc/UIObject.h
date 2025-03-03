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
		_wstring		szProtoName;
		_uint			iShaderPassNum;
		_int			iTexNumber;
		_uint			iGroupID;
		_float3			fRotation;
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
	virtual _bool Mouse_Select(HWND hWnd, _float fX, _float fY);
	virtual _wstring& Get_UI_Name() { return m_szProtoName; }
	
	virtual _float3 Get_UI_Position() { return m_fPos; }
	virtual _float2 Get_UI_Size() { return m_fSize; }
	virtual _uint Get_UI_Type() { return m_iUIType; }

	virtual _uint Get_UI_ShaderPassNum() { return m_iShaderPassNum; }
	virtual _uint Get_UI_GroupID() { return m_iGroupID; }
	virtual _uint Get_UI_TexNumber() { return m_iTexNumber; }

	virtual _uint Set_UI_ShaderPassNum(_uint iShadernum) { return m_iShaderPassNum = iShadernum; }
	virtual _uint Set_UI_GroupID(_uint iGroupID) { return m_iGroupID = iGroupID; }
	void Set_TexNumber(_int iNumber) { m_iTexNumber = iNumber; }

public:
	virtual void Set_Render_OnOff(_bool bOpen) { m_bRenderOpen = bOpen; }
	virtual _bool Get_Render_State() {return  m_bRenderOpen; }
	virtual void Check_Render_Text();
protected:
	HRESULT Add_UI_Object(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, CGameObject** ppOut, void* pArg);
protected:
	_float4x4			m_ViewMatrix{}, m_ProjMatrix{};

	_float3				m_fPos = {}; // 저장용
	_float2				m_fSize = {}; // 저장용
	_wstring			m_szProtoName = {};
	_uint				m_iShaderPassNum = { 0 };		// 쉐이더 패스
	_uint				m_iGroupID = { 0 };		// 그룹아이디
	_uint				m_iUIType = { 0 };		// UI 생성 타입
protected:
	_bool				m_bRenderOpen = { true }; // 기본은 false 이고 특정 이벤트를 통해 on되는 형식
	_uint				m_iTexNumber = {0}; // 텍스처 넘버

	_tchar*				m_pUITextFont = { TEXT("Font_Gulim_Default") }; // UI 위에 텍스트가 있는 경우
	_tchar*				m_pUITextContent = {}; // UI 위에 텍스트가 있는 경우
	_vector             m_vTextClolor = {1.0f,1.0f,1.0f,1.0f};





public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END