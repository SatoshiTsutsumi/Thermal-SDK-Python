# Thermal-SDK-Python
## What Is This?
A python binding implementation for Thermal SDK.

## Get Started
### Install Thermal SDK library

```
$ git clone git@github.com:SatoshiTsutsumi/Thermal-SDK-Python.git
$ cd Thermal-SDK-Python
$ wget https://dl.djicdn.com/downloads/dji_assistant/20201125/dji_thermal_sdk_v1.0_20201110.zip 
$ unzip dji_thermal_sdk_v1.0_20201110.zip
$ mv dji_thermal_sdk_v1.0_20201110 sdk
$ sudo cp sdk/tsdk-core/lib/linux/release_x64/libdirp.so /usr/local/lib
$ sudo ldconfig
```

### Install Thermal-SDK-Python (rjpeg) 
```
$ python3 setup.py install
```

### Test
```
$ python
Python 3.8.0 (default, Feb 25 2021, 22:10:10) 
[GCC 8.4.0] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> import rjpeg
>>> print(rjpeg.__version__)
0.0.1
```

## Example
```
import numpy as np
from PIL import Image
from rjpeg import *

# Register app
dirp_register_app("DJI_TSDK")

with open("example.jpg", "rb") as f:
        # Get DIRP handle
        buf = f.read()
        h = dirp_create_from_rjpeg(buf, len(buf))

        # Get resolution
        resol = dirp_get_rjpeg_resolution(h)
        
        # Get colored image
        color_img_buf = dirp_process(h)
        color_img_array = np.array(color_img_buf, dtype=np.uint8)
        color_img_array = color_img_array.reshape([resol.height, resol.width, 3])
        color_img = Image.fromarray(color_img_array)
        color_img.show()
```

