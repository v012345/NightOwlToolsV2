
import argparse
import csv
import json

parser = argparse.ArgumentParser()
parser.add_argument("--json_path", type=str, required=True, help="Path to json")
parser.add_argument("--japanese_path", type=str, required=True, help="Path to japanese")
parser.add_argument("--example_path", type=str, required=True, help="Path to example")
parser.add_argument("--book_id", type=str, required=True, help="Book id")
args = parser.parse_args()

example_path_map_japanese_to_row_index = {}
example_path_id_collection = {}
example_path_japanese = 1
example_path_id = 0
example_path_new_id = 1
with open(args.example_path, 'r', encoding='utf_8_sig') as file:
    csv_reader = csv.reader(file)
    example_path_data = list(csv_reader)   # 转为二维数组（list of lists）
    for index, row in enumerate(example_path_data):
        example_path_map_japanese_to_row_index[row[example_path_japanese]] = index
        example_path_id_collection[row[example_path_id]] = True
# print(example_path_map_japanese_to_row_index)
# print(example_path_id_collection)
japanese_path_map_id_to_row_index = {}
japanese_path_id_collection = {}
with open(args.japanese_path, 'r', encoding='utf_8_sig') as file:
    csv_reader = csv.reader(file)
    japanese_path_data = list(csv_reader)   # 转为二维数组（list of lists）
    for index, row in enumerate(japanese_path_data):
        japanese_path_map_id_to_row_index[row[0]] = index
        japanese_path_id_collection[row[0]] = True


def get_example_id(exam):
    global example_path_new_id
    global example_path_data
    if exam[0] in example_path_map_japanese_to_row_index:
        return int(example_path_data[example_path_map_japanese_to_row_index[exam[0]]][0])
    else:

        while True:
            if str(example_path_new_id) not in example_path_id_collection:
                example_path_id_collection[str(example_path_new_id)] = True
                break
            example_path_new_id = example_path_new_id + 1
        example_path_data.append([example_path_new_id,exam[0],exam[1],"",0,0])
        example_path_map_japanese_to_row_index[exam[0]] = len(example_path_data) - 1
    return example_path_new_id

# print(japanese_path_map_id_to_row_index)
with open(args.json_path, 'r', encoding='utf-8') as f:
        json_data = json.load(f)
        for row in json_data:
            if str(row["id"]) in japanese_path_map_id_to_row_index:
                japanese_data = japanese_path_data[japanese_path_map_id_to_row_index[str(row["id"])]]
                japanese_data[1] = row["kana"]
                japanese_data[2] = row["kanji"]
                japanese_data[3] = row["chinese"]
                example_ids = []
                for exam in row["example"]:
                    example_ids.append(get_example_id(exam))
                japanese_data[4] = str(example_ids)
            else:
                print("没有")
        # print(cos_files)

#     # for idx, x in enumerate(rows[1]):
#     #     if x == "kana":
#     #         kana_index = idx
#     #     if x == "image_path":
#     #         image_path_index = idx


with open(args.japanese_path, 'w', encoding='utf_8_sig', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(japanese_path_data)
with open(args.example_path, 'w', encoding='utf_8_sig', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(example_path_data)