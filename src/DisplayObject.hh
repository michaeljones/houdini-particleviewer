#ifndef DISPLAYOBJECT_HH
#define DISPLAYOBJECT_HH

class GU_Detail;
class RE_Render;
class GEO_Primitive;

class DisplayObject
{
public:

	DisplayObject() {}
	virtual ~DisplayObject() {};

	virtual void setup( RE_Render& ren ) {};

	virtual void draw(
		GU_Detail *gdp,
		RE_Render &ren,
		GEO_Primitive* prim,
		unsigned nvtx
		) = 0;
};

class DisplayObjectFactory
{
public:

	DisplayObjectFactory() {}

	DisplayObject* createCube( bool wireframe );
	DisplayObject* createDisc( bool wireframe );
};


#endif // DISPLAYOBJECT_HH

