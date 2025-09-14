try:
    from pdf2image import convert_from_path
except:
    import subprocess
    import sys
    subprocess.Popen([sys.executable, "-m", "pip", "install", "pdf2image"],
                     creationflags=subprocess.CREATE_NEW_CONSOLE).wait()
    from pdf2image import convert_from_path
print(0)
pages = convert_from_path(
    r"C:\Users\MH\Downloads\123.pdf",
    # r"C:\Users\MH\Desktop\123.pdf",
    dpi=300,
    poppler_path=r"C:\Users\MH\Downloads\Release-25.07.0-0\poppler-25.07.0\Library\bin"
)
print(1)
for i, page in enumerate(pages):
    print(i)
    page.save(f"temp/page_{i+1}.png", "PNG")