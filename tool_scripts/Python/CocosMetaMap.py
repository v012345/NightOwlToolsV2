import argparse
import sys
import json
import os



if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--txt", type=str, required=True, help="Path to txt")
    args = parser.parse_args()
    txt_file = args.txt
    if not os.path.exists(txt_file):
        txt_file = os.path.join(os.getcwd(), txt_file)
        if not os.path.exists(txt_file):
            print("txt 文件不存在")
            sys.exit(1)
    uuid_file_map = {}
    file_uuid_map = {}
    with open(txt_file, "r") as f:
        lines = f.read().splitlines()
        i = 0
        totol = 0
        for line in lines:
            if line.endswith(".meta"):
                totol += 1
        for line in lines:
            if line.endswith(".meta"):
                with open(line, 'r') as j:
                    json_data = json.load(j)
                    uuid = json_data.get("uuid")
                    if not uuid:
                        print(uuid, line)
                    i += 1
                    original_file = line.replace(".meta", "")
                    uuid_file_map[uuid] = original_file
                    file_uuid_map[original_file] = uuid
                    print(f"获取 uuid: {i} / {totol}", end="\r")
    with open("temp/uuid_file_map.json", "w", encoding="utf-8") as fp:
        json.dump(uuid_file_map, fp, ensure_ascii=False, indent=4)
    with open("temp/file_uuid_map.json", "w", encoding="utf-8") as fp:
        json.dump(file_uuid_map, fp, ensure_ascii=False, indent=4)
    print()
    # print(json_data.get("uuid", "no uuid"), line)

