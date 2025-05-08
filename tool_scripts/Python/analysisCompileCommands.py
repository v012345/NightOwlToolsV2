# 分析 Compile_Commands.json, 因为有多个 abi, 把不同是的 abi 使用的命令收集起来

import json
import subprocess
import shlex
from pathlib import Path
from optparse import OptionParser
import os

parser = OptionParser()
parser.add_option("-t", "--to", action="store",
                  dest="to", type="string", help="输出到的目录")
parser.add_option("-r", "--root", action="store",
                  dest="root", type="string", help="根目录")
(opts, commands_files) = parser.parse_args()
root = opts.root

output_path = Path(opts.to)
all_includes = set()
for commands_file in commands_files:
    compile_commands_path = Path(commands_file)
    if not compile_commands_path.exists():
        continue
    with compile_commands_path.open("r", encoding="utf-8") as f:
        compile_db = json.load(f)
    print(f"正在处理 {commands_file} 中的指令")
    i = 1
    total = len(compile_db)
    for entry in compile_db:
        print(f"{i}/{total}", end="\r")
        file = entry["file"]
        all_includes.add(file)
        i += 1

        directory = entry["directory"]
        args = shlex.split(entry["command"], posix=False)

        # 替换 -c 为 -M 以提取依赖关系
        try:
            idx = args.index("-c")
            args[idx] = "-M"
        except ValueError:
            continue  # 如果没有 -c 参数，跳过

        # 去掉输出文件参数 -o xxx
        if "-o" in args:
            o_idx = args.index("-o")
            del args[o_idx:o_idx+2]

        try:
            result = subprocess.run(args, cwd=directory,
                                    capture_output=True, text=True)
            if result.returncode == 0:
                includes = result.stdout.strip().replace("\\\n", "").split()
                includes = [f for f in includes[1:]]
                all_includes.update(includes)
            else:
                print(f"Error processing {file}: {result.stderr}")
        except Exception as e:
            print(f"Exception: {e}")
    print()

root_abs_path = os.path.abspath(os.path.normpath(root))
index = len(root_abs_path)
normalized_paths = set()
for path in sorted(all_includes):
    abs_path = os.path.abspath(os.path.normpath(path))
    if abs_path.startswith(root_abs_path):
        normalized_paths.add(abs_path[index:])

# 输出所有包含的头文件到 txt
with output_path.open("w", encoding="utf-8") as f:
    for inc in sorted(normalized_paths):
        f.write(inc + "\n")