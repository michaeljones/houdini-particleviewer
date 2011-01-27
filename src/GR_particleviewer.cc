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
	int                  i, nprim, nvtx;
	GEO_Primitive       *prim;

	const GR_UserOption* option = dopt->getOption( "particleviewerbox" );
	if ( ! option ) 
	{
		return;
	}
	
	nprim = gdp->primitives().entries();
	for (i = 0; i < nprim; i++)
	{
	    prim = gdp->primitives()(i);
	
	    // Ignore hidden geomtry
	    if (hidden_geometry && hidden_geometry->containsPrim(prim))
	        continue;
	
	    // Only deal with particles
	    if (!(prim->getPrimitiveId() & GEOPRIMPART))
	        continue;

		nvtx = prim->getVertexCount();

		GEO_AttributeHandle scaleAttr = gdp->getPointAttribute( "scale" );
		GEO_AttributeHandle rotateAttr = gdp->getPointAttribute( "rotate" );

		bool scaleValid = scaleAttr.isAttributeValid();
		bool rotateValid = rotateAttr.isAttributeValid();

		for (int j=0; j < nvtx; j++)
		{
			GEO_Point* point = prim->getVertex(j).getPt();
			UT_Vector4 pos = point->getPos();

			UT_Vector3 scale( 1.0f, 1.0f, 1.0f );
			if ( scaleValid )
			{
				scaleAttr.setElement( point );
				scale = scaleAttr.getV3();
			}

			UT_Vector3 rotate( 0.0f, 0.0f, 0.0f );
			if ( rotateValid )
			{
				rotateAttr.setElement( point );
				rotate = rotateAttr.getV3();
			}

			ren.pushMatrix();

			ren.translate( pos.x(), pos.y(), pos.z() );

			// Rotate around each axis in turn
			ren.rotate( rotate.x(), 'x' );
			ren.rotate( rotate.y(), 'y' );
			ren.rotate( rotate.z(), 'z' );

			ren.scale( scale.x(), scale.y(), scale.z() );

			ren.beginClosedLine();

			ren.vertex3DW( -0.5, -0.5, -0.5 );
			ren.vertex3DW(  0.5, -0.5, -0.5 );
			ren.vertex3DW(  0.5,  0.5, -0.5 );
			ren.vertex3DW( -0.5,  0.5, -0.5 );

			ren.endClosedLine();

			ren.beginClosedLine();

			ren.vertex3DW( -0.5, -0.5, 0.5 );
			ren.vertex3DW(  0.5, -0.5, 0.5 );
			ren.vertex3DW(  0.5,  0.5, 0.5 );
			ren.vertex3DW( -0.5,  0.5, 0.5 );

			ren.endClosedLine();

			ren.beginLines();

			ren.vertex3DW( -0.5, -0.5, -0.5 );
			ren.vertex3DW( -0.5, -0.5,  0.5 );

			ren.vertex3DW(  0.5,  0.5, -0.5 );
			ren.vertex3DW(  0.5,  0.5,  0.5 );

			ren.vertex3DW( -0.5,  0.5, -0.5 );
			ren.vertex3DW( -0.5,  0.5,  0.5 );

			ren.vertex3DW(  0.5, -0.5, -0.5 );
			ren.vertex3DW(  0.5, -0.5,  0.5 );

			ren.endLines();

			ren.popMatrix();
		}

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
	int                  i, nprim, nvtx;
	GEO_Primitive       *prim;

	const GR_UserOption* option = dopt->getOption( "particleviewerbox" );
	if ( ! option ) 
	{
		return;
	}

	nprim = gdp->primitives().entries();
	for (i = 0; i < nprim; i++)
	{
	    prim = gdp->primitives()(i);
	
	    // Ignore hidden geomtry
	    if (hidden_geometry && hidden_geometry->containsPrim(prim))
	        continue;
	
	    // Only deal with particles
	    if (!(prim->getPrimitiveId() & GEOPRIMPART))
	        continue;

		nvtx = prim->getVertexCount();

		GEO_AttributeHandle colourAttr = gdp->getPointAttribute( "colour" );
		GB_AttributeRef scaleRef = gdp->findPointAttrib( "scale", 3, GB_ATTRIB_FLOAT );
		GB_AttributeRef rotateRef = gdp->findPointAttrib( "rotate", 3, GB_ATTRIB_FLOAT );

		// bool colourValid = colourAttr.isAttributeValid();
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

			if ( rotateValid )
			{
				UT_Vector3 rotateBuffer;
				const UT_Vector3* rotate;
				UT_XformOrder xformOrder;
				rotate = ppt->getPointer< UT_Vector3 >( rotateRef, &rotateBuffer, 1 );
				transform.rotate( rotate->x(), rotate->y(), rotate->z(), xformOrder );
			}

			if ( scaleValid )
			{
				UT_Vector3 scaleBuffer;
				const UT_Vector3* scale;
				scale = ppt->getPointer< UT_Vector3 >( scaleRef, &scaleBuffer, 1 );
				transform.scale( scale->x(), scale->y(), scale->z() );
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

		int* indices = new int[nvtx * 24];
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

		float* normals = new float[18];

		normals[ 0] =  0.0f;
		normals[ 1] =  0.0f;
		normals[ 2] = -1.0f;
		normals[ 3] =  0.0f;
		normals[ 4] =  0.0f;
		normals[ 5] = -1.0f;
		normals[ 6] = -1.0f;
		normals[ 7] =  0.0f;
		normals[ 8] =  0.0f;
		normals[ 9] =  1.0f;
		normals[10] =  0.0f;
		normals[11] =  0.0f;
		normals[12] =  0.0f;
		normals[13] =  1.0f;
		normals[14] =  0.0f;
		normals[15] =  0.0f;
		normals[16] = -1.0f;
		normals[17] =  0.0f;

		ren.beginQuads();

		for (int j=0; j < nvtx; ++j )
		{
			int offset = j * 3 * 8;

			for ( int i=0; i<6; ++i )
			{
				// ren.n3DW( normals + i * 3 );

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

