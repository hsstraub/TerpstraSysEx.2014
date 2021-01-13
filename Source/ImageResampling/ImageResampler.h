/*
  ==============================================================================

    ImageResampler.h
    Author: https://forum.juce.com/u/jpo/
    Source: https://forum.juce.com/t/problems-with-downsampling-images/9408/4

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <vector>
#include "resampler.h"

static juce::Image resizeImage(const Image& src, int dst_width, int dst_height, const char* filter_name, float source_gamma)
{
    int n = 1;
    if (src.isRGB()) n = 3; else if (src.isARGB()) n = 4; else jassert(src.isSingleChannel());

    const int max_components = 4;
    int src_width = src.getWidth(), src_height = src.getHeight();

    if (std::max(src_width, src_height) > RESAMPLER_MAX_DIMENSION || n > max_components) {
        printf("Image is too large!\n");
        return Image();
    }

    Image dest(src.getFormat(), dst_width, dst_height, false);

    // Filter scale - values < 1.0 cause aliasing, but create sharper looking mips.
    const float filter_scale = 1.0f;//.75f;

    float srgb_to_linear[256];
    for (int i = 0; i < 256; ++i) {
        float v = i * 1.0f / 255.0f;
        srgb_to_linear[i] = (float)pow(v, source_gamma);
    }

    const int linear_to_srgb_table_size = 4096;
    unsigned char linear_to_srgb[linear_to_srgb_table_size];

    const float inv_linear_to_srgb_table_size = 1.0f / linear_to_srgb_table_size;
    const float inv_source_gamma = 1.0f / source_gamma;

    for (int i = 0; i < linear_to_srgb_table_size; ++i)
    {
        int k = (int)(255.0f * pow(i * inv_linear_to_srgb_table_size, inv_source_gamma) + .5f);
        if (k < 0) k = 0; else if (k > 255) k = 255;
        linear_to_srgb[i] = (unsigned char)k;
    }

    Resampler* resamplers[max_components];
    std::vector<float> samples[max_components];

    for (int i = 0; i < n; i++) {
        resamplers[i] = new Resampler(src_width, src_height, dst_width, dst_height,
            Resampler::Boundary_Op::BOUNDARY_CLAMP, 0.0f, 1.0f, filter_name,
            i == 0 ? 0 : resamplers[0]->get_clist_x(),
            i == 0 ? 0 : resamplers[0]->get_clist_y(), filter_scale, filter_scale);
        samples[i].resize(src_width);
    }

    Image::BitmapData bm_src(src, Image::BitmapData::readOnly);
    Image::BitmapData bm_dst(dest, Image::BitmapData::writeOnly);

    int dst_y = 0;

    //cerr << "Resampling to " << dst_width << “x” << dst_height << “\n”;

    for (int src_y = 0; src_y < src_height; src_y++) {
        const unsigned char* pSrc = bm_src.getLinePointer(src_y);
        for (int x = 0; x < src_width; x++) {
            if (n == 4) {
                PixelARGB& p = (PixelARGB&)(*pSrc);
                p.unpremultiply();
                samples[0][x] = srgb_to_linear[p.getBlue()];
                samples[1][x] = srgb_to_linear[p.getGreen()];
                samples[2][x] = srgb_to_linear[p.getRed()];
                samples[3][x] = p.getAlpha() * (1.0f / 255.0f);
                pSrc += 4;
            }
            else {
                for (int c = 0; c < n; c++) {
                    samples[c][x] = srgb_to_linear[*pSrc++];
                }
            }
        }

        for (int c = 0; c < n; c++) {
            if (!resamplers[c]->put_line(&samples[c][0])) {
                printf("Out of memory!\n");
                return Image();
            }
        }
        for (; ; ) {
            int c;
            for (c = 0; c < n; c++) {
                const float* pOutput_samples = resamplers[c]->get_line();
                if (!pOutput_samples)
                    break;

                const bool alpha_channel = (c == 3) || ((n == 2) && (c == 1));
                jassert(dst_y < dst_height);
                unsigned char* pDst = bm_dst.getLinePointer(dst_y) + c;

                for (int x = 0; x < dst_width; x++) {
                    if (alpha_channel) {
                        int c = (int)(255.0f * pOutput_samples[x] + .5f);
                        if (c < 0) c = 0; else if (c > 255) c = 255;
                        *pDst = (unsigned char)c;
                    }
                    else {
                        int j = (int)(linear_to_srgb_table_size * pOutput_samples[x] + .5f);
                        if (j < 0) j = 0; else if (j >= linear_to_srgb_table_size) j = linear_to_srgb_table_size - 1;
                        *pDst = linear_to_srgb[j];
                    }
                    pDst += n;
                }
            }
            if (c < n)
                break;

            if (n == 4) {
                PixelARGB* p = (PixelARGB*)bm_dst.getLinePointer(dst_y);
                for (int x = 0; x < dst_width; x++) {
                    p[x].premultiply();
                }
            }

            dst_y++;
        }
    }
    return dest;
}