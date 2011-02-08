#include "DisplayObject.hh"

#include <GU/GU_Detail.h>
#include <GR/GR_UserOption.h>
#include <GEO/GEO_Primitive.h>
#include <GB/GB_AttributeRef.h>
#include <RE/RE_Render.h>
#include <UT/UT_Options.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_XformOrder.h>

class WireCubeObject : public DisplayObject
{
public:

	WireCubeObject() {}
	~WireCubeObject() {}

	void draw(
		GU_Detail *gdp,
		RE_Render &ren,
		GEO_Primitive* prim,
		unsigned nvtx
		);
};


void WireCubeObject::draw(
		GU_Detail *gdp,
		RE_Render &ren,
		GEO_Primitive* prim,
		unsigned nvtx
		)
{
	GB_AttributeRef scaleRef = gdp->findPointAttrib( "scale", 3 * sizeof(float), GB_ATTRIB_FLOAT );
	GB_AttributeRef rotateRef = gdp->findPointAttrib( "rotate", 3 * sizeof(float), GB_ATTRIB_FLOAT );

	bool scaleValid = scaleRef.isValid();
	bool rotateValid = rotateRef.isValid();

	float* posData = new float[nvtx * 3 * 8];

	for (int j=0; j < nvtx; j++)
	{
		GEO_Point* ppt = prim->getVertex(j).getPt();
		UT_Vector4 pos = ppt->getPos();

		UT_Matrix4 transform( 1.0f );

		UT_Vector3 bx( 0.5, 0.0, 0.0 );
		UT_Vector3 by( 0.0, 0.5, 0.0 );
		UT_Vector3 bz( 0.0, 0.0, 0.5 );

		if ( scaleValid )
		{
			UT_Vector3 scaleBuffer;
			const UT_Vector3* scale;
			scale = ppt->getPointer< UT_Vector3 >( scaleRef, &scaleBuffer, 1 );
			transform.scale( scale->x(), scale->y(), scale->z() );
		}

		if ( rotateValid )
		{
			UT_Vector3 rotateBuffer;
			const UT_Vector3* rotate;
			UT_XformOrder xformOrder;
			rotate = ppt->getPointer< UT_Vector3 >( rotateRef, &rotateBuffer, 1 );
			transform.rotate( rotate->x(), rotate->y(), rotate->z(), xformOrder );
		}

		bx = bx * transform;
		by = by * transform;
		bz = bz * transform;

		int offset = j * 3 * 8;
		// Top +z
		posData[ offset + 0 + 0 ] = pos[0] + bx[0] + by[0] + bz[0];
		posData[ offset + 0 + 1 ] = pos[1] + bx[1] + by[1] + bz[1];
		posData[ offset + 0 + 2 ] = pos[2] + bx[2] + by[2] + bz[2];

		posData[ offset + 3 + 0 ] = pos[0] + bx[0] - by[0] + bz[0];
		posData[ offset + 3 + 1 ] = pos[1] + bx[1] - by[1] + bz[1];
		posData[ offset + 3 + 2 ] = pos[2] + bx[2] - by[2] + bz[2];

		posData[ offset + 6 + 0 ] = pos[0] - bx[0] - by[0] + bz[0];
		posData[ offset + 6 + 1 ] = pos[1] - bx[1] - by[1] + bz[1];
		posData[ offset + 6 + 2 ] = pos[2] - bx[2] - by[2] + bz[2];

		posData[ offset + 9 + 0 ] = pos[0] - bx[0] + by[0] + bz[0];
		posData[ offset + 9 + 1 ] = pos[1] - bx[1] + by[1] + bz[1];
		posData[ offset + 9 + 2 ] = pos[2] - bx[2] + by[2] + bz[2];

		// Bottom -z
		posData[ offset + 12 + 0 ] = pos[0] + bx[0] + by[0] - bz[0];
		posData[ offset + 12 + 1 ] = pos[1] + bx[1] + by[1] - bz[1];
		posData[ offset + 12 + 2 ] = pos[2] + bx[2] + by[2] - bz[2];

		posData[ offset + 15 + 0 ] = pos[0] + bx[0] - by[0] - bz[0];
		posData[ offset + 15 + 1 ] = pos[1] + bx[1] - by[1] - bz[1];
		posData[ offset + 15 + 2 ] = pos[2] + bx[2] - by[2] - bz[2];

		posData[ offset + 18 + 0 ] = pos[0] - bx[0] - by[0] - bz[0];
		posData[ offset + 18 + 1 ] = pos[1] - bx[1] - by[1] - bz[1];
		posData[ offset + 18 + 2 ] = pos[2] - bx[2] - by[2] - bz[2];

		posData[ offset + 21 + 0 ] = pos[0] - bx[0] + by[0] - bz[0];
		posData[ offset + 21 + 1 ] = pos[1] - bx[1] + by[1] - bz[1];
		posData[ offset + 21 + 2 ] = pos[2] - bx[2] + by[2] - bz[2];
	}

	/*
				  3_______ 0
				  /|     /|
				 / |    / |
			   2--------1 |
				|  |_ _|__|4
	  z  y      | /7   | /
	  | /       |/     |/
	  |/       6--------5
	  ----x
	*/

	/* Indicies are calculated in full as if we're dispatching this as a Vertex
	   Buffer Object. Currently this isn't needed. We only need one set of indicies */
	int* indices = new int[ nvtx * 12 * 2 ];
	for (int j=0; j < nvtx; j++)
	{
		indices[ j * 24 +  0 ] = 0 * 3;
		indices[ j * 24 +  1 ] = 1 * 3;
		indices[ j * 24 +  2 ] = 1 * 3;
		indices[ j * 24 +  3 ] = 2 * 3;
		indices[ j * 24 +  4 ] = 2 * 3;
		indices[ j * 24 +  5 ] = 3 * 3;
		indices[ j * 24 +  6 ] = 3 * 3;
		indices[ j * 24 +  7 ] = 0 * 3;

		indices[ j * 24 +  8 ] = 4 * 3;
		indices[ j * 24 +  9 ] = 5 * 3;
		indices[ j * 24 + 10 ] = 5 * 3;
		indices[ j * 24 + 11 ] = 6 * 3;
		indices[ j * 24 + 12 ] = 6 * 3;
		indices[ j * 24 + 13 ] = 7 * 3;
		indices[ j * 24 + 14 ] = 7 * 3;
		indices[ j * 24 + 15 ] = 4 * 3;

		indices[ j * 24 + 16 ] = 4 * 3;
		indices[ j * 24 + 17 ] = 0 * 3;

		indices[ j * 24 + 18 ] = 5 * 3;
		indices[ j * 24 + 19 ] = 1 * 3;

		indices[ j * 24 + 20 ] = 6 * 3;
		indices[ j * 24 + 21 ] = 2 * 3;

		indices[ j * 24 + 22 ] = 7 * 3;
		indices[ j * 24 + 23 ] = 3 * 3;
	}

	ren.beginLines();

	for (int j=0; j < nvtx; ++j )
	{
		int offset = j * 3 * 8;

		for ( int i=0; i < 24; ++i )
		{
			ren.vertex3DW(
					posData[ offset + indices[ i ] + 0 ],
					posData[ offset + indices[ i ] + 1 ],
					posData[ offset + indices[ i ] + 2 ]
				);
		}
	}

	ren.endLines();

	delete [] posData;
	delete [] indices;
}


