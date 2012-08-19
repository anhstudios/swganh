#ifndef TRN_FRACTAL_FAMILY_H_
#define TRN_FRACTAL_FAMILY_H_

#include <string>

#include "anh/byte_buffer.h"
#include "random.h"
#include "perlin_noise.h"

namespace swganh
{
namespace tre
{
	class Fractal
	{
	public:
		int fractal_id;
		std::string fractal_label;

		Fractal(anh::ByteBuffer& buffer)
		{
			fractal_id = buffer.read<int32_t>();
			fractal_label = buffer.read<std::string>(false, true);
		}

		//MFRCFORM
		int				seed;				//seed 
		int				use_bias;			//Bias type
		float			bias;				//bias amount
		int				use_gain;			//Gain type
		float			gain;				//Gain amount
		unsigned int	octaves;			//Octave count
		float			octaves_arg;		//Octave Parameters
		float			amplitude;			//Amplitude
		float			freq_x;				//Frequency X
		float			freq_z;				//Frequency Y
		float			offset_x;			//X offset
		float			offset_z;			//y offset
		unsigned int	combination_type;	//Use both

		float			offset;				// Resulting offset from amplitude
		Random			rand;				// Random generator
		PerlinNoise		noise;				// Noise generator

		void Deserialize(anh::ByteBuffer& buffer)
		{
			seed = buffer.read<int32_t>();

			use_bias = buffer.read<int32_t>();
			bias = buffer.read<float>();

			use_gain = buffer.read<int32_t>();
			gain = buffer.read<float>();

			octaves = buffer.read<uint32_t>();
			octaves_arg = buffer.read<float>();

			amplitude = buffer.read<float>();
			freq_x = buffer.read<float>();
			freq_z = buffer.read<float>();
			offset_x = buffer.read<float>();
			offset_z = buffer.read<float>();

			combination_type = buffer.read<uint32_t>();

			setSeed(seed);
		}


		// Seed the random generator
		void setSeed(int seed)
		{
			rand.setSeed(seed);
			noise.prep(&rand);

			double coord[2];
			coord[0] = 0;
			coord[1] = 0;
			noise.noise2(coord);
		}

		// Set the offset from the TRN file amplitude
		void setAmplitude(void)
		{
			offset = 0.0f;

			float curr_amplitude = 0.0f;
			float next_amplitude = 1.0f;

			for (unsigned int i = 0; i < octaves; i++)
			{
				curr_amplitude = curr_amplitude + next_amplitude;
				next_amplitude = next_amplitude * amplitude;
			}

			offset = curr_amplitude;

			if (offset != 0)
				offset = (float)(1.0/offset);
		}

		// Generate noise for given coordinates
		double getNoise(double x, double z)
		{
			double xFrequency = x * freq_x;
			double zFrequency = z * freq_z;

			double result = 0;

			switch (combination_type) 
			{
			case 0:
			case 1:
				result = calculateCombination1(xFrequency, zFrequency);
				break;
			case 2:
				result = calculateCombination2(xFrequency, zFrequency);
				break;
			case 3:
				result = calculateCombination3(xFrequency, zFrequency);
				break;
			case 4:
				result = calculateCombination4(xFrequency, zFrequency);
				break;
			case 5:
				result = calculateCombination5(xFrequency, zFrequency);
				break;
			}

			if (use_bias) 
			{
				result = pow(result, log(bias) / log(0.5));
			}

			if (use_gain) 
			{
				if (result < 0.001) 
				{
					result = 0;

					return result;
				}

				if (result > 0.999) 
				{
					result = 1.0;

					return result;
				}

				double log_gain = log(1.0 - gain) / log(0.5);

				if (result < 0.5) 
				{
					result = pow(result * 2, log_gain) * 0.5;

					return result;
				}

				result = 1.0 - pow((1.0 - result) * 2, log_gain) * 0.5;
			}

			return result;
		}

