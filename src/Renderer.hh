#ifndef RENDERER_HH
#define RENDERER_HH

#include "DisplayObject.hh"

#include <memory>

class GU_Detail;
class RE_Render;
class GU_PrimGroupClosure;
class GR_DisplayOption;

class Renderer
{
public:

	Renderer() {}
	virtual ~Renderer() {}

	virtual void render(
		GU_Detail *gdp,
		RE_Render &ren,
		const GU_PrimGroupClosure *hidden_geometry
		) = 0;
};


class RendererFactory
{
public:

	RendererFactory( DisplayObjectFactory* displayObjectFactory )
	 : m_displayObjectFactory( displayObjectFactory ) {}

	Renderer* create( const GR_DisplayOption* dopt, bool wireframe ) const;

private:

	const std::auto_ptr< DisplayObjectFactory > m_displayObjectFactory;

};


#endif // RENDERER_HH

