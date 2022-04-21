///*
// * Based on Obdev's AVRUSB code and under the same license.
// *
// * TODO: Make a proper file header. :-)
// * Modified for Digispark by Digistump
// */
//#ifndef __DigiKeyboard_h__
//#define __DigiKeyboard_h__
//
//#include <Arduino.h>
//#include <avr/pgmspace.h>
//#include <avr/interrupt.h>
//#include <avr/delay.h>
//#include <string.h>
//
//#include "usbdrv.h"
//
//// TODO: Work around Arduino 12 issues better.
////#include <WConstants.h>
////#undef int()
//
//typedef uint8_t byte;
//
//
//#define BUFFER_SIZE 8 // Minimum of 2: 1 for modifiers + 1 for keystroke 
//
//
//static uchar    idleRate;           // in 4 ms units 
//
//
///* We use a simplifed keyboard report descriptor which does not support the
// * boot protocol. We don't allow setting status LEDs and but we do allow
// * simultaneous key presses. 
// * The report descriptor has been created with usb.org's "HID Descriptor Tool"
// * which can be downloaded from http://www.usb.org/developers/hidpage/.
// * Redundant entries (such as LOGICAL_MINIMUM and USAGE_PAGE) have been omitted
// * for the second INPUT item.
// */
//const PROGMEM char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = { /* USB report descriptor */
//      0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
//      0x09, 0x06,        // Usage (Keyboard)
//      0xA1, 0x01,        // Collection (Application)
//      0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
//      0x19, 0xE0,        //   Usage Minimum (0xE0)
//      0x29, 0xE7,        //   Usage Maximum (0xE7)
//      0x15, 0x00,        //   Logical Minimum (0)
//      0x25, 0x01,        //   Logical Maximum (1)
//      0x95, 0x08,        //   Report Count (8)
//      0x75, 0x01,        //   Report Size (1)
//      0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
//      0x95, 0x01,        //   Report Count (1)
//      0x75, 0x08,        //   Report Size (8)
//      0x81, 0x03,        //   Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
//      0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
//      0x19, 0x00,        //   Usage Minimum (0x00)
//      0x29, 0xFF,        //   Usage Maximum (0xFF)
//      0x15, 0x00,        //   Logical Minimum (0)
//      0x26, 0xFF, 0x00,  //   Logical Maximum (255)
//      0x95, 0x06,        //   Report Count (6)
//      0x75, 0x08,        //   Report Size (8)
//      0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
//      0xC0,              // End Collection
//};
//
//
//
//#define MOD_BIT(code) (1 << MOD_INDEX(code))
//#define MOD_INDEX(code) ((code)&0x07)
//
//enum hid_keyboard_usage {
//    KC_NO = 0x00,
//    KC_ROLL_OVER,
//    KC_POST_FAIL,
//    KC_UNDEFINED,
//    KC_A,
//    KC_B,
//    KC_C,
//    KC_D,
//    KC_E,
//    KC_F,
//    KC_G,
//    KC_H,
//    KC_I,
//    KC_J,
//    KC_K,
//    KC_L,
//    KC_M,  // 0x10
//    KC_N,
//    KC_O,
//    KC_P,
//    KC_Q,
//    KC_R,
//    KC_S,
//    KC_T,
//    KC_U,
//    KC_V,
//    KC_W,
//    KC_X,
//    KC_Y,
//    KC_Z,
//    KC_1,
//    KC_2,
//    KC_3,  // 0x20
//    KC_4,
//    KC_5,
//    KC_6,
//    KC_7,
//    KC_8,
//    KC_9,
//    KC_0,
//    KC_ENTER,
//    KC_ESCAPE,
//    KC_BSPACE,
//    KC_TAB,
//    KC_SPACE,
//    KC_MINUS,
//    KC_EQUAL,
//    KC_LBRACKET,
//    KC_RBRACKET,  // 0x30
//    KC_BSLASH,
//    KC_NONUS_HASH,
//    KC_SCOLON,
//    KC_QUOTE,
//    KC_GRAVE,
//    KC_COMMA,
//    KC_DOT,
//    KC_SLASH,
//    KC_CAPSLOCK,
//    KC_F1,
//    KC_F2,
//    KC_F3,
//    KC_F4,
//    KC_F5,
//    KC_F6,
//    KC_F7,  // 0x40
//    KC_F8,
//    KC_F9,
//    KC_F10,
//    KC_F11,
//    KC_F12,
//    KC_PSCREEN,
//    KC_SCROLLLOCK,
//    KC_PAUSE,
//    KC_INSERT,
//    KC_HOME,
//    KC_PGUP,
//    KC_DELETE,
//    KC_END,
//    KC_PGDOWN,
//    KC_RIGHT,
//    KC_LEFT,  // 0x50
//    KC_DOWN,
//    KC_UP,
//    KC_NUMLOCK,
//    KC_KP_SLASH,
//    KC_KP_ASTERISK,
//    KC_KP_MINUS,
//    KC_KP_PLUS,
//    KC_KP_ENTER,
//    KC_KP_1,
//    KC_KP_2,
//    KC_KP_3,
//    KC_KP_4,
//    KC_KP_5,
//    KC_KP_6,
//    KC_KP_7,
//    KC_KP_8,  // 0x60
//    KC_KP_9,
//    KC_KP_0,
//    KC_KP_DOT,
//    KC_NONUS_BSLASH,
//    KC_APPLICATION,
//    KC_POWER,
//    KC_KP_EQUAL,
//    KC_F13,
//    KC_F14,
//    KC_F15,
//    KC_F16,
//    KC_F17,
//    KC_F18,
//    KC_F19,
//    KC_F20,
//    KC_F21,  // 0x70
//    KC_F22,
//    KC_F23,
//    KC_F24,
//    KC_EXECUTE,
//    KC_HELP,
//    KC_MENU,
//    KC_SELECT,
//    KC_STOP,
//    KC_AGAIN,
//    KC_UNDO,
//    KC_CUT,
//    KC_COPY,
//    KC_PASTE,
//    KC_FIND,
//    KC__MUTE,
//    KC__VOLUP,  // 0x80
//    KC__VOLDOWN,
//    KC_LOCKING_CAPS,
//    KC_LOCKING_NUM,
//    KC_LOCKING_SCROLL,
//    KC_KP_COMMA,
//    KC_KP_EQUAL_AS400,
//    KC_INT1,
//    KC_INT2,
//    KC_INT3,
//    KC_INT4,
//    KC_INT5,
//    KC_INT6,
//    KC_INT7,
//    KC_INT8,
//    KC_INT9,
//    KC_LANG1,  // 0x90
//    KC_LANG2,
//    KC_LANG3,
//    KC_LANG4,
//    KC_LANG5,
//    KC_LANG6,
//    KC_LANG7,
//    KC_LANG8,
//    KC_LANG9,
//    KC_ALT_ERASE,
//    KC_SYSREQ,
//    KC_CANCEL,
//    KC_CLEAR,
//    KC_PRIOR,
//    KC_RETURN,
//    KC_SEPARATOR,
//    KC_OUT,  // 0xA0
//    KC_OPER,
//    KC_CLEAR_AGAIN,
//    KC_CRSEL,
//    KC_EXSEL,
//
//    /* Modifiers */
//    KC_LCTRL = 0xE0,
//    KC_LSHIFT,
//    KC_LALT,
//    KC_LGUI,
//    KC_RCTRL,
//    KC_RSHIFT,
//    KC_RALT,
//    KC_RGUI
//
//    // **********************************************
//    // * 0xF0-0xFF are unallocated in the HID spec. *
//    // * QMK uses these for Mouse Keys - see below. *
//    // **********************************************
//};
//
//class DigiKeyboardDevice {
// public:
//  DigiKeyboardDevice () {
//    cli();
//    usbDeviceDisconnect();
//    _delay_ms(250);
//    usbDeviceConnect();
//
//
//    usbInit();
//      
//    sei();
//
//    // TODO: Remove the next two lines once we fix
//    //       missing first keystroke bug properly.
//    memset(reportBuffer, 0, sizeof(reportBuffer));      
//    usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
//  }
//    
//  void update() {
//    usbPoll();
//  }
//  
//  // delay while updating until we are finished delaying
//  void delay(long milli) {
//    unsigned long last = millis();
//    while (milli > 0) {
//      unsigned long now = millis();
//      milli -= now - last;
//      last = now;
//      update();
//    }
//  }
//
//  void record(void)
//  {
//    reportBuffer[0] = 0;
//    reportBuffer[1] = 0;
//    _index = 2;
//  }
//
//
//
//  void addKeyPress(uint8_t kc)
//  {
//    if(kc>=KC_LCTRL && kc<=KC_RGUI)
//    {
//      reportBuffer[0] |= MOD_BIT(kc);
//    }else
//    {
//       if (reportBuffer[2] != kc && reportBuffer[3] != kc &&
//          reportBuffer[4] != kc && reportBuffer[5] != kc &&
//          reportBuffer[6] != kc && reportBuffer[7] != kc) {
//      
//             reportBuffer[_index++] = kc;
//             if (_index == 8) {
//               //TODO error         
//             }
//       }
//    }
//  }
//
//  void report(void)
//  {
//    while(_index < 8)
//    {
//       reportBuffer[_index++] = 0;
//    }
//    while (!usbInterruptIsReady()) {
//        // Note: We wait until we can send keyPress
//        //       so we know the previous keyPress was
//        //       sent.
//        usbPoll();
//        _delay_ms(5);
//      }
//      
//      usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
//  }
//    
//  //private: TODO: Make friend?
//  uchar    reportBuffer[8];    // buffer for HID reports [ 1 modifier byte + (len-1) key strokes]
//  uint8_t _index ;
//};
//
//DigiKeyboardDevice DigiKeyboard = DigiKeyboardDevice();
//
//#ifdef __cplusplus
//extern "C"{
//#endif 
//  // USB_PUBLIC uchar usbFunctionSetup
//  uchar usbFunctionSetup(uchar data[8]) {
//    usbRequest_t    *rq = (usbRequest_t *)((void *)data);
//
//    usbMsgPtr = DigiKeyboard.reportBuffer; //
//    if ((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {
//      /* class request type */
//
//      if (rq->bRequest == USBRQ_HID_GET_REPORT) {
//        /* wValue: ReportType (highbyte), ReportID (lowbyte) */
//
//        /* we only have one report type, so don't look at wValue */
//        // TODO: Ensure it's okay not to return anything here?    
//        return 0;
//
//      } else if (rq->bRequest == USBRQ_HID_GET_IDLE) {
//        //usbMsgPtr = &idleRate;
//        //return 1;
//        return 0;
//        
//      } else if (rq->bRequest == USBRQ_HID_SET_IDLE) {
//        idleRate = rq->wValue.bytes[1];
//        
//      }
//    } else {
//      /* no vendor specific requests implemented */
//    }
//    
//    return 0;
//  }
//#ifdef __cplusplus
//} // extern "C"
//#endif
//
//
//#endif // __DigiKeyboard_h__
