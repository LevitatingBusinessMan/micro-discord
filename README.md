# Micro-Discord
A micro plugin to enable Discord Rich Presence.
It's still in early stages and very buggy.
Because [GopherLua](https://github.com/yuin/gopher-lua) doesn't have an `ffi` module, I can't bind to the RPC SDK directly.
Instead this plugin comes with tiny daemon I wrote in C which the plugin runs in the background and communicates with.

This plugin will only work on linux

### Install
This plugin isn't yet in the repositories
To install clone this repo and copy it to `~/.config/micro/plug/`
```BASH
git clone https://github.com/LevitatingBusinessMan/micro-discord.git ~/.config/micro/plug/discord
```