#ifndef TRN_HEADER_H_
#define TRN_HEADER_H_

#include <string>

struct TrnHeader
{
	std::string filename;
	float map_width;
	float chunk_width;
	int   tiles_per_chunk;
	int   use_global_water_height;
	float global_water_height;
	float water_shader_size;
	std::string water_shader_name;
	float seconds_per_world_cycle;

	/* 
	Values not loaded because server doesn't need them
	
	float Collidable_MinDist;
	float Collidable_MaxDist;
	float Collidable_TileSize;
	float Collidable_TileBorder;
	float Collidable_Seed;

	float NonCollidable_MinDist;
	float NonCollidable_MaxDist;
	float NonCollidable_TileSize;
	float NonCollidable_TileBorder;
	float NonCollidable_Seed;

	float NearRadial_MinDist;
	float NearRadial_MaxDist;
	float NearRadial_TileSize;
	float NearRadial_TileBorder;
	float NearRadial_Seed;

	float FarRadial_MinDist;
	float FarRadial_MaxDist;
	float FarRadial_TileSize;
	float FarRadial_TileBorder;
	float FarRadial_Seed;
	*/
};

#endif