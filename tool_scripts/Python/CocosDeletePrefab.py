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
    all_ui_res_files = set()
    with open(txt_file, "r") as f:
        lines = f.read().splitlines()
        i = 0
        totol = 0
        for line in lines:
            if not line.endswith(".meta") and "/remoteBundle/ui_asset/UI" in line:
                all_ui_res_files.add(line.replace("\\", "/"))
                totol += 1
    with open('temp/all_prefab_uuid_set.json', "r") as f:
        used_files = set(json.load(f))
    used_files = set(file.replace("\\", "/") for file in used_files)
    delete_files = all_ui_res_files - used_files

    with open("temp/delete_prefab_files.json", "w", encoding="utf-8") as fp:
        json.dump(list(delete_files), fp, ensure_ascii=False, indent=4)
    # print(json_data.get("uuid", "no uuid"), line)

