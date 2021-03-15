# FreeGlove OpenVR Driver - LucidVR X Fngrs
Developed by:
* Danwillm
* Lucas_VRTech

## What is this?
This repository contains a OpenVR(SteamVR) driver for building VR Haptic Gloves.

This repository is open for anyone to use wanting to make their own custom VR hardware, specifically VR Gloves. However, we hope for this project to expand to support more generic hardware, such as treadmills, shoes, etc.

## Officically Compatible Hardware:
* LucidVR Gloves - Prototype 3+ (VR Glove)
* Fngrs by danwillm (VR Glove)
* If you've made you're own, we would love to know and incorporate into this project!

__This Repository is a *very early* work in progress - many things are likely to change.__
As such, this repo is not in a stable release state yet.

With that in mind, binaries are available in the releases section, or instructions for building from source code are in the Wiki.

Releases will be available on Steam once the repo hits a stable release.

That said, we are more than happy to help with issues that may arise, skip to the bottom for how to contact us.

## Currently supported:
* Finger flexion tracking
* Positioning from a controller
* Button and joystick inputs
* Communication Protocols:
  - Serial over USB

## Features that are almost certainly going to be supported:
* Positioning from a tracker (If you have some available and don't mind running test driver builds, please do contact us!)
* Communication Protocols:
  - Bluetooth LE
  - Single port serial

### Considered additions:
* Finger splay tracking
* Force feedback haptics
* Communication Protocols:
  - 2.4ghz wifi
  - Single port serial
  - Hex compression for serial

### Games tested:
* Half Life Alyx (compatible)
* SteamVR Home (compatible)
* The Lab (compatible)
* Aperture Hand Lab (compatible)
* Pavlov (compatible)
* Boneworks (known issues)
* Blade and Sorcery (known issues)
* VRChat (NOT compatible due to VRChat's bindings, but we are looking into it.)
* Have a game you've tested that does/doesn't work? Please let us know!

We are actively welcoming contributions, feel free to open a pull request.

If you run into issues, you are more than welcome to open a GitHub issue/discussion, or contact us directly on Discord: 

`danwillm#8254` - Driver maintainer

`LucidVR#0001` - Hardware maintainer
