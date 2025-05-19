
import importlib
import os
import sys

try:
    import xlrd
except:
    import subprocess
    subprocess.Popen([sys.executable, "-m", "pip", "install", "xlrd"],
                     creationflags=subprocess.CREATE_NEW_CONSOLE).wait()
    import xlrd
from optparse import OptionParser


def is_number(s):
    if "_" in s:
        return False
    try:
        float(s)  # 尝试转为浮点数
        return True
    except ValueError:
        return False


parser = OptionParser()
parser.add_option("--xls", action="store",
                  dest="xls", type="string")
parser.add_option("--lua", action="store",
                  dest="lua", type="string")
parser.add_option("--head", action="store",
                  dest="head", type="string")
parser.add_option("--name", action="store",
                  dest="name", type="string")
(opts, args) = parser.parse_args()


def escapeForLua(s: str):
    return s.replace('\r', '').replace(
        "\\", "\\\\").replace('"', '\\"').replace('\n', '\\n')


if __name__ == '__main__':
    print(f"处理表 : {opts.xls}")
    workbook = xlrd.open_workbook(opts.xls)
    sheet = workbook.sheet_by_index(0)
    num_rows = sheet.nrows
    num_cols = sheet.ncols
    xls = []
    for row in range(num_rows):
        is_comment = False
        row_data = []
        for col in range(num_cols):
            cell = sheet.cell(row, col)
            # 判断单元格类型
            if cell.ctype == xlrd.XL_CELL_NUMBER and cell.value.is_integer():
                value = str(int(cell.value))  # 转成整数字符串
            else:
                value = str(cell.value)   # 直接输出文本

            if col == 0 and "//" in value:
                is_comment = True
                break
            value = escapeForLua(value)
            row_data.append(value)
        if is_comment:
            is_comment = False
        else:
            xls.append(row_data)
    sys.path.append(os.getcwd())
    module = importlib.import_module(opts.head)
    cfg = getattr(module, opts.name)

    head = cfg["head"]
    key = cfg["key"]
    key_index = cfg["key_index"]

    with open(opts.lua, 'w', encoding='utf-8') as f:
        f.write("local config = { \n")
        for i, row in enumerate(xls):
            if key_index < 0:
                f.write(f"\t[{i+1}] = {{ \n")
            else:
                if is_number(row[key_index]):
                    f.write(f"\t[{row[key_index]}] = {{ \n")
                else:
                    f.write(f'\t["{row[key_index]}"] = {{ \n')
            for j, cell in enumerate(row):
                if cell != "":
                    lua_key = head[j]
                    if is_number(cell):
                        f.write(f'\t\t{lua_key} = {cell},\n')
                    else:
                        f.write(f'\t\t{lua_key} = "{cell}",\n')
            f.write("\t},\n")
        f.write("}\n")
        f.write("return config\n")
