from optparse import OptionParser
from pathlib import Path
import os
import xlrd


def is_number(s):
    if "_" in s:  # 因为 1_000 会被 float(s) 转为 1000.0
        return False
    try:
        float(s)  # 尝试转为浮点数
        return True
    except ValueError:
        return False


def escapeForLua(s: str):
    return s.replace('\r', '').replace(
        "\\", "\\\\").replace('"', '\\"').replace('\n', '\\n')


parser = OptionParser()
parser.add_option("--xls", action="store",
                  dest="xls", type="string")
parser.add_option("--lua", action="store",
                  dest="lua", type="string")
(opts, args) = parser.parse_args()

if __name__ == '__main__':
    workbook = xlrd.open_workbook(opts.xls)
    sheet = workbook.sheet_by_index(0)
    data = []
    for row in range(sheet.nrows):
        row_data = []
        for col in range(sheet.ncols):
            cell = sheet.cell(row, col)
            # 判断单元格类型
            if cell.ctype == xlrd.XL_CELL_NUMBER and cell.value.is_integer():
                value = str(int(cell.value))
            else:
                value = str(cell.value)
            value = escapeForLua(value)
            row_data.append(value)
        data.append(row_data)
    comment = []
    for i in reversed(range(len(data))):
        start = data[i][0]
        if start and "//" in start:
            comment.insert(0, data.pop(i))

    with open(opts.lua, 'w', encoding='utf-8') as f:
        f.write("return {\n")
        f.write("    comment = {\n")
        for i, row in enumerate(comment):
            f.write(f"      [{i+1}] = {{ \n")
            for j, cell in enumerate(row):
                if is_number(cell):
                    f.write(f'            [{j+1}] = {cell},\n')
                else:
                    f.write(f'            [{j+1}] = "{cell}",\n')
            f.write("        },\n")

        f.write("    },\n")
        f.write("    data = {\n")
        for i, row in enumerate(data):
            f.write(f"       [{i+1}] = {{\n")
            for j, cell in enumerate(row):
                if is_number(cell):
                    f.write(f'            [{j+1}] = {cell},\n')
                else:
                    f.write(f'            [{j+1}] = "{cell}",\n')
            f.write("        },\n")
        f.write("    },\n")
        f.write("}\n")
    print(f"表 {opts.xls} 全部数据转为 lua")
