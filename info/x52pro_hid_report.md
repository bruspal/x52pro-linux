0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
0x09, 0x04,        // Usage (Joystick)
0xA1, 0x01,        // Collection (Application)
0x09, 0x01,        //   Usage (Pointer)
0xA1, 0x00,        //   Collection (Physical)
0x09, 0x30,        //     Usage (X)
0x09, 0x31,        //     Usage (Y)
0x15, 0x00,        //     Logical Minimum (0)
0x26, 0xFF, 0x03,  //     Logical Maximum (1023)
0x75, 0x0A,        //     Report Size (10)
0x95, 0x02,        //     Report Count (2)
0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x75, 0x02,        //     Report Size (2)
0x95, 0x01,        //     Report Count (1)
0x81, 0x01,        //     Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x09, 0x35,        //     Usage (Rz)
0x15, 0x00,        //     Logical Minimum (0)
0x26, 0xFF, 0x03,  //     Logical Maximum (1023)
0x75, 0x0A,        //     Report Size (10)
0x95, 0x01,        //     Report Count (1)
0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x09, 0x32,        //     Usage (Z)
0x09, 0x33,        //     Usage (Rx)
0x09, 0x34,        //     Usage (Ry)
0x09, 0x36,        //     Usage (Slider)
0x15, 0x00,        //     Logical Minimum (0)
0x26, 0xFF, 0x00,  //     Logical Maximum (255)
0x75, 0x08,        //     Report Size (8)
0x95, 0x04,        //     Report Count (4)
0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x05, 0x09,        //     Usage Page (Button)
0x19, 0x01,        //     Usage Minimum (0x01)
0x29, 0x27,        //     Usage Maximum (0x27)
0x15, 0x00,        //     Logical Minimum (0)
0x25, 0x01,        //     Logical Maximum (1)
0x95, 0x27,        //     Report Count (39)
0x75, 0x01,        //     Report Size (1)
0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x75, 0x05,        //     Report Size (5)
0x95, 0x01,        //     Report Count (1)
0x81, 0x01,        //     Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
0x09, 0x39,        //     Usage (Hat switch)
0x15, 0x01,        //     Logical Minimum (1)
0x25, 0x08,        //     Logical Maximum (8)
0x35, 0x00,        //     Physical Minimum (0)
0x46, 0x3B, 0x01,  //     Physical Maximum (315)
0x66, 0x14, 0x00,  //     Unit (System: English Rotation, Length: Centimeter)
0x75, 0x04,        //     Report Size (4)
0x95, 0x01,        //     Report Count (1)
0x81, 0x42,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,Null State)
0x05, 0x05,        //     Usage Page (Game Ctrls)
0x09, 0x24,        //     Usage (Move Right/Left)
0x09, 0x26,        //     Usage (Move Up/Down)
0x15, 0x00,        //     Logical Minimum (0)
0x25, 0x0F,        //     Logical Maximum (15)
0x75, 0x04,        //     Report Size (4)
0x95, 0x02,        //     Report Count (2)
0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              //   End Collection
0xC0,              // End Collection

// 125 bytes
