# nice-view-battery-peripheral

This is my fork of [nice-view-battery](https://github.com/infely/nice-view-battery-peripheral), which changes the original by adding a second battery display to show the battery of two split peripheral devices at once. More importantly, is uses my [zmk-split-status-relay](https://github.com/dmhuisma/zmk-split-status-relay) module to allow it to display useful information when used on a split peripheral. Normally displays on split peripherals are quite limited to what they can show, but this operates with feature parity to the original split central version of the module this was forked from.

> [!NOTE] 
> The index of each split peripheral is determined by the order they are paired. So you likely want to connect left splits before the right one.

The original README of this module is shown below.

![Preview](https://github.com/dmhuisma/nice-view-battery-peripheral/blob/main/.github/assets/preview.jpg?raw=true)

### All You Need: nice!view Battery

I've spent time developing practical customization for the nice!view displays, focusing on what truly matters. No unnecessary features, no input lag - just a clean layout provides everything when you need it at a quick glance. Enjoy!

Add this shield to your keymap repo (see usage below) and run the GitHub action to build your firmware.

## Customization

My preference is to comment out the `display-name` for your base layer so it highlights changes only, keeping layout clean and visualy appealing. But the final decision on you.

![Layer](https://github.com/dmhuisma/nice-view-battery-peripheral/blob/main/.github/assets/layer.jpg?raw=true)

## Usage

To use this shield, first add it to your `config/west.yml` by adding a new entry to remotes and projects:

```yml
manifest:
  remotes:
    - name: zmkfirmware
      url-base: https://github.com/zmkfirmware
    - name: infely                             # new entry
      url-base: https://github.com/infely      # new entry
  projects:
    - name: zmk
      remote: zmkfirmware
      revision: main
      import: app/west.yml
    - name: nice-view-battery                  # new entry
      remote: infely                           # new entry
      revision: main                           # new entry
  self:
    path: config
```

Now, simply swap out the default nice_view shield on the board for nice_view_battery in your `build.yaml` file.

```yml
---
include:
  - board: nice_nano_v2
    shield: corne_left nice_view_adapter nice_view_battery  # updated entry
  - board: nice_nano_v2
    shield: corne_right nice_view_adapter nice_view_battery # updated entry
```

## Configuration

Modify the behavior of this shield by adjusting these options in your personal configuration files. For a more detailed explanation, refer to [Configuration in the ZMK documentation](https://zmk.dev/docs/config).

| Option                             | Type | Description                                      | Default |
| ---------------------------------- | ---- | ------------------------------------------------ | ------- |
| `CONFIG_NICE_VIEW_WIDGET_INVERTED` | bool | To enable color inversion, set the option to `y` | n       |
