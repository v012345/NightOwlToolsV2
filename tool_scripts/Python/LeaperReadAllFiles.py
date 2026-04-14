import re
import os
from pathlib import Path

folder = Path(r"C:\Users\A\Desktop\work\leaper\leaper_design\art\领地界面\TerritoryPage")

pattern = re.compile(r'^[a-z][a-z0-9_]*$')

def normalize_name(name: str) -> str:
    # 去掉扩展名
    stem, ext = os.path.splitext(name)

    # 全部转小写
    stem = stem.lower()
    
    # 非法字符替换成随机字母
    stem = re.sub(r'[^a-z0-9_]', lambda x: chr(ord('a') + int(os.urandom(1)[0] % 26)), stem)

    # 如果不是字母开头，加个 a_
    if not re.match(r'^[a-z]', stem):
        stem = 'a_' + stem

    # 避免空名
    if not stem:
        stem = "a_file"

    return stem + ext


for file in folder.iterdir():
    if file.is_file():
        old_name = file.name
        stem = file.stem

        if not pattern.match(stem):
            new_name = normalize_name(old_name)
            new_path = file.with_name(new_name)

            # 避免重名覆盖
            i = 1
            while new_path.exists():
                new_path = file.with_name(f"{file.stem}_{i}{file.suffix}")
                i += 1

            print(f"重命名: {old_name} -> {new_path.name}")
            file.rename(new_path)