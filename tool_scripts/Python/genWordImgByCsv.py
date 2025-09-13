try:
    from PIL import Image, ImageDraw, ImageFont
except:
    import subprocess
    import sys
    subprocess.Popen([sys.executable, "-m", "pip", "install", "pillow"],
                     creationflags=subprocess.CREATE_NEW_CONSOLE).wait()
    from PIL import Image, ImageDraw, ImageFont
import os
import textwrap
import csv
from dataclasses import dataclass

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--assets_path", type=str, required=True, help="Path to assets folder")
args = parser.parse_args()

@dataclass
class Size:
    width: int
    height: int
size = Size(140, 190)
max_width = size.width - 20

font = ImageFont.truetype("单词游戏/BIZ-UDGothicR.ttc", 24) # 字体 与 大小

kana_index = 0
image_path_index = 0
with open("单词游戏/words.csv", 'r', encoding='utf_8_sig') as file:
    csv_reader = csv.reader(file)
    rows = list(csv_reader)   # 转为二维数组（list of lists）
    data = rows[3:]
    for idx, x in enumerate(rows[1]):
        if x == "kana":
            kana_index = idx
        if x == "image_path":
            image_path_index = idx

for row_num, row in enumerate(data):
    # 使用 RGBA 模式只是为了兼容 draw API；尺寸稍后重建
    tmp_img = Image.new("RGBA", (500, 500), (255, 255, 255, 0))
    draw = ImageDraw.Draw(tmp_img)
    text = str(row[kana_index])
    lines = [] # 用来把行分好
    current = ""
    for char in text:
        if draw.textlength(current + char, font=font) <= max_width:
            current += char
        else:
            lines.append(current)
            current = char
    if current:
        lines.append(current)


    line_height = font.getbbox("A")[3]  # 字体高度
    line_nun = len(lines)

    # 计算大小
    img_h = 0
    img_w = 0
    for i, line in enumerate(lines):
        bbox = draw.textbbox((0, 0), line, font=font)
        text_width = bbox[2] - bbox[0]
        img_w = max(img_w, text_width)
        x = ( size.width - text_width) / 2
        img_h = (line_height + 5) * (i + 1)  # 行间距
    img_h = img_h - 5

    # 正式生成图片
    img = Image.new("RGBA", (img_w, img_h), (255, 255, 255, 0))
    draw = ImageDraw.Draw(img)
    for i, line in enumerate(lines):
        bbox = draw.textbbox((0, 0), line, font=font)
        text_width = bbox[2] - bbox[0]
        x = (img_w - text_width) / 2
        y = (line_height + 5) * i  # 行间距
        draw.text((x, y), line, font=font, fill=(0, 0, 0))
    filename = os.path.join(args.assets_path, row[image_path_index][6:])
    img.save(filename)
    print(f"{row_num + 1} / {len(data)}", end = "\r" )
print()
print("完成")

