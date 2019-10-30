//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Camera.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include "Camera.h"

namespace ShapeShifter {

CCamera::CCamera()
{
	m_cameraPos.x = 0.0f;
	m_cameraPos.y = 0.0f;
	m_cameraPos.z = 0.0f;

	m_cameraAim.x = 0.0f;
	m_cameraAim.y = 0.0f;
	m_cameraAim.z = 1.0f;

	m_cameraUp.x = 0.0f;
	m_cameraUp.y = 1.0f;
	m_cameraUp.z = 0.0f;
}

CCamera::~CCamera()
{
}

HRESULT CCamera::FinalConstruct()
{
	return put_Name(tag_Camera);
}

STDMETHODIMP CCamera::SetLocation(/* [in] */ VALUE3D x, /* [in] */ VALUE3D y, /* [in] */ VALUE3D z)
{
	m_cameraPos.x = x;
	m_cameraPos.y = y;
	m_cameraPos.z = z;
	return S_OK;
}

STDMETHODIMP CCamera::GetLocation(/* [out] */ VALUE3D* px, /* [out] */ VALUE3D* py, /* [out] */ VALUE3D* pz)
{
	*px = m_cameraPos.x;
	*py = m_cameraPos.y;
	*pz = m_cameraPos.z;
	return S_OK;
}

STDMETHODIMP CCamera::SetAimVector(/* [in] */ VALUE3D x, /* [in] */ VALUE3D y, /* [in] */ VALUE3D z)
{
	m_cameraAim.x = x;
	m_cameraAim.y = y;
	m_cameraAim.z = z;
	D3DXVec3Normalize(&m_cameraAim, &m_cameraAim);
	return S_OK;
}

STDMETHODIMP CCamera::GetAimVector(/* [out] */ VALUE3D* px, /* [out] */ VALUE3D* py, /* [out] */ VALUE3D* pz)
{
	*px = m_cameraAim.x;
	*py = m_cameraAim.y;
	*pz = m_cameraAim.z;
	return S_OK;
}

STDMETHODIMP CCamera::SetUpVector(/* [in] */ VALUE3D x, /* [in] */ VALUE3D y, /* [in] */ VALUE3D z)
{
	m_cameraUp.x = x;
	m_cameraUp.y = y;
	m_cameraUp.z = z;
	D3DXVec3Normalize(&m_cameraUp, &m_cameraUp);
	return S_OK;
}

STDMETHODIMP CCamera::GetUpVector(/* [out] */ VALUE3D* px, /* [out] */ VALUE3D* py, /* [out] */ VALUE3D* pz)
{
	*px = m_cameraUp.x;
	*py = m_cameraUp.y;
	*pz = m_cameraUp.z;
	return S_OK;
}

STDMETHODIMP CCamera::PointAt(/* [in] */ VALUE3D x, /* [in] */ VALUE3D y, /* [in] */ VALUE3D z)
{
	m_cameraAim = D3DXVECTOR3(x,y,z) - m_cameraPos;
	D3DXVec3Normalize(&m_cameraAim, &m_cameraAim);
	return S_OK;
}

STDMETHODIMP CCamera::Move(/* [in] */ enum CameraMovement moveType, /* [in] */ VALUE3D fAmount)
{
	HRESULT hr = E_FAIL;

	switch (moveType)
	{
	case SSCL_CAMERA_UP:
		hr = Translate(0.0f, fAmount, 0.0f);
		break;

	case SSCL_CAMERA_DOWN:
		hr = Translate(0.0f, -fAmount, 0.0f);
		break;

	case SSCL_CAMERA_LEFT:
		hr = Translate(-fAmount, 0.0f, 0.0f);
		break;

	case SSCL_CAMERA_RIGHT:
		hr = Translate(fAmount, 0.0f, 0.0f);
		break;

	case SSCL_CAMERA_FORWARD:
		hr = Translate(0.0f, 0.0f, fAmount);
		break;

	case SSCL_CAMERA_BACKWARD:
		hr = Translate(0.0f, 0.0f, -fAmount);
		break;

	case SSCL_CAMERA_PITCH_UP:
		hr = Rotate(fAmount, 0.0f, 0.0f);
		break;

	case SSCL_CAMERA_PITCH_DOWN:
		hr = Rotate(-fAmount, 0.0f, 0.0f);
		break;

	case SSCL_CAMERA_YAW_LEFT:
		hr = Rotate(0.0f, fAmount, 0.0f);
		break;

	case SSCL_CAMERA_YAW_RIGHT:
		hr = Rotate(0.0f, -fAmount, 0.0f);
		break;

	case SSCL_CAMERA_ROLL_LEFT:
		hr = Rotate(0.0f, 0.0f, -fAmount);
		break;

	case SSCL_CAMERA_ROLL_RIGHT:
		hr = Rotate(0.0f, 0.0f, fAmount);
		break;

	case SSCL_CAMERA_LEVEL_XZ:
		{
			D3DXVECTOR3 xVector;
			D3DXVec3Cross(&xVector, &m_cameraUp, &m_cameraAim);
			D3DXVec3Normalize(&xVector, &xVector);
			m_cameraAim.y = 0.0f;
			xVector.y = 0.0f;
			D3DXVec3Cross(&m_cameraUp, &m_cameraAim, &xVector);
			D3DXVec3Normalize(&m_cameraUp, &m_cameraUp);
			hr = S_OK;
		}
		break;

	case SSCL_CAMERA_GOTO_XAXIS:
		{
			m_cameraPos.x = fAmount;
			m_cameraPos.y = 0.0f;
			m_cameraPos.z = 0.0f;
			m_cameraAim.x = -fAmount;
			m_cameraAim.y = 0.0f;
			m_cameraAim.z = 0.0f;
			D3DXVec3Normalize(&m_cameraAim, &m_cameraAim);
			m_cameraUp.x = 0.0f;
			m_cameraUp.y = 1.0f;
			m_cameraUp.z = 0.0f;
			hr = S_OK;
		}
		break;

	case SSCL_CAMERA_GOTO_YAXIS:
		{
			m_cameraPos.x = 0.0f;
			m_cameraPos.y = fAmount;
			m_cameraPos.z = 0.0f;
			m_cameraAim.x = 0.0f;
			m_cameraAim.y = -fAmount;
			m_cameraAim.z = 0.0f;
			D3DXVec3Normalize(&m_cameraAim, &m_cameraAim);
			m_cameraUp.x = 0.0f;
			m_cameraUp.y = 0.0f;
			m_cameraUp.z = 1.0f;
			hr = S_OK;
		}
		break;

	case SSCL_CAMERA_GOTO_ZAXIS:
		{
			m_cameraPos.x = 0.0f;
			m_cameraPos.y = 0.0f;
			m_cameraPos.z = fAmount;
			m_cameraAim.x = 0.0f;
			m_cameraAim.y = 0.0f;
			m_cameraAim.z = -fAmount;
			D3DXVec3Normalize(&m_cameraAim, &m_cameraAim);
			m_cameraUp.x = 0.0f;
			m_cameraUp.y = 1.0f;
			m_cameraUp.z = 0.0f;
			hr = S_OK;
		}
		break;

	default:
		hr = E_INVALIDARG;
		break;
	}

	return hr;
}

STDMETHODIMP CCamera::Translate(/* [in] */ VALUE3D dx, /* [in] */ VALUE3D dy, /* [in] */ VALUE3D dz)
{
	D3DXVECTOR3 xVector;
	D3DXVec3Cross(&xVector, &m_cameraUp, &m_cameraAim);
	D3DXVec3Normalize(&xVector, &xVector);
	xVector *= dx;

	D3DXVECTOR3 yVector = m_cameraUp;
	yVector *= dy;

	D3DXVECTOR3 zVector = m_cameraAim;
	zVector *= dz;

	D3DXVECTOR3 resVector = xVector + yVector + zVector;
	m_cameraPos += resVector;

	return S_OK;
}

STDMETHODIMP CCamera::Rotate(/* [in] */ VALUE3D fXRadians,
                             /* [in] */ VALUE3D fYRadians,
                             /* [in] */ VALUE3D fZRadians)
{
	// Calculate vector along the camera's local X axis
	D3DXVECTOR3 cameraRight;
	D3DXVec3Cross(&cameraRight, &m_cameraUp, &m_cameraAim);
	D3DXVec3Normalize(&cameraRight, &cameraRight);

	// Setup the rotation matrix
	D3DXMATRIX matRotateRight;
	D3DXMATRIX matRotateUp;
	D3DXMATRIX matRotateAim;
	D3DXMATRIX matRotate;
	D3DXMatrixRotationAxis(&matRotateRight, &cameraRight, -fXRadians);
	D3DXMatrixRotationAxis(&matRotateUp, &m_cameraUp, -fYRadians);
	D3DXMatrixRotationAxis(&matRotateAim, &m_cameraAim, -fZRadians);
	matRotate = matRotateRight * matRotateUp * matRotateAim;

	// Apply the matrix to the aim vector
	D3DXVec3TransformNormal(&m_cameraAim, &m_cameraAim, &matRotate);

	// Apply the matrix to the up vector
	D3DXVec3TransformNormal(&m_cameraUp, &m_cameraUp, &matRotate);

	return S_OK;
}

STDMETHODIMP CCamera::GetCameraState(D3DVECTOR* pCameraPos,
                                     D3DVECTOR* pCameraAim,
                                     D3DVECTOR* pCameraUp)
{
	*pCameraPos = m_cameraPos;
	*pCameraAim = m_cameraAim;
	*pCameraUp = m_cameraUp;
	return S_OK;
}

STDMETHODIMP CCamera::SetCameraState(D3DVECTOR cameraPos,
                                     D3DVECTOR cameraAim,
                                     D3DVECTOR cameraUp)
{
	m_cameraPos = cameraPos;
	m_cameraAim = cameraAim;
	m_cameraUp = cameraUp;
	return S_OK;
}

STDMETHODIMP CCamera::GetViewMatrix(D3DXMATRIX* pViewMatrix)
{
	D3DXVECTOR3 vView, vUp, vRight;

	// vView is the vector down which we are looking/traveling
	// Already normalized
	vView = m_cameraAim;

	// vUp is our camera's Y-Axis
	// Already normalized
	vUp = m_cameraUp;

	// Create the vRight vector and Normalize
	D3DXVec3Cross(&vRight, &vUp, &vView);
	D3DXVec3Normalize(&vRight, &vRight);

	// Start building the matrix. The first three rows contains the basis
	// vectors used to rotate the view to point at the lookat point
	D3DXMatrixIdentity(pViewMatrix);
	pViewMatrix->_11 = vRight.x;    pViewMatrix->_12 = vUp.x;    pViewMatrix->_13 = vView.x;
	pViewMatrix->_21 = vRight.y;    pViewMatrix->_22 = vUp.y;    pViewMatrix->_23 = vView.y;
	pViewMatrix->_31 = vRight.z;    pViewMatrix->_32 = vUp.z;    pViewMatrix->_33 = vView.z;

	// Do the translation values (rotations are still about the eyepoint)
	pViewMatrix->_41 = - D3DXVec3Dot(&m_cameraPos, &vRight);
	pViewMatrix->_42 = - D3DXVec3Dot(&m_cameraPos, &vUp);
	pViewMatrix->_43 = - D3DXVec3Dot(&m_cameraPos, &vView);

	return S_OK;
}

STDMETHODIMP CCamera::GetClassID(CLSID *pClassID)
{
	*pClassID = CLSID_Camera;
	return S_OK;
}

STDMETHODIMP CCamera::LoadXML(/* [in] */ IXMLArchive* pArchive)
{
	pArchive;
	return E_NOTIMPL;
}

STDMETHODIMP CCamera::SaveXML(/* [in] */ IXMLArchive* pArchive)
{
	pArchive;
	return E_NOTIMPL;
}

};  // namespace ShapeShifter

