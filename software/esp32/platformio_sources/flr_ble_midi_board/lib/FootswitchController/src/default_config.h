
uint8_t default_bin_config[] = {
0xf0,
0x74,0x65,0x73,0x74,0x31,0x5f,0x4e,0x55,0x58,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x01, // fw
0x01,   // BLE mode
0x04,   // FS nr

0x00,0x06, 0x01, // var 1: min - max - cycle
0x00,0x00, 0x00, // var 2: min - max - cycle
0x00,0x00, 0x00, // var 2: min - max - cycle
0x00,0x00, 0x00, // var 2: min - max - cycle


0x80,0x01,0x01,0x01,0x31,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x88,
0x80,0x03,0x01,0x01,0x31,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x88,

0x80,0x03,0x01,0x01,0x31,0x00,0x00,0x00,0x00,0x01,0x00,0x01,0x88,
0x80,0x03,0x01,0x01,0x31,0x00,0x00,0x00,0x00,0x01,0x00,0x01,0x88,

0x80,0x04,0x01,0x01,0x54,0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x88,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x88,

0x80,0x04,0x01,0x01,0x1c,0x01,0x00,0x00,0x00,0x02,0x00,0x00,0x88,
0x80,0x04,0x01,0x01,0x7a,0x01,0x00,0x00,0x00,0x03,0x00,0x00,0x88,

0xff};

uint8_t default_bin_config_OLD[] = {0xf0, 
0x46,0x4c,0x52,0x20,0x42,0x4c,0x45,0x20,0x4d,0x49,0x44,0x49,0x20,0x43,0x6f,0x6e,0x74,0x72,0x6f,0x6c,0x6c,0x65,0x72,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32bytes - max 30 used

0x01,0x02,  // fw
0x04,       // fs_number
0x01,       // BLE mode

0x80,0x01,0x01,0x01,0x31,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x88,    // TAP (16bytes)
0x80,0x03,0x01,0x01,0x31,0x00,0x00,0x00,0x00,0x01,0x00,0x06,0x01,0x01,0x01,0x88,    // HOLD

0x80,0x03,0x01,0x01,0x31,0x00,0x00,0x00,0x00,0x01,0x00,0x06,0x00,0x01,0x01,0x88,
0x80,0x03,0x01,0x01,0x31,0x00,0x00,0x00,0x00,0x01,0x00,0x06,0x00,0x01,0x01,0x88,

0x80,0x04,0x01,0x01,0x54,0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x88,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x88,

0x80,0x04,0x01,0x01,0x1c,0x01,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x88,
0x80,0x04,0x01,0x01,0x7a,0x01,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x88,
0xff};


// 166bytes totals