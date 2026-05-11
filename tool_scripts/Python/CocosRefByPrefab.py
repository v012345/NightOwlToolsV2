import argparse
import sys
import json
import os

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--txt", type=str, required=True, help="Path to txt")
    parser.add_argument("--output", type=str, required=True, help="Path to output json")
    args = parser.parse_args()
    txt_file = args.txt
    output_file = args.output
    if not os.path.exists(txt_file):
        txt_file = os.path.join(os.getcwd(), txt_file)
        if not os.path.exists(txt_file):
            print("txt 文件不存在")
            sys.exit(1)
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
                def traverse_json(data):
                    if isinstance(data, dict):
                        for key, value in data.items():
                            if key == "__uuid__":
                                uuid_set.add(value)
                            traverse_json(value)
                    elif isinstance(data, list):
                        for item in data:
                            traverse_json(item)
                traverse_json(json_data)
    with open(output_file, "w", encoding="utf-8") as fp:
        json.dump(list(uuid_set), fp, ensure_ascii=False, indent=4)
    print()