		double calculateCombination1(double x, double z)
		{
			double curr_offset = 1, curr_ampl = 1;
			double result = 0;

			double zOffset = z + offset_z; 
			double xOffset = x + offset_x; 
			double zNoise, xNoise, noise_gen = 0;
			double coord[2];

			for (unsigned int i = 0; i < octaves; ++i) 
			{
				zNoise = zOffset * curr_offset;
				xNoise = xOffset * curr_offset;

				coord[0] = xNoise;
				coord[1] = zNoise;

				noise_gen = noise.noise2(coord) * curr_ampl + noise_gen;
				curr_offset = curr_offset * octaves_arg; 
				curr_ampl = curr_ampl * amplitude; 
			}

			result = (noise_gen * offset + 1.0) * 0.5;

			return result;
		}

		double calculateCombination2(double x, double z)
		{
			double noise_gen = 0;
			double zOffset = z + offset_z;
			double zNoise, xOffset, xNoise;
			double curr_offset = 1.0;
			double curr_ampl = 1.0;
			double result = 0;
			double coord[2];

			for (unsigned int i = 0; i < octaves; ++i) 
			{
				zNoise = zOffset * curr_offset;
				xOffset = x + offset_x;
				xNoise = xOffset * curr_offset;

				coord[0] = xNoise;
				coord[1] = zNoise;

				noise_gen = (1.0 - fabs(noise.noise2(coord))) * curr_ampl + noise_gen;
				curr_offset = curr_offset * octaves_arg; 
				curr_ampl = curr_ampl * amplitude;
			}

			result = noise_gen * offset;

			return result;
		}

		double calculateCombination3(double x, double z)
		{
			double curr_offset = 1.0;
			double curr_ampl = 1.0;
			double result = 0;
			double noise_gen = 0;
			double zOffset = z + offset_z; 
			double zNoise, xOffset, xNoise;

			double coord[2];

			for (unsigned int i = 0; i < octaves; ++i) 
			{
				zNoise = zOffset * curr_offset;
				xOffset = x + offset_x; 
				xNoise = xOffset * curr_offset;

				coord[0] = xNoise;
				coord[1] = zNoise;

				noise_gen = fabs(noise.noise2(coord)) * curr_ampl + noise_gen;
				curr_offset = curr_offset * octaves_arg;
				curr_ampl = curr_ampl * amplitude; 
			}

			result = noise_gen * offset; 

			return result;
		}

		double calculateCombination4(double x, double z)
		{
			double noise_gen = 0;
			double zNoise, xNoise, noise_gain;
			double zOffset = z + offset_z; 
			double xOffset = x + offset_x; 
			double coord[2];

			double curr_offset = 1.0;
			double curr_ampl = 1.0;
			double result = 0;

			for (unsigned int i = 0; i < octaves; ++i) 
			{
				zNoise = zOffset * curr_offset;
				xNoise = xOffset * curr_offset;

				coord[0] = xNoise;
				coord[1] = zNoise;

				noise_gain = noise.noise2(coord);
				if ( noise_gain >= 0.0 ) 
					if ( noise_gain > 1.0 )
						noise_gain = 1.0;
				else 
					noise_gain = 0.0;

				noise_gen = (1.0 - noise_gain) * curr_ampl + noise_gen;
				curr_offset = curr_offset * octaves_arg; 
				curr_ampl = curr_ampl * amplitude;
			}

			result = noise_gen * offset;

			return result;
		}

		double calculateCombination5(double x, double z)
		{
			double noise_gen = 0;
			double zNoise, xNoise, noise_gain;
			double zOffset = z + offset_z; 
			double xOffset = x + offset_x;
			double coord[2];
			double result = 0;

			double curr_offset = 1.0;
			double curr_ampl = 1.0;

			for (unsigned int i = 0; i < octaves; ++i) 
			{
				zNoise = zOffset * curr_offset;
				xNoise = xOffset * curr_offset;

				coord[0] = xNoise;
				coord[1] = zNoise;

				noise_gain = noise.noise2(coord);

				if ( noise_gain >= 0.0 ) 
					if (noise_gain > 1.0)
						noise_gain = 1.0;
				else 
					noise_gain = 0.0;

				noise_gen = noise_gain * curr_ampl + noise_gen;
				curr_offset = curr_offset * octaves_arg;
				curr_ampl = curr_ampl * amplitude;
			}

			result = noise_gen * offset;

			return result;
		}
	};

}
}

#endif