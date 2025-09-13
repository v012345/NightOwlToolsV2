import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--csv", type=str, required=True, help="Path to csv")
parser.add_argument("--gd", type=str, required=True, help="Path to gd")
args = parser.parse_args()

import csv
with open(args.csv, 'r', encoding='utf_8_sig') as file:
    csv_reader = csv.reader(file)
    rows = list(csv_reader)   # 转为二维数组（list of lists）
    header = rows[1]
    data_type = rows[2]
    data = rows[3:]

id_index  = 0 # 把 id 的 index 找到
for i,v in enumerate(header):
    if v == "id":
        id_index = i
        break


with open(args.gd, 'w', encoding='utf-8') as f_out:
    f_out.write("# 使用 NightOwlToolsV2/tool_scripts/Python/csvToGodotDB.py 生成\n")
    f_out.write("const DATA = {\n")
    for row in data:
        f_out.write(f"{row[id_index]}:{{")
        for i,cell in enumerate(row):
            f_out.write(f'"{header[i]}" :')
            if data_type[i] == "int":
                f_out.write(f'{cell}')
            elif data_type[i] == "string":
                f_out.write(f'"{cell}"')
            else:
                print("不支持")
            f_out.write(",")
        f_out.write("},\n")
    f_out.write("}\n")
print("生成" , args.gd)