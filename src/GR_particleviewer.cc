#include "GR_particleviewer.hh"

#include <GEO/GEO_PrimType.h>

int ParticleViewerHook::getWireMask(GU_Detail* /*gdp*/, const GR_DisplayOption *dopt) const
{
	return GEOPRIMPART;
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
	ren.beginClosedLine();

	ren.vertex3DW( -1, -1, -1 );
	ren.vertex3DW(  1, -1, -1 );
	ren.vertex3DW(  1,  1, -1 );
	ren.vertex3DW( -1,  1, -1 );

	ren.endClosedLine();
}

int ParticleViewerHook::getShadedMask(GU_Detail* /*gdp*/, const GR_DisplayOption* dopt) const
{
	return GEOPRIMPART;
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
	ren.beginClosedLine();
	
	ren.vertex3DW( -1, -1,  1 );
	ren.vertex3DW(  1, -1,  1 );
	ren.vertex3DW(  1,  1,  1 );
	ren.vertex3DW( -1,  1,  1 );

	ren.endClosedLine();
}


void newRenderHook( GR_RenderTable* table )
{
	table->addHook( new ParticleViewer );
}

