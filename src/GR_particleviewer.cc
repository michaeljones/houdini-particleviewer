#include "GR_particleviewer.hh"

int ParticleViewerHook::getWireMask(GU_Detail* /*gdp*/, const GR_DisplayOption *dopt) const
{
	std::cerr << "mpj-debug: Requesting wire mask" << std::endl;
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

		for (int j=0; j < nvtx; j++)
		{
			GEO_Point* point = prim->getVertex(j).getPt();
			UT_Vector4 pos = point->getPos();

			ren.beginClosedLine();

			ren.vertex3DW( -0.1 + pos.x(), -0.1 + pos.y(), -0.1 + pos.z() );
			ren.vertex3DW(  0.1 + pos.x(), -0.1 + pos.y(), -0.1 + pos.z() );
			ren.vertex3DW(  0.1 + pos.x(),  0.1 + pos.y(), -0.1 + pos.z() );
			ren.vertex3DW( -0.1 + pos.x(),  0.1 + pos.y(), -0.1 + pos.z() );

			ren.endClosedLine();

			ren.beginClosedLine();

			ren.vertex3DW( -0.1 + pos.x(), -0.1 + pos.y(), 0.1 + pos.z() );
			ren.vertex3DW(  0.1 + pos.x(), -0.1 + pos.y(), 0.1 + pos.z() );
			ren.vertex3DW(  0.1 + pos.x(),  0.1 + pos.y(), 0.1 + pos.z() );
			ren.vertex3DW( -0.1 + pos.x(),  0.1 + pos.y(), 0.1 + pos.z() );

			ren.endClosedLine();

			ren.beginLines();

			ren.vertex3DW( -0.1 + pos.x(), -0.1 + pos.y(), -0.1 + pos.z() );
			ren.vertex3DW( -0.1 + pos.x(), -0.1 + pos.y(),  0.1 + pos.z() );

			ren.vertex3DW(  0.1 + pos.x(),  0.1 + pos.y(), -0.1 + pos.z() );
			ren.vertex3DW(  0.1 + pos.x(),  0.1 + pos.y(),  0.1 + pos.z() );

			ren.vertex3DW( -0.1 + pos.x(),  0.1 + pos.y(), -0.1 + pos.z() );
			ren.vertex3DW( -0.1 + pos.x(),  0.1 + pos.y(),  0.1 + pos.z() );

			ren.vertex3DW(  0.1 + pos.x(), -0.1 + pos.y(), -0.1 + pos.z() );
			ren.vertex3DW(  0.1 + pos.x(), -0.1 + pos.y(),  0.1 + pos.z() );

			ren.endLines();
		}

	}

	std::cerr << "mpj-debug: Calliong renderWire" << std::endl;
}

int ParticleViewerHook::getShadedMask(GU_Detail* /*gdp*/, const GR_DisplayOption* dopt) const
{
	std::cerr << "mpj-debug: Requesting shaded mask" << std::endl;
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

		for (int j=0; j < nvtx; j++)
		{
			GEO_Point* point = prim->getVertex(j).getPt();
			UT_Vector4 pos = point->getPos();

			ren.beginQuadStrip();

			ren.vertex3DW( -0.1 + pos.x(),  0.1 + pos.y(),  0.1 + pos.z() );
			ren.vertex3DW(  0.1 + pos.x(),  0.1 + pos.y(),  0.1 + pos.z() );

			ren.vertex3DW( -0.1 + pos.x(), -0.1 + pos.y(),  0.1 + pos.z() );
			ren.vertex3DW(  0.1 + pos.x(), -0.1 + pos.y(),  0.1 + pos.z() );

			ren.vertex3DW( -0.1 + pos.x(), -0.1 + pos.y(), -0.1 + pos.z() );
			ren.vertex3DW(  0.1 + pos.x(), -0.1 + pos.y(), -0.1 + pos.z() );

			ren.vertex3DW( -0.1 + pos.x(),  0.1 + pos.y(), -0.1 + pos.z() );
			ren.vertex3DW(  0.1 + pos.x(),  0.1 + pos.y(), -0.1 + pos.z() );

			ren.endQuadStrip();

			ren.beginQuadStrip();

			ren.vertex3DW( -0.1 + pos.x(), -0.1 + pos.y(), -0.1 + pos.z() );
			ren.vertex3DW( -0.1 + pos.x(), -0.1 + pos.y(),  0.1 + pos.z() );

			ren.vertex3DW( -0.1 + pos.x(),  0.1 + pos.y(), -0.1 + pos.z() );
			ren.vertex3DW( -0.1 + pos.x(),  0.1 + pos.y(),  0.1 + pos.z() );

			ren.vertex3DW(  0.1 + pos.x(),  0.1 + pos.y(), -0.1 + pos.z() );
			ren.vertex3DW(  0.1 + pos.x(),  0.1 + pos.y(),  0.1 + pos.z() );

			ren.vertex3DW(  0.1 + pos.x(), -0.1 + pos.y(), -0.1 + pos.z() );
			ren.vertex3DW(  0.1 + pos.x(), -0.1 + pos.y(),  0.1 + pos.z() );

			ren.endQuadStrip();
		}

	}

	std::cerr << "mpj-debug: Calling render shaded" << std::endl;
}


void
newRenderHook(GR_RenderTable *table)
{
	std::cerr << "mpj-debug: Adding Particle box" << std::endl;
	table->addHook( new ParticleViewerHook, GR_RENDER_HOOK_VERSION );
}

