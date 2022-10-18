# AUTD3

AUTD3はスケーラブルな超音波フェーズドアレイである.
スケーラブルとは即ち, 複数のデバイスを相互に接続して, 単一の大きなアレイを構成することができる, という意味である.
AUTD3は以下の特徴を備えている.

- 複数のデバイスを相互に接続し, デバイス間を$\SI{100}{ns}$程度の精度で同期できる
- AUTD3アレイを構成するすべての振動子の振幅/位相を自由に制御できる
    - 更新レートは通信速度依存, 通常は最大で$\SI{1}{kHz}$
- すべての振動子の周波数を振動子毎に設定できる
- AM変調機能
- 静音化機能
- 高更新レートの時空間変調機能

## 同期

$\SI{40}{kHz}$の超音波の周期は$\SI{25}{\text{μs}}$であるので, すべての振動子が$\SI{1}{\text{μs}}$程度の精度で同期していることが望ましい.

AUTD3は通信プロトコルにEtherCATを採用することで, これを実現している.
EtherCATネットワークは一つのmaster (例えば, PC) と複数のslave (AUTD3デバイス) から構成され, Distribution clockという機能により, これらの時刻を$\SI{100}{ns}$程度の精度で同期させることができる.

AUTD3はこの機能を利用して, $\SI{100}{ns}$程度の同期を行っている.

## 振幅/位相制御

AUTD3は様々な音場を生成するために, すべての振動子の振幅と位相を個別に制御できるようにしている.

振幅と位相の分解能は, $1/\SI{163.84}{MHz} \sim \SI{6.1}{ns}$である.
例えば, 周波数を$\SI{40}{kHz}$に設定した場合は1周期が$\SI{25}{\text{μ}s}$であるので, 振幅と位相は$\SI{25}{\text{μ}s}\times \SI{163.84}{MHz} = 4096$段階で指定できる.

## 周波数制御

各振動子の周波数は
$$
    \frac{\SI{163.84}{MHz}}{T}
$$
のように指定できる. ここで, $T$は$\SI{13}{bit}$で指定できる.

## AM機能

AUTD3はヒトの触覚を効率的に発火させるために振幅変調 (AM) をかける機能を持つ.

AMは予め変調データ列を格納したバッファから一定の間隔で変調データ$m$を順番に周期的にサンプリングし, 振幅データ$D$に掛け合わせることで実現される.
変調データは$\SI{8}{bit}$で指定され, 変調後の振幅データ$D_m$は
$$
    D_m = \frac{m \times D}{255}
$$
となる.

また, バッファのサイズは最大$65536$であり, サンプリング周波数は
$$
    \frac{\SI{163.84}{MHz}}{T_m}
$$
のように指定する. ここで, $T_m$は$\SI{32}{bit}$で指定できる.

## 時空間変調機能

ヒトの触覚を効率的に発火させるもう一つの方法として, 時空間変調がある.
これはAMと異なり, 焦点を動かすことによって刺激する方法である.
AUTD3では, この機能をSTMと呼んでいる.

これは, 振幅/位相を適宜更新することによって実現できる.
しかし, 更新レートが通信速度によって制限されてしまう上に, 時間間隔の精度がEtherCAT masterのタイマの精度 (通常のPCでは$\SI{1}{ms}$程度) に依存する.
そのため, この方法ではたかだか$\SI{1}{kHz}$程度の更新レートしか実現できない.
そこでAUTD3では, AMのように時空間変調データを予めバッファに格納しここから周期的にサンプリングすることで, AUTD3のハードウェアの高精度なタイマによるSTMを実現する機能を実装している.

AUTD3のSTMには2つのモードがある.
- 単一焦点STM (Point STM)
- 任意振幅位相STM (Gain ST)

Point STMでは, 単一焦点のみ動かす事ができるが, バッファサイズが$65536$となっている.
Gain STMでは, 任意の振幅位相によるSTMを利用できるが, バッファサイズは$1024$となっている.

STMデータのサンプリング周波数は, どちらのモードでも
$$
    \frac{\SI{163.84}{MHz}}{T_s}
$$
のように指定する. ここで, $T_s$は$\SI{32}{bit}$で指定できる.