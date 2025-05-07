import json
import subprocess
import shlex
from pathlib import Path
from optparse import OptionParser
import os


parser = OptionParser()
parser.add_option("-c", "--commands", action="store",
                  dest="compile_commands", type="string", help="compile_commands.json 路径")
parser.add_option("-t", "--to", action="store",
                  dest="to", type="string", help="输出到的目录")
(opts, args) = parser.parse_args()

compile_commands_path = Path(opts.compile_commands)
output_path = Path(opts.to)

with compile_commands_path.open("r", encoding="utf-8") as f:
    compile_db = json.load(f)

all_includes = set()
total = len(compile_db)
i = 1
for entry in compile_db:
    print(f"{i}/{total}",end="\r")
    file = entry["file"]
    directory = entry["directory"]
    args = entry.get("arguments")
    i+=1

    if "arguments" in entry:
        args = entry["arguments"]
    elif "command" in entry:
        args = shlex.split(entry["command"], posix=False)
    else:
        continue

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
            # 去掉第一项（源文件名），只保留头文件
            # includes = [f for f in includes[1:] if f.endswith((".h", ".hpp"))]
            includes = [f for f in includes[1:]]
            all_includes.update(includes)
        else:
            print(f"Error processing {file}: {result.stderr}")
    except Exception as e:
        print(f"Exception: {e}")

normalized_paths = set()
for path in sorted(all_includes):
    abs_path = os.path.abspath(os.path.normpath(path))
    normalized_paths.add(abs_path)

# 输出所有包含的头文件到 txt
with output_path.open("w", encoding="utf-8") as f:
    for inc in sorted(normalized_paths):
        f.write(inc + "\n")

print()

