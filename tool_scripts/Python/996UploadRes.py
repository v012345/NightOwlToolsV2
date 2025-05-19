import subprocess
import shutil
from optparse import OptionParser
import os
import sys
import hashlib
import json
import concurrent.futures
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


def update_json_file(db, digital_res):
    """将更新后的 db 写入 JSON 文件"""
    with open(digital_res, "w", encoding="utf-8") as f:
        json.dump(db, f, ensure_ascii=False, indent=4)


def upload_file(file_info):
    subprocess.run([
        "coscmd", "-c", opts.config, "upload",
        opts.client + "/" + file_info.get("key"), file_info.get("uri")
    ])


if __name__ == '__main__':

    (opts, args) = parser.parse_args()
    digital_res = opts.client + "/digital_res.json"
    if os.path.exists(digital_res):
        with open(digital_res, 'r', encoding='utf-8') as f:
            db = json.load(f)
    else:
        db = {}

    dev = opts.client + "\\dev"
    # extensions = [".png", ".plist", ".mp3", ".jpg", ".ttf", ".map"]
    extensions = [".lua", ]
    print("正在获取文件")
    files = find_files(dev, extensions)

    to_upload = []
    # 打印结果
    total = len(files)
    for i, file in enumerate(files):
        print(f"正在检查文件 : {i+1}/{total}", end="\r")
        key = os.path.relpath(file, opts.client)
        info = db.get(key)
        md5 = get_file_md5(file)
        if not (info and info.get("md5") == md5):
            info = {
                "key": key,
                "md5": md5,
                "uri": gen_uri(key, md5[0:5])
            }
            to_upload.append(info)

    print()
    print(f"要上传文件数 : {len(to_upload)}")
    f_number = 0
    s_number = 0
    for i, u in enumerate(to_upload):
        print(f"上传文件 : {i+1}/{len(to_upload)}", end="\r")
        r = subprocess.Popen(["coscmd", "-c", opts.config, "-s", "upload",
                              opts.client + "/" + u.get("key"), u.get("uri")]).wait()
        if r == 0:
            s_number += 1
        else:
            f_number += 1
        db[u["key"]] = u
        if i % 10 == 0:
            update_json_file(db, digital_res)
    update_json_file(db, digital_res)
    # 使用线程池并发上传
    # with concurrent.futures.ThreadPoolExecutor(max_workers=8) as executor:
    #     futures = [executor.submit(upload_file, u) for u in to_upload]
    #     concurrent.futures.wait(futures)
