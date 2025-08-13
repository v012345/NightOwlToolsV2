
import xlrd
from optparse import OptionParser


def is_number(s):
    if "_" in s:
        return False
    if "+" in s:
        return False
    if "-" in s:
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
parser.add_option("--encoding", action="store",
                  dest="encoding", type="string")
parser.add_option("--name", action="store",
                  dest="name", type="string")
(opts, args) = parser.parse_args()


def escapeForLua(s: str):
    return s.replace('\r', '').replace("\\", "\\\\").replace('"', '\\"').replace('\n', '\\n')


def convert_to_lua_data(input_str):
    if is_number(input_str):
        try:
            return int(input_str)
        except ValueError:
            return float(input_str)
    else:
        return f'"{input_str}"'


def get_d(input_str):
    if "^" in input_str:
        return 2
    elif "|" in input_str:
        return 1
    else:
        return 0


def convert_to_lua(input_str, d, dep, name, key_name):
    if name == "cfg_LuckyEvent_BoxData" and key_name == "BuffId":
        if input_str == "nil":
            dep = True
        else:
            dep = False
            d = 1
    if name == "cfg_renwu_target" and key_name == "TouchType":
        dep = False
        d = 1
    if name == "cfg_renwu_target" and key_name == "Param":
        dep = False
        d = 1
    if name == "cfg_renwu_touch" and key_name == "Param":
        dep = False
        d = 1
    if name == "cfg_renwu" and key_name == "Target":
        dep = False
        d = 1

    if dep:
        if "^" in input_str:
            d = 2
        elif "|" in input_str:
            d = 1
        else:
            return f'{convert_to_lua_data(input_str)},'
        if d == 1:
            items = input_str.split('|')
            lua_lines = ["{\n"]
            for i, item in enumerate(items, start=1):
                line = f'\t\t\t[{i}] = {convert_to_lua_data(item)},\n'
                lua_lines.append(line)
            lua_lines.append("\t\t},")
            return ''.join(lua_lines)
        elif d == 2:
            items = input_str.split('|')
            lua_lines = ["{\n"]
            for i, item in enumerate(items, start=1):
                if "^" in item:
                    line = f'\t\t\t[{i}] = {{\n'
                    lua_lines.append(line)
                    sub_items = item.split("^")
                    for j, sub_item in enumerate(sub_items, start=1):
                        line = f'\t\t\t\t[{j}] = {convert_to_lua_data(sub_item)},\n'
                        lua_lines.append(line)
                    lua_lines.append("\t\t\t},\n")
                else:
                    line = f'\t\t\t[{i}] = {convert_to_lua_data(item)},\n'
                    lua_lines.append(line)
            lua_lines.append("\t\t},")
            return ''.join(lua_lines)
    else:
        if d == 0:
            return f'{convert_to_lua_data(input_str)},'
        elif d == 1:
            items = input_str.split('|')
            lua_lines = ["{\n"]
            for i, item in enumerate(items, start=1):
                line = f'\t\t\t[{i}] = {convert_to_lua_data(item)},\n'
                lua_lines.append(line)
            lua_lines.append("\t\t},")
            return ''.join(lua_lines)
        elif d == 2:
            items = input_str.split('|')
            lua_lines = ["{\n"]
            for i, item in enumerate(items, start=1):
                if "^" in item:
                    line = f'\t\t\t[{i}] = {{\n'
                    lua_lines.append(line)
                    sub_items = item.split("^")
                    for j, sub_item in enumerate(sub_items, start=1):
                        line = f'\t\t\t\t[{j}] = {convert_to_lua_data(sub_item)},\n'
                        lua_lines.append(line)
                    lua_lines.append("\t\t\t},\n")
                else:
                    line = f'\t\t\t[{i}] = {convert_to_lua_data(item)},\n'
                    lua_lines.append(line)
            lua_lines.append("\t\t},")
            return ''.join(lua_lines)
        else:
            ValueError("不支持")


special = {
    "cfg_CuiPanGuan": True,
    # "cfg_LuckyEvent_BoxData": True,
    "cfg_renwu_target": True,
    "cfg_renwu_touch": True,
    "cfg_renwu": True,
    "cfg_SetZhuangBan": True,
    "cfg_Task": True,
    "cfg_TitelLookData": True,
    "cfg_TitleNumData": True,
    "cfg_WuXingLingTi": True,
    "cfg_YanWangDaDian": True,
    "cfg_YiYeYiPuTi": True,
    "cfg_YouXiGongLve": True,
    "cfg_ZhuangBan": True,
    "cfg_ZhuangBeiBuff": True,
}

if __name__ == '__main__':
    workbook = xlrd.open_workbook(opts.xls)
    sheet = workbook.sheet_by_index(0)
    num_rows = sheet.nrows
    num_cols = sheet.ncols
    d = []
    head = []
    for col in range(num_cols):
        head.append(sheet.cell_value(0, col))
        d.append(0)

    xls = []
    for row in range(1, num_rows):
        row_data = []
        for col in range(num_cols):
            cell = sheet.cell(row, col)
            # 判断单元格类型
            if cell.ctype == xlrd.XL_CELL_NUMBER and cell.value.is_integer():
                value = str(int(cell.value))  # 转成整数字符串
            else:
                value = str(cell.value)   # 直接输出文本
            value = escapeForLua(value)
            dimensions = get_d(value)
            if dimensions > d[col]:
                d[col] = dimensions
            row_data.append(value)
        xls.append(row_data)

    with open(opts.lua, 'w', encoding=opts.encoding) as f:
        f.write("local config = { \n")
        for i, row in enumerate(xls):
            if is_number(row[0]) and opts.name != "cfg_ZhuangBeiBuff":
                f.write(f"\t[{row[0]}] = {{ \n")
            else:
                f.write(f'\t["{row[0]}"] = {{ \n')
            for j, cell in enumerate(row):
                lua_key = head[j]
                if cell != "":
                    f.write(
                        f'\t\t{lua_key} = {convert_to_lua(cell, d[j], not special.get(opts.name), opts.name, lua_key)}\n')
            f.write("\t},\n")
        f.write("}\n")
        f.write("return config\n")
