/*
rjpeg_module.cpp

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
 */
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "dirp_api.h"

namespace py = pybind11;

typedef struct {
    uint32_t api;
    std::vector<char> magic;
} __dirp_api_version_t;

typedef struct {
    std::vector<std::vector<uint8_t>> red;
    std::vector<std::vector<uint8_t>> green;
    std::vector<std::vector<uint8_t>> blue;
} __dirp_isp_pseudo_color_lut_t;

py::object __dirp_create_from_rjpeg(py::buffer& data, int32_t size) 
{
    DIRP_HANDLE ph;
    py::buffer_info info = data.request();
    auto ptr = static_cast<uint8_t*>(info.ptr);

    int32_t ret = dirp_create_from_rjpeg(ptr, size, &ph);
    if ( ret != DIRP_SUCCESS ) {
        return py::cast<py::none>(Py_None);
    } else {
        return py::cast(ph);
    }
}

int32_t __dirp_destroy(DIRP_HANDLE h)
{
    return dirp_destroy(h);
}

py::object __dirp_get_api_version()
{
    dirp_api_version_t version;
    int32_t ret = dirp_get_api_version(&version);
    if ( ret != DIRP_SUCCESS ) {
        return py::cast<py::none>(Py_None);
    } else {
        __dirp_api_version_t __version = { 
            version.api, 
            std::vector<char>(version.magic, version.magic + sizeof(version.magic) / sizeof(version.magic[0]))};
        return py::cast(__version);
    }
}

py::object __dirp_get_rjpeg_version(DIRP_HANDLE h)
{
    dirp_rjpeg_version_t version;
    int32_t  ret = dirp_get_rjpeg_version(h, &version);
    if ( ret != DIRP_SUCCESS ) {
        return py::cast<py::none>(Py_None);
    } else {
        return py::cast(version);
    }
}

py::object __dirp_get_rjpeg_resolution(DIRP_HANDLE h)
{
    dirp_resolotion_t resolution;
    int32_t ret = dirp_get_rjpeg_resolution(h, &resolution);
    if ( ret != DIRP_SUCCESS ) {
        return py::cast<py::none>(Py_None);
    } else {
        return py::cast(resolution);
    }
}

py::object __dirp_get_original_raw(DIRP_HANDLE h)
{
    dirp_resolotion_t resolution;
    int32_t ret = dirp_get_rjpeg_resolution(h, &resolution);
    if ( ret != DIRP_SUCCESS ) {
        return py::cast<py::none>(Py_None);
    }
    size_t size = resolution.width * resolution.height;
    size_t buf_size = size * sizeof(uint16_t);

    std::vector<uint16_t> raw_image(size);
    ret = dirp_get_original_raw(h, raw_image.data(), buf_size);
    if ( ret != DIRP_SUCCESS ) {
        return py::cast<py::none>(Py_None);
    } else {
        return py::cast(raw_image);
    }
}

py::object __dirp_process(DIRP_HANDLE h)
{
    dirp_resolotion_t resolution;
    int32_t ret = dirp_get_rjpeg_resolution(h, &resolution);
    if ( ret != DIRP_SUCCESS ) {
        return py::cast<py::none>(Py_None);
    }
    size_t size = resolution.width * resolution.height * 3;
    size_t buf_size = size * sizeof(uint8_t);

    std::vector<uint8_t> color_image(size);
    ret = dirp_process(h, color_image.data(), buf_size);
    if ( ret != DIRP_SUCCESS ) {
        return py::cast<py::none>(Py_None);
    } else {
        return py::cast(color_image);
    }
}

py::object __dirp_process_stretch(DIRP_HANDLE h)
{
    dirp_resolotion_t resolution;
    int32_t ret = dirp_get_rjpeg_resolution(h, &resolution);
    if ( ret != DIRP_SUCCESS ) {
        return py::cast<py::none>(Py_None);
    }
    size_t size = resolution.width * resolution.height;
    size_t buf_size = size * sizeof(float);
    std::vector<float> strech_image(size);
    ret = dirp_process_strech(h, strech_image.data(), buf_size);
    if ( ret != DIRP_SUCCESS ) {
        return py::cast<py::none>(Py_None);
    } else {
        return py::cast(strech_image);
    }
}

