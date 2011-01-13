#ifndef GR_PARTICLEVIEWER
#define GR_PARTICLEVIEWER

#include <GR/GR_RenderHook.h>

class ParticleViewerHook : public GR_RenderHoook
{
public:

	ParticleViewer() {}

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

    virtual const char *getName() const { return "ParticleViewer"; }
};

#endif // GR_PARTICLEVIEWER

