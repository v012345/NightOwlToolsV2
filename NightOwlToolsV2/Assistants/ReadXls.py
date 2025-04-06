import pandas as pd

# 读取指定 sheet 页并处理表头和 id 列
def read_xls_with_id(file, sheet_name):
    df = pd.read_excel(file, sheet_name=sheet_name)
    print(df)
    return df

def append_xls_with_id(file, sheet_name, new_data):
    df = pd.read_excel(file, sheet_name=sheet_name)

    # 检查 id 列中是否有重复
    if new_data['id'] in df['id'].values:
        print(f"ID {new_data['id']} 已存在，无法添加")
        return

    # 添加新数据并保存
    df = df.append(new_data, ignore_index=True)
    with pd.ExcelWriter(file, engine='openpyxl', mode='a', if_sheet_exists='replace') as writer:
        df.to_excel(writer, sheet_name=sheet_name, index=False)
    print(f"新数据已添加到文件 {file} 的 sheet 页 {sheet_name}")

read_xls_with_id("text_name.xls","Sheet1")
append_xls_with_id("text_name.xls","Sheet1",{'id':106184,"ok":"1","en":"1","zhcn":"1","zhtw":"1"})