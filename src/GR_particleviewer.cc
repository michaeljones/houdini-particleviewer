#include "GR_particleviewer.hh"

#include <GR/GR_UserOption.h>
#include <GB/GB_AttributeRef.h>
#include <UT/UT_Options.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_XformOrder.h>

int ParticleViewerHook::getWireMask(GU_Detail* /*gdp*/, const GR_DisplayOption *dopt) const
{
	return GEOPRIMALL;
}

void ParticleViewerHook::renderWire(
		GU_Detail *gdp,
		RE_Render &ren,
		const GR_AttribOffset &ptinfo,
		const GR_DisplayOption *dopt,
		float lod,
		const GU_PrimGroupClosure *hidden_geometry
		)
{
	const GR_UserOption* option = dopt->getOption( "particleviewerbox" );
	if ( ! option ) 
	{
		return;
	}

	int nprim = gdp->primitives().entries();
	for (int p = 0; p < nprim; ++p)
	{
	    GEO_Primitive* prim = gdp->primitives()(p);
	
	    // Ignore hidden geomtry
	    if (hidden_geometry && hidden_geometry->containsPrim(prim))
	        continue;
	
	    // Only deal with particles
	    if (!(prim->getPrimitiveId() & GEOPRIMPART))
	        continue;

		int nvtx = prim->getVertexCount();

		// Early exit if there are no particles
		if ( ! nvtx ) continue;

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
}

int ParticleViewerHook::getShadedMask(GU_Detail* /*gdp*/, const GR_DisplayOption* dopt) const
{
	return GEOPRIMALL;
}

void ParticleViewerHook::renderShaded(
		GU_Detail *gdp,
		RE_Render &ren,
		const GR_AttribOffset &ptinfo,
		const GR_DisplayOption *dopt,
		float lod,
		const GU_PrimGroupClosure *hidden_geometry
		)
{
	const GR_UserOption* option = dopt->getOption( "particleviewerbox" );
	if ( ! option ) 
	{
		return;
	}

	int nprim = gdp->primitives().entries();
	for (int p = 0; p < nprim; ++p)
	{
	    GEO_Primitive* prim = gdp->primitives()(p);
	
	    // Ignore hidden geomtry
	    if (hidden_geometry && hidden_geometry->containsPrim(prim))
	        continue;
	
	    // Only deal with particles
	    if (!(prim->getPrimitiveId() & GEOPRIMPART))
	        continue;

		int nvtx = prim->getVertexCount();

		// Early exit if there are no particles
		if ( ! nvtx ) continue;

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

		/*
		float                   life_buffer[2];
        UT_Vector3              cvs_buffer[4], v_buffer;
        const float             *life;
        const UT_Vector3        *cvs, *v;

        life = ppt->getPointer<float>(life_id, life_buffer, 2);
        cvs  = ppt->getPointer<UT_Vector3>(cvs_id, cvs_buffer, 4);
        v    = ppt->getPointer<UT_Vector3>(v_id, &v_buffer, 1);

	    GB_AttributeRef     Cd_ref;
		Cd_ref = gdp->findNormalAttribute(GEO_POINT_DICT);
		if (Cd_ref.isValid())
		{
			FOR_ALL_GPOINTS(gdp, pp)
			{
				ppt->setValue<UT_Vector3>(Cd_ref, UT_Vector3(1, 0, 0));
			}
		}

		for (int j=0; j < nvtx; j++)
		{
			GEO_Point* point = prim->getVertex(j).getPt();
			UT_Vector4 pos = point->getPos();
			
			UT_Vector3 cd( 1.0f, 1.0f, 1.0f );
			if ( colourValid )
			{
				colourAttr.setElement( point );
				cd = colourAttr.getV3();
			}

			UT_Vector3 scale( 1.0f, 1.0f, 1.0f );
			if ( scaleValid )
			{
				scaleAttr.setElement( point );
				scale = scaleAttr.getV3();
			}

			UT_Vector3 rotate( 0.0f, 0.0f, 0.0f );
			GEO_AttributeHandle rotateAttr = gdp->getPointAttribute( "rotate" );
			if ( rotateValid )
			{
				rotateAttr.setElement( point );
				rotate = rotateAttr.getV3();
			}

			setColor( cd.x(), cd.y(), cd.z(), 1.0 );

			pushMatrix();

			translate( pos.x(), pos.y(), pos.z() );

			// Rotate around each axis in turn
			rotate( rotate.x(), 'x' );
			rotate( rotate.y(), 'y' );
			rotate( rotate.z(), 'z' );

			scale( scale.x(), scale.y(), scale.z() );

			beginQuads();

			float nml[3];
			nml[0] = 0.0; nml[1] = 0.0; nml[2] = -1.0;
			n3DW(nml);
			
			vertex3DW( -0.5,  0.5, -0.5 );
			vertex3DW(  0.5,  0.5, -0.5 );
			vertex3DW(  0.5, -0.5, -0.5 );
			vertex3DW( -0.5, -0.5, -0.5 );

			nml[0] = 0.0; nml[1] = 0.0; nml[2] = 1.0; 
			n3DW(nml);

			vertex3DW( -0.5, -0.5,  0.5 );
			vertex3DW(  0.5, -0.5,  0.5 );
			vertex3DW(  0.5,  0.5,  0.5 );
			vertex3DW( -0.5,  0.5,  0.5 );

			nml[0] = 0.0; nml[1] = -1.0; nml[2] = 0.0;
			n3DW(nml);
			
			vertex3DW( -0.5, -0.5, -0.5 );
			vertex3DW(  0.5, -0.5, -0.5 );
			vertex3DW(  0.5, -0.5,  0.5 );
			vertex3DW( -0.5, -0.5,  0.5 );

			nml[0] = 0.0; nml[1] = 1.0; nml[2] = 0.0;
			n3DW(nml);

			vertex3DW( -0.5,  0.5,  0.5 );
			vertex3DW(  0.5,  0.5,  0.5 );
			vertex3DW(  0.5,  0.5, -0.5 );
			vertex3DW( -0.5,  0.5, -0.5 );

			nml[0] = -1.0; nml[1] = 0.0; nml[2] = 0.0;
			n3DW(nml);

			vertex3DW( -0.5, -0.5,  0.5 );
			vertex3DW( -0.5,  0.5,  0.5 );
			vertex3DW( -0.5,  0.5, -0.5 );
			vertex3DW( -0.5, -0.5, -0.5 );

			nml[0] = 1.0; nml[1] = 0.0; nml[2] = 0.0;
			n3DW(nml);

			vertex3DW(  0.5,  0.5, -0.5 );
			vertex3DW(  0.5,  0.5,  0.5 );
			vertex3DW(  0.5, -0.5,  0.5 );
			vertex3DW(  0.5, -0.5, -0.5 );

			endQuadStrip();

			popMatrix();
		}

		*/
	}
}



void
newRenderHook(GR_RenderTable *table)
{
	std::cout << std::endl << "Loading Particle Box 0.2 beta" << std::endl;
	table->addHook( new ParticleViewerHook, GR_RENDER_HOOK_VERSION );
}

