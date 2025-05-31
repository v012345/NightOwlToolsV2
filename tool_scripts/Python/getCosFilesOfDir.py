import re
import subprocess
import optparse
import json


def parseargs():
    parser = optparse.OptionParser()
    parser.add_option(
        "--output_to",
        action="store",
        dest="output_to"
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
        dest="folder",
        help="cos存储桶目标目录"
    )
    return parser.parse_args()


if __name__ == "__main__":
    (opts, _) = parseargs()
    print("正在获取腾讯云文件列表信息")
    result = subprocess.run(
        ["coscmd", "-c", opts.config, "list", opts.folder, "-ar"],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    all_files = {}
    data = re.split("\n", result.stdout)  # result.stderr
    total = len(data)
    with open(opts.output_to, "w", encoding="utf-8") as f:
        for i, file in enumerate(data):
            print(f"处理路径 : {i+1}/{total}", end="\r")
            match = re.split(r'\s+\d+\s+STANDARD\s+', file)
            if match[0]:
                file_path = (match[0]).strip().replace("/", "\\")
                all_files[file_path] = True
        json.dump(all_files, f, ensure_ascii=False, indent=4)
    print()
    print(f"结果已输出到 {opts.output_to}")