py::object __dirp_measure(DIRP_HANDLE h)
{
    dirp_resolotion_t resolution;
    int32_t ret = dirp_get_rjpeg_resolution(h, &resolution);
    if ( ret != DIRP_SUCCESS ) {
        return py::cast<py::none>(Py_None);
    }
    size_t size = resolution.width * resolution.height;
    size_t buf_size = size * sizeof(int16_t);
    std::vector<int16_t> temp_image(size);
    ret = dirp_measure(h, temp_image.data(), buf_size);
    if ( ret != DIRP_SUCCESS ) {
        return py::cast<py::none>(Py_None);
    } else {
        return py::cast(temp_image);
    }
}

py::object __dirp_measure_ex(DIRP_HANDLE h)
{
    dirp_resolotion_t resolution;
    int32_t ret = dirp_get_rjpeg_resolution(h, &resolution);
    if ( ret != DIRP_SUCCESS ) {
        return py::cast<py::none>(Py_None);
    }
    size_t size = resolution.width * resolution.height;
    size_t buf_size = size * sizeof(float);
    std::vector<float> temp_image(size);
    ret = dirp_measure_ex(h, temp_image.data(), buf_size);
    if ( ret != DIRP_SUCCESS ) {
        return py::cast<py::none>(Py_None);
    } else {
        return py::cast(temp_image);
    }
}

int32_t __dirp_set_isotherm(DIRP_HANDLE h, const dirp_isotherm_t *isotherm)
{
    return dirp_set_isotherm(h, isotherm);
}

py::object __dirp_get_isotherm(DIRP_HANDLE h)
{
     dirp_isotherm_t isotherm;
    int32_t ret = dirp_get_isotherm(h, &isotherm);
    if ( ret != DIRP_SUCCESS ) {
        return py::cast<py::none>(Py_None);
    } else {
        return py::cast(isotherm);
    }
}

int32_t __dirp_set_color_bar(DIRP_HANDLE h, const dirp_color_bar_t *color_bar)
{
    return dirp_set_color_bar(h, color_bar);
}

py::object __dirp_get_color_bar(DIRP_HANDLE h)
{
    dirp_color_bar_t color_bar;
    int32_t ret = dirp_get_color_bar(h, &color_bar);
    if ( ret != DIRP_SUCCESS ) {
        return py::cast<py::none>(Py_None);
    } else {
        return py::cast(color_bar);
    }
}

py::object __dirp_get_color_bar_adaptive_params(DIRP_HANDLE h) 
{
    dirp_color_bar_t color_bar;
    int32_t ret = dirp_get_color_bar_adaptive_params(h, &color_bar);
    if ( ret != DIRP_SUCCESS ) {
        return py::cast<py::none>(Py_None);
    } else {
        return py::cast(color_bar);
    }
}

int32_t __dirp_set_pseudo_color(DIRP_HANDLE h, const dirp_pseudo_color_e* pseudo_color)
{
    return dirp_set_pseudo_color(h, *pseudo_color);
}

py::object __dirp_get_pseudo_color(DIRP_HANDLE h)
{
    dirp_pseudo_color_e pseudo_color;
    int32_t ret = dirp_get_pseudo_color(h, &pseudo_color);
    if ( ret != DIRP_SUCCESS ) {
        return py::cast<py::none>(Py_None);
    } else {
        return py::cast(pseudo_color);
    }
}