class ShadedCubeObject : public DisplayObject
{
public:

	ShadedCubeObject() {}
	~ShadedCubeObject() {}

	void draw(
		GU_Detail *gdp,
		RE_Render &ren,
		GEO_Primitive* prim,
		unsigned nvtx
		);
};

void ShadedCubeObject::draw(
		GU_Detail *gdp,
		RE_Render &ren,
		GEO_Primitive* prim,
		unsigned nvtx
		)
{
	GB_AttributeRef colourRef = gdp->findPointAttrib( "Cd", 3 * sizeof(float), GB_ATTRIB_FLOAT );
	GB_AttributeRef scaleRef = gdp->findPointAttrib( "scale", 3 * sizeof(float), GB_ATTRIB_FLOAT );
	GB_AttributeRef rotateRef = gdp->findPointAttrib( "rotate", 3 * sizeof(float), GB_ATTRIB_FLOAT );

	bool colourValid = colourRef.isValid();
	bool scaleValid = scaleRef.isValid();
	bool rotateValid = rotateRef.isValid();

	float* posData = new float[nvtx * 3 * 8];
	float* normals = new float[nvtx * 6 * 3];
	float* colours = new float[nvtx * 3];

	for (int j=0; j < nvtx; j++)
	{
		GEO_Point* ppt = prim->getVertex(j).getPt();
		UT_Vector4 pos = ppt->getPos();

		UT_Matrix4 transform( 1.0f );
		UT_Matrix4 rotTransform( 1.0f );

		UT_Vector3 bx( 0.5, 0.0, 0.0 );
		UT_Vector3 by( 0.0, 0.5, 0.0 );
		UT_Vector3 bz( 0.0, 0.0, 0.5 );

		UT_Vector3 nx( 1.0, 0.0, 0.0 );
		UT_Vector3 ny( 0.0, 1.0, 0.0 );
		UT_Vector3 nz( 0.0, 0.0, 1.0 );

		if ( colourValid )
		{
			UT_Vector3 colourBuffer;
			const UT_Vector3* colour;
			colour = ppt->getPointer< UT_Vector3 >( colourRef, &colourBuffer, 1 );
			colours[ j * 3 + 0 ] = colour->x();
			colours[ j * 3 + 1 ] = colour->y();
			colours[ j * 3 + 2 ] = colour->z();
		}

		if ( scaleValid )
		{
			UT_Vector3 scaleBuffer;
			const UT_Vector3* scale;
			scale = ppt->getPointer< UT_Vector3 >( scaleRef, &scaleBuffer, 1 );
			transform.scale( scale->x(), scale->y(), scale->z() );
		}

		if ( rotateValid )
		{
			UT_Vector3 rotateBuffer;
			const UT_Vector3* rotate;
			UT_XformOrder xformOrder;
			rotate = ppt->getPointer< UT_Vector3 >( rotateRef, &rotateBuffer, 1 );
			transform.rotate( rotate->x(), rotate->y(), rotate->z(), xformOrder );
			rotTransform.rotate( rotate->x(), rotate->y(), rotate->z(), xformOrder );
		}

		nx = nx * rotTransform;
		ny = ny * rotTransform;
		nz = nz * rotTransform;

		bx = bx * transform;
		by = by * transform;
		bz = bz * transform;

		int offset = j * 3 * 8;
		// Top +z
		posData[ offset + 0 + 0 ] = pos[0] + bx[0] + by[0] + bz[0];
		posData[ offset + 0 + 1 ] = pos[1] + bx[1] + by[1] + bz[1];
		posData[ offset + 0 + 2 ] = pos[2] + bx[2] + by[2] + bz[2];

		posData[ offset + 3 + 0 ] = pos[0] + bx[0] - by[0] + bz[0];
		posData[ offset + 3 + 1 ] = pos[1] + bx[1] - by[1] + bz[1];
		posData[ offset + 3 + 2 ] = pos[2] + bx[2] - by[2] + bz[2];

		posData[ offset + 6 + 0 ] = pos[0] - bx[0] - by[0] + bz[0];
		posData[ offset + 6 + 1 ] = pos[1] - bx[1] - by[1] + bz[1];
		posData[ offset + 6 + 2 ] = pos[2] - bx[2] - by[2] + bz[2];

		posData[ offset + 9 + 0 ] = pos[0] - bx[0] + by[0] + bz[0];
		posData[ offset + 9 + 1 ] = pos[1] - bx[1] + by[1] + bz[1];
		posData[ offset + 9 + 2 ] = pos[2] - bx[2] + by[2] + bz[2];

		// Bottom -z
		posData[ offset + 12 + 0 ] = pos[0] + bx[0] + by[0] - bz[0];
		posData[ offset + 12 + 1 ] = pos[1] + bx[1] + by[1] - bz[1];
		posData[ offset + 12 + 2 ] = pos[2] + bx[2] + by[2] - bz[2];

		posData[ offset + 15 + 0 ] = pos[0] + bx[0] - by[0] - bz[0];
		posData[ offset + 15 + 1 ] = pos[1] + bx[1] - by[1] - bz[1];
		posData[ offset + 15 + 2 ] = pos[2] + bx[2] - by[2] - bz[2];

		posData[ offset + 18 + 0 ] = pos[0] - bx[0] - by[0] - bz[0];
		posData[ offset + 18 + 1 ] = pos[1] - bx[1] - by[1] - bz[1];
		posData[ offset + 18 + 2 ] = pos[2] - bx[2] - by[2] - bz[2];

		posData[ offset + 21 + 0 ] = pos[0] - bx[0] + by[0] - bz[0];
		posData[ offset + 21 + 1 ] = pos[1] - bx[1] + by[1] - bz[1];
		posData[ offset + 21 + 2 ] = pos[2] - bx[2] + by[2] - bz[2];

		int normOffset = j * 3 * 6;
		normals[ normOffset +  0 + 0 ] = -2 * nz[0];
		normals[ normOffset +  0 + 1 ] = -2 * nz[1];
		normals[ normOffset +  0 + 2 ] = -2 * nz[2];

		normals[ normOffset +  3 + 0 ] = -2 * nz[0];
		normals[ normOffset +  3 + 1 ] = -2 * nz[1];
		normals[ normOffset +  3 + 2 ] = -2 * nz[2];

		normals[ normOffset +  6 + 0 ] = -2 * nx[0];
		normals[ normOffset +  6 + 1 ] = -2 * nx[1];
		normals[ normOffset +  6 + 2 ] = -2 * nx[2];

		normals[ normOffset +  9 + 0 ] = 2 * nx[0];
		normals[ normOffset +  9 + 1 ] = 2 * nx[1];
		normals[ normOffset +  9 + 2 ] = 2 * nx[2];

		normals[ normOffset + 12 + 0 ] = 2 * ny[0];
		normals[ normOffset + 12 + 1 ] = 2 * ny[1];
		normals[ normOffset + 12 + 2 ] = 2 * ny[2];

		normals[ normOffset + 15 + 0 ] = -2 * ny[0];
		normals[ normOffset + 15 + 1 ] = -2 * ny[1];
		normals[ normOffset + 15 + 2 ] = -2 * ny[2];
	}

	/*
				  3_______ 0
				  /|     /|
				 / |    / |
			   2--------1 |
				|  |_ _|__|4
	  z  y      | /7   | /
	  | /       |/     |/
	  |/       6--------5
	  ----x
	*/

	/* Indicies are calculated in full as if we're dispatching this as a Vertex
	   Buffer Object. Currently this isn't needed. We only need one set of indicies */
	int* indices = new int[ nvtx * 6 * 4 ];
	for (int j=0; j < nvtx; j++)
	{
		indices[ j * 24 +  0 ] = 0 * 3;
		indices[ j * 24 +  1 ] = 1 * 3;
		indices[ j * 24 +  2 ] = 2 * 3;
		indices[ j * 24 +  3 ] = 3 * 3;
		indices[ j * 24 +  4 ] = 4 * 3;
		indices[ j * 24 +  5 ] = 5 * 3;
		indices[ j * 24 +  6 ] = 6 * 3;
		indices[ j * 24 +  7 ] = 7 * 3;
		indices[ j * 24 +  8 ] = 4 * 3;
		indices[ j * 24 +  9 ] = 5 * 3;
		indices[ j * 24 + 10 ] = 1 * 3;
		indices[ j * 24 + 11 ] = 0 * 3;
		indices[ j * 24 + 12 ] = 6 * 3;
		indices[ j * 24 + 13 ] = 7 * 3;
		indices[ j * 24 + 14 ] = 3 * 3;
		indices[ j * 24 + 15 ] = 2 * 3;
		indices[ j * 24 + 16 ] = 5 * 3;
		indices[ j * 24 + 17 ] = 6 * 3;
		indices[ j * 24 + 18 ] = 2 * 3;
		indices[ j * 24 + 19 ] = 1 * 3;
		indices[ j * 24 + 20 ] = 7 * 3;
		indices[ j * 24 + 21 ] = 4 * 3;
		indices[ j * 24 + 22 ] = 0 * 3;
		indices[ j * 24 + 23 ] = 3 * 3;
	}

	ren.beginQuads();

	for (int j=0; j < nvtx; ++j )
	{
		int offset = j * 3 * 8;
		int normOffset = j * 3 * 6;

		if ( colourValid )
		{
			ren.setColor(
					colours[ j * 3 + 0 ],
					colours[ j * 3 + 1 ],
					colours[ j * 3 + 2 ],
					1.0 );
		}

		for ( int i=0; i < 6; ++i )
		{
			ren.n3DW( normals + normOffset + i * 3 );

			for ( int k=0; k<4; ++k )
			{
				ren.vertex3DW(
						posData[ offset + indices[ i * 4 + k ] + 0 ],
						posData[ offset + indices[ i * 4 + k ] + 1 ],
						posData[ offset + indices[ i * 4 + k ] + 2 ]
					);
			}
		}
	}

	ren.endQuads();

	delete [] posData;
	delete [] indices;
	delete [] normals;
	if ( colourValid ) delete [] colours;
}


