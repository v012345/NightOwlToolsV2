try:
    import pytesseract
except:
    import subprocess
    import sys
    subprocess.Popen([sys.executable, "-m", "pip", "install", "pytesseract"],
                     creationflags=subprocess.CREATE_NEW_CONSOLE).wait()
    import pytesseract

from PIL import Image
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--image", type=str, required=True, help="Path to image")
parser.add_argument("--text", type=str, required=True, help="Txt ouput to")
parser.add_argument("--tesseract", type=str, required=True, help="Path to tesseract.exe")
args = parser.parse_args()
pytesseract.pytesseract.tesseract_cmd = args.tesseract
img = Image.open(args.image)
print("识别中...")
text = pytesseract.image_to_string(img, lang="chi_sim+jpn")  # 中+日
with open(args.text, 'w', encoding='utf-8') as f_out:
    f_out.write(text)
print("完成...")

