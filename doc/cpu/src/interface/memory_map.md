# Memory map

以下に, CPUボードから見たFPGA内のMemory mapを載せる.

- FPGA内部のBRAMは4つに分かれている. 書き込み時はアドレス上位$\SI{2}{bit}$ (BRAM_SELECT) でこれを区別する.
    - 0x0: Controller BRAM
    - 0x1: Modulator BRAM
    - 0x2: Normal BRAM
    - 0x3: STM BRAM
- Modulator/STM BRAMはそのままだと書き込みアドレスが足りないので, セグメント方式を採用している.
    - Controller BRAM内の特定のアドレスに書き込まれたセグメント番号を使用し, 書き込むセグメントを選択する.

## Controller

| BRAM_SELECT | BRAM_ADDR (9bit) | DATA (16 bit)          | R/W |
|-------------|------------------|----------------------  |-----|
| 0x0         | 0x000            | CTL_REG                | R/W |
|             | 0x001            | 7:0 = FPGA info        | W   |
|             | 0x010            | -                      | -   |
|             | 0x011            | EC_SYNC_TIME_0         | W   |
|             | 0x012            | EC_SYNC_TIME_1         | W   |
|             | 0x013            | EC_SYNC_TIME_2         | W   |
|             | 0x014            | EC_SYNC_TIME_3         | W   |
|             | 0x020            | 0 = MOD_BRAM_SEGMENT   | W   |
|             | 0x021            | MOD_CYCLE     	      | W   |
|             | 0x022            | MOD_FREQ_DIV_0         | W   |
|             | 0x023            | MOD_FREQ_DIV_1         | W   |
|             | 0x03F            | VERSION_NUM            | R   |
|             | 0x040            | SILENT_CYCLE           | W   |
|             | 0x041            | SILENT_STEP   	      | W   |
|             | 0x050            | 4:0 = STM_BRAM_SEGMENT | W   |
|             | 0x051            | STM_CYCLE              | W   |
|             | 0x052            | STM_FREQ_DIV_0         | W   |
|             | 0x053            | STM_FREQ_DIV_1         | W   |
|             | 0x054            | SOUND_SPEED_0          | W   |
|             | 0x055            | SOUND_SPEED_1          | W   |
|             | 0x100            | CYCLE\[0\]             | W   |
|             | ︙               | ︙                     | ︙  |
|             | 0x1F8            | CYCLE\[248\]           | W   |

* CTL_REG bit
    * 0: LEGACY_MODE
    * 4: FORCE_FAN
    * 5: OP_MODE (0: Normal, 1: STM)
    * 6: STM_MODE (0: Focus STM, 1: Gain STM)
    * 8: SYNC_SET

## Modulator

| BRAM_SELECT | SEGMENT (1bit) | BRAM_ADDR (14bit) | DATA (16bit)              | R/W |
|-------------|----------------|-------------------|---------------------------|-----|
| 0x1         | 0x0            | 0x0000            | mod\[1\]/mod\[0\]         | W   |
|             | ︙             | ︙                | ︙                        | ︙  |
|             | 0x0            | 0x3FFF            | mod\[32767\]/mod\[32766\] | W   |
|             | 0x1            | 0x0000            | mod\[32769\]/mod\[32768\] | W   |
|             | ︙             | ︙                | ︙                        | ︙  |
|             | 0x1            | 0x3FFF            | mod\[65535\]/mod\[65534\] | W   |

## Normal

- Normal BRAMの中身はLEGACY_MODEによって変化する.
    - LEGACY_MODE = 0の場合は, $\SI{13}{bit}$のDuty比/位相を書き込む
    - LEGACY_MODE = 1の場合は, $\SI{8}{bit}$のDuty比/位相を書き込む

- LEGACY_MODE = 0

