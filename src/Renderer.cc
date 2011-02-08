
#include "Renderer.hh"

#include <GU/GU_Detail.h>
#include <GR/GR_UserOption.h>
#include <GR/GR_DisplayOption.h>
#include <RE/RE_Render.h>

#include <GU/GU_PrimGroupClosure.h>

class BasicRenderer : public Renderer
{
public:

	BasicRenderer( DisplayObject* object )
	 : m_object( object ) {}

	void render(
		GU_Detail *gdp,
		RE_Render &ren,
		const GU_PrimGroupClosure *hidden_geometry
		);

private:

	std::auto_ptr< DisplayObject > m_object;
};

void BasicRenderer::render(
		GU_Detail *gdp,
		RE_Render &ren,
		const GU_PrimGroupClosure *hidden_geometry
		)
{
	m_object->setup( ren );

	int nprim = gdp->primitives().entries();
	for (int p = 0; p < nprim; ++p)
	{
		GEO_Primitive* prim = gdp->primitives()(p);
	
		// Ignore hidden geomtry
		if (hidden_geometry && hidden_geometry->containsPrim(prim))
			continue;
	
		// Only deal with particles
		if (!(prim->getPrimitiveId() & GEOPRIMPART))
			continue;

		unsigned nvtx = prim->getVertexCount();

		// Early exit if there are no particles
		if ( ! nvtx ) continue;

		m_object->draw( gdp, ren, prim, nvtx );
	}
}


class NullRenderer : public Renderer
{
public:

	NullRenderer() {};

	void render(
		GU_Detail *gdp,
		RE_Render &ren,
		const GU_PrimGroupClosure *hidden_geometry
		) {}
};



Renderer* RendererFactory::create( const GR_DisplayOption* dopt, bool wireframe ) const
{
	DisplayObject* object = NULL;

	const GR_UserOption* boxOption = dopt->getOption( "particleviewerbox" );
	if ( boxOption ) 
	{
		object = m_displayObjectFactory->createCube( wireframe );
	}

	const GR_UserOption* discOption = dopt->getOption( "particleviewerdisc" );
	if ( ! object && discOption ) 
	{
		object = m_displayObjectFactory->createDisc( wireframe );
	}

	if ( ! object )
	{
		return new NullRenderer;
	}

	return new BasicRenderer( object );
}

