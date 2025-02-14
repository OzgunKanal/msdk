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

#ifndef LIBRARIES_MAXUSB_INCLUDE_DEVCLASS_HID_RAW_H_
#define LIBRARIES_MAXUSB_INCLUDE_DEVCLASS_HID_RAW_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file  hid_raw.h
 * @brief Raw Human Interface Device Class over USB
 */

#include "hid.h"

/// Configuration structure
typedef struct {
  uint8_t in_ep;            // endpoint to be used for IN packets
  uint8_t in_maxpacket;     // max packet size for IN endpoint
  uint8_t out_ep;           // endpoint to be used for OUT packets
  uint8_t out_maxpacket;    // max packet size for OUT endpoint
} hid_cfg_t;

/** 
 *  \brief    Initialize the class driver
 *  \details  Initialize the class driver.
 *  \param    if_desc             pointer to interface descriptor
 *  \param    hid_descriptor      pointer to the descriptor to be used in response to getdescriptor requests
 *  \param    report_descriptor   pointer to the descriptor to be used in response to getdescriptor requests
 *  \return   Zero (0) for success, non-zero for failure
 */
int hidraw_init(const MXC_USB_interface_descriptor_t *if_desc, const hid_descriptor_t *hid_descriptor, const uint8_t *report_descriptor);

/** 
 *  \brief    Set the specified configuration
 *  \details  Configures the class and endpoint(s) and starts operation. This function should be
 *            called upon configuration from the host.
 *  \param    cfg   configuration to be set
 *  \return   Zero (0) for success, non-zero for failure
 */
int hidraw_configure(const hid_cfg_t *cfg);

/**
 *  \brief    Clear the current configuration and resets endpoint(s)
 *  \details  Clear the current configuration and resets endpoint(s).
 *  \return   Zero (0) for success, non-zero for failure
 */
int hidraw_deconfigure(void);

/**
 *  \brief    Register a callback to be called when read data is available.
 *  \details  Register a callback to be called when read data is available. To disable the
 *            callback, call this function with a NULL parameter.
 */
void hidraw_register_callback(int (*func)(void));

/**
 *  \brief    Get the number of bytes available to be read.
 *  \return   The number of bytes available to be read.
 */
int hidraw_canread(void);

/**
 *  \brief    Read the specified number of bytes.
 *  \details  Read the specified number of bytes. This function blocks until the specified
 *            number of bytes have been received.
 *  \param    buf   buffer to store the bytes in
 *  \param    len   number of bytes to read
 *  \return   number of bytes to read, non-zero for failure
 */
int hidraw_read(uint8_t *data, unsigned int len);

/**
 *  \brief    Write the specified number of bytes.
 *  \details  Write the specified number of bytes. The number of bytes must be less than or
 *            equal to the max packet size provided in hid_cfg_t.
 *  \param    data  buffer containing the data to be sent
 *  \param    len   number of bytes to write
 *  \return   Zero (0) for success, non-zero for failure
 *  \note     On some processors, the actually USB transaction is performed asynchronously, after
 *            this function returns. Successful return from this function does not guarantee
 *            successful reception of characters by the host.
 */
int hidraw_write(const uint8_t *data, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif //LIBRARIES_MAXUSB_INCLUDE_DEVCLASS_HID_RAW_H_
