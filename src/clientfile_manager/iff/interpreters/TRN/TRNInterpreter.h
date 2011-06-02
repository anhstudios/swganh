/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2011 The SWG:ANH Team
---------------------------------------------------------------------------------------
Use of this source code is governed by the GPL v3 license that can be found
in the COPYING file or at http://www.gnu.org/licenses/gpl-3.0.html

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,a
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
---------------------------------------------------------------------------------------
*/

#ifndef INTERPRETERS_TRN_H
#define INTERPRETERS_TRN_H

#include "../IFFInterpreter.h"
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

#include "Random.h"
#include "PerlinNoise.h"

namespace ClientFileManager
{
	namespace IFF
	{
		namespace Interpreters
		{
			class TRNInterpreter : public IFFInterpreter
			{
			public:
				TRNInterpreter();
				
				struct AHCN
				{
					int transform_type;
					float height_val;
				};

				struct AHFR
				{
					int fractal_id;
					int transform_type;
					float height_val;
				};

				struct AHTR
				{
					float flat_ratio;
					float height_delta;
				};

				struct BCIR
				{
					int feather_type;
					float feather_amount;
				};

				struct BPLN
				{
					//int size
					//vert vert vert
					std::vector<glm::vec2> verts;

					int feather_type;
					float feather_amount;
					float line_width;
				};

				struct BPOL
				{
					//int size;
					//float x, float y....
					std::vector<glm::vec2> verts;

					int feather_type;
					float feather_amount;
					int use_water_height;
					float water_height;

					float water_shader_size;
					std::string water_shader;
				};

				struct BREC
				{

					int feather_type;
					float feather_amount;
				};

				struct FFRA
				{
				};

				struct FHGT
				{
				};

				struct FSLP
				{
				};

				class MFAM
				{
				public:
					MFAM(unsigned char* data, unsigned char* data2);

					int fractal_id;
					unsigned char* fractal_label;

					//MFRCFORM
					int				seed;
					int				use_bias;
					float			bias;
					int				use_gain;
					float			gain;
					unsigned int	octaves;
					float			octaves_arg;
					float			amplitude;
					float			freq_x;
					float			freq_z;
					float			offset_x;
					float			offset_z;
					unsigned int	combination_type;

					float			offset;
					Random*			rand;
					PerlinNoise*	noise;

					void setSeed(int seed);
					void setAmplitude(void);
					float getNoise(float x, float z);

					double calculateCombination1(float x, float z);
					double calculateCombination2(float x, float z);
					double calculateCombination3(float x, float z);
					double calculateCombination4(float x, float z);
					double calculateCombination5(float x, float z);
				};

				class HEADER
				{
				public:
					//Basic Data
					unsigned char* filename;
					float map_width;
					float chunk_width;
					int   tiles_per_chunk;
					int   use_global_water_height;
					float global_water_height;
					float water_shader_size;
					unsigned char* water_shader_name;
					float seconds_per_world_cycle;

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
				};

				class FOOTER
				{
				public:
					float gameWidth;
					float   chunkGameWidth; //width of a chunk
					int   chunkBitFieldSize;
					int   bitMapWidth;

					unsigned char* WMAP;
					unsigned int WMAP_SIZE;
					unsigned char* SMAP;
					unsigned int SMAP_SIZE;
				};

				virtual InterpreterType getType() { return TRN_INTERPRETER; }

			private:



			};
		}
	};
};
#endif