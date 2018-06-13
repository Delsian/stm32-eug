/**
  ******************************************************************************
  * @file    usbd_customhid_if_template.c
  * @author  MCD Application Team
  * @version V2.4.2
  * @date    11-December-2015
  * @brief   USB Device Custom HID interface file.
  *		     This template should be copied to the user folder, renamed and customized
  *          following user needs.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_customhid_if_template.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static int8_t TEMPLATE_CUSTOM_HID_Init     (void);
static int8_t TEMPLATE_CUSTOM_HID_DeInit   (void);
static int8_t TEMPLATE_CUSTOM_HID_OutEvent (uint8_t event_idx, uint8_t state);

__ALIGN_BEGIN static uint8_t TEMPLATE_CUSTOM_HID_ReportDesc[182] __ALIGN_END = {
    0x05, 0x0d,                    // USAGE_PAGE (Digitizers)
    0x09, 0x01,                    // USAGE (Digitizer)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x01,                    //   REPORT_ID (1)
    0x09, 0x21,                    //   USAGE (Puck)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x75, 0x10,                    //     REPORT_SIZE (16)
    0x95, 0x02,                    //     REPORT_COUNT (2)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xe0, 0x2e,              //     LOGICAL_MAXIMUM (12000)
    0x35, 0x00,                    //     PHYSICAL_MINIMUM (0)
    0x45, 0x0c,                    //     PHYSICAL_MAXIMUM (12)
    0x65, 0x13,                    //     UNIT (Eng Lin:Distance)
    0x55, 0x00,                    //     UNIT_EXPONENT (0)
    0xa4,                          //     PUSH
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x05, 0x0d,                    //     USAGE_PAGE (Digitizers)
    0x09, 0x32,                    //     USAGE (In Range)
    0x09, 0x44,                    //     USAGE (Barrel Switch)
    0x09, 0x42,                    //     USAGE (Tip Switch)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x35, 0x00,                    //     PHYSICAL_MINIMUM (0)
    0x45, 0x01,                    //     PHYSICAL_MAXIMUM (1)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x65, 0x00,                    //     UNIT (None)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x75, 0x05,                    //     REPORT_SIZE (5)
    0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
    0xc0,                          //   END_COLLECTION
    0x85, 0x02,                    //   REPORT_ID (2)
    0x09, 0x20,                    //   USAGE (Stylus)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0xb4,                          //     POP
    0xa4,                          //     PUSH
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x05, 0x0d,                    //     USAGE_PAGE (Digitizers)
    0x09, 0x32,                    //     USAGE (In Range)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x35, 0x00,                    //     PHYSICAL_MINIMUM (0)
    0x45, 0x01,                    //     PHYSICAL_MAXIMUM (1)
    0x65, 0x00,                    //     UNIT (None)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x00,                    //     USAGE_MINIMUM (No Buttons Pressed)
    0x29, 0x10,                    //     USAGE_MAXIMUM (Button 16)
    0x25, 0x10,                    //     LOGICAL_MAXIMUM (16)
    0x75, 0x05,                    //     REPORT_SIZE (5)
    0x81, 0x40,                    //     INPUT (Data,Ary,Abs,Null)
    0x75, 0x02,                    //     REPORT_SIZE (2)
    0x81, 0x01,                    //     INPUT (Cnst,Ary,Abs)
    0xc0,                          //   END_COLLECTION
    0x85, 0x03,                    //   REPORT_ID (3)
    0x05, 0x0d,                    //   USAGE_PAGE (Digitizers)
    0x09, 0x20,                    //   USAGE (Stylus)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0xb4,                          //     POP
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x05, 0x0d,                    //     USAGE_PAGE (Digitizers)
    0x09, 0x32,                    //     USAGE (In Range)
    0x09, 0x44,                    //     USAGE (Barrel Switch)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x35, 0x00,                    //     PHYSICAL_MINIMUM (0)
    0x45, 0x01,                    //     PHYSICAL_MAXIMUM (1)
    0x65, 0x00,                    //     UNIT (None)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x95, 0x06,                    //     REPORT_COUNT (6)
    0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
    0x09, 0x30,                    //     USAGE (Tip Pressure)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
    0x35, 0x00,                    //     PHYSICAL_MINIMUM (0)
    0x45, 0x2d,                    //     PHYSICAL_MAXIMUM (45)
    0x67, 0x11, 0xe1, 0x00, 0x00,  //     UNIT (SI Lin:Force)
    0x55, 0x04,                    //     UNIT_EXPONENT (4)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x81, 0x12,                    //     INPUT (Data,Var,Abs,NLin)
    0xc0,                          //   END_COLLECTION
    0xc0                           // END_COLLECTION
};

/* Private variables ---------------------------------------------------------*/
USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_template_fops = 
{
  TEMPLATE_CUSTOM_HID_ReportDesc,
  TEMPLATE_CUSTOM_HID_Init,
  TEMPLATE_CUSTOM_HID_DeInit,
  TEMPLATE_CUSTOM_HID_OutEvent,
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  TEMPLATE_CUSTOM_HID_Init
  *         Initializes the CUSTOM HID media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t TEMPLATE_CUSTOM_HID_Init(void)
{

  return (0);
}

/**
  * @brief  TEMPLATE_CUSTOM_HID_DeInit
  *         DeInitializes the CUSTOM HID media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t TEMPLATE_CUSTOM_HID_DeInit(void)
{
  /*
     Add your deinitialization code here 
  */  
  return (0);
}


/**
  * @brief  TEMPLATE_CUSTOM_HID_Control
  *         Manage the CUSTOM HID class events       
  * @param  event_idx: event index
  * @param  state: event state
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t TEMPLATE_CUSTOM_HID_OutEvent  (uint8_t event_idx, uint8_t state)
{ 

  return (0);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
