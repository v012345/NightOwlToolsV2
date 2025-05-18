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


def download_file(file_info):
    subprocess.run([
        "coscmd", "-c", opts.config, "download",
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

    digital_res_local = opts.client + "/digital_res_local.json"
    if os.path.exists(digital_res_local):
        with open(digital_res_local, 'r', encoding='utf-8') as f:
            db_local = json.load(f)
    else:
        db_local = {}

    to_download = []
    for k, v in db.items():
        info = db_local.get(k)
        if not info:
            to_download.append(v)
        elif info.get("md5") != v.get("md5"):
            to_download.append(info)

    # for u in to_download:
    #     subprocess.Popen(["coscmd", "-c", opts.config, "download", u.get("uri"), opts.client + "/" + u.get("key")
    #                       ]).wait()

    with concurrent.futures.ThreadPoolExecutor(max_workers=8) as executor:
        # 提交所有任务到线程池
        futures = [executor.submit(download_file, u) for u in to_download]
        # 等待所有任务完成
        concurrent.futures.wait(futures)

    print(f"更新 {len(to_download)} 个文件")
    dev = opts.client + "\\dev"
    extensions = [".png", ".plist", ".mp3", ".jpg", ".ttf", ".map"]
    # extensions = [".jpg", ".map", ]
    local_all_file = []
    for i in find_files(dev, extensions):
        local_all_file.append(os.path.relpath(i, opts.client))
    new_db = {}
    for file in local_all_file:
        new_db[file] = 1
    to_del = new_db.keys() - db.keys()
    for d in to_del:
        os.remove(opts.client+"/"+d)
    print(f"删除 {len(to_del)} 个文件")

    with open(digital_res_local, "w", encoding="utf-8") as f:
        json.dump(db, f, ensure_ascii=False, indent=4)
