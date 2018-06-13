/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.c
  * @version        : v2.0_Cube
  * @brief          : USB Device Custom HID interface file.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_custom_hid_if.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @addtogroup USBD_CUSTOM_HID
  * @{
  */

/** @defgroup USBD_CUSTOM_HID_Private_TypesDefinitions USBD_CUSTOM_HID_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Defines USBD_CUSTOM_HID_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Macros USBD_CUSTOM_HID_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Variables USBD_CUSTOM_HID_Private_Variables
  * @brief Private variables.
  * @{
  */

//Class specific descriptor - HID report descriptor:

//In order to understand the HID report descriptor, it is helpful to become familiar
//with the HID specifications (HID1_11.pdf was used to develop this example), and the
//"HUT" (HID Usage Tables) specifications (Hut1_12.pdf was used during development of this example).
//These documents can be found at the USB implementers forum website: www.usb.org

//Additionally, Microsoft provides a few reference documents pertaining to the development of
//HID touch/multi-touch devices for Windows Touch enabled operating systems.
//The below example HID report descriptor is almost identical to one of MicrosofHID_send_reportt's example
//descriptors.  Microsoft provides some example descriptors and background information in
//the following documents:

//"Digitizer Drivers for Windows Touch and Pen-Based Computers" (publicly available download: DigitizerDrvs_touch.docx)
//"Human Interface Device (HID) Extensions and Windows/Device Protocol for Multi-Touch Digitizers" (not
	//currently a public document[but might be in the future??], contact Microsoft if a
	//public version is unavailable.  However, this document isn't critical to developing
	//a touch/multi-touch device.  Most of the relevant content is already in the "Digitizer
	//Drivers for Windows Touch and Pen-Based Computers" document, and in Microsoft's developer
	//blogs relating to Windows 7 Touch)


//At a minimum a multi-touch capable finger input digitizer
//must support the following report information (one set of data for each simultaneous contact supported):
//"Tip Switch" (ex: a soft button, indicating that the tip of the human's finger is touching the screen)
//"In-range" indicator: For example, if the touch screen can detect when the user's finger is
//		hovering over an area, but not in direct contact (ex: it uses optical detection
//		methods), it may set the in-range indicator bit, but not necessarily simultanously
//		set the tip switch.  If a hard contact is detected, both the tip switch and in
//		range bits should be set.
//X coordinate of contact point (see note)
//Y coordinate of contact point (see note)
//Other input usages are optional.

//NOTE (X/Y Coordinates): The origin is the upper left corner of the screen.  For the X coordinates,
//the leftmost part of the screen has X coordinate = 0.  The rightmost edge of the screen has
//X coordinate = 4800 (for this example HID report descriptor).  The topmost part of the screen
//has Y coordinate = 0.  The bottom most part of the screen has Y coordinate = 3000 for this
//example HID report descriptor.

__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[] __ALIGN_END =
{
		//Below is the report descriptor for an example multi-touch device, which can support
		//up to two simultaneous contacts.  This report descriptor also supports multiple device modes.
		//The OS may select the device mode (ex: mouse, single-touch digitizer, multi-touch digitizer)
		//by sending a SET_REPORT (feature) control transfer.  In each mode, the device firmware
		//should behave differently and send input packets on the HID IN endpoint, formatted
		//based on the report descriptor collections associated with that mode.

		//Each IN packet (HID report) sent to the host (in multi-touch digitizer mode) is 14 bytes long (Bytes 0-13).
		//The data is sent in little endian format.  To send the report to the host, verify
		//that the HID IN endpoint buffer is not already busy, then place the data in the HID
		//IN endpoint buffer SRAM, and then call the HIDTxPacket() function.  The data will
		//then get sent to the host, at the next opportunity when the host polls that endpoint
		//(by sending an IN token packet to the endpoint).

		//NOTE (Contact identifier number): For a multi-touch device, the firmware needs to
		//keep track of each contact point separately and independantly.  For example, suppose
		//a human first presses one finger to the screen.  The firmware would detect this, and
		//it should arbitrarily assign a contact identifier number for this contact. Typically
		//it would be assigned something like contact ID = "1". As the human moves their finger
		//around on the contact surface the firmware will report new X and Y coordinate
		//information for this contact, but the contact ID byte should always be = "1" for
		//this contact.
		//
		//Now suppose the human places a second finger onto the touch surface, so there are
		//two simultanous contacts.  The firmware should assign this second contact a new
		//contact ID, for instance, contact ID = "2".  So long as the first contact is still
		//valid, the firmware needs to continue reporting ID = "1" for the first contact.
		//The firmware will continue to report information using these contact ID numbers
		//until such time as a contact becomes invalid (human removes their fingHID_send_reporter from the
		//screen).

		 //Top level collection for use in Multi-Touch Digitizer device mode.
			//-------------------------------------------------------------------
			//Format for multi-touch digitizer input reports using this report descriptor:
		    //Byte[0] = Report ID == MULTI_TOUCH_DATA_REPORT_ID
			//First contact point info in bytes 1-6.
			//Byte[1] = Bits7-3: pad bits (unused), Bit1:In Range, Bit0:Tip Switch
			//Byte[2] = Contact identifier number (see note above)
			//Byte[3] = X-coordinate LSB
			//Byte[4] = X-coordinate MSB
			//Byte[5] = Y-coordinate LSB
			//Byte[6] = Y-coordinate MSB

			//Second contact point info in bytes 7-12
			//Byte[7] = Bits7-3: pad bits (unused), Bit1:In Range, Bit0:Tip Switch
			//Byte[8] = Contact identifier number (see note above)
			//Byte[9] = X-coordinate LSB
			//Byte[10]= X-coordinate MSB
			//Byte[11]= Y-coordinate LSB
			//Byte[12]= Y-coordinate MSB

			//Byte[13]= 8-bit number indicating how many of the above contact points are valid.
			//			If only the first contact is valid, send "1" here.  If both are valid, send "2".

			//HID Report descriptor for an example Multi-Touch (2 simultaneous contacts), multi-modes device.
			//------------------------------------------------------------------------------------------------
			//Note: In a real application, at a minimum, certain terms in the report descriptor
			//(ex: UNIT, UNIT_EXPONENT, PHYSICAL_MAXIMUM and LOGICAL_MAXIMUM) will need to be modified to
			//match the characteristics (ex: size) of the actual application being developed.  See the HID1_11.pdf
			//specifications regarding these terms.
		0x05, 0x0D,	          // USAGE_PAGE (Digitizers)
		    0x09, 0x04,	          // USAGE (Touch Screen)
		    0xA1, 0x01,           // COLLECTION (Application)
		    0x85, MULTI_TOUCH_DATA_REPORT_ID,	//   REPORT_ID (Touch)    //When the firmware wants to send a HID report (containing multi-touch contact information), the Report ID byte should match this number
		    0x09, 0x22,           //   USAGE (Finger)HID_send_report
		    0xA1, 0x02,           //   COLLECTION (Logical)
		    0x09, 0x42,           //       USAGE (Tip Switch)
		    0x15, 0x00,           //       LOGICAL_MINIMUM (0)
		    0x25, 0x01,           //       LOGICAL_MAXIMUM (1)
		    0x75, 0x01,           //       REPORT_SIZE (1)
		    0x95, 0x01,           //       REPORT_COUNT (1)
		    0x81, 0x02,           //       INPUT (Data,Var,Abs) 		//Makes one, 1-bit field for tip switch
		    0x09, 0x32,	          //       USAGE (In Range)
		    0x81, 0x02,           //       INPUT (Data,Var,Abs)         //Makes one, 1-bit field for In Range indicator
		    0x95, 0x06,           //       REPORT_COUNT (6)
		    0x81, 0x03,           //       INPUT (Cnst,Ary,Abs)			//Makes six, 1-bit fields, which are pad bits (no valid data)
		    0x75, 0x08,           //       REPORT_SIZE (8)
		    0x09, 0x51,           //       USAGE (Contact Identifier)
		    0x95, 0x01,           //       REPORT_COUNT (1)
		    0x81, 0x02,           //       INPUT (Data,Var,Abs) 		//Makes one, 8-bit field for the contact identifier number.  Firmware arbitrarily assigns a contact ID, which stays the same until the contact becomes invalid (finger removed from screen).
		    0x05, 0x01,           //       USAGE_PAGE (Generic Desk..
		    0x26, 0xC0, 0x12,     //       LOGICAL_MAXIMUM (4800)   	//This is the maximum value the firmware should send for the X coordinate contact point.  It can potentially make the firmware design mathematically simpler if this is set to some meaningful value, such as equal to the PHYSICAL_MAXIMUM (which specifies the dimensions of the actual touch screen, in terms of the UNITS parameter).
		    0x75, 0x10,           //       REPORT_SIZE (16)
		    0x55, 0x0E,           //       UNIT_EXPONENT (-2)       //10^(-2)
		    0x65, 0x33,           //       UNIT (Inches, English Linear)  //But exponent -2, so Physical Maximum is in 10’s of mils.
		    0x09, 0x30,           //       USAGE (X)
		    0x35, 0x00,           //       PHYSICAL_MINIMUM (0)
		    0x46, 0x40, 0x06,     //       PHYSICAL_MAXIMUM (0x640 = 1600)     //1600 * 10^(-2) = 16 inches X-dimension
		    0x81, 0x02,           //       INPUT (Data,Var,Abs)           //Makes one, 16-bit field for X coordinate info.  Valid values from: 0-4800 (4800 is the LOGICAL_MAXIMUM, which would correspond to far edge of the screen, which is 1600x10mil distance from X origin [left of screen])
		    0x26, 0xB8, 0x0B,     //       LOGICAL_MAXIMUM (3000)             //16:10 aspect ratio (X:Y) (ex: 4800/3000 = 16/10).  //This is the maximum value the firmware should send for the Y coordinate contact point.  It can potentially make the firmware design mathematically simpler if this is set to some meaningful value, such as equal to the PHYSICAL_MAXIMUM (which specifies the dimensions of the actual touch screen, in terms of the UNIT parameter).
		    0x46, 0xE8, 0x03,     //       PHYSICAL_MAXIMUM (0x3E8 = 1000)    //1000 * 10^(-2) = 10 inches Y-dimension
		    0x09, 0x31,           //       USAGE (Y)
		    0x81, 0x02,           //       INPUT (Data,Var,Abs)			//Makes one, 16-bit field for Y coordinate info.  Valid values from: 0-3000 (3000 is the LOGICAL_MAIXMUM, which would correspond to the far bottom of the screen, which is 1000x10mil distance from the Y origin [top of screen])
		    0xC0,                 //    END_COLLECTION
			0xA1, 0x02,           //    COLLECTION (Logical)
			0x05, 0x0D,	          //       USAGE_PAGE (Digitizers)
			0x09, 0x42,           //       USAGE (Tip Switch)
			0x15, 0x00,           //       LOGICAL_MINIMUM (0)
			0x25, 0x01,           //       LOGICAL_MAXIMUM (1)
			0x75, 0x01,           //       REPORT_SIZE (1)
			0x95, 0x01,           //       REPORT_COUNT (1)
			0x81, 0x02,           //       INPUT (Data,Var,Abs) 		//Makes one, 1-bit field for Tip Switch
			0x09, 0x32,	          //       USAGE (In Range)
			0x81, 0x02,           //       INPUT (Data,Var,Abs)         //Makes one, 1-bit field for In Range Indicator
			0x95, 0x06,           //       REPORT_COUNT (6)
			0x81, 0x03,           //       INPUT (Cnst,Ary,Abs)			//Makes six, 1-bit fields that are pad bits (no valid data)
			0x75, 0x08,           //       REPORT_SIZE (8)
			0x09, 0x51,           //       USAGE (Contact Identifier)
			0x95, 0x01,           //       REPORT_COUNT (1)
			0x81, 0x02,           //       INPUT (Data,Var,Abs)			//Makes one, 8-bit field for the contact identifier number.
			0x05, 0x01,           //       USAGE_PAGE (Generic Desk..
			0x26, 0xC0, 0x12,     //       LOGICAL_MAXIMUM (4800)
			0x75, 0x10,           //       REPORT_SIZE (16)
			0x55, 0x0E,           //       UNIT_EXPONENT (-2)       //10^(-2)
			0x65, 0x33,           //       UNIT (Inches, English Linear)  //But exponent -2, so Physical Maximum is in 10’s of mils.
			0x09, 0x30,           //       USAGE (X)
			0x35, 0x00,           //       PHYSICAL_MINIMUM (0)
			0x46, 0x40, 0x06,     //       PHYSICAL_MAXIMUM (0x640 = 1600)     //1600 * 10^(-2) = 16 inches X-dimension
			0x81, 0x02,           //       INPUT (Data,Var,Abs)           //Makes one, 16-bit field for X coordinate info.  Valid values from: 0-4800
			0x26, 0xB8, 0x0B,     //       LOGICAL_MAXIMUM (3000)             //16:10 aspect ratio (X:Y)
			0x46, 0xE8, 0x03,     //       PHYSICAL_MAXIMUM (0x3E8 = 1000)    //1000 * 10^(-2) = 10 inches Y-dimension
			0x09, 0x31,           //       USAGE (Y)
			0x81, 0x02,           //       INPUT (Data,Var,Abs)                 //Makes one, 16-bit field for Y coordinate info.  Valid values from: 0-3000
			0xC0,                 //   END_COLLECTION
			0xA1, 0x02,           //   COLLECTION (Logical)
			0x05, 0x0D,	          //       USAGE_PAGE (Digitizers)
			0x09, 0x42,           //       USAGE (Tip Switch)
			0x15, 0x00,           //       LOGICAL_MINIMUM (0)
			0x25, 0x01,           //       LOGICAL_MAXIMUM (1)
			0x75, 0x01,           //       REPORT_SIZE (1)
			0x95, 0x01,           //       REPORT_COUNT (1)
			0x81, 0x02,           //       INPUT (Data,Var,Abs) 		//Makes one, 1-bit field for Tip Switch
			0x09, 0x32,	          //       USAGE (In Range)
			0x81, 0x02,           //       INPUT (Data,Var,Abs)         //Makes one, 1-bit field for In Range Indicator
			0x95, 0x06,           //       REPORT_COUNT (6)
			0x81, 0x03,           //       INPUT (Cnst,Ary,Abs)			//Makes six, 1-bit fields that are pad bits (no valid data)
			0x75, 0x08,           //       REPORT_SIZE (8)
			0x09, 0x51,           //       USAGE (Contact Identifier)
			0x95, 0x01,           //       REPORT_COUNT (1)
			0x81, 0x02,           //       INPUT (Data,Var,Abs)			//Makes one, 8-bit field for the contact identifier number.
			0x05, 0x01,           //       USAGE_PAGE (Generic Desk..
			0x26, 0xC0, 0x12,     //       LOGICAL_MAXIMUM (4800)
			0x75, 0x10,           //       REPORT_SIZE (16)
			0x55, 0x0E,           //       UNIT_EXPONENT (-2)       //10^(-2)
			0x65, 0x33,           //       UNIT (Inches, English Linear)  //But exponent -2, so Physical Maximum is in 10’s of mils.
			0x09, 0x30,           //       USAGE (X)
			0x35, 0x00,           //       PHYSICAL_MINIMUM (0)
			0x46, 0x40, 0x06,     //       PHYSICAL_MAXIMUM (0x640 = 1600)     //1600 * 10^(-2) = 16 inches X-dimension
			0x81, 0x02,           //       INPUT (Data,Var,Abs)           //Makes one, 16-bit field for X coordinate info.  Valid values from: 0-4800
			0x26, 0xB8, 0x0B,     //       LOGICAL_MAXIMUM (3000)             //16:10 aspect ratio (X:Y)
			0x46, 0xE8, 0x03,     //       PHYSICAL_MAXIMUM (0x3E8 = 1000)    //1000 * 10^(-2) = 10 inches Y-dimension
			0x09, 0x31,           //       USAGE (Y)
			0x81, 0x02,           //       INPUT (Data,Var,Abs)                 //Makes one, 16-bit field for Y coordinate info.  Valid values from: 0-3000
			0xC0,                 //   END_COLLECTION
			0x05, 0x0D,	          //   USAGE_PAGE (Digitizers)
			0x09, 0x54,	          //   USAGE (Actual (contact) count)
			0x95, 0x01,           //   REPORT_COUNT (1)
			0x75, 0x08,           //   REPORT_SIZE (8)
			0x25, MAX_VALID_CONTACT_POINTS,  //   LOGICAL_MAXIMUM (3)				//Maximum number of valid contact points simutaneously supported.  Increase this number if supporting more than 3 simultaneous contacts.
			0x81, 0x02,           //   INPUT (Data,Var,Abs)				//Makes one, 8-bit field for the actual number of valid contacts reported (valid values: 1 and 2)

			//In addition to standard HID reports (containing contact X/Y/tip-switch/in-range
			//information), the firmware can send feature report information to the host.
			//Feature reports lets the host know information about the firmware/hardware
			//application design.  The host will typically request a feature report by sending
			//a GET_REPORT control transfer, with the bRequest = REPORT, but with wIndex (MSB) = 0x03
			//(get feature report request as indicated in the HID1_11.pdf specifications). If a
			//feature report is sent to the host, it should be sent as a 2-byte packet (for
			//this example project), formatted like follows:
			//Byte[0]= Report ID == VALID_CONTACTS_FEATURE_REPORT_ID == Feature report ID
			//Byte[1]= Maximum number of contacts simultaneously supported by this application.  Always == 2 in this demo, since this report descriptor only has input fields for 2 contact points
			//See the UserGetReportHandler() function that sends this type of packet in the MultiTouch.c file.
			0x85, VALID_CONTACTS_FEATURE_REPORT_ID,  //   REPORT_ID (Feature)   //When the firmware wants to send a feature report (containing the number of valid contacts that can be reported), the Report ID byte should match this number.
			0x09, 0x55,           //   USAGE(Maximum Count)
			0xB1, 0x02,           //   FEATURE (Data,Var,Abs) 			//Makes one, 8-bit field for reporting the maximum number of simultaneous contacts supported by the device.  Since this report descriptor contains two logical collections (one for each contact point), the firmware should always send 0x02 in response to a get feature report request.
			0xC0,                 // END_COLLECTION


			//Top level collection describing the feature report that the host will use (SET_REPORT (feature) control transfer) to set the device mode (ex: mouse, single-touch digitizer, multi-touch digitizer)
			//-------------------------------------------------------------------
			//Format of the feature report output data that the host will send to the device (when it wants to change the device mode):
			//byte[0] = Report ID = DEVICE_MODE_FEATURE_REPORT_ID
			//byte[1] = Device Mode (that the device should switch into)
			//byte[2] = Device Identifier
			0x09, 0x0E,           // USAGE (Device Configuration)
			0xa1, 0x01,           // COLLECTION (Application)
			0x85, DEVICE_MODE_FEATURE_REPORT_ID,     //   REPORT_ID (Configuration)
			0x09, 0x23,           //   USAGE (Device Settings)
			0xa1, 0x02,           //   COLLECTION (logical)
			0x09, 0x52,           //    USAGE (Device Mode)
			0x09, 0x53,           //    USAGE (Device Identifier)
			0x15, 0x00,           //    LOGICAL_MINIMUM (0)
			0x25, 0x0a,           //    LOGICAL_MAXIMUM (10)
			0x75, 0x08,           //    REPORT_SIZE (8)
			0x95, 0x02,           //    REPORT_COUNT (2)
			0xb1, 0x02,           //   FEATURE (Data,Var,Abs)
			0xc0,                 //   END_COLLECTION (logical)
			0xc0,                 // END_COLLECTION (application)


			//Top level collection for use in Mouse device mode.
			//-------------------------------------------------------------------
			//Format for mouse input reports using this report descriptor:
			//byte[0] = Report ID = MOUSE_DATA_REPORT_ID
			//byte[1] = contains bit fields for various input information typically generated by mouse buttons or touch pad tap operations. '1' is the active value (ex: pressed), '0' is the non active value
			//		bit0 = Button 1.
			//		bit1 = Button 2.
			//		bit2 = Button 3.
			//		bits3-7 = pad bits, no valid data.
			//byte[2] = Relative X coordinate change since the last report was sent
			//byte[3] = Relative Y coordinate change since the last report was sent
			0x05, 0x01, // Usage Page (Generic Desktop)
			0x09, 0x02, // Usage (Mouse)
			0xA1, 0x01, // Collection (Application)
			0x85, MOUSE_DATA_REPORT_ID, //  REPORT_ID (mouse)
			0x09, 0x01, //  Usage (Pointer)
			0xA1, 0x00, //  Collection (Physical)
			0x05, 0x09, //      Usage Page (Buttons)
			0x19, 0x01, //      Usage Minimum (01)
			0x29, 0x03, //      Usage Maximum (03)
			0x15, 0x00, //      Logical Minimum (0)
			0x25, 0x01, //      Logical Maximum (1)
			0x45, 0x00, //      Physical Maximum (0)
			0x65, 0x00, //      Unit (none)
			0x95, 0x03, //      Report Count (3)
			0x75, 0x01, //      Report Size (1)
			0x81, 0x02, //      Input (Data, Variable, Absolute)
			0x95, 0x01, //      Report Count (1)
			0x75, 0x05, //      Report Size (5)
			0x81, 0x01, //      Input (Constant)    ;5 bit padding
			0x05, 0x01, //      Usage Page (Generic Desktop)
			0x09, 0x30, //      Usage (X)
			0x09, 0x31, //      Usage (Y)
			0x15, 0x81, //      Logical Minimum (-127)
			0x25, 0x7F, //      Logical Maximum (127)
			0x75, 0x08, //      Report Size (8)
			0x95, 0x02, //      Report Count (2)
			0x81, 0x06, //      Input (Data, Variable, Relative)
			0xC0, 		//  End Collection
			0xC0,  	    // End Collection

			//Top level collection for use in Single-Touch Digitizer device mode.
			//-------------------------------------------------------------------
			//Format for single-touch digitizer input reports using this report descriptor:
			//byte[0] = Report ID = SINGLE_TOUCH_DATA_REPORT_ID
			//byte[1] = contains bit fields for various input information typically generated by an input pen or human finger. '1' is the active value (ex: pressed), '0' is the non active value
			//		bit0 = Tip switch. At the end of a pen input device would normally be a pressure senstive switch.  Asserting this performs an operation analogous to a "left click" on a mouse
			//		bit2 = In range indicator.
			//		bit3 though bit 7 = Pad bits.  Values not used for anything.
			//byte[2] = Pad byte.  Value not used for anything.
			//byte[3] = X coordinate LSB value of contact point
			//byte[4] = X coordinate MSB value of contact point
			//byte[5] = Y coordinate LSB value of contact point
			//byte[6] = Y coordinate MSB value of contact point
			0x05, 0x0D,             // USAGE_PAGE (Digitizers)
			0x09, 0x02,             // USAGE (Pen)
			0xA1, 0x01,             // COLLECTION (Application)
			0x85, SINGLE_TOUCH_DATA_REPORT_ID, 	//   REPORT_ID (Pen digitizer/single touch)  //To send a HID input report containing single-touch digitizer data, use the SINGLE_TOUCH_DATA_REPORT_ID report ID
			0x09, 0x20,             //   USAGE (Stylus)
			0xA1, 0x00,             //   COLLECTION (Physical)
			0x09, 0x42, 			//     USAGE (Tip Switch)           //(byte[1] bit 0)
			0x09, 0x32, 			//     USAGE (In Range)             //(byte[1] bit 1)
			0x15, 0x00,             //     LOGICAL_MINIMUM (0)
			0x25, 0x01,             //     LOGICAL_MAXIMUM (1)
			0x75, 0x01,             //     REPORT_SIZE (1)              //1-bit
			0x95, 0x02,             //     REPORT_COUNT (2)             //two
			0x81, 0x02, 			//     INPUT (Data,Var,Abs)         //Makes two, 1-bit IN packet fields (byte[1] bits 0-4)) for (USAGE) tip sw, barrel sw, invert sw, in range sw.  Send '1' here when switch is active.  Send '0' when switch not active.
			0x95, 0x0E, 			//     REPORT_COUNT (14)            //fourteen
			0x81, 0x03, 			//     INPUT (Cnst,Var,Abs)         //Makes fourteen, 1-bit IN packet fields (byte[1] bits 5-7, and byte[2] all bits) with no usage.  These are pad bits that don't contain useful data.
			0x05, 0x01,             //     USAGE_PAGE (Generic Desktop)
			0x26, 0xC0, 0x12,     //       LOGICAL_MAXIMUM (4800)
			0x75, 0x10,           //       REPORT_SIZE (16)
			0x95, 0x01,             //     REPORT_COUNT (1)   			//one
			0x55, 0x0E,           //       UNIT_EXPONENT (-2)       //10^(-2)
			0x65, 0x33,           //       UNIT (Inches, English Linear)  //But exponent -2, so Physical Maximum is in 10’s of mils.
			0x09, 0x30,           //       USAGE (X)
			0x35, 0x00,           //       PHYSICAL_MINIMUM (0)
			0x46, 0x40, 0x06,     //       PHYSICAL_MAXIMUM (0x640 = 1600)     //1600 * 10^(-2) = 16 inches X-dimension
			0x81, 0x02,           //       INPUT (Data,Var,Abs)           //Makes one, 16-bit field for X coordinate info.  Valid values from: 0-4800
			0x26, 0xB8, 0x0B,     //       LOGICAL_MAXIMUM (3000)             //16:10 aspect ratio (X:Y)
			0x46, 0xE8, 0x03,     //       PHYSICAL_MAXIMUM (0x3E8 = 1000)    //1000 * 10^(-2) = 10 inches Y-dimension
			0x09, 0x31,           //       USAGE (Y)
			0x81, 0x02,             //     INPUT (Data,Var,Abs)		    //Makes one, 16-bit field for Y coordinate info.  Valid values from: 0-3000
			0xC0,                   //   END_COLLECTION (Physical)
			0xC0                    // END_COLLECTION (Application)
};

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Variables USBD_CUSTOM_HID_Exported_Variables
  * @brief Public variables.
  * @{
  */
extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */
/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_FunctionPrototypes USBD_CUSTOM_HID_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CUSTOM_HID_Init_FS(void);
static int8_t CUSTOM_HID_DeInit_FS(void);
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state);

/**
  * @}
  */

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS =
{
  CUSTOM_HID_ReportDesc_FS,
  CUSTOM_HID_Init_FS,
  CUSTOM_HID_DeInit_FS,
  CUSTOM_HID_OutEvent_FS
};

/** @defgroup USBD_CUSTOM_HID_Private_Functions USBD_CUSTOM_HID_Private_Functions
  * @brief Private functions.
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  DeInitializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit_FS(void)
{
  /* USER CODE BEGIN 5 */
  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Manage the CUSTOM HID class events
  * @param  event_idx: Event index
  * @param  state: Event state
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state)
{
  /* USER CODE BEGIN 6 */
  return (USBD_OK);
  /* USER CODE END 6 */
}

/* USER CODE BEGIN 7 */
/**
  * @brief  Send the report to the Host
  * @param  report: The report to be sent
  * @param  len: The report length
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */

static int8_t USBD_CUSTOM_HID_SendReport_FS(uint8_t *report, uint16_t len)
{
  return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report, len);
}

/* USER CODE END 7 */

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