| BRAM_SELECT | BRAM_ADDR (9bit)  | DATA (16bit)        | R/W |
|-------------|-------------------|---------------------|-----|
| 0x2         | 0x000             | 12:0 = phase\[0\]   | W   |
|             | 0x001             | 12:0 = duty\[0\]    | W   |
|             | ︙                | ︙                  | ︙  |
|             | 0x1F0             | 12:0 = phase\[248\] | W   |
|             | 0x1F1             | 12:0 = duty\[248\]  | W   |

- LEGACY_MODE = 1

| BRAM_SELECT | BRAM_ADDR (9bit)  | DATA (16bit)                             | R/W |
|-------------|-------------------|------------------------------------------|-----|
| 0x2         | 0x000             | 7:0 = phase\[0\]<br>15:8 = duty\[0\]     | W   |
|             | 0x001             | -                                        | -   |
|             | ︙                | ︙                                       | ︙  |
|             | 0x1F0             | 7:0 = phase\[248\]<br>15:8 = duty\[248\] | W   |
|             | 0x1F1             | -                                        | -   |

## STM 

STM BRAMはFocus STMとGain STMで共用である.

### Focus STM (STM_MODE == 0)

| BRAM_SELECT | SEGMENT (5bit) | BRAM_ADDR (14bit) | DATA (16bit)                            | R/W |
|-------------| -------------- |-------------------|-----------------------------------------|-----|
| 0x3         | 0x00           | 0x0000            | x\[0\]\[15:0\]                          | W   |
|             | ︙             | 0x0001            | y\[0\]\[13:0\]/x\[0\]\[17:16\]          | W   |
|             | ︙             | 0x0002            | z\[0\]\[11:0\]/y\[0\]\[17:14\]          | W   |
|             | ︙             | 0x0003            | duty_shift\[0\]/z\[0\]\[17:12\]         | W   |
|             | ︙             | 0x0004-0x0007     | -                                       | -   |
|             | ︙             | ︙                | ︙                                      | ︙  |
|             | ︙             | 0x3FF8            | x\[2047\]\[15:0\]                       | W   |
|             | ︙             | 0x3FF9            | y\[2047\]\[13:0\]/x\[2047\]\[17:16\]    | W   |
|             | ︙             | 0x3FFA            | z\[2047\]\[11:0\]/y\[2047\]\[17:14\]    | W   |
|             | ︙             | 0x3FFB            | duty_shift\[2047\]/z\[2047\]\[17:12\]   | W   |
|             | 0x00           | 0x3FFC-0x3FFF     | -                                       | -   |
|             | 0x01           | 0x00000           | x\[2048\]\[15:0\]                       | W   |
|             | ︙             | 0x00001           | y\[2048\]\[13:0\]/x\[2048\]\[17:16\]    | W   |
|             | ︙             | 0x00002           | z\[2048\]\[11:0\]/y\[2048\]\[17:14\]    | W   |
|             | ︙             | 0x00003           | duty_shift\[2048\]/z\[2048\]\[17:12\]   | W   |
|             | ︙             | 0x00004-0x00007   | -                                       | -   |
|             | ︙             | ︙                | ︙                                      | ︙  |
|             | 0x1F           | 0x3FF8            | x\[65535\]\[15:0\]                      | W   |
|             | ︙             | 0x3FF9            | y\[65535\]\[13:0\]/x\[65535\]\[17:16\]  | W   |
|             | ︙             | 0x3FFA            | z\[65535\]\[11:0\]/y\[65535\]\[17:14\]  | W   |
|             | ︙             | 0x3FFB            | duty_shift\[65535\]/z\[65535\]\[17:12\] | W   |
|             | 0x1F           | 0x3FFC-0x3FFF     | -                                       | -   |

### Gain STM (STM_MODE == 1)

- LEGACY_MODE = 0

