/******************************************************************************
 *
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. (now owned by 
 * Analog Devices, Inc.),
 * Copyright (C) 2023-2024 Analog Devices, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#ifndef LIBRARIES_MISCDRIVERS_CAMERA_DEBAYERING_H_
#define LIBRARIES_MISCDRIVERS_CAMERA_DEBAYERING_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Return an x coordinate given an array index and x resolution
unsigned int _x(unsigned int i, unsigned int xres);

// Return a y coordinate given an array index and x resolution
unsigned int _y(unsigned int i, unsigned int xres);

// Return an array index given an x and y coordinate and an x resolution
// This function also enforces array boundaries.
unsigned int _i(unsigned int x, unsigned int y, unsigned int xres, unsigned int yres);

// Convert an RGB value to RGB565
uint16_t rgb_to_rgb565(uint8_t r, uint8_t g, uint8_t b);

// Clamp a float to uint8_t
uint8_t clamp_f_u8(float val);

// Clamp an int to uint8_t
uint8_t clamp_i_u8(int val);

/**
* @brief Apply color correction to a RAW8 bayer pattern image using the "gray world" assumption. (E. Y. Lam,
* “Combining gray world and retinex theory for automatic
* white balance in digital photography,” in Proc. 9th IEEE Intl. Symposium
* on Comsumer Electronics, 2005, pp. 134–139.).  Call this function before debayering to improve color accuracy.
* @param srcimg Pointer to the RAW8 bayer pattern.  The image will be modified in-place.
* @param[in] w Width of the bayer pattern (in pixels)
* @param[in] h Height of the bayer pattern (in pixels)
****************************************************************************/
void color_correct(uint8_t *srcimg, unsigned int w, unsigned int h);

/**
* @brief Formulate a bayer "passthrough" image that splits an HM0360 bayer pattern into its RGB channels while preserving the bayer pattern.  Useful for debugging and demosaicing algorithm development.
* @param[in] srcimg Pointer to the raw bayer pattern
* @param[in] w Width of the bayer pattern (in pixels)
* @param[in] h Height of the bayer pattern (in pixels)
* @param[out] dstimg Output pointer for converted RGB565 image.
****************************************************************************/
void bayer_passthrough(uint8_t *srcimg, uint32_t w, uint32_t h, uint16_t *dstimg);

/**
* @brief Debayer a RAW8 bayer-patterned image array and convert to RGB565.
* @param[in] srcimg Pointer to the raw bayer pattern
* @param[in] w Width of the bayer pattern (in pixels)
* @param[in] h Height of the bayer pattern (in pixels)
* @param[out] dstimg Output pointer for converted RGB565 image.
****************************************************************************/
void bayer_bilinear_demosaicing(uint8_t *srcimg, uint32_t w, uint32_t h, uint16_t *dstimg);

/**
 * @brief Color-correct and debayer a sub-section of a RAW8 bayer-patterned source image.
 *        This function is used to crop images on the fly.
 *
 * @param[in] srcimg     Pointer to the raw bayer pattern.
 * @param[in] src_width  Width of the entire source image (in pixels).
 * @param[in] src_height Height of the entire source image (in pixels).
 * @param[in] w_offset   Horizontal offset of the sub-section within the source image (in pixels).
 * @param[in] h_offset   Vertical offset of the sub-section within the source image (in pixels).
 * @param[out] dstimg     Output pointer for the converted RGB565 image.
 * @param[in] dst_width  Width of the output image (in pixels).
 * @param[in] dst_height Height of the output image (in pixels).
 ****************************************************************************/
void bayer_bilinear_demosaicing_crop(uint8_t *srcimg, uint32_t src_width, uint32_t src_height,
                                     uint32_t w_offset, uint32_t h_offset, uint16_t *dstimg,
                                     uint32_t dst_width, uint32_t dst_height);

#ifdef __cplusplus
}
#endif

#endif // LIBRARIES_MISCDRIVERS_CAMERA_DEBAYERING_H_
