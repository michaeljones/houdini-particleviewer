#include "GR_particleviewer.hh"

#include <GEO/GEO_PrimType.h>
#include <GR/GR_RenderTable.h>
#include <UT/UT_DSOVersion.h>

int ParticleViewerHook::getWireMask(GU_Detail* /*gdp*/, const GR_DisplayOption *dopt) const
{
	std::cerr << "mpj-debug: Requesting wire mask" << std::endl;
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
	std::cerr << "mpj-debug: Calliong renderWire" << std::endl;
	ren.beginClosedLine();

	ren.vertex3DW( -1, -1, -1 );
	ren.vertex3DW(  1, -1, -1 );
	ren.vertex3DW(  1,  1, -1 );
	ren.vertex3DW( -1,  1, -1 );

	ren.endClosedLine();
}

int ParticleViewerHook::getShadedMask(GU_Detail* /*gdp*/, const GR_DisplayOption* dopt) const
{
std::cerr << "mpj-debug: Requesting shaded mask" << std::endl;
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
	std::cerr << "mpj-debug: Calling render shaded" << std::endl;
	ren.beginClosedLine();
	
	ren.vertex3DW( -1, -1,  1 );
	ren.vertex3DW(  1, -1,  1 );
	ren.vertex3DW(  1,  1,  1 );
	ren.vertex3DW( -1,  1,  1 );

	ren.endClosedLine();
}


void newRenderHook( GR_RenderTable* table )
{
	std::cout << "Adding particle viewer hook" << std::endl;
	table->addHook( new ParticleViewerHook, 2 );		// GR Render Hook Version
}

