# C API リファレンス

c言語向けのAPIは[capi](https://github.com/shinolab/autd3/tree/master/capi)以下で定義されている.

以下に, このAPIのリファレンスを載せる. 
実際の利用方法は, [C API Example](https://github.com/shinolab/autd3/tree/master/capi/example)を参照されたい.

## AUTDSetLogLevel (autd3capi)

| Argument name / return | type     | in/out | description                        |
| ---------------------- | -------- | ------ | ---------------------------------- |
| level                  | int32_t  | in     | log level                          |
| return                 | void     | -      | -                                  |

## AUTDSetDefaultLogger (autd3capi)

| Argument name / return | type     | in/out | description                        |
| ---------------------- | -------- | ------ | ---------------------------------- |
| out                    | void*    | in     | output callback                    |
| flush                  | void*    | in     | flush callback                     |
| return                 | void     | -      | -                                  |

## AUTDCreateController (autd3capi)

Controllerを作成する.

作成した`Controller`は最後に`AUTDFreeController`で開放する必要がある.

| Argument name / return | type   | in/out | description                      |
| ---------------------- | ------ | ------ | -------------------------------- |
| out                    | void** | out    | pointer to pointer to Controller |
| driver_version         | uint8_t| in     | driver version                   |
| return                 | bool    | -      | true if successful                                                                                     |

## AUTDOpenController (autd3capi)

Controllerをopenする.

handleは`AUTDCreateController`で作成したものを使う. linkは各々のlinkの生成関数で作成したものを使う.

この関数は失敗した場合にfalseを返す. 

| Argument name / return | type  | in/out | description           |
| ---------------------- | ----- | ------ | --------------------- |
| handle                 | void* | in     | pointer to Controller |
| link                   | void* | in     | pointer to Link       |
| return                 | bool  | -      | true if success       |

## AUTDAddDevice (autd3capi)

ControllerにDeviceを追加する.

handleは`AUTDCreateController`で作成したものを使う. x, y, zは位置で, rz1, ry, rz2はZYZのオイラー角である.

| Argument name / return | type    | in/out | description                               |
| ---------------------- | ------- | ------ | ----------------------------------------- |
| handle                 | void*   | in     | pointer to Controller                     |
| x                      | double  | in     | x coordinate of position in millimeter    |
| y                      | double  | in     | y coordinate of position in millimeter    |
| z                      | double  | in     | z coordinate of position in millimeter    |
| rz1                    | double  | in     | first angle of ZYZ euler angle in radian  |
| ry                     | double  | in     | second angle of ZYZ euler angle in radian |
| rz2                    | double  | in     | third angle of ZYZ euler angle in radian  |
| return                 | void | -      | -                                 |

## AUTDAddDeviceQuaternion (autd3capi)

ControllerにDeviceを追加する.

handleは`AUTDCreateController`で作成したものを使う. x, y, zは位置で, qw, qx, qy,
qzは回転を表すクオータニオンである.

| Argument name / return | type    | in/out | description                            |
| ---------------------- | ------- | ------ | -------------------------------------- |
| handle                 | void*   | in     | pointer to Controller                  |
| x                      | double  | in     | x coordinate of position in millimeter |
| y                      | double  | in     | y coordinate of position in millimeter |
| z                      | double  | in     | z coordinate of position in millimeter |
| qw                     | double  | in     | w parameter of quaternion of rotation  |
| qx                     | double  | in     | x parameter of quaternion of rotation  |
| qy                     | double  | in     | y parameter of quaternion of rotation  |
| qz                     | double  | in     | z parameter of quaternion of rotation  |
| return                 | void    | -      | -                              |

## AUTDClose (autd3capi)

Controllerをcloseする.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type    | in/out | description                                                                                            |
| ---------------------- | ------- | ------ | ------------------------------------------------------------------------------------------------------ |
| handle                 | void*   | in     | pointer to Controller                                                                                  |
| return                 | bool    | -      | true if successful                                                                                     |

## AUTDCreateSilencer (autd3capi)

SilencerConfigを作成する.

作成したSilencerConfigは最後に`AUTDDeleteSilencer`で削除する必要がある.

| Argument name / return | type     | in/out | description                          |
| ---------------------- | -------- | ------ | ------------------------------------ |
| out                    | void**   | out    | pointer to pointer to SilencerConfig |
| step                   | uint16_t | in     | silencer update step                 |
| cycle                  | uint16_t | in     | silencer update cycle                |
| return                 | void     | -      | -                                    |

## AUTDDeleteSilencer (autd3capi)

SilencerConfigを削除する.

| Argument name / return | type  | in/out | description               |
| ---------------------- | ----- | ------ | ------------------------- |
| config                 | void* | in     | pointer to SilencerConfig |
| return                 | void  | -      | -                         |

## AUTDFreeController (autd3capi)

Controllerを削除する.

handleは`AUTDCreateController`で作成したものを使う.

これ以降, handleは使用できない.

| Argument name / return | type  | in/out | description           |
| ---------------------- | ----- | ------ | --------------------- |
| handle                 | void* | in     | pointer to Controller |
| return                 | void  | -      | -                     |

## AUTDIsOpen (autd3capi)

ControllerがOpenされているかどうかを返す.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type  | in/out | description                |
| ---------------------- | ----- | ------ | -------------------------- |
| handle                 | void* | in     | pointer to Controller      |
| return                 | bool  | -      | true if controller is open |

## AUTDGetForceFan (autd3capi)

Force fan flagを返す.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type  | in/out | description           |
| ---------------------- | ----- | ------ | --------------------- |
| handle                 | void* | in     | pointer to Controller |
| return                 | bool  | -      | Force fan flag        |

## AUTDGetReadsFPGAInfo (autd3capi)

Reads FPGA info flagを返す.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type  | in/out | description           |
| ---------------------- | ----- | ------ | --------------------- |
| handle                 | void* | in     | pointer to Controller |
| return                 | bool  | -      | Reads FPGA info flag  |

## AUTDGetAckCheckTimeout (autd3capi)

AckCheckTimeoutをナノ秒単位で返す.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type    | in/out | description           |
| ---------------------- | ------- | ------ | --------------------- |
| handle                 | void*   | in     | pointer to Controller |
| return                 | uint64_t | -      | AckCheckTimeout in ns |

## AUTDGetSendInterval (autd3capi)

Send intervalをナノ秒単位で返す.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type    | in/out | description           |
| ---------------------- | ------- | ------ | --------------------- |
| handle                 | void*   | in     | pointer to Controller |
| return                 | uint64_t | -      | Send interval in ns  |

## AUTDSetReadsFPGAInfo (autd3capi)

Reads FPGA info flagを設定する.

handleは`AUTDCreateController`で作成したものを使う.

デバイスに実際に反映されるのは送信関数のどれかを呼び出し後である.

| Argument name / return | type  | in/out | description           |
| ---------------------- | ----- | ------ | --------------------- |
| handle                 | void* | in     | pointer to Controller |
| reads_fpga_info        | bool  | in     | read FPGA info flag   |
| return                 | void  | -      | -                     |

## AUTDSetAckCheckTimeout (autd3capi)

AckCheckTimeoutをナノ秒単位で設定する.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type    | in/out | description           |
| ---------------------- | ------- | ------ | --------------------- |
| handle                 | void*   | in     | pointer to Controller |
| timeout                 | uint64_t | in     | AckCheckTimeout in ns |
| return                 | void    | -      | -                     |

## AUTDSetSendInterval (autd3capi)

Send intervalをナノ秒単位で設定する.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type    | in/out | description           |
| ---------------------- | ------- | ------ | --------------------- |
| handle                 | void*   | in     | pointer to Controller |
| interval               | uint64_t | in    | Send interval in ns   |
| return                 | void    | -      | -                     |

## AUTDSetForceFan (autd3capi)

Force fan flagを設定する.

handleは`AUTDCreateController`で作成したものを使う.

デバイスに実際に反映されるのは送信関数のどれかを呼び出し後である.

| Argument name / return | type  | in/out | description           |
| ---------------------- | ----- | ------ | --------------------- |
| handle                 | void* | in     | pointer to Controller |
| force                  | bool  | in     | force fan flag        |
| return                 | void  | -      | -                     |

## AUTDGetSoundSpeed (autd3capi)

音速を返す.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type   | in/out | description           |
| ---------------------- | ------ | ------ | --------------------- |
| handle                 | void*  | in     | pointer to Controller |
| return                 | double | -      | Speed of sound in  mm/s |

## AUTDSetSoundSpeed (autd3capi)

音速を設定する.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type   | in/out | description           |
| ---------------------- | ------ | ------ | --------------------- |
| handle                 | void*  | in     | pointer to Controller |
| sound_speed            | double | in     | Speed of sound in mm/s|
| return                 | void   | -      | -                     |

## AUTDSetSoundSpeedFromTemp (autd3capi)

温度から音速を設定する.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type   | in/out | description           |
| ---------------------- | ------ | ------ | --------------------- |
| handle                 | void*  | in     | pointer to Controller |
| temp                   | double | in     | temperature in Celsius degree |
| k                   | double | in     | Heat capacity ratio           |
| r                   | double | in     |  Gas constant [J K^-1 mol^-1]           |
| m                   | double | in     | Molar mass [kg mod^-1]           |
| return                 | void   | -      | -                     |

## AUTDGetTransFrequency (autd3capi)

指定した振動子の周波数を取得する.

handleは`AUTDCreateController`で作成したものを使う. 

| Argument name / return | type    | in/out | description                 |
| ---------------------- | ------- | ------ | --------------------------- |
| handle                 | void*   | in     | pointer to Controller       |
| trans_idx        | int32_t | in     | transducer index      |
| return                 | double  | -      | frequency of the transducer |

## AUTDSetTransFrequency (autd3capi)

指定した振動子の周波数を設定する.

handleは`AUTDCreateController`で作成したものを使う.

Legacyモードにおいては, この関数は何もしない.

| Argument name / return | type    | in/out | description                 |
| ---------------------- | ------- | ------ | --------------------------- |
| handle                 | void*   | in     | pointer to Controller       |
| trans_idx        | int32_t | in     | transducer index      |
| frequency              | double  | in     | frequency of the transducer |
| return                 | void    | -      | -                           |

## AUTDGetTransCycle (autd3capi)

指定した振動子の周期を取得する.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type     | in/out | description             |
| ---------------------- | -------- | ------ | ----------------------- |
| handle                 | void*    | in     | pointer to Controller   |
| trans_idx        | int32_t  | in     | transducer index  |
| return                 | uint16_t | -      | cycle of the transducer |

## AUTDSetTransCycle (autd3capi)

指定した振動子の周期を設定する.

handleは`AUTDCreateController`で作成したものを使う.

Legacyモードにおいては, この関数は何もしない.

| Argument name / return | type     | in/out | description             |
| ---------------------- | -------- | ------ | ----------------------- |
| handle                 | void*    | in     | pointer to Controller   |
| trans_idx        | int32_t  | in     | transducer index  |
| cycle                  | uint16_t | in     | cycle of the transducer |
| return                 | void     | -      | -                       |

## AUTDGetWavelength (autd3capi)

指定した振動子の波長を取得する.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type    | in/out | description                                          |
| ---------------------- | ------- | ------ | ---------------------------------------------------- |
| handle                 | void*   | in     | pointer to Controller                                |
| trans_idx        | int32_t | in     | transducer index                               |
| return                 | double  | -      | wavelength of ultrasound emitted from the transducer |

## AUTDGetAttenuation (autd3capi)

減衰係数を返す.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type   | in/out | description                      |
| ---------------------- | ------ | ------ | -------------------------------- |
| handle                 | void*  | in     | pointer to Controller            |
| return                 | double | -      | attenuation coefficient in Np/mm |

## AUTDSetAttenuation (autd3capi)

減衰係数を設定する.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type   | in/out | description                      |
| ---------------------- | ------ | ------ | -------------------------------- |
| handle                 | void*  | in     | pointer to Controller            |
| attenuation            | double | in     | attenuation coefficient in Np/mm |
| return                 | void   | -      | -                                |

## AUTDGetFPGAInfo (autd3capi)

FPGAの情報を取得する.

handleは`AUTDCreateController`で作成したものを使う. outポインタが指す領域は, 接続しているデバイスと同じ長さである必要がある.

なお, FPGAの情報は下位1bitが温度センサがアサートされているかどうかを表し, 他のbitは全て0である.

この関数を呼び出す前に`AUTDSetReadsFPGAInfo`でread FPGA info flagをOnにしておく必要がある.

この関数は失敗した場合にfalseを返す. 

| Argument name / return | type     | in/out | description           |
| ---------------------- | -------- | ------ | --------------------- |
| handle                 | void*    | in     | pointer to Controller |
| out                    | uint8_t* | in     | FPGA information list |
| return                 | bool     | -      | true if success       |

## AUTDNumTransducers (autd3capi)

接続されているTransducerの数を取得する.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type    | in/out | description           |
| ---------------------- | ------- | ------ | --------------------- |
| handle                 | void*   | in     | pointer to Controller |
| return                 | int32_t | -      | number of transducers     |

## AUTDNumDevices (autd3capi)

接続されているDeviceの数を取得する.

| Argument name / return | type    | in/out | description           |
| ---------------------- | ------- | ------ | --------------------- |
| handle                 | void*   | in     | pointer to Controller |
| return                 | int32_t | -      | number of devices     |

## AUTDGeometryCenter (autd3capi)

Geometryの中心を取得する.

| Argument name / return | type    | in/out | description                         |
| ---------------------- | ------- | ------ | ----------------------------------- |
| handle                 | void*   | in     | pointer to Controller               |
| x                      | double* | out    | x coordinate of center              |
| y                      | double* | out    | y coordinate of center              |
| z                      | double* | out    | z coordinate of center              |
| return                 | void    | -      | -                                   |

## AUTDTransPosition (autd3capi)

指定した振動子の位置を取得する.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type    | in/out | description                         |
| ---------------------- | ------- | ------ | ----------------------------------- |
| handle                 | void*   | in     | pointer to Controller               |
| trans_idx        | int32_t | in     | transducer index              |
| x                      | double* | out    | x coordinate of transducer position |
| y                      | double* | out    | y coordinate of transducer position |
| z                      | double* | out    | z coordinate of transducer position |
| return                 | void    | -      | -                                   |

## AUTDTransXDirection (autd3capi)

指定した振動子のx軸方向を取得する.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type    | in/out | description                 |
| ---------------------- | ------- | ------ | --------------------------- |
| handle                 | void*   | in     | pointer to Controller       |
| trans_idx        | int32_t | in     | transducer index      |
| x                      | double* | out    | x coordinate of x-direction |
| y                      | double* | out    | y coordinate of x-direction |
| z                      | double* | out    | z coordinate of x-direction |
| return                 | void    | -      | -                           |

## AUTDTransYDirection (autd3capi)

指定した振動子のy軸方向を取得する.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type    | in/out | description                 |
| ---------------------- | ------- | ------ | --------------------------- |
| handle                 | void*   | in     | pointer to Controller       |
| trans_idx        | int32_t | in     | transducer index      |
| x                      | double* | out    | x coordinate of y-direction |
| y                      | double* | out    | y coordinate of y-direction |
| z                      | double* | out    | z coordinate of y-direction |
| return                 | void    | -      | -                           |

## AUTDTransZDirection (autd3capi)

指定した振動子のz軸方向を取得する.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type    | in/out | description                 |
| ---------------------- | ------- | ------ | --------------------------- |
| handle                 | void*   | in     | pointer to Controller       |
| trans_idx        | int32_t | in     | transducer index      |
| x                      | double* | out    | x coordinate of z-direction |
| y                      | double* | out    | y coordinate of z-direction |
| z                      | double* | out    | z coordinate of z-direction |
| return                 | void    | -      | -                           |

## AUTDGetFirmwareInfoListPointer (autd3capi)

Firmware information listへのポインタを取得する.

handleは`AUTDCreateController`で作成したものを使う.

この関数で作成したlistは最後に`AUTDFreeFirmwareInfoListPointer`で開放する必要がある.

実際のFirmware informationは`AUTDGetFirmwareInfo`で取得する.

| Argument name / return | type    | in/out | description                                                         |
| ---------------------- | ------- | ------ | ------------------------------------------------------------------- |
| handle                 | void*   | in     | pointer to Controller                                               |
| out                    | void**  | out    | pointer to pointer to Firmware information list                     |
| return                 | int32_t | -      | if $<0$ some error occurred, else size of Firmware information list |

## AUTDGetFirmwareInfo (autd3capi)

Firmware informationを取得する.

`p_firm_info_list`は`AUTDGetFirmwareInfoListPointer`で作成したものを使う.

`info`は長さ256のバッファを渡せば十分である.

| Argument name / return | type    | in/out | description                            |
| ---------------------- | ------- | ------ | -------------------------------------- |
| p_firm_info_list       | void*   | in     | pointer to Firmware information list   |
| index                  | int32_t | in     | device index                           |
| info                   | char*   | out    | pointer to firmware information string |
| return                 | void    | -      | -                                      |

## AUTDFreeFirmwareInfoListPointer (autd3capi)

`AUTDGetFirmwareInfoListPointer`で取得したFirmware information listを開放する.

| Argument name / return | type  | in/out | description                          |
| ---------------------- | ----- | ------ | ------------------------------------ |
| p_firm_info_list       | void* | in     | pointer to Firmware information list |
| return                 | void  | -      | -                                    |

## AUTDGainNull (autd3capi)

Null gainを作成する.

作成したGainは最後に`AUTDDeleteGain`で削除する必要がある.

| Argument name / return | type   | in/out | description                     |
| ---------------------- | ------ | ------ | ------------------------------- |
| gain                   | void** | out    | pointer to pointer to Null gain |
| return                 | void   | -      | -                               |

## AUTDGainGrouped (autd3capi)

Grouped gainを作成する.

handleは`AUTDCreateController`で作成したものを使う.

作成したGainは最後に`AUTDDeleteGain`で削除する必要がある.

| Argument name / return | type   | in/out | description                        |
| ---------------------- | ------ | ------ | ---------------------------------- |
| gain                   | void** | out    | pointer to pointer to Grouped gain |
| handle                 | void*  | in     | pointer to Controller              |
| return                 | void   | -      | -                                  |

## AUTDGainGroupedAdd (autd3capi)

Grouped gainにGainを登録する.

`grouped_gain`は`AUTDGainGrouped`で作成したものを使う.

| Argument name / return | type    | in/out | description             |
| ---------------------- | ------- | ------ | ----------------------- |
| grouped_gain           | void*   | in     | pointer to Grouped gain |
| device_id              | int32_t | in     | Device Id               |
| gain                   | void*   | in     | pointer to gain         |
| return                 | void    | -      | -                       |

## AUTDGainFocus (autd3capi)

Focus gainを作成する.

作成したGainは最後に`AUTDDeleteGain`で削除する必要がある.

| Argument name / return | type   | in/out | description                 |
| ---------------------- | ------ | ------ | --------------------------- |
| gain                   | void** | out    | pointer to Focus gain       |
| x                      | double | in     | x coordinate of focal point |
| y                      | double | in     | y coordinate of focal point |
| z                      | double | in     | z coordinate of focal point |
| amp                    | double | in     | amplitude of focus          |
| return                 | void   | -      | -                           |

## AUTDGainBesselBeam (autd3capi)

Bessel beam gainを作成する.

作成したGainは最後に`AUTDDeleteGain`で削除する必要がある.

| Argument name / return | type   | in/out | description                    |
| ---------------------- | ------ | ------ | ------------------------------ |
| gain                   | void** | out    | pointer to Bessel beam gain    |
| x                      | double | in     | x coordinate of apex           |
| y                      | double | in     | y coordinate of apex           |
| z                      | double | in     | z coordinate of apex           |
| n_x                    | double | in     | x coordinate of beam direction |
| n_y                    | double | in     | y coordinate of beam direction |
| n_z                    | double | in     | z coordinate of beam direction |
| theta_z                | double | in     | tilt angle of beam             |
| amp                    | double | in     | amplitude of beam              |
| return                 | void   | -      | -                              |

## AUTDGainPlaneWave (autd3capi)

Plane wave gainを作成する.

作成したGainは最後に`AUTDDeleteGain`で削除する必要がある.

| Argument name / return | type   | in/out | description                    |
| ---------------------- | ------ | ------ | ------------------------------ |
| gain                   | void** | out    | pointer to Plane wave gain     |
| n_x                    | double | in     | x coordinate of wave direction |
| n_y                    | double | in     | y coordinate of wave direction |
| n_z                    | double | in     | z coordinate of wave direction |
| amp                    | double | in     | amplitude of wave              |
| return                 | void   | -      | -                              |

## AUTDGainTransducerTest (autd3capi)

TransducerTest gainを作成する.

作成したGainは最後に`AUTDDeleteGain`で削除する必要がある.

| Argument name / return | type     | in/out | description                     |
| ---------------------- | -------  | ------ | ---------------------           |
| gain                   | void**   | out    | pointer to TransducerTest gain  |
| return                 | void     | -      | -                               |

## AUTDGainTransducerTestSet (autd3capi)

TransducerTest gainに振幅と位相をセットする.

| Argument name / return | type     | in/out | description                     |
| ---------------------- | -------  | ------ | ---------------------           |
| gain                   | void*    | in     | pointer to TransducerTest gain  |
| tr_idx                 | int32_t  | in     | transducer index          |
| amp                    | double   | in     | amplitude                       |
| phase                  | double   | in     | phase                           |
| return                 | void     | -      | -                               |

## AUTDGainCustom (autd3capi)

Custom gainを作成する.

Custom gainは位相と振幅を直接指定するGainである.

作成したGainは最後に`AUTDDeleteGain`で削除する必要がある.

| Argument name / return | type     | in/out | description            |
| ---------------------- | -------  | ------ | ---------------------  |
| gain                   | void**   | out    | pointer to Custom gain |
| amp                    | double*  | in     | pointer to amplitude   |
| phase                  | double*  | in     | pointer to phase       |
| size                   | uint64_t | in     | size of amp and phase  |
| return                 | void     | -      | -                      |

## AUTDDeleteGain (autd3capi)

作成したGainを削除する.

| Argument name / return | type  | in/out | description     |
| ---------------------- | ----- | ------ | --------------- |
| gain                   | void* | in     | pointer to gain |
| return                 | void  | -      | -               |

## AUTDModulationStatic (autd3capi)

Static modulationを作成する.

作成したModulationは最後に`AUTDDeleteModulation`で削除する必要がある.

| Argument name / return | type   | in/out | description                             |
| ---------------------- | ------ | ------ | --------------------------------------- |
| mod                    | void** | out    | pointer to pointer to Static modulation |
| amp                    | double | in     | amplitude of modulation                 |
| return                 | void   | -      | -                                       |

## AUTDModulationSine (autd3capi)

Sine modulationを作成する.

作成したModulationは最後に`AUTDDeleteModulation`で削除する必要がある.

| Argument name / return | type    | in/out | description                           |
| ---------------------- | ------- | ------ | ------------------------------------- |
| mod                    | void**  | out    | pointer to pointer to Sine modulation |
| freq                   | int32_t | in     | frequency of sine modulation          |
| amp                    | double  | in     | amplitude of sine modulation          |
| offset                 | double  | in     | offset of sine modulation             |
| return                 | void    | -      | -                                     |

## AUTDModulationSineSquared (autd3capi)

SineSquared modulationを作成する.

作成したModulationは最後に`AUTDDeleteModulation`で削除する必要がある.

| Argument name / return | type    | in/out | description                                  |
| ---------------------- | ------- | ------ | -------------------------------------------- |
| mod                    | void**  | out    | pointer to pointer to SineSquared modulation |
| freq                   | int32_t | in     | frequency of sine modulation                 |
| amp                    | double  | in     | amplitude of sine modulation                 |
| offset                 | double  | in     | offset of sine modulation                    |
| return                 | void    | -      | -                                            |

## AUTDModulationSineLegacy (autd3capi)

SineLegacy modulationを作成する.

作成したModulationは最後に`AUTDDeleteModulation`で削除する必要がある.

| Argument name / return | type   | in/out | description                                 |
| ---------------------- | ------ | ------ | ------------------------------------------- |
| mod                    | void** | out    | pointer to pointer to SineLegacy modulation |
| freq                   | double | in     | frequency of sine modulation                |
| amp                    | double | in     | amplitude of sine modulation                |
| offset                 | double | in     | offset of sine modulation                   |
| return                 | void   | -      | -                                           |

## AUTDModulationSquare (autd3capi)

Square modulationを作成する.

作成したModulationは最後に`AUTDDeleteModulation`で削除する必要がある.

| Argument name / return | type    | in/out | description                                  |
| ---------------------- | ------- | ------ | -------------------------------------------- |
| mod                    | void**  | out    | pointer to pointer to Square modulation      |
| freq                   | int32_t | in     | frequency of square modulation               |
| low                    | double  | in     | amplitude at low level of square modulation  |
| high                   | double  | in     | amplitude at high level of square modulation |
| duty                   | double  | in     | duty ratio of square modulation              |
| return                 | void    | -      | -                                            |

## AUTDModulationLPF (autd3capi)

LPF modulationを作成する.

作成したModulationは最後に`AUTDDeleteModulation`で削除する必要がある.

| Argument name / return | type    | in/out | description                                  |
| ---------------------- | ------- | ------ | -------------------------------------------- |
| mod                    | void**  | out    | pointer to pointer to LPF modulation         |
| mod_in                 | void*   | in     | original modulation                          |
| return                 | void    | -      | -                                            |

## AUTDModulationCustom (autd3capi)

Custom modulationを作成する.

Custom modulationは振幅を直接指定するModulationである.

作成したModulationは最後に`AUTDDeleteModulation`で削除する必要がある.

| Argument name / return | type     | in/out | description                             |
| ---------------------- | -------- | ------ | --------------------------------------- |
| mod                    | void**   | out    | pointer to pointer to Custom modulation |
| buffer                 | uint8_t* | in     | pointer to modulation data              |
| size                   | uint64_t | in     | size of buffer                          |
| freq_div               | uint32_t | in     | modulation sampling frequency division  |
| return                 | void     | -      | -                                       |

## AUTDModulationSamplingFrequencyDivision (autd3capi)

Modulation sampling frequency divisionを返す.

| Argument name / return | type     | in/out | description                            |
| ---------------------- | -------- | ------ | -------------------------------------- |
| mod                    | void*    | in     | pointer to modulation                  |
| return                 | uint32_t | -      | modulation sampling frequency division |

## AUTDModulationSetSamplingFrequencyDivision (autd3capi)

Modulation sampling frequency divisionを設定する.

| Argument name / return | type     | in/out | description                            |
| ---------------------- | -------- | ------ | -------------------------------------- |
| mod                    | void*    | in     | pointer to modulation                  |
| freq_div               | uint32_t | in     | modulation sampling frequency division |
| return                 | void     | -      | -                                      |

## AUTDModulationSamplingFrequency (autd3capi)

Sampling frequencyを返す.

| Argument name / return | type   | in/out | description                   |
| ---------------------- | ------ | ------ | ----------------------------- |
| mod                    | void*  | in     | pointer to modulation         |
| return                 | double | -      | modulation sampling frequency |

## AUTDDeleteModulation (autd3capi)

Modulationを削除する.

| Argument name / return | type  | in/out | description           |
| ---------------------- | ----- | ------ | --------------------- |
| mod                    | void* | in     | pointer to modulation |
| return                 | void  | -      | -                     |

## AUTDFocusSTM (autd3capi)

Focus STMを作成する.

作成したSTMは最後に`AUTDDeleteSTM`で削除する必要がある.

| Argument name / return | type   | in/out | description                     |
| ---------------------- | ------ | ------ | ------------------------------- |
| out                    | void** | out    | pointer to pointer to Focus STM |
| sound_speed            | double | in     | sound speed                     |
| return                 | void   | -      | -                               |

## AUTDGainSTM (autd3capi)

Gain STMを作成する.

handleは`AUTDCreateController`で作成したものを使う.

作成したSTMは最後に`AUTDDeleteSTM`で削除する必要がある.

| Argument name / return | type   | in/out | description                    |
| ---------------------- | ------ | ------ | ------------------------------ |
| out                    | void** | out    | pointer to pointer to Gain STM |
| handle                 | void*  | in     | pointer to Controller          |
| return                 | void   | -      | -                              |

## AUTDFocusSTMAdd (autd3capi)

Focus STMに焦点を追加する.

| Argument name / return | type    | in/out | description                 |
| ---------------------- | ------- | ------ | --------------------------- |
| stm                    | void*   | in     | pointer to Focus STM        |
| x                      | double  | in     | x coordinate of focal point |
| y                      | double  | in     | y coordinate of focal point |
| z                      | double  | in     | z coordinate of focal point |
| shift                  | uint8_t | in     | duty shift                  |
| return                 | void    | -      | -                           |

## AUTDGainSTMAdd (autd3capi)

Gain STMにgainを追加する.

| Argument name / return | type  | in/out | description          |
| ---------------------- | ----- | ------ | -------------------- |
| stm                    | void* | in     | pointer to Focus STM |
| gain                   | void* | in     | pointer to Gain      |
| return                 | void    | -      | -                  |

## AUTDSetGainSTMMode (autd3capi)

GainSTMのmodeを設定する.

| Argument name / return | type     | in/out | description                                                                   |
| ---------------------- | -------- | ------ | ----------------------------------------------------------------------------- |
| stm                    | void*    | in     | pointer to STM                                                                |
| mode                   | uint16_t | in     | GainSTM mode (0x0001 = PhaseDutyFull, 0x0002 = PhaseFull, 0x0004 = PhaseHalf) |
| return                 | void     | -      | -                                                                             |

## AUTDGetGainSTMMode (autd3capi)

GainSTMのmodeを取得する.

| Argument name / return | type     | in/out | description    |
| ---------------------- | -------- | ------ | -------------- |
| stm                    | void*    | in     | pointer to STM |
| return                 | uint16_t | -      | GainSTM mode   |

## AUTDSTMGetStartIdx (autd3capi)

start index を取得する.

| Argument name / return | type   | in/out | description      |
| ---------------------- | ------ | ------ | ---------------- |
| stm                    | void*  | in     | pointer to STM   |
| return                 | int32_t | -      | start index (if < 0, nullopt) |

## AUTDSTMGetFinishIdx (autd3capi)

finish index を取得する.

| Argument name / return | type   | in/out | description      |
| ---------------------- | ------ | ------ | ---------------- |
| stm                    | void*  | in     | pointer to STM   |
| return                 | int32_t | -      | finish index (if < 0, nullopt) |

## AUTDSTMSetStartIdx (autd3capi)

start index を設定する.

| Argument name / return | type   | in/out | description      |
| ---------------------- | ------ | ------ | ---------------- |
| stm                    | void*  | in     | pointer to STM   |
| start_idx              | int32_t | in      | start index (if < 0, nullopt) |
| return                 | void | -      | - |

## AUTDSTMSetFinishIdx (autd3capi)

finish index を設定する.

| Argument name / return | type   | in/out | description      |
| ---------------------- | ------ | ------ | ---------------- |
| stm                    | void*  | in     | pointer to STM   |
| finish_idx              | int32_t | in     | start index (if < 0, nullopt) |
| return                 | void | -      | - |

## AUTDSTMSetFrequency (autd3capi)

STMのfrequencyを設定する.

| Argument name / return | type   | in/out | description      |
| ---------------------- | ------ | ------ | ---------------- |
| stm                    | void*  | in     | pointer to STM   |
| freq                   | double | in     | frequency of STM |
| return                 | double | -      | actual frequency |

## AUTDSTMFrequency (autd3capi)

STMのfrequencyを取得する.

| Argument name / return | type   | in/out | description      |
| ---------------------- | ------ | ------ | ---------------- |
| stm                    | void*  | in     | pointer to STM   |
| return                 | double | -      | frequency of STM |

## AUTDSTMSamplingFrequency (autd3capi)

STMのsampling frequencyを取得する.

| Argument name / return | type   | in/out | description               |
| ---------------------- | ------ | ------ | ------------------------- |
| stm                    | void*  | in     | pointer to STM            |
| return                 | double | -      | sampling frequency of STM |

## AUTDSTMSamplingFrequencyDivision (autd3capi)

STMのsampling frequency divisionを取得する.

| Argument name / return | type     | in/out | description                     |
| ---------------------- | -------- | ------ | ------------------------------- |
| stm                    | void*    | in     | pointer to STM                  |
| return                 | uint32_t | in     | STM sampling frequency division |

## AUTDSTMSetSamplingFrequencyDivision (autd3capi)

STMのsampling frequency divisionを設定する.

| Argument name / return | type     | in/out | description                     |
| ---------------------- | -------- | ------ | ------------------------------- |
| stm                    | void*    | in     | pointer to STM                  |
| freq_div               | uint32_t | in     | STM sampling frequency division |
| return                 | void     | -      | -                               |

## AUTDDeleteSTM (autd3capi)

STMを削除する.

| Argument name / return | type  | in/out | description    |
| ---------------------- | ----- | ------ | -------------- |
| stm                    | void* | in     | pointer to STM |
| return                 | void  | -      | -              |

## AUTDUpdateFlags (autd3capi)

`UpdateFlag`特殊データを作成する.

作成した`UpdateFlag`はAUTDDeleteSpecialDataで削除する必要がある.

| Argument name / return | type   | in/out | description                    |
| ---------------------- | ------ | ------ | ------------------------------ |
| out                    | void** | out    | pointer to pointer to UpdateFlag |
| return                 | void   | -      | -                              |

## AUTDSynchronize (autd3capi)

`Synchronize`特殊データを作成する.

作成した`Synchronize`はAUTDDeleteSpecialDataで削除する必要がある.

| Argument name / return | type   | in/out | description                    |
| ---------------------- | ------ | ------ | ------------------------------ |
| out                    | void** | out    | pointer to pointer to Synchronize |
| return                 | void   | -      | -                              |

## AUTDStop (autd3capi)

`Stop`特殊データを作成する.

作成した`Stop`はAUTDDeleteSpecialDataで削除する必要がある.

| Argument name / return | type   | in/out | description                    |
| ---------------------- | ------ | ------ | ------------------------------ |
| out                    | void** | out    | pointer to pointer to Stop     |
| return                 | void   | -      | -                              |

## AUTDClear (autd3capi)

`Clear`特殊データを作成する.

作成した`Clear`はAUTDDeleteSpecialDataで削除する必要がある.

| Argument name / return | type   | in/out | description                    |
| ---------------------- | ------ | ------ | ------------------------------ |
| out                    | void** | out    | pointer to pointer to Clear    |
| return                 | void   | -      | -                              |

## AUTDModDelayConfig (autd3capi)

`ModDelayConfig`特殊データを作成する.

作成した`ModDelayConfig`はAUTDDeleteSpecialDataで削除する必要がある.

| Argument name / return | type   | in/out | description                    |
| ---------------------- | ------ | ------ | ------------------------------ |
| out                    | void** | out    | pointer to pointer to ModDelayConfig    |
| return                 | void   | -      | -                              |

## AUTDDeleteSpecialData (autd3capi)

特殊データを削除する.

| Argument name / return | type  | in/out | description    |
| ---------------------- | ----- | ------ | -------------- |
| data                   | void* | in     | pointer to special data |
| return                 | void  | -      | -              |

## AUTDSend (autd3capi)

ヘッダーデータとボディーデータを送信する.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type    | in/out | description                                                                                            |
| ---------------------- | ------- | ------ | ------------------------------------------------------------------------------------------------------ |
| handle                 | void*   | in     | pointer to Controller                                                                                  |
| header                 | void*   | in     | pointer to header data                                                                                 |
| body                   | void*   | in     | pointer to body data                                                                                   |
| return                 | bool    | -      | true if successful                                                                                     |

## AUTDSendSpecial (autd3capi)

特殊データを送信する.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type    | in/out | description                                                                                            |
| ---------------------- | ------- | ------ | ------------------------------------------------------------------------------------------------------ |
| handle                 | void*   | in     | pointer to Controller                                                                                  |
| special                | void*   | in     | pointer to special data                                                                                 |
| return                 | bool    | -      | true if successful                                                                                     |

## AUTDSendAsync (autd3capi)

非同期にヘッダーデータとボディーデータを送信する.

handleは`AUTDCreateController`で作成したものを使う.

**この関数に使用したheader, 及び, bodyは削除してはいけない.**

| Argument name / return | type    | in/out | description                                                                                            |
| ---------------------- | ------- | ------ | ------------------------------------------------------------------------------------------------------ |
| handle                 | void*   | in     | pointer to Controller                                                                                  |
| header                 | void*   | in     | pointer to header data                                                                                 |
| body                   | void*   | in     | pointer to body data                                                                                   |
| return                 | void     | -      | -                                  |

## AUTDSendSpecialAsync (autd3capi)

非同期に特殊データを送信する.

handleは`AUTDCreateController`で作成したものを使う.

**この関数に使用したspecialは削除してはいけない.**

| Argument name / return | type    | in/out | description                                                                                            |
| ---------------------- | ------- | ------ | ------------------------------------------------------------------------------------------------------ |
| handle                 | void*   | in     | pointer to Controller                                                                                  |
| special                | void*   | in     | pointer to special data                                                                                 |
| return                 | void     | -      | -                                  |

## AUTDGetModDelay (autd3capi)

指定した振動子のModulation Delayを取得する.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type     | in/out | description                        |
| ---------------------- | -------- | ------ | ---------------------------------- |
| handle                 | void*    | in     | pointer to Controller              |
| trans_idx        | int32_t  | in     | transducer index             |
| return                 | uint16_t | -      | modulation delay of the transducer |

## AUTDSetModDelay (autd3capi)

指定した振動子のModulation Delayを設定する.

handleは`AUTDCreateController`で作成したものを使う.

| Argument name / return | type     | in/out | description                        |
| ---------------------- | -------- | ------ | ---------------------------------- |
| handle                 | void*    | in     | pointer to Controller              |
| trans_idx        | int32_t  | in     | transducer index             |
| delay                  | uint16_t | in     | modulation delay of the transducer |
| return                 | void     | -      | -                                  |

## AUTDCreateAmplitudes (autd3capi)

Amplitudesを作成する.

作成したSilencerConfigは最後に`AUTDDeleteAmplitudes`で削除する必要がある.

| Argument name / return | type   | in/out | description                      |
| ---------------------- | ------ | ------ | -------------------------------- |
| out                    | void** | out    | pointer to pointer to Amplitudes |
| amp                    | double | in     | amplitudes                       |
| return                 | void   | -      | -                                |

## AUTDDeleteAmplitudes (autd3capi)

Amplitudesを削除する.

| Argument name / return | type  | in/out | description           |
| ---------------------- | ----- | ------ | --------------------- |
| amplitudes             | void* | in     | pointer to Amplitudes |
| return                 | void  | -      | -                     |

## AUTDSoftwareSTM (autd3capi)

SoftwareSTMを作成する.

| Argument name / return | type  | in/out | description                       |
| ---------------------- | ----- | ------ | --------------------------------- |
| out                    | void**| out    | pointer to pointer to SoftwareSTM |
| return                 | void  | -      | -                                 |

## AUTDSoftwareSTMSetStrategy (autd3capi)

SoftwareSTMのTimerStrategyを設定する.

| Argument name / return | type    | in/out | description                       |
| ---------------------- | -----   | ------ | --------------------------------- |
| stm                    | void*   | in     | pointer to SoftwareSTM            |
| strategy               | uint8_t | in     | TimerStrategy                     |
| return                 | void    | -      | -                                 |

## AUTDSoftwareSTMAdd (autd3capi)

SoftwareSTMにGainを追加する.

この関数に追加したGainは削除してはいけない.

| Argument name / return | type    | in/out | description                       |
| ---------------------- | -----   | ------ | --------------------------------- |
| stm                    | void*   | in     | pointer to SoftwareSTM            |
| gain                   | void*   | in     | pointer to Gain                   |
| return                 | void    | -      | -                                 |

## AUTDSoftwareSTMStart (autd3capi)

SoftwareSTMを開始する.

この関数の呼び出し後, Controllerへのアクセスは禁止される.

| Argument name / return | type    | in/out | description                                   |
| ---------------------- | -----   | ------ | --------------------------------------------- |
| handle                 | void**  | out    | pointer to pointer to SoftwareSTMThreadHandle |
| stm                    | void*   | in     | pointer to SoftwareSTM                        |
| cnt                    | void*   | in     | pointer to Controller                         |
| return                 | void    | -      | -                                             |

## AUTDSoftwareSTMFinish (autd3capi)

| Argument name / return | type    | in/out | description                        |
| ---------------------- | -----   | ------ | -----------------------------------|
| handle                 | void*   | in     | pointer to SoftwareSTMThreadHandle |
| return                 | void    | -      | -                                  |


## AUTDSoftwareSTMSetFrequency (autd3capi)

SoftwareSTMの周波数を設定する.

| Argument name / return | type    | in/out | description            |
| ---------------------- | -----   | ------ | -----------------------|
| stm                    | void*   | in     | pointer to SoftwareSTM |
| freq                   | double  | in     | frequency              |
| return                 | double  | -      | actual frequency       |

## AUTDSoftwareSTMFrequency (autd3capi)

SoftwareSTMの周波数を取得する.

| Argument name / return | type    | in/out | description            |
| ---------------------- | -----   | ------ | -----------------------|
| stm                    | void*   | in     | pointer to SoftwareSTM |
| return                 | double  | -      | frequency              |

## AUTDSoftwareSTMPeriod (autd3capi)

SoftwareSTMの周期をナノ秒単位で取得する.

| Argument name / return | type     | in/out | description            |
| ---------------------- | -----    | ------ | -----------------------|
| stm                    | void*    | in     | pointer to SoftwareSTM |
| return                 | uint64_t | -      | period                 |

## AUTDSoftwareSTMSamplingFrequency (autd3capi)

SoftwareSTMのサンプリング周波数を取得する.

| Argument name / return | type    | in/out | description            |
| ---------------------- | -----   | ------ | -----------------------|
| stm                    | void*   | in     | pointer to SoftwareSTM |
| return                 | double  | -      | sampling frequency     |

## AUTDSoftwareSTMSamplingPeriod (autd3capi)

SoftwareSTMのサンプリング周期をナノ秒単位で取得する.

| Argument name / return | type     | in/out | description            |
| ---------------------- | -----    | ------ | -----------------------|
| stm                    | void*    | in     | pointer to SoftwareSTM |
| return                 | uint64_t | -      | sampling period        |

## AUTDSoftwareSTMSetSamplingPeriod (autd3capi)

SoftwareSTMのサンプリング周期をナノ秒単位で設定する.

| Argument name / return | type     | in/out | description            |
| ---------------------- | -----    | ------ | -----------------------|
| stm                    | void*    | in     | pointer to SoftwareSTM |
| period                 | uint64_t | in     | sampling period        |
| return                 | void     | -      | -                      |

## AUTDDeleteSoftwareSTM (autd3capi)

SoftwareSTMを削除する.

| Argument name / return | type  | in/out | description            |
| ---------------------- | ----- | ------ | ---------------------- |
| stm                    | void* | in     | pointer to SoftwareSTM |
| return                 | void  | -      | -                      |

## AUTDSetMode (autd3capi)

Legacy/Normalモードの設定を行う.

| Argument name / return | type    | in/out | description                                                 |
| ---------------------- | ------- | ------ | ----------------------------------------------------------- |
| handle                 | void*   | in     | pointer to Controller                                       |
| mode                   | uint8_t | in     | mode (0: Legacy mode, 1: Normal mode, 2: Normal Phase mode) |
| return                 | void    | -      | -                                                           |

## AUTDEigenBackend (autd3capi-gain-holo)

Eigen Backendを作成する.

作成したBackendは最後に`AUTDDeleteBackend`で削除する必要がある.

| Argument name / return | type   | in/out | description                         |
| ---------------------- | ------ | ------ | ----------------------------------- |
| out                    | void** | out    | pointer to pointer to Eigen backend |
| return                 | void   | -      | -                                   |

## AUTDDeleteBackend (autd3capi-gain-holo)

Backendを作成する.

| Argument name / return | type  | in/out | description        |
| ---------------------- | ----- | ------ | ------------------ |
| backend                | void* | in     | pointer to backend |
| return                 | void  | -      | -                  |

## AUTDGainHoloSDP (autd3capi-gain-holo)

SDP holo gainを作成する.

作成したGainは最後に`AUTDDeleteGain`で削除する必要がある.

| Argument name / return | type     | in/out | description                    |
| ---------------------- | -------- | ------ | ------------------------------ |
| gain                   | void**   | out    | pointer to pointer to SDP gain |
| backend                | void*    | in     | pointer to backend             |
| alpha                  | double   | in     | parameter                      |
| lambda                 | double   | in     | parameter                      |
| repeat                 | uint64_t | in     | parameter                      |
| return                 | void     | -      | -                              |

## AUTDGainHoloEVD (autd3capi-gain-holo)

EVD holo gainを作成する.

作成したGainは最後に`AUTDDeleteGain`で削除する必要がある.

| Argument name / return | type   | in/out | description                    |
| ---------------------- | ------ | ------ | ------------------------------ |
| gain                   | void** | out    | pointer to pointer to EVD gain |
| backend                | void*  | in     | pointer to backend             |
| gamma                  | double | in     | parameter                      |
| return                 | void   | -      | -                              |

## AUTDGainHoloNaive (autd3capi-gain-holo)

Naive holo gainを作成する.

作成したGainは最後に`AUTDDeleteGain`で削除する必要がある.

| Argument name / return | type   | in/out | description                      |
| ---------------------- | ------ | ------ | -------------------------------- |
| gain                   | void** | out    | pointer to pointer to Naive gain |
| backend                | void*  | in     | pointer to backend               |
| return                 | void   | -      | -                                |

## AUTDGainHoloGS (autd3capi-gain-holo)

GS holo gainを作成する.

作成したGainは最後に`AUTDDeleteGain`で削除する必要がある.

| Argument name / return | type     | in/out | description                   |
| ---------------------- | -------- | ------ | ----------------------------- |
| gain                   | void**   | out    | pointer to pointer to GS gain |
| backend                | void*    | in     | pointer to backend            |
| repeat                 | uint64_t | in     | parameter                     |
| return                 | void     | -      | -                             |

## AUTDGainHoloGSPAT (autd3capi-gain-holo)

GSPAT holo gainを作成する.

作成したGainは最後に`AUTDDeleteGain`で削除する必要がある.

| Argument name / return | type     | in/out | description                      |
| ---------------------- | -------- | ------ | -------------------------------- |
| gain                   | void**   | out    | pointer to pointer to GSPAT gain |
| backend                | void*    | in     | pointer to backend               |
| repeat                 | uint64_t | in     | parameter                        |
| return                 | void     | -      | -                                |

## AUTDGainHoloLM (autd3capi-gain-holo)

LM holo gainを作成する.

作成したGainは最後に`AUTDDeleteGain`で削除する必要がある.

| Argument name / return | type     | in/out | description                   |
| ---------------------- | -------- | ------ | ----------------------------- |
| gain                   | void**   | out    | pointer to pointer to LM gain |
| backend                | void*    | in     | pointer to backend            |
| eps_1                  | double   | in     | parameter                     |
| eps_2                  | double   | in     | parameter                     |
| tau                    | double   | in     | parameter                     |
| k_max                  | uint64_t | in     | parameter                     |
| initial                | double*  | in     | initial guess                 |
| initial_size           | int32_t  | in     | size of initial               |
| return                 | void     | -      | -                             |

## AUTDGainHoloGreedy (autd3capi-gain-holo)

Greedy holo gainを作成する.

作成したGainは最後に`AUTDDeleteGain`で削除する必要がある.

| Argument name / return | type    | in/out | description                       |
| ---------------------- | ------- | ------ | --------------------------------- |
| gain                   | void**  | out    | pointer to pointer to Greedy gain |
| backend                | void*   | in     | pointer to backend                |
| phase_div              | int32_t | in     | parameter                         |
| return                 | void    | -      | -                                 |


## AUTDGainHoloLSSGreedy (autd3capi-gain-holo)

LSSGreedy holo gainを作成する.

作成したGainは最後に`AUTDDeleteGain`で削除する必要がある.

| Argument name / return | type    | in/out | description                          |
| ---------------------- | ------- | ------ | ---------------------------------    |
| gain                   | void**  | out    | pointer to pointer to LSSGreedy gain |
| backend                | void*   | in     | pointer to backend                   |
| phase_div              | int32_t | in     | parameter                            |
| return                 | void    | -      | -                                    |

## AUTDGainHoloAPO (autd3capi-gain-holo)

APO holo gainを作成する.

作成したGainは最後に`AUTDDeleteGain`で削除する必要がある.

| Argument name / return | type    | in/out | description                          |
| ---------------------- | ------- | ------ | ---------------------------------    |
| gain                   | void**  | out    | pointer to pointer to APO gain       |
| backend                | void*   | in     | pointer to backend                   |
| eps                    | double  | in     | parameter                            |
| lambda                 | double  | in     | parameter                            |
| k_max                  | int32_t | in     | parameter                            |
| line_search_max        | int32_t | in     | parameter                            |
| return                 | void    | -      | -                                    |

## AUTDGainHoloAdd (autd3capi-gain-holo)

Holo gainに焦点を追加する.

| Argument name / return | type   | in/out | description               |
| ---------------------- | ------ | ------ | ------------------------- |
| gain                   | void*  | in     | pointer to holo gain      |
| x                      | double | in     | x coordinate of the focus |
| y                      | double | in     | y coordinate of the focus |
| z                      | double | in     | z coordinate of the focus |
| amp                    | double | in     | amplitude of the focus    |
| return                 | void   | -      | -                         |

## AUTDConstraintDontCare (autd3capi-gain-holo)

DontCare AmplitudeConstraintを作成する.

作成したAmplitudeConstraintは必ずAUTDSetConstraintで使用する必要がある.

| Argument name / return | type    | in/out | description                                                           |
| ---------------------- | ------- | ------ | --------------------------------------------------------------------- |
| constraint             | void**  | out    | pointer to pointer to constraint                                      |
| return                 | void    | -      | -                                                                     |

## AUTDConstraintNormalize (autd3capi-gain-holo)

Normalize AmplitudeConstraintを作成する.

作成したAmplitudeConstraintは必ずAUTDSetConstraintで使用する必要がある.

| Argument name / return | type    | in/out | description                                                           |
| ---------------------- | ------- | ------ | --------------------------------------------------------------------- |
| constraint             | void**  | out    | pointer to pointer to constraint                                      |
| return                 | void    | -      | -                                                                     |

## AUTDConstraintUniform (autd3capi-gain-holo)

Uniform AmplitudeConstraintを作成する.

作成したAmplitudeConstraintは必ずAUTDSetConstraintで使用する必要がある.

| Argument name / return | type    | in/out | description                                                           |
| ---------------------- | ------- | ------ | --------------------------------------------------------------------- |
| constraint             | void**  | out    | pointer to pointer to constraint                                      |
| value                  | double  | in     | uniform amplitude                                                     |
| return                 | void    | -      | -                                                                     |

## AUTDConstraintClamp (autd3capi-gain-holo)

Clamp AmplitudeConstraintを作成する.

作成したAmplitudeConstraintは必ずAUTDSetConstraintで使用する必要がある.

| Argument name / return | type    | in/out | description                                                           |
| ---------------------- | ------- | ------ | --------------------------------------------------------------------- |
| constraint             | void**  | out    | pointer to pointer to constraint                                      |
| return                 | void    | -      | -                                                                     |

## AUTDSetConstraint (autd3capi-gain-holo)

Holo GainにAmplitudeConstraintを設定する.

| Argument name / return | type    | in/out | description                                                           |
| ---------------------- | ------- | ------ | --------------------------------------------------------------------- |
| gain                   | void*   | in     | pointer to holo gain                                                  |
| constraint             | void*   | in     | pointer to AmplitudeConstraint                                        |
| return                 | void    | -      | -                                                                     |

## AUTDBLASBackend (autd3capi-backend-blas)

BLAS Backendを作成する.

作成したBackendは最後に`AUTDDeleteBackend`で削除する必要がある.

| Argument name / return | type   | in/out | description                        |
| ---------------------- | ------ | ------ | ---------------------------------- |
| out                    | void** | out    | pointer to pointer to BLAS backend |
| return                 | void   | -      | -                                  |

## AUTDCUDABackend (autd3capi-backend-cuda)

CUDA Backendを作成する.

作成したBackendは最後に`AUTDDeleteBackend`で削除する必要がある.

| Argument name / return | type   | in/out | description                        |
| ---------------------- | ------ | ------ | ---------------------------------- |
| out                    | void** | out    | pointer to pointer to CUDA backend |
| return                 | void   | -      | -                                  |

## AUTDLinkBundle (autd3capi-link-bundle)

Create a Bundle link.

| Argument name / return | type     | in/out | description                         |
| ---------------------- | -------- | ------ | ----------------------------------- |
| out                    | void**   | out    | pointer to pointer to Debug link    |
| links                  | void**   | in     | link pointer array                  |
| n                      | int32_t  | in     | size of links                       |
| return                 | void     | -      | -                                   |

## AUTDLinkDebug (autd3capi-link-debug)

Debug linkを作成する.

| Argument name / return | type     | in/out | description                         |
| ---------------------- | -------- | ------ | ----------------------------------- |
| out                    | void**   | out    | pointer to pointer to Debug link    |
| return                 | void     | -      | -                                   |

## AUTDLinkDebugSetLevel (autd3capi-link-debug)

Debugのログ表示レベルを設定する.

| Argument name / return | type     | in/out | description                         |
| ---------------------- | -------- | ------ | ----------------------------------- |
| level                  | int32_t  | in     | debug level                         |
| return                 | void     | -      | -                                   |

## AUTDLinkSimulator (autd3capi-link-simulator)

Simulator linkを作成する.

| Argument name / return | type     | in/out | description                         |
| ---------------------- | -------- | ------ | ----------------------------------- |
| out                    | void**   | out    | pointer to pointer to Simulator link |
| return                 | void     | -      | -                                   |

## AUTDLinkRemoteTwinCAT (autd3capi-link-remote-twincat)

RemoteTwinCAT linkを作成する.

| Argument name / return | type   | in/out | description                              |
| ---------------------- | ------ | ------ | ---------------------------------------- |
| out                    | void** | out    | pointer to pointer to RemoteTwinCAT link |
| server_ip_addr         | char*  | in     | server ip address                        |
| server_ams_net_id      | char*  | in     | server ams net id                        |
| client_ams_net_id      | char*  | in     | client ams net id                        |
| return                 | void   | -      | -                                        |

## AUTDGetAdapterPointer (autd3capi-link-soem)

Adapter listへのポインタを取得する.

この関数で作成したlistは最後に`AUTDFreeAdapterPointer`で開放する必要がある.

実際のAdapterの情報は`AUTDGetAdapter`で取得する.

この関数はAdapter listのサイズを返す.

| Argument name / return | type    | in/out | description                        |
| ---------------------- | ------- | ------ | ---------------------------------- |
| out                    | void**  | out    | pointer to pointer to adapter list |
| return                 | int32_t | -      | size of adapter list               |

## AUTDGetAdapter (autd3capi-link-soem)

アダプターの名前と説明を取得する.

`p_adapter`は`AUTDGetAdapterPointer`で作成したものを使う.

`desc`, `name`はそれぞれ長さ128のバッファを渡せば十分である.

| Argument name / return | type    | in/out | description                    |
| ---------------------- | ------- | ------ | ------------------------------ |
| p_adapter              | void*   | in     | pointer to adapter list        |
| index                  | int32_t | in     | index                          |
| desc                   | char*   | out    | pointer to adapter description |
| name                   | char*   | out    | pointer to adapter name        |
| return                 | void    | -      | -                              |

## AUTDFreeAdapterPointer (autd3capi-link-soem)

Adapter listへのポインタを削除する.

| Argument name / return | type  | in/out | description             |
| ---------------------- | ----- | ------ | ----------------------- |
| p_adapter              | void* | in     | pointer to adapter list |
| return                 | void  | -      | size of adapter list    |

## AUTDLinkSOEM (autd3capi-link-soem)

| Argument name / return | type     | in/out | description                        |
| ---------------------- | -------- | ------ | ---------------------------------- |
| out                    | void**   | out    | pointer to pointer to TwinCAT link |
| ifname                 | char*    | in     | interface name                     |
| sync0_cycle            | uint16_t | in     | sync0 cycle                        |
| send_cycle             | uint16_t | in     | send cycle                         |
| freerun                | bool     | in     | free run mode                      |
| on_lost                | void*    | in     | pointer to on-lost callback        |
| high_precision         | bool     | in     | high precision mode                |
| state_check_interval   | uint64_t | in     | state check interval in ms         |
| return                 | void     | -      | -                                  |

## AUTDLinkSOEMSetLogLevel (autd3capi-link-soem)

| Argument name / return | type     | in/out | description                        |
| ---------------------- | -------- | ------ | ---------------------------------- |
| level                  | int32_t  | in     | log level                          |
| return                 | void     | -      | -                                  |

## AUTDLinkSOEMSetDefaultLogger (autd3capi-link-soem)

| Argument name / return | type     | in/out | description                        |
| ---------------------- | -------- | ------ | ---------------------------------- |
| out                    | void*    | in     | output callback                    |
| flush                  | void*    | in     | flush callback                     |
| return                 | void     | -      | -                                  |

## AUTDLinkRemoteSOEM (autd3capi-link-remote-soem)

Create RemoteSOEM link.

| Argument name / return | type    | in/out | description                              |
| ---------------------- | ------  | ------ | ---------------------------------------- |
| out                    | void**  | out    | pointer to pointer to RemoteSOEM link    |
| ip                     | char*   | in     | server ip address                        |
| port                   | uint16_t| in     | port                                     |
| return                 | void    | -      | -                                        |

## AUTDLinkTwinCAT (autd3capi-link-twincat)

TwinCAT linkを作成する.

| Argument name / return | type   | in/out | description                        |
| ---------------------- | ------ | ------ | ---------------------------------- |
| out                    | void** | out    | pointer to pointer to TwinCAT link |
| return                 | void   | -      | -                                  |

## AUTDModulationRawPCM (autd3capi-modulation-audio-file)

RawPCM modulationを作成する.

作成したModulationは最後に`AUTDDeleteModulation`で削除する必要がある.

| Argument name / return | type     | in/out | description                            |
| ---------------------- | -------- | ------ | -------------------------------------- |
| mod                    | void**   | out    | pointer to pointer to Sine modulation  |
| filename               | char*    | in     | path to pcm file                       |
| sampling_freq          | double   | in     | pcm sampling frequency                 |
| mod_sampling_freq_div  | uint32_t | in     | modulation sampling frequency division |
| return                 | void     | -      | -                                      |

## AUTDModulationWav (autd3capi-modulation-audio-file)

Wav modulationを作成する.

作成したModulationは最後に`AUTDDeleteModulation`で削除する必要がある.

| Argument name / return | type     | in/out | description                            |
| ---------------------- | -------- | ------ | -------------------------------------- |
| mod                    | void**   | out    | pointer to pointer to Sine modulation  |
| filename               | char*    | in     | path to pcm file                       |
| mod_sampling_freq_div  | uint32_t | in     | modulation sampling frequency division |
| return                 | void     | -      | -                                      |

## AUTDExtraGeometryViewer (autd3capi-extra-geometry-viewer)

Geometry Viewerを起動する.

| Argument name / return | type    | in/out | description                        |
| ---------------------- | ------- | ------ | ---------------------------------- |
| cnt                    | void*   | in     | pointer to Controller              |
| width                  | int32_t | in     | window width                       |
| height                 | int32_t | in     | window height                      |
| vsync                  | bool    | in     | vsync                              |
| gpu_idx                | int32_t | in     | GPU index                          |
| return                 | bool    | -      | true if successful                                                                                     |

## AUTDExtraSimulator (autd3capi-extra-simulator)

Simulatorを起動する.

`settings_path`に設定ファイルが存在する場合, `vsync`, `gpu_idx`は設定ファイルの内容が優先される.

| Argument name / return | type    | in/out | description                        |
| ---------------------- | ------- | ------ | ---------------------------------- |
| settings_path          | char*   | in     | path to setting file               |
| vsync                  | bool    | in     | vsync                              |
| gpu_idx                | int32_t | in     | GPU index                          |
| return                 | bool    | -      | true if successful                                                                                     |
