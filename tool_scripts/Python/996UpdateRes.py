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


def download_file(file_info, i, t):
    print(f"下载 {i}/{t}", end="\r")
    subprocess.run([
        "coscmd", "-c", opts.config, "-s", "download",
        file_info.get("uri"), opts.client + "/" + file_info.get("key")
    ])


if __name__ == '__main__':
    (opts, args) = parser.parse_args()
    digital_res = opts.client + "/digital_res.json"
    if not os.path.exists(digital_res):
        print("miss digital_res.json")
        exit(0)
    else:
        with open(digital_res, 'r', encoding='utf-8') as f:
            db = json.load(f)

    to_download = []
    total = len(db)
    i = 1
    for file, info in db.items():
        print(f"正在检查文件 : {i}/{total}", end="\r")
        f = opts.client + "/" + file
        if not os.path.exists(f) or get_file_md5(f) != info["md5"]:
            to_download.append(info)
        i += 1
    print()
    with concurrent.futures.ThreadPoolExecutor(max_workers=8) as executor:
        # 提交所有任务到线程池
        futures = [executor.submit(download_file, u, i, len(
            to_download)) for i, u in enumerate(to_download)]
        # 等待所有任务完成
        concurrent.futures.wait(futures)
    print(f"下载 {len(to_download)}/{len(to_download)}", end="\r")
    print()
