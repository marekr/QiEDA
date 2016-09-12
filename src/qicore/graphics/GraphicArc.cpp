#include "qicore/graphics/GraphicArc.hpp"
#include "nanovg.h"

using namespace qicore::graphics;

void GraphicArc::draw(GLPainter* painter)
{
}


BoundingBox GraphicArc::GetBoundingBox()
{
    Point corner(radius_, radius_);

    return BoundingBox(center_-corner,radius_*2,radius_*2);
}