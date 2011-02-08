#include "GR_particleviewer.hh"

#include <GR/GR_UserOption.h>
#include <GB/GB_AttributeRef.h>
#include <UT/UT_Options.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_XformOrder.h>

GR_RenderHook::GR_HookRender ParticleViewerHook::getHookRenderType(GU_Detail * /*gdp*/, const GR_DisplayOption* dopt)
{
	const GR_UserOption* discOption = dopt->getOption( "particleviewerdisc" );
	if ( discOption ) 
	{
		return GR_RenderHook::VIEW_DEPENDENT;
	}

	return GR_RenderHook::VIEW_INDEPENDENT;
}

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
	bool wireframe = true;
	std::auto_ptr< Renderer > renderer( m_rendererFactory->create( dopt, wireframe ) );
	renderer->render( gdp, ren, hidden_geometry );
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
	bool wireframe = false;
	std::auto_ptr< Renderer > renderer( m_rendererFactory->create( dopt, wireframe ) );
	renderer->render( gdp , ren, hidden_geometry );
}
 

