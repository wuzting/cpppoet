from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy
import os


class CpppoetConan(ConanFile):
    name = "cpppoet"
    version = "0.1.0"
    license = "MIT"
    author = "wuzting"
    url = "https://github.com/wuzting/cpppoet"
    homepage = "https://github.com/wuzting/cpppoet"
    description = "A JavaPoet-style C++17 code-generation library."
    topics = ("cpp", "code-generation", "codegen", "cmake")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    exports_sources = "CMakeLists.txt", "src/*", "include/*", "cmake/*"
    generators = "CMakeToolchain"

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["BUILD_SHARED_LIBS"] = self.options.shared
        tc.variables["ENABLE_CPPPOET_EXAMPLE"] = False
        tc.variables["ENABLE_CPPPOET_TEST"] = False
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
        copy(
            self,
            "LICENSE",
            src=self.source_folder,
            dst=os.path.join(self.package_folder, "licenses"),
        )

    def package_info(self):
        self.cpp_info.libs = ["cpppoet"]
