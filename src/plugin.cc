
#include <UT/UT_DSOVersion.h>

#include "GR_particleviewer.hh"

void newRenderHook( GR_RenderTable *table )
{
	std::cout << std::endl << "Loading Particle Box 0.4 beta" << std::endl;

	RendererFactory* rendererFactory = new RendererFactory( new DisplayObjectFactory );

	table->addHook( new ParticleViewerHook( rendererFactory ), GR_RENDER_HOOK_VERSION );
}

