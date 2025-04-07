""" 导出热更资源

- 流程

            
"""

import os
import optparse

EXPORT_RESOURCES_COMMOND = "py -3 publish_res.py -p %s -v %s -b %s -l %s %s"




def parseargs():
    parser = optparse.OptionParser()
    parser.add_option(
        "--version",
        action  = "store",
        dest    = "version",
        default = "",
        help    = "Set version"
    )
    parser.add_option(
        "--platform",
        action  = "store",
        dest    = "platform",
        default = "android",
        help    = "Set platform android/ios/none"
    )
    parser.add_option(
        "--pack_path",
        action  = "store",
        dest    = "pack_path",
        default = "",
        help    = "Set pack path"
    )
    parser.add_option(
        "--pack_all",
        action  = "store",
        dest    = "pack_all",
        default = False,
        help    = "is pack all file or changed file"
    )
    parser.add_option(
        "--branch",
        action="store",
        dest="branch",
        help="pack which branch's res"
    )
    parser.add_option(
        "--language",
        action="store",
        dest="language",
        default="res",
        help="pack which language resource"
    )
    return parser.parse_args()


if __name__ == "__main__":
    (opts, args) = parseargs()
    
    commond = EXPORT_RESOURCES_COMMOND % (opts.platform, opts.version, opts.branch, opts.language, opts.pack_all == "yes" and "--pack_all" or "")
    os.chdir(opts.pack_path)
    os.system(commond)