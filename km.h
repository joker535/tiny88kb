/*
 * Based on Obdev's AVRUSB code and under the same license.
 *
 * TODO: Make a proper file header. :-)
 * Modified for Digispark by Digistump
 */
#ifndef __DigiKeyboard_h__
#define __DigiKeyboard_h__

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <string.h>

#include "usbdrv.h"

// TODO: Work around Arduino 12 issues better.
//#include <WConstants.h>
//#undef int()

typedef uint8_t byte;


#define BUFFER_SIZE 8 // Minimum of 2: 1 for modifiers + 1 for keystroke 


static uchar    idleRate;           // in 4 ms units 


/* We use a simplifed keyboard report descriptor which does not support the
 * boot protocol. We don't allow setting status LEDs and but we do allow
 * simultaneous key presses. 
 * The report descriptor has been created with usb.org's "HID Descriptor Tool"
 * which can be downloaded from http://www.usb.org/developers/hidpage/.
 * Redundant entries (such as LOGICAL_MINIMUM and USAGE_PAGE) have been omitted
 * for the second INPUT item.
 */
const PROGMEM char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = { /* USB report descriptor */
      0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
      0x09, 0x06,        // Usage (Keyboard)
      0xA1, 0x01,        // Collection (Application)
      0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
      0x19, 0xE0,        //   Usage Minimum (0xE0)
      0x29, 0xE7,        //   Usage Maximum (0xE7)
      0x15, 0x00,        //   Logical Minimum (0)
      0x25, 0x01,        //   Logical Maximum (1)
      0x95, 0x08,        //   Report Count (8)
      0x75, 0x01,        //   Report Size (1)
      0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
      0x95, 0x01,        //   Report Count (1)
      0x75, 0x08,        //   Report Size (8)
      0x81, 0x03,        //   Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
      0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
      0x19, 0x00,        //   Usage Minimum (0x00)
      0x29, 0xFF,        //   Usage Maximum (0xFF)
      0x15, 0x00,        //   Logical Minimum (0)
      0x26, 0xFF, 0x00,  //   Logical Maximum (255)
      0x95, 0x06,        //   Report Count (6)
      0x75, 0x08,        //   Report Size (8)
      0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
      0xC0,              // End Collection
};



#define MOD_BIT(code) (1 << MOD_INDEX(code))
#define MOD_INDEX(code) ((code)&0x07)

enum hid_keyboard_usage {
   /* Modifiers */
    KC_LCTRL = 0xE0,
    KC_LSHIFT,
    KC_LALT,
    KC_LGUI,
    KC_RCTRL,
    KC_RSHIFT,
    KC_RALT,
    KC_RGUI
};

class DigiKeyboardDevice {
 public:
  DigiKeyboardDevice () {
    cli();
    usbDeviceDisconnect();
    _delay_ms(250);
    usbDeviceConnect();


    usbInit();
      
    sei();

    // TODO: Remove the next two lines once we fix
    //       missing first keystroke bug properly.
    memset(reportBuffer, 0, sizeof(reportBuffer));      
    usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
  }
    
  void update() {
    usbPoll();
  }
  
  // delay while updating until we are finished delaying
  void delay(long milli) {
    unsigned long last = millis();
    while (milli > 0) {
      unsigned long now = millis();
      milli -= now - last;
      last = now;
      update();
    }
  }

  void record(void)
  {
    reportBuffer[0] = 0;
    reportBuffer[1] = 0;
    _index = 2;
  }



  void addKeyPress(uint8_t kc)
  {
    if(kc>=KC_LCTRL && kc<=KC_RGUI)
    {
      reportBuffer[0] |= MOD_BIT(kc);
    }else
    {
       if (reportBuffer[2] != kc && reportBuffer[3] != kc &&
          reportBuffer[4] != kc && reportBuffer[5] != kc &&
          reportBuffer[6] != kc && reportBuffer[7] != kc) {
      
             reportBuffer[_index++] = kc;
             if (_index == 8) {
               //TODO error         
             }
       }
    }
  }

  void report(void)
  {
    while(_index < 8)
    {
       reportBuffer[_index++] = 0;
    }
    while (!usbInterruptIsReady()) {
        // Note: We wait until we can send keyPress
        //       so we know the previous keyPress was
        //       sent.
        usbPoll();
        _delay_ms(5);
      }
      
      usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
  }
    
  //private: TODO: Make friend?
  uchar    reportBuffer[8];    // buffer for HID reports [ 1 modifier byte + (len-1) key strokes]
  uint8_t _index ;
};

DigiKeyboardDevice DigiKeyboard = DigiKeyboardDevice();

#ifdef __cplusplus
extern "C"{
#endif 
  // USB_PUBLIC uchar usbFunctionSetup
  uchar usbFunctionSetup(uchar data[8]) {
    usbRequest_t    *rq = (usbRequest_t *)((void *)data);

    usbMsgPtr = DigiKeyboard.reportBuffer; //
    if ((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {
      /* class request type */

      if (rq->bRequest == USBRQ_HID_GET_REPORT) {
        /* wValue: ReportType (highbyte), ReportID (lowbyte) */

        /* we only have one report type, so don't look at wValue */
        // TODO: Ensure it's okay not to return anything here?    
        return 0;

      } else if (rq->bRequest == USBRQ_HID_GET_IDLE) {
        //usbMsgPtr = &idleRate;
        //return 1;
        return 0;
        
      } else if (rq->bRequest == USBRQ_HID_SET_IDLE) {
        idleRate = rq->wValue.bytes[1];
        
      }
    } else {
      /* no vendor specific requests implemented */
    }
    
    return 0;
  }
#ifdef __cplusplus
} // extern "C"
#endif


#endif // __DigiKeyboard_h__
