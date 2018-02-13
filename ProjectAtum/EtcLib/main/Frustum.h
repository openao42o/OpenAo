#ifndef FRUSTUM_H_
#define FRUSTUM_H_

#define COMPLETE_OUT 0
#define INTERSECT    1
#define COMPLETE_IN  2

class CFrustum
{
public:
	CFrustum();
	virtual ~CFrustum();
	BOOL Construct(IDirect3DDevice9* pDevice, float ZDistance = 0.0f);
	BOOL CheckPoint(float XPos, float YPos, float ZPos);
	BOOL CheckCube(float XCenter, float YCenter, float ZCenter, float Size);


	int BoxInFrustum(float x, float y, float z, float sizeX, float sizeY, float sizeZ);

	BOOL CheckRectangle(float XCenter, float YCenter, float ZCenter, float XSize, float YSize, float ZSize);
	bool CheckSphere(const D3DXVECTOR3* vCenter, float Radius);
	bool CheckSphere(float XCenter, float YCenter, float ZCenter, float Radius);

private:
	D3DXPLANE m_Planes[6];
};


#endif
