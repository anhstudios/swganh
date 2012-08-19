#include "layer_loader.h"

#include <anh/byte_buffer.h>

#include "container_layer.h"
#include "boundary_circle.h"
#include "boundary_polygon.h"
#include "boundary_polyline.h"
#include "boundary_rectangle.h"
#include "filter_fractal.h"
#include "filter_height.h"
#include "filter_slope.h"
#include "height_constant.h"
#include "height_fractal.h"

Layer* LayerLoader(std::shared_ptr<folder_node> node)
{
	if(node->name() == "LAYRFORM")
		return new ContainerLayer();
	else if(node->name() == "BCIRFORM")
		return new BoundaryCircle();
	else if(node->name() == "BPOLFORM")
		return new BoundaryPolygon();
	else if(node->name() == "BPLNFORM")
		return new BoundaryPolyline();
	else if(node->name() == "BRECFORM")
		return new BoundaryRectangle();
	else if(node->name() == "FFRAFORM")
		return new FractalFilter();
	else if(node->name() == "FHGTFORM")
		return new HeightFilter();
	else if(node->name() == "FSLPFORM")
		return new SlopeFilter();
	else if(node->name() == "AHCNFORM")
		return new HeightConstant();
	else if(node->name() == "AHFRFORM")
		return new HeightFractal();
	else
		return nullptr;
}