| BRAM_SELECT | SEGMENT (5bit) | BRAM_ADDR (14bit) | DATA (16bit)                | R/W |
|-------------| -------------- |-------------------|-----------------------------|-----|
| 0x3         | 0x00           | 0x0000            | 12:0 = phase\[0\]\[0\]      | W   |
|             | ︙             | 0x0001            | 12:0 = duty\[0\]\[0\]       | W   |
|             | ︙             | ︙                | ︙                          | ︙  |
|             | ︙             | 0x01F0            | 12:0 = phase\[0\]\[248\]    | W   |
|             | ︙             | 0x01F1            | 12:0 = duty\[0\]\[248\]     | W   |
|             | ︙             | 0x01F2-0x01FF     | -                           | -   |
|             | ︙             | ︙                | ︙                          | ︙  |
|             | ︙             | 0x3E00            | 12:0 = phase\[31\]\[0\]     | W   |
|             | ︙             | 0x3E01            | 12:0 = duty\[31\]\[0\]      | W   |
|             | ︙             | ︙                | ︙                          | ︙  |
|             | ︙             | 0x3FF0            | 12:0 = phase\[31\]\[248\]   | W   |
|             | ︙             | 0x3FF1            | 12:0 = duty\[31\]\[248\]    | W   |
|             | 0x00           | 0x3FF2-0x3FFF     | -                           | -   |
|             | 0x01           | 0x0000            | 12:0 = phase\[32\]\[0\]     | W   |
|             | ︙             | 0x0001            | 12:0 = duty\[32\]\[0\]      | W   |
|             | ︙             | ︙                | ︙                          | ︙  |
|             | 0x1F           | ︙                | ︙                          | W   |
|             | ︙             | 0x3FF0            | 12:0 = phase\[1023\]\[248\] | W   |
|             | ︙             | 0x3FF1            | 12:0 = duty\[1023\]\[248\]  | W   |
|             | 0x1F           | 0x3FF2-0x3FFF     | -                           | -   |

- LEGACY_MODE = 1


| BRAM_SELECT | SEGMENT (5bit) | BRAM_ADDR (14bit) | DATA (16bit)                                             | R/W |
|-------------| -------------- |-------------------|----------------------------------------------------------|-----|
| 0x3         | 0x00           | 0x0000            | 7:0 = phase\[0\]\[0\]<br>15:8 = duty\[0\]\[0\]           | W   |
|             | ︙             | 0x0001            | -                                                        | -   |
|             | ︙             | ︙                | ︙                                                       | ︙  |
|             | ︙             | 0x01F0            | 7:0 = phase\[0\]\[248\]<br>15:8 = duty\[0\]\[248\]       | W   |
|             | ︙             | 0x01F1            | -                                                        | -   |
|             | ︙             | 0x01F2-0x01FF     | -                                                        | -   |
|             | ︙             | ︙                | ︙                                                       | ︙  |
|             | ︙             | 0x3E00            | 7:0 = phase\[31\]\[0\]<br>15:8 = duty\[31\]\[0\]         | W   |
|             | ︙             | 0x3E01            | -                                                        | -   |
|             | ︙             | ︙                | ︙                                                       | ︙  |
|             | ︙             | 0x3FF0            | 7:0 = phase\[31\]\[248\]<br>15:8 = duty\[31\]\[248\]     | W   |
|             | ︙             | 0x3FF1            | -                                                        | -   |
|             | 0x00           | 0x3FF2-0x3FFF     | -                                                        | -   |
|             | 0x01           | 0x0000            | 7:0 = phase\[32\]\[0\]<br>15:8 = duty\[32\]\[0\]         | W   |
|             | ︙             | 0x0001            | -                                                        | -   |
|             | ︙             | ︙                | ︙                                                       | ︙  |
|             | 0x1F           | ︙                | ︙                                                       | ︙  |
|             | ︙             | 0x3FF0            | 7:0 = phase\[1023\]\[248\]<br>15:8 = duty\[1023\]\[248\] | W   |
|             | ︙             | 0x3FF1            | -                                                        | -   |
|             | 0x1F           | 0x3FF2-0x3FFF     | -                                                        | -   |
