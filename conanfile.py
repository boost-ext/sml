import os

from conans import ConanFile, tools


class SmlConan(ConanFile):
    name = "SML"
    version = "latest"
    license = "Boost"
    url = "https://github.com/paulbendixen/sml.git"
    description = "[Boost].SML: C++14 State Machine Library"
    #no_copy_source = True
    # No settings/options are necessary, this is header only
    exports_sources = "include/*"

    def package(self):
        self.copy("*", src="include", dst="include")

