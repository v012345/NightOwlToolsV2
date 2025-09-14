

try:
    from PIL import Image, ImageDraw, ImageFont
except:
    import subprocess
    import sys
    subprocess.Popen([sys.executable, "-m", "pip", "install", "pillow"],
                     creationflags=subprocess.CREATE_NEW_CONSOLE).wait()
    from PIL import Image, ImageDraw, ImageFont
import os

# 输出目录
output_dir = "temp"
os.makedirs(output_dir, exist_ok=True)

# 图片尺寸
size = (64, 64)

# 字体大小
font_size = 36

try:
    # Windows 下可用 Arial，Linux/Mac 可能要换成 DejaVuSans.ttf
    font = ImageFont.truetype("arial.ttf", font_size)
except IOError:
    font = ImageFont.load_default()

# 生成 0~9 的 tile
for num in range(27):
    img = Image.new("RGB", size, (20, 200, 20))  # 绿色背景
    draw = ImageDraw.Draw(img)

    text = str(num)

    # 用 textbbox 计算文本大小
    bbox = draw.textbbox((0, 0), text, font=font)
    text_width = bbox[2] - bbox[0]
    text_height = bbox[3] - bbox[1]

    # 居中
    x = (size[0] - text_width) / 2
    y = (size[1] - text_height) / 3

    draw.text((x, y), text, font=font, fill=(255, 255, 255))

    filename = os.path.join(output_dir, f"tile_{num}.png")
    img.save(filename)

print("生成完成，图片保存在 tiles 文件夹里")


# 小图目录
input_dir = "temp"
# 输出大图
output_file = "temp/tilesheet.png"

# 参数：小图尺寸
tile_size = (64, 64)
# 拼图布局
cols = 6  # 列数
rows = 5 # 行数 (根据你的小图数量调整)

# 计算大图尺寸
sheet_width = cols * tile_size[0]
sheet_height = rows * tile_size[1]

# 创建空白大图（黑底可改成透明: "RGBA", (0,0,0,0)）
sheet = Image.new("RGBA", (sheet_width, sheet_height), (0, 0, 0,0))

# 读取 tiles 文件夹里排序后的图片
images = sorted([f for f in os.listdir(input_dir) if f.endswith(".png")])

for idx, filename in enumerate(images):
    img = Image.open(os.path.join(input_dir, filename))

    # 计算贴图位置
    x = (idx % cols) * tile_size[0]
    y = (idx // cols) * tile_size[1]

    sheet.paste(img, (x, y))

# 保存大图
sheet.save(output_file)
print(f"拼图完成，保存为 {output_file}")