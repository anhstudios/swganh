#ifndef ZONE_COMPONENTS_SPATIAL_INDEX_LOADER_H
#define ZONE_COMPONENTS_SPATIAL_INDEX_LOADER_H

#include <anh/component/component_loader_interface.h>

class SpatialIndexLoader : public anh::component::ComponentLoaderInterface
{
public:
    bool Load(std::shared_ptr<anh::component::ComponentInterface> comp);
    void Unload(std::shared_ptr<anh::component::ComponentInterface> comp);
};

#endif // ZONE_COMPONENTS_SPATIAL_INDEX_LOADER_H