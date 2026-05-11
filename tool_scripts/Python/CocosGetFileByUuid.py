import argparse
import sys
import json
import os



if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--uuid", type=str, required=True, help="Path to uuid json")
    parser.add_argument("--uuid_file_map", type=str, required=True, help="Path to uuid file map json")
    args = parser.parse_args()
    uuid_file_map_file = args.uuid_file_map
    uuids = args.uuid
   
    with open(uuid_file_map_file, "r") as f:
        uuid_file_map = json.load(f)
    with open(uuids, "r") as f:
        uuid_list = json.load(f)
    ## uuid 可能会发 @#### 为结尾, 这里要去掉 @#### 之后的部分
    files = set()
    for uuid in uuid_list:
        if "@" in uuid:
            uuid = uuid.split("@")[0]
        used_file = uuid_file_map.get(uuid)
        if used_file:
            files.add(used_file)
    with open("temp/used_files.json", "w", encoding="utf-8") as fp:
        json.dump(list(files), fp, ensure_ascii=False, indent=4)