py::object  __dirp_get_pseudo_color_lut()
{
    dirp_isp_pseudo_color_lut_t pseudo_lut;
    int32_t ret = dirp_get_pseudo_color_lut(&pseudo_lut);
     if ( ret != DIRP_SUCCESS ) {
        return py::cast<py::none>(Py_None);
    } else {
        __dirp_isp_pseudo_color_lut_t* __pseudo_lut = new __dirp_isp_pseudo_color_lut_t();
        for (int i = 0; i < DIRP_PSEUDO_COLOR_NUM; i++) {
            std::vector<uint8_t> red, green, blue;
            for (int j = 0; j < DIRP_PSEUDO_COLOR_LUT_DEPTH; j++) {
                red.push_back(pseudo_lut.red[i][j]);
                green.push_back(pseudo_lut.green[i][j]);
                blue.push_back(pseudo_lut.blue[i][j]);
            }
            __pseudo_lut->red.emplace_back(red);
            __pseudo_lut->green.emplace_back(green);
            __pseudo_lut->blue.emplace_back(blue);
        }
        return py::cast(__pseudo_lut);
    }
}

int32_t __dirp_set_enhancement_params(DIRP_HANDLE h, const dirp_enhancement_params_t *enhancement_params)
{
    return dirp_set_enhancement_params(h, enhancement_params);
}

py::object __dirp_get_enhancement_params(DIRP_HANDLE h)
{
    dirp_enhancement_params_t enhancement_params;
    int32_t ret = dirp_get_enhancement_params(h, &enhancement_params);
    if ( ret != DIRP_SUCCESS ) {
        return py::cast<py::none>(Py_None);
    } else {
        return py::cast(enhancement_params);
    }
}

int32_t __dirp_set_measurement_params(DIRP_HANDLE h, const dirp_measurement_params_t *measurement_params)
{
    return dirp_set_measurement_params(h, measurement_params);
}

py::object __dirp_get_measurement_params(DIRP_HANDLE h)
{
    dirp_measurement_params_t measurement_params;
    int32_t ret = dirp_get_measurement_params(h, &measurement_params);
    if ( ret != DIRP_SUCCESS ) {
        return py::cast<py::none>(Py_None);
    } else {
        return py::cast(measurement_params);
    }
}

int32_t __dirp_register_app(const std::string& key) {
    return dirp_register_app(key.c_str());
}

void __dirp_set_verbose_level(dirp_verbose_level_e level) {
    dirp_set_verbose_level(level);
}

void __dirp_set_logger_file(const std::string& file_name) {
    dirp_set_logger_file(file_name.c_str());
}

