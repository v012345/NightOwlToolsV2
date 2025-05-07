import subprocess
import sys

# 定义一个函数来动态安装库
def install_and_import(package):
    try:
        __import__(package)
    except ImportError:
        print(f"{package} 未安装，正在安装...")
        subprocess.check_call([sys.executable, "-m", "pip", "install", package])
        print(f"{package} 安装完成。")
        __import__(package)

# 动态安装 pandas、openpyxl 和 xlwt
install_and_import('pandas')
install_and_import('openpyxl')
install_and_import('xlwt')
install_and_import('xlrd')

# 导入已安装的库
import pandas as pd
import openpyxl
import xlwt
import csv

# 转换 CSV 到 XLSX
def csv_to_xlsx(csv_file, xlsx_file):
    df = pd.read_csv(csv_file)
    df.to_excel(xlsx_file, index=False, engine='openpyxl')
    print(f"CSV 文件 {csv_file} 已转换为 XLSX 文件 {xlsx_file}")

# 转换 XLSX 到 CSV
def xlsx_to_csv(xlsx_file, csv_file):
    df = pd.read_excel(xlsx_file, engine='openpyxl')
    df.to_csv(csv_file, index=False)
    print(f"XLSX 文件 {xlsx_file} 已转换为 CSV 文件 {csv_file}")

# 转换 CSV 到 XLS (不使用 pandas)
def csv_to_xls(csv_file, xls_file):
    workbook = xlwt.Workbook()
    sheet = workbook.add_sheet('Sheet1')

    with open(csv_file, 'r', newline='', encoding='utf-8') as f:
        reader = csv.reader(f)
        for row_idx, row in enumerate(reader):
            for col_idx, value in enumerate(row):
                sheet.write(row_idx, col_idx, value)
    
    workbook.save(xls_file)
    print(f"CSV 文件 {csv_file} 已转换为 XLS 文件 {xls_file}")

# 转换 XLS 到 CSV
def xls_to_csv(xls_file, csv_file):
    df = pd.read_excel(xls_file, engine='xlrd')
    df.to_csv(csv_file, index=False)
    print(f"XLS 文件 {xls_file} 已转换为 CSV 文件 {csv_file}")

# 转换 XLSX 到 XLS（不使用 pandas 的写入功能）
def xlsx_to_xls(xlsx_file, xls_file):
    # 读取 XLSX 文件
    df = pd.read_excel(xlsx_file, engine='openpyxl')
    
    # 创建一个新的 .xls 文件
    workbook = xlwt.Workbook()
    sheet = workbook.add_sheet('Sheet1')
    
    # 将 DataFrame 中的内容逐行逐列写入 .xls 文件
    for row_idx, row in df.iterrows():
        for col_idx, value in enumerate(row):
            sheet.write(row_idx, col_idx, value)
    
    # 保存 .xls 文件
    workbook.save(xls_file)
    print(f"XLSX 文件 {xlsx_file} 已转换为 XLS 文件 {xls_file}")

# 转换 XLS 到 XLSX
def xls_to_xlsx(xls_file, xlsx_file):
    df = pd.read_excel(xls_file, engine='xlrd')
    df.to_excel(xlsx_file, index=False, engine='openpyxl')
    print(f"XLS 文件 {xls_file} 已转换为 XLSX 文件 {xlsx_file}")

# 示例用法
if __name__ == "__main__":
    csv_to_xlsx('text_ui.csv', 'output1.xlsx')
    xlsx_to_csv('text_ui.xlsx', 'output2.csv')
    csv_to_xls('text_ui.csv', 'output3.xls')
    xls_to_csv('text_ui.xls', 'output4.csv')
    xlsx_to_xls('text_ui.xlsx', 'output5.xls')
    xls_to_xlsx('text_ui.xls', 'output6.xlsx')