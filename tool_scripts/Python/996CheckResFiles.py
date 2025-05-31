import re
import subprocess
import optparse
import json


def parseargs():
    parser = optparse.OptionParser()
    parser.add_option(
        "--cos_files",
        action="store",
        dest="cos_files"
    )
    parser.add_option(
        "--local_files",
        action="store",
        dest="local_files",
    )
    parser.add_option(
        "--output_to",
        action="store",
        dest="output_to",
    )
    return parser.parse_args()


if __name__ == "__main__":
    (opts, _) = parseargs()
    with open(opts.cos_files, 'r', encoding='utf-8') as f:
        cos_files = json.load(f)
    with open(opts.local_files, 'r', encoding='utf-8') as f:
        local_files = json.load(f)
    i = 0
    total = len(local_files)
    miss = []
    for file, info in local_files.items():
        i += 1
        print(f"正在检查本地文件是否已经上传: {i}/{total}", end="\r")
        if not cos_files.get(info['uri']):
            miss.append(file)
    print()
    with open(opts.output_to, 'w', encoding='utf-8') as f:
        for m in miss:
            f.write(m + "\n")
    print(f"有 {len(miss)} 个文件没有上传, 已经输出到 {opts.output_to}")
