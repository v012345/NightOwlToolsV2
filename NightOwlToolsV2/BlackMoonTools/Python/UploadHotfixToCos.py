import sys
import shutil
import subprocess
import optparse


def parseargs():
    parser = optparse.OptionParser()
    parser.add_option(
        "--compressed_res",
        action="store",
        dest="compressed_res",
        help="cos上传文件夹, 不包括自己"
    )
    parser.add_option(
        "--config",
        action="store",
        dest="config",
        help="cos配置文件"
    )
    parser.add_option(
        "--cnd_folder",
        action="store",
        dest="to",
        help="cos存储桶目标目录"
    )
    return parser.parse_args()


if __name__ == "__main__":
    if shutil.which("coscmd") is None:
        subprocess.check_call([sys.executable, "-m", "pip", "install", "coscmd"])
    (opts, _) = parseargs()
    subprocess.Popen(["coscmd", "-c", opts.config, "upload","-r",opts.compressed_res, opts.to],creationflags=subprocess.CREATE_NEW_CONSOLE).wait()
