#include "GR_particleviewer.hh"

#include <GR/GR_UserOption.h>
#include <UT/UT_Options.h>

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
		GEO_AttributeHandle scaleAttr = gdp->getPointAttribute( "scale" );
		GEO_AttributeHandle rotateAttr = gdp->getPointAttribute( "rotate" );

		bool colourValid = colourAttr.isAttributeValid();
		bool scaleValid = scaleAttr.isAttributeValid();
		bool rotateValid = rotateAttr.isAttributeValid();

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

			ren.setColor( cd.x(), cd.y(), cd.z(), 1.0 );

			ren.pushMatrix();

			ren.translate( pos.x(), pos.y(), pos.z() );

			// Rotate around each axis in turn
			ren.rotate( rotate.x(), 'x' );
			ren.rotate( rotate.y(), 'y' );
			ren.rotate( rotate.z(), 'z' );

			ren.scale( scale.x(), scale.y(), scale.z() );

			ren.beginQuads();

			float nml[3];
			nml[0] = 0.0; nml[1] = 0.0; nml[2] = -1.0;
			ren.n3DW(nml);
			
			ren.vertex3DW( -0.5,  0.5, -0.5 );
			ren.vertex3DW(  0.5,  0.5, -0.5 );
			ren.vertex3DW(  0.5, -0.5, -0.5 );
			ren.vertex3DW( -0.5, -0.5, -0.5 );

			nml[0] = 0.0; nml[1] = 0.0; nml[2] = 1.0; 
			ren.n3DW(nml);

			ren.vertex3DW( -0.5, -0.5,  0.5 );
			ren.vertex3DW(  0.5, -0.5,  0.5 );
			ren.vertex3DW(  0.5,  0.5,  0.5 );
			ren.vertex3DW( -0.5,  0.5,  0.5 );

			nml[0] = 0.0; nml[1] = -1.0; nml[2] = 0.0;
			ren.n3DW(nml);
			
			ren.vertex3DW( -0.5, -0.5, -0.5 );
			ren.vertex3DW(  0.5, -0.5, -0.5 );
			ren.vertex3DW(  0.5, -0.5,  0.5 );
			ren.vertex3DW( -0.5, -0.5,  0.5 );

			nml[0] = 0.0; nml[1] = 1.0; nml[2] = 0.0;
			ren.n3DW(nml);

			ren.vertex3DW( -0.5,  0.5,  0.5 );
			ren.vertex3DW(  0.5,  0.5,  0.5 );
			ren.vertex3DW(  0.5,  0.5, -0.5 );
			ren.vertex3DW( -0.5,  0.5, -0.5 );

			nml[0] = -1.0; nml[1] = 0.0; nml[2] = 0.0;
			ren.n3DW(nml);

			ren.vertex3DW( -0.5, -0.5,  0.5 );
			ren.vertex3DW( -0.5,  0.5,  0.5 );
			ren.vertex3DW( -0.5,  0.5, -0.5 );
			ren.vertex3DW( -0.5, -0.5, -0.5 );

			nml[0] = 1.0; nml[1] = 0.0; nml[2] = 0.0;
			ren.n3DW(nml);

			ren.vertex3DW(  0.5,  0.5, -0.5 );
			ren.vertex3DW(  0.5,  0.5,  0.5 );
			ren.vertex3DW(  0.5, -0.5,  0.5 );
			ren.vertex3DW(  0.5, -0.5, -0.5 );

			ren.endQuadStrip();

			ren.popMatrix();
		}

	}
}



void
newRenderHook(GR_RenderTable *table)
{
	std::cout << std::endl << "Loading Particle Box 0.2 beta" << std::endl;
	table->addHook( new ParticleViewerHook, GR_RENDER_HOOK_VERSION );
}

