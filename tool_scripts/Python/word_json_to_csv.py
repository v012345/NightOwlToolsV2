import argparse
import csv
import json

parser = argparse.ArgumentParser()
parser.add_argument("--json_path", type=str, required=True, help="Path to json")
parser.add_argument("--japanese_path", type=str, required=True, help="Path to japanese")
parser.add_argument("--example_path", type=str, required=True, help="Path to example")
parser.add_argument("--book_id", type=str, required=True, help="Book id")
args = parser.parse_args()

def get_table_meta_info(data):
    key = id(data)
    if not hasattr(get_table_meta_info,"m"):
        get_table_meta_info.m = {}
    if key not in get_table_meta_info.m:
        id_set = set()
        id_to_row_map = {}
        col_name_index = {}
        for index, name in enumerate(data[1]): # 约定为列名
            col_name_index[name] = index
        for index, row in enumerate(data):
            idx = row[0]
            id_set.add(idx) # 约定 id 就在第一列
            id_to_row_map[idx] = index
        get_table_meta_info.m[key] = {
            "new_id" : 1,
            "id_set" : id_set,
            "id_to_row_map" : id_to_row_map,
            "col_name_index" : col_name_index
        }
    return get_table_meta_info.m[key]

# 空 [] 居然是相同的 id
def get_an_available_id(data):
    data_info = get_table_meta_info(data)
    id_set = data_info["id_set"]
    new_id = data_info["new_id"]
    while True:
        if str(new_id) in id_set:
            new_id += 1
        else:
            data_info["new_id"] = new_id
            break
    return new_id

def table_has_id(data,idx):
    data_info = get_table_meta_info(data)
    id_set = data_info["id_set"]
    return str(idx) in id_set

def get_row_data_by_id(data,idx):
    if table_has_id(data,idx):
        data_info = get_table_meta_info(data)
        return data[data_info["id_to_row_map"][str(idx)]]
    else: raise Exception("id not exist")

def insert_a_row_or_update(data,new_row_map):
    if "id" in new_row_map:
        if table_has_id(data,new_row_map["id"]):
            update_by_id(data,new_row_map["id"],new_row_map)
            return
        new_id = new_row_map["id"]
    else:
        new_id = get_an_available_id(data)
    data_info = get_table_meta_info(data)
    col_name_index = data_info["col_name_index"]
    new_row = []
    for _ in col_name_index:
        new_row.append("")
    for key, value in new_row_map.items():
        if key != "id":
            new_row[col_name_index[key]] = str(value)
    new_row[0] = str(new_id)
    data_info["id_set"].add(new_row[0])
    data_info["id_to_row_map"][new_row[0]] = len(data)
    data.append(new_row)

def update_by_id(data,idx,new_row_map):
    row = get_row_data_by_id(data,idx)
    data_info = get_table_meta_info(data)
    col_name_index = data_info["col_name_index"]
    for key, value in new_row_map.items():
        if key != "id":
            row[col_name_index[key]] = str(value)


def get_id_if_column_has_value(data,col_name,value):
    data_info = get_table_meta_info(data)
    if col_name not in data_info["col_name_index"]:
        raise Exception("col_name not exist")
    index = data_info["col_name_index"][col_name]
    for row in data: 
        if row[index] == value:
            return int(row[0])
    return False

with open(args.example_path, 'r', encoding='utf_8_sig') as file:
    csv_reader = csv.reader(file)
    example_data = list(csv_reader)   # 转为二维数组（list of lists）


with open(args.japanese_path, 'r', encoding='utf_8_sig') as file:
    csv_reader = csv.reader(file)
    japanese_data = list(csv_reader)   # 转为二维数组（list of lists）


with open(args.json_path, 'r', encoding='utf-8') as f:
    json_data = json.load(f)

for row in json_data:
    example_ids = []
    for exam in row["example"]:
        example_id = get_id_if_column_has_value(example_data,"japanese",exam[0])
        if not example_id:
            example_id = get_an_available_id(example_data)
            insert_a_row_or_update(example_data,{
                "id":example_id,
                "japanese":exam[0],
                "chinese":exam[1],
                "sound_start":0,
                "sound_end":0,
            })
        example_ids.append(example_id)
    insert_a_row_or_update(japanese_data,{
        "id":row["id"],
        "kana": row["kana"],
        "kanji": row["kanji"],
        "chinese": row["chinese"],
        "example_id": example_ids,
        "sound_start": 0,
        "sound_end": 0,
        "sound_end": 0,
        "book_id" : args.book_id
    })

with open(args.japanese_path, 'w', encoding='utf_8_sig', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(japanese_data)
with open(args.example_path, 'w', encoding='utf_8_sig', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(example_data)