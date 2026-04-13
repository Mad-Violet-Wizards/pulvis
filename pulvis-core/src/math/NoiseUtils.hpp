#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <cstdint>

namespace pulvis::core::math
{
    /* Fractional Brownian Motion */
    inline float FBM(glm::vec2 _pos, int _octaves, float _lacunarity = 2.f, float _gain = 0.5f)
    {
        float value = 0.f;
        float amplitude = 1.f;
        float frequency = 1.f;
        float normalization = 0.f;

        for(int i = 0; i < _octaves; ++i)
        {
			value += amplitude * glm::perlin(_pos * frequency);
            normalization += amplitude;
            amplitude *= _gain;
            frequency *= _lacunarity;
        }

        return value / normalization;
    }

    /* FBM normalized to [0...1]*/
    inline float NormalizedFBM(glm::vec2 _pos, int _octaves, float _lacunarity = 2.f, float _gain = 0.5f)
    {
        return FBM(_pos, _octaves, _lacunarity, _gain) * 0.5f + 0.5f;
    }

    /* Ridged noise - inverted abs(perlin), good for ridgers/rivers */
    inline float RidgedNoise(glm::vec2 _pos, int _octaves, float _lacunarity = 2.f, float _gain = 0.5f)
    {
        float value = 0.f;
        float amplitude = 1.f;
        float frequency = 1.f;
        float normalization = 0.f;

        for(int i = 0; i < _octaves; ++i)
        {
            float n = 1.f - std::abs(glm::perlin(_pos * frequency));
            n *= n;
            value += amplitude * n;
            normalization += amplitude;
            amplitude *= _gain;
            frequency *= _lacunarity;
        }

        return value / normalization;
    }

    /* Domain-warped FBM. Produces organic, swirling patterns. Return [0, 1]*/
    inline float WarpedNoise(glm::vec2 _pos, int _octaves, float _warpStrength = 0.5f)
    {
		const float wx = FBM(_pos, _octaves);
		const float wy = FBM(_pos + glm::vec2(5.2f, 1.3f), _octaves);
		return NormalizedFBM(_pos + glm::vec2(wx, wy) * _warpStrength, _octaves);
    }

    /* Simplex FBM. Lower directional artifacts compared to Perlin FBM */
    inline float SimplexFBM(glm::vec2 _pos, int _octaves, float _lacunarity = 2.f, float _gain = 0.5f)
    {
        float value = 0.f;
        float amplitude = 1.f;
        float frequency = 1.f;
        float normalization = 0.f;

        for(int i = 0; i < _octaves; ++i)
        {
            value += amplitude * glm::simplex(_pos * frequency);
            normalization += amplitude;
            amplitude *= _gain;
            frequency *= _lacunarity;
        }

        return value / normalization;
    }

    /* Billow noise - abs(perlin), puffy clouds/ soft hills.*/
    inline float BillowNoise(glm::vec2 _pos, int _octaves, float _lacunarity = 2.f, float _gain = 0.5f)
    {
        float value = 0.f;
        float amplitude = 1.f;
        float frequency = 1.f;
        float normalization = 0.f;

        for(int i = 0; i < _octaves; ++i)
        {
            value += amplitude * std::abs(glm::perlin(_pos * frequency));
            normalization += amplitude;
            amplitude *= _gain;
            frequency *= _lacunarity;
        }

        return value / normalization;
    }
}