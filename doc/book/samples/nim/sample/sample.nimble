# Package

version       = "2.6.5"
author        = "shun suzuki"
description   = "An autd3 sample"
license       = "MIT"
srcDir        = "src"
bin           = @["sample"]


# Dependencies

requires "nim >= 1.6.6"
requires "https://github.com/shinolab/autd3.git?subdir=nim == 2.6.5"