#include "VIBuffer_SliceNine.h"

CVIBuffer_SliceNine::CVIBuffer_SliceNine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{

}

CVIBuffer_SliceNine::CVIBuffer_SliceNine(const CVIBuffer_SliceNine& Prototype)
	: CVIBuffer{ Prototype }
{

}

HRESULT CVIBuffer_SliceNine::Initialize_Prototype()
{
    m_iNumVertices = 16;
    m_iVertexStride = sizeof(VTXPOSTEX);
    m_iNumIndices = 54;
    m_iIndexStride = 2;
    m_iNumVertexBuffers = 1;
    m_eIndexFormat = DXGI_FORMAT_R16_UINT;
    m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    // ����� ���� UI ������ (�ȼ� ����)
    const _float fDestWidth = 300.f;
    const _float fDestHeight = 300.f;

    // �ؽ�ó ���� ������
    const _float fTexWidth = 156.f;
    const _float fTexHeight = 150.f;

    // ���� �ȼ� ũ��
    const _float fEdgeLeft = 24.f;
    const _float fEdgeRight = 24.f;
    const _float fEdgeTop = 24.f;
    const _float fEdgeBottom = 24.f;

    // ȭ�� ���� ��ǥ ��� (ȭ�� �������� ������ ����� ��ǥ)
    _float fX[4] = {
        0.f,
        fEdgeLeft,
        fDestWidth - fEdgeRight,
        fDestWidth
    };

    _float fY[4] = {
        0.f,
        fEdgeBottom,
        fDestHeight - fEdgeTop,
        fDestHeight
    };

    // UV ��ǥ (����ȭ�� �ؽ�ó ��ǥ)
    _float fU[4] = {
        0.f,
        fEdgeLeft / fTexWidth,
        (fTexWidth - fEdgeRight) / fTexWidth,
        1.f
    };

    _float fV[4] = {
        1.f,
        (fTexHeight - fEdgeBottom) / fTexHeight,
        fEdgeTop / fTexHeight,
        0.f
    };

#pragma region VERTEX_BUFFER
    ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
    m_BufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
    m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
    m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    m_BufferDesc.StructureByteStride = m_iVertexStride;
    m_BufferDesc.CPUAccessFlags = 0;
    m_BufferDesc.MiscFlags = 0;

    VTXPOSTEX* pVertices = new VTXPOSTEX[m_iNumVertices];
    ZeroMemory(pVertices, sizeof(VTXPOSTEX) * m_iNumVertices);

    int iVert = 0;
    for (int j = 0; j < 4; ++j)
    {
        for (int i = 0; i < 4; ++i)
        {
            // �߽� ���ĵ� ȭ�� ��ǥ (-0.5 ~ +0.5 �������� �ű�� ������ ���� ����)
            float posX = fX[i] - fDestWidth * 0.5f;
            float posY = (fDestHeight - fY[j]) - fDestHeight * 0.5f; // Y ������ (DirectX�� +Y�� �Ʒ�)

            pVertices[iVert].vPosition = _float3(posX, posY, 0.f);
            pVertices[iVert].vTexcoord = _float2(fU[i], fV[j]);
            ++iVert;
        }
    }

    ZeroMemory(&m_InitialData, sizeof m_InitialData);
    m_InitialData.pSysMem = pVertices;

    if (FAILED(__super::Create_Buffer(&m_pVB)))
        return E_FAIL;

    Safe_Delete_Array(pVertices);
#pragma endregion

#pragma region INDEX_BUFFER
    ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
    m_BufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
    m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
    m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    m_BufferDesc.StructureByteStride = m_iIndexStride;
    m_BufferDesc.CPUAccessFlags = 0;
    m_BufferDesc.MiscFlags = 0;

    _ushort* pIndices = new _ushort[m_iNumIndices];
    ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

    int iIndex = 0;
    for (int y = 0; y < 3; ++y)
    {
        for (int x = 0; x < 3; ++x)
        {
            int v0 = (y * 4) + x;
            int v1 = v0 + 1;
            int v2 = v0 + 4;
            int v3 = v2 + 1;

            // �ð� ���� �ε��� ����
            pIndices[iIndex++] = v0;
            pIndices[iIndex++] = v2;
            pIndices[iIndex++] = v1;

            pIndices[iIndex++] = v1;
            pIndices[iIndex++] = v2;
            pIndices[iIndex++] = v3;
        }
    }

    ZeroMemory(&m_InitialData, sizeof m_InitialData);
    m_InitialData.pSysMem = pIndices;

    if (FAILED(__super::Create_Buffer(&m_pIB)))
        return E_FAIL;

    Safe_Delete_Array(pIndices);
#pragma endregion

    return S_OK;
}

HRESULT CVIBuffer_SliceNine::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_SliceNine* CVIBuffer_SliceNine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_SliceNine* pInstance = new CVIBuffer_SliceNine(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CVIBuffer_SliceNine");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent* CVIBuffer_SliceNine::Clone(void* pArg)
{
	CVIBuffer_SliceNine* pInstance = new CVIBuffer_SliceNine(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CVIBuffer_SliceNine");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CVIBuffer_SliceNine::Free()
{
	__super::Free();

}
