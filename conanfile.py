import os

from conans import ConanFile, tools

class SML(ConanFile):
    name = "SML"
    version = "latest"
    url = "https://github.com/boost-ext/sml"
    license = "Boost"
    description = "[Boost::ext].SML: C++14 State Machine Library"
    settings = "os", "compiler", "arch", "build_type"
    exports_sources = "include/*"
    no_copy_source = True

    def package(self):
        self.copy("*.hpp")
