# Waker Online [MMORPG]
[![forthebadge](https://forthebadge.com/images/badges/built-with-love.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)

A C++ based MMORPG. Vulkan based & raytraced. Long term educational / life project.

* This repository is currently prealpha and documentation is near absent.

# Table of contents

- [Plans](#plans)
- [Installation](#installation)
- [Usage](#updating)
- [Uninstallation](#uninstallation)
- [Contributing](#contributing)
- [License](#license)

# Plans

[(Back to top)](#table-of-contents)

   * Here is a list of things I'll accomplish eventually:

      - Vulkan RTX multithreaded 3D rendering
      - P2P networking
      - User level/boss/dungeon/island editor
      - Full expressive Game UI
      - Machine learning based enemy intelligence
      - Vulkan RTX multithreaded 3D spatial sound computation
      - Proximity chat

# Usage

[(Back to top)](#table-of-contents)

# Installation

[(Back to top)](#table-of-contents)

1. Install dependencies
    * premake (version >= 5.0 alpha),
    * glfw
    * vulkan

2. Run preliminary submodule downloads

  ```
  git submodule update --init
  ```
3. Run premake build generation
  *Note configuration options --logging to enable debug logging
  *Note premake5 --help for a full list of build options

  ```
  premake5 gmake2 --logging
  ```

4. Run make on desired configuration

  *Note debug has Vulkan Validation layers included
  ```
  make config=debug
  make config=release
  ```
5. Run build in binary directory

  ```
  ./waker-online/waker-online/binary/debug/linux/waker-online/waker-online
  ./waker-online/waker-online/binary/release/linux/waker-online/waker-online
  ```

# Uninstallation

[(Back to top)](#table-of-contents)

Build is currently only local; simply delete the repository.

  ```
  sudo rm -r waker-online
  ```
# Contributing

[(Back to top)](#table-of-contents)

No Guidelines are currently set. They will be specified in [contribution guidelines](CONTRIBUTING.md)

# License

[(Back to top)](#table-of-contents)

This is subject to change once development reaches beta stage (I won't want it to be used commercially.). The liscensing provided is only valid while it exists in the respository.

MIT Liscense 2021 - [Robbie VanDerzee](https://github.com/robbie-vanderzee/). Please have a look at the [LICENSE.md](LICENSE.md) for more details.
