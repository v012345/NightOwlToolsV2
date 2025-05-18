import subprocess
import shutil
from optparse import OptionParser
import os
import importlib
import sys
import xlwt
import hashlib
import time
import json

parser = OptionParser()
parser.add_option("--client", action="store",
                  dest="client", type="string")
parser.add_option("--config", action="store",
                  dest="config", type="string")


def get_file_md5(filepath):
    """Calculates the MD5 hash of a file.

    Args:
        filepath (str): The path to the file.

    Returns:
        str: The hexadecimal representation of the MD5 hash.
    """
    md5_hash = hashlib.md5()
    try:
        with open(filepath, "rb") as file:
            # Read the file in chunks to handle large files efficiently
            for chunk in iter(lambda: file.read(4096), b""):
                md5_hash.update(chunk)
    except FileNotFoundError:
        return "File not found"
    return md5_hash.hexdigest()


def find_files(root_dir, extensions):
    result = []
    for dirpath, _, filenames in os.walk(root_dir):
        for filename in filenames:
            if any(filename.endswith(ext) for ext in extensions):
                result.append(os.path.join(dirpath, filename))
    return result


def gen_uri(key, timestamp):
    dir_name = os.path.dirname(key)
    base_name = os.path.basename(key)
    name, ext = os.path.splitext(base_name)
    return os.path.join(dir_name, f"{name}_{timestamp}{ext}")


if shutil.which("coscmd") is None:
    subprocess.check_call(
        [sys.executable, "-m", "pip", "install", "coscmd"])

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
    # extensions = [".mp3", ".ttf", ]
    files = find_files(dev, extensions)

    to_upload = []
    local_all_file = []
    # 打印结果
    total = len(files)
    for i, file in enumerate(files):
        print(f"检查文件中 : {i+1}/{total}", end="\r")
        key = os.path.relpath(file, opts.client)
        local_all_file.append(key)
        info = db.get(key)
        timestamp = int(os.path.getmtime(file))
        if info:
            if timestamp != info.get("timestamp"):
                info["timestamp"] = timestamp
                md5 = get_file_md5(file)
                if info.get("md5") != md5:
                    info["md5"] = md5
                    info["uri"] = gen_uri(info.get("key"), timestamp)
                    to_upload.append(info)
        else:
            info = {
                "key": key,
                "timestamp": timestamp,
                "md5": get_file_md5(file),
                "uri": gen_uri(key, timestamp)
            }
            to_upload.append(info)
    print()
    print(f"要上传 {len(to_upload)} 个文件")
    for u in to_upload:
        subprocess.Popen(["coscmd", "-c", opts.config, "upload", opts.client + "/"+u.get("key"),
                          u.get("uri")]).wait()

    for info in to_upload:
        db[info.get("key")] = info

    new_db = {}
    for file in local_all_file:
        new_db[file] = db.get(file)
    with open(digital_res, "w", encoding="utf-8") as f:
        json.dump(new_db, f, ensure_ascii=False, indent=4)
