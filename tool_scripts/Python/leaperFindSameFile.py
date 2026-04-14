import os
import sys
import hashlib

exclude_dirs = {".git", ".svn", ".vscode"}

from_dir = sys.argv[1]

def calc_sha1(file_path):
    sha1 = hashlib.sha1()
    with open(file_path, "rb") as f:
        while True:
            data = f.read(8192)
            if not data:
                break
            sha1.update(data)
    return sha1.hexdigest()

sha1_map = {}

for root, dirs, files in os.walk(from_dir):
    dirs[:] = [d for d in dirs if d not in exclude_dirs]

    for f in files:
        full_path = os.path.join(root, f)

        try:
            sha1 = calc_sha1(full_path)
        except Exception as e:
            print(f"跳过文件: {full_path}, 错误: {e}")
            continue

        if sha1 in sha1_map:
            sha1_map[sha1].append(full_path)
        else:
            sha1_map[sha1] = [full_path]

# 输出重复文件（同一个 sha1 下有多个文件）
print("\n=== 重复文件 ===")
for sha1, file_list in sha1_map.items():
    if len(file_list) > 1:
        print(f"\nSHA1: {sha1}")
        for f in file_list:
            print(f"  {f}")