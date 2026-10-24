# nice-view-battery-peripheral

A [nice!view](https://nicekeyboards.com/nice-view) status screen for **split peripherals** in a dongle setup (the dongle is the central and has no display).

This is a fork of [nice-view-battery](https://github.com/infely/nice-view-battery) by infely, via [dmhuisma's fork](https://github.com/dmhuisma/nice-view-battery-peripheral). Normally a display on a split peripheral can only show its own battery and whether it is connected to the central. This module uses [zmk-split-status-relay](https://github.com/dmhuisma/zmk-split-status-relay), which sends status from the dongle to the peripherals over [zmk_arbitrary_split_data_channel](https://github.com/dmhuisma/zmk_arbitrary_split_data_channel), so the peripheral display can show:

- The battery level and charging state of both split peripherals (or only the half the display is on, see [Configuration](#configuration))
- The active layer
- The active output (USB or BLE) and BLE profile status

> [!NOTE]
> The index (slot) of each split peripheral is determined by the order they are paired with the dongle. So you likely want to connect the left half before the right one.

![Preview](https://github.com/dmhuisma/nice-view-battery-peripheral/blob/main/.github/assets/preview.jpg?raw=true)

## Customization

The layer name is taken from the `display-name` of each layer in your keymap. My preference is to comment out the `display-name` for your base layer so the screen highlights changes only, keeping the layout clean and visually appealing. But the final decision is yours.

![Layer](https://github.com/dmhuisma/nice-view-battery-peripheral/blob/main/.github/assets/layer.jpg?raw=true)

## Usage

### 1. Add the modules

This shield needs the split status relay and arbitrary split data channel modules. Add all three to your `config/west.yml`:

```yml
manifest:
  remotes:
    - name: zmkfirmware
      url-base: https://github.com/zmkfirmware
    - name: dmhuisma                           # new entry
      url-base: https://github.com/dmhuisma    # new entry
  projects:
    - name: zmk
      remote: zmkfirmware
      revision: main
      import: app/west.yml
    - name: nice-view-battery-peripheral       # new entry
      remote: dmhuisma                         # new entry
      revision: zmk_0.4                        # new entry
    - name: zmk_arbitrary_split_data_channel   # new entry
      remote: dmhuisma                         # new entry
      revision: main                           # new entry
    - name: zmk-split-status-relay             # new entry
      remote: dmhuisma                         # new entry
      revision: main                           # new entry
  self:
    path: config
```

If you are building against ZMK 0.4, check which branch of each module supports it (for example `zmk_0.4`) and use that as the `revision`.

### 2. Enable the relay in your keyboard's devicetree

Define the data channel and status relay nodes, and enable them (`status = "okay"`) on the dongle **and** on both split peripherals:

```c
/ {
    asdc0: split_data_channel {
        compatible = "zmk,arbitrary-split-data-channel";
        channel-id = <1>;
        status = "okay";
    };

    ssr0: split_status_relay {
        compatible = "zmk,split-status-relay";
        asdc-channel = <&asdc0>;
        status = "okay";
    };
};
```

### 3. Configure the Bluetooth buffers

The data channel uses L2CAP, so the buffer sizes must be large enough for its packets. Add the settings from the [zmk_arbitrary_split_data_channel README](https://github.com/dmhuisma/zmk_arbitrary_split_data_channel) (`CONFIG_BT_L2CAP_TX_MTU`, `CONFIG_BT_BUF_ACL_*`, `CONFIG_BT_CTLR_DATA_LENGTH_MAX`, `CONFIG_BT_L2CAP_DYNAMIC_CHANNEL`, etc.) to the config of the dongle and the peripherals. The dongle's `ZMK_SPLIT_BLE_CENTRAL_PERIPHERALS` must also match the number of peripherals.

### 4. Add the shield to your build

Add `nice_view_battery_peripheral` to the **peripheral** builds in your `build.yaml`. The dongle does not need it. The keyboard shield must expose `nice_view_header` (or you must add a `nice_view_adapter`) and provide the `nice_view_spi` bus.

```yml
---
include:
  - board: nice_nano_v2
    shield: mykeyboard_left nice_view_battery_peripheral   # updated entry
  - board: nice_nano_v2
    shield: mykeyboard_right nice_view_battery_peripheral  # updated entry
  - board: nice_nano_v2
    shield: mykeyboard_dongle                               # no display shield needed
```

## Configuration

Modify the behavior of this shield by adjusting these options in your personal configuration files. For a more detailed explanation, refer to [Configuration in the ZMK documentation](https://zmk.dev/docs/config).

| Option                             | Type | Description                                      | Default |
| ---------------------------------- | ---- | ------------------------------------------------ | ------- |
| `CONFIG_NICE_VIEW_WIDGET_INVERTED` | bool | To enable color inversion, set the option to `y` | n       |
| `CONFIG_NICE_VIEW_BATTERY_PERIPHERAL_SLOT` | int | Show only the battery of one peripheral slot. Set it to `0` on the half paired first and `1` on the half paired second. `-1` shows both batteries | -1      |
