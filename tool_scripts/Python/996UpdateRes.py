import subprocess
import shutil
from optparse import OptionParser
import os
import sys
import concurrent.futures
import hashlib
import json

parser = OptionParser()
parser.add_option("--client", action="store",
                  dest="client", type="string")
parser.add_option("--config", action="store",
                  dest="config", type="string")


def find_files(root_dir, extensions):
    result = []
    for dirpath, _, filenames in os.walk(root_dir):
        for filename in filenames:
            if any(filename.endswith(ext) for ext in extensions):
                result.append(os.path.join(dirpath, filename))
    return result


if __name__ == '__main__':

    (opts, args) = parser.parse_args()
    digital_res = opts.client + "/digital_res.json"
    if os.path.exists(digital_res):
        with open(digital_res, 'r', encoding='utf-8') as f:
            db = json.load(f)
    else:
        db = {}

    dev = opts.client + "\\dev"
    extensions = [".png", ".plist", ".mp3", ".jpg", ".ttf", ".map"]
    # extensions = [".lua", ]
    print("正在获取文件")
    files = find_files(dev, extensions)
    total = len(files)
    to_remve = []
    for file in db:
        if not os.path.exists(opts.client + "\\"+file):
            to_remve.append(file)
    for k in to_remve:
        db.pop(k)
    print(f"删除文件 : {len(to_remve)}")
    with open(digital_res, "w", encoding="utf-8") as f:
        json.dump(db, f, ensure_ascii=False, indent=4)
