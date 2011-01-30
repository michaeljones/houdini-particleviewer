#ifndef GR_PARTICLEVIEWER
#define GR_PARTICLEVIEWER

#include <UT/UT_DSOVersion.h>

#include <RE/RE_Render.h>

#include <GEO/GEO_Primitive.h>

#include <GU/GU_Detail.h>
#include <GU/GU_PrimGroupClosure.h>

#include <GR/GR_Detail.h>
#include <GR/GR_RenderHook.h>
#include <GR/GR_RenderTable.h>
#include <GR/GR_DisplayOption.h>


class ParticleViewerHook : public GR_RenderHook
{
public:

	ParticleViewerHook() {}

    int getWireMask(GU_Detail* /*gdp*/, const GR_DisplayOption *dopt) const;

    void renderWire(
			GU_Detail *gdp,
			RE_Render &ren,
			const GR_AttribOffset &ptinfo,
			const GR_DisplayOption *dopt,
			float lod,
			const GU_PrimGroupClosure *hidden_geometry
			);

	void renderWireCubes(
		GU_Detail *gdp,
		RE_Render &ren,
		const GU_PrimGroupClosure *hidden_geometry
		);

	void renderWireDiscs(
		GU_Detail *gdp,
		RE_Render &ren,
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

	void renderShadedCubes(
		GU_Detail *gdp,
		RE_Render &ren,
		const GU_PrimGroupClosure *hidden_geometry
		);

	void renderShadedDiscs(
		GU_Detail *gdp,
		RE_Render &ren,
		const GU_PrimGroupClosure *hidden_geometry
		);

	virtual GR_HookRender getHookRenderType(GU_Detail * /*gdp*/, const GR_DisplayOption * /*dopt*/);

    virtual const char *getName() const { return "ParticleViewer"; }
};

#endif // GR_PARTICLEVIEWER

