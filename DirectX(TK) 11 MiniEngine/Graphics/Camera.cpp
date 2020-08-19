#include "Camera.h"

Camera::Camera() : posVec{}, rotVec{}, pos{}, rot{}, viewMatrix{}, projectionMatrix{}
{
    UpdateViewMatrix();
}

void Camera::SetLookAtPos( const XMFLOAT3& lookAtPos )
{
    // Look at position cant be the same as camera position
    if ( pos.x == lookAtPos.x && pos.y == lookAtPos.y && pos.z == lookAtPos.z )
        return;

    XMFLOAT3 diff;
    XMStoreFloat3( &diff, XMVectorSubtract( XMLoadFloat3( &pos ), XMLoadFloat3( &lookAtPos ) ) );

    float pitch = 0.0f;
    if ( diff.y != 0.0f )
    {
        const double distance = sqrt( (double) diff.x * (double) diff.x + (double) diff.z * (double) diff.z );
        pitch = static_cast<float>( atan( diff.y / distance ) );
    }

    float yaw = 0.0f;

    if ( diff.x != 0.0f )
    {
        yaw = static_cast<float>( atan( diff.x / diff.z ) );
    }
    if ( diff.z > 0 )
        yaw += XM_PI;

    SetRotation( pitch, yaw, 0.0f );
}

void Camera::SetProjectionValues( float fovDegrees, float aspectRatio, float nearZ, float farZ )
{
    projectionMatrix = XMMatrixPerspectiveFovLH( XMConvertToRadians( fovDegrees ), aspectRatio, nearZ, farZ );
}

void Camera::SetPosition( const XMVECTOR& pos )
{
    XMStoreFloat3( &this->pos, pos );
    posVec = pos;
    UpdateViewMatrix();
}

void Camera::SetPosition( float x, float y, float z )
{
    pos = XMFLOAT3{ x,y,z };
    posVec = XMLoadFloat3( &pos );
    UpdateViewMatrix();
}

void Camera::AdjustPosition( const XMVECTOR& pos )
{
    posVec += pos;
    XMStoreFloat3( &this->pos, posVec );
    UpdateViewMatrix();
}

void Camera::AdjustPosition( float x, float y, float z )
{
    pos.x += x;
    pos.y += y;
    pos.z += z;
    posVec = XMLoadFloat3( &pos );
    UpdateViewMatrix();
}

void Camera::SetRotation( const XMVECTOR& rot )
{
    XMStoreFloat3( &this->rot, rot );
    rotVec = rot;
    UpdateViewMatrix();
}

void Camera::SetRotation( float x, float y, float z )
{
    rot = XMFLOAT3{ x,y,z };
    rotVec = XMLoadFloat3( &rot );
    UpdateViewMatrix();
}

void Camera::AdjustRotation( const XMVECTOR& rot )
{
    rotVec += rot;
    XMStoreFloat3( &this->rot, rotVec );
    UpdateViewMatrix();
}

void Camera::AdjustRotation( float x, float y, float z )
{
    rot.x += x;
    rot.y += y;
    rot.z += z;
    rotVec = XMLoadFloat3( &rot );
    UpdateViewMatrix();
}

const XMMATRIX& Camera::GetViewMatrix() const
{
    return viewMatrix;
}

const XMMATRIX& Camera::GetProjectionMatrix() const
{
    return projectionMatrix;
}

const XMVECTOR& Camera::GetPositionVector() const
{
    return posVec;
}

const XMFLOAT3& Camera::GetPositionFloat3() const
{
    return pos;
}

const XMVECTOR& Camera::GetRotationVector() const
{
    return rotVec;
}

const XMFLOAT3& Camera::GetRotationFloat3() const
{
    return rot;
}

const XMVECTOR& Camera::GetForwardVector()
{
    return vecForward;
}

const XMVECTOR& Camera::GetRightVector()
{
    return vecRight;
}

const XMVECTOR& Camera::GetBackwardVector()
{
    return vecBackward;
}

const XMVECTOR& Camera::GetLeftVector()
{
    return vecLeft;
}

void Camera::UpdateViewMatrix()
{
    // Calculate camera rotation matrix
    XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw( rot.x, rot.y, rot.z );

    // Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
    XMVECTOR camTarget = XMVector3TransformCoord( DEFAULT_FORWARD_VECTOR, camRotationMatrix );

    // Adjust cam target to be offset by the camera's current position
    camTarget += posVec;

    // Calculate up direction based on current rotation
    XMVECTOR upDir = XMVector3TransformCoord( DEFAULT_UP_VECTOR, camRotationMatrix );

    // Rebuild view matrix
    viewMatrix = XMMatrixLookAtLH( posVec, camTarget, upDir );

    XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw( 0.0f, rot.y, 0.0f );
    vecForward = XMVector3TransformCoord( this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix );
    vecBackward = XMVector3TransformCoord( this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix );
    vecLeft = XMVector3TransformCoord( this->DEFAULT_LEFT_VECTOR, vecRotationMatrix );
    vecRight = XMVector3TransformCoord( this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix );
}
