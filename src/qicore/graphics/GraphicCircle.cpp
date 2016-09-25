#include "qicore/graphics/GraphicCircle.hpp"

using namespace qicore::graphics;

void GraphicCircle::Draw(GraphicLayer* layer)
{
//    painter->DrawCircle(center_, radius_, color);
}

BoundingBox GraphicCircle::GetBoundingBox()
{
    Point corner(radius_, radius_);

    return BoundingBox(center_-corner,radius_*2,radius_*2);
}