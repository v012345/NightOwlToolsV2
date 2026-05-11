import argparse
import sys
import json
import os
parser = argparse.ArgumentParser()
parser.add_argument("--txt", type=str, required=True, help="Path to txt")
parser.add_argument("--output", type=str, required=True, help="Path to output json")
parser.add_argument("--uuid_file_map", type=str, required=True, help="Path to uuid file map json")
args = parser.parse_args()
txt_file = args.txt
output_file = args.output
uuid_file_map_file = args.uuid_file_map
with open(uuid_file_map_file, "r") as f:
    uuid_file_map = json.load(f)

def traverse_json(data, uuid_set):
    if isinstance(data, dict):
        for key, value in data.items():
            if key == "__uuid__":
                if "@" in value:
                    value = value.split("@")[0]
                if value in uuid_file_map:
                    file = uuid_file_map[value]
                    if file.endswith(".prefab"):
                        with open(file, 'r', encoding="utf-8") as j:
                            json_data = json.load(j)
                            traverse_json(json_data, uuid_set)
                        uuid_set.add(file)
            traverse_json(value, uuid_set)
    elif isinstance(data, list):
        for item in data:
            traverse_json(item, uuid_set)
            
if __name__ == "__main__":
    
    
    uuid_set = set()
    with open(txt_file, "r", encoding="utf-8") as f:
        lines = f.read().splitlines()
        i = 0
        for line in lines:
            with open(line, 'r', encoding="utf-8") as j:
                json_data = json.load(j)
                ## 这里要递归遍历 json_data , 输出 key 为 __uuid__ 的值
                print(f"正在提取 prefab 中的 __uuid__ :{i}/{len(lines)}", end="\r")
                i += 1
                uuid_set.add(line)
                traverse_json(json_data, uuid_set)
    with open(output_file, "w", encoding="utf-8") as fp:
        json.dump(list(uuid_set), fp, ensure_ascii=False, indent=4)
    print()

