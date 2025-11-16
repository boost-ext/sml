# Support for using the boost::sml as a [Zephyr](https://docs.zephyrproject.org/latest/index.html) module

To use add the reposiory path to your west maifest file, probably `west.yml`. See the zephyr documentation on [project manifests](https://docs.zephyrproject.org/latest/develop/west/manifest.html#projects) for details.


``` yaml
  projects:
    - name: boost-sml
      url: http://your.git.server/sml
      revision: tag|branch|sha
```

Then run `west update` to pull in the code. You need to then add the following config options to `prj.conf` or whereever you manage your configuration.

```
CONFIG_CPP=y
CONFIG_STD_CPP17=y
CONFIG_REQUIRES_FULL_LIBCPP=y

# Include boost sml state machine
CONFIG_LIB_BOOST_SML=y
```

Then you can `#include <boost/sml.hpp>` to use the library.