class CameraFacingObject : public DisplayObject
{
public:

	CameraFacingObject() {}

	void setup( RE_Render& ren );

protected:

	UT_Vector3 m_cameraPosition;
	UT_Vector3 m_up;

};

void CameraFacingObject::setup( RE_Render& ren )
{
 	UT_Matrix4 camMatrix;
 	ren.getMatrix( camMatrix );
 	camMatrix.invert();
 
 	camMatrix.getTranslates( m_cameraPosition );
 	m_up = UT_Vector3( camMatrix.myFloats[4], camMatrix.myFloats[5], camMatrix.myFloats[6] );
}


class WireDiscObject : public CameraFacingObject
{
public:

	WireDiscObject() {}
	~WireDiscObject() {}

	void draw(
		GU_Detail *gdp,
		RE_Render &ren,
		GEO_Primitive* prim,
		unsigned nvtx
		);
};


void WireDiscObject::draw(
		GU_Detail *gdp,
		RE_Render &ren,
		GEO_Primitive* prim,
		unsigned nvtx
		)
{
	GB_AttributeRef scaleRef = gdp->findPointAttrib( "scale", 3 * sizeof(float), GB_ATTRIB_FLOAT );
	GB_AttributeRef rotateRef = gdp->findPointAttrib( "rotate", 3 * sizeof(float), GB_ATTRIB_FLOAT );

	bool scaleValid = scaleRef.isValid();
	bool rotateValid = rotateRef.isValid();

	float* posData = new float[nvtx * 3 * 11];

	for (int j=0; j < nvtx; j++)
	{
		GEO_Point* ppt = prim->getVertex(j).getPt();
		UT_Vector4 pos = ppt->getPos();

		UT_Matrix4 transform( 1.0f );

		UT_Vector3 bx( 0.5f, 0.0f, 0.0f );
		UT_Vector3 by( 0.0f, 0.5f, 0.0f );

		if ( rotateValid )
		{
			if ( scaleValid )
			{
				UT_Vector3 scaleBuffer;
				const UT_Vector3* scale;
				scale = ppt->getPointer< UT_Vector3 >( scaleRef, &scaleBuffer, 1 );
				bx[0] *= scale->x();
				by[1] *= scale->y();
			}

			UT_Vector3 rotateBuffer;
			const UT_Vector3* rotate;
			UT_XformOrder xformOrder;
			rotate = ppt->getPointer< UT_Vector3 >( rotateRef, &rotateBuffer, 1 );
			transform.rotate( rotate->x(), rotate->y(), rotate->z(), xformOrder );

			bx = bx * transform;
			by = by * transform;
		}
		else
		{
			UT_Vector3 pointToCam = m_cameraPosition - pos;

			UT_Vector3 localUp = m_up;
			localUp.cross( pointToCam );

			UT_Vector3 left = localUp;
			left.cross( pointToCam );
			localUp.normalize();
			left.normalize();

			bx = left;
			by = localUp;

			if ( scaleValid )
			{
				UT_Vector3 scaleBuffer;
				const UT_Vector3* scale;
				scale = ppt->getPointer< UT_Vector3 >( scaleRef, &scaleBuffer, 1 );
				bx *= scale->x();
				by *= scale->y();
			}
		}

		// bx = bx * transform;
		// by = by * transform;

		int offset = j * 3 * 10;

		// sin & cos of 0.0/10.0 * 2 * pi
		posData[ offset + 0 + 0 ] = pos[0] + 0.0f * bx[0] + 1.0f * by[0];
		posData[ offset + 0 + 1 ] = pos[1] + 0.0f * bx[1] + 1.0f * by[1];
		posData[ offset + 0 + 2 ] = pos[2] + 0.0f * bx[2] + 1.0f * by[2];

		// sin & cos of 1.0/10.0 * 2 * pi
		posData[ offset + 3 + 0 ] = pos[0] + 0.587785252292f * bx[0] + 0.809016994375f * by[0];
		posData[ offset + 3 + 1 ] = pos[1] + 0.587785252292f * bx[1] + 0.809016994375f * by[1];
		posData[ offset + 3 + 2 ] = pos[2] + 0.587785252292f * bx[2] + 0.809016994375f * by[2];

		// sin & cos of 2.0/10.0 * 2 * pi
		posData[ offset + 6 + 0 ] = pos[0] + 0.951056516295f * bx[0] + 0.309016994375f * by[0];
		posData[ offset + 6 + 1 ] = pos[1] + 0.951056516295f * bx[1] + 0.309016994375f * by[1];
		posData[ offset + 6 + 2 ] = pos[2] + 0.951056516295f * bx[2] + 0.309016994375f * by[2];

		// sin & cos of 3.0/10.0 * 2 * pi
		posData[ offset + 9 + 0 ] = pos[0] + 0.951056516295f * bx[0] - 0.309016994375f * by[0];
		posData[ offset + 9 + 1 ] = pos[1] + 0.951056516295f * bx[1] - 0.309016994375f * by[1];
		posData[ offset + 9 + 2 ] = pos[2] + 0.951056516295f * bx[2] - 0.309016994375f * by[2];

		// sin & cos of 4.0/10.0 * 2 * pi
		posData[ offset + 12 + 0 ] = pos[0] + 0.587785252292f * bx[0] - 0.809016994375f * by[0];
		posData[ offset + 12 + 1 ] = pos[1] + 0.587785252292f * bx[1] - 0.809016994375f * by[1];
		posData[ offset + 12 + 2 ] = pos[2] + 0.587785252292f * bx[2] - 0.809016994375f * by[2];

		// sin & cos of 5.0/10.0 * 2 * pi
		posData[ offset + 15 + 0 ] = pos[0] + 0.0f * bx[0] - 1.0f * by[0];
		posData[ offset + 15 + 1 ] = pos[1] + 0.0f * bx[1] - 1.0f * by[1];
		posData[ offset + 15 + 2 ] = pos[2] + 0.0f * bx[2] - 1.0f * by[2];

		// sin & cos of 6.0/10.0 * 2 * pi
		posData[ offset + 18 + 0 ] = pos[0] - 0.587785252292f * bx[0] - 0.809016994375f * by[0];
		posData[ offset + 18 + 1 ] = pos[1] - 0.587785252292f * bx[1] - 0.809016994375f * by[1];
		posData[ offset + 18 + 2 ] = pos[2] - 0.587785252292f * bx[2] - 0.809016994375f * by[2];

		// sin & cos of 7.0/10.0 * 2 * pi
		posData[ offset + 21 + 0 ] = pos[0] - 0.951056516295f * bx[0] - 0.309016994375f * by[0];
		posData[ offset + 21 + 1 ] = pos[1] - 0.951056516295f * bx[1] - 0.309016994375f * by[1];
		posData[ offset + 21 + 2 ] = pos[2] - 0.951056516295f * bx[2] - 0.309016994375f * by[2];

		// sin & cos of 8.0/10.0 * 2 * pi
		posData[ offset + 24 + 0 ] = pos[0] - 0.951056516295f * bx[0] + 0.309016994375f * by[0];
		posData[ offset + 24 + 1 ] = pos[1] - 0.951056516295f * bx[1] + 0.309016994375f * by[1];
		posData[ offset + 24 + 2 ] = pos[2] - 0.951056516295f * bx[2] + 0.309016994375f * by[2];

		// sin & cos of 9.0/10.0 * 2 * pi
		posData[ offset + 27 + 0 ] = pos[0] - 0.587785252292f * bx[0] + 0.809016994375f * by[0];
		posData[ offset + 27 + 1 ] = pos[1] - 0.587785252292f * bx[1] + 0.809016994375f * by[1];
		posData[ offset + 27 + 2 ] = pos[2] - 0.587785252292f * bx[2] + 0.809016994375f * by[2];
	}

	int* indices = new int[nvtx * 20];

	/* Indicies are calculated in full as if we're dispatching this as a Vertex
	   Buffer Object. Currently this isn't needed. We only need one set of indicies */
	for (int j=0; j < nvtx; ++j )
	{
		int offset = j * 20;

		indices[ offset +  0 ] =  0 * 3;
		indices[ offset +  1 ] =  1 * 3;
		indices[ offset +  2 ] =  1 * 3;
		indices[ offset +  3 ] =  2 * 3;
		indices[ offset +  4 ] =  2 * 3;
		indices[ offset +  5 ] =  3 * 3;
		indices[ offset +  6 ] =  3 * 3;
		indices[ offset +  7 ] =  4 * 3;
		indices[ offset +  8 ] =  4 * 3;
		indices[ offset +  9 ] =  5 * 3;
		indices[ offset + 10 ] =  5 * 3;
		indices[ offset + 11 ] =  6 * 3;
		indices[ offset + 12 ] =  6 * 3;
		indices[ offset + 13 ] =  7 * 3;
		indices[ offset + 14 ] =  7 * 3;
		indices[ offset + 15 ] =  8 * 3;
		indices[ offset + 16 ] =  8 * 3;
		indices[ offset + 17 ] =  9 * 3;
		indices[ offset + 18 ] =  9 * 3;
		indices[ offset + 19 ] =  0 * 3;
	}

	ren.beginLines();

	for (int j=0; j < nvtx; ++j )
	{
		int offset = j * 3 * 10;

		for ( int i=0; i < 20; ++i )
		{
			ren.vertex3DW(
					posData[ offset + indices[ i ] + 0 ],
					posData[ offset + indices[ i ] + 1 ],
					posData[ offset + indices[ i ] + 2 ]
				);
		}
	}

	ren.endLines();

	delete [] posData;
	delete [] indices;
}

