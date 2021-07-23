"""
setup.py

Copyright (c) 2021 SatoshiTsutsumi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""
from setuptools import setup
from setuptools import find_packages
from pybind11.setup_helpers import Pybind11Extension, build_ext
from pybind11 import get_cmake_dir

import inspect
import os
import platform
import shutil
import sys

__version__ = "0.0.1"

def get_dirp_dir():
    system = platform.system().lower()
    arch = 'x64' if platform.architecture()[0] == '64bit' else 'x86'
    return 'sdk/tsdk-core/lib/' + system + '/release_' + arch

ext_modules = [
    Pybind11Extension(
        "rjpeg",
        ["src/rjpeg_module.cpp"],
        package_dir='rjpeg',
        define_macros = [('VERSION_INFO', '"' + __version__ + '"')],
        include_dirs=['sdk/tsdk-core/api'],
        library_dirs=[get_dirp_dir()],
        libraries=['dirp']
        ),
]

setup(
    name="rjpeg",
    version=__version__,
    author="Satoshi Tsutsumi",
    author_email="satoshi.tsutsumi@gmail.com",
    url="https://github.com/SatoshiTsutsumi/Thermal-SDK-Python",
    description="Thermal SDK for Python",
    long_description="",
    ext_modules=ext_modules,
    install_requires=['pybind11>=2.7'],
    extras_require={"test": "pytest"},
    cmdclass={"build_ext": build_ext},
    zip_safe=False
)
