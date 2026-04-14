import os
import sys

exclude_dirs = {".git", ".svn", ".vscode"}

from_dir = sys.argv[1]
output_to = sys.argv[2]

results = []
for root, dirs, files in os.walk(from_dir):
    dirs[:] = [d for d in dirs if d not in exclude_dirs]
    for f in files:
        full = os.path.join(root, f)
        rel = os.path.relpath(full, from_dir).replace("\\", "/")
        results.append("/" + rel)

print(f"获取 {from_dir} 下的文件: {len(results)}")

with open(output_to, "w", encoding="utf-8") as fp:
    fp.write("\n".join(results))