class ShadedDiscObject : public CameraFacingObject
{
public:

	ShadedDiscObject() {}
	~ShadedDiscObject() {}

	void draw(
		GU_Detail *gdp,
		RE_Render &ren,
		GEO_Primitive* prim,
		unsigned nvtx
		);
};

void ShadedDiscObject::draw(
		GU_Detail *gdp,
		RE_Render &ren,
		GEO_Primitive* prim,
		unsigned nvtx
		)
{
	GB_AttributeRef colourRef = gdp->findPointAttrib( "Cd", 3 * sizeof(float), GB_ATTRIB_FLOAT );
	GB_AttributeRef scaleRef = gdp->findPointAttrib( "scale", 3 * sizeof(float), GB_ATTRIB_FLOAT );
	GB_AttributeRef rotateRef = gdp->findPointAttrib( "rotate", 3 * sizeof(float), GB_ATTRIB_FLOAT );

	bool colourValid = colourRef.isValid();
	bool scaleValid = scaleRef.isValid();
	bool rotateValid = rotateRef.isValid();

	float* posData = new float[nvtx * 3 * 11];
	float* normals = new float[nvtx * 3];
	float* colours = new float[nvtx * 3];

	for (int j=0; j < nvtx; j++)
	{
		GEO_Point* ppt = prim->getVertex(j).getPt();
		UT_Vector4 pos = ppt->getPos();

		UT_Matrix4 transform( 1.0f );
		UT_Matrix4 rotTransform( 1.0f );

		if ( colourValid )
		{
			UT_Vector3 colourBuffer;
			const UT_Vector3* colour;
			colour = ppt->getPointer< UT_Vector3 >( colourRef, &colourBuffer, 1 );
			colours[ j * 3 + 0 ] = colour->x();
			colours[ j * 3 + 1 ] = colour->y();
			colours[ j * 3 + 2 ] = colour->z();
		}

		UT_Vector3 bx( 0.5f, 0.0f, 0.0f );
		UT_Vector3 by( 0.0f, 0.5f, 0.0f );

		UT_Vector3 n( 0.0f, 0.0f, 1.0f );

		if ( rotateValid )
		{
			if ( scaleValid )
			{
				UT_Vector3 scaleBuffer;
				const UT_Vector3* scale;
				scale = ppt->getPointer< UT_Vector3 >( scaleRef, &scaleBuffer, 1 );
				bx[0] *= scale->x();
				by[1] *= scale->y();
			}

			UT_Vector3 rotateBuffer;
			const UT_Vector3* rotate;
			UT_XformOrder xformOrder;
			rotate = ppt->getPointer< UT_Vector3 >( rotateRef, &rotateBuffer, 1 );
			transform.rotate( rotate->x(), rotate->y(), rotate->z(), xformOrder );
			rotTransform.rotate( rotate->x(), rotate->y(), rotate->z(), xformOrder );

			bx = bx * transform;
			by = by * transform;

			n = n * rotTransform;
		}
		else
		{
			UT_Vector3 pointToCam = m_cameraPosition - pos;

			UT_Vector3 localUp = m_up;
			localUp.cross( pointToCam );

			UT_Vector3 left = localUp;
			left.cross( pointToCam );
			localUp.normalize();
			left.normalize();

			bx = left;
			by = localUp;
			n = pointToCam;
			n.normalize();

			if ( scaleValid )
			{
				UT_Vector3 scaleBuffer;
				const UT_Vector3* scale;
				scale = ppt->getPointer< UT_Vector3 >( scaleRef, &scaleBuffer, 1 );
				bx *= scale->x();
				by *= scale->y();
			}
		}

		int offset = j * 3 * 11;

		// Centre
		posData[ offset + 0 + 0 ] = pos[0];
		posData[ offset + 0 + 1 ] = pos[1];
		posData[ offset + 0 + 2 ] = pos[2];

		// sin & cos of 0.0/10.0 * 2 * pi
		posData[ offset + 3 + 0 ] = pos[0] + 0.0f * bx[0] + 1.0f * by[0];
		posData[ offset + 3 + 1 ] = pos[1] + 0.0f * bx[1] + 1.0f * by[1];
		posData[ offset + 3 + 2 ] = pos[2] + 0.0f * bx[2] + 1.0f * by[2];

		// sin & cos of 1.0/10.0 * 2 * pi
		posData[ offset + 6 + 0 ] = pos[0] + 0.587785252292f * bx[0] + 0.809016994375f * by[0];
		posData[ offset + 6 + 1 ] = pos[1] + 0.587785252292f * bx[1] + 0.809016994375f * by[1];
		posData[ offset + 6 + 2 ] = pos[2] + 0.587785252292f * bx[2] + 0.809016994375f * by[2];

		// sin & cos of 2.0/10.0 * 2 * pi
		posData[ offset + 9 + 0 ] = pos[0] + 0.951056516295f * bx[0] + 0.309016994375f * by[0];
		posData[ offset + 9 + 1 ] = pos[1] + 0.951056516295f * bx[1] + 0.309016994375f * by[1];
		posData[ offset + 9 + 2 ] = pos[2] + 0.951056516295f * bx[2] + 0.309016994375f * by[2];

		// sin & cos of 3.0/10.0 * 2 * pi
		posData[ offset + 12 + 0 ] = pos[0] + 0.951056516295f * bx[0] - 0.309016994375f * by[0];
		posData[ offset + 12 + 1 ] = pos[1] + 0.951056516295f * bx[1] - 0.309016994375f * by[1];
		posData[ offset + 12 + 2 ] = pos[2] + 0.951056516295f * bx[2] - 0.309016994375f * by[2];

		// sin & cos of 4.0/10.0 * 2 * pi
		posData[ offset + 15 + 0 ] = pos[0] + 0.587785252292f * bx[0] - 0.809016994375f * by[0];
		posData[ offset + 15 + 1 ] = pos[1] + 0.587785252292f * bx[1] - 0.809016994375f * by[1];
		posData[ offset + 15 + 2 ] = pos[2] + 0.587785252292f * bx[2] - 0.809016994375f * by[2];

		// sin & cos of 5.0/10.0 * 2 * pi
		posData[ offset + 18 + 0 ] = pos[0] + 0.0f * bx[0] - 1.0f * by[0];
		posData[ offset + 18 + 1 ] = pos[1] + 0.0f * bx[1] - 1.0f * by[1];
		posData[ offset + 18 + 2 ] = pos[2] + 0.0f * bx[2] - 1.0f * by[2];

		// sin & cos of 6.0/10.0 * 2 * pi
		posData[ offset + 21 + 0 ] = pos[0] - 0.587785252292f * bx[0] - 0.809016994375f * by[0];
		posData[ offset + 21 + 1 ] = pos[1] - 0.587785252292f * bx[1] - 0.809016994375f * by[1];
		posData[ offset + 21 + 2 ] = pos[2] - 0.587785252292f * bx[2] - 0.809016994375f * by[2];

		// sin & cos of 7.0/10.0 * 2 * pi
		posData[ offset + 24 + 0 ] = pos[0] - 0.951056516295f * bx[0] - 0.309016994375f * by[0];
		posData[ offset + 24 + 1 ] = pos[1] - 0.951056516295f * bx[1] - 0.309016994375f * by[1];
		posData[ offset + 24 + 2 ] = pos[2] - 0.951056516295f * bx[2] - 0.309016994375f * by[2];

		// sin & cos of 8.0/10.0 * 2 * pi
		posData[ offset + 27 + 0 ] = pos[0] - 0.951056516295f * bx[0] + 0.309016994375f * by[0];
		posData[ offset + 27 + 1 ] = pos[1] - 0.951056516295f * bx[1] + 0.309016994375f * by[1];
		posData[ offset + 27 + 2 ] = pos[2] - 0.951056516295f * bx[2] + 0.309016994375f * by[2];

		// sin & cos of 9.0/10.0 * 2 * pi
		posData[ offset + 30 + 0 ] = pos[0] - 0.587785252292f * bx[0] + 0.809016994375f * by[0];
		posData[ offset + 30 + 1 ] = pos[1] - 0.587785252292f * bx[1] + 0.809016994375f * by[1];
		posData[ offset + 30 + 2 ] = pos[2] - 0.587785252292f * bx[2] + 0.809016994375f * by[2];

		int normOffset = j * 3;
		normals[ normOffset + 0 ] = - n[0];
		normals[ normOffset + 1 ] = - n[1];
		normals[ normOffset + 2 ] = - n[2];
	}

	int* indices = new int[nvtx * 30];

	/* Indicies are calculated in full as if we're dispatching this as a Vertex
	   Buffer Object. Currently this isn't needed. We only need one set of indicies */
	for (int j=0; j < nvtx; ++j )
	{
		int offset = j * 30;

		indices[ offset +  0 ] =  0 * 3;
		indices[ offset +  1 ] =  1 * 3;
		indices[ offset +  2 ] =  2 * 3;
		indices[ offset +  3 ] =  0 * 3;
		indices[ offset +  4 ] =  2 * 3;
		indices[ offset +  5 ] =  3 * 3;
		indices[ offset +  6 ] =  0 * 3;
		indices[ offset +  7 ] =  3 * 3;
		indices[ offset +  8 ] =  4 * 3;
		indices[ offset +  9 ] =  0 * 3;
		indices[ offset + 10 ] =  4 * 3;
		indices[ offset + 11 ] =  5 * 3;
		indices[ offset + 12 ] =  0 * 3;
		indices[ offset + 13 ] =  5 * 3;
		indices[ offset + 14 ] =  6 * 3;
		indices[ offset + 15 ] =  0 * 3;
		indices[ offset + 16 ] =  6 * 3;
		indices[ offset + 17 ] =  7 * 3;
		indices[ offset + 18 ] =  0 * 3;
		indices[ offset + 19 ] =  7 * 3;
		indices[ offset + 20 ] =  8 * 3;
		indices[ offset + 21 ] =  0 * 3;
		indices[ offset + 22 ] =  8 * 3;
		indices[ offset + 23 ] =  9 * 3;
		indices[ offset + 24 ] =  0 * 3;
		indices[ offset + 25 ] =  9 * 3;
		indices[ offset + 26 ] = 10 * 3;
		indices[ offset + 27 ] =  0 * 3;
		indices[ offset + 28 ] = 10 * 3;
		indices[ offset + 29 ] =  1 * 3;
	}


	ren.beginTriangles();

	for (int j=0; j < nvtx; ++j )
	{

		if ( colourValid )
		{
			ren.setColor(
					colours[ j * 3 + 0 ],
					colours[ j * 3 + 1 ],
					colours[ j * 3 + 2 ],
					1.0 );
		}

		ren.n3DW( normals + j * 3 );

		int offset = j * 3 * 11;

		for ( int i=0; i < 30; ++i )
		{
			ren.vertex3DW(
					posData[ offset + indices[ i ] + 0 ],
					posData[ offset + indices[ i ] + 1 ],
					posData[ offset + indices[ i ] + 2 ]
				);
		}
	}

	ren.endTriangles();

	delete [] posData;
	delete [] normals;
	delete [] indices;
	if ( colourValid ) delete [] colours;
}


DisplayObject* DisplayObjectFactory::createCube( bool wireframe )
{
	if ( wireframe )
		return new WireCubeObject();

	return new ShadedCubeObject();
}
	
DisplayObject* DisplayObjectFactory::createDisc( bool wireframe )
{
	if ( wireframe )
		return new WireDiscObject();

	return new ShadedDiscObject();
}

