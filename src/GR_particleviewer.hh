#ifndef GR_PARTICLEVIEWER
#define GR_PARTICLEVIEWER

#include <GU/GU_Detail.h>
#include <GU/GU_PrimGroupClosure.h>

#include <GR/GR_Detail.h>
#include <GR/GR_RenderHook.h>
#include <GR/GR_RenderTable.h>
#include <GR/GR_DisplayOption.h>

#include "Renderer.hh"


class ParticleViewerHook : public GR_RenderHook
{
public:

	ParticleViewerHook( RendererFactory* rendererFactory )
	 : m_rendererFactory( rendererFactory ) {}

    int getWireMask(GU_Detail* /*gdp*/, const GR_DisplayOption *dopt) const;

    void renderWire(
			GU_Detail *gdp,
			RE_Render &ren,
			const GR_AttribOffset &ptinfo,
			const GR_DisplayOption *dopt,
			float lod,
			const GU_PrimGroupClosure *hidden_geometry
			);

    int getShadedMask(GU_Detail* /*gdp*/, const GR_DisplayOption* dopt) const;

    virtual void renderShaded(
			GU_Detail *gdp,
			RE_Render &ren,
			const GR_AttribOffset &ptinfo,
			const GR_DisplayOption *dopt,
			float lod,
			const GU_PrimGroupClosure *hidden_geometry
			);

	virtual GR_HookRender getHookRenderType(GU_Detail * /*gdp*/, const GR_DisplayOption * /*dopt*/);

    virtual const char *getName() const { return "ParticleViewer"; }

private:

	const std::auto_ptr< RendererFactory > m_rendererFactory;
};

#endif // GR_PARTICLEVIEWER

