# v1.xからの移行ガイド

ここでは, v1.x系からの移行のためのガイドを示す.

## 削除された機能

### Output enable, Output balance flag

これらのフラグは削除された.

### pause, resume

これらの関数は削除された.

出力を止めたい場合は`stop`を呼ぶか`modulation::Static(0.0)`を送信する.
また, 出力を再開したい場合は改めて所望のデータを送信すること.

### Duty Offset指定

この機能は削除された.

## 変更されたAPI

### Silent mode

Silent modeのフラグは削除された.

代わりに, `SilentConfig`を`send`することより細かく静音化の調整ができるようになった.

デフォルトの`SilentConfig`が従来の`silent_mode = true`と概ね等価であり, `SilentConfig::none()`が`silent_mode = false`と等価である.

詳細は[Silencer](silencer.md)を参照されたい.

### Synchronize

`Controller::synchronize`関数を最初に必ず一度呼び出すこと.