PYBIND11_MODULE(rjpeg, m) {
    m.doc() = "Thermal SDK for Python";

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif

    py::enum_<dirp_ret_code_e>(m, "dirp_ret_code_e")
    .value("DIRP_SUCCESS", DIRP_SUCCESS)
    .value("DIRP_ERROR_MALLOC", DIRP_ERROR_MALLOC)
    .value("DIRP_ERROR_POINTER_NULL", DIRP_ERROR_POINTER_NULL)
    .value("DIRP_ERROR_INVALID_PARAMS", DIRP_ERROR_INVALID_PARAMS)
    .value("DIRP_ERROR_INVALID_RAW", DIRP_ERROR_INVALID_RAW)
    .value("DIRP_ERROR_INVALID_HEADER", DIRP_ERROR_INVALID_HEADER)
    .value("DIRP_ERROR_INVALID_CURVE", DIRP_ERROR_INVALID_CURVE)
    .value("DIRP_ERROR_RJPEG_PARSE", DIRP_ERROR_RJPEG_PARSE)
    .value("DIRP_ERROR_SIZE", DIRP_ERROR_SIZE)
    .value("DIRP_ERROR_INVALID_HANDLE", DIRP_ERROR_INVALID_HANDLE)
    .value("DIRP_ERROR_FORMAT_INPUT", DIRP_ERROR_FORMAT_INPUT)
    .value("DIRP_ERROR_FORMAT_OUTPUT", DIRP_ERROR_FORMAT_OUTPUT)
    .value("DIRP_ERROR_UNSUPPORTED_FUNC", DIRP_ERROR_UNSUPPORTED_FUNC)
    .value("DIRP_ERROR_NOT_READY", DIRP_ERROR_NOT_READY)
    .value("DIRP_ERROR_ACTIVATION", DIRP_ERROR_ACTIVATION)
    .value("DIRP_ERROR_ADVANCED", DIRP_ERROR_ADVANCED)
    .export_values();

    py::enum_<dirp_pseudo_color_e>(m, "dirp_pseudo_color_e")
    .value("DIRP_PSEUDO_COLOR_WHITEHOT", DIRP_PSEUDO_COLOR_WHITEHOT)
    .value("DIRP_PSEUDO_COLOR_FULGURITE", DIRP_PSEUDO_COLOR_FULGURITE)
    .value("DIRP_PSEUDO_COLOR_IRONRED", DIRP_PSEUDO_COLOR_IRONRED)
    .value("DIRP_PSEUDO_COLOR_HOTIRON", DIRP_PSEUDO_COLOR_HOTIRON)
    .value("DIRP_PSEUDO_COLOR_MEDICAL", DIRP_PSEUDO_COLOR_MEDICAL)
    .value("DIRP_PSEUDO_COLOR_ARCTIC", DIRP_PSEUDO_COLOR_ARCTIC)
    .value("DIRP_PSEUDO_COLOR_RAINBOW1", DIRP_PSEUDO_COLOR_RAINBOW1)
    .value("DIRP_PSEUDO_COLOR_RAINBOW2", DIRP_PSEUDO_COLOR_RAINBOW2)
    .value("DIRP_PSEUDO_COLOR_TINT", DIRP_PSEUDO_COLOR_TINT)
    .value("DIRP_PSEUDO_COLOR_BLACKHOT", DIRP_PSEUDO_COLOR_BLACKHOT)
    .value("DIRP_PSEUDO_COLOR_NUM", DIRP_PSEUDO_COLOR_NUM)
    .export_values();

    py::enum_<dirp_verbose_level_e>(m, "dirp_verbose_level_e")
    .value("DIRP_VERBOSE_LEVEL_NONE", DIRP_VERBOSE_LEVEL_NONE)
    .value("DIRP_VERBOSE_LEVEL_DEBUG", DIRP_VERBOSE_LEVEL_DEBUG)
    .value("DIRP_VERBOSE_LEVEL_DETAIL", DIRP_VERBOSE_LEVEL_DETAIL)
    .value("DIRP_VERBOSE_LEVEL_NUM", DIRP_VERBOSE_LEVEL_NUM)
    .export_values();

    py::class_<__dirp_api_version_t>(m, "dirp_api_version_t")
    .def_readwrite("api", &__dirp_api_version_t::api)
    .def_readwrite("magic", &__dirp_api_version_t::magic);

    py::class_<dirp_rjpeg_version_t>(m, "dirp_rjpeg_version_t")
    .def_readwrite("rjpeg", &dirp_rjpeg_version_t::rjpeg)
    .def_readwrite("header", &dirp_rjpeg_version_t::header)
    .def_readwrite("curve", &dirp_rjpeg_version_t::curve);

    py::class_<dirp_resolotion_t>(m, "dirp_resolotion_t")
    .def_readwrite("width", &dirp_resolotion_t::width)
    .def_readwrite("height", &dirp_resolotion_t::height);

    py::class_<dirp_isotherm_t>(m, "dirp_isotherm_t")
    .def_readwrite("enable", &dirp_isotherm_t::enable)
    .def_readwrite("high", &dirp_isotherm_t::high)
    .def_readwrite("low", &dirp_isotherm_t::low);

    py::class_<dirp_color_bar_t>(m, "dirp_color_bar_t")
    .def_readwrite("manual_enable", &dirp_color_bar_t::manual_enable)
    .def_readwrite("high", &dirp_color_bar_t::high)
    .def_readwrite("low", &dirp_color_bar_t::low);

    py::class_<__dirp_isp_pseudo_color_lut_t>(m, "dirp_isp_pseudo_color_lut_t")
    .def_readwrite("red", &__dirp_isp_pseudo_color_lut_t::red)
    .def_readwrite("green", &__dirp_isp_pseudo_color_lut_t::green)
    .def_readwrite("blue", &__dirp_isp_pseudo_color_lut_t::blue);

    py::class_<dirp_enhancement_params_t>(m, "dirp_enhancement_params_t")
    .def_readwrite("brightness", &dirp_enhancement_params_t::brightness);

    py::class_<dirp_measurement_params_t>(m, "dirp_measurement_params_t")
    .def_readwrite("distance", &dirp_measurement_params_t::distance)
    .def_readwrite("humidity", &dirp_measurement_params_t::humidity)
    .def_readwrite("emissivity", &dirp_measurement_params_t::emissivity)
    .def_readwrite("reflection", &dirp_measurement_params_t::reflection);

    m.def("dirp_create_from_rjpeg", &__dirp_create_from_rjpeg, "Create a new DIRP handle with specified R-JPEG binary data");
    m.def("dirp_destroy", &__dirp_destroy, "Destroy the DIRP handle");
    m.def("dirp_get_api_version", &__dirp_get_api_version, "Get API version");
    m.def("dirp_get_rjpeg_version", &__dirp_get_rjpeg_version, "Get R-JPEG version");
    m.def("dirp_get_rjpeg_resolution", &__dirp_get_rjpeg_resolution, "Get R-JPEG image resolution information");
    m.def("dirp_get_original_raw", &__dirp_get_original_raw, "Get original RAW binary data from R-JPEG");
    m.def("dirp_process", &__dirp_process, "Run ISP algorithm with RAW data in R-JPEG and output RGB pseudo color image");
    m.def("dirp_process_stretch", &__dirp_process_stretch, "Run ISP strech algorithm with RAW data in R-JPEG and output FLOAT32 streching image");
    m.def("dirp_measure", &__dirp_measure, "Measure temperature of whole thermal image with RAW data in R-JPEG");
    m.def("dirp_measure_ex", &__dirp_measure_ex, "Measure temperature of whole thermal image with RAW data in R-JPEG");
    m.def("dirp_set_isotherm", &__dirp_set_isotherm, "Set custom ISP isotherm parameters");
    m.def("dirp_get_isotherm", &__dirp_get_isotherm, "Get orignial/custom ISP isotherm parameters");
    m.def("dirp_set_color_bar", &__dirp_set_color_bar, "Set custom ISP color bar parameters");
    m.def("dirp_get_color_bar", &__dirp_get_color_bar, "Get orignial/custom ISP color bar parameters");
    m.def("dirp_get_color_bar_adaptive_params", &__dirp_get_color_bar_adaptive_params, "Get adaptive ISP color bar parameters in automatic mode");
    m.def("dirp_set_pseudo_color", &__dirp_set_pseudo_color, "Set custom ISP pseudo color type");
    m.def("dirp_get_pseudo_color", &__dirp_get_pseudo_color, "Get orignial/custom ISP pseudo color type");
    m.def("dirp_get_pseudo_color_lut", &__dirp_get_pseudo_color_lut, "Get ISP pseudo color LUT");
    m.def("dirp_set_enhancement_params", &__dirp_set_enhancement_params, "Set custom ISP enhancement parameters");
    m.def("dirp_get_enhancement_params", &__dirp_get_enhancement_params, "Get orignial/custom ISP enhancement parameters");
    m.def("dirp_set_measurement_params", &__dirp_set_measurement_params, "Set custom temperature measurement parameters");
    m.def("dirp_get_measurement_params", &__dirp_get_measurement_params, "Get orignial/custom temperature measurement parameters");
    m.def("dirp_register_app", &__dirp_register_app, "Register SDK for the application");
    m.def("dirp_set_verbose_level", &__dirp_set_verbose_level, "Set log print level");
    m.def("dirp_set_logger_file", &__dirp_set_logger_file, "Set external logger file");
}
